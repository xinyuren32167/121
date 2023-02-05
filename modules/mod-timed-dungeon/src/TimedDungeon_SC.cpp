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
#include "TimeDungeonManager.h"
#include <boost/algorithm/string.hpp>

 // Add player scripts
class TimeDungeon_PlayerScripts : public PlayerScript
{
public:
    TimeDungeon_PlayerScripts() : PlayerScript("TimeDungeon_PlayerScripts") { }

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


class TimeDungeon_WorldScript : public WorldScript
{
public:
    TimeDungeon_WorldScript() : WorldScript("TimeDungeon_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
    }
};


// Add all scripts in one
void AddSC_TimeDungeons()
{
    new TimeDungeon_PlayerScripts();
    new TimeDungeon_WorldScript();
}
