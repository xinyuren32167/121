#include "MythicManager.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Config.h"

MythicManager::~MythicManager()
{
}

MythicManager* MythicManager::instance()
{
    static MythicManager instance;
    return &instance;
}

void MythicManager::InitializeMythicDungeonBosses()
{

}

void MythicManager::InitializeMythicDungeons()
{

}

void MythicManager::InitializePlayerMythicKeys()
{

}

void MythicManager::InitializeRewardsDungeons()
{

}

void MythicManager::InitializeRewardsPlayersBag()
{

}

void MythicManager::InitializeCreatureKillingCount()
{

}

void MythicManager::InitializeMultipliers()
{

}

void MythicManager::HandleChangeDungeonDifficulty(Player* _player, uint8 mode)
{
    Group* group = _player->GetGroup();
    if (group)
    {
        if (group->IsLeader(_player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
            {
                Player* groupGuy = itr->GetSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInWorld())
                {
                    _player->SendDungeonDifficulty(group != nullptr);
                    return;
                }

                if (groupGuy->GetGUID() == _player->GetGUID() ? groupGuy->GetMap()->IsDungeon() : groupGuy->GetMap()->IsNonRaidDungeon())
                {
                    _player->SendDungeonDifficulty(group != nullptr);
                    return;
                }
            }

            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false, _player);
            group->SetDungeonDifficulty(Difficulty(mode));
        }
    }
    else {
        _player->SetDungeonDifficulty(Difficulty(mode));
        _player->SendDungeonDifficulty(false, DUNGEON_DIFFICULTY_EPIC);
        Player::ResetInstances(_player->GetGUID(), INSTANCE_RESET_CHANGE_DIFFICULTY, false);
    }
}

void MythicManager::SaveMythicKey(Player* player)
{

}

void MythicManager::Update(uint32 diff)
{
    for (auto mythic : MythicStore)
        mythic.second->Update(diff);
}

void MythicManager::AddMythicDungeon(ObjectGuid guid, Mythic* m)
{
    MythicStore[guid] = m;
}

void MythicManager::RemoveGroup(Group* group)
{
    MythicStore.erase(group->GetGUID());
}

void MythicManager::OnGroupDisband(Group* group)
{
    RemoveGroup(group);
}

bool MythicManager::IsThisMapIdAvailableForMythic(uint32 mapId)
{
    return false;
}

bool MythicManager::IsPlayerMeetingConditionsToStartMythic(Player* player)
{
    return false;
}

uint32 MythicManager::GetRandomMythicDungeonForPlayer(Player* player)
{
    if (MythicDungeonStore.empty())
        return 0;

    return MythicDungeonStore[urand(0, MythicDungeonStore.size() - 1)].id;
}

uint32 MythicManager::GetRandomLoot(Player* player, uint32 dungeonId, uint32 level)
{
    std::vector<MythicReward> loots = {};

    auto itr = MythicDungeonLootStore.find(dungeonId);
    if (itr != MythicDungeonLootStore.end())
        for (auto item : itr->second)
            if ((item.classMask & player->getClassMask() || item.classMask == -1) && item.level == level)
                loots.push_back(item);


    if (loots.empty())
        return 0;

    uint32 rand = urand(0, loots.size() - 1);

    return loots[rand].itemId;
}

uint32 MythicManager::GetItemIdWithDungeonId(uint32 dungeonId)
{
    return uint32();
}

uint32 MythicManager::GetEnchantByMythicLevel(uint32 level)
{
    return level + 100000;
}


Mythic* MythicManager::GetMythicDungeonByGroupGuid(ObjectGuid groupGuid)
{
    auto itr = MythicStore.find(groupGuid);
    if (itr != MythicStore.end())
        return itr->second;

    return nullptr;
}

float MythicManager::GetKillCountByCreatureId(uint32 creatureId)
{
    float count = 1.f;
    auto itr = MythicKillCounterStore.find(creatureId);
    if (itr != MythicKillCounterStore.end())
        count = itr->second;

    return count;
}

MythicMultiplier MythicManager::GetMultplierByLevel(uint32 level)
{
    MythicMultiplier multiplier = { 1.f, 1.f };
    auto itr = MythicMultiplierStore.find(level);
    if (itr != MythicMultiplierStore.end())
        multiplier = itr->second;

    return multiplier;
}


void MythicManager::FindMythicDungeonByItsKeyItemId(uint32 itemId, MythicDungeon& dungeon)
{
    for (auto const& dg : MythicDungeonStore)
        if (dg.itemId == itemId)
            dungeon = dg;
}

