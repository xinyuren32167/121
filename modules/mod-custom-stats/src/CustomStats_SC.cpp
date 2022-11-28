/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "Log.h"
#include "CustomStatsManager.h"


class CustomStats_PlayerScripts : public PlayerScript
{
public:
    CustomStats_PlayerScripts() : PlayerScript("CustomStats_PlayerScripts") { }

    void OnUpdateMastery(Player* player, uint32 value)
    {
        CustomStatsManager::UpdateMastery(player, value);
    }

    void OnUpdateVersatility(Player* player, uint32 value)
    {
        CustomStatsManager::UpdateVersatility(player, value);
    }

    void OnLogin(Player* player)
    {
        uint32 amount = player->GetUInt32Value(static_cast<uint16>(PLAYER_FIELD_COMBAT_RATING_1) + CR_HIT_MELEE);
        if (amount < 0)
            amount = 0;
        CustomStatsManager::UpdateMastery(player, amount);
    }
};

class CustomStats_WorldScript : public WorldScript
{
public:
    CustomStats_WorldScript() : WorldScript("CustomStats_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        LOG_INFO("Runes", "Initialize Spells Mastery...");
        CustomStatsManager::LoadSpellsMastery();
    }
};


// Add all scripts in one
void AddSC_CustomStats()
{
    new CustomStats_PlayerScripts();
    new CustomStats_WorldScript();
}
