#include "RunesManager.h"
#include "boost/bind.hpp"
#include "Config.h"
#include "ElunaIncludes.h"
#include "LuaEngine.h"
#include "boost/iterator/counting_iterator.hpp"

std::map<uint32 /* SpellId */, Rune> RunesManager::m_Runes = {};
std::unordered_multimap<uint32 /* groupId */, Rune> RunesManager::m_unorderedRunes = {};
std::map<uint32 /* accountId */, std::vector<KnownRune>> RunesManager::m_KnownRunes = {};
std::map<uint64 /* guid */, std::vector<Loadout>> RunesManager::m_Loadout = {};
std::map<uint64 /* LoadoutId */, std::vector<SlotRune>> RunesManager::m_SlotRune = {};
std::map<uint32 /* accountId */, AccountProgression> RunesManager::m_Progression = {};
std::map<uint32 /* achievementId */, RewardAchievement> RunesManager::m_RewardAchievement = {};

std::vector<SpellRuneConversion> RunesManager::m_SpellRuneConversion = {};
std::map<uint64, int8> RunesManager::m_CharacterRuneDraw = {};

RuneConfig RunesManager::config = {};

void RunesManager::SetupConfig()
{
    config.enabled = sConfigMgr->GetOption<bool>("RuneManager.enabled", true);
    config.debug = false;
    config.maxSlots = 10;
    config.defaultSlot = 4;

    config.upgradeCostRunicEssence.insert(std::make_pair(RARE_QUALITY, sWorld->GetValue("CONFIG_COST_RUNIC_ESSENCE_RARE")));
    config.upgradeCostRunicEssence.insert(std::make_pair(EPIC_QUALITY, sWorld->GetValue("CONFIG_COST_RUNIC_ESSENCE_EPIC")));
    config.upgradeCostRunicEssence.insert(std::make_pair(LEGENDARY_QUALITY, sWorld->GetValue("CONFIG_COST_RUNIC_ESSENCE_LEGENDARY")));
    config.upgradeCostRunicEssence.insert(std::make_pair(MYTHICAL_QUALITY, sWorld->GetValue("CONFIG_COST_RUNIC_ESSENCE_MYTHICAL")));

    config.costMineralToUpgradeToEpic = sWorld->GetValue("CONFIG_COST_MINERAL_EPIC");
    config.costMineralToUpgradeToLegendary = sWorld->GetValue("CONFIG_COST_MINERAL_LEGENDARY");
    config.costMineralToUpgradeToMythic = sWorld->GetValue("CONFIG_COST_MINERAL_MYTHICAL");

    config.costSkullUpgradeRankLegendary = sWorld->GetValue("CONFIG_COST_TOKEN_UPGRADE_LEGENDARY");
    config.costSkullUpgradeRankMythic = sWorld->GetValue("CONFIG_COST_TOKEN_UPGRADE_MYTHICAL");

    config.costBetterSkullUpgradeRankLegendary = sWorld->GetValue("CONFIG_COST_UPPER_TOKEN_UPGRADE_LEGENDARY");
    config.costBetterSkullUpgradeRankMythic = sWorld->GetValue("CONFIG_COST_UPPER_TOKEN_UPGRADE_MYTHICAL");

}

void RunesManager::LoadAllRunes()
{
    RunesManager::m_Runes = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM runes");

    if (!result)
        return;
    do
    {
        Field* fields = result->Fetch();
        uint32 id = fields[0].Get<uint32>();
        uint32 groupId = fields[1].Get<uint32>();
        int32 allowableClass = fields[2].Get<int32>();
        int32 allowableRace = fields[3].Get<int32>();
        int8 quality = fields[4].Get<int8>();
        int8 maxStacks = fields[5].Get<int8>();
        uint32 refundItemId = fields[6].Get<uint32>();
        uint32 refundDusts = fields[7].Get<uint32>();
        std::string keywords = fields[8].Get<std::string>();
        Rune rune = { id, groupId, allowableClass, allowableRace, quality, maxStacks, refundItemId, refundDusts, keywords };
        m_Runes.insert(std::make_pair(id, rune));
        m_unorderedRunes.insert(std::make_pair(groupId, rune));
    } while (result->NextRow());

}

void RunesManager::LoadSpellsConversion()
{

    RunesManager::m_SpellRuneConversion = {};
    QueryResult result = WorldDatabase.Query("SELECT * FROM runes_spell_switch");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 runeSpellId = fields[0].Get<uint32>();
        uint32 oldSpellId = fields[1].Get<uint32>();
        uint32 newSpellId = fields[2].Get<int32>();
        SpellRuneConversion s = { runeSpellId, oldSpellId, newSpellId };
        m_SpellRuneConversion.push_back(s);
    } while (result->NextRow());
}


