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
#include "LearningSpellsManager.h"

// Add player scripts
class LearningSpells_PlayerScripts : public PlayerScript
{
public:
    LearningSpells_PlayerScripts() : PlayerScript("LearningSpells_PlayerScripts") { }

    void OnLevelChanged(Player* player, uint8 oldlevel) {
        if (player->getLevel() > oldlevel) {
            LearningSpellsManager::GiveSpellsForLevelup(player);
        }
    }

    void OnFirstLogin(Player* player) {

        LearningSpellsManager::GiveSpellsForLevelup(player);

        switch (player->getClass())
        {
        case CLASS_WARRIOR:
            player->removeSpell(78, SPEC_MASK_ALL, false);
            break;
        case CLASS_MAGE:
            player->removeSpell(168, SPEC_MASK_ALL, false);
            player->removeSpell(133, SPEC_MASK_ALL, false);
            break;
        default:
            break;
        }
    }
};

class LearningSpells_WordlScript : public WorldScript
{
public:
    LearningSpells_WordlScript() : WorldScript("LearningSpells_WordlScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        LearningSpellsManager::PreloadAllSpells();
    }
};


// Add all scripts in one
void AddSC_LearningSpells()
{
    new LearningSpells_PlayerScripts();
    new LearningSpells_WordlScript();
}

