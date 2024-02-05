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
    SPELL_IMPENDING_DOOM_MYTHIC    = 2000049,

    SPELL_LUCIFRON_CURSE           = 19703,
    SPELL_SHADOW_MARK              = 2000050,
    SPELL_SHADOW_MARK_MYTHIC       = 2000051,
    SPELL_DEATH_AND_DECAY          = 2000052,

    SPELL_SHADOW_SHOCK             = 20603,

    NPC_FLAMEWAKLER = 12119,
};

enum Events
{
    EVENT_IMPENDING_DOOM    = 1,
    EVENT_LUCIFRON_CURSE    = 2,
    EVENT_SHADOW_SHOCK      = 3,
    EVENT_SHADOW_MARK       = 4,
    EVENT_SUMMON_FLAMWAKLER_PROTECTOR = 5,
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
            events.ScheduleEvent(EVENT_SHADOW_MARK, 5500);
            events.ScheduleEvent(EVENT_IMPENDING_DOOM, urand(6000, 11000));
            events.ScheduleEvent(EVENT_LUCIFRON_CURSE, urand(11000, 14000));
            events.ScheduleEvent(EVENT_SHADOW_SHOCK, 5000);
            if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC) {
                events.ScheduleEvent(EVENT_SUMMON_FLAMWAKLER_PROTECTOR, 60000);
            }
        }

        void ExecuteEvent(uint32 eventId) override
        {
            switch (eventId)
            {
                case EVENT_IMPENDING_DOOM:
                {
                    if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC)
                        DoCastRandomTarget(SPELL_IMPENDING_DOOM_MYTHIC);
                    else
                        DoCastRandomTarget(SPELL_IMPENDING_DOOM);

                    events.RepeatEvent(20000);
                    break;
                }
                case EVENT_LUCIFRON_CURSE:
                {
                    DoCastRandomTarget(SPELL_LUCIFRON_CURSE);
                    events.RepeatEvent(20000);
                    break;
                }
                case EVENT_SHADOW_SHOCK:
                {
                    DoCastRandomTarget(SPELL_SHADOW_SHOCK);
                    events.RepeatEvent(5000);
                    break;
                }
                case EVENT_SHADOW_MARK:
                {
                    if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC)
                        DoCastRandomTarget(SPELL_SHADOW_MARK_MYTHIC);
                    else
                        DoCastRandomTarget(SPELL_SHADOW_MARK);
                    events.RepeatEvent(13000);
                    break;
                }
                case EVENT_SUMMON_FLAMWAKLER_PROTECTOR:
                {
                    TempSummon* summon = me->SummonCreature(NPC_FLAMEWAKLER, me->GetPosition(), TEMPSUMMON_TIMED_DESPAWN_OOC_ALIVE, 5000);
                    summon->AI()->DoZoneInCombat();
                    events.RepeatEvent(60000);
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

class spell_impending_doom : public AuraScript
{
    PrepareAuraScript(spell_impending_doom);


    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;

        caster->CastSpell(GetTarget(), SPELL_DEATH_AND_DECAY, true);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_impending_doom::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_boss_lucifron()
{
    new boss_lucifron();
    RegisterSpellScript(spell_impending_doom);
}
