#include "AutobalanceManager.h"
#include "PlayerSpecialization.h"
#include "WorldSession.h"
#include "Chat.h"
#include "MythicDungeonManager.h"

std::map<uint8, AutobalanceScalingInfo> AutoBalanceManager::m_ScalingPerSpecialization = {};
std::map<uint32, AutobalanceScalingInfo> AutoBalanceManager::m_OverrideScalingPerCreatureId = {};
std::map<uint32, std::map<Difficulty, AutobalanceScalingInfo>> AutoBalanceManager::m_ScalingDungeonDifficulty = {};

std::list<Player*> AutoBalanceManager::GetPlayersMap(Map* map)
{
    std::list<Player*> players = {};

    Map::PlayerList const& playerList = map->GetPlayers();
    if (!playerList.IsEmpty())
        for (Map::PlayerList::const_iterator playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
            if (Player* playerHandle = playerIteration->GetSource())
                players.push_back(playerHandle);

    return players;
}

bool AutoBalanceManager::SomeoneIsTooHighLevel(Map* map)
{
    uint32 minLevel;

    DungeonProgressionRequirements const* ar = sObjectMgr->GetAccessRequirement(map->GetId(), Difficulty(map->GetDifficulty()));

    if (ar)
        minLevel = ar->levelMin;

    Map::PlayerList const& playerList = map->GetPlayers();
    if (!playerList.IsEmpty())
        for (Map::PlayerList::const_iterator playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
            if (Player* playerHandle = playerIteration->GetSource())
                return (playerHandle->getLevel() - 3) > minLevel;

    return false;
}

void AutoBalanceManager::SendMessageScalingInfo(Map* map)
{

    if (!map->IsDungeon() && !map->IsRaid())
        return;

    std::list<Player*> players = GetPlayersMap(map);

    if (players.size() == 0)
        return;

    std::string mapName = map->GetMapName();

    for (const auto& player : players)
        ChatHandler(player->GetSession()).SendSysMessage("Scaling " + mapName + " for " + std::to_string(players.size()) + " players.");
}

Player* AutoBalanceManager::GetFirstPlayerMap(Map* map)
{
    std::list<Player*> players = GetPlayersMap(map);

    if (players.size() == 0)
        return nullptr;

    return players.front();
}

void AutoBalanceManager::InitializeScalingPerSpecialization()
{
    m_ScalingPerSpecialization = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM scalings_per_specialization");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint8 specialization = fields[0].Get<uint8>();
        float meleeDamage = fields[1].Get<float>();
        float healthModifier = fields[2].Get<float>();
        float spellDamageModifier = fields[3].Get<float>();
        float periodicDamageModifier = fields[4].Get<float>();
        AutobalanceScalingInfo info = { meleeDamage, healthModifier, spellDamageModifier, periodicDamageModifier };
        m_ScalingPerSpecialization[specialization] = info;
    } while (result->NextRow());
}

void AutoBalanceManager::InitializeScalingRaid()
{
    m_ScalingDungeonDifficulty = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM scaling_instance");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        uint32 difficulty = fields[1].Get<uint32>();
        double healthModifier = fields[2].Get<double>();
        AutobalanceScalingInfo info = { 0, healthModifier, 0, 0 };
        m_ScalingDungeonDifficulty[mapId].insert(std::make_pair(Difficulty(difficulty), info));
    } while (result->NextRow());
}

AutobalanceScalingInfo AutoBalanceManager::GetScalingInfo(Map* map, Creature* creature)
{
    if (!map->IsRaid() && !map->IsDungeon())
        return {};

    if (creature->isDead())
        return {};

    bool isRaid = map->IsRaid();
    Difficulty difficulty = map->GetDifficulty();
    uint8 playerCount = map->GetPlayers().getSize();

    if (playerCount <= 1 && !isRaid) {

        auto mapPlayer = map->GetPlayers().getFirst();
        Player* player = mapPlayer->GetSource();

        if (!player)
            return {};

        uint8 CurrentSpecialization = PlayerSpecialization::GetCurrentSpecId(player);

        if (CurrentSpecialization <= 0)
            return {};

        auto match = m_ScalingPerSpecialization.find(CurrentSpecialization);

        if (match != m_ScalingPerSpecialization.end())
            return match->second;

        return {};
    }

    uint32 creatureId = creature->GetEntry();

    auto match = m_OverrideScalingPerCreatureId.find(creatureId);

    if (match != m_OverrideScalingPerCreatureId.end())
        return match->second;

    if (playerCount > 1) {
        auto match = m_ScalingDungeonDifficulty.find(map->GetId());
        if (match != m_ScalingDungeonDifficulty.end()) {
            auto found = match->second.find(map->GetDifficulty());
            if (found != match->second.end())
                return found->second;
        }
    }

    return {};
}

void AutoBalanceManager::ApplyScalingHealthAndMana(Map* map, Creature* creature)
{
    if (!creature || !creature->GetMap())
        return;

    if (!creature->GetMap()->IsDungeon() && !creature->GetMap()->IsBattleground())
        return;

    if (((creature->IsHunterPet() || creature->IsPet() || creature->IsSummon()) && creature->IsControlledByPlayer()))
        return;

    if (!creature->IsAlive())
        return;

    uint8 playerCount = map->GetPlayers().getSize();
    bool isRaid = map->IsRaid();

    if (playerCount == 0)
        return;

    bool shouldRecalculate = (playerCount != creature->AutobalancePlayerCount) || creature->ShouldRecalculate;

    if (!shouldRecalculate)
        return;

    creature->AutobalancePlayerCount = playerCount;

    AutobalanceScalingInfo scaling = GetScalingInfo(map, creature);

    if (!scaling.healthModifier)
        return;

    CreatureTemplate const* creatureTemplate = creature->GetCreatureTemplate();

    if (!creatureTemplate)
        return;

    if (creature->prevMaxHealth <= 0)
        creature->prevMaxHealth = creature->GetMaxHealth();

    uint32 scaledHealth = 0;
    creature->ShouldRecalculate = false;

    if (playerCount == 1)
        scaledHealth = creature->prevMaxHealth * scaling.healthModifier;
    else {
        int8 maxPlayers = map->IsRaid() ? 25 : 5;
        int8 missingPlayers = (maxPlayers - playerCount);
        int8 minPlayersCount = map->IsRaid() && missingPlayers < 10 ? 10 : missingPlayers;
        double totalReduction = scaling.healthModifier * minPlayersCount;
        scaledHealth = creature->prevMaxHealth - (totalReduction * creature->prevMaxHealth);
    }

    AddPct(scaledHealth, MythicDungeonManager::GetHPMultiplicator(map));

    creature->SetMaxHealth(scaledHealth);
    creature->SetCreateHealth(scaledHealth);
    creature->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, (float)scaledHealth);
    creature->ResetPlayerDamageReq();
    creature->SetHealth(scaledHealth);
    
}
