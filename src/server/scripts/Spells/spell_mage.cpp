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
  * Scripts for spells with SPELLFAMILY_MAGE and SPELLFAMILY_GENERIC spells used by mage players.
  * Ordered alphabetically using scriptname.
  * Scriptnames of files in this file should be prefixed with "spell_mage_".
  */

#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "SpellAuras.h"
#include "SpellAuraDefines.h"
#include "Unit.h"

enum MageSpells
{
    // Ours
    SPELL_MAGE_ARCANE_CHARGE = 81500,
    SPELL_MAGE_ARCANE_CHARGE_BUFF1 = 81501,
    SPELL_MAGE_ARCANE_CHARGE_BUFF2 = 81502,
    SPELL_MAGE_ARCANE_CHARGE_VISUAL = 81503,
    SPELL_MAGE_ARCANE_MISSILE = 42843,
    SPELL_MAGE_ARCANE_MISSILE_MOVING = 300924,
    SPELL_MAGE_ARCANE_ORB = 80016,
    SPELL_MAGE_ARCANE_ORB_DAMAGE = 80017,
    SPELL_MAGE_ARCANE_SURGE_DAMAGE = 81519,
    SPELL_MAGE_ARCHON_DAMAGE = 81552,
    SPELL_MAGE_BLACK_HOLE = 81553,
    SPELL_MAGE_BLACK_HOLE_AURA = 81554,
    SPELL_MAGE_BLACK_HOLE_TARGET_SELECT = 81559,
    SPELL_MAGE_BLAZING_BARRIER = 81526,
    SPELL_MAGE_BLAZING_BARRIER_DAMAGE = 81527,
    SPELL_MAGE_BLINK = 1953,
    SPELL_MAGE_BRAIN_FREEZE = 44549,
    SPELL_MAGE_BRAIN_FREEZE_BUFF = 57761,
    SPELL_MAGE_BURNOUT_TRIGGER = 44450,
    SPELL_MAGE_CAUTERIZE = 81528,
    SPELL_MAGE_CAUTERIZE_AURA = 81529,
    SPELL_MAGE_CAUTERIZE_DEBUFF = 81530,
    SPELL_MAGE_COMBUSTION = 11129,
    SPELL_MAGE_COMET_STORM = 81536,
    SPELL_MAGE_COMET_STORM_DAMAGE = 81537,
    SPELL_MAGE_CONE_OF_COLD = 42931,
    SPELL_MAGE_DISPLACEMENT_AURA = 81510,
    SPELL_MAGE_FINGERS_OF_FROST_R1 = 44543,
    SPELL_MAGE_FINGERS_OF_FROST_R2 = 44545,
    SPELL_MAGE_FINGERS_OF_FROST_AURA = 44544,
    SPELL_MAGE_FINGERS_OF_FROST_VISUAL = 74396,
    SPELL_MAGE_FLURRY = 81533,
    SPELL_MAGE_FLURRY_DAMAGE = 81534,
    SPELL_MAGE_FROSTBOLT = 81504,
    SPELL_MAGE_FROST_NOVA = 42917,
    SPELL_MAGE_FROZEN_ORB = 80013,
    SPELL_MAGE_FROZEN_ORB_DAMAGE = 80012,
    SPELL_MAGE_INVISIBILITY = 66,
    SPELL_MAGE_GREATER_INVISIBILITY = 81511,
    SPELL_MAGE_GREATER_INVISIBILITY_AURA = 81513,
    SPELL_MAGE_ICE_BARRIER = 43039,
    SPELL_MAGE_ICE_BARRIER_SLOW = 43040,
    SPELL_MAGE_ICE_BLOCK = 45438,
    SPELL_MAGE_ICE_LANCE = 42914,
    SPELL_MAGE_IMPROVED_BLIZZARD_CHILLED = 12486,
    SPELL_MAGE_MAGIC_BLOSSOM = 81561,
    SPELL_MAGE_MAGIC_BLOSSOM_BUFF = 81562,
    SPELL_MAGE_MIRROR_IMAGE_DAMAGE_REDUCTION = 55343,
    SPELL_MAGE_MIRROR_IMAGE_SUMMON_ID = 31216,
    SPELL_MAGE_PRISMATIC_BARRIER = 81523,
    SPELL_MAGE_GALVANIZING_BARRIER = 81560,
    SPELL_MAGE_RAY_OF_FROST_FINGERS = 81539,
    SPELL_MAGE_RAY_OF_FROST_BUFF = 81540,
    SPELL_MAGE_SUPERNOVA_DAMAGE = 81525,
    SPELL_MAGE_SUPERNOVA_KNOCKUP = 81541,
    SPELL_MAGE_WINTERS_CHILL = 81535,
    SPELL_MAGE_FLURRY_OF_SLASHES = 81547,
    SPELL_MAGE_DEATH_BLOSSOM = 81556,
    SPELL_MAGE_UNSTABLE_ANOMALY = 81564,
    SPELL_MAGE_UNSTABLE_ANOMALY_SHIELD = 81565,
    SPELL_MAGE_UNSTABLE_ANOMALY_KNOCKBACK = 81566,
    SPELL_MAGE_UNSTABLE_ANOMALY_COOLDOWN = 81567,
    SPELL_MAGE_WAVE_OF_FORCE = 81568,

    // Masteries
    MASTERY_MAGE_SAVANT = 300111,
    MASTERY_MAGE_IGNITE = 300109,
    MASTERY_MAGE_ICICLE = 300105,
    MASTERY_MAGE_ICICLE_BUFF = 300106,
    MASTERY_MAGE_BATTLE_KNOWLEDGE = 300114,

    // Theirs
    SPELL_MAGE_COLD_SNAP = 11958,
    SPELL_MAGE_FOCUS_MAGIC_PROC = 54648,
    SPELL_MAGE_FROST_WARDING_R1 = 11189,
    SPELL_MAGE_FROST_WARDING_TRIGGERED = 57776,
    SPELL_MAGE_INCANTERS_ABSORBTION_R1 = 44394,
    SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED = 44413,
    SPELL_MAGE_IGNITE = 12654,
    SPELL_MAGE_MASTER_OF_ELEMENTS_ENERGIZE = 29077,
    SPELL_MAGE_SQUIRREL_FORM = 32813,
    SPELL_MAGE_GIRAFFE_FORM = 32816,
    SPELL_MAGE_SERPENT_FORM = 32817,
    SPELL_MAGE_DRAGONHAWK_FORM = 32818,
    SPELL_MAGE_WORGEN_FORM = 32819,
    SPELL_MAGE_SHEEP_FORM = 32820,
    SPELL_MAGE_GLYPH_OF_ETERNAL_WATER = 70937,
    SPELL_MAGE_SUMMON_WATER_ELEMENTAL_PERMANENT = 70908,
    SPELL_MAGE_SUMMON_WATER_ELEMENTAL_TEMPORARY = 70907,
    SPELL_MAGE_GLYPH_OF_BLAST_WAVE = 62126,
    //SPELL_MAGE_FINGERS_OF_FROST = 44543

    // Talents
    SPELL_MAGE_TALENT_IMPROVED_FINGERS_OF_FROST_R1 = 80027,
    SPELL_MAGE_TALENT_IMPROVED_FINGERS_OF_FROST_R2 = 80028,
    SPELL_MAGE_TALENT_RULE_OF_THREES_R1 = 11237,
    SPELL_MAGE_TALENT_RULE_OF_THREES_R2 = 12463,
    SPELL_MAGE_TALENT_RULE_OF_THREES_R3 = 12464,
    SPELL_MAGE_TALENT_RULE_OF_THREES_R4 = 16769,
    SPELL_MAGE_TALENT_RULE_OF_THREES_R5 = 16770,
    SPELL_MAGE_TALENT_RULE_OF_THREES_BUFF = 16771,
    SPELL_MAGE_TALENT_PARRY_WARD_PROC = 81647,
    SPELL_MAGE_TALENT_ARCANIC_BARRIER_PROC = 81659,
    SPELL_MAGE_TALENT_INFUSED_BLADES_PROC = 81668,
    SPELL_MAGE_TALENT_IMPROVED_DEATH_BLOSSOM = 81622,

    // Weapon Enchants
    SPELL_MAGE_ENCHANT_ARCANIZE_PROC = 81571,
    SPELL_MAGE_ENCHANT_CONDUIT_PROC = 81574,
    SPELL_MAGE_ENCHANT_DEFLECTION_PROC = 81577,
    SPELL_MAGE_ENCHANT_IGNIS_PROC = 81582,
    SPELL_MAGE_ENCHANT_IGNIS_PROC_STACKED = 81670,
    SPELL_MAGE_ENCHANT_SNOWBOUND_PROC = 81585,
    SPELL_MAGE_ENCHANT_ARCANIZE = 81570,
    SPELL_MAGE_ENCHANT_FORCE = 81579,
    SPELL_MAGE_ENCHANT_CONDUIT = 81573,
    SPELL_MAGE_ENCHANT_IGNIS = 81581,
    SPELL_MAGE_ENCHANT_DEFLECTION = 81576,
    SPELL_MAGE_ENCHANT_SNOWBOUND = 81584,

    // Visuals
    SPELL_VISUAL_FROZEN_ORB = 72067,
    SPELL_VISUAL_ARCANE_ORB = 80015,

    // Passives
    PASSIVE_MAGE_HOT_STREAK = 44448,
    PASSIVE_MAGE_HOT_STREAK_BUFF = 48108,

    // Runes
    RUNE_MAGE_SEARING_TOUCH_BUFF = 300747,
    RUNE_MAGE_SIPHON_STORM_BUFF = 301034,
    RUNE_MAGE_MANA_ADEPT_ENERGIZE = 301158,
    RUNE_MAGE_FRACTURED_FROST_BUFF = 301358,

    // Item Sets
    T1_MAGE_ARCANE_2PC = 95500,
    T1_MAGE_ARCANE_2PC_BUFF = 95501,
    T1_MAGE_ARCANE_4PC = 95502,
    T1_MAGE_ARCANE_4PC_LISTENER = 95503,
    T1_MAGE_ARCANE_4PC_MAIN_BUFF = 95504,
    T1_MAGE_ARCANE_4PC_SECONDARY_BUFF = 95505,
    T1_MAGE_FROST_4PC = 95702,
    T1_MAGE_MAGEBLADE_4PC_BUFF = 95803,
};

class npc_mage_spell_arcane_orbs : public CreatureScript
{
public:
    npc_mage_spell_arcane_orbs() : CreatureScript("npc_mage_spell_arcane_orbs") { }

    struct npc_mage_spell_arcane_orbsAI : public ScriptedAI
    {
        npc_mage_spell_arcane_orbsAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 update = 785;

