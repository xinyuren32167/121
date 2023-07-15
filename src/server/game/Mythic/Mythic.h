
#include <list>
#include <map>
#include <mutex>
#include <unordered_map>

#include "Group.h"

#ifndef AZEROTHCORE_MYTHIC_H
#define AZEROTHCORE_MYTHIC_H

class Mythic {

public:
    Mythic(Map* map, Group* group, uint32 dungeonId, uint32 level, Player* leader);
    ~Mythic();

    typedef std::map<uint32, bool> StateBossMythicContainer;
    void Prepare();
    void Update(uint32 diff);
    void PrepareCreature(Creature* creature);
    uint32 GetLevel() { return Level; };
    float GetEnemyForces() { return EnemyForces; };
    uint32 GetTimeToComplete() { return TimeToComplete; };
    uint32 GetElapsedTime() { return ElapsedTime; };
    uint32 GetTotalDeaths() { return Deaths; };
    StateBossMythicContainer GetBosses() { return StateBossMythicStore; };
private:

    void OnCompleteMythicDungeon(Player* player);
    void OnKillBoss(Player* player, Creature* killed);
    void OnKillCreature(Player* player, Creature* killed);
    void OnPlayerKilledByCreature();
    void OnPlayerRelease();
    bool MeetTheConditionsToCompleteTheDungeon();
    void GiveRewards();
    void UpdatePlayerKey(Player* player);
    void SaveMythicDungeon();
    void SetBossDead(uint32 creatureId);

    // Addon Messages;
    void SendStart(Player* player);
    void SendUpdateBossKill();
    void SendEnemyForces();

    int8 CalculateUpgradeKey();

    void SetRespawnPosition(Position position) { LastestPosition = position; };

    bool IsDungeonDone() { return Done; };
    bool IsDungeonNotStarted() { return !Started; };

    Position LastestPosition;
    Group* m_Group;
    Player* KeyOwner;
    uint32 DungeonId;
    uint32 TimeToComplete;
    Map* Dungeon;
    int StartTimer;
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
