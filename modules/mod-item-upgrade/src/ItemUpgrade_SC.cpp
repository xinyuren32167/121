/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "LuaEngine.h"
#include "Spell.h"
#include "ItemUpgradeManager.h"

// Add player scripts
class ItemUpgrade_PlayerScripts : public PlayerScript
{
public:
    ItemUpgrade_PlayerScripts() : PlayerScript("ItemUpgrade_PlayerScripts") { }

    void OnLevelChanged(Player* player, uint8 oldlevel) {
      
    }

    void OnFirstLogin(Player* player) {
    }
};

class ItemUpgrade_WorldScript : public WorldScript
{
public:
    ItemUpgrade_WorldScript() : WorldScript("ItemUpgrade_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        LOG_INFO("Runes", "Initializing Runes...");
        ItemUpgradeManager::LoadCosts();
    }
};

// Add all scripts in one
void AddSC_ItemUpgrade()
{
    new ItemUpgrade_PlayerScripts();
    new ItemUpgrade_WorldScript();
}

