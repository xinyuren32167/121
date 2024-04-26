/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <list>
#include <map>
#include <mutex>
#include <unordered_map>

#include "Define.h"
#include "DatabaseEnvFwd.h"
#include "DBCEnums.h"
#include "ObjectDefines.h"
#include <ObjectGuid.h>
#include "Mythic.h"
#include "Player.h"
#include "Group.h"
#include "Map.h"
#include "Mythic.h"
#include "Item.h"

#ifndef _MYTHICMGR_H
#define _MYTHICMGR_H

struct MythicDungeon {
    uint32 id;
    uint32 amountToKill;
    uint32 timeToComplete;
    uint32 mapId;
    float x;
    float y;
    float z;
    float o;
    uint32 itemId;
    explicit operator bool() const
    {
        return id;
    }
};

struct MythicMultiplier {
    float hp;
    float damage;
};

struct MythicBoss {
    uint32 dungeonId;
    uint32 order;
    uint32 creatureId;
};

struct MythicKey {
    uint32 dungeonId;
    uint32 level;
};

struct MythicRewardItem {
    uint32 itemId;
    uint32 minLevel;
    uint32 maxLevel;
    uint32 classId;
    uint32 subClassId;
    uint32 statType1;
};

struct MythicRewardToken {
    uint32 level;
    uint32 runicEssence;
    uint32 runicDust;
    uint32 tokenId;
    uint32 tokenCount;
};

class MythicManager
{
private:
    MythicManager() { };
    ~MythicManager();
public:


    static MythicManager* instance();
    typedef std::map<uint32, std::unique_ptr<Mythic>> RunMythicDungeonContainer;
    typedef std::map<uint32, MythicMultiplier> MythicMutiplierContainer;
    typedef std::map<uint32, float> MythicKillCounterContainer;
    typedef std::map<uint64, std::vector<uint32>> MythicBagRewardPlayerContainer;
    typedef std::map<uint32, std::vector<MythicBoss>> MythicDungeonBossContainer;
    typedef std::vector<MythicDungeon> MythicDungeonContainer;
    typedef std::map<uint32, MythicRewardToken> MythicRewardsTokenContainer;
    typedef std::vector<MythicRewardItem> MythicRewardsItemContainer;
    typedef std::map<uint64, MythicKey> MythicPlayerKeyContainer;
    typedef std::map<ObjectGuid, MythicKey*> MythicAsyncCreationContainer;


    void InitializeMythicDungeonBosses();
    void InitializeMythicDungeons();
    void InitializePlayerMythicKeys();
    void InitializeMultipliers();
    void InitializeRewardsToken();
    void InitializeRewardsItems();
    void HandleChangeDungeonDifficulty(Player* _player, uint8 mode);
    void SaveMythicKey(Player* player, uint32 newDungeonId, uint32 level);
    void GenerateFirstRandomMythicKey(Player* player);

    void UpdatePlayerKey(Player* player, int8 upgrade);
    bool ForceCompleteMythic(Player* player);

    uint32 GetRandomMythicDungeonForPlayer(Player* player);
    uint32 GetItemIdWithDungeonId(uint32 dungeonId);
    uint32 GetEnchantByMythicLevel(uint32 level);

    MythicRewardToken GetMythicRewardTokenByLevel(uint32 level);
    uint32 GetMythicRewardItemByLevel(Player* player, uint32 level);

    bool IsItemAllowableClass(uint32 classPlayer, uint32 classId, uint32 subClassId);
    bool IsStatTypeAllowableSpec(uint32 currentSpec, uint32 statType);

    void Update(uint32 diff);

    void AddMythicDungeon(uint32 instanceId, Mythic* m);
    void RemoveMythic(uint32 instanceId);
    bool IsThisMapIdAvailableForMythic(uint32 mapId);
    bool IsPlayerMeetingConditionsToStartMythic(Player* player);
    Mythic* GetMythicPlayer(Player* player);
    Mythic* GetMythicInMap(Map* map);

    uint32 GetAmountToKillByDungeonId(uint32 dungeonId);
    MythicMultiplier GetMultplierByLevel(uint32 level);
    void GetMythicDungeonByDungeonId(uint32 dungeonId, MythicDungeon& dungeon);
    void FindMythicDungeonByItsKeyItemId(uint32 itemId, MythicDungeon& dungeon);
    MythicKey* GetCurrentPlayerMythicKey(Player* player);
    uint32 GetTimeToCompleteByDungeonId(uint32 dungeonId);
    std::vector<MythicBossState> GetMythicBossesByDungeonId(uint32 dungeonId);

    // Communication
    std::vector<std::string> GetDataMythicRun(Player* player);
    std::vector<std::string> GetDataMythicBag(Item* item);

    uint32 GetDungeonKeyLevelPreperation(Player* player);
    void PreparationMythicDungeon(Player* player);
    void ResetPlayerInstanceBound(Player* player, uint32 mapId);
    void ListenCreationMythicOnMapChanged(Player* player);
    bool ShouldShowMythicUI(Player* player);
    void OnKill(Player* player, Creature* killed);
    void OnPlayerDie(Player* player, Creature* killed);

    void Update(Creature* creature);

private:

    RunMythicDungeonContainer MythicStore;
    MythicMutiplierContainer MythicMultiplierStore;
    MythicKillCounterContainer MythicKillCounterStore;
    MythicDungeonBossContainer MythicDungeonBossStore;
    MythicDungeonContainer MythicDungeonStore;
    MythicPlayerKeyContainer MythicPlayerKeyStore;
    MythicAsyncCreationContainer AsyncCreationMythic;
    MythicRewardsTokenContainer MythicDungeonRewardTokenStore;
    MythicRewardsItemContainer MythicDungeonRewardItemsStore;
};

#define sMythicMgr MythicManager::instance()
#endif
