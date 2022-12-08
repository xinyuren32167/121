/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "RunesManager.h"

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
}
