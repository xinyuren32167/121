#include "MythicManager.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Config.h"
#include "Player.h"
#include "LuaEngine.h"

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
    MythicDungeonBossStore = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic_bosses ORDER by `order` ASC");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        uint32 order = fields[1].Get<uint32>();
        uint32 bossId = fields[2].Get<uint32>();
        MythicBoss boss = { mapId, order, bossId };
        MythicDungeonBossStore[mapId].push_back(boss);
    } while (result->NextRow());
}

void MythicManager::InitializeMythicDungeons()
{
    MythicDungeonStore = {};

    QueryResult result = WorldDatabase.Query("SELECT id, timeToComplete, mapId, `name`, position_x, position_y, position_z, orientation, itemId FROM dungeon_mythic WHERE `enable` = 1");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 id = fields[0].Get<uint32>();
        uint32 timeToComplete = fields[1].Get<uint32>();
        uint32 mapId = fields[2].Get<uint32>();
        std::string name = fields[3].Get<std::string>();
        float x = fields[4].Get<float>();
        float y = fields[5].Get<float>();
        float z = fields[6].Get<float>();
        float o = fields[7].Get<float>();
        uint32 itemId = fields[8].Get<uint32>();
        MythicDungeon dungeon = { id, timeToComplete, mapId, x, y, z, o, itemId };
        MythicDungeonStore.push_back(dungeon);
    } while (result->NextRow());
}

void MythicManager::InitializePlayerMythicKeys()
{
    MythicPlayerKeyStore = {};
    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_mythic_key");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint64 guid = fields[0].Get<uint64>();
        uint32 dungeonId = fields[1].Get<uint32>();
        uint32 level = fields[2].Get<uint32>();
        MythicKey key = { dungeonId, level };
        MythicPlayerKeyStore[guid] = key;
    } while (result->NextRow());
}

void MythicManager::InitializeRewardsDungeons()
{
    MythicDungeonLootStore = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic_rewards");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        uint32 itemId = fields[1].Get<uint32>();
        uint32 level = fields[2].Get<uint32>();
        int32 classMask = fields[3].Get<int32>();
        MythicReward reward = { itemId, level, classMask };
        MythicDungeonLootStore[mapId].push_back(reward);
    } while (result->NextRow());
}

void MythicManager::InitializeRewardsPlayersBag()
{
    MythicDungeonBagRewardStore = {};
    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_mythic_rewards");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint64 itemGUID = fields[0].Get<uint64>();
        uint32 itemId = fields[1].Get<uint32>();
        MythicDungeonBagRewardStore[itemGUID].push_back(itemId);
    } while (result->NextRow());
}

void MythicManager::InitializeCreatureKillingCount()
{

}

void MythicManager::InitializeMultipliers()
{
    MythicMultiplierStore = {};
    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic_scaling");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 level = fields[0].Get<uint32>();
        float damage = fields[1].Get<float>();
        float health = fields[2].Get<float>();
        MythicMultiplier multi = { health, damage};
        MythicMultiplierStore[level] = multi;
    } while (result->NextRow());
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

void MythicManager::SaveMythicKey(Player* player, uint32 newDungeonId, uint32 level)
{
    CharacterDatabase.Query("UPDATE character_mythic_key SET dungeonId = {}, level = {} WHERE guid = {}", newDungeonId, level, player->GetGUID().GetCounter());
}

uint8 MythicManager::GetBossIndex(uint32 dungeonId, uint32 creatureId)
{
    return uint8();
}

void MythicManager::Update(uint32 diff)
{
    for (auto mythic : MythicStore)
        mythic.second->Update(diff);
}

void MythicManager::AddMythicDungeon(uint32 instanceId, Mythic* m)
{
    MythicStore[instanceId] = m;
}

void MythicManager::RemoveMythic(uint32 instanceId)
{
    MythicStore.erase(instanceId);
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
    for (auto const& dg : MythicDungeonStore)
        if (dg.id == dungeonId)
            return dg.itemId;

    return 0;
}

uint32 MythicManager::GetEnchantByMythicLevel(uint32 level)
{
    return level + 4006;
}

void MythicManager::UpdatePlayerKey(Player* player, uint8 upgrade)
{
    MythicKey* key = sMythicMgr->GetCurrentPlayerMythicKey(player);

    if (!key)
        return;

    uint32 currentItemId = sMythicMgr->GetItemIdWithDungeonId(key->dungeonId);
    player->DestroyItemCount(currentItemId, 1, true);

    uint32 dungeonId = sMythicMgr->GetRandomMythicDungeonForPlayer(player);

    key->dungeonId = dungeonId;
    key->level += upgrade;

    if (uint32 newItemId = sMythicMgr->GetItemIdWithDungeonId(dungeonId)) {

        player->AddItem(newItemId, 1);

        Item* item = player->GetItemByEntry(newItemId);

        if (item) {
            uint32 enchantId = sMythicMgr->GetEnchantByMythicLevel(key->level);
            item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantId, 0, 0, player->GetGUID());
        }

       //  sMythicMgr->SaveMythicKey(player, dungeonId, key->level);
    }

}

bool MythicManager::ForceCompleteMythic(Player* player)
{
    Mythic* mythic = GetMythicPlayer(player);
    if (!mythic)
        return false;

    mythic->OnCompleteMythicDungeon(player);

    return true;
}