        void Reset() override
        {
            Position pos = me->GetFirstCollisionPosition(40.0f, 0);
            me->GetMotionMaster()->MovePoint(0, pos);
            me->CastSpell(me, SPELL_VISUAL_ARCANE_ORB, true); // VISUAL
            me->CombatStop(true);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(uint32 diff) override
        {
            if (update >= 785) {
                if (Unit* owner = me->ToTempSummon()->GetSummonerUnit()) {
                    me->CastSpell(me, SPELL_MAGE_ARCANE_ORB_DAMAGE, true, nullptr, nullptr, owner->GetGUID());
                }
                update = 0;
            }

            update += diff;
        }

        void MovementInform(uint32 /*type*/, uint32 id) override {
            if (id == 0) {
                me->DespawnOrUnsummon();
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_mage_spell_arcane_orbsAI(creature);
    }
};

class npc_mage_spell_frozen_orbs : public CreatureScript
{
public:
    npc_mage_spell_frozen_orbs() : CreatureScript("npc_mage_spell_frozen_orbs") { }

    struct npc_mage_spell_frozen_orbsAI : public ScriptedAI
    {
        npc_mage_spell_frozen_orbsAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 update = 785;

        void Reset() override
        {
            Position pos = me->GetFirstCollisionPosition(40.0f, 0);
            me->GetMotionMaster()->MovePoint(0, pos);
            me->CastSpell(me, SPELL_VISUAL_FROZEN_ORB, true); // VISUAL
            me->CombatStop(true);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(uint32 diff) override
        {
            if (update >= 785) {
                if (Unit* owner = me->ToTempSummon()->GetSummonerUnit()) {
                    me->CastSpell(me, SPELL_MAGE_FROZEN_ORB_DAMAGE, true, nullptr, nullptr, owner->GetGUID());
                }
                update = 0;
            }

            update += diff;
        }

        void MovementInform(uint32 /*type*/, uint32 id) override {
            if (id == 0) {
                me->DespawnOrUnsummon();
            }
        }

        void SpellHitTarget(Unit* /*target*/, SpellInfo const* /*spell*/)
        {

        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_mage_spell_frozen_orbsAI(creature);
    }
};

class spell_mage_pheonix_flame : public SpellScript
{
    PrepareSpellScript(spell_mage_pheonix_flame);

    void HandleAfterHit()
    {
        GetCaster()->CastSpell(GetExplTargetUnit(), 80030, true);
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_mage_pheonix_flame::HandleAfterHit);
    }
};

// 11129 - Combustion
class spell_mage_combustion : public AuraScript
{
    PrepareAuraScript(spell_mage_combustion);

    Aura* GetWildfireAura(Unit* caster)
    {
        for (size_t i = 301222; i < 301228; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetWildfireAura(caster))
        {
            int32 selfBuff = GetWildfireAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            int32 allyBuff = GetWildfireAura(caster)->GetEffect(EFFECT_1)->GetAmount();

            caster->AddAura(selfBuff, caster);
            caster->CastSpell(caster, allyBuff, TRIGGERED_FULL_MASK);

            caster->RemoveAura(allyBuff);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->RemoveAura(11129);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_combustion::HandleApply, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_mage_combustion::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_frozen_orb_damage : public SpellScript
{
    PrepareSpellScript(spell_mage_frozen_orb_damage);

    void HandleAfterHit()
    {
        Unit* frozenOrb = GetCaster();

        if (Unit* player = GetCaster()->ToTempSummon()->GetSummonerUnit()) {

            frozenOrb->SetSpeed(MOVE_WALK, 0.02);
            frozenOrb->SetSpeed(MOVE_RUN, 0.02);

            if (player->HasAura(SPELL_MAGE_TALENT_IMPROVED_FINGERS_OF_FROST_R1) && roll_chance_i(5))
                player->CastSpell(player, SPELL_MAGE_FINGERS_OF_FROST_AURA);
            if (player->HasAura(SPELL_MAGE_TALENT_IMPROVED_FINGERS_OF_FROST_R2) && roll_chance_i(10))
                player->CastSpell(player, SPELL_MAGE_FINGERS_OF_FROST_AURA);
        }
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_mage_frozen_orb_damage::HandleAfterHit);
    }
};

class spell_mage_frozen_orbs : public SpellScript
{
    PrepareSpellScript(spell_mage_frozen_orbs);

    Aura* GetFreezingRainAura(Unit* caster)
    {
        for (size_t i = 301460; i < 301466; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleSummon()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetFreezingRainAura(caster))
        {
            int32 procSpell = GetFreezingRainAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            caster->AddAura(procSpell, caster);
        }

        Position pos = *caster;
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = caster->SummonCreature(500500, pos, TEMPSUMMON_TIMED_DESPAWN, GetSpellInfo()->GetDuration(), 0, properties);

        if (!summon)
            return;

        summon->SetOwnerGUID(caster->GetGUID());
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        summon->SetReactState(REACT_PASSIVE);
        summon->SetTarget();
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mage_frozen_orbs::HandleSummon);
    }
};

// 80013 - Frozen Orb
class spell_mage_frozen_orb_aura : public AuraScript
{
    PrepareAuraScript(spell_mage_frozen_orb_aura);

    Aura* GetFreezingWindsAura(Unit* caster)
    {
        for (size_t i = 301472; i < 301478; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetFreezingWindsAura(caster))
            caster->CastSpell(caster, SPELL_MAGE_FINGERS_OF_FROST_AURA, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_frozen_orb_aura::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_mage_arcane_orb : public SpellScript
{
    PrepareSpellScript(spell_mage_arcane_orb);

    void HandleSummon()
    {
        Position pos = *GetCaster();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = GetCaster()->SummonCreature(500501, pos, TEMPSUMMON_TIMED_DESPAWN, GetSpellInfo()->CalcDuration(GetCaster(), GetSpell()), 0, properties);

        if (!summon)
            return;

        summon->SetOwnerGUID(GetCaster()->GetGUID());
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        summon->SetReactState(REACT_PASSIVE);
        summon->SetTarget();
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mage_arcane_orb::HandleSummon);
    }
};


// 42897 - Arcane Blast
class spell_mage_arcane_blast : public SpellScript
{
    PrepareSpellScript(spell_mage_arcane_blast);

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();

        if (!caster || !caster->HasAura(SPELL_MAGE_ARCANE_CHARGE))
            return;

        GetCaster()->CastSpell(caster, SPELL_MAGE_ARCANE_CHARGE_VISUAL, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_mage_arcane_blast::HandleAfterCast);
    }
};

// 44781 - Arcane Barrage
class spell_mage_arcane_barrage : public SpellScript
{
    PrepareSpellScript(spell_mage_arcane_barrage);

    Aura* GetArcaneTempoAura(Unit* caster)
    {
        for (size_t i = 301134; i < 301140; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetOrbBarrageAura(Unit* caster)
    {
        for (size_t i = 301146; i < 301152; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetManaAdeptAura(Unit* caster)
    {
        for (size_t i = 301152; i < 301158; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;

        if (Aura* arcaneCharges = caster->GetAura(SPELL_MAGE_ARCANE_CHARGE_VISUAL))
        {
            if (GetArcaneTempoAura(caster))
            {
                int32 procSpell = GetArcaneTempoAura(caster)->GetEffect(EFFECT_0)->GetAmount();
                caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
            }

            if (GetOrbBarrageAura(caster))
            {
                int32 procChance = GetOrbBarrageAura(caster)->GetEffect(EFFECT_0)->GetAmount();
                procChance *= arcaneCharges->GetStackAmount();

                if (roll_chance_i(procChance))
                    caster->CastSpell(caster, SPELL_MAGE_ARCANE_ORB, TRIGGERED_FULL_MASK);
            }

            if (GetManaAdeptAura(caster))
            {
                int32 maxMana = caster->GetMaxPower(POWER_MANA);
                int32 manaPct = GetManaAdeptAura(caster)->GetEffect(EFFECT_0)->GetAmount();
                manaPct *= arcaneCharges->GetStackAmount();
                int32 amount = CalculatePct(maxMana, manaPct);
                amount /= 10;

                caster->CastCustomSpell(RUNE_MAGE_MANA_ADEPT_ENERGIZE, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
            }

            arcaneCharges->Remove();
        }

        // remove Cord of Infinity's damage buff
        for (size_t i = 300610; i < 300616; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }

        // remove Explosive Barrage's damage buff
        for (size_t i = 300622; i < 300628; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }

        // remove Arcane Harmony's damage buff
        for (size_t i = 300099; i < 300105; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_mage_arcane_barrage::HandleAfterHit);
    }
};

class spell_mage_burning_determination : public AuraScript
{
    PrepareAuraScript(spell_mage_burning_determination);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetSpellInfo() || !eventInfo.GetActionTarget())
            return false;

        // Need Interrupt or Silenced mechanic
        if (!(eventInfo.GetSpellInfo()->GetAllEffectsMechanicMask() & ((1 << MECHANIC_INTERRUPT) | (1 << MECHANIC_SILENCE))))
            return false;

        // Xinef: immuned effect should just eat charge
        if (eventInfo.GetHitMask() & PROC_EX_IMMUNE)
        {
            eventInfo.GetActionTarget()->RemoveAurasDueToSpell(54748);
            return false;
        }
        if (Aura* aura = eventInfo.GetActionTarget()->GetAura(54748))
        {
            if (aura->GetDuration() < aura->GetMaxDuration())
                eventInfo.GetActionTarget()->RemoveAurasDueToSpell(54748);
            return false;
        }

        return true;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo&  /*eventInfo*/)
    {
        PreventDefaultAction();
        GetUnitOwner()->CastSpell(GetUnitOwner(), 54748, true);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_burning_determination::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mage_burning_determination::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_mage_molten_armor : public AuraScript
{
    PrepareAuraScript(spell_mage_molten_armor);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo || (eventInfo.GetTypeMask() & PROC_FLAG_TAKEN_MELEE_AUTO_ATTACK))
            return true;

        if (!eventInfo.GetActionTarget())
        {
            return false;
        }

        // Xinef: Molten Shields talent
        if (AuraEffect* aurEff = eventInfo.GetActionTarget()->GetAuraEffect(SPELL_AURA_ADD_FLAT_MODIFIER, SPELLFAMILY_MAGE, 16, EFFECT_0))
            return roll_chance_i(aurEff->GetSpellInfo()->GetRank() * 50);

        return false;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_molten_armor::CheckProc);
    }
};

// 55342 - Mirror Image
class spell_mage_mirror_image : public AuraScript
{
    PrepareAuraScript(spell_mage_mirror_image)

        void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->CastSpell((Unit*)nullptr, GetSpellInfo()->Effects[aurEff->GetEffIndex()].TriggerSpell, true);
        GetCaster()->AddAura(SPELL_MAGE_MIRROR_IMAGE_DAMAGE_REDUCTION, GetCaster());
    }

    void CalcPeriodic(AuraEffect const* /*effect*/, bool& isPeriodic, int32&  /*amplitude*/)
    {
        isPeriodic = false;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes  /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(SPELL_MAGE_MIRROR_IMAGE_DAMAGE_REDUCTION))
            caster->RemoveAura(SPELL_MAGE_MIRROR_IMAGE_DAMAGE_REDUCTION);

        auto summonedUnits = caster->m_Controlled;
        for (auto const& unit : summonedUnits)
        {
            if (unit->GetEntry() == SPELL_MAGE_MIRROR_IMAGE_SUMMON_ID)
                if (Creature* creature = unit->ToCreature()) {
                    creature->DespawnOrUnsummon();
                }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_mirror_image::HandleEffectApply, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_mage_mirror_image::CalcPeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_mirror_image::HandleRemove, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_burnout : public AuraScript
{
    PrepareAuraScript(spell_mage_burnout);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_BURNOUT_TRIGGER });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() != nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        int32 mana = int32(eventInfo.GetSpellInfo()->CalcPowerCost(GetTarget(), eventInfo.GetSchoolMask()));
        mana = CalculatePct(mana, aurEff->GetAmount());

        GetTarget()->CastCustomSpell(SPELL_MAGE_BURNOUT_TRIGGER, SPELLVALUE_BASE_POINT0, mana, GetTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_burnout::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mage_burnout::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
    }
};

class spell_mage_burnout_trigger : public SpellScript
{
    PrepareSpellScript(spell_mage_burnout_trigger);

    void HandleDummy(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        if (Unit* target = GetHitUnit())
        {
            int32 newDamage = -(target->ModifyPower(POWER_MANA, -GetEffectValue()));
            GetSpell()->ExecuteLogEffectTakeTargetPower(effIndex, target, POWER_MANA, newDamage, 0.0f);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_mage_burnout_trigger::HandleDummy, EFFECT_0, SPELL_EFFECT_POWER_BURN);
    }
};

class spell_mage_pet_scaling : public AuraScript
{
    PrepareAuraScript(spell_mage_pet_scaling);

    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: mage pet inherits 40% of resistance from owner and 35% of armor (guessed)
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            SpellSchoolMask schoolMask = SpellSchoolMask(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            int32 modifier = schoolMask == SPELL_SCHOOL_MASK_NORMAL ? 35 : 40;
            amount = CalculatePct(std::max<int32>(0, owner->GetResistance(schoolMask)), modifier);
        }
    }

    void CalculateStatAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: mage pet inherits 30% of intellect / stamina
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            Stats stat = Stats(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            amount = CalculatePct(std::max<int32>(0, owner->GetStat(stat)), 30);
        }
    }

    void CalculateAPAmount(AuraEffect const*  /*aurEff*/, int32&   /*amount*/, bool& /*canBeRecalculated*/)
    {
        // xinef: mage pet inherits 0% AP
    }

    void CalculateSPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: mage pet inherits 33% of SP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 frost = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST);
            amount = CalculatePct(std::max<int32>(0, frost), 33);

            // xinef: Update appropriate player field
            if (owner->GetTypeId() == TYPEID_PLAYER)
                owner->SetUInt32Value(PLAYER_PET_SPELL_POWER, (uint32)amount);
        }
    }

    void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (GetUnitOwner()->IsPet())
            return;

        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, aurEff->GetAuraType(), true, SPELL_BLOCK_TYPE_POSITIVE);
        if (aurEff->GetAuraType() == SPELL_AURA_MOD_ATTACK_POWER)
            GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ATTACK_POWER_PCT, true, SPELL_BLOCK_TYPE_POSITIVE);
        else if (aurEff->GetAuraType() == SPELL_AURA_MOD_STAT)
            GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, true, SPELL_BLOCK_TYPE_POSITIVE);
    }

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        if (!GetUnitOwner()->IsPet())
            return;

        isPeriodic = true;
        amplitude = 2 * IN_MILLISECONDS;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        PreventDefaultAction();
        if (aurEff->GetAuraType() == SPELL_AURA_MOD_STAT && (aurEff->GetMiscValue() == STAT_STAMINA || aurEff->GetMiscValue() == STAT_INTELLECT))
        {
            int32 currentAmount = aurEff->GetAmount();
            int32 newAmount = GetEffect(aurEff->GetEffIndex())->CalculateAmount(GetCaster());
            if (newAmount != currentAmount)
            {
                if (aurEff->GetMiscValue() == STAT_STAMINA)
                {
                    uint32 actStat = GetUnitOwner()->GetHealth();
                    GetEffect(aurEff->GetEffIndex())->ChangeAmount(newAmount, false);
                    GetUnitOwner()->SetHealth(std::min<uint32>(GetUnitOwner()->GetMaxHealth(), actStat));
                }
                else
                {
                    uint32 actStat = GetUnitOwner()->GetPower(POWER_MANA);
                    GetEffect(aurEff->GetEffIndex())->ChangeAmount(newAmount, false);
                    GetUnitOwner()->SetPower(POWER_MANA, std::min<uint32>(GetUnitOwner()->GetMaxPower(POWER_MANA), actStat));
                }
            }
        }
        else
            GetEffect(aurEff->GetEffIndex())->RecalculateAmount();
    }

    void Register() override
    {
        if (m_scriptSpellId != 35657)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_pet_scaling::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);

        if (m_scriptSpellId == 35657 || m_scriptSpellId == 35658)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_pet_scaling::CalculateStatAmount, EFFECT_ALL, SPELL_AURA_MOD_STAT);

        if (m_scriptSpellId == 35657)
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_pet_scaling::CalculateAPAmount, EFFECT_ALL, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_pet_scaling::CalculateSPAmount, EFFECT_ALL, SPELL_AURA_MOD_DAMAGE_DONE);
        }

        OnEffectApply += AuraEffectApplyFn(spell_mage_pet_scaling::HandleEffectApply, EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_mage_pet_scaling::CalcPeriodic, EFFECT_ALL, SPELL_AURA_ANY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_pet_scaling::HandlePeriodic, EFFECT_ALL, SPELL_AURA_ANY);
    }
};

