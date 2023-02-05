#include "AutobalanceManager.h"
#include "PlayerSpecialization.h"

std::list<Player*> AutoBalanceManager::GetPlayersMap(Map* map)
{
    std::list<Player*> players = {};

    Map::PlayerList const& playerList = map->GetPlayers();
    if (!playerList.IsEmpty())
        for (Map::PlayerList::const_iterator playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
            if (Player* playerHandle = playerIteration->GetSource())
                players.push_back(playerHandle);

    return players;
}

Player* AutoBalanceManager::GetAlonePlayerMap(Map* map)
{
    std::list<Player*> players = GetPlayersMap(map);

    if (players.size() == 0)
        return nullptr;

    return players.front();
}

AutobalanceScalingInfo AutoBalanceManager::GetScalingInfo(Map* map, Creature* creature)
{

    if (!map->IsRaid() || map->IsDungeon())
        return {};

    bool isRaid = map->IsRaid();
    Difficulty difficulty = map->GetDifficulty();
    uint8 playerCount = map->GetPlayersCountExceptGMs();

    if (playerCount <= 1) {
        Player* player = GetAlonePlayerMap(map);

        if (!player)
            return {};

        uint8 CurrentSpecialization = PlayerSpecialization::GetCurrentSpecId(player);

        if (CurrentSpecialization <= 0)
            return {};

        auto match = m_ScalingPerSpecialization.find(CurrentSpecialization);

        if (match == m_ScalingPerSpecialization.end())
            return {};

        return match->second;
    }

    if (playerCount > 1 && isRaid) {
        // We check first if they are a speciifc scaling for a creatureId;
    }

    if (playerCount > 1 && !isRaid) {
        // We check first if they are a speciifc scaling for a creatureId;
    }
  
}
