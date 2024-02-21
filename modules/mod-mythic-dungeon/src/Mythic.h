
#include <list>
#include <map>
#include <mutex>
#include <unordered_map>
#include "Group.h"
#include "Position.h"
#include "Map.h"

#ifndef AZEROTHCORE_MYTHIC_H
#define AZEROTHCORE_MYTHIC_H

struct MythicBossState {
    uint32 creatureId;
    bool alive;
    uint32 index;
};

class Mythic {

public:
    Mythic(Player* keyOwner, uint32 dungeonId, uint32 level);
    ~Mythic();

    typedef std::vector<MythicBossState> StateBossMythicContainer;
    void Update(uint32 diff);
    uint32 GetLevel() { return Level; };
    float GetEnemyForces() { return EnemyForces; };
    uint32 GetTimeToComplete() { return TimeToComplete; };
    uint32 GetElapsedTime() { return ElapsedTime; };
    uint32 GetTotalDeaths() { return Deaths; };
    StateBossMythicContainer GetBosses() {
        return StateBossMythicStore;
    };
    void OnKillBoss(Player* player, Creature* killed);
    void OnKillCreature(Player* player, Creature* killed);
    void OnPlayerKilledByCreature();
    bool IsDungeonDone() { return Done; };
    bool IsDungeonStarted() { return Started; };
    bool IsAllowedTimeOver() { return !ChestDecrapeted; };
    void OnCompleteMythicDungeon(Player* player);
    uint32 GetDungeonMapId() { return Dungeon->GetId(); };

private:

    void OnPlayerRelease();
    bool MeetTheConditionsToCompleteTheDungeon();
    void GiveRewards();
    void SaveMythicDungeon();
    void SetBossDead(uint32 creatureId);
    uint32 GetBossIndex(uint32 creatureId);

    int8 CalculateUpgradeKey();

    void SetRespawnPosition(Position position) { LastestPosition = position; };

    bool IsDungeonNotStarted() { return !Started; };

    Position LastestPosition;
    Group* m_Group;
    Player* KeyOwner;
    uint32 DungeonId;
    uint32 TimeToComplete;
    Map* Dungeon;
    int StartTimer;
    uint32 Countdown;
    uint8 Iteration;
    bool Started;
    bool ChestDecrapeted;
    bool Done;
    uint32 ElapsedTime;
    float EnemyForces;
    uint32 Deaths;
    uint32 Level;
    StateBossMythicContainer StateBossMythicStore;
};
#endif