void RunesManager::LoadRewardsAchievement()
{
    RunesManager::m_RewardAchievement = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM achievement_reward_bonus");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 achievementId = fields[0].Get<uint32>();

        uint32 itemId1 = fields[1].Get<uint32>();
        uint32 itemId1Amount = fields[2].Get<uint32>();

        uint32 itemId2 = fields[3].Get<uint32>();
        uint32 itemId2Amount = fields[4].Get<uint32>();

        uint32 itemId3 = fields[5].Get<uint32>();
        uint32 itemId3Amount = fields[6].Get<uint32>();

        m_RewardAchievement[achievementId] = { itemId1, itemId1Amount, itemId2, itemId2Amount, itemId3, itemId3Amount };

    } while (result->NextRow());
}

void RunesManager::LoadAccountsRunes()
{
    RunesManager::m_KnownRunes = {};

    QueryResult result = CharacterDatabase.Query("SELECT * FROM account_know_runes");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 accountId = fields[0].Get<uint32>();
        uint32 id = fields[1].Get<uint32>();
        uint32 runeId = fields[2].Get<uint32>();
        Rune rune = GetRuneBySpellId(runeId);
        KnownRune knowRune = { accountId, id, rune, 1 };
        m_KnownRunes[accountId].push_back(knowRune);
    } while (result->NextRow());


    for (auto& accountRunes : m_KnownRunes) {

        std::vector<KnownRune> newRunesForAccount = {};

        for (const auto& accountRune : accountRunes.second)
        {
            uint32 spellIdToFind = accountRune.rune.spellId; // ID du spell à rechercher
            int8 qualityToFind = accountRune.rune.quality;  // Qualité à rechercher

            auto ij = std::find_if(newRunesForAccount.begin(), newRunesForAccount.end(),
                [&](const KnownRune& accountRune) {
                return accountRune.rune.quality == qualityToFind && accountRune.rune.spellId == spellIdToFind;
            });

            if (ij != newRunesForAccount.end())
                ij->count += 1;
            else
                newRunesForAccount.push_back(accountRune);
        }

        m_KnownRunes[accountRunes.first] = newRunesForAccount;
    }

}

void RunesManager::LoadAllLoadout()
{
    RunesManager::m_Loadout = {};

    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_rune_loadout ORDER BY `active` DESC");
    if (!result)
        return;
    do
    {
        Field* fields = result->Fetch();
        uint64 guid = fields[0].Get<uint64>();
        uint64 id = fields[1].Get<uint64>();
        std::string title = fields[2].Get<std::string>();
        bool active = fields[3].Get<bool>();
        Loadout loadout = { guid, id, title, active };
       m_Loadout[guid].push_back(loadout);
    } while (result->NextRow());
}

void RunesManager::LoadCharacterDraws()
{
    RunesManager::m_CharacterRuneDraw = {};

    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_draw");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint64 guid = fields[0].Get<uint64>();
        int8 count = fields[1].Get<int8>();
        m_CharacterRuneDraw.insert(std::make_pair(guid, count));
    } while (result->NextRow());
}


void RunesManager::LoadAllSlotRune()
{
    RunesManager::m_SlotRune = {};

    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_rune_slots ORDER BY `order` DESC");
    if (!result)
        return;
    do
    {
        Field* fields = result->Fetch();
        uint64 loadoutId = fields[0].Get<uint64>();
        uint32 runeSpellId = fields[1].Get<uint32>();
        uint64 order = fields[2].Get<uint64>();
        SlotRune slot = { loadoutId, runeSpellId, order };
        m_SlotRune[loadoutId].push_back(slot);
    } while (result->NextRow());
}

void RunesManager::LoadAllProgression()
{
    RunesManager::m_Progression = {};

    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_rune_progression");
    if (!result)
        return;
    do
    {
        Field* fields = result->Fetch();
        uint32 accountId = fields[0].Get<uint32>();
        uint32 dusts  = fields[1].Get<uint32>();
        uint32 loadoutUnlocked  = fields[2].Get<uint32>();
        uint32 slotsUnlocked = fields[3].Get<uint32>();
        AccountProgression progression = { dusts, loadoutUnlocked, slotsUnlocked };
        m_Progression.insert(std::make_pair(accountId, progression));
    } while (result->NextRow());
}

void RunesManager::CreateDefaultCharacter(Player* player)
{
    CharacterDatabasePreparedStatement* stmt = nullptr;
    uint64 startIdSlot = 1;
    bool active = true;
    std::string slotDefault = "Default";
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MAXID_LOADOUT);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        Field* fields = result->Fetch();
        startIdSlot = fields[0].Get<uint64>();

        if (startIdSlot == 0)
            startIdSlot = 1;
    }

    uint64 guid = player->GetGUID().GetCounter();
    uint32 accountId = player->GetSession()->GetAccountId();
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_FIRST_LOADOUT);
    stmt->SetData(0, guid);
    stmt->SetData(1, startIdSlot);
    stmt->SetData(2, slotDefault);
    stmt->SetData(3, active);

    CharacterDatabase.Execute(stmt);

    Loadout loadout = { guid, startIdSlot, slotDefault, true };
    m_Loadout[guid].push_back(loadout);

    CharacterDatabase.Query("INSERT INTO character_rune_progression (accountId, dusts, loadoutUnlocked, slotsUnlocked) VALUES ({}, 0, 0, {}) ", accountId, config.defaultSlot);
    CharacterDatabase.Query("INSERT INTO character_draw (guid, totalDraw) VALUES ({}, 0) ", guid);

    m_CharacterRuneDraw.insert(std::make_pair(guid, 0));

    auto progression = m_Progression.find(player->GetSession()->GetAccountId());

    if (progression == m_Progression.end()) {
        AccountProgression progression = { 0, 0, config.defaultSlot };
        m_Progression[accountId] = { progression };
    }

}

