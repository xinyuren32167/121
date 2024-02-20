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
    SAY_SUMMON_MAJ = 0,
    SAY_ARRIVAL1_RAG = 1,
    SAY_ARRIVAL2_MAJ = 2,
    SAY_ARRIVAL3_RAG = 3,
    SAY_ARRIVAL5_RAG = 4,
    SAY_REINFORCEMENTS1 = 5,
    SAY_REINFORCEMENTS2 = 6,
    SAY_KNOCKBACK = 7,        // Text is used for SPELL_HAND_OF_RAGNAROS & SPELL_MIGHT_OF_RAGNAROS. "By fire be purged!"
    SAY_WRATH = 8,
    SAY_KILL = 9,
    SAY_MAGMABURST = 10,
};

enum Spells
{
    SPELL_WRATH_OF_RAGNAROS = 20566,
    SPELL_MAGMA_BLAST = 20565,    // Ranged attack
    SPELL_SONS_OF_FLAME_DUMMY = 21108,    // Server side effect
    SPELL_RAGSUBMERGE = 21107,    // Stealth aura
    SPELL_RAGNA_SUBMERGE_VISUAL = 20567,    // Visual for submerging into lava
    SPELL_RAGEMERGE = 20568,
    SPELL_RAGNAROS_SUBMERGE_EFFECT = 21859,    // Applies pacify state and applies all schools immunity (server side)
    SPELL_ELEMENTAL_FIRE_KILL = 19773,    // Spell is used only on Majordomo
    SPELL_MIGHT_OF_RAGNAROS = 21154,
    SPELL_SUMMON_SONS_FLAME = 21108,    // Trigger the eight spells summoning the Son of Flame adds


    // Custom

    SPELL_FIRE_BLAST = 2000093,
    SPELL_CAST_ON_TRIGGER_MAGMA_TRAP = 2000095,
    SPELL_PRE_HIT_MAGMA_TRAP_EFFECT = 2000091,
    SPELL_MAGMA_TRAP_EFFECT = 2000096,
    SPELL_INTENSE_HEAT = 2000098,
    SPELL_VISUAL_FLAME_TSUNAMI = 57494,
    SPELL_FIRE_NOVA_SUMMON_LAVA_ELEMENTAL = 2000100,


    SPELL_LAVA_BURST_A = 21886,
    SPELL_LAVA_BURST_B = 21900,
    SPELL_LAVA_BURST_C = 21901,
    SPELL_LAVA_BURST_D = 21902,
    SPELL_LAVA_BURST_E = 21903,
    SPELL_LAVA_BURST_F = 21905,
    SPELL_LAVA_BURST_G = 21906,
    SPELL_LAVA_BURST_H = 21907,
    SPELL_LAVA_BURST_TRAP = 21158,

    SPELL_SUMMON_SON_OF_FLAME_A = 21117,
    SPELL_SUMMON_SON_OF_FLAME_B = 21110,
    SPELL_SUMMON_SON_OF_FLAME_C = 21111,
    SPELL_SUMMON_SON_OF_FLAME_D = 21112,
    SPELL_SUMMON_SON_OF_FLAME_E = 21113,
    SPELL_SUMMON_SON_OF_FLAME_F = 21114,
    SPELL_SUMMON_SON_OF_FLAME_G = 21115,
    SPELL_SUMMON_SON_OF_FLAME_H = 21116
};

enum Events
{
    EVENT_ERUPTION = 1,
    EVENT_WRATH_OF_RAGNAROS,
    EVENT_MIGHT_OF_RAGNAROS,
    EVENT_MAGMA_BLAST,
    EVENT_SUBMERGE,

    // Custom
    EVENT_FIRE_BLAST,
    EVENT_MAGMA_TRAP_SUMMON_TRIGGER,
    EVENT_FIRE_ON_MAGMA_TRAP,
    EVENT_SUMMON_LAVA_WAVE,

    EVENT_SUMMON_LAVA_ELEMENTAL_SPHERE,

    // Submerge
    EVENT_EMERGE,
    EVENT_TRIGGER_FLAMING_CINDER,

    // Intro
    EVENT_INTRO_SAY,
    EVENT_INTRO_MAKE_ATTACKABLE,

    // Misc
    EVENT_RESET_KNOCKBACK_EMOTE,
};