Mythic* MythicManager::GetMythicPlayer(Player* player)
{
    auto itr = MythicStore.find(player->GetInstanceId());

    if (itr != MythicStore.end())
        if(itr->second->GetDungeonMapId() == player->GetMapId())
            return itr->second;

    return nullptr;
}

Mythic* MythicManager::GetMythicInMap(Map* map)
{
    auto itr = MythicStore.find(map->GetInstanceId());

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
    auto itr = MythicPlayerKeyStore.find(player->GetGUID().GetCounter());
    if (itr != MythicPlayerKeyStore.end()) {
        return &itr->second;
    }

    return nullptr;
}


uint32 MythicManager::GetTimeToCompleteByDungeonId(uint32 dungeonId)
{
    uint32 timeToComplete = 0;

    for (auto const& dg : MythicDungeonStore)
        if (dg.id == dungeonId)
            timeToComplete = dg.timeToComplete;

    return timeToComplete;
}


std::vector<MythicBossState> MythicManager::GetMythicBossesByDungeonId(uint32 dungeonId)
{
    std::vector<MythicBossState> bosses = {};
    auto itr = MythicDungeonBossStore.find(dungeonId);

    for (auto const& boss : itr->second)
        bosses.push_back({ boss.creatureId, true, boss.order + 1 });

    return bosses;
}

std::vector<std::string> MythicManager::GetDataMythicRun(Player* player)
{
    Mythic* mythic = GetMythicPlayer(player);

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
        CreatureTemplate const* creatureTemplate = sObjectMgr->GetCreatureTemplate(boss.creatureId);
        bossStatus += ";" + creatureTemplate->Name + "+" + std::to_string(boss.alive);
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


void MythicManager::ListenCreationMythicOnMapChanged(Player* leader)
{
    auto it = AsyncCreationMythic.find(leader->GetGUID());

    if (it == AsyncCreationMythic.end())
        return;

    Mythic* mythic = new Mythic(leader, it->second->dungeonId, it->second->level);

    AddMythicDungeon(leader->GetInstanceId(), mythic);

    MythicDungeon dungeon;
    GetMythicDungeonByDungeonId(it->second->dungeonId, dungeon);

    if(Group* group = leader->GetGroup())
        PrepareAndTeleportGroupMembers(leader, dungeon);

    AsyncCreationMythic.erase(it);
}

bool MythicManager::ShouldShowMythicUI(Player* player)
{
    Mythic* mythic = GetMythicPlayer(player);

    if (!mythic)
        return false;

    if (mythic->IsDungeonDone())
        return false;

    return true;
}

void MythicManager::OnKill(Player* player, Creature* killed)
{
    Map* map = player->GetMap();
    Mythic* mythic = GetMythicPlayer(player);

    if (!mythic)
        return;

    if (killed->IsDungeonBoss())
        mythic->OnKillBoss(player, killed);
    else
        mythic->OnKillCreature(player, killed);
}

void MythicManager::OnPlayerDie(Player* player, Creature* killed)
{
    Map* map = player->GetMap();
    Mythic* mythic = GetMythicPlayer(player);

    if (!mythic)
        return;

    mythic->OnPlayerKilledByCreature();
}

void MythicManager::Update(Creature* creature)
{
    Map* map = creature->GetMap();
    Mythic* mythic = GetMythicInMap(map);

    if (!mythic)
        return;

    if (creature->HasAura(90000))
        return;

    MythicMultiplier multi = GetMultplierByLevel(mythic->GetLevel());

    int32 damage = multi.damage;
    int32 health = multi.hp;

    creature->CastCustomSpell(creature, 90000, &damage, &health, nullptr, true);

    creature->ShouldRecalculate = true;
}

void MythicManager::PrepareAndTeleportGroupMembers(Player* player, MythicDungeon dungeon)
{
    if (Group* group = player->GetGroup()) {

        group->SetDungeonDifficulty(DUNGEON_DIFFICULTY_EPIC_PLUS);

        auto const& allyList = group->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* member = ObjectAccessor::FindPlayer(target.guid);
            if (member) {
                member->ClearUnitState(UNIT_STATE_ROOT);
                member->SetControlled(true, UNIT_STATE_ROOT);
                if (group->GetLeaderGUID() != player->GetGUID()) {
                    member->SetDungeonDifficulty(DUNGEON_DIFFICULTY_EPIC_PLUS);
                    member->TeleportTo(dungeon.mapId, dungeon.x, dungeon.y, dungeon.z, dungeon.o, 0, nullptr, true);
                }
            }
        }
    }
}



// Received when the player click on Start the Mythic dungeon in the game.

void MythicManager::PreparationMythicDungeon(Player* leader)
{
    MythicKey* mythicKey = GetCurrentPlayerMythicKey(leader);

    if (!mythicKey)
        return;

    MythicDungeon dungeon;
    GetMythicDungeonByDungeonId(mythicKey->dungeonId, dungeon);

    if(!dungeon)
        return;

    Group* group = leader->GetGroup();

    if (!group)
        leader->SetDungeonDifficulty(DUNGEON_DIFFICULTY_EPIC_PLUS);

    AsyncCreationMythic[leader->GetGUID()] = mythicKey;

    leader->ClearUnitState(UNIT_STATE_ROOT);
    leader->SetControlled(true, UNIT_STATE_ROOT);

    leader->TeleportTo(dungeon.mapId, dungeon.x, dungeon.y, dungeon.z, dungeon.o, 0, nullptr, true);
}
