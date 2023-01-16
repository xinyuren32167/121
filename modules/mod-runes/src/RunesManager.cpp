#include "RunesManager.h"
#include "boost/bind.hpp"
#include "Config.h"
#include "ElunaIncludes.h"
#include "LuaEngine.h"
#include "boost/iterator/counting_iterator.hpp"

std::map<uint32 /* SpellId */, Rune> RunesManager::m_Runes = {};
std::map<uint32 /* accountId */, std::vector<KnownRune>> RunesManager::m_KnownRunes = {};
std::map<uint64 /* guid */, std::vector<Loadout>> RunesManager::m_Loadout = {};
std::map<uint64 /* LoadoutId */, std::vector<SlotRune>> RunesManager::m_SlotRune = {};
std::map<uint32 /* accountId */, AccountProgression> RunesManager::m_Progression = {};
RuneConfig RunesManager::config = {};

void RunesManager::SetupConfig()
{
    config.enabled = sConfigMgr->GetOption<bool>("RuneManager.enabled", true);
    config.debug = true;
    config.maxSlots  = sConfigMgr->GetOption<uint32>("RuneManager.maxSlots", 10);
    config.defaultSlot = sConfigMgr->GetOption<uint32>("RuneManager.defaultSlot", 8);
    config.chanceDropRuneQualityWhite = sConfigMgr->GetOption<float>("RuneManager.chanceDropRuneQualityWhite", 90.0f);
    config.chanceDropRuneQualityGreen = sConfigMgr->GetOption<float>("RuneManager.chanceDropRuneQualityGreen", 10.0f);
    config.chanceDropRuneQualityBlue = sConfigMgr->GetOption<float>("RuneManager.chanceDropRuneQualityBlue", 0.2f);
    config.chanceDropRuneQualityEpic = sConfigMgr->GetOption<float>("RuneManager.chanceDropRuneQualityEpic", 0);
    config.chanceDropRuneQualityLegendary = sConfigMgr->GetOption<float>("RuneManager.chanceDropRuneQualityLegendary", 0);
    config.chanceDropRuneQualityRed = sConfigMgr->GetOption<float>("RuneManager.chanceDropRuneQualityRed", 0);
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
        KnownRune knowRune = { accountId, id, rune };
        m_KnownRunes[accountId].push_back(knowRune);
    } while (result->NextRow());
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
        uint64 runeId = fields[1].Get<uint64>();
        uint64 runeSpellId = fields[2].Get<uint64>();
        uint32 order = fields[3].Get<uint32>();
        SlotRune slot = { loadoutId, runeId, runeSpellId, order };
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
    }

    uint64 guid = player->GetGUID().GetCounter();
    uint32 accountId = player->GetSession()->GetAccountId();
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_FIRST_LOADOUT);
    stmt->SetData(0, guid);
    stmt->SetData(1, startIdSlot);
    stmt->SetData(2, slotDefault);
    stmt->SetData(3, active);

    CharacterDatabase.Execute(stmt);

    Loadout loadout = { guid, startIdSlot, slotDefault, active };
    m_Loadout[guid].push_back(loadout);
}

std::vector<std::string> RunesManager::RunesForClient(Player* player)
{
    std::vector<uint32> runeIds = {};
    std::vector<std::string > elements = {};
    auto known = m_KnownRunes.find(player->GetSession()->GetAccountId());
    auto pushRune = [&](Rune rune, uint32 id, bool kwown, bool activable, bool activated) {
        std::string fmt = 
        std::to_string(rune.spellId)
            + ";" +
            std::to_string(rune.quality)
            + ";" +
            std::to_string(rune.maxStack)
            + ";" +
                std::to_string(rune.refundItemId)
            + ";" +
                std::to_string(rune.refundDusts)
            + ";" +
            rune.keywords
            + ";" +
            std::to_string(config.debug ? true : kwown)
            + ";" +
            std::to_string(config.debug ? rune.spellId : id)
            + ";" +
            std::to_string(config.debug ? (rune.allowableClass & player->getClassMask()) != 0 : activable)
            + ";" +
                std::to_string(activated)
            + ";" + std::to_string(rune.allowableClass);
        elements.push_back(fmt);
    };

    if (known != m_KnownRunes.end())
        for (auto const& kwownRune : known->second) {
            runeIds.push_back(kwownRune.rune.spellId);
            Rune rune = GetRuneBySpellId(kwownRune.rune.spellId);
            const uint32 count = GetCoutSameGroupRune(player, rune.spellId);
            bool haveLessThanMaxStack = count < rune.maxStack;
            bool activable = bool(!RuneAlreadyActivated(player, kwownRune.id) && haveLessThanMaxStack &&
                (GetCountActivatedRune(player) < config.maxSlots));
            bool activated = RuneAlreadyActivated(player, kwownRune.id);
            pushRune(rune, kwownRune.id, true, activable, activated);
        }

    for (auto it = m_Runes.begin(); it != m_Runes.end(); it++)
    {
        Rune rune = it->second;
        if (std::find(runeIds.begin(), runeIds.end(), rune.spellId) == runeIds.end()) {
            pushRune(rune, 0, false, false, false);
        }
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
            fmt << slot.id << ";" << slot.runeId << ";" << slot.runeSpellId << ";" << slot.order << ";" << std::to_string(rune.quality);
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
        fmt << progression->second.dusts << ";" << progression->second.unlockedLoadoutCount << ";" << progression->second.unlockedSlotRunes << ";" << config.maxSlots;
        value = fmt.str();
    }
    return value;
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

    auto it = m_KnownRunes.find(player->GetSession()->GetAccountId());

    if (it != m_KnownRunes.end()) {
        auto match = std::find_if(it->second.begin(), it->second.end(), [runeId](const KnownRune& account) {
            return account.id == runeId;
        });
        if (match != it->second.end())
            return true;
        else
            return false;
    }
    return false;
}

