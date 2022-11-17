#include "RunesManager.h"
#include "boost/bind.hpp"

void RunesManager::SetupConfig(Config config)
{
}

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

std::string RunesManager::GetAccountProgressionCachingForClient(Player* player, uint32 spellId)
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
  
}


uint32 RunesManager::GetNextRankSpellId(uint32 spellId)
{
    return uint32();
}
