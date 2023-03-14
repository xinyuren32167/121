/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "RunesManager.h"
#include "LuaEngine.h"
#include "Spell.h"
#include "MythicDungeonManager.h"
#include <boost/algorithm/string.hpp>

 // Add player scripts
class MythicDungeon_PlayerScripts : public PlayerScript
{
public:
    MythicDungeon_PlayerScripts() : PlayerScript("MythicDungeon_PlayerScripts") { }

    void OnLogin(Player* player) override
    {
        MythicDungeonManager::InitHighestCompletedDungeonAllTime(player);
        MythicDungeonManager::InitHighestCompletedDungeonThisSeason(player);
        MythicDungeonManager::InitHighestCompletedDungeonThisWeek(player);
    }

    void OnSpellCast(Player* player, Spell* spell, bool /*skipCheck*/)
    {

    }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Player* receiver)
    {

    }

};

class MythicDungeon_AllMapScript : public AllMapScript
{
public:
    MythicDungeon_AllMapScript() : AllMapScript("MythicDungeon_AllMapScript") { }
   
    void OnMapUpdate(Map* map, uint32 diff) override
    {
       MythicDungeonManager::Update(map, diff);
    }

    void OnPlayerEnterAll(Map* map, Player* player)
    {

    }
};

class MythicDungeon_AllCreatureScript : public AllCreatureScript
{
public:
    MythicDungeon_AllCreatureScript()
        : AllCreatureScript("MythicDungeon_AllCreatureScript")
    {

    }

    void OnAllCreatureUpdate(Creature* creature, uint32 /*diff*/) override
    {
        Map* map = creature->GetMap();
        MythicDungeonManager::ApplyAffixesAndOtherUpgrade(creature, map);
    }
};



class MythicDungeon_WorldScript : public WorldScript
{
public:
    MythicDungeon_WorldScript() : WorldScript("MythicDungeon_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        MythicDungeonManager::InitializeRewardsDungeons();
        MythicDungeonManager::InitializeMythicDungeons();
        MythicDungeonManager::InitializeMythicDungeonBosses();
        MythicDungeonManager::InitializeWeeklyAffixes();
        MythicDungeonManager::InitializeMythicKeys();
        MythicDungeonManager::InitializeConfig();
    }
};


using namespace Acore::ChatCommands;

class MythicDungeon_commandsScript : public CommandScript
{
public:
    MythicDungeon_commandsScript() : CommandScript("MythicDungeon_ommandsScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "startmythic",  HandleStartMythicCommand, SEC_MODERATOR,     Console::No },
        };
        return commandTable;
    }

    static bool HandleStartMythicCommand(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        Player* selectedPlayer = handler->getSelectedPlayer();
        if (selectedPlayer) {
            MythicDungeonManager::StartMythicDungeon(selectedPlayer, selectedPlayer->GetMapId(), 20);
        }
        return true;
    }
};


// Add all scripts in one
void AddSC_MythicDungeons()
{
    new MythicDungeon_PlayerScripts();
    new MythicDungeon_WorldScript();
    new MythicDungeon_AllMapScript();
    new MythicDungeon_AllCreatureScript();
    new MythicDungeon_commandsScript();
}