class spell_mage_brain_freeze : public AuraScript
{
    PrepareAuraScript(spell_mage_brain_freeze);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = GetCaster()->ToPlayer())
            player->RemoveSpellCooldown(SPELL_MAGE_FLURRY, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_brain_freeze::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_mage_glyph_of_eternal_water : public AuraScript
{
    PrepareAuraScript(spell_mage_glyph_of_eternal_water);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* target = GetTarget())
            if (Player* player = target->ToPlayer())
                if (Pet* pet = player->GetPet())
                    if (pet->GetEntry() == NPC_WATER_ELEMENTAL_PERM)
                        pet->Remove(PET_SAVE_NOT_IN_SLOT);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_glyph_of_eternal_water::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_combustion_proc : public AuraScript
{
    PrepareAuraScript(spell_mage_combustion_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_COMBUSTION });
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_MAGE_COMBUSTION);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_mage_combustion_proc::OnRemove, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

// Incanter's Absorbtion
class spell_mage_incanters_absorbtion_base_AuraScript : public AuraScript
{
public:
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED, SPELL_MAGE_INCANTERS_ABSORBTION_R1 });
    }

    void Trigger(AuraEffect* aurEff, DamageInfo& /*dmgInfo*/, uint32& absorbAmount)
    {
        Unit* target = GetTarget();

        if (!target || target->isDead())
            return;

        if (AuraEffect* talentAurEff = target->GetAuraEffectOfRankedSpell(SPELL_MAGE_INCANTERS_ABSORBTION_R1, EFFECT_0))
        {
            int32 bp = CalculatePct(absorbAmount, talentAurEff->GetAmount());
            if (AuraEffect* currentAura = target->GetAuraEffect(SPELL_AURA_MOD_DAMAGE_DONE, SPELLFAMILY_MAGE, 2941, EFFECT_0))
            {
                bp += int32(currentAura->GetAmount() * (currentAura->GetBase()->GetDuration() / (float)currentAura->GetBase()->GetMaxDuration()));
                currentAura->ChangeAmount(bp);
                currentAura->GetBase()->RefreshDuration();
            }
            else
                target->CastCustomSpell(target, SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED, &bp, nullptr, nullptr, true, nullptr, aurEff);
        }
    }
};

// -11113 - Blast Wave
class spell_mage_blast_wave : public SpellScript
{
    PrepareSpellScript(spell_mage_blast_wave);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_GLYPH_OF_BLAST_WAVE });
    }

    void HandleKnockBack(SpellEffIndex effIndex)
    {
        if (GetCaster()->HasAura(SPELL_MAGE_GLYPH_OF_BLAST_WAVE))
            PreventHitDefaultEffect(effIndex);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_mage_blast_wave::HandleKnockBack, EFFECT_2, SPELL_EFFECT_KNOCK_BACK);
    }
};

// 11958 - Cold Snap
class spell_mage_cold_snap : public SpellScript
{
    PrepareSpellScript(spell_mage_cold_snap);

    bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Player* caster = GetCaster()->ToPlayer();
        // immediately finishes the cooldown on Frost spells

        /*PlayerSpellMap const& spellMap = caster->GetSpellMap();
        for (PlayerSpellMap::const_iterator itr = spellMap.begin(); itr != spellMap.end(); ++itr)
        {
            SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(itr->first);
            if (spellInfo->SpellFamilyName == SPELLFAMILY_MAGE && (spellInfo->GetSchoolMask() & SPELL_SCHOOL_MASK_FROST) && spellInfo->Id != SPELL_MAGE_COLD_SNAP && spellInfo->GetRecoveryTime() > 0)
            {
                SpellCooldowns::iterator citr = caster->GetSpellCooldownMap().find(spellInfo->Id);
                if (citr != caster->GetSpellCooldownMap().end() && citr->second.needSendToClient)
                    caster->RemoveSpellCooldown(spellInfo->Id, true);
                else
                    caster->RemoveSpellCooldown(spellInfo->Id, false);
            }
        }*/

        caster->RemoveSpellCooldown(SPELL_MAGE_CONE_OF_COLD, true);
        caster->RemoveSpellCooldown(SPELL_MAGE_FROST_NOVA, true);
        caster->RemoveSpellCooldown(SPELL_MAGE_ICE_BARRIER, true);
        caster->RemoveSpellCooldown(SPELL_MAGE_ICE_BLOCK, true);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_mage_cold_snap::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// -543  - Fire Ward
// -6143 - Frost Ward
class spell_mage_fire_frost_ward : public spell_mage_incanters_absorbtion_base_AuraScript
{
    PrepareAuraScript(spell_mage_fire_frost_ward);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_FROST_WARDING_TRIGGERED, SPELL_MAGE_FROST_WARDING_R1 });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = false;
        if (Unit* caster = GetCaster())
        {
            // +80.68% from sp bonus
            float bonus = amount;

            bonus *= caster->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask());
            bonus /= 100;
            //bonus *= caster->CalculateLevelPenalty(GetSpellInfo());

            amount = int32(bonus);
        }
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* target = GetTarget();
        if (AuraEffect* talentAurEff = target->GetAuraEffectOfRankedSpell(SPELL_MAGE_FROST_WARDING_R1, EFFECT_0))
        {
            int32 chance = talentAurEff->GetSpellInfo()->Effects[EFFECT_1].CalcValue(); // SPELL_EFFECT_DUMMY with NO_TARGET

            if (roll_chance_i(chance))
            {
                int32 bp = dmgInfo.GetDamage();
                target->CastCustomSpell(target, SPELL_MAGE_FROST_WARDING_TRIGGERED, &bp, nullptr, nullptr, true, nullptr, aurEff);
                absorbAmount = 0;

                // Xinef: trigger Incanters Absorbtion
                uint32 damage = dmgInfo.GetDamage();
                Trigger(aurEff, dmgInfo, damage);

                // Xinef: hack for chaos bolt
                /*if (!dmgInfo.GetSpellInfo() || dmgInfo.GetSpellInfo()->SpellIconID != 3178)
                    dmgInfo.AbsorbDamage(bp);*/

                PreventDefaultAction();
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_fire_frost_ward::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_fire_frost_ward::Absorb, EFFECT_0);
        AfterEffectAbsorb += AuraEffectAbsorbFn(spell_mage_fire_frost_ward::Trigger, EFFECT_0);
    }
};

// 54646 - Focus Magic
class spell_mage_focus_magic : public AuraScript
{
    PrepareAuraScript(spell_mage_focus_magic);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_FOCUS_MAGIC_PROC });
    }

    bool Load() override
    {
        _procTarget = nullptr;
        return true;
    }

    bool CheckProc(ProcEventInfo& /*eventInfo*/)
    {
        _procTarget = GetCaster();
        return _procTarget && _procTarget->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(_procTarget, SPELL_MAGE_FOCUS_MAGIC_PROC, true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_focus_magic::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mage_focus_magic::HandleProc, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
    }

private:
    Unit* _procTarget;
};

// 43039 - Ice Barrier / 81523 - Prismatic Barrier / 81526 - Blazing Barrier
class spell_mage_barrier_absorb_aura : public spell_mage_incanters_absorbtion_base_AuraScript
{
    PrepareAuraScript(spell_mage_barrier_absorb_aura);

    // TODO: Rework
    static int32 CalculateSpellAmount(Unit* caster, int32 amount, SpellInfo const* spellInfo, const AuraEffect* aurEff)
    {
        // +80.68% from sp bonus
        float bonus = amount;

        bonus = CalculatePct(caster->GetMaxHealth(), amount);

        // Glyph of Ice Barrier: its weird having a SPELLMOD_ALL_EFFECTS here but its blizzards doing :)
        // Glyph of Ice Barrier is only applied at the spell damage bonus because it was already applied to the base value in CalculateSpellDamage
        bonus = caster->ApplyEffectModifiers(spellInfo, aurEff->GetEffIndex(), bonus);

        //bonus *= caster->CalculateLevelPenalty(spellInfo);

        amount = int32(bonus);
        return amount;
    }

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = false;

        if (Unit* caster = GetCaster())
            amount = CalculateSpellAmount(caster, amount, GetSpellInfo(), aurEff);
    }

    void Trigger(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* caster = GetCaster();
        Unit* target = dmgInfo.GetAttacker();

        if (!caster || caster->isDead())
            return;

        if (!target || target->isDead())
            return;

        if (GetAura()->GetId() == SPELL_MAGE_ICE_BARRIER)
            caster->CastSpell(target, SPELL_MAGE_ICE_BARRIER_SLOW, TRIGGERED_FULL_MASK);

        if (GetAura()->GetId() == SPELL_MAGE_BLAZING_BARRIER)
            caster->CastSpell(target, SPELL_MAGE_BLAZING_BARRIER_DAMAGE, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_barrier_absorb_aura::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        AfterEffectAbsorb += AuraEffectAbsorbFn(spell_mage_barrier_absorb_aura::Trigger, EFFECT_0);
    }
};

// 43039 - Ice Barrier / 81523 - Prismatic Barrier / 81526 - Blazing Barrier
class spell_mage_barrier : public SpellScript
{
    PrepareSpellScript(spell_mage_barrier);

    // TODO: Rework
    static int32 CalculateSpellAmount(Unit* caster, int32 amount, SpellInfo const* spellInfo, const AuraEffect* aurEff)
    {
        // +80.68% from sp bonus
        float bonus = amount;

        bonus = CalculatePct(caster->GetMaxHealth(), amount);

        // Glyph of Ice Barrier: its weird having a SPELLMOD_ALL_EFFECTS here but its blizzards doing :)
        // Glyph of Ice Barrier is only applied at the spell damage bonus because it was already applied to the base value in CalculateSpellDamage
        bonus = caster->ApplyEffectModifiers(spellInfo, aurEff->GetEffIndex(), bonus);

        //bonus *= caster->CalculateLevelPenalty(spellInfo);

        amount = int32(bonus);
        return amount;
    }

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();

        if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_AURA_SCHOOL_ABSORB, (SpellFamilyNames)GetSpellInfo()->SpellFamilyName, GetSpellInfo()->SpellIconID, EFFECT_0))
        {
            int32 newAmount = GetSpellInfo()->Effects[EFFECT_0].CalcValue(caster, nullptr, nullptr);
            newAmount = CalculateSpellAmount(caster, newAmount, GetSpellInfo(), aurEff);

            if (aurEff->GetAmount() > newAmount)
                return SPELL_FAILED_AURA_BOUNCED;
        }

        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_mage_barrier::CheckCast);
    }
};

// -11119 - Ignite
class spell_mage_ignite : public AuraScript
{
    PrepareAuraScript(spell_mage_ignite);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_IGNITE });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetActor() || !eventInfo.GetProcTarget())
            return false;

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetSpellInfo())
        {
            return false;
        }

        // Molten Armor
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            if (spellInfo->SpellFamilyFlags[1] & 0x8)
            {
                return false;
            }
        }

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        const float masteryPct = GetCaster()->ToPlayer()->GetMastery();
        float pct = aurEff->GetAmount() + masteryPct;
        int32 totalTicks = sSpellMgr->AssertSpellInfo(SPELL_MAGE_IGNITE)->GetMaxTicks();
        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), pct) / totalTicks);
        int32 maxAmount = int32(CalculatePct(GetCaster()->GetMaxHealth(), 50));


        if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(SPELL_MAGE_IGNITE, 0))
        {
            int32 remainingTicks = totalTicks - protEff->GetTickNumber();
            int32 remainingAmount = protEff->GetAmount() * remainingTicks;
            int32 remainingAmountPerTick = remainingAmount / totalTicks;

            amount = (std::min<int32>(amount + remainingAmountPerTick, maxAmount));
        }

        // Xinef: implement ignite bug
        eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), SPELL_MAGE_IGNITE, SPELL_AURA_PERIODIC_DAMAGE, amount, TRIGGERED_IGNORE_AURA_SCALING);
        //GetTarget()->CastCustomSpell(SPELL_MAGE_IGNITE, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_ignite::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mage_ignite::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// -44457 - Living Bomb
class spell_mage_living_bomb : public AuraScript
{
    PrepareAuraScript(spell_mage_living_bomb);

    bool Validate(SpellInfo const* spell) override
    {
        if (!sSpellMgr->GetSpellInfo(uint32(spell->Effects[EFFECT_1].CalcValue())))
            return false;
        return true;
    }

    void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
        if (removeMode != AURA_REMOVE_BY_ENEMY_SPELL && removeMode != AURA_REMOVE_BY_EXPIRE)
            return;

        if (Unit* caster = GetCaster())
            caster->CastSpell(GetTarget(), uint32(aurEff->GetAmount()), true, nullptr, aurEff);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_mage_living_bomb::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// -1463 - Mana Shield
class spell_mage_mana_shield : public spell_mage_incanters_absorbtion_base_AuraScript
{
    PrepareAuraScript(spell_mage_mana_shield);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = false;
        if (Unit* caster = GetCaster())
        {
            // +80.68% from sp bonus
            float bonus = amount;

            bonus *= caster->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask());
            bonus /= 100;
            //bonus *= caster->CalculateLevelPenalty(GetSpellInfo());

            amount = int32(bonus);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_mana_shield::CalculateAmount, EFFECT_0, SPELL_AURA_MANA_SHIELD);
        AfterEffectManaShield += AuraEffectManaShieldFn(spell_mage_mana_shield::Trigger, EFFECT_0);
    }
};

