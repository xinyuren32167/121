#include "PerksManager.h"

void PerksManager::LoadAllPerks()
{

}

void PerksManager::LoadAccountsPerks()
{
}

void PerksManager::LoadAllLoadout()
{
}

void PerksManager::LoadAllAccountProgression()
{
}

void PerksManager::SavePlayer(Player* player)
{

}

PerkMessage PerksManager::LearnRandomPerk(Player* player)
{
    return PerkMessage();
}

PerkMessage PerksManager::LearnSpecificPerk(Player* player, uint32 spellId)
{
    return PerkMessage();
}

PerkMessage PerksManager::UpgradePerk(Player* player, uint32 spellId)
{
    return PerkMessage();
}

PerkMessage PerksManager::RefundPerk(Player* player, uint32 spellId)
{
    return PerkMessage();
}

PerkMessage PerksManager::ActivatePerk(Player* player, uint32 spellId)
{
    return PerkMessage();
}

PerkMessage PerksManager::ActivateLoadout(Player* player, uint8 slotId)
{
    return PerkMessage();
}

std::string PerksManager::GetAccountProgressionCachingForClient(Player* player, uint32 spellId)
{
    return std::string();
}

std::string PerksManager::GetLearningPerkForClient(Player* player, uint32 spellId)
{
    return std::string();
}

std::vector<std::string> PerksManager::GetCollectionCachingForClient(Player* player)
{
    return std::vector<std::string>();
}

std::vector<std::string> PerksManager::GetSlotRunesCachingForClient(Player* player, uint8 slotId)
{
    return std::vector<std::string>();
}

std::vector<std::string> PerksManager::GetLoadoutCachingForClient(Player* player)
{
    return std::vector<std::string>();
}

uint32 PerksManager::GetNextRankSpellId(uint32 spellId)
{
    return m_Perks[spellId].nextRankSpellId;
}

