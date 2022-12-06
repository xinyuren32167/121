#include "RunesManager.h"
#include "boost/bind.hpp"

std::map<uint32 /* SpellId */, Rune> RunesManager::m_Runes = {};
std::map<uint32 /* accountId */, std::vector<KnownRune>> RunesManager::m_KnownRunes = {};
std::map<uint64 /* guid */, std::vector<Loadout>> RunesManager::m_Loadout = {};
std::map<uint64 /* slotId */, std::vector<SlotRune>> RunesManager::m_SlotRune = {};

void RunesManager::SetupConfig(Config config)
{

}

void RunesManager::LoadAllRunes()
{
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
    QueryResult result = CharacterDatabase.Query("SELECT * FROM account_know_runes");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 accountId = fields[0].Get<uint32>();
        uint32 id = fields[1].Get<uint32>();
        uint32 runeId = fields[2].Get<uint32>();
        Rune rune = GetRuneById(runeId);
        KnownRune knowRune = { accountId, id, rune };
        m_KnownRunes[accountId].push_back(knowRune);
    } while (result->NextRow());
}

void RunesManager::LoadAllLoadout()
{
    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_rune_loadout");
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
    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_rune_slots");
    if (!result)
        return;
    do
    {
        Field* fields = result->Fetch();
        uint64 slotId = fields[0].Get<uint64>();
        uint64 runeId = fields[1].Get<uint64>();
        uint64 runeSpellId = fields[2].Get<uint64>();
        uint32 order = fields[3].Get<uint32>();
        SlotRune slot = { slotId, runeId, runeSpellId, order };
        m_SlotRune[slotId].push_back(slot);
    } while (result->NextRow());
}

void RunesManager::SavePlayer(Player* player)
{

}

void RunesManager::CreateDefaultCharacter(Player* player)
{

    CharacterDatabasePreparedStatement* stmt = nullptr;
    uint64 startIdSlot = 1;
    bool active = 1;
    std::string slotDefault = "Default";
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MAXID_LOADOUT);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        Field* fields = result->Fetch();
        startIdSlot = fields[0].Get<uint64>() + 1;
    }

    uint64 guid = player->GetGUID().GetCounter();
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_FIRST_LOADOUT);
    stmt->SetData(0, guid);
    stmt->SetData(1, startIdSlot);
    stmt->SetData(2, slotDefault);
    stmt->SetData(3, active);
    CharacterDatabase.Execute(stmt);

    Loadout loadout = { guid, startIdSlot, slotDefault, active };
    m_Loadout[guid].push_back(loadout);


}

std::vector<std::string> RunesManager::AllRunesCachingForClient(Player* player)
{
    std::vector<uint32> runeIds = {};
    std::vector<std::string > elements = {};
    auto known = m_KnownRunes.find(player->GetSession()->GetAccountId());

    auto pushRune = [&elements](Rune rune, uint32 id, bool kwown, bool activable) {
        auto runestring = std::format(
            "{};{};{};{};{};{};{};{};{}",
            rune.spellId,
            rune.quality,
            rune.maxStack,
            rune.refundItemId,
            rune.refundDusts,
            rune.keywords,
            kwown,
            id,
            activable
        );
        elements.push_back(runestring);
    };

    if (known != m_KnownRunes.end())
        for (auto const& kwownRune : known->second) {
            runeIds.push_back(kwownRune.rune.spellId);
            Rune rune = GetRuneById(kwownRune.rune.spellId);
            const uint32 count = GetCountActivatedRune(player, rune.spellId);
            bool haveLessThanMaxStack = count < rune.maxStack;
            bool activable = bool(RuneAlreadyActivated(player, kwownRune.id) || haveLessThanMaxStack);
            pushRune(rune, kwownRune.id, true, activable);
        }

    for (auto it = m_Runes.begin(); it != m_Runes.end(); it++)
    {
        Rune rune = it->second;
        if (std::find(runeIds.begin(), runeIds.end(), rune.spellId) == runeIds.end()) {
            pushRune(rune, 0, false, false);
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
            auto runestring = std::format(
                "{};{};{}",
                loadout.id,
                loadout.title,
                loadout.active
            );
            elements.push_back(runestring);
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

    auto match = m_SlotRune.find(player->GetGUID().GetCounter());

    if (match != m_SlotRune.end())
        for (auto const& slot : match->second) {
            Rune rune = GetRuneById(slot.runeSpellId);
            auto runestring = std::format(
                "{};{};{};{}",
                slot.id,
                slot.runeId,
                slot.runeSpellId,
                rune.quality
            );
            elements.push_back(runestring);
        }

    return elements;
}


Rune RunesManager::GetRuneById(uint32 runeId)
{
    auto it = m_Runes.find(runeId);

    if (it != m_Runes.end())
        return it->second;

    return {};
}

bool RunesManager::KnowRuneId(Player* player, uint32 runeId)
{
    if (!player)
        return false;

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

bool RunesManager::RuneAlreadyActivated(Player* player, uint64 runeId)
{
    uint64 activeId = GetActiveLoadoutId(player);

    if (activeId <= 0)
        return true;

    auto match = m_SlotRune.find(player->GetGUID().GetCounter());

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

uint32 RunesManager::GetCountActivatedRune(Player* player, uint32 spellId)
{
    uint64 activeId = GetActiveLoadoutId(player);
    uint32 count = 0;

    if (activeId <= 0)
        return count;

    auto match = m_SlotRune.find(activeId);

    if (match != m_SlotRune.end()) {
        count = std::count_if(match->second.begin(), match->second.end(),
            [&](const SlotRune& s) { return s.runeSpellId == spellId; });
    }
    return count;
}
