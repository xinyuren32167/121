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
#include "PetDefines.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"
#include "Log.h"
#include "ScriptedGossip.h"
#include "SpellInfo.h"

class Runes_PlayerScripts: public PlayerScript
{
public:
    Runes_PlayerScripts() : PlayerScript("Runes_PlayerScripts") { }

    void OnLogin(Player* player) override
    {
        RunesManager::ApplyRunesOnLogin(player);
    }

    void OnCreate(Player* player)
    {
        RunesManager::CreateDefaultCharacter(player);
    }

    void OnDelete(ObjectGuid guid, uint32 accountId)
    {
        RunesManager::RemoveSlotsOnCharacterDel(guid);
    }

    void OnPlayerResurrect(Player* player, float restore_percent, bool applySickness)
    {
        if (!player)
            return;

        RunesManager::ApplyRunesOnLogin(player);
    }

    void OnAchiComplete(Player* player, AchievementEntry const* achievement)
    {
        if (!achievement)
            return;

        if (!player)
            return;

        uint32 achievementId = achievement->ID;

        RunesManager::GiveAchievementReward(player, achievementId);
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
            { "addRune",  HandleAddRuneCommand, SEC_MODERATOR,     Console::No },
            { "getRunes",  HandleGetRunesCommand, SEC_MODERATOR,     Console::No },

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

    static bool HandleAddRuneCommand(ChatHandler* handler, Optional<PlayerIdentifier> playerID)
    {
        Player* player = handler->GetPlayer();
        int8 quality = NORMAL_QUALITY;
        Rune rune = RunesManager::GetRandomRune(player, quality);

        if (!rune) {
            LOG_ERROR("No Rune", "No rune found, error");
            return false;
        }
        RunesManager::AddRunesPlayer(player, { rune });
        return true;
    }

    static bool HandleGetRunesCommand(ChatHandler* handler, Optional<PlayerIdentifier> playerID)
    {
        return true;
    }
};

class spell_activate_rune : public SpellScript
{
    PrepareSpellScript(spell_activate_rune);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        SpellValue const* value = GetSpellValue();
        uint32 runeId = value->EffectBasePoints[EFFECT_0];
        Rune rune = RunesManager::GetRuneBySpellId(runeId);

        if (rune.spellId > 0)
        {
            RunesManager::SpellConversion(rune.spellId, player, true);
            RunesManager::AddRuneToSlot(player, rune);
            sEluna->OnActivateRune(player, "Rune successfully activated!", 0);
        }

    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_activate_rune::HandleProc);
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
        RunesManager::LoadSpellsConversion();
        RunesManager::LoadCharacterDraws();
        RunesManager::LoadRewardsAchievement();
        RunesManager::LoadAllAutoRefund();
        RunesManager::LoadAllLuckyRunes();
    }
};


class spell_generate_random_rune : public SpellScript
{
    PrepareSpellScript(spell_generate_random_rune);

    void HandleProc()
    {

        Player* player = GetCaster()->ToPlayer();
        int8 quality ;

        if (m_scriptSpellId == 79500) quality = NORMAL_QUALITY;
        else if (m_scriptSpellId == 79501) quality = UNCOMMON_QUALITY;
        else if (m_scriptSpellId == 79502) quality = RARE_QUALITY;
        else if (m_scriptSpellId == 79503) quality = EPIC_QUALITY;
        else quality = LEGENDARY_QUALITY;

        Rune rune = RunesManager::GetRandomRune(player, quality);

        if (!rune) {
            LOG_ERROR("No Rune", "No rune found, error");
            return;
        }

        bool isAutoRefund = RunesManager::IsSpellIdAutoRefund(player, rune.spellId);

        if (isAutoRefund) {
            RunesManager::AutomaticalyRefundRune(player, rune);
            return;
        }

        RunesManager::AddRunesPlayer(player, { rune });
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_generate_random_rune::HandleProc);
    }
};

class spell_generate_more_random_rune : public SpellScript
{
    PrepareSpellScript(spell_generate_more_random_rune);

    void HandleProc()
    {

        Player* player = GetCaster()->ToPlayer();

        std::vector<Rune> runes = {};

        SpellValue const* value = GetSpellValue();
        uint32 count = value->EffectBasePoints[EFFECT_0];

        while (count > 0)
        {
            Rune rune = RunesManager::GetRandomRune(player, NORMAL_QUALITY);

            if (!rune)
                continue;

            bool isAutoRefund = RunesManager::IsSpellIdAutoRefund(player, rune.spellId);

            if (isAutoRefund) {
                RunesManager::AutomaticalyRefundRune(player, rune);
                count--;
            }
            else {
                runes.push_back(rune);
                count--;
            }
        }

        RunesManager::AddRunesPlayer(player, runes);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_generate_more_random_rune::HandleProc);
    }
};

class spell_upgrade_rune : public SpellScript
{
    PrepareSpellScript(spell_upgrade_rune);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();
        SpellValue const* value = GetSpellValue();
        uint32 runeId = value->EffectBasePoints[EFFECT_0];

        Rune rune = RunesManager::GetRuneBySpellId(runeId);

        if (!rune)
            return;

        RunesManager::RemoveNecessaryItemsForUpgrade(player, rune);
        RunesManager::AddRunesPlayer(player, { rune });
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_upgrade_rune::HandleProc);
    }
};

class go_rune_upgrader : public GameObjectScript
{
public:
    go_rune_upgrader() : GameObjectScript("go_rune_upgrader") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        sEluna->OpenForgeRune(player);
        return true;
    }
};

class npc_buying_rune : public CreatureScript
{
public:
    npc_buying_rune() : CreatureScript("npc_buying_rune") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);
        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF:
        {
            RunesManager::ApplyBuyingRuneWithGold(player);
            uint32 cost = RunesManager::CalculateGoldCostToBuyRune(player);
            player->ModifyMoney(-cost);
            player->AddItem(70002, 1);
            CloseGossipMenuFor(player);
        }
            break;
        case GOSSIP_ACTION_TRADE:
            player->GetSession()->SendListInventory(creature->GetGUID());
            break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        uint32 cost = RunesManager::CalculateGoldCostToBuyRune(player);
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Buy a Sealed Common Rune with Gold", 50, GOSSIP_ACTION_INFO_DEF, "Are you sure want to buy a Sealed Common Rune?", cost, false);
        AddGossipItemFor(player, GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        SendGossipMenuFor(player, 900900, creature->GetGUID());
        return true;
    }
};

// Add all scripts in one
void AddSC_runesScripts()
{
    new Runes_PlayerScripts();
    new Runes_WorldScript();
    new Runes_CommandsScript();
    new go_rune_upgrader();
    new npc_buying_rune();
    RegisterSpellScript(spell_activate_rune);
    RegisterSpellScript(spell_generate_random_rune);
    RegisterSpellScript(spell_upgrade_rune);
    RegisterSpellScript(spell_generate_more_random_rune);
}