void MythicManager::GetMythicDungeonByDungeonId(uint32 dungeonId, MythicDungeon& dungeon)
{
    for (auto const& dg : MythicDungeonStore)
        if (dg.id == dungeonId)
            dungeon = dg;
}

MythicKey* MythicManager::GetCurrentPlayerMythicKey(Player* player)
{
    auto itr = MythicPlayerKeyStore.find(player->GetGUID());
    if (itr != MythicPlayerKeyStore.end()) {
        return &itr->second;
    }

    return nullptr;
}

std::map<uint32, bool> MythicManager::GetMythicBossesByDungeonId(uint32 dungeonId)
{
    std::map<uint32, bool> map = {};
    auto itr = MythicDungeonBossStore.find(dungeonId);
    for (auto const& boss : itr->second)
        map[boss.creatureId] = true;

    return map;
}

std::vector<std::string> MythicManager::GetDataMythicRun(Player* player)
{
   Mythic* mythic = player->GetMap()->GetMythic();

    if (!mythic)
        return {};

    std::vector<std::string> elements = {};
    std::string first =
        std::to_string(mythic->GetLevel())
        + ";" + std::to_string(mythic->GetEnemyForces())
        + ";" + std::to_string(mythic->GetTimeToComplete())
        + ";" + std::to_string(mythic->GetElapsedTime())
        + ";" + std::to_string(mythic->GetTotalDeaths())
        + ";" + player->GetMap()->GetMapName();

    elements.push_back(first);

    std::string bossStatus = "";
    for (auto const& boss : mythic->GetBosses()) {
        CreatureTemplate const* creatureTemplate = sObjectMgr->GetCreatureTemplate(boss.first);
        bossStatus += ";" + creatureTemplate->Name + "+" + std::to_string(boss.second);
    }

    elements.push_back(bossStatus);
    return elements;
}

std::vector<std::string> MythicManager::GetDataMythicBag(Item* item)
{
    uint64 guid = item->GetGUID().GetCounter();
    std::vector<std::string> elements = {};

    return elements;
}

uint32 MythicManager::GetDungeonKeyLevelPreperation(Player* player)
{
    MythicKey* mythicKey = GetCurrentPlayerMythicKey(player);

    if (!mythicKey)
        return 0;

    uint32 level = mythicKey->level;

    if (level <= 1)
        return 0;

    return level;
}

void MythicManager::StartMythicDungeon(Player* player)
{
    MythicKey* mythicKey = GetCurrentPlayerMythicKey(player);

    if (!mythicKey)
        return;

    MythicDungeon dungeon;
    GetMythicDungeonByDungeonId(mythicKey->dungeonId, dungeon);

    if(!dungeon)
        return;

    Group* group = player->GetGroup();

    if (group)
        group->SetDungeonDifficulty(DUNGEON_DIFFICULTY_EPIC_PLUS, true);
    else
        player->SetDungeonDifficulty(DUNGEON_DIFFICULTY_EPIC_PLUS);



    /* uint32 maxLevel = 2;

    auto key = m_PlayerKey.find(player->GetGUID().GetCounter());

    if (key != m_PlayerKey.end())
        maxLevel = key->second.level;

    if (level > maxLevel) {
        ChatHandler(player->GetSession()).SendSysMessage("Nice try! You cannot start a dungeon of this level!");
        return;
    }

    Group* group = player->GetGroup();

    m_DelayedCreationRun[player->GetGUID()] = { key->second.dungeonId, level };

    player->ClearUnitState(UNIT_STATE_ROOT);
    player->SetControlled(true, UNIT_STATE_ROOT);
    player->SetDungeonDifficulty(DUNGEON_DIFFICULTY_EPIC_PLUS);

    MythicDungeon dungeon = GetMythicDungeonByDungeonId(key->second.dungeonId);

    if (!dungeon.id) {
        ChatHandler(player->GetSession()).SendSysMessage("This dungeon does not exist!");
        return;
    }

    player->TeleportTo(dungeon.mapId, dungeon.x, dungeon.y, dungeon.z, dungeon.o, 0, nullptr, true);

    if (group)
        group->SetDungeonDifficulty(DUNGEON_DIFFICULTY_EPIC_PLUS, true);
    else
        player->SetDungeonDifficulty(DUNGEON_DIFFICULTY_EPIC_PLUS);*/

}
