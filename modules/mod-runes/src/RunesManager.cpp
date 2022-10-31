#include "RunesManager.h"

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

void RunesManager::LoadAllSpellsSwitch()
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

uint32 RunesManager::GetNextRankSpellId(uint32 spellId)
{
    return uint32();
}
