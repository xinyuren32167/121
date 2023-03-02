#include "AutobalanceManager.h"
#include "PlayerSpecialization.h"
std::map<uint8, AutobalanceScalingInfo> AutoBalanceManager::m_ScalingPerSpecialization = {};
std::map<uint32, AutobalanceScalingInfo> AutoBalanceManager::m_OverrideScalingPerCreatureId = {};
std::map<Difficulty, AutobalanceScalingInfo> AutoBalanceManager::m_ScalingRaidDifficulty = {};
std::map<Difficulty, AutobalanceScalingInfo> AutoBalanceManager::m_ScalingDungeonDifficulty = {};

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

AutobalanceScalingInfo AutoBalanceManager::GetScalingInfo(Map* map, Creature* creature)
{
    if (!map->IsRaid() && !map->IsDungeon())
        return {};

    if (creature->isDead())
        return {};

    bool isRaid = map->IsRaid();
    Difficulty difficulty = map->GetDifficulty();
    uint8 playerCount = map->GetPlayersCountExceptGMs();

    if (SomeoneIsTooHighLevel(map)) {
        AutobalanceScalingInfo info = { 1.0f, 1.0f, 1.0f, 1.0f };
        return info;
    }

    if (playerCount <= 1) {
        Player* player = GetFirstPlayerMap(map);

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
        if (isRaid) {
            auto match = m_ScalingRaidDifficulty.find(map->GetDifficulty());
            if (match != m_ScalingRaidDifficulty.end())
                return match->second;
        }
        else {
            auto match = m_ScalingDungeonDifficulty.find(map->GetDifficulty());
            if (match != m_ScalingDungeonDifficulty.end())
                return match->second;
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

    uint8 playerCount = map->GetPlayersCountExceptGMs();

    if (playerCount == 0)
        return;

    bool shouldRecalculate = playerCount != creature->AutobalancePlayerCount;

    if (!shouldRecalculate)
        return;

    creature->AutobalancePlayerCount = playerCount;

    AutobalanceScalingInfo scaling = GetScalingInfo(map, creature);

    if (!scaling.healthModifier)
        return;

    CreatureTemplate const* creatureTemplate = creature->GetCreatureTemplate();

    if (!creatureTemplate)
        return;

    float healthModifier = scaling.healthModifier;

    if (creature->IsDungeonBoss())
        healthModifier *= 2;

    if (creature->prevMaxHealth <= 0)
        creature->prevMaxHealth = creature->GetMaxHealth();

    uint32 maxHealth = creature->prevMaxHealth;
    uint32 scaledHealth = 0;

    if (playerCount == 1)
        scaledHealth = maxHealth * healthModifier;
    else {
        int8 maxPlayers = map->IsRaid() ? 25 : 5;
        int8 missingPlayers = (maxPlayers - playerCount);
        float scalingPerPlayer = missingPlayers > 1 ? (missingPlayers * healthModifier) : healthModifier;
        scaledHealth = missingPlayers > 0 ? maxHealth / scalingPerPlayer : maxHealth;
    }

    creature->SetMaxHealth(scaledHealth);
    creature->SetCreateHealth(scaledHealth);
    creature->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, (float)scaledHealth);
    creature->ResetPlayerDamageReq();
    creature->SetHealth(scaledHealth);
}
