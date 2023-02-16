#pragma once
#include "Player/Player.h"
#include "DatabaseEnv.h"
#include "Group.h"

struct TimedDungeon {
    uint32 mapId;
    uint32 timeToComplete;
    uint32 totalCreatureToKill;
    bool enable;
};

struct TimedRewardDungeon {
    uint32 mapId;
    uint32 itemId;
    uint32 level;
};

struct MythicKey {
    uint32 mapId;
    uint32 level;
};


struct Affixe {
    uint32 spellId;
    uint32 level;
};


struct DungeonBoss {
    uint32 mapId;
    uint32 order;
    uint32 bossId;
};

struct TimedDungeonBoss {
    uint32 creatureId;
    bool alive;
};

struct TimedRun {
    uint32 mapId;
    uint32 level;
    uint32 timeToComplete;
    bool started;
    uint32 elapsedTime;
    std::vector<TimedDungeonBoss> bosses;
    uint32 enemyForces;
    uint32 totalEnemyForces;
    uint32 deaths;
    std::vector<Affixe> affixes;
};


class TimedDungeonManager {
private:
    static std::map<uint32, std::vector<DungeonBoss>> m_TimedDungeonBosses;
    static std::map<uint32, TimedDungeon> m_TimedDungeon;
    static std::map<uint32, std::vector<TimedRewardDungeon>> m_TimedRewardDungeon;
    static std::map<uint64, MythicKey> m_PlayerKey;
    static std::vector<Affixe> m_WeeklyAffixes;
    static std::map<uint32, TimedRun> m_TimedRun;
public:
    static void InitializeMythicKeys();
    static void InitializeTimedDungeons();
    static void InitializeRewardsDungeons();
    static void InitializeTimedDungeonBosses();
    static void InitializeWeeklyAffixes();
    static void HandleChangeDungeonDifficulty(Player* _player, uint8 mode);
    static void StartMythicDungeon(Player* player, uint32 keyId, uint32 level);
    // Fired when you loggin or when you enter on a mythic dungeon or and when you start a dungeon.
    static std::vector<std::string> SendStatsMythicRun(Player* player, TimedRun run);
    static MythicKey GetCurrentMythicKey(Player* player);
    static std::vector<std::string> GetWeeklyAffixes(Player* player);
    static std::vector<std::string> GetDungeonsEnabled(Player* player);
    // static voAu nivid EndMythicDungeon(Player* player);
};