enum Creatures
{
    NPC_SON_OF_FLAME = 12143,

    NPC_FLAME_OF_RAGNAROS = 13148,
    NPC_TRIGGER_MAGMA_TRAP = 129990,
    NPC_MAGMA_TRAP = 129991,
    NPC_LAVA_WAVE = 129992,
    NPC_FLAME_SPHERE = 129993,
};

enum Misc
{
    MAX_SON_OF_FLAME_COUNT = 8,
    MAX_SON_OF_FLAME_COUNT_20 = 16,

    // Event phase
    PHASE_INTRO = 1,        // Intro events with Majordomu
    PHASE_EMERGED = 2,        // events which are executed while Ragnaros is visible
    PHASE_SUBMERGED = 3,        // events which are executed while Ragnaros is submerged (not visible)
};

constexpr float DEATH_ORIENTATION = 4.0f;

std::vector<Position> randomPosition = {
    { 809.991516, -827.103516, -228, 0.239236 },
    { 816.431763, -814.661377, -228, 5.917666 },
    { 822.158691, -805.792114, -228, 5.595655 },
    { 832.530762, -798.538269, -228, 5.140127 },
    { 844.593628, -796.655750, -228, 4.723869 },
    { 856.208435, -800.358459, -228, 4.240852 },
    { 865.782227, -809.684998, -228, 3.659657 },
    { 869.610901, -820.611877, -228, 3.490796 },
    { 872.576660, -833.548096, -228, 3.277953 },
    { 871.853455, -846.316162, -228, 2.842056 },
    { 867.085388, -865.178223, -228, 2.276569 },
    { 852.085388, -872.082153, -228, 1.862400 },
    { 840.322327, -875.814331, -228, 1.611072 },
    { 823.429565, -874.685242, -228, 1.383307 },
};

class boss_ragnaros : public CreatureScript
{
public:
    boss_ragnaros() : CreatureScript("boss_ragnaros") {}

    struct boss_ragnarosAI : public BossAI
    {
        boss_ragnarosAI(Creature* creature) : BossAI(creature, DATA_RAGNAROS),
            _isIntroDone(false),
            _hasYelledMagmaBurst(false),
            _processingMagmaBurst(false),
            _hasSubmergedOnce(false),
            _isKnockbackEmoteAllowed(true)
        {
        }

        void Reset() override
        {
            _Reset();

            // Never reset intro events!
            if (_isIntroDone && !(extraEvents.GetPhaseMask() & (1 << (PHASE_INTRO - 1))))
            {
                extraEvents.Reset();
                extraEvents.SetPhase(PHASE_EMERGED);
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveUnitFlag(UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetImmuneToAll(false);
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
            }

            _hasYelledMagmaBurst = false;
            _processingMagmaBurst = false;
            _hasSubmergedOnce = false;
            _isKnockbackEmoteAllowed = true;
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
            me->SetControlled(true, UNIT_STATE_ROOT);
            _lavaBurstGUIDS.clear();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType, SpellSchoolMask) override
        {
            if (events.IsInPhase(PHASE_SUBMERGED) && damage >= me->GetHealth())
            {
                damage = 0;
            }
        }

        void DoAction(int32 action) override
        {
            if (action == ACTION_FINISH_RAGNAROS_INTRO)
            {
                extraEvents.SetPhase(PHASE_INTRO);
                extraEvents.ScheduleEvent(EVENT_INTRO_SAY, 5s, 0, PHASE_INTRO);
            }
        }

        void JustSummoned(Creature* summon) override
        {
            BossAI::JustSummoned(summon);
            if (summon->GetEntry() == NPC_SON_OF_FLAME)
            {
                DoZoneInCombat(summon);
            }
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) override
        {
            summons.Despawn(summon);
            if (events.IsInPhase(PHASE_SUBMERGED) && !summons.HasEntry(NPC_SON_OF_FLAME))
            {
                HandleEmerge();
            }
        }

        void EnterCombat(Unit* /*who*/) override
        {
            events.SetPhase(PHASE_EMERGED);
            DoCastSelf(SPELL_INTENSE_HEAT);
            ScheduleCombatEvents();
        }

        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();
            extraEvents.Reset();
            me->SetFacingTo(DEATH_ORIENTATION);
        }

