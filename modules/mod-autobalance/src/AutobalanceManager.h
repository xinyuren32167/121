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

struct CreatureInfo {
    uint32 prevMaxHealth;
    uint32 prevMaxMana;
};

class AutoBalanceManager {

private:
    static std::map<uint8, AutobalanceScalingInfo> m_ScalingPerSpecialization;
    static std::map<uint32, AutobalanceScalingInfo> m_OverrideScalingPerCreatureId;
    static std::map<Difficulty, AutobalanceScalingInfo> m_ScalingRaidDifficulty;
    static std::map<Difficulty, AutobalanceScalingInfo> m_ScalingDungeonDifficulty;
public:
    static void InitializeScalingPerSpecialization();
    static std::list<Player*> GetPlayersMap(Map* map);
    static Player* GetFirstPlayerMap(Map* map);
    static AutobalanceScalingInfo GetScalingInfo(Map* map, Creature* creature);
    static void ApplyScalingHealthAndMana(Map* map, Creature* creature);
    static void ApplyScalingMeleeDamage(Map* map, Creature* creature);
    static void ApplyScalingSpellDamage(Map* map, Creature* creature);
    static void ApplyScalingPeriodicDamage(Map* map, Creature* creature);
    static void ApplyScaleDownMoney(Player* player, Loot* loot);
    static void ApplyScaleDownXp(Player* player, uint32& amount, Unit* victim);
    static bool SomeoneIsTooHighLevel(Map* map);
};

