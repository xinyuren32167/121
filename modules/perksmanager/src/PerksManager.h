#pragma once
#include "Player/Player.h"
#include "DatabaseEnv.h"



enum PerkMessage {
    LEARN_OK = 0,
    LEARN_ALREALDY_HAVE_THIS_PERK = 1,
    LEARN_TOO_MUCH_OF_THIS_PERK = 2,
    TOO_MUCH_PERKS_ACTIVATED = 3,
    REFUND_OK = 4,
    DONT_HAVE_THIS_PERK = 5,
    UPGRADE_OK = 6,
    NOT_ENOUGH_OF_SIMILAR_PERK = 7,
    ACTIVATE_PERK_OK = 8,
    ACTIVATE_LOADOUT_OK = 9,
};

struct Perk {
    uint32 spellId;
    uint32 groupId;
    int allowableClass;
    uint32 quality;
    uint32 nextRankSpellId;
    uint8 maxStack;
};

struct PlayerPerk {
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
    uint8 slotCountAvailable;
    uint8 loadoutCountAvailable;
    uint64 runesDust;
};

class PerksManager {

private:
    static std::map<int, Perk> m_Perks;
    static std::map<uint32 /* accountId */, std::vector<PlayerPerk>> m_accountPerks;
    static std::map<uint64 /* guid */, std::vector<Loadout>> m_Loadouts;
    static std::map<uint32 /* accountId */, AccountProgression> m_AccountsProgression;
public:
    static void LoadAllPerks();
    static void LoadAccountsPerks();
    static void LoadAllLoadout();
    static void LoadAllAccountProgression();
    static void SavePlayer(Player* player);
    static PerkMessage LearnRandomPerk(Player* player);
    static PerkMessage LearnSpecificPerk(Player* player, uint32 spellId);
    static PerkMessage UpgradePerk(Player* player, uint32 spellId);
    static PerkMessage RefundPerk(Player* player, uint32 spellId);
    static PerkMessage ActivatePerk(Player* player, uint32 spellId);
    static PerkMessage ActivateLoadout(Player* player, uint8 slotId);
    // Return messsage string(slotCountAvailable;loadoutCountAvailable;RunesCountDust)
    static std::string GetAccountProgressionCachingForClient(Player* player, uint32 spellId);
    // Return messsage string(spellId;quality)
    static std::string GetLearningPerkForClient(Player* player, uint32 spellId);
    // Return messsage array(spellId;unlocked(0-1);count;quality;can be upgraded(0-1);can be refunded(0-1))
    static std::vector<std::string> GetCollectionCachingForClient(Player* player);
    // Return messsage array(slotId, spellId);
    static std::vector<std::string> GetSlotRunesCachingForClient(Player* player, uint8 slotId);
    // Return messsage array(uint8 slotId, title, active (0-1));
    static std::vector<std::string> GetLoadoutCachingForClient(Player* player);
    static uint32 GetNextRankSpellId(uint32 spellId);
};
