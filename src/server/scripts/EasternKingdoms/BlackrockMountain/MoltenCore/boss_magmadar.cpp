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
#include "SpellScript.h"
#include "molten_core.h"

enum Texts
{
    EMOTE_FRENZY                    = 0,
};

enum Spells
{
    SPELL_FRENZY                    = 2000064,
    SPELL_STOMP                     = 2000072,

    SPELL_MAGMA_SPIT                = 19449,
    SPELL_MORTAL_WOUND              = 2000053,
    SPELL_PANIC                     = 19408,
    SPELL_LAVA_BOMB                 = 19411,                    // This calls a dummy server side effect that cast spell 20494 to spawn GO 177704 for 30s
    SPELL_LAVA_BOMB_EFFECT          = 20494,                    // Spawns trap GO 177704 which triggers 19428
    SPELL_LAVA_BOMB_RANGED          = 20474,                    // This calls a dummy server side effect that cast spell 20495 to spawn GO 177704 for 60s
    SPELL_LAVA_BOMB_RANGED_EFFECT   = 20495,                    // Spawns trap GO 177704 which triggers 19428

    SPELL_MAGMADAR_JUMP_TARGET      = 2000071,
    SPELL_PRE_CAST_STOMP            = 2000073,
};

enum Events
{
    EVENT_FRENZY                    = 1,
    EVENT_PANIC,
    EVENT_LAVA_BOMB,
    EVENT_LAVA_BOMB_RANGED,
    EVENT_MORTAL_WOUND,

    EVENT_JUMP_ON_PLAYER,
    EVENT_TARGET_JUMP_ON_PLAYER,

    EVENT_STOMP_ON_PLAYER,

};

constexpr float MELEE_TARGET_LOOKUP_DIST = 10.0f;

class boss_magmadar : public CreatureScript
{
public:
    boss_magmadar() : CreatureScript("boss_magmadar") {}

    struct boss_magmadarAI : public BossAI
    {

        boss_magmadarAI(Creature* creature) : BossAI(creature, DATA_MAGMADAR) {}

        void ScheduleEvents() {
            events.ScheduleEvent(EVENT_MORTAL_WOUND, 5500);
            events.ScheduleEvent(EVENT_FRENZY, 19000);
            events.ScheduleEvent(EVENT_PANIC, 9500);
            events.ScheduleEvent(EVENT_LAVA_BOMB, 12000);
            events.ScheduleEvent(EVENT_LAVA_BOMB_RANGED, 15000);
            if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC) {
                events.ScheduleEvent(EVENT_TARGET_JUMP_ON_PLAYER, 32000);
            }
        }

        void EnterCombat(Unit* /*victim*/) override
        {
            _EnterCombat();
            ScheduleEvents();
        }

        void DamageTaken(Unit*, uint32& damage, DamageEffectType, SpellSchoolMask) override
        {
            if (GetDifficulty() != RAID_DIFFICULTY_10_25MAN_MYTHIC)
                return;

            if (me->HealthBelowPctDamaged(75, damage) && !firstSummon)
            {
                for (size_t i = 0; i < 2; i++)
                {
                    TempSummon* summon = me->SummonCreature(11671, me->GetPosition(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
                    summon->AI()->DoZoneInCombat();
                }

                firstSummon = true;
            }

            if (me->HealthBelowPctDamaged(50, damage) && !secondSummon)
            {
                for (size_t i = 0; i < 2; i++)
                {
                    TempSummon* summon = me->SummonCreature(11671, me->GetPosition(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
                    summon->AI()->DoZoneInCombat();
                }
                secondSummon = true;
            }

            if (me->HealthBelowPctDamaged(25, damage) && !lastSummon)
            {
                for (size_t i = 0; i < 2; i++)
                {
                    TempSummon* summon = me->SummonCreature(11671, me->GetPosition(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
                    summon->AI()->DoZoneInCombat();
                }

                lastSummon = true;
            }
        }

        void ExecuteEvent(uint32 eventId) override
        {
            switch (eventId)
            {
                case EVENT_TARGET_JUMP_ON_PLAYER:
                {
                    events.Reset();
                    std::list<Unit*> targets;
                    SelectTargetList(targets, [this](Unit* target)
                    {
                        return target && target->IsPlayer() && target->GetDistance(me) > MELEE_TARGET_LOOKUP_DIST && target->GetDistance(me) < 100.0f;
                    }, 1, SelectTargetMethod::Random);

                    if (!targets.empty())
                    {
                        Unit* target = targets.front();
                        jumpPosition = target->GetPosition();
                        me->CastSpell(target, SPELL_MAGMADAR_JUMP_TARGET);
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
                    Talk(EMOTE_FRENZY);
                    DoCastSelf(SPELL_STOMP);
                    ScheduleEvents();
                    break;
                }
                case EVENT_FRENZY:
                {
                    Talk(EMOTE_FRENZY);
                    DoCastSelf(SPELL_FRENZY);
                    events.RepeatEvent(19000);
                    break;
                }
                case EVENT_PANIC:
                {
                    DoCastVictim(SPELL_PANIC);
                    events.RepeatEvent(urand(31000, 38000));
                    break;
                }
                case EVENT_MORTAL_WOUND:
                {
                    DoCastVictim(SPELL_MORTAL_WOUND);
                    events.RepeatEvent(12500);
                    break;
                }
                case EVENT_LAVA_BOMB:
                {
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, MELEE_TARGET_LOOKUP_DIST, true))
                    {
                        DoCast(target, SPELL_LAVA_BOMB);
                    }

                    events.RepeatEvent(11000);
                    break;
                }
                case EVENT_LAVA_BOMB_RANGED:
                {
                    uint32 playerCount = me->GetMap()->GetPlayers().getSize();

                    std::list<Unit*> targets;
                    SelectTargetList(targets, [this](Unit* target)
                    {
                        return target && target->IsPlayer() && target->GetDistance(me) > MELEE_TARGET_LOOKUP_DIST && target->GetDistance(me) < 100.0f;
                    }, 1, SelectTargetMethod::Random);

                    if (!targets.empty())
                    {
                        DoCast(targets.front() , SPELL_LAVA_BOMB_RANGED);
                    }
                    events.RepeatEvent(12000);
                    break;
                }
            }
        }

        private:
            Position jumpPosition;
            bool firstSummon;
            bool secondSummon;
            bool lastSummon;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<boss_magmadarAI>(creature);
    }
};

// 19411 Lava Bomb
// 20474 Lava Bomb
class spell_magmadar_lava_bomb : public SpellScriptLoader
{
public:
    spell_magmadar_lava_bomb() : SpellScriptLoader("spell_magmadar_lava_bomb") {}

    class spell_magmadar_lava_bomb_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_magmadar_lava_bomb_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_LAVA_BOMB_EFFECT, SPELL_LAVA_BOMB_RANGED_EFFECT });
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
            {
                uint32 spellId = 0;
                switch (m_scriptSpellId)
                {
                    case SPELL_LAVA_BOMB:
                    {
                        spellId = SPELL_LAVA_BOMB_EFFECT;
                        break;
                    }
                    case SPELL_LAVA_BOMB_RANGED:
                    {
                        spellId = SPELL_LAVA_BOMB_RANGED_EFFECT;
                        break;
                    }
                    default:
                    {
                        return;
                    }
                }
                target->CastSpell(target, spellId, true, nullptr, nullptr, GetCaster()->GetGUID());
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_magmadar_lava_bomb_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_magmadar_lava_bomb_SpellScript();
    }
};

void AddSC_boss_magmadar()
{
    new boss_magmadar();

    // Spells
    new spell_magmadar_lava_bomb();
}
