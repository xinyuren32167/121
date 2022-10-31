/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"

// Add player scripts
class Perk_Player_Scripts : public PlayerScript
{
public:
    Perk_Player_Scripts() : PlayerScript("Perk_Player_Scripts") { }

    void OnLogin(Player* player) override
    {

    }
};

// Add all scripts in one
void AddPerk_Player_Scripts()
{
    new Perk_Player_Scripts();
}
