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

#include "PassiveAI.h"
#include "PetAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "TotemAI.h"

enum PriestSpells
{
    SPELL_PRIEST_GLYPH_OF_SHADOWFIEND = 58228,
    SPELL_PRIEST_GLYPH_OF_SHADOWFIEND_MANA = 58227,
    SPELL_PRIEST_SHADOWFIEND_DODGE = 8273,
    SPELL_PRIEST_LIGHTWELL_CHARGES = 59907,

    RUNE_PRIEST_IDOL_OF_CTHUN_MIND_FLAY = 901224,
    RUNE_PRIEST_IDOL_OF_CTHUN_MIND_SEAR = 901226,
    RUNE_PRIEST_IDOL_OF_YOGG_SARON_VOID_SPIKE = 901378,
};

enum ShamanEvents
{
    // IDOL OF CTHUN
    EVENT_PRIEST_CAST_SPELL = 1,
};

struct npc_pet_pri_lightwell : public TotemAI
{
    npc_pet_pri_lightwell(Creature* creature) : TotemAI(creature) { }

    void InitializeAI() override
    {
        if (Unit* owner = me->ToTempSummon()->GetSummonerUnit())
        {
            uint32 hp = uint32(owner->GetMaxHealth() * 0.3f);
            me->SetMaxHealth(hp);
            me->SetHealth(hp);
            me->SetLevel(owner->getLevel());
            me->CastSpell(me, SPELL_PRIEST_LIGHTWELL_CHARGES);
        }
        TotemAI::InitializeAI();
    }
};

struct npc_pet_pri_shadowfiend : public PetAI
{
    npc_pet_pri_shadowfiend(Creature* creature) : PetAI(creature) { }

    void Reset() override
    {
        PetAI::Reset();
        if (!me->HasAura(SPELL_PRIEST_SHADOWFIEND_DODGE))
            me->AddAura(SPELL_PRIEST_SHADOWFIEND_DODGE, me);

        if (Unit* target = me->SelectNearestTarget(15.0f))
            AttackStart(target);
    }

    void JustDied(Unit* /*killer*/) override
    {
        if (me->IsSummon())
            if (Unit* owner = me->ToTempSummon()->GetSummonerUnit())
                if (owner->HasAura(SPELL_PRIEST_GLYPH_OF_SHADOWFIEND))
                    owner->CastSpell(owner, SPELL_PRIEST_GLYPH_OF_SHADOWFIEND_MANA, true);
    }
};

struct npc_pri_idol_of_cthun_tendril : public ScriptedAI
{
    npc_pri_idol_of_cthun_tendril(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    void EnterCombat(Unit*) override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_PRIEST_CAST_SPELL, 0);
    }

    void InitializeAI() override { }

    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (!me->IsInCombat())
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                    if (Unit* target = owner->GetSelectedUnit())
                        if (me->CanCreatureAttack(target))
                            AttackStart(target);
            _initAttack = false;
        }

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (_events.ExecuteEvent() == EVENT_PRIEST_CAST_SPELL)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                if (Unit* target = owner->GetSelectedUnit())
                {
                    int32 amount = owner->CalculateDamageAmount(owner, target, SPELL_SCHOOL_MASK_SHADOW, 86.15, RUNE_PRIEST_IDOL_OF_CTHUN_MIND_FLAY);

                    me->CastCustomSpell(RUNE_PRIEST_IDOL_OF_CTHUN_MIND_FLAY, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
                }
            _events.ScheduleEvent(EVENT_PRIEST_CAST_SPELL, 5000);
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    bool _initAttack;
};

struct npc_pri_idol_of_cthun_lasher : public ScriptedAI
{
    npc_pri_idol_of_cthun_lasher(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    void EnterCombat(Unit*) override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_PRIEST_CAST_SPELL, 0);
    }

    void InitializeAI() override { }

    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (!me->IsInCombat())
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                    if (Unit* target = owner->GetSelectedUnit())
                        if (me->CanCreatureAttack(target))
                            AttackStart(target);
            _initAttack = false;
        }

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (_events.ExecuteEvent() == EVENT_PRIEST_CAST_SPELL)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                if (Unit* target = owner->GetSelectedUnit())
                {
                    int32 amount = owner->CalculateDamageAmount(owner, target, SPELL_SCHOOL_MASK_SHADOW, 86.15, RUNE_PRIEST_IDOL_OF_YOGG_SARON_VOID_SPIKE);

                    me->CastCustomSpell(RUNE_PRIEST_IDOL_OF_YOGG_SARON_VOID_SPIKE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
                }
            _events.ScheduleEvent(EVENT_PRIEST_CAST_SPELL, 5000);
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    bool _initAttack;
};

struct npc_pri_idol_of_yogg_saron : public ScriptedAI
{
    npc_pri_idol_of_yogg_saron(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    void EnterCombat(Unit*) override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_PRIEST_CAST_SPELL, 0);
    }

    void InitializeAI() override { }

    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (!me->IsInCombat())
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                    if (Unit* target = owner->GetSelectedUnit())
                        if (me->CanCreatureAttack(target))
                            AttackStart(target);
            _initAttack = false;
        }

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (_events.ExecuteEvent() == EVENT_PRIEST_CAST_SPELL)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                if (Unit* target = owner->GetSelectedUnit())
                {
                    int32 amount = owner->CalculateDamageAmount(owner, target, SPELL_SCHOOL_MASK_SHADOW, 86.15, RUNE_PRIEST_IDOL_OF_YOGG_SARON_VOID_SPIKE);

                    me->CastCustomSpell(RUNE_PRIEST_IDOL_OF_YOGG_SARON_VOID_SPIKE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
                }

            _events.ScheduleEvent(EVENT_PRIEST_CAST_SPELL, 2000);
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    bool _initAttack;
};

void AddSC_priest_pet_scripts()
{
    RegisterCreatureAI(npc_pet_pri_lightwell);
    RegisterCreatureAI(npc_pet_pri_shadowfiend);
    RegisterCreatureAI(npc_pri_idol_of_cthun_tendril);
    RegisterCreatureAI(npc_pri_idol_of_cthun_lasher);
    RegisterCreatureAI(npc_pri_idol_of_yogg_saron);

}
