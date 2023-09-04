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
    SPELL_MAGE_ARCANE_SURGE_DAMAGE = 81519,
    SPELL_MAGE_BLAZING_BARRIER = 81526,
    SPELL_MAGE_BLAZING_BARRIER_DAMAGE = 81527,
    SPELL_MAGE_BURNOUT_TRIGGER = 44450,
    SPELL_MAGE_CAUTERIZE = 81528,
    SPELL_MAGE_CAUTERIZE_AURA = 81529,
    SPELL_MAGE_CAUTERIZE_DEBUFF = 81530,
    SPELL_MAGE_COMBUSTION = 11129,
    SPELL_MAGE_COMET_STORM = 81537,
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
    SPELL_MAGE_INVISIBILITY = 66,
    SPELL_MAGE_GREATER_INVISIBILITY = 81511,
    SPELL_MAGE_GREATER_INVISIBILITY_AURA = 81513,
    SPELL_MAGE_ICE_BARRIER = 43039,
    SPELL_MAGE_ICE_BARRIER_SLOW = 43040,
    SPELL_MAGE_ICE_BLOCK = 45438,
    SPELL_MAGE_ICE_LANCE = 42914,
    SPELL_MAGE_IMPROVED_BLIZZARD_CHILLED = 12486,
    SPELL_MAGE_MIRROR_IMAGE_DAMAGE_REDUCTION = 55343,
    SPELL_MAGE_MIRROR_IMAGE_SUMMON_ID = 31216,
    SPELL_MAGE_PRISMATIC_BARRIER = 81523,
    SPELL_MAGE_PLACEHOLDER_BARRIER = 000,
    SPELL_MAGE_RAY_OF_FROST_FINGERS = 81539,
    SPELL_MAGE_RAY_OF_FROST_BUFF = 81540,
    SPELL_MAGE_SUPERNOVA_DAMAGE = 81525,
    SPELL_MAGE_SUPERNOVA_KNOCKUP = 81541,
    SPELL_MAGE_WINTERS_CHILL = 81535,

    // Masteries
    MASTERY_MAGE_SAVANT = 300111,
    MASTERY_MAGE_IGNITE = 300109,
    MASTERY_MAGE_ICICLE = 300105,
    MASTERY_MAGE_BATTLE_KNOWLEDGE = 000,

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


    SPELL_VISUAL_FROZEN_ORB = 72067,
    SPELL_VISUAL_ARCANE_ORB = 80015,
    SPELL_MAGE_FROZEN_ORB_DAMAGE = 80012,
    SPELL_MAGE_ARCANE_ORB_DAMAGE = 80017,
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

class spell_mage_combustion_on_remove : public AuraScript
{
    PrepareAuraScript(spell_mage_combustion_on_remove);

    void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->RemoveAura(11129);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_mage_combustion_on_remove::OnRemove, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mage_frozen_orb_damage : public SpellScript
{
    PrepareSpellScript(spell_mage_frozen_orb_damage);

    void HandleAfterHit()
    {
        Unit* frozenOrb = GetCaster();

        if (Unit* player = GetCaster()->ToTempSummon()->GetSummonerUnit()) {

            frozenOrb->SetSpeed(MOVE_WALK, 0.10);
            frozenOrb->SetSpeed(MOVE_RUN, 0.10);

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

    void HandleSummon()
    {
        Position pos = *GetCaster();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = GetCaster()->SummonCreature(500500, pos, TEMPSUMMON_TIMED_DESPAWN, GetSpellInfo()->GetDuration(), 0, properties);

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
        OnCast += SpellCastFn(spell_mage_frozen_orbs::HandleSummon);
    }
};

class spell_mage_arcane_orb : public SpellScript
{
    PrepareSpellScript(spell_mage_arcane_orb);

    void HandleSummon()
    {
        Position pos = *GetCaster();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = GetCaster()->SummonCreature(500501, pos, TEMPSUMMON_TIMED_DESPAWN, GetSpellInfo()->GetDuration(), 0, properties);

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

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;

        if (caster->HasAura(SPELL_MAGE_ARCANE_CHARGE_VISUAL))
            caster->RemoveAura(SPELL_MAGE_ARCANE_CHARGE_VISUAL);
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

        for (Unit::ControlSet::iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
            if (Unit* target = (*itr)->ToUnit())
                if (target->GetEntry() == SPELL_MAGE_MIRROR_IMAGE_SUMMON_ID)
                    target->ToCreature()->DespawnOrUnsummon();
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
        Player* player = GetCaster()->ToPlayer();

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

        if (!target || !caster)
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


        caster->CastSpell(caster, SPELL_MAGE_ARCANE_CHARGE_VISUAL, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_arcane_explosion::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// 42847 - Arcane Missile
class spell_mage_arcane_missiles : public SpellScript
{
    PrepareSpellScript(spell_mage_arcane_missiles);

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();

        if (!caster || !caster->HasAura(SPELL_MAGE_ARCANE_CHARGE))
            return;

        GetCaster()->CastSpell(caster, SPELL_MAGE_ARCANE_CHARGE_VISUAL, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_mage_arcane_missiles::HandleAfterCast);
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
            procSpell = SPELL_MAGE_PLACEHOLDER_BARRIER;

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

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        Unit* target = GetUnitOwner();

        if (!caster || caster->isDead())
            return;

        caster->CastSpell(target, SPELL_MAGE_FLURRY_DAMAGE, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {

        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_flurry::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
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

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        if (Aura* fingerOfFrost = caster->GetAura(SPELL_MAGE_FINGERS_OF_FROST_VISUAL))
            fingerOfFrost->ModStackAmount(-1);
        else if (Aura* wintersChill = target->GetAura(SPELL_MAGE_WINTERS_CHILL))
            wintersChill->ModStackAmount(-1);
    }

    void Register() override
    {
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

        Position targetPosition = target->GetPosition();
        //int32 rand = urand(0, GetAura()->GetEffect(EFFECT_1)->GetAmount());
        //Position position = targetPosition.GetPositionWithOffset(rand);

        caster->CastSpell(targetPosition.GetPositionX(), targetPosition.GetPositionY(), targetPosition.GetPositionZ(), SPELL_MAGE_COMET_STORM, true);
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

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_ray_of_frost_proc::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_ray_of_frost_proc::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

class spell_mage_nether_tempest_aoe : public AuraScript
{
    PrepareAuraScript(spell_mage_nether_tempest_aoe);

    void OnPeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        Unit* target = GetTarget();
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
    RegisterSpellScript(spell_mage_rule_of_threes);
    RegisterSpellScript(spell_mage_arcane_meditation);
    RegisterSpellScript(spell_mage_improved_fireball);
    RegisterSpellScript(spell_mage_arcane_orb);
    RegisterSpellScript(spell_mage_raging_winds);
    RegisterSpellScript(spell_mage_pheonix_flame);
    RegisterSpellScript(spell_mage_empowered_fire);
    RegisterSpellScript(spell_mage_combustion_on_remove);
    RegisterSpellScript(spell_mage_arcane_charge);
    RegisterSpellScript(spell_mage_arcane_explosion);
    RegisterSpellScript(spell_mage_arcane_missiles);
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
}
