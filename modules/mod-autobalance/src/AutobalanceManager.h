#pragma once

#include "Player/Player.h"
#include "DatabaseEnv.h"
#include "Map.h"
#include "Creature.h"

struct AutobalanceScalingInfo {
    double meleeDamageModifier;
    double healthModifier;
    double spellDamageModifier;
    double periodicDamageModifier;
};

struct CreatureInfo {
    uint32 prevMaxHealth;
    uint32 prevMaxMana;
};

class AutoBalanceManager {

private:
    static std::map<uint8, AutobalanceScalingInfo> m_ScalingPerSpecialization;
    static std::map<uint32, AutobalanceScalingInfo> m_OverrideScalingPerCreatureId;
    static std::map<uint32, std::map<Difficulty, AutobalanceScalingInfo>> m_ScalingDungeonDifficulty;
public:
    static void InitializeScalingPerSpecialization();
    static void InitializeScalingRaid();
    static std::list<Player*> GetPlayersMap(Map* map);
    static Player* GetFirstPlayerMap(Map* map);
    static AutobalanceScalingInfo GetScalingInfo(Map* map, Unit* creature);
    static void ApplyScalingHealthAndMana(Map* map, Creature* creature);
    static bool SomeoneIsTooHighLevel(Map* map);
    static void SendMessageScalingInfo(Map* map);
};