        void KilledUnit(Unit* victim) override
        {
            if (roll_chance_i(25) && victim->IsPlayer())
            {
                Talk(SAY_KILL);
            }
        }

        void AttackStart(Unit* target) override
        {
            if (target && me->Attack(target, true))
            {
                DoStartNoMovement(target);
            }
        }

        void EnterEvadeMode(EvadeReason why) override
        {
            if (!me->GetThreatMgr().GetThreatList().empty())
            {
                if (!_processingMagmaBurst)
                {
                    // Boss try to evade, but still got some targets on threat list - it means that none of these targets are in melee range - cast magma blast
                    _processingMagmaBurst = true;
                    events.ScheduleEvent(EVENT_MAGMA_BLAST, 4s, PHASE_EMERGED, PHASE_EMERGED);
                }
            }
            else
            {
                BossAI::EnterEvadeMode(why);
            }
        }

        bool CanAIAttack(Unit const* victim) const override
        {
            return me->IsWithinMeleeRange(victim);
        }

        void SummonSonsOfFlame() {

            uint32 count = GetPlayersCount() > 20 ? MAX_SON_OF_FLAME_COUNT_20 : MAX_SON_OF_FLAME_COUNT;

            for (size_t i = 0; i < count; i++)
            {
                const Position spawnPos = randomPosition[urand(1, 13)];
                me->SummonCreature(NPC_SON_OF_FLAME, spawnPos, TEMPSUMMON_MANUAL_DESPAWN);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!extraEvents.Empty())
            {
                extraEvents.Update(diff);

                while (uint32 const eventId = extraEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                            // Intro events
                        case EVENT_INTRO_SAY:
                        {
                            Talk(SAY_ARRIVAL5_RAG);
                            extraEvents.ScheduleEvent(EVENT_INTRO_MAKE_ATTACKABLE, 2500ms, 0, PHASE_INTRO);
                            break;
                        }
                        case EVENT_INTRO_MAKE_ATTACKABLE:
                        {
                            _isIntroDone = true;
                            extraEvents.SetPhase(PHASE_EMERGED);
                            me->RemoveAurasDueToSpell(SPELL_RAGNAROS_SUBMERGE_EFFECT);
                            me->RemoveUnitFlag(UNIT_FLAG_NON_ATTACKABLE);
                            me->SetImmuneToAll(false);
                            me->SetReactState(REACT_AGGRESSIVE);
                            DoZoneInCombat();
                            break;
                        }
                        // Submerge events
                        case EVENT_EMERGE:
                        {
                            HandleEmerge();
                            break;
                        }
                        case EVENT_RESET_KNOCKBACK_EMOTE:
                        {
                            _isKnockbackEmoteAllowed = true;
                            break;
                        }
                    }
                }
            }

            if (!UpdateVictim())
            {
                if (!_processingMagmaBurst)
                {
                    return;
                }
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                return;
            }

