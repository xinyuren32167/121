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

// Add player scripts
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
        RunesManager::ApplyRunesOnLogin(player);
    }


    void OnSpellCast(Player* player, Spell* spell, bool /*skipCheck*/) {

        if (spell->GetSpellInfo()->Id != 5000000)
            return;

        const int32 loadoutId = spell->GetSpellValue()->EffectBasePoints[EFFECT_0];
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
            { "powerleveling",  HandleLevelupCustom, SEC_PLAYER,     Console::No },

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

    static bool HandleLevelupCustom(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        if (handler->GetPlayer()->getLevel() < 60)
            handler->GetPlayer()->GiveLevel(handler->GetPlayer()->getLevel() + 1);

        return true;
    }
};

class spell_activate_rune : public SpellScript
{
    PrepareSpellScript(spell_activate_rune);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();
        SpellValue const* value = GetSpellValue();
        uint32 runeId = value->EffectBasePoints[EFFECT_0];
        Rune rune;

        if(RunesManager::IsDebugEnabled())
            rune = RunesManager::GetRuneBySpellId(runeId);
        else
            rune = RunesManager::GetRuneById(player, runeId);

        RunesManager::SpellConversion(rune.spellId, player, true);
        GetCaster()->AddAura(rune.spellId, GetCaster());
        RunesManager::AddRuneToSlot(player, rune, runeId);
        sEluna->OnActivateRune(player, "Rune successfully activated!", 0);
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
    }
};



// Add all scripts in one
void AddSC_runesScripts()
{
    new Runes_PlayerScripts();
    new Runes_WorldScript();
    new Runes_CommandsScript();
    RegisterSpellScript(spell_activate_rune);
}
