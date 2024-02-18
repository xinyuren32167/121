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
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "molten_core.h"

enum Emotes
{
    EMOTE_SERVICE                   = 0
};

enum Spells
{
    SPELL_IGNITE_MANA               = 19659,
    SPELL_LIVING_BOMB               = 20475,
    SPELL_ARMAGEDDON                = 20478,
    SPELL_FIRE_AURA                 = 2000069,
    SPELL_FIRE_NOVA                 = 2000070,
    SPELL_IGNITED                   = 2000068,
    SPELL_HELLFIRE                   = 2000076,
};

enum Npc {
    NPC_EMBER = 300300
};

enum Events
{
    EVENT_INFERNO                   = 1,
    EVENT_IGNITE_MANA,
    EVENT_LIVING_BOMB,
    EVENT_IGNITED,
    EVENT_FIRE_NOVA,

    // Ads
    EVENT_HELLFIRE,
};

class boss_baron_geddon : public CreatureScript
{
public:
    boss_baron_geddon() : CreatureScript("boss_baron_geddon") { }

    struct boss_baron_geddonAI : public BossAI
    {
        boss_baron_geddonAI(Creature* creature) : BossAI(creature, DATA_GEDDON),
            armageddonCasted(false)
        {
        }

        void Reset() override
        {
            _Reset();
            armageddonCasted = false;
        }

        void EnterCombat(Unit* /*attacker*/) override
        {
            _EnterCombat();
            DoCastSelf(SPELL_FIRE_AURA);
            events.ScheduleEvent(EVENT_IGNITE_MANA, urand(7000, 19000));
            events.ScheduleEvent(EVENT_LIVING_BOMB, 15000);
            events.ScheduleEvent(EVENT_IGNITED, 5500);

            if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC) {
                events.ScheduleEvent(EVENT_FIRE_NOVA, 30000);
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*dmgType*/, SpellSchoolMask /*school*/) override
        {
            // If boss is below 2% hp - cast Armageddon
            if (!armageddonCasted && damage < me->GetHealth() && me->HealthBelowPctDamaged(2, damage))
            {
                me->StopMoving();
                if (me->CastSpell(me, SPELL_ARMAGEDDON, TRIGGERED_FULL_MASK) == SPELL_CAST_OK)
                {
                    Talk(EMOTE_SERVICE);
                    armageddonCasted = true;
                }
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
                case EVENT_FIRE_NOVA:
                {
                    DoCastVictim(2000070);
                    events.RepeatEvent(30000);
                    break;
                }
                case EVENT_IGNITE_MANA:
                {
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, 0.0f, true, -SPELL_IGNITE_MANA))
                    {
                        DoCast(target, SPELL_IGNITE_MANA);
                    }

                    events.RepeatEvent(urand(27000, 32000));
                    break;
                }
                case EVENT_LIVING_BOMB:
                {
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, 0.0f, true))
                    {
                        DoCast(target, SPELL_LIVING_BOMB);
                    }
                    events.RepeatEvent(urand(11000, 16000));
                    break;
                }
            }
        }

    private:
        bool armageddonCasted;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<boss_baron_geddonAI>(creature);
    }
};

class npc_ember : public CreatureScript
{
public:
    npc_ember() : CreatureScript("npc_ember") {}

    struct npc_emberAI : public ScriptedAI
    {
        npc_emberAI(Creature* creature) : ScriptedAI(creature) {}

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
            events.ScheduleEvent(EVENT_HELLFIRE, 2000);
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
                    case EVENT_HELLFIRE:
                    {
                        DoCastVictim(SPELL_HELLFIRE);
                        events.RepeatEvent(2000);
                        break;
                    }
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<npc_emberAI>(creature);
    }
};


// 20478 Armageddon
class spell_geddon_armageddon : public SpellScriptLoader
{
public:
    spell_geddon_armageddon() : SpellScriptLoader("spell_geddon_armageddon") { }

    class spell_geddon_armageddon_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_geddon_armageddon_AuraScript);

        void HandleAfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Creature* pCreatureTarget = GetTarget()->ToCreature())
            {
                pCreatureTarget->SetReactState(REACT_PASSIVE);
                pCreatureTarget->AttackStop();
            }
        }

        void HandleAfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Creature* pCreatureTarget = GetTarget()->ToCreature())
            {
                pCreatureTarget->SetReactState(REACT_AGGRESSIVE);
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_geddon_armageddon_AuraScript::HandleAfterApply, EFFECT_1, SPELL_AURA_MOD_PACIFY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_geddon_armageddon_AuraScript::HandleAfterRemove, EFFECT_1, SPELL_AURA_MOD_PACIFY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_geddon_armageddon_AuraScript();
    }
};

class spell_living_bomb_remove : public AuraScript
{
    PrepareAuraScript(spell_living_bomb_remove);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;

        Map* map = caster->GetMap();


        if (map->GetDifficulty() != RAID_DIFFICULTY_10_25MAN_MYTHIC)
            return;

        uint32 playerCount = map->GetPlayers().getSize();
        uint8 count = playerCount > 20 ? 3 : 1;

        for (size_t i = 0; i < count; i++)
        {
            TempSummon* summon = caster->SummonCreature(NPC_EMBER, GetTarget()->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 120000);

            if (summon) {
                summon->AI()->DoZoneInCombat();
            }
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_living_bomb_remove::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};


void AddSC_boss_baron_geddon()
{
    new boss_baron_geddon();
    new npc_ember();
    new spell_geddon_armageddon();

    RegisterSpellScript(spell_living_bomb_remove);
}
