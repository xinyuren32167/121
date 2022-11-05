#include "RunesManager.h"
#include "boost/bind.hpp"

void RunesManager::LoadAllRunes()
{
}

void RunesManager::LoadAccountsRunes()
{

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

RuneMessage RunesManager::LearnRandomRune(Player* player)
{
    return RuneMessage();
}

RuneMessage RunesManager::LearnSpecificRune(Player* player, uint32 spellId)
{
    return RuneMessage();
}

RuneMessage RunesManager::UpgradeRune(Player* player, uint32 spellId)
{
    return RuneMessage();
}

RuneMessage RunesManager::RefundRune(Player* player, uint32 spellId)
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

RuneMessage RunesManager::ActivateLoadout(Player* player, uint8 slotId)
{
    return RuneMessage();
}

std::string RunesManager::GetAccountProgressionCachingForClient(Player* player, uint32 spellId)
{
    return std::string();
}

std::string RunesManager::GetLearningRuneForClient(Player* player, uint32 spellId)
{
    return std::string();
}

std::vector<std::string> RunesManager::GetCollectionCachingForClient(Player* player)
{
    return std::vector<std::string>();
}

std::vector<std::string> RunesManager::GetSlotRunesCachingForClient(Player* player, uint8 slotId)
{
    return std::vector<std::string>();
}

std::vector<std::string> RunesManager::GetLoadoutCachingForClient(Player* player)
{
    return std::vector<std::string>();
}

void RunesManager::ProcessSpellFromRune(Player* player, uint32 spellId, bool unlearnRunes)
{
    auto it = std::find_if(m_SpellRune.begin(), m_SpellRune.end(), [spellId](const SpellRunes& e)
    { return e.runeSpellId == spellId; });

    if (it == std::end(m_SpellRune))
        return;

    uint32 newSpellId = it->newSpellId;
    uint32 oldSpellId = it->oldSpellId;

    if (unlearnRunes) {
        if (oldSpellId)
            player->learnSpell(oldSpellId, false, false, false);

        if (newSpellId)
            player->removeSpell(newSpellId, SPEC_MASK_ALL, false, false);
    }
    else {
        if (newSpellId)
            player->learnSpell(newSpellId, false, false, false);

        if (oldSpellId)
            player->removeSpell(oldSpellId, SPEC_MASK_ALL, false, false);
    }
}


uint32 RunesManager::GetNextRankSpellId(uint32 spellId)
{
    return uint32();
}
