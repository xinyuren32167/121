/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AZEROTHCORE_PET_DEFINES_H
#define AZEROTHCORE_PET_DEFINES_H

#include "Define.h"
#include "Optional.h"
#include <array>
#include <string>
#include <vector>

enum ReactStates : uint8;

enum PetType : uint8
{
    SUMMON_PET                  = 0,
    HUNTER_PET                  = 1,
    MAX_PET_TYPE                = 4
};

constexpr auto MAX_PET_STABLES = 4;

// stored in character_pet.slot
enum PetSaveMode : int8
{
    PET_SAVE_AS_DELETED         = -1,                        // not saved in fact
    PET_SAVE_AS_CURRENT         =  0,                        // in current slot (with player)
    PET_SAVE_FIRST_STABLE_SLOT  =  1,
    PET_SAVE_LAST_STABLE_SLOT   =  MAX_PET_STABLES,          // last in DB stable slot index (including), all higher have same meaning as PET_SAVE_NOT_IN_SLOT
    PET_SAVE_NOT_IN_SLOT        =  100                       // for avoid conflict with stable size grow will use 100
};

enum HappinessState
{
    UNHAPPY                     = 1,
    CONTENT                     = 2,
    HAPPY                       = 3
};

enum PetSpellState
{
    PETSPELL_UNCHANGED          = 0,
    PETSPELL_CHANGED            = 1,
    PETSPELL_NEW                = 2,
    PETSPELL_REMOVED            = 3
};

enum PetSpellType
{
    PETSPELL_NORMAL             = 0,
    PETSPELL_FAMILY             = 1,
    PETSPELL_TALENT             = 2
};

enum ActionFeedback
{
    FEEDBACK_NONE               = 0,
    FEEDBACK_PET_DEAD           = 1,
    FEEDBACK_NOTHING_TO_ATT     = 2,
    FEEDBACK_CANT_ATT_TARGET    = 3
};

enum PetTalk
{
    PET_TALK_SPECIAL_SPELL      = 0,
    PET_TALK_ATTACK             = 1
};

enum PetLoadState
{
    PET_LOAD_OK                 = 0,
    PET_LOAD_NO_RESULT          = 1,
    PET_LOAD_ERROR              = 2
};

enum NPCEntries
{
    // Warlock
    NPC_INFERNAL                = 89,
    NPC_IMP                     = 416,
    NPC_FELHUNTER               = 417,
    NPC_VOIDWALKER              = 1860,
    NPC_SUCCUBUS                = 1863,
    NPC_DOOMGUARD               = 600616,
    NPC_FELGUARD                = 17252,
    NPC_FELGUARD_SUMMONED       = 600605,
    NPC_DARKGLARE               = 600604,
    NPC_DEMONIC_TYRANT          = 600603,
    NPC_DREADSTALKER            = 600600,
    NPC_BILESCOURGE             = 600607,
    NPC_VILEFIEND               = 600602,
    NPC_WILD_IMP                = 600601,
    NPC_INQUISITORS_GAZE        = 800000,
    NPC_PIT_LORD                = 800001,


    NPC_EYE_OF_KILROGG          = 4277,

    // Mage
    NPC_WATER_ELEMENTAL_TEMP    = 510,
    NPC_MIRROR_IMAGE            = 31216,
    NPC_WATER_ELEMENTAL_PERM    = 37994,
    NPC_POWERFUL_WATER_ELEMENTAL = 300000,
    NPC_POWERFUL_WATER_ELEMENTAL_PERM = 300001,

    // Druid
    NPC_TREANT                  = 1964,

    // Priest
    NPC_SHADOWFIEND             = 19668,

    // Shaman
    NPC_FIRE_ELEMENTAL          = 15438,
    NPC_STORM_ELEMENTAL          = 400408,

    NPC_EARTH_ELEMENTAL         = 15352,
    NPC_FERAL_SPIRIT            = 29264,

    // Death Knight
    NPC_RISEN_GHOUL             = 26125,
    NPC_BLOODWORM               = 28017,
    NPC_ARMY_OF_THE_DEAD        = 24207,
    NPC_EBON_GARGOYLE           = 27829,

    // Generic
    NPC_GENERIC_IMP             = 12922,
    NPC_GENERIC_VOIDWALKER      = 8996
};

enum PetScalingSpells
{
    SPELL_PET_AVOIDANCE                 = 32233,

    SPELL_HUNTER_PET_SCALING_01         = 34902,
    SPELL_HUNTER_PET_SCALING_02         = 34903,
    SPELL_HUNTER_PET_SCALING_03         = 34904,
    SPELL_HUNTER_PET_SCALING_04         = 61017, // Hit / Expertise

