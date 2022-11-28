#pragma once
#include "Player/Player.h"
#include "DatabaseEnv.h"

enum RuneMessage {
    LEARN_RUNE_OK = 1,
    LEARN_RUNE_ERROR = 2,
    ACTIVATE_RUNE_OK = 3,
    ACTIVATE_RUNE_TOO_MUCH = 4,
    REFUND_RUNE_OK = 5,
    DONT_HAVE_THIS_RUNE = 6,
    UPGRADE_RUNE_OK = 7,
    UPGRADE_RUNE_ERROR = 8,
    SYSTEM_TEMPORARILY_DISABLED = 9,
    LEARN_SLOT_OK = 10,
    LEARN_SLOT_ERROR = 11,
    ACTIVATE_LOADOUT_OK = 12,
    ACTIVATE_LOADOUT_ERROR = 13,
    UPDATE_LOADOUT_OK = 14,
    CONVERT_RUNE_OK = 15,
    CONVERT_RUNE_ERROR = 16,
};

struct Rune {
    uint32 spellId;
    uint32 groupId;
    int32 allowableClass;
    int32 allowableRaces;
    int8 quality;
    int8 maxStack;
    uint32 refundItemId;
    uint32 refundDusts;
    std::string keywords;
};

struct SlotRune {
    uint64 guid;
    uint32 runeId;
    uint32 slotId;
    bool unlocked;
};

struct Loadout {
    uint64 guid;
    uint32 id;
    std::string title;
    std::string data;
    bool active;
};

struct SpellRunes {
    uint32 runeSpellId;
    uint32 oldSpellId;
    uint32 newSpellId;
};

struct Config {
    bool isEnabled;
    bool debugEnabled;
    uint32 maxSlotRunes;
    float chanceDropRuneQualityWhite = 90.0f;
    float chanceDropRuneQualityGreen = 10.0f;
    float chanceDropRuneQualityBlue = 0.2f;
    float chanceDropRuneQualityEpic = 0;
    float chanceDropRuneQualityLegendary = 0;
    float chanceDropRuneQualityRed = 0;
};

struct RuneAccount {
    uint64 accountId;
    uint64 id;
    Rune rune;
};

struct LearnRune {
    uint32 runeId;
    RuneMessage message;
};

class RunesManager {

private:
    static std::map<uint32, Rune> m_Runes;
    static std::map<uint32 /* accountId */, std::vector<RuneAccount>> m_accountRunes;
    static std::map<uint64 /* guid */, std::vector<Loadout>> m_Loadouts;
    static std::map<uint64 /* guid */, std::vector<SlotRune>> m_SlotsRune;
    static std::vector<SpellRunes> m_SpellRune;
    static Config config;
public:
    static void SetupConfig(Config config);
    static void LoadAllRunes();
    static void LoadAccountsRunes();
    static void LoadAllLoadout();
    static void LoadAllAccountProgression();
    static void LoadAllSpells();
    static void SavePlayer(Player* player);
    static void CreateSlotRunes(Player* player);
    static LearnRune LearnRandomRune(Player* player, uint8 quality);
    static RuneMessage LearnSpecificRune(Player* player, uint32 spellId);
    static RuneMessage UpgradeRune(Player* player, uint32 spellId);
    static RuneMessage UnlockSlotRune(Player* player);
    static RuneMessage RefundRune(Player* player, uint32 spellId);
    static RuneMessage ConvertRuneToItem(Player* player, uint32 runeId);
    static RuneMessage ActivateRune(Player* player, uint32 spellId);
    static RuneMessage DeactivateRune(Player* player, uint32 spellId);
    static RuneMessage UpdateLoadout(Player* player, uint8 slotId);
    static RuneMessage ActivateLoadout(Player* player, uint8 slotId);
    static std::vector<std::string> AllRunesCachingForClient();
    static void ProcessSpellFromRune(Player* player, uint32 spellId, bool unlearnRunes);
    static uint32 GetNextRankSpellId(uint32 spellId);
    static Rune GetRuneById(uint32 runeId);
};
