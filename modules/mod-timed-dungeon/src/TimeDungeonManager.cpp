#include "TimeDungeonManager.h"
#include "Group.h"

std::vector<PlayerTimedDungeon>TimedDungeonManager::m_PlayersTimedDungeon = {};
std::vector<TimedDungeon> TimedDungeonManager::m_TimedDungeon;
std::map<uint32, std::vector<TimedRewardDungeon>> TimedDungeonManager::m_TimedRewardDungeon = {};
std::map<uint64, std::vector<PlayerTimedDungeon>> TimedDungeonManager::m_PlayerTimedDungeon = {};

void TimedDungeonManager::InitializeTimedDungeons()
{
    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        uint32 timeToComplete = fields[1].Get<uint32>();
        bool enable = fields[2].Get<bool>();
        TimedDungeon dungeon = { mapId, timeToComplete, enable };
        m_TimedDungeon.push_back(dungeon);
    } while (result->NextRow());
}

void TimedDungeonManager::InitializeRewardsDungeons()
{
    m_TimedRewardDungeon = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic_rewards");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        uint32 itemId = fields[1].Get<uint32>();
        uint32 level = fields[2].Get<uint32>();
        TimedRewardDungeon reward = { mapId, itemId, level };
        m_TimedRewardDungeon[mapId].push_back(reward);
    } while (result->NextRow());
}

void TimedDungeonManager::InitializePlayersTimedDungeons()
{
    QueryResult result = WorldDatabase.Query("SELECT guid, mapId, level, (TIMESTAMPDIFF(SECOND, startDate, endDate) * 1000) as diff, leave, season FROM mythic_group_members JOIN mythic_group ON mythic_group_members.groupId = id");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 guid = fields[0].Get<uint32>();
        uint32 mapId = fields[1].Get<uint32>();
        uint32 level = fields[2].Get<uint32>();
        uint32 diff = fields[3].Get<uint32>();
        bool leave = fields[4].Get<bool>();
        uint32 season = fields[5].Get<uint32>();
        PlayerTimedDungeon playerTimedDungeon = { guid, mapId, level, diff, season, leave };
        m_PlayersTimedDungeon.push_back(playerTimedDungeon);
    } while (result->NextRow());

}


void TimedDungeonManager::InitializePlayerTimedDungeons()
{
    for (auto const& timed : m_PlayersTimedDungeon)
        if (!timed.leave) {
            m_PlayerTimedDungeon[timed.guid].push_back(timed);
        }

}

void TimedDungeonManager::HandleChangeDungeonDifficulty(Player* _player, uint8 mode)
{
    Group* group = _player->GetGroup();
    if (group)
    {
        if (group->IsLeader(_player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
            {
                Player* groupGuy = itr->GetSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInWorld())
                {
                    _player->SendDungeonDifficulty(group != nullptr);
                    return;
                }

                if (groupGuy->GetGUID() == _player->GetGUID() ? groupGuy->GetMap()->IsDungeon() : groupGuy->GetMap()->IsNonRaidDungeon())
                {
                    _player->SendDungeonDifficulty(group != nullptr);
                    return;
                }
            }

            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false, _player);
            group->SetDungeonDifficulty(Difficulty(mode));
        }
    }
    else {
        _player->SetDungeonDifficulty(Difficulty(mode));
        _player->SendDungeonDifficulty(false, DUNGEON_DIFFICULTY_EPIC);

        Player::ResetInstances(_player->GetGUID(), INSTANCE_RESET_CHANGE_DIFFICULTY, false);
    }

}

uint32 TimedDungeonManager::GetHighestCompletedLevelByDungeonMapId(Player* player, uint32 mapId)
{
    return uint32();
}

std::vector<std::string> TimedDungeonManager::GetCompletedDungeons(Player* player)
{
    return std::vector<std::string>();
}

std::vector<std::string> TimedDungeonManager::GetDungeonsEnabled(Player* player)
{
    return std::vector<std::string>();
}

void TimedDungeonManager::StartMythicDungeon(Player* player, uint32 keyId, uint32 level)
{
    uint32 highestDifficultyCleared = GetHighestCompletedLevelByDungeonMapId(player, keyId);

    if (highestDifficultyCleared < level)
        return;

    Group* group = player->GetGroup();

    if (group) {

        if (group->GetMembersCount() <= 1)
            return;

        auto const& allyList = group->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* member = ObjectAccessor::FindPlayer(target.guid);
            if (member) {

            }
        }

    }
    else {

    }
}
