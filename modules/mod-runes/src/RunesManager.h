#pragma once
#include "Player/Player.h"
#include "DatabaseEnv.h"

enum RuneMessage {
    LEARN_OK = 0,
    LEARN_ALREALDY_HAVE_THIS_RUNE = 1,
    LEARN_TOO_MUCH_OF_THIS_RUNE = 2,
    TOO_MUCH_Runes_ACTIVATED = 3,
    REFUND_OK = 4,
    DONT_HAVE_THIS_RUNE = 5,
    UPGRADE_OK = 6,
    NOT_ENOUGH_OF_SIMILAR_RUNE = 7,
    ACTIVATE_RUNE_OK = 8,
    ACTIVATE_LOADOUT_OK = 9,
};

struct Rune {
    uint32 spellId;
    uint32 groupId;
    int allowableClass;
    uint32 quality;
    uint32 nextRankSpellId;
    uint8 maxStack;
};

struct PlayerRune {
    uint8 slotId;
    uint32 accountId;
    uint32 spellId;
    bool active;
};

struct Loadout {
    uint64 guid;
    uint8 slotId;
    std::string title;
    bool active;
};

struct AccountProgression {
    uint32 accountId;
    uint8 slotRuneCountAvailable;
    uint8 loadoutCountAvailable;
    uint64 runesDust;
};


struct SwitchSpellRune {
    uint32 runeSpellId;
    uint32 oldSpellId;
    uint32 newSpellId;
};

class RunesManager {

private:
    static std::map<int, Rune> m_Runes;
    static std::map<uint32 /* accountId */, std::vector<PlayerRune>> m_accountRunes;
    static std::map<uint64 /* guid */, std::vector<Loadout>> m_Loadouts;
    static std::map<uint32 /* accountId */, AccountProgression> m_AccountsProgression;
    static std::vector<SwitchSpellRune> m_SwitchSpellRune;
public:
    static void LoadAllRunes();
    static void LoadAccountsRunes();
    static void LoadAllLoadout();
    static void LoadAllAccountProgression();
    static void LoadAllSpellsSwitch();
    static void SavePlayer(Player* player);
    static RuneMessage LearnRandomRune(Player* player);
    static RuneMessage LearnSpecificRune(Player* player, uint32 spellId);
    static RuneMessage UpgradeRune(Player* player, uint32 spellId);
    static RuneMessage RefundRune(Player* player, uint32 spellId);
    static RuneMessage ActivateRune(Player* player, uint32 spellId);
    static RuneMessage DeactivateRune(Player* player, uint32 spellId);
    static RuneMessage ActivateLoadout(Player* player, uint8 slotId);
    // Return messsage string(slotCountAvailable;loadoutCountAvailable;RunesCountDust)
    static std::string GetAccountProgressionCachingForClient(Player* player, uint32 spellId);
    // Return messsage string(spellId;quality)
    static std::string GetLearningRuneForClient(Player* player, uint32 spellId);
    // Return messsage array(spellId;unlocked(0-1);count;quality;can be upgraded(0-1);can be refunded(0-1))
    static std::vector<std::string> GetCollectionCachingForClient(Player* player);
    // Return messsage array(slotId, spellId);
    static std::vector<std::string> GetSlotRunesCachingForClient(Player* player, uint8 slotId);
    // Return messsage array(uint8 slotId, title, active (0-1));
    static std::vector<std::string> GetLoadoutCachingForClient(Player* player);
    static uint32 GetNextRankSpellId(uint32 spellId);
};
