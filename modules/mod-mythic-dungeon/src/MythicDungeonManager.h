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
    uint32 mapId;
    uint32 timeToComplete;
    uint32 totalEnemyForces;
    bool enable;
    bool operator !()
    {
        return !mapId;
    }
};

struct MythicRewardDungeon {
    uint32 mapId;
    uint32 itemId;
    uint32 level;
};

struct MythicKey {
    uint32 mapId;
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
    uint32 mapId;
    uint32 order;
    uint32 bossId;
};

struct MythicDungeonBoss {
    uint32 creatureId;
    bool alive;
};

struct MythicRun {
    uint32 mapId;
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

    static std::map<uint32, std::vector<DungeonBoss>> m_MythicDungeonBosses;
    static std::map<uint32, MythicDungeon> m_MythicDungeon;
    static std::map<uint32, std::map<MythicTypeData, std::vector<MythicPlayerDataCompletion>>> m_MythicDungeonPlayerDataCompletion;
    static std::map<uint32, std::vector<MythicRewardDungeon>> m_MythicRewardDungeon;
    static std::map<uint64, MythicKey> m_PlayerKey;
    static std::vector<Affixe> m_WeeklyAffixes;
    static std::map<uint32, MythicRun> m_MythicRun;
    static Config m_Config;
    static uint32 GetHighestRunId();
    static void SaveRun(MythicRun* run, Player* player, uint32 increaseAmountKey, uint32 runId = 0);
    static bool MeetTheConditionsToCompleteTheDungeon(MythicRun* run);
    static bool IsCreatureNpc(Creature* creature);
    static void UpdateOrCreateMythicKey(MythicRun* run, Player* player, uint32 increaseAmountKey);
public:
    static void InitializeMythicKeys();
    static void InitializeMythicDungeonBosses();
    static void InitializeMythicDungeons();
    static void InitializeRewardsDungeons();
    static void InitializeWeeklyAffixes();
    static void InitializeConfig();
    static void ApplyAffixesAndOtherUpgrade(Creature* creature, Map* map);
    static void HandleAffixes(Map* map);
    static void Update(Map* map, uint32 diff);
    static void HandleChangeDungeonDifficulty(Player* _player, uint8 mode);
    static void StartMythicDungeon(Player* player, uint32 keyId, uint32 level);
    static void OnKillBoss(Player* player, Creature* killed);
    static void OnKillMinion(Player* player, Creature* killed);
    static void OnPlayerKilledByCreature(Creature* killer, Player* killed);
    static void CompleteMythicDungeon(MythicRun* run, Player* player);
    static void OnPlayerRelease(Player* player);
    static MythicDungeon GetMythicDungeonByMapId(uint32 mapId);
    static std::vector<std::string> GetDataMythicRun(Player* player);
    static MythicKey GetCurrentMythicKey(Player* player);
    static uint32 GetMythicScore(Player* player);
    static void InitHighestCompletedDungeonThisWeek(Player* player);
    static void InitHighestCompletedDungeonThisSeason(Player* player);
    static void InitHighestCompletedDungeonAllTime(Player* player);
    static std::vector<std::string> GetWeeklyAffixes(Player* player);
    static std::vector<std::string> GetDungeonsEnabled(Player* player);
};
