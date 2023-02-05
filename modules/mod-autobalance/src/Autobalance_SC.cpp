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
#include "PlayerSpecialization.h"

 // Add player scripts
class Autobalance_PlayerScripts : public PlayerScript
{
public:
    Autobalance_PlayerScripts() : PlayerScript("Autobalance_PlayerScripts") { }

    void OnLogin(Player* player) override
    {
    }

    void OnCreate(Player* player)
    {
    }

    void OnSpellCast(Player* player, Spell* spell, bool /*skipCheck*/)
    {
    }
};


using namespace Acore::ChatCommands;

class Autobalance_CommandsScript : public CommandScript
{
public:
    Autobalance_CommandsScript() : CommandScript("Autobalance_CommandsScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "reload timeddungeon",  HandleReloadCommand, SEC_MODERATOR,     Console::No },
        };
        return commandTable;
    }

    static bool HandleReloadCommand(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        Eluna::ReloadEluna();
        return true;
    }
};

class Autobalance_WorldScript : public WorldScript
{
public:
    Autobalance_WorldScript() : WorldScript("Autobalance_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
    }
};


// Add all scripts in one
void AddSC_Autobalance()
{
    new Autobalance_PlayerScripts();
    new Autobalance_WorldScript();
}