void RunesManager::SpellConversion(uint32 runeId, Player* player, bool apply)
{
    for (auto const& spell : m_SpellRuneConversion)
        if (spell.runeSpellId == runeId) {
            if (apply) {
                player->learnSpell(spell.newSpellId, false, false, true);
                player->removeSpell(spell.oldSpellId, SPEC_MASK_ALL, false, true);
            }
            else {
                player->learnSpell(spell.oldSpellId, false, false, true);
                player->removeSpell(spell.newSpellId, SPEC_MASK_ALL, false, true);
            }
        }

}

void RunesManager::SendPlayerMessage(Player* player, std::string msg)
{
    player->GetSession()->SendAreaTriggerMessage(msg.c_str());
}

Rune RunesManager::GetRandomRune(Player* player, uint8 quality)
{
    std::vector<Rune> possibleRunes = {};
    uint64 guid = player->GetGUID().GetCounter();

    auto match = m_CharacterRuneDraw.find(guid);

    if (match == m_CharacterRuneDraw.end()) {
        return {};
    }

    if (quality == NORMAL_QUALITY) {

        bool isUpgradedThroughLuck = roll_chance_i(10);
        int8 totalNormalDraw = match->second;

        if (isUpgradedThroughLuck)
            quality = UNCOMMON_QUALITY;

        if (totalNormalDraw >= 10) {
            quality = UNCOMMON_QUALITY;
            match->second = 0;
        }
        else
            match->second += 1;

        CharacterDatabase.Execute("UPDATE character_draw SET totalDraw = {} WHERE guid = {}", match->second, guid);
    }

    for (const auto& pair : m_Runes)
        if (pair.second.quality == quality && (pair.second.allowableClass & player->getClassMask()))
            possibleRunes.push_back(pair.second);

    if (possibleRunes.empty())
        return {};

    uint32 rand = urand(0, possibleRunes.size() - 1);

    Rune rune = possibleRunes[rand];

    return rune;
}


int RunesManager::GetPreviousWeekFromBuyingRuneWithGold(Player* player)
{
    uint32 guid = player->GetGUID().GetCounter();

    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_buying_rune_gold WHERE guid = {}", guid);

    if (!result)
        return -1;

    do
    {
        Field* fields = result->Fetch();
        uint32 oldWeek = fields[2].Get<uint32>();
        return oldWeek;

    } while (result->NextRow());

}

void RunesManager::ApplyBuyingRuneWithGold(Player* player)
{
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);

    int weekNumber = now->tm_yday / 7 + 1;

    uint32 guid = player->GetGUID().GetCounter();

    uint32 oldWeek = GetPreviousWeekFromBuyingRuneWithGold(player);

    if (oldWeek == -1)
    {
        CharacterDatabase.Execute("INSERT INTO character_buying_rune_gold (guid, goldT, dayOfTheWeek) VALUES ({}, {}, {}) ", guid, 1, oldWeek);
        return;
    }

    if (oldWeek >= 0)
    {
        if (oldWeek != weekNumber)
            CharacterDatabase.Execute("UPDATE character_buying_rune_gold SET goldT = 1, dayOfTheWeek = {} WHERE guid = {}", guid, oldWeek);
        else
            CharacterDatabase.Execute("UPDATE character_buying_rune_gold SET goldT = goldT + 1 WHERE guid = {}", guid, oldWeek);
    }
}

uint32 RunesManager::CalculateGoldCostToBuyRune(Player* player)
{

    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);

    uint32 cost = sWorld->GetValue("CONFIG_BASE_COST_BUYING_RUNE");

    int weekNumber = now->tm_yday / 7 + 1;

    uint32 guid = player->GetGUID().GetCounter();

    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_buying_rune_gold WHERE guid = {}", guid);

    if (!result)
    {
        return cost;
    }

    int newCost = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 guid = fields[0].Get<uint32>();
        uint32 goldT = fields[1].Get<uint32>();
        uint32 oldWeek = fields[2].Get<uint32>();

        if (oldWeek == weekNumber)
        {
            newCost = cost + goldT * sWorld->GetValue("CONFIG_INCREMENT_COST_BUYING_RUNE");
        }
        else
            newCost = cost;
            

    } while (result->NextRow());

    return std::min(newCost, sWorld->GetValue("CONFIG_GOLD_CAP_BUYING_RUNE"));
}


