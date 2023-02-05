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
#include "mod-autobalance/AutobalanceManager.h"

 // Add player scripts
class Autobalance_PlayerScripts : public PlayerScript
{
public:
    Autobalance_PlayerScripts() : PlayerScript("Autobalance_PlayerScripts") { }

    void OnLogin(Player* player) override
    {
    }

    void OnCreate(Player* player)
    {
    }

    void OnSpellCast(Player* player, Spell* spell, bool /*skipCheck*/)
    {
    }
};

class AutoBalance_UnitScript : public UnitScript
{
public:
    AutoBalance_UnitScript()
        : UnitScript("AutoBalance_UnitScript", true)
    {
    }

    uint32 DealDamage(Unit* AttackerUnit, Unit* playerVictim, uint32 damage, DamageEffectType /*damagetype*/) override
    {
        return _Modifer_DealDamage(playerVictim, AttackerUnit, damage);
    }

    void ModifyPeriodicDamageAurasTick(Unit* target, Unit* attacker, uint32& damage) override
    {
        damage = _Modifer_DealDamage(target, attacker, damage);
    }

    void ModifyMeleeDamage(Unit* target, Unit* attacker, uint32& damage) override
    {
        damage = _Modifer_DealDamage(target, attacker, damage);
    }

    void ModifyHealRecieved(Unit* target, Unit* attacker, uint32& damage) override {
        damage = _Modifer_DealDamage(target, attacker, damage);
    }

    uint32 _Modifer_DealDamage(Unit* target, Unit* attacker, uint32 damage)
    {
        if (!attacker || attacker->GetTypeId() == TYPEID_PLAYER || !attacker->IsInWorld())
            return damage;

        Creature* creature = target->ToCreature();

        if (!creature)
            return damage;

        Map* map = creature->GetMap();
             
        AutobalanceScalingInfo scaling = AutoBalanceManager::GetScalingInfo(map, creature);

        if (!scaling.meleeDamageModifier)
            return damage;

        if (scaling.meleeDamageModifier == 1)
            return damage;

        if (!(target->GetMap()->IsDungeon() && attacker->GetMap()->IsDungeon()) || (attacker->GetMap()->IsBattleground()
                && target->GetMap()->IsBattleground()))
            return damage;

        if ((attacker->IsHunterPet() || attacker->IsPet() || attacker->IsSummon()) && attacker->IsControlledByPlayer())
            return damage;

        return damage * scaling.meleeDamageModifier;
    }
};

class AutoBalance_AllCreatureScript : public AllCreatureScript
{
public:
    AutoBalance_AllCreatureScript()
        : AllCreatureScript("AutoBalance_AllCreatureScript")
    {

    }

    void OnAllCreatureUpdate(Creature* creature, uint32 /*diff*/) override
    {
        Map* map = creature->GetMap();
        AutoBalanceManager::ApplyScalingHealthAndMana(map, creature);
    }
};


using namespace Acore::ChatCommands;

class Autobalance_CommandsScript : public CommandScript
{
public:
    Autobalance_CommandsScript() : CommandScript("Autobalance_CommandsScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "reload autobalance",  HandleReloadCommand, SEC_MODERATOR,     Console::No },
        };
        return commandTable;
    }

    static bool HandleReloadCommand(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        Eluna::ReloadEluna();
        return true;
    }
};

class Autobalance_WorldScript : public WorldScript
{
public:
    Autobalance_WorldScript() : WorldScript("Autobalance_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        AutoBalanceManager::InitializeScalingPerSpecialization();
    }
};


// Add all scripts in one
void AddSC_Autobalance()
{
    new Autobalance_PlayerScripts();
    new Autobalance_WorldScript();
    new AutoBalance_UnitScript();
    new AutoBalance_AllCreatureScript();
}
