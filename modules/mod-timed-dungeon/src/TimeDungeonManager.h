#pragma once
#include "Player/Player.h"
#include "DatabaseEnv.h"
#include "Group.h"

struct MythicDungeon {
    uint32 mapId;
    uint32 timeToComplete;
    uint32 totalEnemyForces;
    bool enable;
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
    // 1 / 2 / 3
    uint8 timer; 
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
    float enemyForces;
    uint32 deaths;
    uint32 startTimer;
};


class MythicDungeonManager {
private:

    static std::map<uint32, std::vector<DungeonBoss>> m_MythicDungeonBosses;
    static std::map<uint32, MythicDungeon> m_MythicDungeon;
    static std::map<uint32, std::vector<MythicRewardDungeon>> m_MythicRewardDungeon;
    static std::map<uint64, MythicKey> m_PlayerKey;
    static std::vector<Affixe> m_WeeklyAffixes;
    static std::map<uint32, MythicRun> m_MythicRun;
    static uint32 GetHighestRunId();
    static void SaveRun(MythicRun* run, Player* player, uint32 increaseAmountKey, uint32 runId = 0);
    static bool MeetTheConditionsToCompleteTheDungeon(MythicRun* run);
    static void UpdateOrCreateMythicKey(MythicRun* run, Player* player, uint32 increaseAmountKey);
public:
    static void InitializeMythicKeys();
    static void InitializeMythicDungeonBosses();
    static void InitializeMythicDungeons();
    static void InitializeRewardsDungeons();
    static void InitializeWeeklyAffixes();
    static void Update(Map* map, uint32 diff);
    static void HandleChangeDungeonDifficulty(Player* _player, uint8 mode);
    static void StartMythicDungeon(Player* player, uint32 keyId, uint32 level);
    static void OnKillBoss(Player* player, Creature* killed);
    static void OnKillMinion(Player* player, Creature* killed);
    static void OnPlayerKilledByCreature(Creature* killer, Player* killed);
    static void CompleteMythicDungeon(MythicRun* run, Player* player);
    static void OnPlayerRelease(Player* player);
    // Fired when you loggin or when you enter on a mythic dungeon or and when you start a dungeon.
    static std::vector<std::string> GetDataMythicRun(Player* player);
    static MythicKey GetCurrentMythicKey(Player* player);
    static std::vector<std::string> GetHighestCompletedDungeonThisWeek(Player* player);
    static std::vector<std::string> GetHighestCompletedDungeonAllTime(Player* player);
    static std::vector<std::string> GetWeeklyAffixes(Player* player);
    static std::vector<std::string> GetDungeonBosses(Player* player);
    static std::vector<std::string> GetDungeonsEnabled(Player* player);
    // static voAu nivid EndMythicDungeon(Player* player);
};