    // Warlock
    SPELL_WARLOCK_GENERAL_PET_PASSIVE_01        = 83540,
    SPELL_WARLOCK_GENERAL_PET_PASSIVE_02        = 83541,
    SPELL_WARLOCK_GENERAL_PET_PASSIVE_03        = 83542,

    // FELGUARD
    SPELL_WARLOCK_FELGUARD_PET_SCALING_01       = 83500,
    SPELL_WARLOCK_FELGUARD_PET_SCALING_02       = 83501,
    SPELL_WARLOCK_FELGUARD_PET_SCALING_03       = 83502,
    SPELL_WARLOCK_FELGUARD_PET_PASSIVE_01       = 83543,
    SPELL_WARLOCK_FELGUARD_PET_PASSIVE_02       = 83544,
    SPELL_WARLOCK_FELGUARD_PET_PASSIVE_03       = 83545,

    // FELHUNTER
    SPELL_WARLOCK_FELHUNTER_PET_SCALING_01      = 83503,
    SPELL_WARLOCK_FELHUNTER_PET_SCALING_02      = 83504,
    SPELL_WARLOCK_FELHUNTER_PET_SCALING_03      = 83505,
    SPELL_WARLOCK_FELHUNTER_PET_PASSIVE_01      = 83546,
    SPELL_WARLOCK_FELHUNTER_PET_PASSIVE_02      = 83547,
    SPELL_WARLOCK_FELHUNTER_PET_PASSIVE_03      = 83548,

    // IMP
    SPELL_WARLOCK_IMP_PET_SCALING_01            = 83506,
    SPELL_WARLOCK_IMP_PET_SCALING_02            = 83507,
    SPELL_WARLOCK_IMP_PET_SCALING_03            = 83508,
    SPELL_WARLOCK_IMP_PET_PASSIVE_01            = 83549,
    SPELL_WARLOCK_IMP_PET_PASSIVE_02            = 83550,
    SPELL_WARLOCK_IMP_PET_PASSIVE_03            = 83551,

    // SUCCUBUS
    SPELL_WARLOCK_SUCCUBUS_PET_SCALING_01       = 83509,
    SPELL_WARLOCK_SUCCUBUS_PET_SCALING_02       = 83510,
    SPELL_WARLOCK_SUCCUBUS_PET_SCALING_03       = 83511,
    SPELL_WARLOCK_SUCCUBUS_PET_PASSIVE_01       = 83552,
    SPELL_WARLOCK_SUCCUBUS_PET_PASSIVE_02       = 83553,
    SPELL_WARLOCK_SUCCUBUS_PET_PASSIVE_03       = 83554,

    // VOIDWALKER
    SPELL_WARLOCK_VOIDWALKER_PET_SCALING_01     = 83512,
    SPELL_WARLOCK_VOIDWALKER_PET_SCALING_02     = 83513,
    SPELL_WARLOCK_VOIDWALKER_PET_SCALING_03     = 83514,
    SPELL_WARLOCK_VOIDWALKER_PET_PASSIVE_01     = 83555,
    SPELL_WARLOCK_VOIDWALKER_PET_PASSIVE_02     = 83556,
    SPELL_WARLOCK_VOIDWALKER_PET_PASSIVE_03     = 83557,

    // DARKGLARE
    SPELL_WARLOCK_DARKGLARE_PET_SCALING_01      = 83515,
    SPELL_WARLOCK_DARKGLARE_PET_SCALING_02      = 83516,
    SPELL_WARLOCK_DARKGLARE_PET_SCALING_03      = 83517,
    SPELL_WARLOCK_DARKGLARE_PET_PASSIVE_01      = 83558,
    SPELL_WARLOCK_DARKGLARE_PET_PASSIVE_02      = 83559,
    SPELL_WARLOCK_DARKGLARE_PET_PASSIVE_03      = 83560,

    // DEMONIC TYRANT
    SPELL_WARLOCK_DEMONIC_TYRANT_PET_SCALING_01 = 83518,
    SPELL_WARLOCK_DEMONIC_TYRANT_PET_SCALING_02 = 83519,
    SPELL_WARLOCK_DEMONIC_TYRANT_PET_SCALING_03 = 83520,
    SPELL_WARLOCK_DEMONIC_TYRANT_PET_PASSIVE_01 = 83561,
    SPELL_WARLOCK_DEMONIC_TYRANT_PET_PASSIVE_02 = 83562,
    SPELL_WARLOCK_DEMONIC_TYRANT_PET_PASSIVE_03 = 83563,
    SPELL_WARLOCK_DEMONIC_TYRANT_PET_FLAME_NOURISH = 83207,