            // Base combat events - (mainly emerge phase)
            while (uint32 const eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_MAGMA_TRAP_SUMMON_TRIGGER:
                {
                    std::list<Unit*> targets;
                    SelectTargetList(targets, [this](Unit* target)
                    {
                        return target && target->IsPlayer() && target->GetDistance(me) > 10.f && target->GetDistance(me) < 100.0f;
                    }, 1, SelectTargetMethod::Random);

                    if (!targets.empty())
                    {

                        Unit* target = targets.front();
                        TempSummon* summon = me->SummonCreature(NPC_TRIGGER_MAGMA_TRAP, target->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 4000);
                        summon->AddAura(SPELL_PRE_HIT_MAGMA_TRAP_EFFECT, summon);
                        Creature* magmaTrap = me->FindNearestCreature(NPC_TRIGGER_MAGMA_TRAP, 100.f, true);
                        me->CastSpell(magmaTrap, SPELL_CAST_ON_TRIGGER_MAGMA_TRAP);
                    }
                    events.RepeatEvent(19000);
                    break;
                }
                case EVENT_FIRE_BLAST:
                {
                    std::list<Unit*> targets;
                    SelectTargetList(targets, [this](Unit* target)
                    {
                        return target && target->IsPlayer() && target != me->GetVictim();
                    }, 1, SelectTargetMethod::Random);

                    if (!targets.empty())
                    {
                        DoCast(targets.front(), SPELL_FIRE_BLAST);
                    }
                    events.RepeatEvent(8000);
                    break;
                }
                case EVENT_SUMMON_LAVA_ELEMENTAL_SPHERE:
                {
                    std::list<Unit*> targets;
                    uint32 total = round(GetPlayersCount() / 2);
                    SelectTargetList(targets, [this](Unit* target)
                    {
                        return target && target->IsPlayer();
                    }, total, SelectTargetMethod::Random);

                    if (!targets.empty())
                        for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                        {
                            me->SummonCreature(NPC_FLAME_SPHERE, (*itr)->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 8000);
                            (*itr)->AddAura(55928, *itr);
                        }

                    break;
                }
                case EVENT_SUMMON_LAVA_WAVE:
                {
                    const Position lavaWavePos = randomPosition[urand(1, 13)];
                    TempSummon* summon = me->SummonCreature(NPC_LAVA_WAVE, lavaWavePos, TEMPSUMMON_MANUAL_DESPAWN);
                    summon->AddAura(SPELL_VISUAL_FLAME_TSUNAMI, summon);
                    summon->AddAura(57492, summon);
                    summon->SetFaction(FACTION_MONSTER);
                    summon->SetCanFly(true);
                    summon->SetDisableGravity(true);
                    events.RepeatEvent(23000);
                    break;
                }
                case EVENT_WRATH_OF_RAGNAROS:
                {
                    DoCastVictim(SPELL_WRATH_OF_RAGNAROS);
                    Talk(SAY_WRATH);
                    events.RepeatEvent(12500);
                    break;
                }
                case EVENT_MAGMA_BLAST:
                {
                    _processingMagmaBurst = false;

                    if (!IsVictimWithinMeleeRange())
                    {
                        DoCastRandomTarget(SPELL_MAGMA_BLAST);

                        if (!_hasYelledMagmaBurst)
                        {
                            Talk(SAY_MAGMABURST);
                            _hasYelledMagmaBurst = true;
                        }
                    }

                    break;
                }
                case EVENT_SUBMERGE:
                    {
                        events.CancelEventGroup(PHASE_EMERGED);
                        events.SetPhase(PHASE_SUBMERGED);
                        extraEvents.SetPhase(PHASE_SUBMERGED);
                        me->SetReactState(REACT_PASSIVE);
                        me->InterruptNonMeleeSpells(false);
                        me->AttackStop();
                        DoResetThreat();
                        me->SetUnitFlag(UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
                        DoCastSelf(SPELL_RAGNA_SUBMERGE_VISUAL, true);

                        Talk(_hasSubmergedOnce ? SAY_REINFORCEMENTS2 : SAY_REINFORCEMENTS1);

                        // DoCastAOE(SPELL_SUMMON_SONS_FLAME);
                        SummonSonsOfFlame();

                        if (!_hasSubmergedOnce)
                        {
                            _hasSubmergedOnce = true;
                        }

                        extraEvents.ScheduleEvent(EVENT_EMERGE, 80s, PHASE_SUBMERGED, PHASE_SUBMERGED);
                        break;
                    }
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                {
                    return;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap extraEvents;
        bool _isIntroDone;
        bool _hasYelledMagmaBurst;
        bool _processingMagmaBurst;
        bool _hasSubmergedOnce;
        bool _isKnockbackEmoteAllowed;  // Prevents possible text overlap

        GuidSet _lavaBurstGUIDS;

        void HandleEmerge()
        {
            if (events.IsInPhase(PHASE_EMERGED))
            {
                return;
            }

            events.SetPhase(PHASE_EMERGED);

            if (GetDifficulty() == RAID_DIFFICULTY_10_25MAN_MYTHIC)
            {
                events.ScheduleEvent(EVENT_SUMMON_LAVA_ELEMENTAL_SPHERE, 5s, PHASE_EMERGED, PHASE_EMERGED);
            }

            ScheduleCombatEvents();
            extraEvents.CancelEventGroup(PHASE_SUBMERGED);
            extraEvents.SetPhase(PHASE_EMERGED);

            me->SetReactState(REACT_AGGRESSIVE);
            me->RemoveUnitFlag(UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
            me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);

            me->RemoveAurasDueToSpell(SPELL_RAGNA_SUBMERGE_VISUAL);

            if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, 0.0f, true))
            {
                AttackStart(target);
            }
        }

        void ScheduleCombatEvents()
        {
            events.RescheduleEvent(EVENT_SUMMON_LAVA_WAVE, 23s, PHASE_EMERGED, PHASE_EMERGED);
            events.RescheduleEvent(EVENT_MAGMA_TRAP_SUMMON_TRIGGER, 19s, PHASE_EMERGED, PHASE_EMERGED);
            events.RescheduleEvent(EVENT_FIRE_BLAST, 8s, PHASE_EMERGED, PHASE_EMERGED);
            events.RescheduleEvent(EVENT_WRATH_OF_RAGNAROS, 5s, PHASE_EMERGED, PHASE_EMERGED);
            events.RescheduleEvent(EVENT_SUBMERGE, 160s, PHASE_EMERGED, PHASE_EMERGED);
        }

        bool IsVictimWithinMeleeRange() const
        {
            return me->GetVictim() && me->IsWithinMeleeRange(me->GetVictim());
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<boss_ragnarosAI>(creature);
    }
};


class spell_magma_fire_hit : public SpellScript
{
    PrepareSpellScript(spell_magma_fire_hit);

