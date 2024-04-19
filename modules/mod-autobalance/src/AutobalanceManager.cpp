#include "AutobalanceManager.h"
#include "PlayerSpecialization.h"
#include "WorldSession.h"
#include "Chat.h"

std::map<uint8, AutobalanceScalingInfo> AutoBalanceManager::m_ScalingPerSpecialization = {};
std::map<uint32, float> AutoBalanceManager::m_ScalingPerSpecializationValueHealth = {
    { }

};
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

void AutoBalanceManager::SendMessageScalingInfo(Map* map)
{

    if (!map->IsDungeon() && !map->IsRaid())
        return;

    std::list<Player*> players = GetPlayersMap(map);

    if (players.size() == 0)
        return;

    std::string mapName = map->GetMapName();

    uint32 maxPlayers = map->IsRaid() && players.size() < 10 ? 10 : players.size();

    for (const auto& player : players)
    {
        ChatHandler(player->GetSession()).SendSysMessage(mapName + " is now scaling for " + std::to_string(maxPlayers) + " player(s).");
    }
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
        double damageModifier = fields[3].Get<double>();
        
        AutobalanceScalingInfo info = { damageModifier, healthModifier, 0, 0 };

        m_ScalingDungeonDifficulty[mapId].insert(std::make_pair(Difficulty(difficulty), info));
    } while (result->NextRow());
}

AutobalanceScalingInfo AutoBalanceManager::GetScalingInfo(Map* map, Unit* creature)
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
            return { 0.5, 0.3, 0, 0 };

        auto match = m_ScalingPerSpecialization.find(CurrentSpecialization);

        if (match != m_ScalingPerSpecialization.end())
            return match->second;

        return {};
    }

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

    if (((creature->IsHunterPet() || creature->IsPet() || creature->IsSummon()) && creature->IsControlledByPlayer() && creature->IsTotem()))
        return;

    if (!creature->IsAlive())
        return;

    uint8 playerCount = map->GetPlayersCountExceptGMs();
    bool isRaid = map->IsRaid();

    if (playerCount == 0)
        return;

    bool shouldRecalculate = (playerCount != creature->AutobalancePlayerCount) || creature->ShouldRecalculate == true;

    if (!shouldRecalculate)
        return;

    creature->AutobalancePlayerCount = playerCount;

    CreatureTemplate const* creatureTemplate = creature->GetCreatureTemplate();

    if (!creatureTemplate)
        return;

    if (creature->prevMaxHealth <= 0)
        creature->prevMaxHealth = creature->GetMaxHealth();

    uint32 scaledHealth = 0;
    creature->ShouldRecalculate = false;

    if (map->IsRaid())
    {
        double totalReduction = CalculateHealthRaidScaling(playerCount);
        scaledHealth = creature->prevMaxHealth * totalReduction;
    }
    else
    {
        double totalReduction = CalculateHealthDungeonScaling(map);
        scaledHealth = creature->prevMaxHealth * totalReduction;
    }

    creature->SetMaxHealth(scaledHealth);
    creature->SetCreateHealth(scaledHealth);
    creature->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, (float)scaledHealth);
    creature->ResetPlayerDamageReq();
    creature->SetHealth(scaledHealth);
}

float AutoBalanceManager::CalculateHealthRaidScaling(uint8 totalPlayerInRaid)
{
    if (totalPlayerInRaid < 10)
        totalPlayerInRaid = 10;

    return ((totalPlayerInRaid - 10) * 0.10) + 1;
}

float AutoBalanceManager::CalculateHealthDungeonScaling(Map* map)
{
    std::list<Player*> players = GetPlayersMap(map);

    float healthScaling = 0;

    for (auto const& i : players) {
        const float value = GetPlayerSpecializationHealthValueForScaling(i);
        healthScaling += value;
    }

    return healthScaling;
}

float AutoBalanceManager::CalculateDamageDungeonScaling(Map* map)
{
    std::list<Player*> players = GetPlayersMap(map);

    float damageScaling = 0;

    for (auto const& i : players) {
        const float value = GetPlayerSpecializationDamageValueForScaling(i);
        damageScaling += value;
    }

    return damageScaling;
}