    // DOOMGUARD
    SPELL_WARLOCK_DOOMGUARD_PET_SCALING_01      = 83521,
    SPELL_WARLOCK_DOOMGUARD_PET_SCALING_02      = 83522,
    SPELL_WARLOCK_DOOMGUARD_PET_SCALING_03      = 83523,
    SPELL_WARLOCK_DOOMGUARD_PET_PASSIVE_01      = 83564,
    SPELL_WARLOCK_DOOMGUARD_PET_PASSIVE_02      = 83565,
    SPELL_WARLOCK_DOOMGUARD_PET_PASSIVE_03      = 83566,

    // DREADSTALKER
    SPELL_WARLOCK_DREADSTALKER_PET_SCALING_01   = 83524,
    SPELL_WARLOCK_DREADSTALKER_PET_SCALING_02   = 83525,
    SPELL_WARLOCK_DREADSTALKER_PET_SCALING_03   = 83526,
    SPELL_WARLOCK_DREADSTALKER_PET_PASSIVE_01   = 83567,
    SPELL_WARLOCK_DREADSTALKER_PET_PASSIVE_02   = 83568,
    SPELL_WARLOCK_DREADSTALKER_PET_PASSIVE_03   = 83569,

    // BILESCOURGE
    SPELL_WARLOCK_BILESCOURGE_PET_SCALING_01    = 83527,
    SPELL_WARLOCK_BILESCOURGE_PET_SCALING_02    = 83528,
    SPELL_WARLOCK_BILESCOURGE_PET_SCALING_03    = 83529,
    SPELL_WARLOCK_BILESCOURGE_PET_PASSIVE_01    = 83570,
    SPELL_WARLOCK_BILESCOURGE_PET_PASSIVE_02    = 83571,
    SPELL_WARLOCK_BILESCOURGE_PET_PASSIVE_03    = 83572,

    // INFERNAL
    SPELL_WARLOCK_INFERNAL_PET_SCALING_01       = 83530,
    SPELL_WARLOCK_INFERNAL_PET_SCALING_02       = 83531,
    SPELL_WARLOCK_INFERNAL_PET_SCALING_03       = 83532,
    SPELL_WARLOCK_INFERNAL_PET_PASSIVE_01       = 83573,
    SPELL_WARLOCK_INFERNAL_PET_PASSIVE_02       = 83574,
    SPELL_WARLOCK_INFERNAL_PET_PASSIVE_03       = 83575,
    SPELL_WARLOCK_INFERNAL_PET_FEL_FIRE         = 83213,
    SPELL_WARLOCK_INFERNAL_PET_FEL_ATTACKS      = 83214,

    // VILEFIEND
    SPELL_WARLOCK_VILEFIEND_PET_SCALING_01      = 83533,
    SPELL_WARLOCK_VILEFIEND_PET_SCALING_02      = 83534,
    SPELL_WARLOCK_VILEFIEND_PET_SCALING_03      = 83535,
    SPELL_WARLOCK_VILEFIEND_PET_PASSIVE_01      = 83576,
    SPELL_WARLOCK_VILEFIEND_PET_PASSIVE_02      = 83577,
    SPELL_WARLOCK_VILEFIEND_PET_PASSIVE_03      = 83578,

    // WILD IMP
    SPELL_WARLOCK_WILD_IMP_PET_SCALING_01       = 83536,
    SPELL_WARLOCK_WILD_IMP_PET_SCALING_02       = 83537,
    SPELL_WARLOCK_WILD_IMP_PET_SCALING_03       = 83538,
    SPELL_WARLOCK_WILD_IMP_PET_PASSIVE_01       = 83579,
    SPELL_WARLOCK_WILD_IMP_PET_PASSIVE_02       = 83580,
    SPELL_WARLOCK_WILD_IMP_PET_PASSIVE_03       = 83581,

    // Pit Lord
    SPELL_WARLOCK_PIT_LORD_PET_SCALING_01       = 83582,
    SPELL_WARLOCK_PIT_LORD_PET_SCALING_02       = 83583,
    SPELL_WARLOCK_PIT_LORD_PET_SCALING_03       = 83584,
    SPELL_WARLOCK_PIT_LORD_PET_PASSIVE_01       = 83585,
    SPELL_WARLOCK_PIT_LORD_PET_PASSIVE_02       = 83586,
    SPELL_WARLOCK_PIT_LORD_PET_PASSIVE_03       = 83587,
    
