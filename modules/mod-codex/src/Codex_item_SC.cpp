/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "LuaEngine.h"
#include "Spell.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"
#include "Log.h"


enum AccountStoryProgression {
    NONE = 0,
    REACH_LEVEL_60_FIRST_TIME = 1,

    // The First Chapter gives 10 talents point to the players and everytime other character reach level 60 they get 10 talent points. 
    HAS_COMPLETED_THE_FIRST_CHAPTER
};


static std::map<uint32, AccountStoryProgression> mAccountStoryProgression = {};

class CodexProgression {

public:
    static void initializingCharactersStoryProgression()
    {
        mAccountStoryProgression = {};

        QueryResult result = LoginDatabase.Query("SELECT id, storyProgression FROM account");

        if (!result)
            return;

        do
        {
            Field* fields = result->Fetch();
            uint32 accountId = fields[0].Get<uint32>();
            AccountStoryProgression progression = AccountStoryProgression(fields[1].Get<uint8>());
            mAccountStoryProgression[accountId] = progression;
        } while (result->NextRow());
    }

    static AccountStoryProgression GetStoryProgressionAccount(Player* player)
    {
        auto it = mAccountStoryProgression.find(player->GetSession()->GetAccountId());

        if (it != mAccountStoryProgression.end())
            return it->second;

        return NONE;
    }

    static void UpdateStoryProgressionAccount(Player* player, AccountStoryProgression progression)
    {
        auto it = mAccountStoryProgression.find(player->GetSession()->GetAccountId());

        if (it != mAccountStoryProgression.end())
            it->second = progression;

        LoginDatabase.Query("UPDATE account SET storyProgression = {} WHERE id = {}", progression, player->GetSession()->GetAccountId());
    }
};


 // Add player scripts
class Codex_PlayerScripts : public PlayerScript
{
public:
    Codex_PlayerScripts() : PlayerScript("Codex_PlayerScripts") { }

    void SendLetter(Player* player)
    {
        // char const* text = sObjectMgr->GetAcoreString(e.action.playerTalk.textId, DEFAULT_LOCALE);
    }

    void OnLevelChanged(Player* player, uint8 oldlevel)
    {
        if (player->getLevel() >= 60)
        {
            AccountStoryProgression progressionStory = CodexProgression::GetStoryProgressionAccount(player);

            if (progressionStory == NONE)
            {
                CodexProgression::UpdateStoryProgressionAccount(player, REACH_LEVEL_60_FIRST_TIME);
                SendLetter(player);
            }

            if (progressionStory >= HAS_COMPLETED_THE_FIRST_CHAPTER)
            {
                // Give the items that gives 10 talents point to the players.
            }
        }
    }
};

// Add all scripts in one
void AddSC_Codex()
{
    new Codex_PlayerScripts();
}