float AutoBalanceManager::GetPlayerSpecializationHealthValueForScaling(Player* player)
{
    uint32 specId = PlayerSpecialization::GetCurrentSpecId(player);

    if (!specId)
        return 0.25;

    switch (specId) {
        case WARRIOR_ARMS: return 0.25;
        case WARRIOR_FURY: return 0.25;
        case WARRIOR_PROTECTION: return 0.15;
        case WARRIOR_HOPLITE: return 0.25;
        case MAGE_ARCANE: return 0.25;
        case MAGE_FIRE: return 0.25;
        case MAGE_FROST: return 0.25;
        case MAGE_SPELLBLADE: return 0.25;
        case DK_BLOOD: return 0.15;
        case DK_FROST: return 0.25;
        case DK_UNHOLY: return 0.25;
        case DK_SOULWEAVER: return 0.10;
        case DRUID_BALANCE: return 0.25;
        case DRUID_FERAL: return 0.25;
        case DRUID_RESTO: return 0.10;
        case DRUID_GUARDIAN: return 0.15;
        case HUNTER_BEAST: return 0.25;
        case HUNTER_MARSKMANSHIP: return 0.25;
        case HUNTER_SURVIVAL: return 0.25;
        case HUNTER_DARK_RANGER: return 0.25;
        case PALADIN_HOLY: return 0.10;
        case PALADIN_PROTECTION: return 0.15;
        case PALADIN_RETRIBUTION: return 0.25;
        case PALADIN_INQUISITOR: return 0.25;
        case ROGUE_ASSASSINATION: return 0.25;
        case ROGUE_COMBAT: return 0.25;
        case ROGUE_SUBTLETY: return 0.25;
        case ROGUE_OUTLAW: return 0.25;
        case SHAMAN_ELEMENTAL: return 0.25;
        case SHAMAN_ENCHANCEMENT: return 0.25;
        case SHAMAN_RESTORATION: return 0.10;
        case SHAMAN_SPIRIT_MASTER: return 0.25;
        case WARLOCK_AFFLICTION: return 0.25;
        case WARLOCK_DEMONOLOGY: return 0.25;
        case WARLOCK_DESTRUCTION: return 0.25;
        case WARLOCK_DEMONBOUND: return 0.25;
        case PRIEST_DISCI: return 0.10;
        case PRIEST_HOLY: return 0.10;
        case PRIEST_SHADOW: return 0.25;
        case PRIEST_ABSOLUTION: return 0.25;
    }

    return 0.25;
}

float AutoBalanceManager::GetPlayerSpecializationDamageValueForScaling(Player* player)
{
    uint32 specId = PlayerSpecialization::GetCurrentSpecId(player);

    if (!specId)
        return 0.25;

    switch (specId) {
    case WARRIOR_ARMS: return 0.10;
    case WARRIOR_FURY: return 0.10;
    case WARRIOR_PROTECTION: return 0.30;
    case WARRIOR_HOPLITE: return 0.10;
    case MAGE_ARCANE: return 0.10;
    case MAGE_FIRE: return 0.10;
    case MAGE_FROST: return 0.10;
    case MAGE_SPELLBLADE: return 0.10;
    case DK_BLOOD: return 0.30;
    case DK_FROST: return 0.10;
    case DK_UNHOLY: return 0.10;
    case DK_SOULWEAVER: return 0.40;
    case DRUID_BALANCE: return 0.10;
    case DRUID_FERAL: return 0.10;
    case DRUID_RESTO: return 0.40;
    case DRUID_GUARDIAN: return 0.30;
    case HUNTER_BEAST: return 0.10;
    case HUNTER_MARSKMANSHIP: return 0.10;
    case HUNTER_SURVIVAL: return 0.10;
    case HUNTER_DARK_RANGER: return 0.10;
    case PALADIN_HOLY: return 0.40;
    case PALADIN_PROTECTION: return 0.30;
    case PALADIN_RETRIBUTION: return 0.10;
    case PALADIN_INQUISITOR: return 0.10;
    case ROGUE_ASSASSINATION: return 0.10;
    case ROGUE_COMBAT: return 0.10;
    case ROGUE_SUBTLETY: return 0.10;
    case ROGUE_OUTLAW: return 0.10;
    case SHAMAN_ELEMENTAL: return 0.10;
    case SHAMAN_ENCHANCEMENT: return 0.10;
    case SHAMAN_RESTORATION: return 0.40;
    case SHAMAN_SPIRIT_MASTER: return 0.30;
    case WARLOCK_AFFLICTION: return 0.10;
    case WARLOCK_DEMONOLOGY: return 0.10;
    case WARLOCK_DESTRUCTION: return 0.10;
    case WARLOCK_DEMONBOUND: return 0.30;
    case PRIEST_DISCI: return 0.40;
    case PRIEST_HOLY: return 0.40;
    case PRIEST_SHADOW: return 0.10;
    case PRIEST_ABSOLUTION: return 0.40;
    }

    return 0.25;
}

