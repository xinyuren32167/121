#pragma once
#include "Player/Player.h"
#include "DatabaseEnv.h"
#include "Group.h"

struct PlayerTimedDungeon {
    uint32 guid;
    uint32 mapId;
    uint32 level;
    uint32 diff;
    uint32 season;
    bool leave;
};

struct TimedDungeon {
    uint32 mapId;
    uint32 timeToComplete;
    bool enable;
};

struct TimedRewardDungeon {
    uint32 mapId;
    uint32 itemId;
    uint32 level;
};

struct RunPlayerTimedDungeon {
    uint32 mapId;
    uint32 completionTime;
    bool timed;
    uint8 updgrade;
    std::vector<uint32> affixeIds;
};


class TimedDungeonManager {
private:
    static std::vector<PlayerTimedDungeon> m_PlayersTimedDungeon;
    static std::vector<TimedDungeon> m_TimedDungeon;
    static std::map<uint32, std::vector<TimedRewardDungeon>> m_TimedRewardDungeon;
    /* ObjectGuid, Timed */
    static std::map<uint64, std::vector<PlayerTimedDungeon>> m_PlayerTimedDungeon;
public:
    static void InitializePlayersTimedDungeons();
    static void InitializePlayerTimedDungeons();
    static void InitializeTimedDungeons();
    static void InitializeRewardsDungeons();
    static void HandleChangeDungeonDifficulty(Player* _player, uint8 mode);
    static void StartMythicDungeon(Player* player, uint32 keyId, uint32 level);
    static uint32 GetHighestCompletedLevelByDungeonMapId(Player* player, uint32 mapId);
    static std::vector<std::string> GetCompletedDungeons(Player* player);
    static std::vector<std::string> GetDungeonsEnabled(Player* player);
    // static voAu nivid EndMythicDungeon(Player* player);
};