void RunesManager::RemoveNecessaryItemsForUpgrade(Player* player, Rune nextRune)
{
    Rune previousRune = GetRuneByQuality(nextRune.groupId, nextRune.quality - 1);

    uint32 accountId = player->GetSession()->GetAccountId();

    auto it = m_KnownRunes.find(accountId); 

    auto ij = std::find_if(it->second.begin(), it->second.end(),
        [&](const KnownRune& accountRune) {
        return accountRune.rune.quality == previousRune.quality && accountRune.rune.spellId == previousRune.spellId;
    });

    if (ij == it->second.end())
        return;

    if (ij->count > 3)
        ij->count -= 3;
    else
        it->second.erase(ij);

    CharacterDatabase.Execute("DELETE FROM account_know_runes WHERE accountId = {} AND spellId = {} LIMIT 3", accountId, previousRune.spellId);

    if (nextRune.quality >= RARE_QUALITY) {
        uint32 cost = config.upgradeCostRunicEssence[nextRune.quality];
        player->DestroyItemCount(70009, cost, true);
    }

    if (nextRune.quality == EPIC_QUALITY) {
        int8 cost = config.costMineralToUpgradeToEpic;
        player->DestroyItemCount(70010, cost, true);
    }

    if (nextRune.quality == LEGENDARY_QUALITY) {
        int8 costMineralToUpgradeToLegendary = config.costMineralToUpgradeToLegendary;
        int8 costSkullUpgradeRankLegendary = config.costSkullUpgradeRankLegendary;
        int8 costBetterSkullUpgradeRankLegendary = config.costBetterSkullUpgradeRankLegendary;

        player->DestroyItemCount(70011, costMineralToUpgradeToLegendary, true);
        player->DestroyItemCount(70013, costSkullUpgradeRankLegendary, true);
        player->DestroyItemCount(70014, costBetterSkullUpgradeRankLegendary, true);
    }

    if (nextRune.quality == MYTHICAL_QUALITY) {
        int8 costMineralToUpgradeToMythic = config.costMineralToUpgradeToMythic;
        int8 costSkullUpgradeRankMythic = config.costSkullUpgradeRankMythic;
        int8 costBetterSkullUpgradeRankMythic = config.costBetterSkullUpgradeRankMythic;

        player->DestroyItemCount(70012, costMineralToUpgradeToMythic, true);
        player->DestroyItemCount(70013, costSkullUpgradeRankMythic, true);
        player->DestroyItemCount(70014, costBetterSkullUpgradeRankMythic, true);
    }

    std::stringstream fmt;

    fmt << std::to_string(previousRune.spellId);

    sEluna->RemoveRune(player, fmt.str());
}

void RunesManager::AddRunePlayer(Player* player, Rune rune)
{
    uint32 accountId = player->GetSession()->GetAccountId();
    auto it = m_KnownRunes.find(accountId);

    uint32 count = 1;

    if (it != m_KnownRunes.end()) {
        auto ij = std::find_if(it->second.begin(), it->second.end(),
            [&](const KnownRune& accountRune) {
            return accountRune.rune.quality == rune.quality && accountRune.rune.spellId == rune.spellId;
        });

        if (ij != it->second.end()) {
            ij->count += 1;
            count = ij->count;
        }
        else {
            KnownRune knownRune = { accountId, 0, rune, 1 };
            it->second.push_back(knownRune);
        }
    }
    else {
        KnownRune knownRune = { accountId, 0, rune, 1 };
        m_KnownRunes[accountId].push_back(knownRune);
    }

    CharacterDatabase.Execute("INSERT INTO account_know_runes (accountId, spellId) VALUES ({}, {}) ", accountId, rune.spellId);

    std::string str = RuneForClient(player, rune, true, count);
  
    sEluna->PushRune(player, str);
}


std::string RunesManager::RuneForClient(Player* player, Rune rune, bool known, uint32 count)
{
    std::string fmt =
            std::to_string(rune.spellId)
        + ";" + std::to_string(rune.quality)
        + ";" + std::to_string(rune.maxStack)
        + ";" + rune.keywords
        + ";" + std::to_string(rune.allowableClass)
        + ";" + std::to_string(config.debug ? 1 : count)
        + ";" + std::to_string(config.debug ? true : known);

    return fmt;
}

std::vector<std::string> RunesManager::RunesForClient(Player* player)
{

    std::vector<uint32> runeIds = {};
    std::vector<std::string> elements = {};

    auto known = m_KnownRunes.find(player->GetSession()->GetAccountId());
    if (known != m_KnownRunes.end()) {
        for (auto const& kwownRune : known->second) {
            runeIds.push_back(kwownRune.rune.spellId);
            elements.emplace_back(RuneForClient(player, kwownRune.rune, true, kwownRune.count));
        }
    }

    for (auto it = m_Runes.begin(); it != m_Runes.end(); it++)
    {
        Rune rune = it->second;
        if (std::find(runeIds.begin(), runeIds.end(), rune.spellId) == runeIds.end()) {
            elements.emplace_back(RuneForClient(player, rune, false, 0));
        }
    }
    return elements;
}


