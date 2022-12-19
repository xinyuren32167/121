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
// Add player scripts
class Runes_PlayerScripts: public PlayerScript
{
public:
    Runes_PlayerScripts() : PlayerScript("Runes_PlayerScripts") { }

    void OnLogin(Player* player) override
    {
       
    }


    void OnCreate(Player* player)
    {
        RunesManager::CreateDefaultCharacter(player);
    }

    void OnSpellCast(Player* player, Spell* spell, bool /*skipCheck*/) {

        if (spell->GetSpellInfo()->Id != 5000000)
            return;

        const int32 loadoutId = spell->GetSpellValue()->EffectBasePoints[EFFECT_0];
    }

};


using namespace Acore::ChatCommands;

class Runes_CommandsScript : public CommandScript
{
public:
    Runes_CommandsScript() : CommandScript("Runes_CommandsScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "reload runes",  HandleReloadCommand, SEC_MODERATOR,     Console::No },
        };
        return commandTable;
    }

    static bool HandleReloadCommand(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        LOG_INFO("Runes", "Reload Runes...");

        RunesManager::SetupConfig();
        RunesManager::LoadAllRunes();
        RunesManager::LoadAccountsRunes();
        RunesManager::LoadAllLoadout();
        RunesManager::LoadAllSlotRune();
        RunesManager::LoadAllProgression();
        Eluna::ReloadEluna();

        return true;
    }
};

class Runes_WorldScript : public WorldScript
{
public:
    Runes_WorldScript() : WorldScript("Runes_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        LOG_INFO("Runes", "Initialize Runes...");
        RunesManager::SetupConfig();
        RunesManager::LoadAllRunes();
        RunesManager::LoadAccountsRunes();
        RunesManager::LoadAllLoadout();
        RunesManager::LoadAllSlotRune();
        RunesManager::LoadAllProgression();
    }
};


// Add all scripts in one
void AddSC_runesScripts()
{
    new Runes_PlayerScripts();
    new Runes_WorldScript();
    new Runes_CommandsScript();
}
