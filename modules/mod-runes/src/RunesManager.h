#pragma once
#include "Player/Player.h"
#include "DatabaseEnv.h"

enum RuneMessage {
    RUNE_ACTIVATE_OK = 1,
    RUNE_ALREADY_ACTIVATED = 2,
    RUNE_TOO_MUCH_RUNE_ACTIVATED = 3,
    RUNE_NOT_KNOW = 4,
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

    bool operator !()
    {
        return !spellId;
    }
};

struct Loadout {
    uint64 guid;
    uint64 id;
    std::string title;
    bool active;
};

struct SpellRunes {
    uint32 runeSpellId;
    uint32 oldSpellId;
    uint32 newSpellId;
};

struct RuneConfig {
    bool enabled;
    bool debug;
    uint32 maxSlots;
    uint32 defaultSlot;
    float chanceDropRuneQualityWhite;
    float chanceDropRuneQualityGreen;
    float chanceDropRuneQualityBlue;
    float chanceDropRuneQualityEpic;
    float chanceDropRuneQualityLegendary;
    float chanceDropRuneQualityRed;
};

struct KnownRune {
    uint64 accountId;
    uint64 id;
    Rune rune;
};

struct SlotRune {
    uint64 id;
    uint64 runeId;
    uint64 runeSpellId;
    uint32 order;
};

struct AccountProgression {
    uint32 dusts;
    uint32 unlockedLoadoutCount;
    uint32 unlockedSlotRunes;
};

struct LearnRune {
    uint32 runeId;
    RuneMessage message;
};

class RunesManager {

private:
    static std::map<uint32, Rune> m_Runes;
    static std::map<uint32 /* accountId */, std::vector<KnownRune>> m_KnownRunes;
    static std::map<uint64 /* guid */, std::vector<Loadout>> m_Loadout;
    static std::map<uint64 /* slotId */, std::vector<SlotRune>> m_SlotRune;
    static std::map<uint32 /* accountId */, AccountProgression> m_Progression;
    static std::vector<SpellRunes> m_SpellRune;
    static RuneConfig config;
public:
    static void SetupConfig();
    static void LoadAllRunes();
    static void LoadAccountsRunes();
    static void LoadAllLoadout();
    static void LoadAllSlotRune();
    static void LoadAllProgression();
    static void CreateDefaultCharacter(Player* player);
    static std::vector<std::string> RunesForClients(Player* player);
    static std::vector<std::string> LoadoutCachingForClient(Player* player);
    static std::vector<std::string> SlotsCachingForClient(Player* player);
    static std::string ProgressionCachingForClient(Player* player);
    static Rune GetRuneBySpellId(uint32 spellId);
    static Rune GetRuneById(Player* player, uint64 id);
    static bool KnowRuneId(Player* player, uint64 runeId);
    static bool RuneAlreadyActivated(Player* player, uint64 runeId);
    static uint64 GetActiveLoadoutId(Player* player);
    static uint32 GetCoutSameGroupRune(Player* player, uint32 spellId);
    static uint32 GetCountActivatedRune(Player* player);
    static void ActivateRune(Player* player, uint32 index, uint64 runeId);
    static void DisableRune(Player* player, uint64 runeId);
    static void RefundRune(Player* player, uint64 runeId);
    static void UpgradeRune(Player* player, uint64 runeId);
    static void AddRuneToSlot(Player* player, Rune rune, uint64 runeId);
    static void RemoveRuneFromSlots(Player* player, Rune rune);
    static bool HasEnoughToUpgrade(Player* player, uint32 spellId);
};
