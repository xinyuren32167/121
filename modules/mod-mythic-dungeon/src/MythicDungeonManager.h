#pragma once
#include "Player/Player.h"
#include "DatabaseEnv.h"
#include "Group.h"


enum MythicTypeData {
    HIGHEST_SEASONAL_COMPLETION = 1,
    HIGHEST_WEEKLY_COMPLETION = 2,
    HIGHEST_ALL_TIME_COMPLETION = 3,
};

struct MythicDungeon {
    uint32 id;
    uint32 timeToComplete;
    uint32 totalEnemyForces;
    uint32 mapId;
    std::string name;
    float x;
    float y;
    float z;
    float o;
    bool enable;
    uint32 itemId;
    bool operator !()
    {
        return !id;
    }
};

struct MythicRewardDungeon {
    uint32 mapId;
    uint32 itemId;
    uint32 level;
};

struct MythicKey {
    uint32 dungeonId;
    uint32 level;
};

struct MythicKeyCompleted {
    uint32 mapId;
    uint32 level;
    uint8 timer; 
};

struct Affixe {
    uint32 spellId;
    uint32 level;
    bool applyOnPlayer;
    bool applyOnCreature;
    bool applyOnlyOnElits;
    bool applyOnlyOnBoss;
    bool isPeriodic;
    uint32 periodic;
    bool targetOnlyOnePlayer;
};

struct DungeonBoss {
    uint32 dungeonId;
    uint32 order;
    uint32 bossId;
};

struct MythicDungeonBoss {
    uint32 creatureId;
    bool alive;
};

struct MythicRun {
    uint32 dungeonId;
    uint32 level;
    uint32 timeToComplete;
    bool started;
    bool chestDecrapeted;
    bool done;
    uint32 elapsedTime;
    std::vector<MythicDungeonBoss> bosses;
    double enemyForces;
    uint32 deaths;
    int startTimer;
};

struct MythicPlayerDataCompletion {
    uint64 guid;
    uint32 mapId;
    uint32 level;
    uint8 timer;
    MythicTypeData type;
};

struct Config {
    std::time_t start;
    std::time_t end;
    uint8 season;
};

class MythicDungeonManager {
private:

    static Config m_Config;
    static bool IsCreatureNpc(Creature* creature);
    static bool MeetTheConditionsToCompleteTheDungeon(MythicRun* run);
    static std::map<ObjectGuid, MythicKey> m_DelayedCreationRun;
    static std::map<uint32, MythicDungeon> m_MythicDungeon;
    static std::map<uint32, MythicRun> m_MythicRun;
    static std::map<uint32, std::map<MythicTypeData, std::vector<MythicPlayerDataCompletion>>> m_MythicDungeonPlayerDataCompletion;
    static std::map<uint32, std::vector<DungeonBoss>> m_MythicDungeonBosses;
    static std::map<uint32, std::vector<MythicRewardDungeon>> m_MythicRewardDungeon;
    static std::map<uint32, uint32> m_ItemIdToDungeonId;
    static std::map<uint64, MythicKey> m_PlayerKey;
    static std::vector<Affixe> m_WeeklyAffixes;
    static uint32 GetHighestRunId();
    static void SaveRun(MythicRun* run, Player* player, uint32 increaseAmountKey, uint32 runId = 0);
    static void UpdateOrCreateMythicKey(MythicRun* run, Player* player, uint32 increaseAmountKey);

public:
    static MythicDungeon FindMythicDungeonByItsKeyItemId(uint32 itemId);
    static MythicDungeon GetMythicDungeonByDungeonId(uint32 mapId);
    static MythicKey GetCurrentMythicKey(Player* player);
    static double GetDamageMultiplicator(Map* map);
    static double GetHPMultiplicator(Map* map);
    static std::vector<std::string> GetDataMythicRun(Player* player);
    static std::vector<std::string> GetDungeonsEnabled(Player* player);
    static std::vector<std::string> GetWeeklyAffixes(Player* player);
    static uint32 GetMythicScore(Player* player);
    static void CompleteMythicDungeon(MythicRun* run, Player* player);
    static void CreateRun(Player* player, MythicKey key);
    static void HandleAffixes(Map* map);
    static void HandleChangeDungeonDifficulty(Player* _player, uint8 mode);
    static void InitHighestCompletedDungeonAllTime(Player* player);
    static void InitHighestCompletedDungeonThisSeason(Player* player);
    static void InitHighestCompletedDungeonThisWeek(Player* player);
    static void InitializeConfig();
    static void InitializeMythicDungeonBosses();
    static void InitializeMythicDungeons();
    static void InitializeMythicKeys();
    static void InitializeRewardsDungeons();
    static void InitializeWeeklyAffixes();
    static void OnKillBoss(Player* player, Creature* killed);
    static void OnKillMinion(Player* player, Creature* killed);
    static void OnMapChanged(Player* player);
    static void OnPlayerKilledByCreature(Creature* killer, Player* killed);
    static void OnPlayerRelease(Player* player);
    static void ReactivateAllGameObject(Map* map);
    static void SendPreperationMythicDungeon(Player* player);
    static void StartMythicDungeon(Player* player, uint32 level);
    static void Update(Map* map, uint32 diff);
};