Rune RunesManager::GetRuneById(Player* player, uint64 id)
{
    if (!player)
        return {};

    auto it = m_KnownRunes.find(player->GetSession()->GetAccountId());

    if (it != m_KnownRunes.end()) {
        auto match = std::find_if(it->second.begin(), it->second.end(), [id](const KnownRune& account) {
            return account.id == id;
        });
        if (match != it->second.end())
            return match->rune;
        else
            return {};
    }
    return {};
}

bool RunesManager::RuneAlreadyActivated(Player* player, uint64 runeId)
{
    uint64 activeId = GetActiveLoadoutId(player);

    if (activeId <= 0)
        return false;

    auto match = m_SlotRune.find(activeId);

    if (match != m_SlotRune.end())
        for (auto const& slot : match->second)
            if (slot.runeId == runeId)
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


void RunesManager::ActivateRune(Player* player, uint32 index, uint64 runeId)
{
    Rune rune = GetRuneById(player, runeId);

    if (config.debug)
        rune = GetRuneBySpellId(runeId);

    if (!rune) {
        sEluna->OnRuneMessage(player, "You don't have this rune.");
        return;
    }

    if((rune.allowableClass & player->getClassMask()) == 0)
    {
        sEluna->OnRuneMessage(player, "You can't activate this rune.");
        return;
    }

    if (RuneAlreadyActivated(player, runeId))
    {
        sEluna->OnRuneMessage(player, "This rune is already activated.");
        return;
    }

    auto progression = m_Progression.find(player->GetSession()->GetAccountId());

    if (GetCountActivatedRune(player) >= progression->second.unlockedSlotRunes)
    {
        sEluna->OnRuneMessage(player, "You have reach the maximum rune.");
        return;
    }

    const uint32 count = GetCoutSameGroupRune(player, rune.spellId);
    bool tooMuchStack = count >= rune.maxStack;

    if (tooMuchStack)
    {
        sEluna->OnRuneMessage(player, "You can't activate more of this rune.");
        return;
    }

    player->AddAura(rune.spellId, player);
    AddRuneToSlot(player, rune, runeId);
    sEluna->OnActivateRune(player, "Rune successfully activated!", index);
}

void RunesManager::DisableRune(Player* player, uint64 runeId)
{
    Rune rune = GetRuneById(player, runeId);

    if (config.debug)
        rune = GetRuneBySpellId(runeId);

    if (!rune)
    {
        sEluna->OnRuneMessage(player, "You don't have this rune.");
        return;
    }

    if (!RuneAlreadyActivated(player, runeId))
    {
        sEluna->OnRuneMessage(player, "This rune is not activated.");
        return;
    }

    RemoveRuneFromSlots(player, rune);
}

void RunesManager::RefundRune(Player* player, uint64 runeId)
{
    Rune rune = GetRuneById(player, runeId);

    if (!rune) {
        sEluna->OnRuneMessage(player, "You don't have this rune.");
        return;
    }

    RemoveRuneFromSlots(player, rune);
    player->AddItem(1000000, rune.refundDusts);
}

void RunesManager::UpgradeRune(Player* player, uint64 runeId)
{
    Rune rune = GetRuneById(player, runeId);

    if (!rune) {
        sEluna->OnRuneMessage(player, "You don't have this rune.");
        return;
    }

    if (!HasEnoughToUpgrade(player, rune.spellId)) {
        sEluna->OnRuneMessage(player, "You don't have enough of this similar rune to upgrade.");
        return;
    }
}

void RunesManager::AddRuneToSlot(Player* player, Rune rune, uint64 runeId)
{
    uint64 activeId = GetActiveLoadoutId(player);

    if (activeId <= 0)
        return;

    auto match = m_SlotRune.find(activeId);
    SlotRune slot = { activeId, runeId, rune.spellId, 1 };

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
    stmt->SetData(1, slot.runeId);
    stmt->SetData(2, slot.runeSpellId);
    stmt->SetData(3, slot.order);
    trans->Append(stmt);
    CharacterDatabase.CommitTransaction(trans);
    sEluna->RefreshSlotsRune(player);
}


void RunesManager::RemoveRuneFromSlots(Player* player, Rune rune)
{
    player->RemoveAura(rune.spellId);

    uint64 activeId = GetActiveLoadoutId(player);

    if (activeId <= 0)
        return;

    auto match = m_SlotRune.find(activeId);

    if (match == m_SlotRune.end())
        return;

    auto slotToFind = std::find_if(match->second.begin(), match->second.end(), [&](const SlotRune& slot) {
        return slot.runeSpellId == rune.spellId;
    });

    if (slotToFind == match->second.end())
        return;

    match->second.erase(slotToFind);

    CharacterDatabase.Query("DELETE FROM character_rune_slots WHERE runeId = {} AND id = {}", slotToFind->runeId, activeId);
    sEluna->RefreshSlotsRune(player);
}

uint32 RunesManager::GetMissingSlotNumber(std::vector<SlotRune> slots, Player* p) {

    auto progression = m_Progression.find(p->GetSession()->GetAccountId());

    std::set<uint32> usedOrders;
    for (auto& slot : slots) {
        usedOrders.insert(slot.order);
    }
    uint32 idx = 1;
    while (usedOrders.count(idx) != 0) {
        idx++;
    }
    return idx;
}
