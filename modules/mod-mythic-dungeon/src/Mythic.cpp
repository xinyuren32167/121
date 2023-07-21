#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "Item.h"
#include "Mythic.h"
#include "MythicManager.h"
#include "Chat.h"
#include "LuaEngine.h"

enum MYTHIC_SPELLS {
    HEALTH_AND_DAMAGE_SPELL = 0,
};

#define itemCoin = 0;

Mythic::Mythic(Player* keyOwner, uint32 dungeonId, uint32 level)
{
    EnemyForces = 0.f;
    DungeonId = dungeonId;
    Dungeon = keyOwner->GetMap();
    TimeToComplete = sMythicMgr->GetTimeToCompleteByDungeonId(dungeonId);;
    StartTimer = 10 * IN_MILLISECONDS;
    Countdown = 0;
    Iteration = 10;
    Started = false;
    ChestDecrapeted = false;
    Done = false;
    ElapsedTime = 0;
    Deaths = 0;
    Level = level;
    m_Group = keyOwner->GetGroup();
    KeyOwner = keyOwner;
    StateBossMythicStore = sMythicMgr->GetMythicBossesByDungeonId(dungeonId);
}

Mythic::~Mythic()
{

}


void Mythic::Update(uint32 diff)
{
    if (IsDungeonDone())
        return;

    if (IsDungeonNotStarted()) {
        Map::PlayerList const& playerList = Dungeon->GetPlayers();

        if (playerList.IsEmpty())
            return;


        Countdown += diff;
        StartTimer -= diff;

        if (Countdown > 1000) {
            Iteration -= 1;
            Countdown = 0;
            for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration) {
                if (Player* player = playerIteration->GetSource()) {
                    std::string countdownMessage = "Start in " + std::to_string(Iteration) + "...";
                    ChatHandler(player->GetSession()).SendSysMessage(countdownMessage);
                }
            }
        }

        if (StartTimer <= 0) {
            StartTimer = 0;
            Started = true;
            for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration) {
                if (Player* player = playerIteration->GetSource()) {
                    player->ClearUnitState(UNIT_STATE_ROOT);
                    player->SetControlled(false, UNIT_STATE_ROOT);
                    sEluna->SendStartMythicDungeon(player);
                }
            }
        }
    }
    else {
        if(IsAllowedTimeOver())
            ElapsedTime += diff;

        if (ElapsedTime >= TimeToComplete && !ChestDecrapeted && !Done) {
            ChestDecrapeted = true;
            Map::PlayerList const& playerList = Dungeon->GetPlayers();
            if (!playerList.IsEmpty())
                for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
                    if (Player* player = playerIteration->GetSource())
                        sEluna->SendMythicUpdateChestDecrapeted(player);
        }
    }
}


void Mythic::SaveMythicDungeon()
{
    auto const& allyList = m_Group->GetMemberSlots();

    for (auto const& target : allyList)
    {
        uint32 guid = m_Group->GetGUID().GetCounter();
        // Save Mythic Run (Timer, DungeonId, KeyCompletion, Affixes) for everyone in the group.
    }
}

void Mythic::SetBossDead(uint32 creatureId)
{
    for (MythicBossState& boss : StateBossMythicStore)
        if (boss.creatureId == creatureId)
            boss.alive = false;
}

uint32 Mythic::GetBossIndex(uint32 creatureId)
{
    for (MythicBossState& boss : StateBossMythicStore)
        if (boss.creatureId == creatureId)
            return boss.index;

    return 0;
}

void Mythic::OnCompleteMythicDungeon(Player* player)
{
    // Send Addon Mythic Completion;
    Done = true;

    Map::PlayerList const& playerList = Dungeon->GetPlayers();
    for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
        if (Player* currentPlayer = playerIteration->GetSource())
            sEluna->SendCompletedMythicDungeon(currentPlayer, ElapsedTime);

    GiveRewards();
    SaveMythicDungeon();
    UpdatePlayerKey(KeyOwner);
    sMythicMgr->RemoveMythic(player->GetInstanceId());
}

