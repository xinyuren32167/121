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
#include "ScriptedGossip.h"

 // Add player scripts
class MythicDungeon_PlayerScripts : public PlayerScript
{
public:
    MythicDungeon_PlayerScripts() : PlayerScript("MythicDungeon_PlayerScripts") { }


    bool CanResurrectThroughSpirit(Player* player)
    {
        Mythic* mythic = sMythicMgr->GetMythicPlayer(player);

        if (mythic)
            return false;

        return true;
    }

    void OnLogin(Player* player) override
    {
        if (!player)
            return;

        bool showUI = sMythicMgr->ShouldShowMythicUI(player);

        if (showUI) {
            Mythic* mythic = sMythicMgr->GetMythicPlayer(player);
            sEluna->SendShowMythicUI(player, true, mythic->IsDungeonStarted());
        }
        else
            sEluna->SendShowMythicUI(player, false, false);
    }

    void OnCreatureKilledByPet(Player* PetOwner, Creature* killed)
    {
        sMythicMgr->OnKill(PetOwner, killed);
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
            if (mythic) {
                sEluna->SendShowMythicUI(player, true, mythic->IsDungeonStarted());
            }
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
        Mythic* mythic = sMythicMgr->GetMythicPlayer(player);

        if (!mythic)
            return;

        mythic->OnPlayerRelease(player);
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

        if (Group* group = player->GetGroup()) {
            if (group->isRaidGroup())
                return false;
        }

        if (Mythic* mythic = sMythicMgr->GetMythicPlayer(player))
            return false;

        sMythicMgr->FindMythicDungeonByItsKeyItemId(itemId, foundDungeon);

        if (!foundDungeon)
        {
            std::string message = "We could not find the Id " + std::to_string(itemId) + ", if you see this message, contact a gamemaster.";
            ChatHandler(player->GetSession()).SendSysMessage(message);
            return false;
        }

        Map* map = sMapMgr->FindBaseMap(foundDungeon.mapId);

        if (!map) {
            ChatHandler(player->GetSession()).SendSysMessage("You can't use that here.");
            return false;
        }

        std::string name = map->GetMapName();

        if (player->GetDungeonDifficulty() != DUNGEON_DIFFICULTY_EPIC) {
            ChatHandler(player->GetSession()).SendSysMessage("You are not in Mythic Difficulty!");
            return false;
        }

        if (uint32 keyLevel = sMythicMgr->GetDungeonKeyLevelPreperation(player))
        {
            MythicMultiplier multi = sMythicMgr->GetMultplierByLevel(keyLevel);
            sEluna->SendPreperationMythicDungeon(player, name, foundDungeon.timeToComplete, keyLevel, multi.damage, multi.hp);
        }
        else
            ChatHandler(player->GetSession()).SendSysMessage("it seems you don't have any Mythic Key Active.");

        return true;
    }
};


class MythicDungeon_WorldScript : public WorldScript
{
public:
    MythicDungeon_WorldScript() : WorldScript("MythicDungeon_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        sMythicMgr->InitializeMultipliers();
        sMythicMgr->InitializePlayerMythicKeys();
        sMythicMgr->InitializeMythicDungeons();
        sMythicMgr->InitializeMythicDungeonBosses();
        sMythicMgr->InitializeRewardsToken();
        sMythicMgr->InitializeRewardsItems();
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
            { "updateKey",  HandleUpdateKey, SEC_MODERATOR,     Console::No },
            { "forceCompleteKey",  HandleForceCompleteKeystone, SEC_MODERATOR,     Console::No },
            { "generateMythicKey",  HandleCreateKey, SEC_MODERATOR,     Console::No },
        };
        return commandTable;
    }

    static bool HandleUpdateKey(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        Player* selectedPlayer = handler->getSelectedPlayer();

        if (!selectedPlayer)
            return false;

        sMythicMgr->UpdatePlayerKey(selectedPlayer, 3);
        return true;
    }

    static bool HandleCreateKey(ChatHandler* handler, uint32 dungeonId, uint32 level)
    {
        Player* selectedPlayer = handler->getSelectedPlayer();

        if (!selectedPlayer)
            return false;

        sMythicMgr->GenerateMythicKeyByLevelAndDungeonId(selectedPlayer, level, dungeonId);
        return true;
    }

    static bool HandleForceCompleteKeystone(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        Player* selectedPlayer = handler->getSelectedPlayer();
        Player* moderator = handler->GetPlayer();

        if (!selectedPlayer)
            return false;

        if(sMythicMgr->ForceCompleteMythic(selectedPlayer))
            ChatHandler(moderator->GetSession()).SendSysMessage("Success : Key completed.");
        else
            ChatHandler(moderator->GetSession()).SendSysMessage("Error : There is no mythic key...!");

        return true;
    }
};


class npc_mythic_generate_key : public CreatureScript
{
public:
    npc_mythic_generate_key() : CreatureScript("npc_mythic_generate_key") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);
        MythicKey* mythicKey = sMythicMgr->GetCurrentPlayerMythicKey(player);

        if (mythicKey)
            return false;

        sMythicMgr->GenerateFirstRandomMythicKey(player);
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Give me a random Mythic+ key.", GOSSIP_SENDER_MAIN, 16);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Bye.", GOSSIP_SENDER_MAIN, 17);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
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
    new npc_mythic_generate_key();
}
