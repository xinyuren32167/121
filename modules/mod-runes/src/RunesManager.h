#pragma once
#include "Player/Player.h"
#include "DatabaseEnv.h"
#include "PlayerSpecialization.h"


/*

    Warrior, ClassMask 1
    --------------
        Warrior - Arms : 1
        Warrior - Fury : 2
        Warrior - Protection : 4
        Warrior - Hoplite : 8
    --------------
    --------------
    --------------

*/

struct Rune {
    uint32 spellId;
    uint32 groupId;
    int32 allowableClass;
    int32 allowableRaces;
    int8 quality;
    int8 maxStack;
    std::string keywords;
    int32 specMask;
    bool isLucky;
    bool isAutorefund;
    uint8 type;

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

struct SpellRuneConversion {
    uint32 runeSpellId;
    uint32 oldSpellId;
    uint32 newSpellId;
};

struct RuneConfig {
    bool enabled;
    bool debug;
    uint32 maxSlots;
    uint32 defaultSlot;
    std::map<uint8, uint32> upgradeCostRunicEssence;

    int8 costMineralToUpgradeToEpic;
    int8 costMineralToUpgradeToLegendary;
    int8 costMineralToUpgradeToMythic;

    int8 costSkullUpgradeRankLegendary;
    int8 costSkullUpgradeRankMythic;

    int8 costBetterSkullUpgradeRankLegendary;
    int8 costBetterSkullUpgradeRankMythic;
};

struct KnownRune {
    uint64 accountId;
    uint64 id;
    Rune rune;
    uint32 count;
};

struct SlotRune {
    uint64 id;
    uint64 runeSpellId;
    uint32 order;
    bool operator !()
    {
        return !order;
    }
};

struct LuckyRunes {
    uint32 runeSpellId1;
    uint32 runeSpellId2;
    uint32 runeSpellId3;
};

struct Draw {
    float luckyDrawChanceCommon;
    float luckyDrawChanceUncommon;
    float luckyDrawChanceRare;
    float luckyDrawChanceEpic;
};

struct AccountProgression {
    uint32 dusts;
    uint32 unlockedLoadoutCount;
    uint32 unlockedSlotRunes;
};

struct RewardAchievement {
    uint32 itemId1;
    uint32 itemId1Amount;

    uint32 itemId2;
    uint32 itemId2Amount;

    uint32 itemId3;
    uint32 itemId3Amount;
};

enum RuneQuality {
    NORMAL_QUALITY = 1,
    UNCOMMON_QUALITY = 2,
    RARE_QUALITY = 3,
    EPIC_QUALITY = 4,
    LEGENDARY_QUALITY = 5,
    MYTHICAL_QUALITY = 6,
};

class RunesManager {

private:
    static std::map<uint32, Rune> m_Runes;
    static std::unordered_multimap<uint32 /* groupId */, Rune>  m_unorderedRunes;
    static std::map<uint32 /* accountId */, std::vector<KnownRune>> m_KnownRunes;
    static std::map<uint64 /* guid */, std::vector<Loadout>> m_Loadout;
    static std::map<uint64 /* slotId */, std::vector<SlotRune>> m_SlotRune;
    static std::map<uint32 /* accountId */, AccountProgression> m_Progression;
    static std::map<uint32 /* achievementId */, RewardAchievement> m_RewardAchievement;
    static std::vector<SpellRuneConversion> m_SpellRuneConversion;
    static std::map<uint64, std::vector<uint32>> m_CharacterAutoRefundRunes;
    static std::map<uint64, LuckyRunes> m_CharacterLuckyRunes;
    static std::map<uint64, Draw> m_CharacterRuneDraw;
    static RuneConfig config;
    static uint32 GetMissingSlotNumber(std::vector<SlotRune> slots, Player* p);
public:

    static bool IsDebugEnabled() { return config.debug; };
    static void SetupConfig();
    static void LoadAllLuckyRunes();
    static void LoadAllAutoRefund();
    static void LoadAllRunes();
    static void LoadAccountsRunes();
    static void LoadAllLoadout();
    static void LoadCharacterDraws();
    static void LoadAllSlotRune();
    static void LoadAllProgression();
    static void LoadSpellsConversion();
    static void LoadRewardsAchievement();
    static void CreateDefaultCharacter(Player* player);
    static bool IsSpellIdLuckyRune(Player* player, uint32 spellId);
    static bool IsSpellIdAutoRefund(Player* player, uint32 spellId);
    static std::vector<std::string> KnownRuneForClient(Player* player);
    static std::string RuneForClient(Player* player, Rune rune, bool known, uint32 count);
    static std::vector<std::string> RunesForClient(Player* player);
    static std::vector<std::string> RunesUpgradeForClient(Player* player);
    static std::vector<std::string> LoadoutCachingForClient(Player* player);
    static std::vector<std::string> SlotsCachingForClient(Player* player);
    static std::string ProgressionCachingForClient(Player* player);
    static Rune GetRuneByQuality(uint32 groupId, int8 nextQuality);
    static bool IsRuneUpgradable(Player* player, Rune rune, uint32 countRune);
    static Rune GetRuneBySpellId(uint32 spellId);
    static bool KnowRuneId(Player* player, uint64 runeId);
    static void AutomaticalyRefundRune(Player* player, Rune rune);
    static bool RuneAlreadyActivated(Player* player, uint64 runeId);
    static uint64 GetActiveLoadoutId(Player* player);
    static uint32 GetCoutSameGroupRune(Player* player, uint32 spellId);
    static uint32 GetCountActivatedRune(Player* player);
    static void ActivateRune(Player* player, uint32 index, uint64 runeId);
    static void ResetAllSlots(Player* player);
    static void ActivateLoadout(Player* player, uint64 loadoutId);
    static void CastActivateLoadout(Player* player, uint64 loadoutId);
    static void RemoveSlotsOnCharacterDel(ObjectGuid guid);
    static void ApplyRunesOnLogin(Player* player);
    static void OnCastActivateLoadout(Player* player, uint64 loadoutId);
    static void DisableRune(Player* player, uint64 runeId);
    static std::vector<std::string> GetRunesByPlayerName(std::string name);
    static void RefundRune(Player* player, uint32 runeSpellId);
    static void ApplyLuckyRune(Player* player, uint32 runeSpellId, bool enabled);
    static void ApplyAutorefund(Player* player, uint32 runeSpellId);
    static void UpgradeRune(Player* player, uint32 runeSpellId);
    static void AddRuneToSlot(Player* player, Rune rune);
    static uint8 GetCountRuneOfSameQuality(Player* player, uint32 spellId);
    static void RemoveRuneFromSlots(Player* player, Rune rune);
    static void RemoveRune(Player* player, Rune rune, int8 quantity);
    static bool HasEnoughToUpgrade(Player* player, uint32 spellId);
    static void SpellConversion(uint32 runeId, Player* player, bool apply);
    static void SendPlayerMessage(Player* player, std::string msg);
    static std::vector<std::string> GetActivatedRunes(Player* player);
    static Rune GetRandomRune(Player* player, uint8 quality);
    static void RemoveNecessaryItemsForUpgrade(Player* player, Rune rune);
    static void AddRunesPlayer(Player* player, std::vector<Rune> runes);
    static uint32 CalculateGoldCostToBuyRune(Player* player);
    static void GiveAchievementReward(Player* player, uint32 achievementId);
    static void ApplyBuyingRuneWithGold(Player* player);
    static int GetPreviousWeekFromBuyingRuneWithGold(Player* player);
    static void SendChat(Player* player, std::string msg);
};