// -29074 - Master of Elements
class spell_mage_master_of_elements : public AuraScript
{
    PrepareAuraScript(spell_mage_master_of_elements);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MAGE_MASTER_OF_ELEMENTS_ENERGIZE });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!GetCaster() || !GetCaster()->IsAlive())
            return false;

        _spellInfo = eventInfo.GetSpellInfo();
        if (!_spellInfo || !eventInfo.GetActor() || !eventInfo.GetActionTarget())
        {
            return false;
        }

        bool selectCaster = false;
        // Triggered spells cost no mana so we need triggering spellInfo
        if (SpellInfo const* triggeredByAuraSpellInfo = eventInfo.GetTriggerAuraSpell())
        {
            _spellInfo = triggeredByAuraSpellInfo;
            selectCaster = true;
        }

        // If spell is periodic, mana amount is divided by tick number
        if (eventInfo.GetTriggerAuraEffectIndex() >= EFFECT_0)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = (selectCaster ? eventInfo.GetActor() : eventInfo.GetActionTarget()))
                {
                    if (AuraEffect const* aurEff = target->GetAuraEffect(_spellInfo->Id, eventInfo.GetTriggerAuraEffectIndex(), caster->GetGUID()))
                    {
                        ticksModifier = aurEff->GetTotalTicks();
                    }
                }
            }
        }

        return _spellInfo; // eventInfo.GetSpellInfo()
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        if (_spellInfo && eventInfo.GetSchoolMask()) {
            int32 mana = int32(_spellInfo->CalcPowerCost(GetTarget(), eventInfo.GetSchoolMask()) / ticksModifier);
            mana = CalculatePct(mana, aurEff->GetAmount());

            if (mana > 0)
                GetTarget()->CastCustomSpell(SPELL_MAGE_MASTER_OF_ELEMENTS_ENERGIZE, SPELLVALUE_BASE_POINT0, mana, GetTarget(), true, nullptr, aurEff);
        }

    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_master_of_elements::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mage_master_of_elements::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }

private:
    SpellInfo const* _spellInfo = nullptr;
    uint8 ticksModifier = 1;
};

enum SilvermoonPolymorph
{
    NPC_AUROSALIA = 18744,
};

/// @todo move out of here and rename - not a mage spell
// 32826 - Polymorph (Visual)
class spell_mage_polymorph_cast_visual : public SpellScript
{
    PrepareSpellScript(spell_mage_polymorph_cast_visual);

