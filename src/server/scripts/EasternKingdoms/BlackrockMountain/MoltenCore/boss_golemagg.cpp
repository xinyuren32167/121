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

#include "ObjectAccessor.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "molten_core.h"

enum Texts
{
    EMOTE_LOWHP                 = 0,
};

enum Spells
{
    // Golemagg
    SPELL_PYROBLAST             = 20228,
    SPELL_IGNITED               = 2000068,

    SPELL_EARTHQUAKE            = 19798,
    SPELL_EARTHQUAKE_MYTHIC     = 2000077,

    SPELL_GELOMAG_JUMP_TARGET   = 2000078,
    SPELL_PRE_CAST_STOMP        = 2000073,
    SPELL_STOMP                 = 2000072,

    // Core Rager
    SPELL_MANGLE                = 19820,
    SPELL_FULL_HEAL             = 17683,
    SPELL_RAGE                  = 2000079,

};


enum Events {
    EVENT_PYROBLAST = 1,
    EVENT_TARGET_JUMP_ON_PLAYER,
    EVENT_JUMP_ON_PLAYER,
    EVENT_STOMP_ON_PLAYER,
    EVENT_IGNITED,
};

class boss_golemagg : public CreatureScript
{
public:
    boss_golemagg() : CreatureScript("boss_golemagg") { }

    struct boss_golemaggAI : public BossAI
    {
        boss_golemaggAI(Creature* creature) : BossAI(creature, DATA_GOLEMAGG),
            enraged(false)
        {}

        void Reset() override
        {
            _Reset();
            enraged = false;
        }

        void EnterCombat(Unit* /*attacker*/) override
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_PYROBLAST, urand(3000, 7000));
            events.ScheduleEvent(EVENT_IGNITED, 5500);
            events.ScheduleEvent(EVENT_TARGET_JUMP_ON_PLAYER, 32000);
        }

        void DamageTaken(Unit*, uint32& damage, DamageEffectType, SpellSchoolMask) override
        {
            if (!enraged && me->HealthBelowPctDamaged(10, damage))
            {
                uint32 spellEarthquake = GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC ? SPELL_EARTHQUAKE_MYTHIC : SPELL_EARTHQUAKE;
                DoCastAOE(spellEarthquake, true);
                enraged = true;
            }
        }

        void ExecuteEvent(uint32 eventId) override
        {
            switch (eventId)
            {
                case EVENT_IGNITED:
                {
                    DoCastVictim(SPELL_IGNITED);
                    events.RepeatEvent(12500);
                    break;
                }
                case EVENT_PYROBLAST:
                {
                    DoCastRandomTarget(SPELL_PYROBLAST);
                    events.RepeatEvent(5000);
                    break;
                }
                case EVENT_TARGET_JUMP_ON_PLAYER:
                {
                    std::list<Unit*> targets;
                    SelectTargetList(targets, [this](Unit* target)
                    {
                        return target && target->IsPlayer() && target->GetDistance(me) > 10.f && target->GetDistance(me) < 100.0f;
                    }, 1, SelectTargetMethod::Random);

                    if (!targets.empty())
                    {
                        events.Reset();
                        Unit* target = targets.front();
                        jumpPosition = target->GetPosition();
                        me->CastSpell(target, SPELL_GELOMAG_JUMP_TARGET);
                        DoCastSelf(SPELL_PRE_CAST_STOMP);
                        events.ScheduleEvent(EVENT_JUMP_ON_PLAYER, 4000);
                    }
                    break;
                }
                case EVENT_JUMP_ON_PLAYER:
                {
                    float distance = me->GetDistance(jumpPosition);
                    float x = jumpPosition.GetPositionX();
                    float y = jumpPosition.GetPositionY();
                    float z = jumpPosition.GetPositionZ();

                    float speedZ = 12.f;
                    float speedXY = 40.0f;
                    float gravity = Movement::gravity;

                    float timeOfFlightVertical = (2 * speedZ) / gravity;
                    float timeToCoverDistance = distance / speedXY;
                    float totalTimeOfFlight = std::max(timeOfFlightVertical, timeToCoverDistance);
                    me->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ);
                    events.ScheduleEvent(EVENT_STOMP_ON_PLAYER, (totalTimeOfFlight * 1000) + 100);
                    break;
                }

                case EVENT_STOMP_ON_PLAYER:
                {
                    DoCastSelf(SPELL_STOMP);
                    events.ScheduleEvent(EVENT_PYROBLAST, urand(3000, 7000));
                    events.ScheduleEvent(EVENT_IGNITED, 5500);
                    if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC) {
                        events.ScheduleEvent(EVENT_TARGET_JUMP_ON_PLAYER, 32000);
                    }
                    break;
                }
            }
        }

    private:
        bool enraged;
        Position jumpPosition;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<boss_golemaggAI>(creature);
    }
};

class npc_core_rager : public CreatureScript
{
public:
    npc_core_rager() : CreatureScript("npc_core_rager") { }

    struct npc_core_ragerAI : public ScriptedAI
    {
        npc_core_ragerAI(Creature* creature) : ScriptedAI(creature),
            instance(creature->GetInstanceScript()),
            mangleTimer(7000),
            rangeCheckTimer(1000),
            timerNewVictim(12000)
        {
            if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC) {
                me->SetReactState(REACT_PASSIVE);
            }
        }

        void Reset() override
        {
            mangleTimer = 7000;               // These times are probably wrong
            rangeCheckTimer = 1000;
            timerNewVictim = 12000;
        }

        void JustDied(Unit* /*killer*/) override
        {
            Creature* creature = me->FindNearestCreature(11672, 100.f, true);
            if (creature) {
                creature->AddAura(SPELL_RAGE, creature);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
            {
                return;
            }

            // Should have no impact from unit state
            if (rangeCheckTimer <= diff)
            {
                Creature const* golemagg = ObjectAccessor::GetCreature(*me, instance->GetGuidData(DATA_GOLEMAGG));
                if (golemagg && me->GetDistance(golemagg) > 100.0f)
                {
                    instance->DoAction(ACTION_RESET_GOLEMAGG_ENCOUNTER);
                    return;
                }

                rangeCheckTimer = 1000;
            }
            else
            {
                rangeCheckTimer -= diff;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                return;
            }

            // Mangle
            if (mangleTimer <= diff)
            {
                DoCastVictim(SPELL_MANGLE);
                mangleTimer = 10000;
            }
            else
            {
                mangleTimer -= diff;
            }

            if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC) {
                if (timerNewVictim <= diff)
                {
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, 30.f, true))
                    {
                        me->AI()->AttackStart(target);
                    }
                    timerNewVictim = 12000;
                }
                else
                {
                    timerNewVictim -= diff;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        uint32 mangleTimer;
        uint32 rangeCheckTimer;
        uint32 timerNewVictim;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<npc_core_ragerAI>(creature);
    }
};

void AddSC_boss_golemagg()
{
    new boss_golemagg();
    new npc_core_rager();
}
