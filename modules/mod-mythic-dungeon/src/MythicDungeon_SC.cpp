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
#include <boost/algorithm/string.hpp>
#include "InstanceScript.h"
#include "MapMgr.h"
#include "MythicManager.h"

 // Add player scripts
class MythicDungeon_PlayerScripts : public PlayerScript
{
public:
    MythicDungeon_PlayerScripts() : PlayerScript("MythicDungeon_PlayerScripts") { }

    void OnLogin(Player* player) override
    {
        bool showUI = sMythicMgr->ShouldShowMythicUI(player);

        if (showUI) {
            Mythic* mythic = sMythicMgr->GetMythicPlayer(player);
            sEluna->SendShowMythicUI(player, true, mythic->IsDungeonStarted());
        }
        else
            sEluna->SendShowMythicUI(player, false, false);
    }

    void OnCreatureKill(Player* killer, Creature* killed)
    {
        sMythicMgr->OnKill(killer, killed);
    }

    void OnMapChanged(Player* player)
    {
        sMythicMgr->ListenCreationMythicOnMapChanged(player);

        bool showUI = sMythicMgr->ShouldShowMythicUI(player);

        if (showUI) {
            Mythic* mythic = sMythicMgr->GetMythicPlayer(player);
            sEluna->SendShowMythicUI(player, true, mythic->IsDungeonStarted());
        }
        else
            sEluna->SendShowMythicUI(player, false, false);
    }


    void OnPlayerKilledByCreature(Creature* killer, Player* killed)
    {
        sMythicMgr->OnPlayerDie(killed, killer);
    }

    void OnPlayerReleasedGhost(Player* player)
    {
       
    }
};


class Mythic_Keystone_Item : public ItemScript
{
public:
    Mythic_Keystone_Item() : ItemScript("Mythic_Keystone_Item") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        uint32 itemId = item->GetEntry();
        MythicDungeon foundDungeon;

        sMythicMgr->FindMythicDungeonByItsKeyItemId(itemId, foundDungeon);

        if (!foundDungeon)
        {

            std::string message = "We could not find the Id " + std::to_string(itemId) + ", if you see this message, contact a gamemaster.";
            ChatHandler(player->GetSession()).SendSysMessage(message);
            return false;
        }

        Map* map = sMapMgr->FindBaseMap(foundDungeon.mapId);
        std::string name = map->GetMapName();

        if (map->GetId() != foundDungeon.mapId) {
            ChatHandler(player->GetSession()).SendSysMessage("You can only activate this Mythic Keystone in " + name);
            return false;
        }

        if (player->GetDungeonDifficulty() != DUNGEON_DIFFICULTY_EPIC) {
            ChatHandler(player->GetSession()).SendSysMessage("You are not in Mythic Difficulty!");
            return false;
        }

        if (uint32 keyLevel = sMythicMgr->GetDungeonKeyLevelPreperation(player))
            sEluna->SendPreperationMythicDungeon(player, name, foundDungeon.timeToComplete, keyLevel);
        else
            ChatHandler(player->GetSession()).SendSysMessage("it seems you don't have any MythicKey Active.");
      

        return true;
    }
};


class MythicDungeon_WorldScript : public WorldScript
{
public:
    MythicDungeon_WorldScript() : WorldScript("MythicDungeon_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        sMythicMgr->InitializeCreatureKillingCount();
        sMythicMgr->InitializeMultipliers();
        sMythicMgr->InitializeRewardsDungeons();
        sMythicMgr->InitializePlayerMythicKeys();
        sMythicMgr->InitializeMythicDungeons();
        sMythicMgr->InitializeRewardsPlayersBag();
        sMythicMgr->InitializeMythicDungeonBosses();
    }

    void OnUpdate(uint32 diff)
    {
        sMythicMgr->Update(diff);
    }
};


using namespace Acore::ChatCommands;

class MythicDungeon_commandsScript : public CommandScript
{
public:
    MythicDungeon_commandsScript() : CommandScript("MythicDungeon_ommandsScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "startmythic",  HandleStartMythicCommand, SEC_MODERATOR,     Console::No },
        };
        return commandTable;
    }

    static bool HandleStartMythicCommand(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        Player* selectedPlayer = handler->getSelectedPlayer();
        return true;
    }
};


// Add all scripts in one
void AddSC_MythicDungeons()
{
    new MythicDungeon_PlayerScripts();
    new MythicDungeon_WorldScript();
    new MythicDungeon_commandsScript();
    new Mythic_Keystone_Item();
}
