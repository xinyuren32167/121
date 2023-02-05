#pragma once

#include "Player/Player.h"
#include "DatabaseEnv.h"
#include "Map.h"
#include "Creature.h"

struct AutobalanceScalingInfo {
    float meleeDamageModifier;
    float healthModifier;
    float spellDamageModifier;
    float periodicDamageModifier;
};

class AutoBalanceManager {

private:
    static std::map<uint8, AutobalanceScalingInfo> m_ScalingPerSpecialization;
    static std::map<uint32, AutobalanceScalingInfo> m_OverrideScalingPerCreatureId;
    static std::map<Difficulty, AutobalanceScalingInfo> m_ScalingRaidDifficulty;
    static std::map<Difficulty, AutobalanceScalingInfo> m_ScalingDungeonDifficulty;
public:
    static void InitializeScalingPerSpecialization();
    static void InitializeScalingPerCreatureId();
    static void InitializeScalingPerDifficulty();
    static std::list<Player*> GetPlayersMap(Map* map);
    static Player* GetAlonePlayerMap(Map* map);
    static AutobalanceScalingInfo GetScalingInfo(Map* map, Creature* creature);
};

