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
    SPELL_SHADOW_IMPACT = 19716,
    SPELL_RAIN_OF_FIRE = 19717,
    SPELL_SHADOW_BOLT_RANDOM = 19728,
    SPELL_SHADOW_BOLT_VICTIM = 19729,
    SPELL_HELLFIRE = 2000054,
    SPELL_INNER_FIRE = 2000057,

    // Ads

    SPELL_STRIKE = 19730,
    SPELL_FIST_OF_RAGNAROS = 20277,
    SPELL_SUNDER_ARMOR = 15502,
    SPELL_INCREASE_DAMAGE = 2000060,
    SPELL_VOLCANO_ERUPTION = 2000062,
};

enum Events
{
    EVENT_SHADOW_IMPACT    = 1,
    EVENT_RAIN_OF_FIRE,
    EVENT_SHADOW_BOLT,
    EVENT_INNERFIRE,


    EVENT_STRIKE,
    EVENT_FIST_OF_RAGNAROS,
    EVENT_SUNDER_ARMOR,
    EVENT_INCREASE_DAMAGE,
    EVENT_TARGET_VOLCANO_ERUPTION,

};

class boss_gehennas : public CreatureScript
{
public:
    boss_gehennas() : CreatureScript("boss_gehennas") { }

    struct boss_gehennasAI : public BossAI
    {
        boss_gehennasAI(Creature* creature) : BossAI(creature, DATA_GEHENNAS) {}

        void EnterCombat(Unit* /*attacker*/) override
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_SHADOW_IMPACT, 25000);
            events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 10000);
            events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(3000, 5000));
        }

        void ExecuteEvent(uint32 eventId) override
        {
            switch (eventId)
            {
                case EVENT_SHADOW_IMPACT:
                {
                    std::list<Unit*> targets;
                    SelectTargetList(targets, 5, SelectTargetMethod::MinThreat, 100.0f, true);
                    if (!targets.empty())
                        for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                            if ((*itr) != me->GetVictim()) {
                                me->CastSpell(*itr, SPELL_SHADOW_IMPACT, true);
                            }

                    events.RepeatEvent(25000);
                    break;
                }
                case EVENT_RAIN_OF_FIRE:
                {
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, 0.0f, true))
                    {
                        DoCast(target, SPELL_RAIN_OF_FIRE, true);
                    }
                    events.RepeatEvent(6000);
                    break;
                }
                case EVENT_SHADOW_BOLT:
                {
                    if (urand(0, 1))
                    {
                        if (Unit* target = SelectTarget(SelectTargetMethod::Random, 1, 0.0f, true))
                        {
                            DoCast(target, SPELL_SHADOW_BOLT_RANDOM);
                        }
                        else
                        {
                            DoCastVictim(SPELL_SHADOW_BOLT_VICTIM);
                        }
                    }
                    else
                    {
                        DoCastVictim(SPELL_SHADOW_BOLT_VICTIM);
                    }

                    events.RepeatEvent(5000);
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<boss_gehennasAI>(creature);
    }
};


class npc_flamewaker : public CreatureScript
{
public:
    npc_flamewaker() : CreatureScript("npc_flamewaker") {}

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
            events.ScheduleEvent(EVENT_STRIKE, urand(3000, 8000));
            events.ScheduleEvent(EVENT_FIST_OF_RAGNAROS, urand(3000, 6000));
            events.ScheduleEvent(EVENT_SUNDER_ARMOR, urand(4000, 9000));
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
                    case EVENT_STRIKE:
                    {
                        DoCastVictim(SPELL_STRIKE);
                        events.RepeatEvent(urand(3000, 8000));
                        break;
                    }
                    case EVENT_FIST_OF_RAGNAROS:
                    {
                        DoCastVictim(SPELL_FIST_OF_RAGNAROS);
                        events.RepeatEvent(20000);
                        break;
                    }
                    case EVENT_SUNDER_ARMOR:
                    {
                        DoCastVictim(SPELL_SUNDER_ARMOR);
                        events.RepeatEvent(urand(4000, 9000));
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


class spell_effect_remove_hellfire : public AuraScript
{
    PrepareAuraScript(spell_effect_remove_hellfire)

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_effect_remove_hellfire::HandleEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_soaking_hellfire_finder : public SpellScript
{
    PrepareSpellScript(spell_soaking_hellfire_finder);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        if (targets.empty())
        {
        }

        caster->ToTempSummon()->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_soaking_hellfire_finder::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

void AddSC_boss_gehennas()
{
    new boss_gehennas();
    new npc_flamewaker();
    // RegisterSpellScript(spell_effect_remove_hellfire);
    // RegisterSpellScript(spell_soaking_hellfire_finder);
}
