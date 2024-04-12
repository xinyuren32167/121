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
    Powers powerType;
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
    ROGUE_COMBAT = 21,
    ROGUE_SUBTLETY = 22,
    SHAMAN_ELEMENTAL = 23,
    SHAMAN_ENCHANCEMENT = 24,
    SHAMAN_RESTORATION = 25,
    WARLOCK_AFFLICTION = 26,
    WARLOCK_DEMONOLOGY = 27,
    WARLOCK_DESTRUCTION = 28,
    PRIEST_DISCI = 29,
    PRIEST_HOLY = 30,
    PRIEST_SHADOW = 31,
    SHAMAN_SPIRIT_MASTER = 32,
    WARRIOR_HOPLITE = 33,
    MAGE_SPELLBLADE = 34,
    HUNTER_DARK_RANGER = 35,
    PRIEST_ABSOLUTION = 36,
    PALADIN_INQUISITOR = 37,
    ROGUE_OUTLAW = 38,
    DK_SOULWEAVER = 39,
    WARLOCK_DEMONBOUND = 40,
};

enum WarlockSpells
{
    SPELL_MINION_INCREASE_DREAD_STALKER = 1100009,
    SPELL_MINION_INCREASE_WILD_IMP = 1100010,
    SPELL_MINION_INCREASE_DARKGLARE = 1100011,
    SPELL_MINION_INCREASE_VILEFIEND = 1100012,
    SPELL_MINION_INCREASE_DEMONIC_TYRANT = 1100013,
    SPELL_MINION_INCREASE_BOMBER = 1100014,
};

enum WarriorSpells
{
    SPELL_WARRIOR_WARBREAKER_REPLACER = 84546,
    SPELL_WARRIOR_WARBREAKER = 84519,
    SPELL_WARRIOR_COLOSSUS_SMASH = 80002,
};

enum HunterSpells
{
    SPELL_HUNTER_LONE_WOLF = 80182,
    SPELL_HUNTER_MONGOOSE_BITE_REPLACER = 80234,
    SPELL_HUNTER_MONGOOSE_BITE = 53339,
    SPELL_HUNTER_RAPTOR_STRIKE = 48996,
};

enum DruidSpells
{
    SPELL_DRUID_AVATAR_OF_ASHAMANE_REPLACER = 80675,
    SPELL_DRUID_AVATAR_OF_ASHAMANE = 80548,
    SPELL_DRUID_BERSERK_CAT = 50334,
    SPELL_DRUID_GUARDIAN_OF_URSOC_REPLACER = 80674,
    SPELL_DRUID_GUARDIAN_OF_URSOC = 80568,
    SPELL_DRUID_BERSERK_BEAR = 80566,
};

struct SpecValue {
    int32 specMask;
    // 0 : all, 1 is melee, 2 is for caster
    int32 type;
    uint32 statType;
};

struct Spec {
    uint32 specId;
    int32 PreferredSpecId;
};

class PlayerSpecialization {
public:
    static std::map<uint64 /* guid */, Spec> m_PlayersSpecialization;
    static std::map<uint32 /* specId */, Specialization> m_Specializations;
    static std::map<uint32 /* specId */, std::vector<uint32>> m_SpecSpells;
    static void LoadAllSpecsPlayers();
    static void LoadAllSpecsSpells();
    static void InitializeSpecializations();
    static void ActivateSpecialization(Player* player, uint32 specId);
    static void UpdateMastery(Player* player, uint32 rating);
    static SpecValue GetSpecValue(uint32 specId);
    static bool Exception(Player* player, uint32 spellId);
    static void RemoveSpellsAndAuras(Player* player);
    static uint32 GetCurrentSpecId(Player* player);
    static uint32 GetPreferredSpecId(Player* player);
    static void SetPreferredSpecId(Player* player, uint32 specMask);
    static std::vector<std::string> GetSpecializations(Player* player);
};
