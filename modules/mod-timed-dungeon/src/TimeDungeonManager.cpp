#include "TimeDungeonManager.h"
#include "Group.h"

std::map<uint32, TimedDungeon> TimedDungeonManager::m_TimedDungeon = {};
std::map<uint32, std::vector<TimedRewardDungeon>> TimedDungeonManager::m_TimedRewardDungeon = {};
std::map<uint32, std::vector<DungeonBoss>> TimedDungeonManager::m_TimedDungeonBosses = {};
std::map<uint64, MythicKey> TimedDungeonManager::m_PlayerKey = {};
std::vector<Affixe> TimedDungeonManager::m_WeeklyAffixes = {};
std::map<uint32, TimedRun> TimedDungeonManager::m_TimedRun = {};

void TimedDungeonManager::InitializeMythicKeys()
{

    m_PlayerKey = {};
    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_mythic_key");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint64 guid = fields[0].Get<uint64>();
        uint32 mapId = fields[2].Get<uint32>();
        uint32 level = fields[3].Get<uint32>();
        bool enable = fields[4].Get<bool>();
        MythicKey dungeon = { mapId, level };
        m_PlayerKey[guid] = dungeon;
    } while (result->NextRow());
}

void TimedDungeonManager::InitializeWeeklyAffixes()
{
    m_WeeklyAffixes = {};
    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic_affixes");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 spellId = fields[0].Get<uint32>();
        uint32 level = fields[1].Get<uint32>();
        Affixe affixe = { spellId, level };
        m_WeeklyAffixes.push_back(affixe);
    } while (result->NextRow());
}

void TimedDungeonManager::InitializeTimedDungeons()
{
    m_TimedDungeonBosses = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic_bosses ORDER by `order` ASC");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        uint32 order = fields[1].Get<uint32>();
        uint32 bossId = fields[2].Get<uint32>();
        DungeonBoss boss = { mapId, order, bossId };
        m_TimedDungeonBosses[mapId].push_back(boss);
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

void TimedDungeonManager::InitializeTimedDungeonBosses()
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

std::vector<std::string> TimedDungeonManager::GetDungeonsEnabled(Player* player)
{
    std::vector<std::string> elements = {};

    for (auto const& dungeon : m_TimedDungeon)
    {
        if (dungeon.second.enable) {
            std::string fmt =
                std::to_string(dungeon.second.mapId)
                + ";" + std::to_string(dungeon.second.timeToComplete)
                + ";" + std::to_string(dungeon.second.totalCreatureToKill);
            elements.push_back(fmt);
        }
    }
    return elements;
}

std::vector<std::string> TimedDungeonManager::GetWeeklyAffixes(Player* player)
{
    std::vector<std::string> elements = {};

    for (auto const& dungeon : m_WeeklyAffixes)
    {
        std::string fmt =
            std::to_string(dungeon.spellId)
            + ";" + std::to_string(dungeon.level);
        elements.push_back(fmt);
    }
    return elements;
}

void TimedDungeonManager::StartMythicDungeon(Player* player, uint32 keyId, uint32 level)
{
    Map* map = player->GetMap();

    if (!map)
        return;

    if (map->GetId() != keyId)
        return;

    if (player->GetDungeonDifficulty() != DUNGEON_DIFFICULTY_EPIC)
        return;

    Group* group = player->GetGroup();

    std::vector<TimedDungeonBoss> m_TimedDungeonBoss = {};
    std::vector<Affixe> affixes = {};
    std::vector<DungeonBoss> DungeonBosses = m_TimedDungeonBosses[keyId];
    TimedDungeon dungeon = m_TimedDungeon[keyId];

    for (auto const& timed : DungeonBosses)
        m_TimedDungeonBoss.push_back({ timed.bossId, true });

    uint32 totalDeath = 0;

    for (auto const& affixe : m_WeeklyAffixes)
        if (affixe.level <= level)
            affixes.push_back(affixe);

    TimedRun run = { keyId, level, dungeon.timeToComplete, 0, false, m_TimedDungeonBoss, 0, dungeon.totalCreatureToKill, totalDeath, affixes };
    m_TimedRun[map->GetInstanceId()] = run;

    if (group) {

        if (group->GetLeaderGUID() != player->GetGUID())
            return;

        if (group->GetMembersCount() <= 1)
            return;

        auto const& allyList = group->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* member = ObjectAccessor::FindPlayer(target.guid);
            if (member) {
                SendStatsMythicRun(member, run);
            }
        }

    }
    else {
        SendStatsMythicRun(player, run);
    }
}

std::vector<std::string> TimedDungeonManager::SendStatsMythicRun(Player* player, TimedRun run)
{
    std::vector<std::string> elements = {};

    std::string first =
        std::to_string(run.level)
        + ";" + std::to_string(run.enemyForces)
        + ";" + std::to_string(run.totalEnemyForces)
        + ";" + std::to_string(run.timeToComplete)
        + ";" + std::to_string(run.elapsedTime)
        + ";" + std::to_string(run.deaths);

    elements.push_back(first);

    std::string affixeStr = "";
    for (auto const& affixe : run.affixes)
        affixeStr += ";" + std::to_string(affixe.spellId);

    std::string bossStatus = "";
    for (auto const& boss : run.bosses) {
        CreatureTemplate const* creatureTemplate = sObjectMgr->GetCreatureTemplate(boss.creatureId);
        affixeStr += ";" + creatureTemplate->Name + "+" + std::to_string(boss.alive);
    }

    elements.push_back(bossStatus);


    return elements;
}

MythicKey TimedDungeonManager::GetCurrentMythicKey(Player* player)
{
    auto it = m_PlayerKey.find(player->GetGUID().GetCounter());

    if (it != m_PlayerKey.end())
        return it->second;

    return {};
}
