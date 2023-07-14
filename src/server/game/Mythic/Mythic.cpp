#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "Item.h"
#include "Mythic.h"
#include "MythicManager.h"

enum MYTHIC_SPELLS {
    HEALTH_AND_DAMAGE_SPELL = 0,
};

Mythic::Mythic(Map* map, Group* group, uint32 dungeonId, uint32 level, Player* leader)
{
    EnemyForces = 0.f;
    DungeonId = dungeonId;
    Dungeon = map;
    TimeToComplete = 0; // Get the time to complete.
    StartTimer = 0;
    Started = false;
    ChestDecrapeted = false;
    Done = false;
    ElapsedTime = 0;
    Deaths = 0;
    Level = level;
    m_Group = group;
    Leader = leader;
}

Mythic::~Mythic()
{

}

void Mythic::Prepare()
{
    Dungeon->EnableMythic(this);

    auto const& allyList = m_Group->GetMemberSlots();

    for (auto const& target : allyList)
    {
        Player* member = ObjectAccessor::FindPlayer(target.guid);
        if (member) {
            member->ClearUnitState(UNIT_STATE_ROOT);
            member->SetControlled(true, UNIT_STATE_ROOT);
            member->SetDungeonDifficulty(DUNGEON_DIFFICULTY_EPIC_PLUS);
            // member->TeleportTo(mapId, dungeon.x, dungeon.y, dungeon.z, dungeon.o, 0, nullptr, true);
        }
    }
}

void Mythic::Update(uint32 diff)
{
    if (IsDungeonDone())
        return;

    if (IsDungeonNotStarted()) {
        Map::PlayerList const& playerList = Dungeon->GetPlayers();

        if (playerList.IsEmpty())
            return;

        StartTimer -= diff;

        if (StartTimer <= 0) {
            StartTimer = 0;
            Started = true;
            for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration) {
                if (Player* player = playerIteration->GetSource()) {
                    player->ClearUnitState(UNIT_STATE_ROOT);
                    player->SetControlled(false, UNIT_STATE_ROOT);
                    // Send Addon Start
                    // SendStart(player);
                }
            }
        }
    }
    else {
        ElapsedTime += diff;
        if (ElapsedTime >= TimeToComplete && ChestDecrapeted && !Done) {
            ChestDecrapeted = true;
            Map::PlayerList const& playerList = Dungeon->GetPlayers();
            if (!playerList.IsEmpty())
                for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
                    if (Player* player = playerIteration->GetSource()) {
                        // Send Addon Chest Decapreted;
                    }
        }
    }
}

void Mythic::PrepareCreature(Creature* creature)
{
    if (creature->IsPet() || creature->IsControlledByPlayer())
        return;

    if (!creature->HasAura(HEALTH_AND_DAMAGE_SPELL)) {
        MythicMultiplier multiplier = sMythicMgr->GetMultplierByLevel(Level);
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

void Mythic::OnCompleteMythicDungeon(Player* player)
{
    // Send Addon Mythic Completion;
    GiveRewards();
    SaveMythicDungeon();
    UpdatePlayerKey(Leader);
    sMythicMgr->RemoveGroup(m_Group);
}

void Mythic::OnKillBoss(Player* player, Creature* killed)
{
    if (Done && !Started)
        return;

    if (!MeetTheConditionsToCompleteTheDungeon())
        return;

    OnCompleteMythicDungeon(player);
}

void Mythic::OnKillCreature(Player* player, Creature* killed)
{
    if ((EnemyForces >= 100 || Done) && !Started)
        return;

    uint8 count = sMythicMgr->GetKillCountByCreatureId(killed->GetEntry());

    EnemyForces += count;

    Map::PlayerList const& playerList = Dungeon->GetPlayers();

    if (playerList.IsEmpty())
        return;

    for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration) {
        if (Player* player = playerIteration->GetSource()) {
            // Send Update Enemy Forces;
            // Send Addon
        }
    }

    if (!MeetTheConditionsToCompleteTheDungeon())
        return;

    OnCompleteMythicDungeon(player);
}

void Mythic::OnPlayerKilledByCreature()
{
    Deaths += 1;

    uint32 timeToReduce = 0;

    Map::PlayerList const& playerList = Dungeon->GetPlayers();

    if (playerList.IsEmpty())
        return;

    for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration) {
        if (Player* player = playerIteration->GetSource()) {
            // Send Addon;
        }
    }
}

void Mythic::OnPlayerRelease()
{

}

/*/std::list<Group::MemberSlot> Mythic::GetRandomMemberSlot()
{

    std::list<Group::MemberSlot> allyList = {};

    std::random_device rd;
    std::mt19937 rng(rd());

    auto temp = m_Group->GetMemberSlots();

    std::shuffle(temp.begin(), temp.end(), rng);

    allyList.push_back(temp.front());
    allyList.push_back(temp.back());
}*/

bool Mythic::MeetTheConditionsToCompleteTheDungeon()
{
    bool allBossesAreDead = true;

    for (auto ij = StateBossMythicStore.begin(); ij != StateBossMythicStore.end(); ++ij)
        if (ij->second == false)
            allBossesAreDead = false;

    return allBossesAreDead && EnemyForces >= 100.0f;
}

void Mythic::GiveRewards()
{
    // We always garantee 2 loots;
    /*std::list<Group::MemberSlot> members = GetRandomMemberSlot();

    for (auto const& target : members)
    {
        Player* member = ObjectAccessor::FindPlayer(target.guid);
        if (member) {
            uint32 itemId = sMythicMgr->GetRandomLoot(member, DungeonId, Level);
            member->AddItem(itemId, 1);
        }
    }*/
}

void Mythic::UpdatePlayerKey(Player* player)
{
    if (ChestDecrapeted)
        return;

    uint8 upgrade = 1;

    uint32 difference = TimeToComplete - ElapsedTime;
    float pourcentage = (difference / ElapsedTime) * 100;

    if (pourcentage >= 65.0f)
        upgrade = 3;
    if (pourcentage < 65.0f && pourcentage > 45.0f)
        upgrade = 2;

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
