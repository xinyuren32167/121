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
#include "PlayerSpecialization.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"
#include "Log.h"

enum WarlockSpells
{
    SPELL_MINION_INCREASE_DREAD_STALKER = 1100009,
    SPELL_MINION_INCREASE_WILD_IMP = 1100010,
    SPELL_MINION_INCREASE_DARKGLARE = 1100011,
    SPELL_MINION_INCREASE_VILEFIEND = 1100012,
    SPELL_MINION_INCREASE_DEMONIC_TYRANT = 1100013,
    SPELL_MINION_INCREASE_BOMBER = 1100014,
};


enum HunterSpells {
    SPELL_HUNTER_LONE_WOLF = 80182,
};

 // Add player scripts
class SpecChoice_PlayerScripts : public PlayerScript
{
public:
    SpecChoice_PlayerScripts() : PlayerScript("SpecChoice_PlayerScripts") { }

    void SetWarlockPowerOnlevelup(Player* player) {
        player->setPowerType(POWER_ENERGY);
        player->SetMaxPower(POWER_ENERGY, 25);
        player->SetPower(POWER_ENERGY, 0);
    }

    void OnLevelChanged(Player* player, uint8 oldlevel)
    {
        if (player->getLevel() == 10)
            sEluna->ShowWarningSpecialization(player);

        if (player->getClass() == CLASS_WARLOCK)
            SetWarlockPowerOnlevelup(player);

        uint32 currentSpecId = PlayerSpecialization::GetCurrentSpecId(player);

        if (currentSpecId == 0 && player->getLevel() >= 10)
            sEluna->ShowWarningSpecialization(player);
    }

    void IniatializeWarlockOnLogin(Player* player) {

        player->setPowerType(POWER_ENERGY);
        player->SetMaxPower(POWER_ENERGY, 25);
        player->SetPower(POWER_ENERGY, 0);

        player->AddAura(SPELL_MINION_INCREASE_DREAD_STALKER, player);
        player->AddAura(SPELL_MINION_INCREASE_WILD_IMP, player);
        player->AddAura(SPELL_MINION_INCREASE_DARKGLARE, player);
        player->AddAura(SPELL_MINION_INCREASE_VILEFIEND, player);
        player->AddAura(SPELL_MINION_INCREASE_DEMONIC_TYRANT, player);
        player->AddAura(SPELL_MINION_INCREASE_BOMBER, player);

        player->SetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER + POWER_ENERGY, -10.f);
        player->SetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER + POWER_ENERGY, -10.f);

    }

    void InitializeElementalSpecialization(Player* player) {
        uint32 currentSpecId = PlayerSpecialization::GetCurrentSpecId(player);
        if (currentSpecId == SHAMAN_ELEMENTAL) {
            player->setPowerType(POWER_RUNIC_POWER);
            player->SetMaxPower(POWER_RUNIC_POWER, 1000);
            player->SetPower(POWER_RUNIC_POWER, 0);
        }
    }

    void OnLogin(Player* player)
    {
        if (player->getClass() == CLASS_PALADIN) {
            player->SetMaxPower(POWER_ENERGY, 5);
            player->SetPower(POWER_ENERGY, 0);
        }

        uint32 currentSpecId = PlayerSpecialization::GetCurrentSpecId(player);

        if (currentSpecId == 0 && player->getLevel() >= 10)
            sEluna->ShowWarningSpecialization(player);

        if (player->getClass() == CLASS_WARLOCK)
            IniatializeWarlockOnLogin(player);

        if (player->getClass() == CLASS_SHAMAN)
            InitializeElementalSpecialization(player);
    }

    void OnPlayerLearnTalents(Player* player, uint32 talentId, uint32 talentRank, uint32 spellid)
    {
        player->UpdateMastery();
    }
};


using namespace Acore::ChatCommands;

class SpecChoice_CommandsScript : public CommandScript
{
public:
    SpecChoice_CommandsScript() : CommandScript("SpecChoice_CommandsScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "reload specs",  HandleReloadCommand, SEC_MODERATOR,     Console::No },
        };
        return commandTable;
    }

    static bool HandleReloadCommand(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        LOG_INFO("Runes", "Reload specs...");
        PlayerSpecialization::LoadAllSpecsPlayers();
        PlayerSpecialization::LoadAllSpecsSpells();
        PlayerSpecialization::InitializeSpecializations();
        Eluna::ReloadEluna();
        return true;
    }
};

class spell_activate_specialization : public SpellScript
{
    PrepareSpellScript(spell_activate_specialization);


    void UnlearnSpells()
    {

    }

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();
        SpellValue const* value = GetSpellValue();
        uint32 newSpecId = value->EffectBasePoints[EFFECT_0];

        uint32 currentSpecId = PlayerSpecialization::GetCurrentSpecId(player);
        Specialization newSpec = PlayerSpecialization::m_Specializations[newSpecId];

        if (currentSpecId == HUNTER_MARSKMANSHIP)
            player->RemoveAura(80182);

        if (currentSpecId > 0) {
            for (auto const& spellId : PlayerSpecialization::m_SpecSpells[currentSpecId]) {
                player->removeSpell(spellId, SPEC_MASK_ALL, false, true);
                player->RemoveAura(spellId);
            }
        }

        for (auto const& spellId : PlayerSpecialization::m_SpecSpells[newSpecId])
            player->learnSpell(spellId, false, false);

        if (newSpec.powerType != POWER_ALL) {
            player->setPowerType(newSpec.powerType);
            if (newSpec.powerType == POWER_RUNIC_POWER)
                player->SetMaxPower(POWER_RUNIC_POWER, 1000);
            else
                player->SetMaxPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
        }

        auto summonedUnits = player->ToPlayer()->m_Controlled;

        for (const auto& pet : summonedUnits)
            if (pet->GetCharmInfo())
                pet->ToTempSummon()->DespawnOrUnsummon();

        if(player->HasAura(SPELL_HUNTER_LONE_WOLF))
            player->RemoveAura(80182);

        PlayerSpecialization::m_PlayersSpecialization[player->GetGUID().GetCounter()] = newSpecId;
        CharacterDatabase.Execute("UPDATE characters SET specId = {} WHERE guid = {}", newSpecId, player->GetGUID().GetCounter());
        sEluna->OnActivateSpec(player, "Specialization " + newSpec.name + " successfully activated!", true, newSpecId);
        player->UpdateMastery();
        player->SaveToDB(false, false);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_activate_specialization::HandleProc);
    }
};

class SpecChoice_WorldScript : public WorldScript
{
public:
    SpecChoice_WorldScript() : WorldScript("SpecChoice_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        PlayerSpecialization::LoadAllSpecsPlayers();
        PlayerSpecialization::LoadAllSpecsSpells();
        PlayerSpecialization::InitializeSpecializations();
    }
};


// Add all scripts in one
void AddSC_SpecChoices()
{
    new SpecChoice_PlayerScripts();
    new SpecChoice_WorldScript();
    new SpecChoice_CommandsScript();
    RegisterSpellScript(spell_activate_specialization);
}