std::vector<std::string> RunesManager::RunesUpgradeForClient(Player* player)
{
    std::vector<std::string > elements = {};
    auto known = m_KnownRunes.find(player->GetSession()->GetAccountId());

    if (known == m_KnownRunes.end())
        return elements;


    for (auto const& knownRune : known->second) {

        int8 nextQuality = knownRune.rune.quality + 1;

        Rune nextRune = GetRuneByQuality(knownRune.rune.groupId, nextQuality);
        bool isUpgradable = IsRuneUpgradable(player, nextRune, knownRune.count);

        if ((knownRune.rune.allowableClass & player->getClassMask()) == 0)
            continue;

        if (!isUpgradable)
            continue;

        if (knownRune.count <= 2)
            continue;

        if (nextQuality > MYTHICAL_QUALITY)
            continue;

        std::stringstream fmt;

        fmt << knownRune.rune.spellId << "," << knownRune.count << "," << 3 << "," << std::to_string(knownRune.rune.quality);

        fmt << "|" << nextRune.spellId << "," << std::to_string(nextQuality);

        if(nextQuality == RARE_QUALITY)
            fmt << "|" << "70009" << "," << config.upgradeCostRunicEssence[nextQuality];
        if (nextQuality == EPIC_QUALITY)
            fmt << "|" << "70009" << "," << config.upgradeCostRunicEssence[nextQuality] << ";"  << "70010"
            << "," << std::to_string(config.costMineralToUpgradeToEpic);
        if (nextQuality == LEGENDARY_QUALITY)
            fmt << "|" << "70009" << "," << config.upgradeCostRunicEssence[nextQuality] << ";" << "70011" << "," << std::to_string(config.costMineralToUpgradeToLegendary)
            << ";" << "70013" << "," << std::to_string(config.costSkullUpgradeRankLegendary) << ";" << "70014" << "," << std::to_string(config.costBetterSkullUpgradeRankLegendary);
        if (nextQuality == MYTHICAL_QUALITY)
            fmt << "|" << "70009" << "," << config.upgradeCostRunicEssence[nextQuality] << ";"  << "70012" << "," << std::to_string(config.costMineralToUpgradeToMythic)
            << ";" << "70013" << "," << std::to_string(config.costSkullUpgradeRankMythic) << ";" << "70014" << "," << std::to_string(config.costBetterSkullUpgradeRankMythic);


        elements.push_back(fmt.str());
    }
   
    return elements;
}


std::vector<std::string> RunesManager::LoadoutCachingForClient(Player* player)
{
    std::vector<std::string> elements = {};

    auto match = m_Loadout.find(player->GetGUID().GetCounter());

    if (match != m_Loadout.end()) {
        for (auto const& loadout : match->second) {
            std::stringstream fmt;
            fmt << loadout.id << ";" << loadout.title << ";" << loadout.active;
            elements.push_back(fmt.str());
        }
    }
    return elements;
}

std::vector<std::string> RunesManager::SlotsCachingForClient(Player* player)
{
    std::vector<std::string> elements = {};
    uint64 activeId = GetActiveLoadoutId(player);

    if (activeId <= 0)
        return elements;

    auto match = m_SlotRune.find(activeId);

    if (match != m_SlotRune.end())
        for (auto const& slot : match->second) {
            Rune rune = GetRuneBySpellId(slot.runeSpellId);
            std::stringstream fmt;
            fmt << slot.id << ";" << slot.runeSpellId << ";" << slot.order << ";" << std::to_string(rune.quality);
            elements.push_back(fmt.str());
        }

    return elements;
}


std::string RunesManager::ProgressionCachingForClient(Player* player)
{
    std::string value = "";
    auto progression = m_Progression.find(player->GetSession()->GetAccountId());

    if (progression != m_Progression.end()) {
        std::stringstream fmt;
        fmt << progression->second.unlockedLoadoutCount << ";" << progression->second.unlockedSlotRunes << ";" << config.maxSlots;
        value = fmt.str();
    }
    return value;
}

Rune RunesManager::GetRuneByQuality(uint32 groupId, int8 nextQuality)
{
    auto range = m_unorderedRunes.equal_range(groupId);
    Rune runeQuality;

    for (auto it = range.first; it != range.second; ++it) {
        const Rune& rune = it->second;
        if (rune.quality == nextQuality) {
            runeQuality = rune;
            break;
        }
    }

    return runeQuality;
}

bool RunesManager::IsRuneUpgradable(Player* player, Rune targetRune, uint32 countRune)
{
    int8 nextQuality = targetRune.quality;

    bool didHasRunes = countRune >= 3;

    if (nextQuality == UNCOMMON_QUALITY)
        return didHasRunes;

    if (nextQuality == RARE_QUALITY)
        return didHasRunes && player->HasItemCount(70009, 200);

    if (nextQuality == EPIC_QUALITY)
        return didHasRunes && player->HasItemCount(70009, 500) && player->HasItemCount(70010, 6);

    if (nextQuality == LEGENDARY_QUALITY)
        return didHasRunes && player->HasItemCount(70009, 1500) && player->HasItemCount(70011, 20) && player->HasItemCount(70013, 3) && player->HasItemCount(70014, 1);

    if (nextQuality == MYTHICAL_QUALITY)
        return didHasRunes && player->HasItemCount(70009, 2500) && player->HasItemCount(70012, 100) && player->HasItemCount(70013, 3) && player->HasItemCount(70014, 5);

    return false;
}


