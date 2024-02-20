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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "molten_core.h"

enum Spells
{
    SPELL_IMPENDING_DOOM           = 19702,
    SPELL_LUCIFRON_CURSE           = 19703,
    SPELL_SHADOW_SHOCK             = 20603,
    SPELL_WELL_OF_SOUL             = 2000061,

    // ads
    SPELL_DOMINATE_MIND = 20604,
    SPELL_CLEAVE = 20605,
    SPELL_INCREASE_DAMAGE = 2000060,
    SPELL_VOLCANO_ERUPTION = 2000062,

};

enum Events
{
    EVENT_IMPENDING_DOOM    = 1,
    EVENT_LUCIFRON_CURSE    = 2,
    EVENT_SHADOW_SHOCK      = 3,
    EVENT_SHADOW_MARK       = 4,

    // Adds
    EVENT_DOMINATE_MIND,
    EVENT_CLEAVE,
    EVENT_INCREASE_DAMAGE,
    EVENT_TARGET_VOLCANO_ERUPTION

};

class boss_lucifron : public CreatureScript
{

public:
    boss_lucifron() : CreatureScript("boss_lucifron") { }

    struct boss_lucifronAI : public BossAI
    {
        boss_lucifronAI(Creature* creature) : BossAI(creature, DATA_LUCIFRON) {}

        void Reset() override
        {
            _Reset();
            me->AttackStopSummonedUnits();
        }

        void EnterCombat(Unit* /*victim*/) override
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_IMPENDING_DOOM, urand(6000, 11000));
            events.ScheduleEvent(EVENT_SHADOW_SHOCK, 5000);

            if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC) {
                events.ScheduleEvent(EVENT_LUCIFRON_CURSE, urand(11000, 14000));
            }
        }

        void ExecuteEvent(uint32 eventId) override
        {
            switch (eventId)
            {
                case EVENT_IMPENDING_DOOM:
                {
                    DoCastRandomTarget(SPELL_IMPENDING_DOOM);
                    events.RepeatEvent(20000);
                    break;
                }

                case EVENT_LUCIFRON_CURSE:
                {
                    std::list<Unit*> targets;
                    SelectTargetList(targets, 3, SelectTargetMethod::MinThreat, 100.0f, true);
                    if (!targets.empty())
                        for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                            if ((*itr) != me->GetVictim()) {
                                me->CastSpell(*itr, SPELL_LUCIFRON_CURSE, true);
                            }

                    events.RepeatEvent(20000);
                    break;
                }
                case EVENT_SHADOW_SHOCK:
                {
                    DoCastRandomTarget(SPELL_SHADOW_SHOCK);
                    events.RepeatEvent(5000);
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<boss_lucifronAI>(creature);
    }
};


class npc_flamewaker_protector : public CreatureScript
{
public:
    npc_flamewaker_protector() : CreatureScript("npc_flamewaker_protector") {}

    struct npc_flamewakerAI : public ScriptedAI
    {
        npc_flamewakerAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            events.Reset();
        }

        void JustDied(Unit* /*killer*/) override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*victim*/) override
        {
            events.ScheduleEvent(EVENT_DOMINATE_MIND, 10000);
            events.ScheduleEvent(EVENT_CLEAVE, urand(5000, 10000));
            events.ScheduleEvent(EVENT_INCREASE_DAMAGE, 19500);

            if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC) {
                events.ScheduleEvent(EVENT_TARGET_VOLCANO_ERUPTION, 12000);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
            {
                return;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                return;
            }

            while (uint32 const eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_DOMINATE_MIND:
                {

                    Unit* target = SelectTarget(SelectTargetMethod::Random, 0, [&](Unit* target) -> bool
                    {
                        if (target->GetTypeId() != TYPEID_PLAYER)
                            return false;

                        if (me->IsWithinMeleeRange(target) || me->GetVictim() == target)
                            return false;

                        return true;
                    });

                    if (target) {
                        DoCastVictim(SPELL_DOMINATE_MIND);
                    }

                    events.RepeatEvent(10000);
                    break;
                }
                case EVENT_CLEAVE:
                {
                    DoCastVictim(SPELL_CLEAVE);
                    events.RepeatEvent(urand(5000, 10000));
                    break;
                }
                case EVENT_INCREASE_DAMAGE:
                {
                    DoCastSelf(SPELL_INCREASE_DAMAGE);
                    events.RepeatEvent(19000);
                    break;
                }
                case EVENT_TARGET_VOLCANO_ERUPTION:
                {

                    Unit* target = SelectTarget(SelectTargetMethod::Random, 0, [&](Unit* target) -> bool
                    {
                        if (target->GetTypeId() != TYPEID_PLAYER)
                            return false;

                        if (me->GetVictim() == target)
                            return false;

                        if (target->HasAura(SPELL_VOLCANO_ERUPTION))
                            return false;

                        return true;
                    });

                    if (target) {
                        DoCast(target, SPELL_VOLCANO_ERUPTION);
                    }

                    events.RepeatEvent(15000);
                    break;
                }
                }
                DoMeleeAttackIfReady();
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<npc_flamewakerAI>(creature);
    }
};

class spell_impending_doom_expired : public AuraScript
{
    PrepareAuraScript(spell_impending_doom_expired);


    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;

        caster->CastSpell(GetTarget(), SPELL_WELL_OF_SOUL);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_impending_doom_expired::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_volcano_eruption : public AuraScript
{
    PrepareAuraScript(spell_volcano_eruption);

    void HandlePeriodic(AuraEffect const*  /*aurEff*/)
    {
        GetCaster()->CastSpell(GetTarget(), 2000063);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_volcano_eruption::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};


void AddSC_boss_lucifron()
{
    new boss_lucifron();
    new npc_flamewaker_protector();
    RegisterSpellScript(spell_impending_doom_expired);
    RegisterSpellScript(spell_volcano_eruption);
}
