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
#include "DatabaseEnv.h"



enum spellMount {
    SPECTRAL_TIGER = 33225,
    MAGIC_ROOSTER = 46778,
};

class SpecialMount {

private:
    static std::vector<uint32> playersFirstAlpha;
    static std::vector<uint32> playersLastAlpha;
public:
    static void PreloadPlayer()
    {
        QueryResult resultIt = LoginDatabase.Query("SELECT id FROM account WHERE joindate BETWEEN '2022-01-04' AND '2024-01-21' AND totaltime > 0");

        if (resultIt)
        {
            do
            {
                Field* fields = resultIt->Fetch();
                uint32 accountId = fields[0].Get<uint32>();
                playersFirstAlpha.push_back(accountId);
            } while (resultIt->NextRow());
        }

        QueryResult resultIJ = LoginDatabase.Query("SELECT id FROM account WHERE joindate BETWEEN '2022-01-04' AND '2024-05-21' AND totaltime > 0");

        if (resultIJ)
        {
            do
            {
                Field* fields = resultIJ->Fetch();
                uint32 accountId = fields[0].Get<uint32>();
                playersLastAlpha.push_back(accountId);
            } while (resultIJ->NextRow());
        }
    }

    static void RewardPlayer(Player* player) {

        uint32 accountId = player->GetSession()->GetAccountId();

        if (player->getLevel() >= 40)
        {
            auto it = std::find(playersFirstAlpha.begin(), playersFirstAlpha.end(), accountId);

            if (it != playersFirstAlpha.end())
            {
                player->AddItem(SPECTRAL_TIGER, 1);
            }

            auto ij = std::find(playersLastAlpha.begin(), playersLastAlpha.end(), accountId);

            if (ij != playersLastAlpha.end())
            {
                player->AddItem(MAGIC_ROOSTER, 1);
            }
        }

    }
};

std::vector<uint32> SpecialMount::playersFirstAlpha = {};
std::vector<uint32> SpecialMount::playersLastAlpha = {};


// Add player scripts
class LearningSpells_PlayerScripts : public PlayerScript
{
public:
    LearningSpells_PlayerScripts() : PlayerScript("LearningSpells_PlayerScripts") { }

    void OnLevelChanged(Player* player, uint8 oldlevel) {

        if (player->getLevel() > oldlevel) {
            LearningSpellsManager::GiveSpellsForLevelup(player);
        }

        SpecialMount::RewardPlayer(player);
    }

    void OnFirstLogin(Player* player)
    {
        AchievementEntry const* entry = sAchievementMgr->GetAchievement(6496);

        if (entry) {
            player->CompletedAchievement(entry);
        }

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
        player->UpdateSkillsToMaxSkillsForLevel();
    }
};

class LearningSpells_WordlScript : public WorldScript
{
public:
    LearningSpells_WordlScript() : WorldScript("LearningSpells_WordlScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        LearningSpellsManager::PreloadAllSpells();
        SpecialMount::PreloadPlayer();
    }
};


// Add all scripts in one
void AddSC_LearningSpells()
{
    new LearningSpells_PlayerScripts();
    new LearningSpells_WordlScript();
}