Rune RunesManager::GetRuneBySpellId(uint32 runeId)
{
    auto it = m_Runes.find(runeId);

    if (it != m_Runes.end())
        return it->second;

    return {};
}

bool RunesManager::KnowRuneId(Player* player, uint64 runeId)
{
    if (!player)
        return false;

    if (config.debug)
        return true;

    auto it = m_KnownRunes.find(player->GetSession()->GetAccountId());

    if (it != m_KnownRunes.end()) {
        auto match = std::find_if(it->second.begin(), it->second.end(), [runeId](const KnownRune& account) {
            return account.rune.spellId == runeId;
        });
        if (match != it->second.end())
            return true;
        else
            return false;
    }
    return false;
}

void RunesManager::RemoveSlotsOnCharacterDel(ObjectGuid guid)
{
    auto match = m_Loadout.find(guid.GetCounter());

    if (match != m_Loadout.end())
        for (auto const& loadout : match->second)
            if (loadout.active)
                CharacterDatabase.Query("DELETE FROM character_rune_slots WHERE id = {}", loadout.id);


    CharacterDatabase.Execute("DELETE FROM character_rune_loadout WHERE guid = {}", guid.GetCounter());
}

void RunesManager::GiveAchievementReward(Player* player, uint32 achievementId)
{
    auto match = m_RewardAchievement.find(achievementId);

    if (match != m_RewardAchievement.end())
    {
        if (match->second.itemId1 > 0)
            player->AddItem(match->second.itemId1, match->second.itemId1Amount);

        if (match->second.itemId2 > 0)
            player->AddItem(match->second.itemId2, match->second.itemId2Amount);

        if (match->second.itemId3 > 0)
            player->AddItem(match->second.itemId3, match->second.itemId3Amount);

        SendPlayerMessage(player, "Congratulations! You have completed an achievement, an award has been added to your bag!");
    }
}

void RunesManager::ApplyRunesOnLogin(Player* player)
{
    uint64 activeId = GetActiveLoadoutId(player);

    if (activeId <= 0)
        return;

    auto match = m_SlotRune.find(activeId);

    if (match != m_SlotRune.end())
        for (auto const& slot : match->second)
            if (!player->HasAura(slot.runeSpellId) && slot.id)
                player->AddAura(slot.runeSpellId, player);
}

bool RunesManager::RuneAlreadyActivated(Player* player, uint64 runeId)
{
    uint64 activeId = GetActiveLoadoutId(player);

    if (activeId <= 0)
        return false;

    auto match = m_SlotRune.find(activeId);

    if (match != m_SlotRune.end())
        for (auto const& slot : match->second)
            if (slot.runeSpellId == runeId)
                return true;

    return false;
}

uint64 RunesManager::GetActiveLoadoutId(Player* player)
{
    auto match = m_Loadout.find(player->GetGUID().GetCounter());

    if (match != m_Loadout.end())
        for (auto const& loadout: match->second)
            if (loadout.active)
                return loadout.id;

    return 0;
}

uint32 RunesManager::GetCoutSameGroupRune(Player* player, uint32 spellId)
{
    uint64 activeId = GetActiveLoadoutId(player);
    uint32 count = 0;

    if (activeId <= 0)
        return count;

    auto match = m_SlotRune.find(activeId);

    if (match != m_SlotRune.end()) {
        Rune rune = GetRuneBySpellId(spellId);
        count = std::count_if(match->second.begin(), match->second.end(),
            [&](const SlotRune& s)
        {
            Rune runeToCompare = GetRuneBySpellId(s.runeSpellId);
            return runeToCompare.groupId == rune.groupId;
        });
    }
    return count;
}

bool RunesManager::HasEnoughToUpgrade(Player* player, uint32 spellId)
{
    if (!player)
        return false;

    auto it = m_KnownRunes.find(player->GetSession()->GetAccountId());

    if (it != m_KnownRunes.end())
    {
        auto count = std::count_if(it->second.begin(), it->second.end(), [&](const KnownRune& account) {
            return account.rune.spellId == spellId;
        });
        return count >= 3;
    }

    return false;
}

uint32 RunesManager::GetCountActivatedRune(Player* player)
{
    uint64 activeId = GetActiveLoadoutId(player);
    uint32 count = 0;

    if (activeId <= 0)
        return count;

    auto match = m_SlotRune.find(activeId);

    if (match != m_SlotRune.end())
        count = match->second.size();

    return count;
}

