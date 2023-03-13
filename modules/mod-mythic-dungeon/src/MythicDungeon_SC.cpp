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

    }

    void OnCreate(Player* player)
    {

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
    }
};


// Add all scripts in one
void AddSC_MythicDungeons()
{
    new MythicDungeon_PlayerScripts();
    new MythicDungeon_WorldScript();
    new MythicDungeon_AllMapScript();
}