    SPELL_GLYPH_OF_FELGUARD                     = 56246,
    SPELL_RITUAL_ENSLAVEMENT                    = 22987,

    // Shaman
    SPELL_FERAL_SPIRIT_SPIRIT_HUNT      = 58877,
    SPELL_FERAL_SPIRIT_SCALING_01       = 35674,
    SPELL_FERAL_SPIRIT_SCALING_02       = 35675,
    SPELL_FERAL_SPIRIT_SCALING_03       = 35676,
    SPELL_FIRE_ELEMENTAL_SCALING_01     = 35665,
    SPELL_FIRE_ELEMENTAL_SCALING_02     = 35666,
    SPELL_FIRE_ELEMENTAL_SCALING_03     = 35667,
    SPELL_FIRE_ELEMENTAL_SCALING_04     = 35668,
    SPELL_EARTH_ELEMENTAL_SCALING_01    = 65225,
    SPELL_EARTH_ELEMENTAL_SCALING_02    = 65226,
    SPELL_EARTH_ELEMENTAL_SCALING_03    = 65227,
    SPELL_EARTH_ELEMENTAL_SCALING_04    = 65228,
    SPELL_EARTH_ELEMENTAL_SCALING_05    = 2064,
    SPELL_ORC_RACIAL_COMMAND_SHAMAN     = 65223,

    // Priest
    SPELL_SHADOWFIEND_SCALING_01        = 35661,
    SPELL_SHADOWFIEND_SCALING_02        = 35662,
    SPELL_SHADOWFIEND_SCALING_03        = 35663,
    SPELL_SHADOWFIEND_SCALING_04        = 35664,

    // Druid
    SPELL_TREANT_SCALING_01             = 35669,
    SPELL_TREANT_SCALING_02             = 35670,
    SPELL_TREANT_SCALING_03             = 35671,
    SPELL_TREANT_SCALING_04             = 35672,

    // Mage
    SPELL_MAGE_PET_SCALING_01           = 35657,
    SPELL_MAGE_PET_SCALING_02           = 35658,
    SPELL_MAGE_PET_SCALING_03           = 35659,
    SPELL_MAGE_PET_SCALING_04           = 35660,

    // Death Knight
    SPELL_ORC_RACIAL_COMMAND_DK         = 65221,
    SPELL_NIGHT_OF_THE_DEAD_AVOIDANCE   = 62137,
    SPELL_DK_PET_SCALING_01             = 51996,
    SPELL_DK_PET_SCALING_02             = 54566,
    SPELL_DK_PET_SCALING_03             = 61697,
    SPELL_DK_PET_SCALING_04             = 600003,
    SPELL_IMPROVED_BLOODWORMS           = 80390,
};

#define PET_FOLLOW_DIST  1.0f
#define SECOND_PET_FOLLOW_DIST  -3.0f

#define GUARDIAN_WARLOCK_WILD_IMP_DIST -4.0f
#define GUARDIAN_WARLOCK_DREADSTALKER_DIST  -4.5f
#define GUARDIAN_WARLOCK_DEMONIC_TYRAN_DIST 2.5f
#define GUARDIAN_WARLOCK_VILEFIEND_DIST 1.5f


#define PET_FOLLOW_ANGLE (M_PI/2)

class PetStable
{
public:
    struct PetInfo
    {
        PetInfo() { }

        std::string Name;
        std::string ActionBar;
        uint32 PetNumber = 0;
        uint32 CreatureId = 0;
        uint32 DisplayId = 0;
        uint32 Experience = 0;
        uint32 Health = 0;
        uint32 Mana = 0;
        uint32 Happiness = 0;
        uint32 LastSaveTime = 0;
        uint32 CreatedBySpellId = 0;
        uint8 Level = 0;
        ReactStates ReactState = ReactStates(0);
        PetType Type = MAX_PET_TYPE;
        bool WasRenamed = false;
    };

    Optional<PetInfo> CurrentPet;                                   // PET_SAVE_AS_CURRENT
    std::array<Optional<PetInfo>, MAX_PET_STABLES> StabledPets;     // PET_SAVE_FIRST_STABLE_SLOT - PET_SAVE_LAST_STABLE_SLOT
    uint32 MaxStabledPets = 0;
    std::vector<PetInfo> UnslottedPets;                             // PET_SAVE_NOT_IN_SLOT

    [[nodiscard]] PetInfo const* GetUnslottedHunterPet() const
    {
        return UnslottedPets.size() == 1 && UnslottedPets[0].Type == HUNTER_PET ? &UnslottedPets[0] : nullptr;
    }
};

#endif