    static const uint32 PolymorhForms[6];

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        // check if spell ids exist in dbc
        for (uint32 i = 0; i < 6; ++i)
            if (!sSpellMgr->GetSpellInfo(PolymorhForms[i]))
                return false;
        return true;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetCaster()->FindNearestCreature(NPC_AUROSALIA, 30.0f))
            if (target->GetTypeId() == TYPEID_UNIT)
                target->CastSpell(target, PolymorhForms[urand(0, 5)], true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_mage_polymorph_cast_visual::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

const uint32 spell_mage_polymorph_cast_visual::spell_mage_polymorph_cast_visual::PolymorhForms[6] =
{
    SPELL_MAGE_SQUIRREL_FORM,
    SPELL_MAGE_GIRAFFE_FORM,
    SPELL_MAGE_SERPENT_FORM,
    SPELL_MAGE_DRAGONHAWK_FORM,
    SPELL_MAGE_WORGEN_FORM,
    SPELL_MAGE_SHEEP_FORM
};

// 31687 - Summon Water Elemental
class spell_mage_summon_water_elemental : public SpellScript
{
    PrepareSpellScript(spell_mage_summon_water_elemental)
        bool Validate(SpellInfo const* /*spellEntry*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_MAGE_GLYPH_OF_ETERNAL_WATER,
                SPELL_MAGE_SUMMON_WATER_ELEMENTAL_TEMPORARY,
                SPELL_MAGE_SUMMON_WATER_ELEMENTAL_PERMANENT
            });
    }

    bool HasPerkPowerfulElemental(Unit* caster)
    {
        return (caster->HasAura(300415) ||
            caster->HasAura(300416) ||
            caster->HasAura(300417) ||
            caster->HasAura(300418) ||
            caster->HasAura(300419) ||
            caster->HasAura(300420));
    }

    Aura* GetLonelyWinterAura(Unit* caster)
    {
        for (size_t i = 301372; i < 301378; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();

        if (GetLonelyWinterAura(caster))
            return SPELL_FAILED_ALREADY_HAVE_SUMMON;

        return SPELL_CAST_OK;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        if (Creature* pet = ObjectAccessor::GetCreature(*caster, caster->GetPetGUID()))
            if (!pet->IsAlive())
                pet->ToTempSummon()->UnSummon();


        // Glyph of Eternal Water
        if (HasPerkPowerfulElemental(caster))
            caster->CastSpell(caster, 300425, true);
        else
            caster->CastSpell(caster, SPELL_MAGE_SUMMON_WATER_ELEMENTAL_PERMANENT, true);


        if (Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*caster, caster->GetPetGUID()))
        {

            if (HasPerkPowerfulElemental(caster))
            {
                SpellInfo const* spell = sSpellMgr->GetSpellInfo(300421);
                Aura::TryRefreshStackOrCreate(spell, MAX_EFFECT_MASK, pet, pet);
            }

            if (pet->GetCharmInfo() && caster->ToPlayer())
            {
                pet->m_CreatureSpellCooldowns.clear();
                SpellInfo const* spellEntry = sSpellMgr->GetSpellInfo(31707);
                pet->GetCharmInfo()->ToggleCreatureAutocast(spellEntry, true);
                pet->GetCharmInfo()->SetSpellAutocast(spellEntry, true);
                caster->ToPlayer()->CharmSpellInitialize();
            }
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_mage_summon_water_elemental::CheckCast);
        OnEffectHit += SpellEffectFn(spell_mage_summon_water_elemental::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_mage_raging_winds : public AuraScript
{
    PrepareAuraScript(spell_mage_raging_winds);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* caster = GetCaster()->ToPlayer()) {
            caster->ModifySpellCooldown(80013, -aurEff->GetAmount());
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_raging_winds::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

//#define FingersOfFrostScriptName "spell_mage_fingers_of_frost_proc_aura"
//class spell_mage_fingers_of_frost_proc_aura : public AuraScript
//{
//    PrepareAuraScript(spell_mage_fingers_of_frost_proc_aura);
//
//    bool CheckProc(ProcEventInfo& eventInfo)
//    {
//        if (eventInfo.GetSpellPhaseMask() != PROC_SPELL_PHASE_CAST)
//        {
//            eventInfo.SetProcChance(_chance);
//        }
//
//        return true;
//    }
//
//    bool CheckAfterProc(ProcEventInfo& eventInfo)
//    {
//        if (eventInfo.GetSpellPhaseMask() != PROC_SPELL_PHASE_CAST)
//        {
//            eventInfo.ResetProcChance();
//        }
//
//        return true;
//    }
//
//    void HandleOnEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
//    {
//        if (!GetCaster() || !GetCaster()->IsAlive())
//            return;
//
//        if (eventInfo.GetSpellPhaseMask() == PROC_SPELL_PHASE_CAST)
//        {
//            _chance = 100.f;
//            _spell = eventInfo.GetProcSpell();
//
//            if (!_spell || _spell->GetDelayMoment() <= 0)
//            {
//                PreventDefaultAction();
//            }
//        }
//        else
//        {
//            if (eventInfo.GetSpellPhaseMask() == PROC_SPELL_PHASE_FINISH || ((_spell && _spell->GetDelayMoment() > 0) || !eventInfo.GetDamageInfo()))
//            {
//                PreventDefaultAction();
//            }
//
//            _chance = 0.f;
//            _spell = nullptr;
//        }
//    }
//
//    void HandleAfterEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
//    {
//        if (!GetCaster() || !GetCaster()->IsAlive())
//            return;
//
//        if (eventInfo.GetSpellPhaseMask() == PROC_SPELL_PHASE_HIT)
//        {
//            _chance = 100.f;
//        }
//        else if (eventInfo.GetSpellPhaseMask() == PROC_SPELL_PHASE_FINISH)
//        {
//            _chance = 0.f;
//            _spell = nullptr;
//        }
//    }
//
//    void Register()
//    {
//        DoCheckProc += AuraCheckProcFn(spell_mage_fingers_of_frost_proc_aura::CheckProc);
//        DoCheckAfterProc += AuraCheckProcFn(spell_mage_fingers_of_frost_proc_aura::CheckAfterProc);
//        OnEffectProc += AuraEffectProcFn(spell_mage_fingers_of_frost_proc_aura::HandleOnEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
//        AfterEffectProc += AuraEffectProcFn(spell_mage_fingers_of_frost_proc_aura::HandleAfterEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
//    }
//
//public:
//    Spell const* GetProcSpell() const { return _spell; }
//
//private:
//    float _chance = 0.f;
//    Spell const* _spell = nullptr;
//};

//typedef spell_mage_fingers_of_frost_proc_aura spell_mage_fingers_of_frost_proc_aura_script;
//
//class spell_mage_fingers_of_frost_proc : public AuraScript
//{
//    PrepareAuraScript(spell_mage_fingers_of_frost_proc);
//
//    bool CheckProc(ProcEventInfo& eventInfo)
//    {
//        if (Aura* aura = GetCaster()->GetAuraOfRankedSpell(SPELL_MAGE_FINGERS_OF_FROST))
//        {
//            if (spell_mage_fingers_of_frost_proc_aura_script* script = dynamic_cast<spell_mage_fingers_of_frost_proc_aura_script*>(aura->GetScriptByName(FingersOfFrostScriptName)))
//            {
//                if (Spell const* fofProcSpell = script->GetProcSpell())
//                {
//                    if (fofProcSpell == eventInfo.GetProcSpell())
//                    {
//                        return false;
//                    }
//                }
//            }
//        }
//
//        return true;
//    }
//
//    void Register()
//    {
//        DoCheckProc += AuraCheckProcFn(spell_mage_fingers_of_frost_proc::CheckProc);
//    }
//};

class spell_mage_rule_of_threes : public AuraScript
{
    PrepareAuraScript(spell_mage_rule_of_threes);

    Aura* GetTalent()
    {
        if (GetCaster()->HasAura(11237))
            return GetCaster()->GetAura(11237);

        if (GetCaster()->HasAura(12463))
            return GetCaster()->GetAura(12463);

        if (GetCaster()->HasAura(12464))
            return GetCaster()->GetAura(12464);

        if (GetCaster()->HasAura(16769))
            return GetCaster()->GetAura(16769);

        if (GetCaster()->HasAura(16770))
            return GetCaster()->GetAura(16770);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster() || !GetCaster()->IsAlive())
            return;

        if (!GetTalent())
            return;

        if (aurEff->GetBase()->GetStackAmount() != 2)
            return;

        uint32 amount = GetTalent()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->CastCustomSpell(16771, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_rule_of_threes::HandleProc, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

class spell_mage_arcane_meditation : public AuraScript
{
    PrepareAuraScript(spell_mage_arcane_meditation);

    Aura* GetTalent()
    {
        if (GetCaster()->HasAura(18462))
            return GetCaster()->GetAura(18462);

        if (GetCaster()->HasAura(18463))
            return GetCaster()->GetAura(18463);

        if (GetCaster()->HasAura(18464))
            return GetCaster()->GetAura(18464);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetTalent())
            return;

        if (!GetCaster() || !GetCaster()->IsAlive())
            return;

        int32 amount = CalculatePct(int32(GetCaster()->GetMaxPower(POWER_MANA)), aurEff->GetAmount());

        GetCaster()->CastCustomSpell(18465, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_arcane_meditation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mage_improved_fireball : public AuraScript
{
    PrepareAuraScript(spell_mage_improved_fireball);

    void HandleDummy(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        if (!GetCaster() || !GetCaster()->IsAlive())
            return;

        uint32 procSpell = aurEff->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints;

        if (procInfo.GetHitMask() != PROC_EX_CRITICAL_HIT)
            GetCaster()->CastSpell(GetCaster(), procSpell, TRIGGERED_FULL_MASK);
        else
            if (GetCaster()->HasAura(procSpell))
                GetCaster()->RemoveAura(procSpell);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes  /*mode*/)
    {
        if (!GetCaster() || !GetCaster()->IsAlive())
            return;

        uint32 procSpell = aurEff->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints;

        if (GetCaster()->HasAura(procSpell))
            GetCaster()->RemoveAura(procSpell);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_improved_fireball::HandleDummy, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_improved_fireball::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_empowered_fire : public AuraScript
{
    PrepareAuraScript(spell_mage_empowered_fire);

    void HandleDummy(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        if (!GetCaster() || !GetCaster()->IsAlive())
            return;

        int32 amount = CalculatePct(int32(GetCaster()->GetMaxPower(POWER_MANA)), aurEff->GetAmount());
        GetCaster()->CastCustomSpell(67545, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_empowered_fire::HandleDummy, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 81500 - Arcane Charge
class spell_mage_arcane_charge : public AuraScript
{
    PrepareAuraScript(spell_mage_arcane_charge);

    Aura* GetRuleOfThreesTalent()
    {
        if (GetCaster()->HasAura(SPELL_MAGE_TALENT_RULE_OF_THREES_R1))
            return GetCaster()->GetAura(SPELL_MAGE_TALENT_RULE_OF_THREES_R1);

        if (GetCaster()->HasAura(SPELL_MAGE_TALENT_RULE_OF_THREES_R2))
            return GetCaster()->GetAura(SPELL_MAGE_TALENT_RULE_OF_THREES_R2);

        if (GetCaster()->HasAura(SPELL_MAGE_TALENT_RULE_OF_THREES_R3))
            return GetCaster()->GetAura(SPELL_MAGE_TALENT_RULE_OF_THREES_R3);

        if (GetCaster()->HasAura(SPELL_MAGE_TALENT_RULE_OF_THREES_R4))
            return GetCaster()->GetAura(SPELL_MAGE_TALENT_RULE_OF_THREES_R4);

        if (GetCaster()->HasAura(SPELL_MAGE_TALENT_RULE_OF_THREES_R5))
            return GetCaster()->GetAura(SPELL_MAGE_TALENT_RULE_OF_THREES_R5);

        return nullptr;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        uint32 stackAmount = GetAura()->GetStackAmount();

        caster->CastSpell(caster, SPELL_MAGE_ARCANE_CHARGE_BUFF1, TRIGGERED_FULL_MASK);
        caster->CastSpell(caster, SPELL_MAGE_ARCANE_CHARGE_BUFF2, TRIGGERED_FULL_MASK);

        if (Aura* buff1 = caster->GetAura(SPELL_MAGE_ARCANE_CHARGE_BUFF1))
            buff1->SetStackAmount(stackAmount);
        if (Aura* buff2 = caster->GetAura(SPELL_MAGE_ARCANE_CHARGE_BUFF2))
            buff2->SetStackAmount(stackAmount);

        if (GetRuleOfThreesTalent())
        {
            int32 amount = GetRuleOfThreesTalent()->GetEffect(EFFECT_0)->GetAmount();
            if (stackAmount == 3)
                caster->CastCustomSpell(SPELL_MAGE_TALENT_RULE_OF_THREES_BUFF, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes  /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(SPELL_MAGE_ARCANE_CHARGE_BUFF1))
            caster->RemoveAura(SPELL_MAGE_ARCANE_CHARGE_BUFF1);

        if (caster->HasAura(SPELL_MAGE_ARCANE_CHARGE_BUFF2))
            caster->RemoveAura(SPELL_MAGE_ARCANE_CHARGE_BUFF2);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_arcane_charge::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_arcane_charge::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 42921 - Arcane Explosion
class spell_mage_arcane_explosion : public SpellScript
{
    PrepareSpellScript(spell_mage_arcane_explosion);

    Aura* GetReverberateAura(Unit* caster)
    {
        for (size_t i = 301090; i < 301096; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        int32 targetQte = targets.size();

        if (!caster->HasAura(SPELL_MAGE_ARCANE_CHARGE))
            return;

        if (targetQte == 0)
        {
            if (Aura* chargeVisual = caster->GetAura(SPELL_MAGE_ARCANE_CHARGE_VISUAL))
                chargeVisual->RefreshDuration();

            return;
        }

        if (GetReverberateAura(caster))
        {
            int32 targetThreshold = GetReverberateAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            int32 procChance = GetReverberateAura(caster)->GetEffect(EFFECT_1)->GetAmount();

            if (targetQte >= targetThreshold)
            {
                if (roll_chance_i(procChance))
                    caster->CastSpell(caster, SPELL_MAGE_ARCANE_CHARGE_VISUAL, TRIGGERED_FULL_MASK);
            }
        }

        caster->CastSpell(caster, SPELL_MAGE_ARCANE_CHARGE_VISUAL, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_arcane_explosion::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// 42843 - Arcane Missile
class spell_mage_arcane_missiles_aura : public AuraScript
{
    PrepareAuraScript(spell_mage_arcane_missiles_aura);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes  /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        if (Aura* setBonus = caster->GetAura(T1_MAGE_ARCANE_4PC))
            if (Aura* listener = caster->GetAura(T1_MAGE_ARCANE_4PC_LISTENER))
                if (listener->GetStackAmount() >= setBonus->GetEffect(EFFECT_0)->GetAmount())
                {
                    listener->Remove();
                    int32 additionalTargets = setBonus->GetEffect(EFFECT_1)->GetAmount();
                    caster->AddAura(T1_MAGE_ARCANE_4PC_MAIN_BUFF, target);

                    auto const& threatList = caster->getAttackers();

                    for (auto const& targets : threatList)
                        if (targets->IsAlive())
                        {
                            if (targets == target)
                                continue;

                            float distance = targets->GetDistance(target->GetPosition());

                            if (distance > 12)
                                continue;

                            caster->AddAura(T1_MAGE_ARCANE_4PC_SECONDARY_BUFF, targets);
                            caster->CastSpell(targets, SPELL_MAGE_ARCANE_MISSILE_MOVING, TRIGGERED_FULL_MASK);
                            additionalTargets--;

                            if (additionalTargets <= 0)
                                break;
                        }
                }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes  /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(SPELL_MAGE_ARCANE_CHARGE))
            GetCaster()->CastSpell(caster, SPELL_MAGE_ARCANE_CHARGE_VISUAL, TRIGGERED_FULL_MASK);

        // remove Amplification buff
        for (size_t i = 301166; i < 301172; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_arcane_missiles_aura::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_arcane_missiles_aura::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

// 42844 - Arcane Missile Damage
class spell_mage_arcane_missile_damage : public SpellScript
{
    PrepareSpellScript(spell_mage_arcane_missile_damage);

    void HandleDummy(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetHitUnit();

        if (!target || target->isDead())
            return;

        int32 damage = GetHitDamage();

        if (Aura* buffAura = target->GetAura(T1_MAGE_ARCANE_4PC_MAIN_BUFF))
            AddPct(damage, buffAura->GetEffect(EFFECT_0)->GetAmount());

        if (Aura* buffAura = target->GetAura(T1_MAGE_ARCANE_4PC_SECONDARY_BUFF))
            AddPct(damage, buffAura->GetEffect(EFFECT_0)->GetAmount());

        SetHitDamage(damage);
    }

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetHitUnit();

        if (!target || target->isDead())
            return;

        if (!caster->HasAura(SPELL_MAGE_ARCANE_MISSILE) && !caster->HasAura(SPELL_MAGE_ARCANE_MISSILE_MOVING))
        {
            if (target->HasAura(T1_MAGE_ARCANE_4PC_MAIN_BUFF))
                target->RemoveAura(T1_MAGE_ARCANE_4PC_MAIN_BUFF);

            if (target->HasAura(T1_MAGE_ARCANE_4PC_SECONDARY_BUFF))
                target->RemoveAura(T1_MAGE_ARCANE_4PC_SECONDARY_BUFF);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_mage_arcane_missile_damage::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        AfterHit += SpellHitFn(spell_mage_arcane_missile_damage::HandleAfterHit);
    }
};

// 1953 - Blink / 81509 - Displacement
class spell_mage_blink_displacement : public SpellScript
{
    PrepareSpellScript(spell_mage_blink_displacement);

    void HandleBeforeCast()
    {
        Unit* caster = GetCaster();

        caster->SetPositionBeforeBlink(caster->GetPosition());
        GetCaster()->AddAura(SPELL_MAGE_DISPLACEMENT_AURA, GetCaster());
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_mage_blink_displacement::HandleBeforeCast);
    }
};

// 81509 - Displacement
class spell_mage_displacement : public SpellScript
{
    PrepareSpellScript(spell_mage_displacement);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();

        if (!caster->HasAura(SPELL_MAGE_DISPLACEMENT_AURA))
            return SPELL_FAILED_NO_VALID_TARGETS;

        if (caster->GetDistance(caster->GetPositionBeforeBlink()) > GetSpellInfo()->GetMaxRange())
            return SPELL_FAILED_OUT_OF_RANGE;

        return SPELL_CAST_OK;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();

        Position targetPosition = caster->GetPositionBeforeBlink();

        caster->NearTeleportTo(targetPosition);
        caster->RemoveAura(SPELL_MAGE_DISPLACEMENT_AURA);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_mage_displacement::CheckCast);
        OnCast += SpellCastFn(spell_mage_displacement::HandleCast);
    }
};

// 81012 - Greater Invisibility
class spell_mage_greater_invisibility : public AuraScript
{
    PrepareAuraScript(spell_mage_greater_invisibility);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        caster->AddAura(SPELL_MAGE_GREATER_INVISIBILITY_AURA, caster);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes  /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Aura* buffAura = caster->GetAura(SPELL_MAGE_GREATER_INVISIBILITY_AURA))
        {
            int32 duration = sSpellMgr->AssertSpellInfo(SPELL_MAGE_GREATER_INVISIBILITY)->GetEffect(EFFECT_1).CalcValue(caster);
            buffAura->SetDuration(duration);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_greater_invisibility::HandleApply, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_greater_invisibility::HandleRemove, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 81514 - Mass Barrier
class spell_mage_mass_barrier : public SpellScript
{
    PrepareSpellScript(spell_mage_mass_barrier);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        uint32 const maxTargets = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(GetCaster());
        int32 procSpell = 0;

        if (caster->HasAura(MASTERY_MAGE_SAVANT))
            procSpell = SPELL_MAGE_PRISMATIC_BARRIER;

        if (caster->HasAura(MASTERY_MAGE_IGNITE))
            procSpell = SPELL_MAGE_BLAZING_BARRIER;

        if (caster->HasAura(MASTERY_MAGE_ICICLE))
            procSpell = SPELL_MAGE_ICE_BARRIER;

        if (caster->HasAura(MASTERY_MAGE_BATTLE_KNOWLEDGE))
            procSpell = SPELL_MAGE_GALVANIZING_BARRIER;

        if (procSpell == 0)
            return;

        targets.remove_if(Acore::ObjectGUIDCheck(caster->GetGUID(), true));

        if (targets.size() > maxTargets)
        {
            targets.sort(Acore::HealthPctOrderPred());
            targets.resize(maxTargets);
        }

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead())
                continue;

            target->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
        }

        caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_mass_barrier::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// 81517 - Alter Time
class spell_mage_alter_time : public AuraScript
{
    PrepareAuraScript(spell_mage_alter_time);

    Aura* GetTemporalVelocityAura(Unit* caster)
    {
        for (size_t i = 300932; i < 300938; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetMasterOfTimeAura(Unit* caster)
    {
        for (size_t i = 301066; i < 301072; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        originalHealth = caster->GetHealth();
        originalPosition = caster->GetPosition();
        originalMap = caster->GetMap();
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes  mode)
    {
        Unit* caster = GetCaster();
        AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();

        if (!caster || caster->isDead())
            return;

        if (removeMode != AURA_REMOVE_BY_EXPIRE && removeMode != AURA_REMOVE_BY_CANCEL)
            return;

        if (originalMap != caster->GetMap())
            return;

        caster->NearTeleportTo(originalPosition);
        caster->SetHealth(originalHealth);

        if (GetTemporalVelocityAura(caster))
        {
            int32 procSpell = GetTemporalVelocityAura(caster)->GetEffect(EFFECT_1)->GetAmount();
            caster->AddAura(procSpell, caster);
        }

        if (Player* player = caster->ToPlayer())
        {
            if (GetMasterOfTimeAura(caster))
                player->RemoveSpellCooldown(SPELL_MAGE_BLINK, true);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_alter_time::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_mage_alter_time::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }

private:
    Position originalPosition;
    int32 originalHealth;
    Map* originalMap;
};

// 81518 - Arcane Surge
class spell_mage_arcane_surge : public SpellScript
{
    PrepareSpellScript(spell_mage_arcane_surge);

    void HandleDummy(SpellEffIndex effIndex)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();

        if (!target || target->isDead())
            return;

        int32 maxMana = caster->GetMaxPower(POWER_MANA);
        int32 currentMana = caster->GetPower(POWER_MANA);
        int32 maxDamage = CalculatePct(caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ARCANE), GetSpellInfo()->GetEffect(effIndex).CalcValue(caster));
        float manaPct = 100 * currentMana / maxMana;
        int32 amount = CalculatePct(maxDamage, manaPct);

        caster->SetPower(POWER_MANA, 0);
        caster->CastCustomSpell(SPELL_MAGE_ARCANE_SURGE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_mage_arcane_surge::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 81524 - Supernova
class spell_mage_supernova : public SpellScript
{
    PrepareSpellScript(spell_mage_supernova);

    void HandleDummy(SpellEffIndex effIndex)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();

        if (!target || target->isDead())
            return;

        Position targetPosition = target->GetPosition();

        caster->CastSpell(targetPosition.GetPositionX(), targetPosition.GetPositionY(), targetPosition.GetPositionZ(), SPELL_MAGE_SUPERNOVA_DAMAGE, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_mage_supernova::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 81525 - Supernova AOE
class spell_mage_supernova_aoe : public SpellScript
{
    PrepareSpellScript(spell_mage_supernova_aoe);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead())
                continue;

            target->CastSpell(target, SPELL_MAGE_SUPERNOVA_KNOCKUP, true, 0, 0, caster->GetGUID());
            //target->CastSpell(target, SPELL_MAGE_SUPERNOVA_KNOCKUP, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_supernova_aoe::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// 81528 - Cauterize
class spell_mage_cauterize : public AuraScript
{
    PrepareAuraScript(spell_mage_cauterize);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* caster = GetTarget();
        int32 remainingHealth = caster->GetHealth() - dmgInfo.GetDamage();

        // If damage kills us
        if (remainingHealth <= 0 && !caster->HasAura(SPELL_MAGE_CAUTERIZE_DEBUFF))
        {
            // Avoid damage
            absorbAmount = dmgInfo.GetDamage();

            // Set health to desired amount : 35% max Health
            int32 targetHealth = CalculatePct(caster->GetMaxHealth(), GetAura()->GetEffect(EFFECT_2)->GetAmount());
            caster->SetHealth(targetHealth);
            caster->AddAura(SPELL_MAGE_CAUTERIZE_DEBUFF, caster);

            // Set damage for the burn
            int32 amount = CalculatePct(caster->GetMaxHealth(), GetAura()->GetEffect(EFFECT_1)->GetAmount());
            caster->CastCustomSpell(SPELL_MAGE_CAUTERIZE_AURA, SPELLVALUE_BASE_POINT1, amount, caster, TRIGGERED_FULL_MASK);

        }
        else
            absorbAmount = 0;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_cauterize::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_cauterize::Absorb, EFFECT_0);
    }
};

// 81534 - Flurry
class spell_mage_flurry_proc : public SpellScript
{
    PrepareSpellScript(spell_mage_flurry_proc);

    void HandleAfterHit()
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();

        if (Aura* wintersChill = target->GetAura(SPELL_MAGE_WINTERS_CHILL))
        {
            int32 maxStacks = sSpellMgr->AssertSpellInfo(SPELL_MAGE_FLURRY)->GetEffect(EFFECT_1).CalcValue(caster);

            if (wintersChill->GetStackAmount() == maxStacks)
                return;

            wintersChill->SetStackAmount(maxStacks);
        }
        else
        {
            caster->AddAura(SPELL_MAGE_WINTERS_CHILL, target);
            if (Aura* wintersChill = target->GetAura(SPELL_MAGE_WINTERS_CHILL))
            {
                int32 maxStacks = sSpellMgr->AssertSpellInfo(SPELL_MAGE_FLURRY)->GetEffect(EFFECT_1).CalcValue(caster);

                wintersChill->SetStackAmount(maxStacks);
            }
        }
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_mage_flurry_proc::HandleAfterHit);
    }
};

// 81533 - Flurry
class spell_mage_flurry : public AuraScript
{
    PrepareAuraScript(spell_mage_flurry);

    Aura* GetSplinteringColdAura(Unit* caster)
    {
        for (size_t i = 300366; i < 300372; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(MASTERY_MAGE_ICICLE))
            caster->CastSpell(caster, MASTERY_MAGE_ICICLE_BUFF, TRIGGERED_FULL_MASK);

        if (GetSplinteringColdAura(caster))
        {
            int32 procChance = GetSplinteringColdAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            if (roll_chance_i(procChance))
                caster->CastSpell(caster, MASTERY_MAGE_ICICLE_BUFF, TRIGGERED_FULL_MASK);
        }
    }


    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        Unit* target = GetUnitOwner();

        if (!caster || caster->isDead())
            return;

        if (!target || target->isDead())
            return;

        caster->CastSpell(target, SPELL_MAGE_FLURRY_DAMAGE, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // Handle Brain Freeze removal
        if (Aura* brainFreeze = caster->GetAura(SPELL_MAGE_BRAIN_FREEZE_BUFF))
            if (brainFreeze->GetDuration() < brainFreeze->GetMaxDuration() - 500)
                brainFreeze->Remove();

        // remove Brain Fire damage buff
        for (size_t i = 300384; i < 300390; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_flurry::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_flurry::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_flurry::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 81534 - Winter's Chill
class spell_mage_winters_chill_stack : public SpellScript
{
    PrepareSpellScript(spell_mage_winters_chill_stack);

    void HandleAfterHit()
    {
        if (Aura* wintersChill = GetHitUnit()->GetAura(SPELL_MAGE_WINTERS_CHILL))
            wintersChill->ModStackAmount(-1);
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_mage_winters_chill_stack::HandleAfterHit);
    }
};

// 42914 - Ice Lance
class spell_mage_ice_lance : public SpellScript
{
    PrepareSpellScript(spell_mage_ice_lance);

    Aura* GetCryopathyAura(Unit* caster)
    {
        for (size_t i = 301390; i < 301396; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Aura* T1bonus4 = caster->GetAura(T1_MAGE_FROST_4PC))
            if (roll_chance_i(T1bonus4->GetEffect(EFFECT_0)->GetAmount()))
            {
                if (Player* player = GetCaster()->ToPlayer())
                    player->RemoveSpellCooldown(SPELL_MAGE_FLURRY, true);

                caster->CastSpell(caster, SPELL_MAGE_BRAIN_FREEZE_BUFF, TRIGGERED_FULL_MASK);
            }              
    }

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetHitUnit();

        if (!target || target->isDead())
            return;

        if (Aura* fingerOfFrost = caster->GetAura(SPELL_MAGE_FINGERS_OF_FROST_VISUAL))
        {
            fingerOfFrost->ModStackAmount(-1);

            if (GetCryopathyAura(caster))
            {
                int32 procSpell = GetCryopathyAura(caster)->GetEffect(EFFECT_0)->GetAmount();

                caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
            }
        }

        else if (Aura* wintersChill = target->GetAura(SPELL_MAGE_WINTERS_CHILL))
            wintersChill->ModStackAmount(-1);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mage_ice_lance::HandleCast);
        AfterHit += SpellHitFn(spell_mage_ice_lance::HandleAfterHit);
    }
};

// 44543 - Fingers of Frost / 81504 - Frostbolt / 80013 - Frozen Orb
class spell_mage_fingers_of_frost : public AuraScript
{
    PrepareAuraScript(spell_mage_fingers_of_frost);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetSpellInfo() || !eventInfo.GetActionTarget())
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procChance = _procChance;

        if (eventInfo.GetSpellInfo()->Id == SPELL_MAGE_FROSTBOLT)
            procChance += aurEff->GetAmount();
        else if (eventInfo.GetSpellInfo()->Id == SPELL_MAGE_FROZEN_ORB_DAMAGE)
            procChance += GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (!roll_chance_i(procChance))
        {
            _procChance += 2;
            return;
        }
        _procChance = 0;

        caster->CastSpell(caster, SPELL_MAGE_FINGERS_OF_FROST_AURA, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mage_fingers_of_frost::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mage_fingers_of_frost::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
private:
    int32 _procChance = 0;
};

// 44544 - Fingers of Frost
class spell_mage_fingers_of_frost_proc : public SpellScript
{
    PrepareSpellScript(spell_mage_fingers_of_frost_proc);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (Aura* fingers = caster->GetAura(SPELL_MAGE_FINGERS_OF_FROST_AURA))
        {
            if (Aura* fingersVisual = caster->GetAura(SPELL_MAGE_FINGERS_OF_FROST_VISUAL))
            {
                int32 maxStacks = GetSpellInfo()->GetEffect(EFFECT_2).CalcValue(caster);

                if (int32 currentStacks = fingersVisual->GetStackAmount() < maxStacks)
                    fingersVisual->ModStackAmount(1);

                fingersVisual->RefreshDuration();
            }
            else
                caster->AddAura(SPELL_MAGE_FINGERS_OF_FROST_VISUAL, caster);

            fingers->RefreshDuration();
        }
        else
            caster->AddAura(SPELL_MAGE_FINGERS_OF_FROST_VISUAL, caster);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mage_fingers_of_frost_proc::HandleCast);
    }
};

// 81536 - Comet Storm
class spell_mage_comet_storm : public AuraScript
{
    PrepareAuraScript(spell_mage_comet_storm);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        Unit* target = GetUnitOwner();

        if (!caster || caster->isDead())
            return;

        if (!target || target->isDead())
            return;

        Position targetPosition = target->GetPosition();
        //int32 rand = urand(0, GetAura()->GetEffect(EFFECT_1)->GetAmount());
        //Position position = targetPosition.GetPositionWithOffset(rand);

        caster->CastSpell(target, SPELL_MAGE_COMET_STORM_DAMAGE, true);
    }

    void Register() override
    {

        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_comet_storm::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 81539 - Ray of Frost fingers of frost proc
class spell_mage_ray_of_frost_fingers : public AuraScript
{
    PrepareAuraScript(spell_mage_ray_of_frost_fingers);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        caster->CastSpell(caster, SPELL_MAGE_FINGERS_OF_FROST_AURA, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_ray_of_frost_fingers::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 81538 - Ray of Frost Proc
class spell_mage_ray_of_frost_proc : public AuraScript
{
    PrepareAuraScript(spell_mage_ray_of_frost_proc);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        caster->AddAura(SPELL_MAGE_RAY_OF_FROST_FINGERS, caster);
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        GetAura()->GetEffect(EFFECT_0)->RecalculateAmount(caster);
        GetAura()->GetEffect(EFFECT_1)->RecalculateAmount(caster);
        caster->CastSpell(caster, SPELL_MAGE_RAY_OF_FROST_BUFF, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // Remove Cryopathy damage buff
        for (size_t i = 301396; i < 301402; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_ray_of_frost_proc::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_ray_of_frost_proc::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_ray_of_frost_proc::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_nether_tempest_aoe : public AuraScript
{
    PrepareAuraScript(spell_mage_nether_tempest_aoe);

    void OnPeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetTarget();

        if (!target || target->isDead())
            return;

        target->CastSpell(target, 81522, TRIGGERED_FULL_MASK, nullptr, nullptr, caster->GetGUID());
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_nether_tempest_aoe::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_mage_nether_tempest : public SpellScript
{
    PrepareSpellScript(spell_mage_nether_tempest);

    void FilterTargets(std::list<WorldObject*>& targets)
    {

        targets.remove_if([&](WorldObject* target) -> bool
            {
                Unit* unit = target->ToUnit();
        if (unit->IsAlive() && unit->IsVisible() && !unit->IsFriendlyTo(GetCaster()))
            return true;

        return false;
            });
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_nether_tempest::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
    }
};

class spell_mage_greater_invisibility_replacer : public AuraScript
{
    PrepareAuraScript(spell_mage_greater_invisibility_replacer);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_MAGE_INVISIBILITY, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_MAGE_GREATER_INVISIBILITY);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_MAGE_GREATER_INVISIBILITY, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_MAGE_INVISIBILITY);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_greater_invisibility_replacer::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_greater_invisibility_replacer::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_masterful_combustion : public AuraScript
{
    PrepareAuraScript(spell_mage_masterful_combustion);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = CalculatePct(GetCaster()->GetFloatValue(static_cast<uint16>(PLAYER_SPELL_CRIT_PERCENTAGE1) + SPELL_SCHOOL_FIRE), amount);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_masterful_combustion::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_RATING);
    }
};

class spell_mage_flurry_of_slashes : public AuraScript
{
    PrepareAuraScript(spell_mage_flurry_of_slashes);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // remove Arcane Infusion rune damage buff
        for (size_t i = 301504; i < 301510; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }

        // remove Arcane Onslaught rune damage buff
        for (size_t i = 301670; i < 301676; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_flurry_of_slashes::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_flurry_of_slashes_listener : public AuraScript
{
    PrepareAuraScript(spell_mage_flurry_of_slashes_listener);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 reduction = aurEff->GetAmount();
        if (Player* caster = GetCaster()->ToPlayer())
            caster->ModifySpellCooldown(SPELL_MAGE_FLURRY_OF_SLASHES, reduction);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_flurry_of_slashes_listener::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mage_arcanic_slash : public SpellScript
{
    PrepareSpellScript(spell_mage_arcanic_slash);

    Aura* GetArcaneEchoesAura(Unit* caster)
    {
        for (size_t i = 301510; i < 301516; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            uint32 reduction = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(caster);
            uint32 targetQte = targets.size();

            if (targetQte == 0)
                return;

            reduction = reduction * targetQte;

            caster->ModifySpellCooldown(SPELL_MAGE_FLURRY_OF_SLASHES, reduction);

            if (GetArcaneEchoesAura(caster))
            {
                int32 targetThreshold = GetArcaneEchoesAura(caster)->GetEffect(EFFECT_0)->GetAmount();

                if (targetQte >= targetThreshold)
                {
                    int32 procChance = GetArcaneEchoesAura(caster)->GetEffect(EFFECT_1)->GetAmount();

                    if (roll_chance_i(procChance))
                        caster->CastSpell(caster, SPELL_MAGE_WAVE_OF_FORCE, TRIGGERED_FULL_MASK);
                }
            }
        }
    }

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // Remove Arcane Overload Rune buff
        for (size_t i = 301724; i < 301730; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_arcanic_slash::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
        AfterHit += SpellHitFn(spell_mage_arcanic_slash::HandleAfterHit);
    }
};

class spell_mage_archon : public AuraScript
{
    PrepareAuraScript(spell_mage_archon);

    Aura* GetArcaneResonanceAura(Unit* caster)
    {
        for (size_t i = 301554; i < 301560; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        caster->SetDisplayId(19778);

        if (GetArcaneResonanceAura(caster))
        {
            int32 procSpell = GetArcaneResonanceAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            caster->AddAura(procSpell, caster);
        }
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage && GetCaster()->IsAlive())
            {
                int32 damagePct = aurEff->GetAmount();
                int32 damageAmount = CalculatePct(damage, damagePct);
                GetCaster()->CastCustomSpell(SPELL_MAGE_ARCHON_DAMAGE, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
            }
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        GetCaster()->DeMorph();

        // Remove Arcane Resonance buff
        for (size_t i = 301560; i < 301566; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_archon::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectProc += AuraEffectProcFn(spell_mage_archon::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_archon::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_blackhole : public SpellScript
{
    PrepareSpellScript(spell_mage_blackhole);

    std::list <Unit*> FindCreatures(Creature* creature)
    {
        auto const& threatlist = GetCaster()->getAttackers();
        Position creaturepos = creature->GetPosition();
        std::list <Unit*> targetAvailable;

        for (auto const& target : threatlist)
        {
            Position targetpos = target->GetPosition();
            float distance = creature->GetDistance(targetpos);

            if (distance <= 8)
            {
                targetAvailable.push_back(target);
            }
        } return targetAvailable;
    }

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            Position pos = GetExplTargetDest()->GetPosition();

            summon = caster->SummonCreature(600614, pos, TEMPSUMMON_TIMED_DESPAWN, 2200);
            summon->SetOwnerGUID(caster->GetGUID());
            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            summon->SetReactState(REACT_PASSIVE);

            caster->CastSpell(summon, SPELL_MAGE_BLACK_HOLE_AURA, TRIGGERED_FULL_MASK);
            caster->CastSpell(summon, SPELL_MAGE_BLACK_HOLE_TARGET_SELECT, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_mage_blackhole::HandleCast);
    }

private:
    Creature* summon;
};

class spell_mage_black_hole_target_select : public SpellScript
{
    PrepareSpellScript(spell_mage_black_hole_target_select);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Creature* creature = GetCaster()->FindNearestCreature(600614, 30);
        if (!creature || creature->GetCharmerOrOwnerGUID() != GetCaster()->GetGUID())
            return;

        for (auto const& enemy : targets)
        {
            if (Unit* target = enemy->ToUnit())
            {
                if (target->isDead())
                    continue;

                Position targetpos = target->GetPosition();
                Position pos = creature->GetPosition();
                float distance = creature->GetDistance(targetpos);

                if (distance <= 15)
                {
                    if (Creature* creatureTarget = target->ToCreature())
                    {
                        if (CreatureTemplate const* cinfo = sObjectMgr->GetCreatureTemplate(creatureTarget->GetEntry()))
                            if (cinfo->MechanicImmuneMask & 32)
                                return;

                        if (!creatureTarget->isWorldBoss() || !creatureTarget->IsDungeonBoss())
                            target->GetMotionMaster()->MoveJump(pos, 8.0f, 8.0f);
                    }
                }
            }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_black_hole_target_select::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class spell_mage_black_hole_explosion : public SpellScript
{
    PrepareSpellScript(spell_mage_black_hole_explosion);

    Aura* GetDimensionalRiftsAura(Unit* caster)
    {
        for (size_t i = 301580; i < 301586; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetDimensionalRiftsAura(caster))
        {
            Position pos = GetExplTargetDest()->GetPosition();
            int32 procAura = GetDimensionalRiftsAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            int32 duration = sSpellMgr->AssertSpellInfo(procAura)->GetMaxDuration();

            summon = caster->SummonCreature(600615, pos, TEMPSUMMON_TIMED_DESPAWN, duration);
            summon->SetOwnerGUID(caster->GetGUID());
            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            summon->SetReactState(REACT_PASSIVE);

            caster->CastSpell(summon, procAura, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_mage_black_hole_explosion::HandleCast);
    }

private:
    Creature* summon;
};

class spell_mage_death_blossom : public SpellScript
{
    PrepareSpellScript(spell_mage_death_blossom);

    Aura* GetBlossomBladeAura(Unit* caster)
    {
        for (size_t i = 301682; i < 301688; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetBlossomBladeAura(caster))
        {
            int32 procSpell = GetBlossomBladeAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            if (caster->HasAura(SPELL_MAGE_MAGIC_BLOSSOM_BUFF))
                caster->AddAura(procSpell, caster);
        }

        if (Player* player = caster->ToPlayer())
        {
            uint32 reduction = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(caster);
            player->ModifySpellCooldown(SPELL_MAGE_FLURRY_OF_SLASHES, reduction);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mage_death_blossom::HandleCast);
    }
};

class spell_mage_death_blossom_target_select : public SpellScript
{
    PrepareSpellScript(spell_mage_death_blossom_target_select);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetTarget());

        if (!target || target->isDead())
            return;

        targets.remove(target);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_death_blossom_target_select::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
    }
};

class spell_mage_galvanizing_barrier : public AuraScript
{
    PrepareAuraScript(spell_mage_galvanizing_barrier);

    Aura* GetTemporalBarrierAura(Unit* caster)
    {
        for (size_t i = 301712; i < 301718; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
    {
        amount = GetCaster()->CountPctFromMaxHealth(amount);
    }

    void HandleAbsorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            if (GetTemporalBarrierAura(player))
            {
                int32 cooldown = GetTemporalBarrierAura(player)->GetEffect(EFFECT_0)->GetAmount();

                if (dmgInfo.GetDamage() > absorbAmount)
                    player->ModifySpellCooldown(SPELL_MAGE_BLACK_HOLE, -cooldown);
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_galvanizing_barrier::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_galvanizing_barrier::HandleAbsorb, EFFECT_0);
    }
};

class spell_mage_magic_blossom : public SpellScript
{
    PrepareSpellScript(spell_mage_magic_blossom);

    void HandleCast()
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_MAGE_DEATH_BLOSSOM, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mage_magic_blossom::HandleCast);
    }
};

class spell_mage_unstable_anomaly : public AuraScript
{
    PrepareAuraScript(spell_mage_unstable_anomaly);

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* victim = GetTarget();

        if (!victim || victim->isDead())
            return;

        int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
        int32 healPct = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (remainingHealth <= 0 && !victim->HasAura(SPELL_MAGE_UNSTABLE_ANOMALY_COOLDOWN))
        {
            absorbAmount = dmgInfo.GetDamage();
            int32 shieldAmount = int32(victim->CountPctFromMaxHealth(healPct));
            victim->CastCustomSpell(SPELL_MAGE_UNSTABLE_ANOMALY_SHIELD, SPELLVALUE_BASE_POINT0, shieldAmount, victim, true, nullptr, aurEff);
            victim->CastSpell(victim, SPELL_MAGE_UNSTABLE_ANOMALY_KNOCKBACK, TRIGGERED_FULL_MASK);
            victim->CastSpell(victim, SPELL_MAGE_UNSTABLE_ANOMALY_COOLDOWN, TRIGGERED_FULL_MASK);
        }
        else
            absorbAmount = 0;
    }

    void Register() override
    {
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_unstable_anomaly::Absorb, EFFECT_0);
    }
};

class spell_mage_wave_of_force : public SpellScript
{
    PrepareSpellScript(spell_mage_wave_of_force);

    Aura* GetArcaneOverloadAura(Unit* caster)
    {
        for (size_t i = 301718; i < 301724; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (targets.size() == 0)
            return;

        if (GetArcaneOverloadAura(caster))
        {
            int32 procSpell = GetArcaneOverloadAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
        }

        if (Player* player = caster->ToPlayer())
        {
            uint32 reduction = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(player);

            player->ModifySpellCooldown(SPELL_MAGE_FLURRY_OF_SLASHES, reduction);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_wave_of_force::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_mage_enchant_arcanize : public AuraScript
{
    PrepareAuraScript(spell_mage_enchant_arcanize);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && eventInfo.GetProcTarget())
        {
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_MAGE_ENCHANT_ARCANIZE_PROC, true, nullptr, aurEff);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_enchant_arcanize::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mage_enchant_conduit : public AuraScript
{
    PrepareAuraScript(spell_mage_enchant_conduit);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            Unit* caster = GetCaster();

            if (!caster || caster->isDead())
                return;

            int32 amount = CalculatePct(caster->GetMaxPower(POWER_MANA), 0.5);

            if (Aura* T1B4buff = caster->GetAura(T1_MAGE_MAGEBLADE_4PC_BUFF))
                AddPct(amount, T1B4buff->GetEffect(EFFECT_0)->GetAmount());

            caster->CastCustomSpell(SPELL_MAGE_ENCHANT_CONDUIT_PROC, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_enchant_conduit::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mage_enchant_deflection : public AuraScript
{
    PrepareAuraScript(spell_mage_enchant_deflection);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            Unit* caster = GetCaster();

            if (!caster || caster->isDead())
                return;

            int32 amount = caster->CountPctFromMaxHealth(aurEff->GetAmount());

            if (Aura* T1B4buff = caster->GetAura(T1_MAGE_MAGEBLADE_4PC_BUFF))
                AddPct(amount, T1B4buff->GetEffect(EFFECT_0)->GetAmount());

            if (Aura* shield = caster->GetAura(SPELL_MAGE_ENCHANT_DEFLECTION_PROC))
            {
                shield->GetEffect(EFFECT_0)->SetAmount(amount);
                shield->RefreshDuration();
            }
            else
                caster->CastCustomSpell(SPELL_MAGE_ENCHANT_DEFLECTION_PROC, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_enchant_deflection::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mage_enchant_ignis : public AuraScript
{
    PrepareAuraScript(spell_mage_enchant_ignis);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && eventInfo.GetProcTarget())
        {
            GetCaster()->CastSpell(eventInfo.GetProcTarget(), SPELL_MAGE_ENCHANT_IGNIS_PROC, true, nullptr, aurEff);
            if (Aura* ignis = eventInfo.GetProcTarget()->GetAura(SPELL_MAGE_ENCHANT_IGNIS_PROC))
                if (ignis->GetStackAmount() == 5)
                    GetCaster()->CastSpell(eventInfo.GetProcTarget(), SPELL_MAGE_ENCHANT_IGNIS_PROC_STACKED, true, nullptr, aurEff);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_enchant_ignis::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mage_enchant_snowbound : public AuraScript
{
    PrepareAuraScript(spell_mage_enchant_snowbound);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && eventInfo.GetProcTarget())
        {
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_MAGE_ENCHANT_SNOWBOUND_PROC, true, nullptr, aurEff);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_enchant_snowbound::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mage_prismatic_guard : public AuraScript
{
    PrepareAuraScript(spell_mage_prismatic_guard);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();
        target->SetCanParry(true);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();
        target->SetCanParry(false);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_prismatic_guard::HandleLearn, EFFECT_0, SPELL_AURA_MOD_PARRY_PERCENT_FROM_CRITICAL_STRIKE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_prismatic_guard::HandleUnlearn, EFFECT_0, SPELL_AURA_MOD_PARRY_PERCENT_FROM_CRITICAL_STRIKE, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_parry_ward : public AuraScript
{
    PrepareAuraScript(spell_mage_parry_ward);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 amount = caster->CountPctFromMaxHealth(aurEff->GetAmount());
        caster->CastCustomSpell(SPELL_MAGE_TALENT_PARRY_WARD_PROC, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_parry_ward::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mage_arcanic_barrier : public AuraScript
{
    PrepareAuraScript(spell_mage_arcanic_barrier);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            Unit* caster = GetCaster();

            if (!caster || caster->isDead())
                return;

            int32 amount = caster->CountPctFromMaxHealth(aurEff->GetAmount());
            if (Aura* shield = caster->GetAura(SPELL_MAGE_TALENT_ARCANIC_BARRIER_PROC))
            {
                shield->GetEffect(EFFECT_0)->SetAmount(amount);
                shield->RefreshDuration();
            }
            else
                caster->CastCustomSpell(SPELL_MAGE_TALENT_ARCANIC_BARRIER_PROC, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_arcanic_barrier::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mage_infused_blades : public AuraScript
{
    PrepareAuraScript(spell_mage_infused_blades);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage && GetCaster()->IsAlive())
            {
                int32 damagePct = aurEff->GetAmount();
                int32 damageAmount = CalculatePct(damage, damagePct);
                GetCaster()->CastCustomSpell(SPELL_MAGE_TALENT_INFUSED_BLADES_PROC, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_infused_blades::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mage_blade_master : public AuraScript
{
    PrepareAuraScript(spell_mage_blade_master);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->learnSpell(SPELL_MAGE_MAGIC_BLOSSOM);
        target->learnSpell(SPELL_MAGE_ENCHANT_ARCANIZE);
        target->learnSpell(SPELL_MAGE_ENCHANT_FORCE);
        target->learnSpell(SPELL_MAGE_ENCHANT_CONDUIT);
        target->learnSpell(SPELL_MAGE_ENCHANT_IGNIS);
        target->learnSpell(SPELL_MAGE_ENCHANT_DEFLECTION);
        target->learnSpell(SPELL_MAGE_ENCHANT_SNOWBOUND);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_MAGE_MAGIC_BLOSSOM, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_MAGE_ENCHANT_ARCANIZE, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_MAGE_ENCHANT_FORCE, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_MAGE_ENCHANT_CONDUIT, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_MAGE_ENCHANT_IGNIS, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_MAGE_ENCHANT_DEFLECTION, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_MAGE_ENCHANT_SNOWBOUND, SPEC_MASK_ALL, false);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_blade_master::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_blade_master::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_improved_death_blossom : public AuraScript
{
    PrepareAuraScript(spell_mage_improved_death_blossom);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage && GetCaster()->IsAlive())
            {
                int32 damagePct = aurEff->GetAmount();
                int32 damageAmount = CalculatePct(damage, damagePct);
                GetCaster()->CastSpell(eventInfo.GetProcTarget(), SPELL_MAGE_TALENT_IMPROVED_DEATH_BLOSSOM, true, nullptr, aurEff);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mage_improved_death_blossom::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 12051 Evocation - 300925 Evocation (Movement)
class spell_mage_evocation : public AuraScript
{
    PrepareAuraScript(spell_mage_evocation);

    Aura* GetSiphonStormAura(Unit* caster)
    {
        for (size_t i = 300598; i < 300604; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetSiphonStormAura(caster))
            caster->CastSpell(caster, RUNE_MAGE_SIPHON_STORM_BUFF, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_evocation::HandlePeriodic, EFFECT_0, SPELL_AURA_OBS_MOD_POWER);
    }
};

// 42859 - Scorch
class spell_mage_scorch : public SpellScript
{
    PrepareSpellScript(spell_mage_scorch);

    Aura* GetSearingTouchAura(Unit* caster)
    {
        for (size_t i = 300741; i < 300747; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetCardinalAccelerationAura(Unit* caster)
    {
        for (size_t i = 301072; i < 301078; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetExplTargetUnit();

        if (!target || target->isDead())
            return;

        if (GetSearingTouchAura(caster))
        {
            int32 healthThreshold = GetSearingTouchAura(caster)->GetEffect(EFFECT_1)->GetAmount();

            if (target->GetHealthPct() < healthThreshold)
                caster->AddAura(RUNE_MAGE_SEARING_TOUCH_BUFF, caster);
        }

        if (GetCardinalAccelerationAura(caster))
        {
            int32 healthThreshold = GetCardinalAccelerationAura(caster)->GetEffect(EFFECT_2)->GetAmount();
            int32 procSpell = GetCardinalAccelerationAura(caster)->GetEffect(EFFECT_1)->GetAmount();

            if (target->GetHealthPct() < healthThreshold)
                caster->AddAura(procSpell, target);
        }
    }

    void HandleDamage(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetHitUnit();

        if (!target || target->isDead())
            return;

        int32 damage = GetHitDamage();

        if (GetSearingTouchAura(caster))
        {
            int32 damageIncrease = GetSearingTouchAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            int32 healthThreshold = GetSearingTouchAura(caster)->GetEffect(EFFECT_1)->GetAmount();

            if (target->GetHealthPct() < healthThreshold)
                AddPct(damage, damageIncrease);
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_mage_scorch::HandleCast);
        OnEffectHitTarget += SpellEffectFn(spell_mage_scorch::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 42931 - Cone of Cold
class spell_mage_cone_of_cold : public SpellScript
{
    PrepareSpellScript(spell_mage_cone_of_cold);

    Aura* GetColdestSnapAura(Unit* caster)
    {
        for (size_t i = 301384; i < 301390; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 targetQte = targets.size();

        if (Player* player = caster->ToPlayer())
        {
            if (GetColdestSnapAura(caster))
            {
                int32 targetThreshold = GetColdestSnapAura(caster)->GetEffect(EFFECT_0)->GetAmount();

                if (targetQte >= targetThreshold)
                {
                    player->RemoveSpellCooldown(SPELL_MAGE_FROZEN_ORB, true);
                    player->RemoveSpellCooldown(SPELL_MAGE_COMET_STORM, true);
                }
            }
        }
    }

    void HandleEffectHitTarget(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetHitUnit();

        if (!target || target->isDead())
            return;

        if (GetColdestSnapAura(caster))
            caster->CastSpell(target, SPELL_MAGE_WINTERS_CHILL, TRIGGERED_FULL_MASK);
    }

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // remove Snowstorm Cone of Cold buff
        for (size_t i = 301036; i < 301042; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_cone_of_cold::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
        OnEffectHitTarget += SpellEffectFn(spell_mage_cone_of_cold::HandleEffectHitTarget, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
        AfterHit += SpellHitFn(spell_mage_cone_of_cold::HandleAfterHit);
    }
};

// 42926 - Flamestrike
class spell_mage_flamestrike : public SpellScript
{
    PrepareSpellScript(spell_mage_flamestrike);

    Aura* GetFuelTheFireAura(Unit* caster)
    {
        for (size_t i = 301042; i < 301048; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetFuelTheFireAura(caster))
        {
            if (AuraEffect* hotStreak = caster->GetAura(PASSIVE_MAGE_HOT_STREAK)->GetEffect(EFFECT_1))
            {
                int32 chancePct = GetFuelTheFireAura(caster)->GetEffect(EFFECT_0)->GetAmount();
                float critChance = caster->GetFloatValue(static_cast<uint16>(PLAYER_SPELL_CRIT_PERCENTAGE1) + SPELL_SCHOOL_FIRE);
                int32 procChance = CalculatePct(critChance, chancePct);

                if (roll_chance_i(procChance))
                {
                    hotStreak->SetAmount(hotStreak->GetAmount() * 2);
                    if (hotStreak->GetAmount() < 100) // not enough
                        return;

                    caster->CastSpell(caster, PASSIVE_MAGE_HOT_STREAK_BUFF, TRIGGERED_FULL_MASK);
                    hotStreak->SetAmount(25);
                }
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mage_flamestrike::HandleCast);
    }
};

// 44401 - Missile Barrage Proc
class spell_mage_missile_barrage_proc : public AuraScript
{
    PrepareAuraScript(spell_mage_missile_barrage_proc);

    Aura* GetAmplificationAura(Unit* caster)
    {
        for (size_t i = 301160; i < 301166; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetAmplificationAura(caster))
        {
            int32 procSpell = GetAmplificationAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            caster->AddAura(procSpell, caster);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_missile_barrage_proc::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

// 42833 - Fireball
class spell_mage_fireball : public SpellScript
{
    PrepareSpellScript(spell_mage_fireball);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // remove Flame Accelerant listener
        for (size_t i = 301264; i < 301270; i++)
            if (caster->HasAura(i))
                caster->RemoveAura(i);
    }

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // remove Flame Accelerant buff
        for (size_t i = 301270; i < 301276; i++)
            if (caster->HasAura(i))
                caster->RemoveAura(i);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mage_fireball::HandleCast);
        AfterHit += SpellHitFn(spell_mage_fireball::HandleAfterHit);
    }
};

// 81504 - Frostbolt
class spell_mage_frostbolt : public SpellScript
{
    PrepareSpellScript(spell_mage_frostbolt);

    Aura* GetDeepShatterAura(Unit* caster)
    {
        for (size_t i = 301346; i < 301352; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetFracturedFrostAura(Unit* caster)
    {
        for (size_t i = 301352; i < 301358; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleBeforeCast()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // Fractured Frost multi target
        if (GetFracturedFrostAura(caster))
        {
            int32 procChance = GetFracturedFrostAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            if (roll_chance_i(procChance))
                caster->AddAura(RUNE_MAGE_FRACTURED_FROST_BUFF, caster);
        }
    }

    void HandleDamage(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetHitUnit();

        if (!target || target->isDead())
            return;

        int32 damage = GetHitDamage();
        // Deep Shatter damage increase
        if (GetDeepShatterAura(caster))
        {
            if (target->HasAuraState(AURA_STATE_FROZEN))
            {
                int32 damageIncrease = GetDeepShatterAura(caster)->GetEffect(EFFECT_0)->GetAmount();
                AddPct(damage, damageIncrease);
            }
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_mage_frostbolt::HandleBeforeCast);
        OnEffectHitTarget += SpellEffectFn(spell_mage_frostbolt::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 12472 - Icy Veins
class spell_mage_icy_veins : public SpellScript
{
    PrepareSpellScript(spell_mage_icy_veins);

    Aura* GetCryopathyAura(Unit* caster)
    {
        for (size_t i = 301390; i < 301396; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetCryopathyAura(caster))
        {
            int32 procSpell = GetCryopathyAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            int32 stackAmount = GetCryopathyAura(caster)->GetEffect(EFFECT_1)->GetAmount();

            caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
            caster->GetAura(procSpell)->SetStackAmount(stackAmount);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mage_icy_veins::HandleCast);
    }
};

class spell_mage_ice_nova_target_select : public SpellScript
{
    PrepareSpellScript(spell_mage_ice_nova_target_select);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetTarget());

        if (!target || target->isDead())
            return;

        targets.remove(target);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_ice_nova_target_select::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_ice_nova_target_select::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// 12536 - Clearcasting
class spell_mage_clearcasting : public AuraScript
{
    PrepareAuraScript(spell_mage_clearcasting);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(T1_MAGE_ARCANE_2PC))
            caster->CastSpell(caster, T1_MAGE_ARCANE_2PC_BUFF, TRIGGERED_FULL_MASK);

        if (caster->HasAura(T1_MAGE_ARCANE_4PC))
            caster->CastSpell(caster, T1_MAGE_ARCANE_4PC_LISTENER, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_mage_clearcasting::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};



void AddSC_mage_spell_scripts()
{
    new npc_mage_spell_frozen_orbs();
    new npc_mage_spell_arcane_orbs();
    RegisterSpellScript(spell_mage_nether_tempest);
    RegisterSpellScript(spell_mage_nether_tempest_aoe);
    RegisterSpellScript(spell_mage_arcane_blast);
    RegisterSpellScript(spell_mage_burning_determination);
    RegisterSpellScript(spell_mage_molten_armor);
    RegisterSpellScript(spell_mage_mirror_image);
    RegisterSpellScript(spell_mage_burnout);
    RegisterSpellScript(spell_mage_burnout_trigger);
    RegisterSpellScript(spell_mage_pet_scaling);
    RegisterSpellScript(spell_mage_brain_freeze);
    RegisterSpellScript(spell_mage_glyph_of_eternal_water);
    RegisterSpellScript(spell_mage_combustion_proc);
    RegisterSpellScript(spell_mage_blast_wave);
    RegisterSpellScript(spell_mage_cold_snap);
    RegisterSpellScript(spell_mage_fire_frost_ward);
    RegisterSpellScript(spell_mage_barrier_absorb_aura);
    RegisterSpellScript(spell_mage_barrier);
    RegisterSpellScript(spell_mage_focus_magic);
    RegisterSpellScript(spell_mage_ignite);
    RegisterSpellScript(spell_mage_living_bomb);
    RegisterSpellScript(spell_mage_mana_shield);
    RegisterSpellScript(spell_mage_master_of_elements);
    RegisterSpellScript(spell_mage_polymorph_cast_visual);
    RegisterSpellScript(spell_mage_summon_water_elemental);
    //RegisterSpellScript(spell_mage_fingers_of_frost_proc_aura);
    //RegisterSpellScript(spell_mage_fingers_of_frost_proc);
    RegisterSpellScript(spell_mage_arcane_barrage);
    RegisterSpellScript(spell_mage_frozen_orbs);
    RegisterSpellScript(spell_mage_frozen_orb_damage);
    RegisterSpellScript(spell_mage_frozen_orb_aura);
    RegisterSpellScript(spell_mage_rule_of_threes);
    RegisterSpellScript(spell_mage_arcane_meditation);
    RegisterSpellScript(spell_mage_improved_fireball);
    RegisterSpellScript(spell_mage_arcane_orb);
    RegisterSpellScript(spell_mage_raging_winds);
    RegisterSpellScript(spell_mage_pheonix_flame);
    RegisterSpellScript(spell_mage_empowered_fire);
    RegisterSpellScript(spell_mage_combustion);
    RegisterSpellScript(spell_mage_arcane_charge);
    RegisterSpellScript(spell_mage_arcane_explosion);
    RegisterSpellScript(spell_mage_arcane_missiles_aura);
    RegisterSpellScript(spell_mage_arcane_missile_damage);
    RegisterSpellScript(spell_mage_blink_displacement);
    RegisterSpellScript(spell_mage_displacement);
    RegisterSpellScript(spell_mage_greater_invisibility);
    RegisterSpellScript(spell_mage_mass_barrier);
    RegisterSpellScript(spell_mage_alter_time);
    RegisterSpellScript(spell_mage_arcane_surge);
    RegisterSpellScript(spell_mage_supernova);
    RegisterSpellScript(spell_mage_supernova_aoe);
    RegisterSpellScript(spell_mage_cauterize);
    RegisterSpellScript(spell_mage_flurry);
    RegisterSpellScript(spell_mage_flurry_proc);
    RegisterSpellScript(spell_mage_winters_chill_stack);
    RegisterSpellScript(spell_mage_ice_lance);
    RegisterSpellScript(spell_mage_fingers_of_frost);
    RegisterSpellScript(spell_mage_fingers_of_frost_proc);
    RegisterSpellScript(spell_mage_comet_storm);
    RegisterSpellScript(spell_mage_ray_of_frost_fingers);
    RegisterSpellScript(spell_mage_ray_of_frost_proc);
    RegisterSpellScript(spell_mage_greater_invisibility_replacer);
    RegisterSpellScript(spell_mage_masterful_combustion);
    RegisterSpellScript(spell_mage_flurry_of_slashes);
    RegisterSpellScript(spell_mage_flurry_of_slashes_listener);
    RegisterSpellScript(spell_mage_arcanic_slash);
    RegisterSpellScript(spell_mage_archon);
    RegisterSpellScript(spell_mage_blackhole);
    RegisterSpellScript(spell_mage_black_hole_target_select);
    RegisterSpellScript(spell_mage_black_hole_explosion);
    RegisterSpellScript(spell_mage_death_blossom);
    RegisterSpellScript(spell_mage_death_blossom_target_select);
    RegisterSpellScript(spell_mage_galvanizing_barrier);
    RegisterSpellScript(spell_mage_magic_blossom);
    RegisterSpellScript(spell_mage_unstable_anomaly);
    RegisterSpellScript(spell_mage_wave_of_force);
    RegisterSpellScript(spell_mage_enchant_arcanize);
    RegisterSpellScript(spell_mage_enchant_conduit);
    RegisterSpellScript(spell_mage_enchant_deflection);
    RegisterSpellScript(spell_mage_enchant_ignis);
    RegisterSpellScript(spell_mage_enchant_snowbound);
    RegisterSpellScript(spell_mage_prismatic_guard);
    RegisterSpellScript(spell_mage_parry_ward);
    RegisterSpellScript(spell_mage_arcanic_barrier);
    RegisterSpellScript(spell_mage_infused_blades);
    RegisterSpellScript(spell_mage_blade_master);
    RegisterSpellScript(spell_mage_improved_death_blossom);
    RegisterSpellScript(spell_mage_evocation);
    RegisterSpellScript(spell_mage_scorch);
    RegisterSpellScript(spell_mage_cone_of_cold);
    RegisterSpellScript(spell_mage_flamestrike);
    RegisterSpellScript(spell_mage_missile_barrage_proc);
    RegisterSpellScript(spell_mage_fireball);
    RegisterSpellScript(spell_mage_frostbolt);
    RegisterSpellScript(spell_mage_icy_veins);
    RegisterSpellScript(spell_mage_ice_nova_target_select);
    RegisterSpellScript(spell_mage_clearcasting);





}
