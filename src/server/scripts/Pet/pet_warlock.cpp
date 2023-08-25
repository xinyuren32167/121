/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

 /*
  * Ordered alphabetically using scriptname.
  * Scriptnames of files in this file should be prefixed with "npc_pet_pri_".
  */

#include "Cell.h"
#include "CellImpl.h"
#include "CombatAI.h"
#include "GridNotifiers.h"
#include "PassiveAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"

enum WarlockSpells
{
    SPELL_WILDIMP_FIREBOLT = 83003,
    SPELL_DEMONIC_CORE = 83029,
    SPELL_CHARGE_FELBOAR = 83005,
    SPELL_DEMONBOLT = 83008,
    SPELL_DARKGLARE_DAMAGE = 83050,

    EVENT_TRY_ATTACK_NEW_TARGET = 1,

    TALENT_WARLOCK_IMPROVED_IMPS = 18694,
};

enum PET_WARLOCKS {

    PET_DARKHOUND = 600600,
    PET_WILDIMP = 600601,
    PET_FELBOAR = 600602,
    PET_DEMONIC_TYRAN = 600603,
};

struct npc_pet_warlock_wildimp : public ScriptedAI
{
    npc_pet_warlock_wildimp(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    void InitializeAI() override { }

    void EnterCombat(Unit*) override
    {
        attackCount = 0;
        _events.Reset();
        _events.ScheduleEvent(1, 500);
    }


    bool IsDemonicTyrantSummoned(Unit* owner)
    {
        bool isSummoned = false;

        for (Unit::ControlSet::const_iterator itr = owner->m_Controlled.begin(); itr != owner->m_Controlled.end(); ++itr)
            if ((*itr)->IsAlive() && (*itr)->GetEntry() == PET_DEMONIC_TYRAN)
            {
                isSummoned = true;
                break;
            }
        return isSummoned;
    }

    void Reset() override
    {
        if (me->IsAlive())
        {

        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    int32 damage = owner->CalculateSpellDamageWithRatio(SPELL_SCHOOL_MASK_FIRE, 0.63f);
                    if (AuraEffect const* aurEff = owner->GetAuraEffectOfRankedSpell(TALENT_WARLOCK_IMPROVED_IMPS, EFFECT_0))
                        damage += CalculatePct(damage, aurEff->GetAmount());
                    me->CastCustomSpell(SPELL_WILDIMP_FIREBOLT, SPELLVALUE_BASE_POINT0, damage, me->GetVictim());
                }
                _events.ScheduleEvent(1, 2000);
                break;
            default:
                break;
            }
        }

        DoMeleeAttackIfReady();
    }


    void JustDespawned() override
    {
        if (!roll_chance_i(10))
            return;

        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            if (Aura* aura = owner->GetAura(SPELL_DEMONIC_CORE))
                aura->SetCharges(aura->GetCharges() + 1);
            else
                owner->CastCustomSpell(SPELL_DEMONIC_CORE, SPELLVALUE_AURA_CHARGE, 1, owner, true);
        }
    }

private:
    EventMap _events;
    bool _initAttack;
    uint32 attackCount;
};

struct npc_pet_warlock_bomber : public ScriptedAI
{
    npc_pet_warlock_bomber(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    void InitializeAI() override { }

    void EnterCombat(Unit*) override
    {
        attackCount = 0;
        _events.Reset();
        _events.ScheduleEvent(1, 1000);
    }


    void AttackBomb()
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            if (Unit* target = owner->GetSelectedUnit())
            {
                if (me->CanCreatureAttack(target))
                {
                    me->CastSpell(target, 83058, true, nullptr, nullptr, owner->GetGUID());
                    AttackStart(target);
                }
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                AttackBomb();
                _events.ScheduleEvent(1, 1000);
                break;
            default:
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    bool _initAttack;
    uint32 attackCount;
};


struct npc_pet_warlock_darkglare: public ScriptedAI
{
    npc_pet_warlock_darkglare(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    void InitializeAI() override { }

    void EnterCombat(Unit*) override
    {
        _events.Reset();
        _events.ScheduleEvent(1, 0);
    }


    uint8 EveryDamageOverTimeEffect()
    {
        Unit* victim = me->GetVictim();
        auto auras = victim->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);

        auras.remove_if([&](AuraEffect const* effect) -> bool
        {
            return effect->GetCasterGUID() == me->GetGUID();
        });

        return auras.size();
    }

    int32 CalculateDamageOutput(Unit* owner)
    {
        int32 damage = owner->CalculateSpellDamageWithRatio(SPELL_SCHOOL_MASK_SHADOW, 0.43f);
        int32 damageIncreasePerEffect = 25.f;

        float pct = EveryDamageOverTimeEffect() * damageIncreasePerEffect;

        damage = AddPct(damage, pct);

        return damage;
    }


    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (Unit* target = owner->GetSelectedUnit())
                {
                    if (me->CanCreatureAttack(target))
                    {
                        AttackStart(target);
                        _initAttack = false;
                    }
                }
            }
        }

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    int32 damage = CalculateDamageOutput(owner);
                    me->CastCustomSpell(SPELL_DARKGLARE_DAMAGE, SPELLVALUE_BASE_POINT0, damage, me->GetVictim());
                }
                _events.ScheduleEvent(1, 2000);
                break;
            default:
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    bool _initAttack;
};

