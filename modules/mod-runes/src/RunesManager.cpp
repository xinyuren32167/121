#include "RunesManager.h"
#include "boost/bind.hpp"

std::map<uint32, Rune> RunesManager::m_Runes = {};
std::map<uint32, std::vector<RuneAccount>> RunesManager::m_accountRunes = {};

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
    QueryResult result = CharacterDatabase.Query("SELECT * FROM account_runes");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 accountId = fields[0].Get<uint32>();
        uint32 id = fields[1].Get<uint32>();
        uint32 runeId = fields[2].Get<uint32>();
        Rune rune = GetRuneById(runeId);
        RuneAccount runeAccount = { accountId, id, rune };
        m_accountRunes[accountId].push_back(runeAccount);
    } while (result->NextRow());
}

void RunesManager::LoadAllLoadout()
{

}

void RunesManager::LoadAllAccountProgression()
{

}

void RunesManager::LoadAllSpells()
{

}

void RunesManager::SavePlayer(Player* player)
{

}

void RunesManager::CreateSlotRunes(Player* player)
{

}

LearnRune RunesManager::LearnRandomRune(Player* player, uint8 quality)
{
    return LearnRune();
}

RuneMessage RunesManager::LearnSpecificRune(Player* player, uint32 spellId)
{
    return RuneMessage();
}

RuneMessage RunesManager::UpgradeRune(Player* player, uint32 spellId)
{
    return RuneMessage();
}

RuneMessage RunesManager::UnlockSlotRune(Player* player)
{
    return RuneMessage();
}

RuneMessage RunesManager::RefundRune(Player* player, uint32 spellId)
{
    return RuneMessage();
}

RuneMessage RunesManager::ConvertRuneToItem(Player* player, uint32 runeId)
{
    return RuneMessage();
}

RuneMessage RunesManager::ActivateRune(Player* player, uint32 spellId)
{
    return RuneMessage();
}

RuneMessage RunesManager::DeactivateRune(Player* player, uint32 spellId)
{
    return RuneMessage();
}

RuneMessage RunesManager::UpdateLoadout(Player* player, uint8 slotId)
{
    return RuneMessage();
}

RuneMessage RunesManager::ActivateLoadout(Player* player, uint8 slotId)
{
    return RuneMessage();
}

std::vector<std::string> RunesManager::AllRunesCachingForClient()
{
    std::vector<std::string > elements = {};
    for (auto it = m_Runes.begin(); it != m_Runes.end(); it++)
    {
        Rune rune = it->second;
        auto s = std::format("{}{}{}", rune.spellId, rune.quality);
    }
}

void RunesManager::ProcessSpellFromRune(Player* player, uint32 spellId, bool unlearnRunes)
{
}

uint32 RunesManager::GetNextRankSpellId(uint32 spellId)
{
    return uint32();
}

Rune RunesManager::GetRuneById(uint32 runeId)
{
    auto it = m_Runes.find(runeId);

    if (it != m_Runes.end())
        return it->second;

    return {};
}
