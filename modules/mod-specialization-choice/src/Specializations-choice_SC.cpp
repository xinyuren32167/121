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
class SpecChoice_PlayerScripts : public PlayerScript
{
public:
    SpecChoice_PlayerScripts() : PlayerScript("SpecChoice_PlayerScripts") { }

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

class SpecChoice_CommandsScript : public CommandScript
{
public:
    SpecChoice_CommandsScript() : CommandScript("SpecChoice_CommandsScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "reload specs",  HandleReloadCommand, SEC_MODERATOR,     Console::No },
        };
        return commandTable;
    }

    static bool HandleReloadCommand(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        LOG_INFO("Runes", "Reload specs...");
        PlayerSpecialization::LoadAllSpecsPlayers();
        PlayerSpecialization::LoadAllSpecsSpells();
        PlayerSpecialization::InitializeSpecializations();
        Eluna::ReloadEluna();
        return true;
    }
};

class SpecChoice_WorldScript : public WorldScript
{
public:
    SpecChoice_WorldScript() : WorldScript("SpecChoice_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        PlayerSpecialization::LoadAllSpecsPlayers();
        PlayerSpecialization::LoadAllSpecsSpells();
        PlayerSpecialization::InitializeSpecializations();
    }
};


// Add all scripts in one
void AddSC_SpecChoices()
{
    new SpecChoice_PlayerScripts();
    new SpecChoice_WorldScript();
    new SpecChoice_CommandsScript();
}