void RunesManager::ActivateRune(Player* player, uint32 index, uint64 spellId)
{
    if (!player)
        return;

    if (player->isDead())
        return;

    if (!player->HasPlayerFlag(PLAYER_FLAGS_RESTING) && player->getLevel() > 10) {
        SendPlayerMessage(player, "You may only change your runes inside resting areas or while under level 10.");
        return;
    }

    bool knownRune = KnowRuneId(player, spellId);

    if (!knownRune) {
        SendPlayerMessage(player, "You do not know this rune.");
        return;
    }

    Rune rune = GetRuneBySpellId(spellId);

    if (!rune) {
        SendPlayerMessage(player, "This rune does not exist.");
        return;
    }

    if((rune.allowableClass & player->getClassMask()) == 0)
    {
        SendPlayerMessage(player, "You cannot activate this rune.");
        return;
    }
   
    auto progression = m_Progression.find(player->GetSession()->GetAccountId());

    if (GetCountActivatedRune(player) >= progression->second.unlockedSlotRunes)
    {
        SendPlayerMessage(player, "You have reached the maximum amount of runes.");
        return;
    }

    const uint32 count = GetCoutSameGroupRune(player, rune.spellId);
    bool tooMuchStack = count >= rune.maxStack;

    if (tooMuchStack)
    {
        SendPlayerMessage(player, "You cannot activate more of this rune.");
        return;
    }

    player->CastCustomSpell(79850, SPELLVALUE_BASE_POINT0, spellId, player, TRIGGERED_NONE);
}


void RunesManager::ResetAllSlots(Player* player)
{
    if (!player)
        return;

    uint64 activeId = GetActiveLoadoutId(player);

    if (activeId <= 0)
        return;

    CharacterDatabase.Query("DELETE FROM character_rune_slots WHERE id = {}", activeId);

    auto match = m_SlotRune.find(activeId);
    if (match != m_SlotRune.end()) {
        for (auto& slot : match->second) {
            player->RemoveAura(slot.runeSpellId);
        }
        match->second.clear();
    }

    sEluna->RefreshSlotsRune(player);
}

void RunesManager::ActivateLoadout(Player* player, uint64 newLoadoutId)
{
    uint32 activeId = GetActiveLoadoutId(player);

    if (activeId == newLoadoutId)
        return;

    auto match = m_Loadout.find(player->GetGUID().GetCounter());

    auto it = m_SlotRune.find(activeId);
    if (it != m_SlotRune.end()) {
        for (auto& slot : it->second) {
            player->RemoveAura(slot.runeSpellId);
            SpellConversion(slot.runeSpellId, player, false);
        }
    }

    auto ij = m_SlotRune.find(newLoadoutId);
    if (ij != m_SlotRune.end()) {
        for (auto& slot : ij->second) {
            player->AddAura(slot.runeSpellId, player);
            SpellConversion(slot.runeSpellId, player, true);
        }
    }

    if (match != m_Loadout.end())
        for (auto& loadout : match->second) {
            if (loadout.id == newLoadoutId)
                loadout.active = true;
            if (loadout.id == activeId)
                loadout.active = false;
        }

    sEluna->RefreshSlotsRune(player);
}

void RunesManager::DisableRune(Player* player, uint64 runeSpellId)
{
    if (!player)
        return;

    if (player->isDead())
        return;

    if (!player->HasPlayerFlag(PLAYER_FLAGS_RESTING) && player->getLevel() > 10) {
        SendPlayerMessage(player, "You may only change your runes inside resting areas or while under level 10.");
        return;
    }

    bool knownRune = KnowRuneId(player, runeSpellId);

    if (!knownRune) {
        SendPlayerMessage(player, "You do not know this rune.");
        return;
    }

    Rune rune = GetRuneBySpellId(runeSpellId);

    if (!rune) {
        SendPlayerMessage(player, "This rune does not exist.");
        return;
    }

    if (!RuneAlreadyActivated(player, runeSpellId))
    {
        SendPlayerMessage(player, "This rune is not activated.");
        return;
    }

    RemoveRuneFromSlots(player, rune);
}

void RunesManager::RefundRune(Player* player, uint32 runeSpellId)
{
    if (!player)
        return;

    if (player->isDead())
        return;

    bool knownRune = KnowRuneId(player, runeSpellId);

    if (!knownRune) {
        SendPlayerMessage(player, "You do not know this rune.");
        return;
    }

    Rune rune = GetRuneBySpellId(runeSpellId);

    if (!rune) {
        SendPlayerMessage(player, "This rune does not exist.");
        return;
    }

    int multiplier = pow(3, rune.quality - 1);
    int8 diminishing = 5 * (rune.quality - 1);
    uint8 runicDust = (50 - diminishing) * multiplier;

    uint32 accountId = player->GetSession()->GetAccountId();

    auto it = m_KnownRunes.find(accountId);

    auto ij = std::find_if(it->second.begin(), it->second.end(),
        [&](const KnownRune& accountRune) {
        return accountRune.rune.quality == rune.quality && accountRune.rune.spellId == rune.spellId;
    });

    if (ij == it->second.end())
        return;

    if (ij->count > 1)
        ij->count -= 1;
    else
        it->second.erase(ij);


    CharacterDatabase.Execute("DELETE FROM account_know_runes WHERE accountId = {} AND spellId = {} LIMIT 1", accountId, rune.spellId);

    player->AddItem(70008, runicDust);
    UpdateRunicDustAmount(player, runicDust);

    if (RuneAlreadyActivated(player, rune.spellId))
    {
        RemoveRuneFromSlots(player, rune);
        sEluna->RefreshSlotsRune(player);
        return;
    }
}