    void HandleScript(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        Unit* ragnaros = GetCaster();
        TempSummon* summon = ragnaros->SummonCreature(NPC_MAGMA_TRAP, GetHitUnit()->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 300000);
        summon->SetFaction(FACTION_MONSTER);
        summon->AddAura(SPELL_MAGMA_TRAP_EFFECT, summon);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_magma_fire_hit::HandleScript, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_fire_nova_hit_distance : public SpellScript
{
    PrepareSpellScript(spell_fire_nova_hit_distance);

    void HandleScript(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        int32 damage = GetHitDamage();
        Unit* sphere = GetCaster();
        Unit* hitTarget = GetHitUnit();

        float distance = sphere->GetDistance(hitTarget);
        int32 finalDamage = damage / distance;

        SetHitDamage(finalDamage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_fire_nova_hit_distance::HandleScript, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};


class npc_lava_wave : public CreatureScript
{
public:
    npc_lava_wave() : CreatureScript("npc_lava_wave") { }

    struct npc_lava_waveAI : public ScriptedAI
    {
        npc_lava_waveAI(Creature* creature) : ScriptedAI(creature),
            instance(creature->GetInstanceScript()),
            startingMoveTimer(3500)
        { }

        void Reset() override
        {
            me->CombatStop(true);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
            startingMoveTimer = 3500;
        }

        void UpdateAI(uint32 diff) override
        {
            if (startingMoveTimer <= diff)
            {
                Position pos = me->GetFirstCollisionPosition(100.f, 0);
                me->GetMotionMaster()->MovePoint(0, pos);
            }
            else
            {
                startingMoveTimer -= diff;
            }
        }

    private:
        InstanceScript* instance;
        uint32 startingMoveTimer;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<npc_lava_waveAI>(creature);
    }
};

class npc_flame_sphere : public CreatureScript
{
public:
    npc_flame_sphere() : CreatureScript("npc_flame_sphere") { }

    struct npc_flame_sphereAI : public ScriptedAI
    {
        npc_flame_sphereAI(Creature* creature) : ScriptedAI(creature),
            instance(creature->GetInstanceScript()),
            startSummonLavaElemental(5000)
        { }

        void Reset() override
        {
            me->CombatStop(true);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
            startSummonLavaElemental = 5000;
        }

        void UpdateAI(uint32 diff) override
        {
            if (startSummonLavaElemental <= diff)
            {
                DoCastSelf(SPELL_FIRE_NOVA_SUMMON_LAVA_ELEMENTAL);
                me->DespawnOrUnsummon();
            }
            else
            {
                startSummonLavaElemental -= diff;
            }
        }

    private:
        InstanceScript* instance;
        uint32 startSummonLavaElemental;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<npc_flame_sphereAI>(creature);
    }
};


void AddSC_boss_ragnaros()
{
    new boss_ragnaros();
    new npc_lava_wave();
    RegisterSpellScript(spell_magma_fire_hit);
    RegisterSpellScript(spell_fire_nova_hit_distance);
}