struct npc_pet_warlock_vilefiend : public ScriptedAI
{
    npc_pet_warlock_vilefiend(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    void InitializeAI() override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);
    }

    void AttackTarget(Unit* target)
    {
        me->JumpTo(target, 9.375f);
        AttackStart(target);
    }

    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (Unit* target = owner->GetSelectedUnit())
                {
                    if (me->CanCreatureAttack(target))
                    {
                        AttackTarget(target);
                        _initAttack = false;
                    }
                }
            }
        }

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_TRY_ATTACK_NEW_TARGET:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* newTarget = owner->GetSelectedUnit())
                    {
                        if (Unit* victim = me->GetVictim()) {
                            if (victim->GetGUID() != newTarget->GetGUID())
                            {
                                if (me->CanCreatureAttack(newTarget))
                                    AttackTarget(newTarget);
                            }
                        }
                    }
                }
                _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);
                break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            if (Aura* aura = owner->GetAura(SPELL_DEMONIC_CORE))
                aura->SetCharges(aura->GetCharges() + 1);
            else
                owner->CastCustomSpell(SPELL_DEMONIC_CORE, SPELLVALUE_AURA_CHARGE, 1, owner, true);
        }
    }

private:
    EventMap _events;
    bool _initAttack;
    Player* owner;
    int32 shadow;
};

struct npc_pet_warlock_felguard : public ScriptedAI
{
    npc_pet_warlock_felguard(Creature* creature) : ScriptedAI(creature), _initAttack(true), _didCharge(false) { }

    void InitializeAI() override
    {
        _events.Reset();
        me->AddAura(83031, me);
        _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);
    }

    void AttackTarget(Unit* target)
    {
        if (!_didCharge)
        {
            me->CastSpell(target, 25821);
            _didCharge = true;
        }
        AttackStart(target);
    }

    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (Unit* target = owner->GetSelectedUnit())
                {
                    if (me->CanCreatureAttack(target))
                    {
                        AttackTarget(target);
                        _initAttack = false;
                    }
                }
            }
        }

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_TRY_ATTACK_NEW_TARGET:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* newTarget = owner->GetSelectedUnit())
                    {
                        if (Unit* victim = me->GetVictim()) {
                            if (victim->GetGUID() != newTarget->GetGUID())
                            {
                                if (me->CanCreatureAttack(newTarget))
                                    AttackTarget(newTarget);
                            }
                        }
                    }
                }
                _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);
                break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    bool _initAttack;
    bool _didCharge;
    Player* owner;
};


struct npc_pet_warlock_demonic_tyrant : public ScriptedAI
{
    npc_pet_warlock_demonic_tyrant(Creature* creature) : ScriptedAI(creature) { }

    void InitializeAI() override
    {
        _events.Reset();
        _events.ScheduleEvent(1, 2000);
        owner = me->GetCharmerOrOwnerPlayerOrPlayerItself();
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                if (Unit* target = owner->GetSelectedUnit()) {
                    int32 spellDamage = owner->CalculateSpellDamageWithRatio(SPELL_SCHOOL_MASK_SHADOW, 0.53f);
                    me->CastCustomSpell(SPELL_DEMONBOLT, SPELLVALUE_BASE_POINT0, spellDamage, target);
                    _events.ScheduleEvent(1, 2000);
                }
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    Player* owner;
    bool alreadyFollowing;
    int32 shadow;
};


struct npc_pet_darkhound : public ScriptedAI
{
    npc_pet_darkhound(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    void InitializeAI() override { }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }


    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
            owner->CastSpell(owner, 83029);
    }


private:
    bool _initAttack;
};

void AddSC_warlock_pet_scripts()
{
    RegisterCreatureAI(npc_pet_warlock_bomber);
    RegisterCreatureAI(npc_pet_warlock_felguard);
    RegisterCreatureAI(npc_pet_warlock_darkglare);
    RegisterCreatureAI(npc_pet_warlock_wildimp);
    RegisterCreatureAI(npc_pet_darkhound);
    RegisterCreatureAI(npc_pet_warlock_vilefiend);
    RegisterCreatureAI(npc_pet_warlock_demonic_tyrant);
}