void Mythic::OnKillBoss(Player* player, Creature* killed)
{
    Map::PlayerList const& playerList = Dungeon->GetPlayers();

    if (playerList.IsEmpty())
        return;

    if (IsDungeonDone() || IsDungeonNotStarted())
        return;

    SetBossDead(killed->GetEntry());

    uint32 index = GetBossIndex(killed->GetEntry());

    if (!index)
        return;

    for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
        if (Player* player = playerIteration->GetSource())
            sEluna->SendMythicUpdateBossKill(player, index);

    if (!MeetTheConditionsToCompleteTheDungeon())
        return;

    OnCompleteMythicDungeon(player);
}

void Mythic::OnKillCreature(Player* player, Creature* killed)
{
    if (IsDungeonNotStarted())
        return;

    if (EnemyForces >= 100)
        return;

    if (Done)
        return;

    uint8 count = sMythicMgr->GetKillCountByCreatureId(killed->GetEntry());

    count = killed->isElite() ? count * 2 : count;

    if ((EnemyForces + count) > 100)
        EnemyForces = 100;
    else
        EnemyForces += count;

    Map::PlayerList const& playerList = Dungeon->GetPlayers();

    if (playerList.IsEmpty())
        return;

    for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
        if (Player* player = playerIteration->GetSource())
            sEluna->SendMythicUpdateEnemyForces(player, EnemyForces);

    if (!MeetTheConditionsToCompleteTheDungeon())
        return;

    OnCompleteMythicDungeon(player);
}

void Mythic::OnPlayerKilledByCreature()
{
    Deaths += 1;

    uint32 timeToReduce = 5000;

    ElapsedTime += timeToReduce;

    Map::PlayerList const& playerList = Dungeon->GetPlayers();

    if (playerList.IsEmpty())
        return;

    for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
        if (Player* player = playerIteration->GetSource()) 
            sEluna->SendMythicUpdateDeath(player, Deaths);
}

void Mythic::OnPlayerRelease()
{

}

bool Mythic::MeetTheConditionsToCompleteTheDungeon()
{
    bool allBossesAreDead = true;

    for (auto ij = StateBossMythicStore.begin(); ij != StateBossMythicStore.end(); ++ij)
        if (ij->alive == false)
            allBossesAreDead = false;

    return allBossesAreDead && EnemyForces >= 100.0f;
}

void Mythic::GiveRewards()
{

    bool isAlone = !!m_Group;

    // We always garantee 2 loots;
    // std::list<Group::MemberSlot> members = GetRandomMemberSlot();

    /*for (auto const& target : members)
    {
        Player* member = ObjectAccessor::FindPlayer(target.guid);
        if (member) {
            Item* item = member->AddItem(100000);
            uint8 upgrade = CalculateUpgradeKey();
        }
    }*/
}

void Mythic::UpdatePlayerKey(Player* player)
{
    if (ChestDecrapeted)
        return;

    uint8 upgrade = CalculateUpgradeKey();

    MythicKey* key = sMythicMgr->GetCurrentPlayerMythicKey(player);

    if (!key)
        return;

    uint32 currentItemId = sMythicMgr->GetItemIdWithDungeonId(key->dungeonId);
    player->DestroyItemCount(currentItemId, 1, true);

    uint32 dungeonId = sMythicMgr->GetRandomMythicDungeonForPlayer(player);

    key->dungeonId = dungeonId;
    key->level += upgrade;

    uint32 newItemId = sMythicMgr->GetItemIdWithDungeonId(dungeonId);
    player->AddItem(newItemId, 1);

    Item* item = player->GetItemByEntry(newItemId);

    if (item) {
        uint32 enchantId = sMythicMgr->GetEnchantByMythicLevel(key->level);
        item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantId, 0, 0, player->GetGUID());
    }

    sMythicMgr->SaveMythicKey(player);
}

void Mythic::SendStart(Player* player)
{

}

void Mythic::SendUpdateBossKill()
{

}

void Mythic::SendEnemyForces()
{

}

int8 Mythic::CalculateUpgradeKey()
{
    uint8 upgrade = 1;

    uint32 difference = TimeToComplete - ElapsedTime;
    float pourcentage = (difference / ElapsedTime) * 100;

    if (pourcentage >= 65.0f)
        upgrade = 3;
    if (pourcentage < 65.0f && pourcentage > 45.0f)
        upgrade = 2;


    return upgrade;
}