void RunesManager::UpdateRunicDustCountOnLogin(Player* player)
{
    uint32 accountId = player->GetSession()->GetAccountId();

    auto it = m_Progression.find(accountId);

    if (it != m_Progression.end()) {
        Item* item = player->GetItemByEntry(70008);

        if (!item)
        {
            player->AddItem(70008, it->second.dusts);
            return;
        }

        if (item->GetCount() != it->second.dusts)
            item->SetCount(it->second.dusts);
    }
}

void RunesManager::UpdateRunicDustAmount(Player* player, int32 amount)
{
    uint32 accountId = player->GetSession()->GetAccountId();

    auto it = m_Progression.find(accountId);

    if (it != m_Progression.end()) {

        if (amount > 1)
            it->second.dusts += amount;
        else
            it->second.dusts -= amount;

        CharacterDatabase.Execute("UPDATE character_rune_progression SET dusts = {} WHERE accountId = {}", it->second.dusts, accountId);
    }
}

void RunesManager::UpgradeRune(Player* player, uint32 runeSpellId)
{
    Rune rune = GetRuneBySpellId(runeSpellId);

    uint32 accountId = player->GetSession()->GetAccountId();

    auto it = m_KnownRunes.find(accountId);

    auto ij = std::find_if(it->second.begin(), it->second.end(),
        [&](const KnownRune& accountRune) {
        return accountRune.rune.quality == rune.quality && accountRune.rune.spellId == rune.spellId;
    });

    if (ij == it->second.end())
        return;

    if (RuneAlreadyActivated(player, runeSpellId))
    {
        SendPlayerMessage(player, "You cannot upgrade this rune, you have one or more of these runes activated.");
        return;
    }

    Rune nextRune = GetRuneByQuality(ij->rune.groupId, rune.quality + 1);
    bool isUpgradable = IsRuneUpgradable(player, nextRune, ij->count);

    if (!isUpgradable) {
        return SendPlayerMessage(player, "You do not meet the requirement to upgrade this rune.");
    }


    player->CastCustomSpell(79505, SPELLVALUE_BASE_POINT0, nextRune.spellId, player, TRIGGERED_NONE);

}

void RunesManager::AddRuneToSlot(Player* player, Rune rune)
{
    uint64 activeId = GetActiveLoadoutId(player);

    if (activeId <= 0)
    {
        LOG_ERROR("Error", "Could not find the Active Loadout.");
        return;
    }

    auto match = m_SlotRune.find(activeId);
    SlotRune slot = { activeId, rune.spellId, 1 };

    if (match != m_SlotRune.end()) {
        if (match->second.size() > 0) {
            slot.order = GetMissingSlotNumber(match->second, player);
        }
        match->second.push_back(slot);
    }
    else
        m_SlotRune[activeId].push_back(slot);


    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_RUNE_SLOT);
    stmt->SetData(0, slot.id);
    stmt->SetData(1, slot.runeSpellId);
    stmt->SetData(2, slot.order);
    trans->Append(stmt);
    CharacterDatabase.CommitTransaction(trans);
    player->AddAura(rune.spellId, player);
    sEluna->RefreshSlotsRune(player);
}


void RunesManager::RemoveRuneFromSlots(Player* player, Rune rune)
{
    uint64 activeId = GetActiveLoadoutId(player);

    if (activeId <= 0)
        return;

    auto match = m_SlotRune.find(activeId);

    if (match == m_SlotRune.end())
    {
        LOG_ERROR("Error", "Could not find the Slot rune.");
        return;
    }

    auto it = std::find_if(match->second.begin(),
        match->second.end(),
        [&]
    (const SlotRune& slot) -> bool { return slot.runeSpellId == rune.spellId; });

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_SLOT);
    stmt->SetData(0, uint32(it->order));
    stmt->SetData(1, uint32(activeId));
    CharacterDatabase.Execute(stmt);

    SpellConversion(rune.spellId, player, false);
    player->RemoveAura(rune.spellId);
    match->second.erase(it);

    sEluna->RefreshSlotsRune(player);
}

uint32 RunesManager::GetMissingSlotNumber(std::vector<SlotRune> slots, Player* p) {

    std::list<uint32> usedOrders;
    for (auto& slot : slots)
        usedOrders.push_back(slot.order);

    for (uint32 order = 1; order <= config.maxSlots; order++)
        if (std::find(usedOrders.begin(), usedOrders.end(), order) == usedOrders.end())
            return order;


    return 1;
}
