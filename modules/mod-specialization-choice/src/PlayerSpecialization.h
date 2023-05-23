#pragma once

#include "Player/Player.h"
#include "DatabaseEnv.h"
#include "SharedDefines.h"

struct SpecializationPlayer {
    uint64 guid;
    uint32 specId;
};

struct Specialization {
    uint32 id;
    uint32 spellIcon;
    std::string name;
    uint32 classInfo;

    uint32 masteryTalentId;
    float modifierPerPoint;
    bool update;
    uint32 castSpellId;
};

enum Specializations {
    WARRIOR_ARMS = 1,
    WARRIOR_FURY = 2,
    WARRIOR_PROTECTION = 3,
    MAGE_ARCANE = 4,
    MAGE_FIRE = 5,
    MAGE_FROST = 6,
    DK_BLOOD = 7,
    DK_FROST = 8,
    DK_UNHOLY = 9,
    DRUID_BALANCE = 10,
    DRUID_FERAL = 11,
    DRUID_RESTO = 12,
    DRUID_GUARDIAN = 13,
    HUNTER_BEAST = 14,
    HUNTER_MARSKMANSHIP = 15,
    HUNTER_SURVIVAL = 16,
    PALADIN_HOLY = 17,
    PALADIN_PROTECTION = 18,
    PALADIN_RETRIBUTION = 19,
    ROGUE_ASSASSINATION = 20,
    ROGUE_OUTLAW = 21,
    ROGUE_SUBTETLY = 22,
    SHAMAN_ELEMENTAL = 23,
    SHAMAN_ENCHANCEMENT = 24,
    SHAMAN_RESTAURATION = 25,
    WARLOCK_AFFLICTION = 26,
    WARLOCK_DEMONOLOGY = 27,
    WARLOCK_DESTRUCTION = 28,
    PRIEST_HOLY = 29,
    PRIEST_DISCI = 30,
    PRIEST_SHADOW = 31,
};


class PlayerSpecialization {
public:
    static std::map<uint64 /* guid */, uint32 /* specId */> m_PlayersSpecialization;
    static std::map<uint32 /* specId */, Specialization> m_Specializations;
    static std::map<uint32 /* specId */, std::vector<uint32>> m_SpecSpells;
    static void LoadAllSpecsPlayers();
    static void LoadAllSpecsSpells();
    static void InitializeSpecializations();
    static void ActivateSpecialization(Player* player, uint32 specId);
    static void UpdateMastery(Player* player, uint32 rating);
    static uint32 GetCurrentSpecId(Player* player);
    static std::vector<std::string> GetSpecializations(Player* player);
};
