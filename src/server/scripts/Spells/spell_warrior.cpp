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
  * Scripts for spells with SPELLFAMILY_WARRIOR and SPELLFAMILY_GENERIC spells used by warrior players.
  * Ordered alphabetically using scriptname.
  * Scriptnames of files in this file should be prefixed with "spell_warr_".
  */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Object.h"

enum WarriorSpells
{
    SPELL_WARRIOR_INTERVENE_TRIGGER = 59667,
    SPELL_WARRIOR_SPELL_REFLECTION = 23920,
    SPELL_WARRIOR_IMPROVED_SPELL_REFLECTION_TRIGGER = 59725,
    SPELL_WARRIOR_BLOODTHIRST_HEAL = 23880,
    SPELL_WARRIOR_BLOODTHIRST = 23881,
    SPELL_WARRIOR_BLADESTORM = 46924,
    SPELL_WARRIOR_BLADESTORM_WHIRLWIND = 50622,
    SPELL_WARRIOR_CHARGE = 34846,
    SPELL_WARRIOR_CHARGE_DAMAGE = 7921,
    SPELL_WARRIOR_INTERCEPT_DAMAGE = 20248,
    SPELL_WARRIOR_DAMAGE_SHIELD_DAMAGE = 59653,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_1 = 12162,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_2 = 12850,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_3 = 12868,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC = 12721,
    SPELL_WARRIOR_EXECUTE = 20647,
    SPELL_WARRIOR_GLYPH_OF_EXECUTION = 58367,
    SPELL_WARRIOR_GLYPH_OF_VIGILANCE = 63326,
    SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_BUFF = 65156,
    SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_TALENT = 64976,
    SPELL_WARRIOR_LAST_STAND_TRIGGERED = 12976,
    SPELL_WARRIOR_COMMANDING_SHOUT_TRIGGERED = 47461,
    SPELL_WARRIOR_REND = 47465,
    SPELL_WARRIOR_RETALIATION_DAMAGE = 22858,
    SPELL_WARRIOR_SLAM = 50783,
    SPELL_WARRIOR_SUNDER_ARMOR = 58567,
    SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_1 = 12723,
    SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_2 = 26654,
    SPELL_WARRIOR_TAUNT = 355,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_1 = 46859,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_2 = 46860,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_1 = 64849,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_2 = 64850,
    SPELL_WARRIOR_VIGILANCE_PROC = 50725,
    SPELL_WARRIOR_VIGILANCE_REDIRECT_THREAT = 59665,
    SPELL_WARRIOR_WHIRLWIND_OFF = 44949
};

enum WarriorSpellIcons
{
    WARRIOR_ICON_ID_SUDDEN_DEATH = 1989
};

enum MiscSpells
{
    SPELL_PALADIN_BLESSING_OF_SANCTUARY = 20911,
    SPELL_PALADIN_GREATER_BLESSING_OF_SANCTUARY = 25899,
    SPELL_PRIEST_RENEWED_HOPE = 63944,
    SPELL_GEN_DAMAGE_REDUCTION_AURA = 68066,
};

class spell_warr_mocking_blow : public SpellScript
{
    PrepareSpellScript(spell_warr_mocking_blow);

    void HandleOnHit()
    {
        if (Unit* target = GetHitUnit())
            if (target->IsImmunedToSpellEffect(GetSpellInfo(), EFFECT_1))
                SetHitDamage(0);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_warr_mocking_blow::HandleOnHit);
    }
};

enum VictoryRushEnum
{
    SPELL_VICTORIOUS = 32216
};

class spell_warr_victory_rush : public SpellScript
{
    PrepareSpellScript(spell_warr_victory_rush);

    void VictoryRushHit()
    {
        if (Unit* player = GetCaster())
        {
            if (Unit* victim = GetHitUnit())
            {
                if (victim->isDead())
                {
                    player->CastSpell(player, SPELL_VICTORIOUS, true);
                }
            }
        }
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_warr_victory_rush::VictoryRushHit);
    }
};

class spell_warr_intervene : public SpellScript
{
    PrepareSpellScript(spell_warr_intervene);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARRIOR_INTERVENE_TRIGGER });
    }

    void HandleApplyAura(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            target->CastSpell((Unit*)nullptr, SPELL_WARRIOR_INTERVENE_TRIGGER, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_intervene::HandleApplyAura, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
    }
};

class spell_warr_improved_spell_reflection : public AuraScript
{
    PrepareAuraScript(spell_warr_improved_spell_reflection);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARRIOR_SPELL_REFLECTION, SPELL_WARRIOR_IMPROVED_SPELL_REFLECTION_TRIGGER });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetActor() && eventInfo.GetSpellInfo()->Id == SPELL_WARRIOR_SPELL_REFLECTION;
    }

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        CustomSpellValues values;
        values.AddSpellMod(SPELLVALUE_MAX_TARGETS, aurEff->GetAmount());
        values.AddSpellMod(SPELLVALUE_RADIUS_MOD, 2000); // Base range = 100, final range = 20 value / 10000.0f = 0.2f
        eventInfo.GetActor()->CastCustomSpell(SPELL_WARRIOR_IMPROVED_SPELL_REFLECTION_TRIGGER, values, eventInfo.GetActor(), TRIGGERED_FULL_MASK, nullptr);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warr_improved_spell_reflection::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_warr_improved_spell_reflection::OnProc, EFFECT_1, SPELL_AURA_DUMMY);
    }
};

class spell_warr_improved_spell_reflection_trigger : public SpellScript
{
    PrepareSpellScript(spell_warr_improved_spell_reflection_trigger);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARRIOR_SPELL_REFLECTION });
    }

    void FilterTargets(std::list<WorldObject*>& unitList)
    {
        GetCaster()->RemoveAurasDueToSpell(SPELL_WARRIOR_SPELL_REFLECTION);
        unitList.sort(Acore::ObjectDistanceOrderPred(GetCaster()));
        while (unitList.size() > GetSpellValue()->MaxAffectedTargets)
            unitList.pop_back();
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_improved_spell_reflection_trigger::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_PARTY);
    }
};

class spell_warr_improved_spell_reflection_trigger_aura : public AuraScript
{
    PrepareAuraScript(spell_warr_improved_spell_reflection_trigger_aura);

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes  /*mode*/)
    {
        if (!IsExpired())
        {
            // aura remove - remove auras from all party members
            std::list<Unit*> PartyMembers;
            GetUnitOwner()->GetPartyMembers(PartyMembers);
            for (std::list<Unit*>::iterator itr = PartyMembers.begin(); itr != PartyMembers.end(); ++itr)
            {
                if ((*itr)->GetGUID() != GetOwner()->GetGUID())
                    if (Aura* aur = (*itr)->GetAura(59725, GetCasterGUID()))
                    {
                        aur->SetDuration(0);
                        aur->Remove();
                    }
            }
        }
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_warr_improved_spell_reflection_trigger_aura::HandleRemove, EFFECT_0, SPELL_AURA_REFLECT_SPELLS, AURA_EFFECT_HANDLE_REAL);
    }
};

// 12975 - Last Stand
class spell_warr_last_stand : public SpellScript
{
    PrepareSpellScript(spell_warr_last_stand);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARRIOR_LAST_STAND_TRIGGERED });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(GetEffectValue()));
        caster->CastCustomSpell(caster, SPELL_WARRIOR_LAST_STAND_TRIGGERED, &healthModSpellBasePoints0, nullptr, nullptr, true, nullptr);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_warr_last_stand::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 47440 - Commanding Shout
class spell_warr_commanding_shout : public SpellScript
{
    PrepareSpellScript(spell_warr_commanding_shout);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        if (GetCaster()->HasAura(SPELL_WARRIOR_COMMANDING_SHOUT_TRIGGERED))
            GetCaster()->RemoveAura(SPELL_WARRIOR_COMMANDING_SHOUT_TRIGGERED);

        int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(GetEffectValue()));

        caster->CastCustomSpell(caster, SPELL_WARRIOR_COMMANDING_SHOUT_TRIGGERED, &healthModSpellBasePoints0, nullptr, nullptr, true, nullptr);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_warr_commanding_shout::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// -12162 - Deep Wounds
class spell_warr_deep_wounds : public SpellScript
{
    PrepareSpellScript(spell_warr_deep_wounds);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        int32 damage = std::max(GetEffectValue(), 0);
        Unit* caster = GetCaster();
        if (Unit* target = GetHitUnit())
        {
            // include target dependant auras
            damage = target->MeleeDamageBonusTaken(caster, damage, BASE_ATTACK, GetSpellInfo());
            // apply percent damage mods
            ApplyPct(damage, 16.0f * GetSpellInfo()->GetRank() / 6.0f);
            target->CastDelayedSpellWithPeriodicAmount(caster, SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC, SPELL_AURA_PERIODIC_DAMAGE, damage, EFFECT_0);

            //caster->CastCustomSpell(target, SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC, &damage, nullptr, nullptr, true);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_deep_wounds::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// -100 - Charge
class spell_warr_charge : public SpellScript
{
    PrepareSpellScript(spell_warr_charge);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_TALENT,
                SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_BUFF,
                SPELL_WARRIOR_CHARGE
            });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        int32 chargeBasePoints0 = GetEffectValue();
        Unit* caster = GetCaster();
        caster->CastCustomSpell(caster, SPELL_WARRIOR_CHARGE, &chargeBasePoints0, nullptr, nullptr, true);

        // Juggernaut crit bonus
        if (caster->HasAura(SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_TALENT))
            caster->CastSpell(caster, SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_BUFF, true);

        GetCaster()->CastSpell(GetExplTargetUnit(), SPELL_WARRIOR_CHARGE_DAMAGE);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_charge::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
    }
};

// 7921 Charge Damages
class spell_warr_charge_damage : public SpellScript
{
    PrepareSpellScript(spell_warr_charge_damage);

    void HandleDamage(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_charge_damage::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 20252 - Intercept
class spell_warr_intercept : public SpellScript
{
    PrepareSpellScript(spell_warr_intercept);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        GetCaster()->CastCustomSpell(SPELL_WARRIOR_INTERCEPT_DAMAGE, SPELLVALUE_BASE_POINT0, damage, GetHitUnit(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_intercept::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
    }
};

// 47475 - Slam
class spell_warr_slam : public SpellScript
{
    PrepareSpellScript(spell_warr_slam);

    void HandleDummy(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_slam::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// -58872 - Damage Shield
class spell_warr_damage_shield : public AuraScript
{
    PrepareAuraScript(spell_warr_damage_shield);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARRIOR_DAMAGE_SHIELD_DAMAGE });
    }

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        // % of amount blocked
        int32 damage = CalculatePct(int32(GetTarget()->GetShieldBlockValue()), aurEff->GetAmount());
        GetTarget()->CastCustomSpell(SPELL_WARRIOR_DAMAGE_SHIELD_DAMAGE, SPELLVALUE_BASE_POINT0, damage, eventInfo.GetProcTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warr_damage_shield::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// -5308 - Execute
class spell_warr_execute : public SpellScript
{
    PrepareSpellScript(spell_warr_execute);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARRIOR_EXECUTE, SPELL_WARRIOR_GLYPH_OF_EXECUTION });
    }

    void SendMiss(SpellMissInfo missInfo)
    {
        if (missInfo != SPELL_MISS_NONE)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetHitUnit())
                {
                    caster->SendSpellMiss(target, SPELL_WARRIOR_EXECUTE, missInfo);
                }
            }
        }
    }

    void HandleEffect(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        if (Unit* target = GetHitUnit())
        {
            SpellInfo const* spellInfo = GetSpellInfo();
            int32 rageUsed = std::min<int32>(400 - spellInfo->CalcPowerCost(caster, SpellSchoolMask(spellInfo->SchoolMask)), caster->GetPower(POWER_RAGE));
            int32 newRage = std::max<int32>(0, caster->GetPower(POWER_RAGE) - rageUsed);

            // Sudden Death rage save
            if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_AURA_PROC_TRIGGER_SPELL, SPELLFAMILY_GENERIC, WARRIOR_ICON_ID_SUDDEN_DEATH, EFFECT_0))
            {
                int32 ragesave = aurEff->GetSpellInfo()->Effects[EFFECT_1].CalcValue() * 10;
                newRage = std::max(newRage, ragesave);
            }

            caster->SetPower(POWER_RAGE, uint32(newRage));
            // Glyph of Execution bonus
            if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_WARRIOR_GLYPH_OF_EXECUTION, EFFECT_0))
                rageUsed += aurEff->GetAmount() * 10;

            /*int32 damage = int32(rageUsed * spellInfo->Effects[effIndex].DamageMultiplier + CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), GetEffectValue()));

            if (Unit* target = GetHitUnit())
            {
                damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
                damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
            }
            SetHitDamage(damage);*/

            int32 bp = int32(rageUsed * spellInfo->Effects[effIndex].DamageMultiplier + CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), GetEffectValue()));
            caster->CastCustomSpell(target, SPELL_WARRIOR_EXECUTE, &bp, nullptr, nullptr, true, nullptr, nullptr, GetOriginalCaster()->GetGUID());
        }
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_warr_execute::SendMiss);
        OnEffectHitTarget += SpellEffectFn(spell_warr_execute::HandleEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 12809 - Concussion Blow
class spell_warr_concussion_blow : public SpellScript
{
    PrepareSpellScript(spell_warr_concussion_blow);

    void HandleDummy(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_concussion_blow::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
    }
};

// 50622 - 46924 - Bladestorm Damages
class spell_warr_bladestorm : public AuraScript
{
    PrepareAuraScript(spell_warr_bladestorm);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        const SpellInfo* bladestorm = sSpellMgr->AssertSpellInfo(SPELL_WARRIOR_BLADESTORM);
        int32 damage = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), bladestorm->GetEffect(EFFECT_0).CalcValue());

        GetCaster()->CastCustomSpell(SPELL_WARRIOR_BLADESTORM_WHIRLWIND, SPELLVALUE_BASE_POINT0, damage, GetCaster(), true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_bladestorm::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 23881 - Bloodthirst
class spell_warr_bloodthirst : public SpellScript
{
    PrepareSpellScript(spell_warr_bloodthirst);

    void HandleDamage(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        if (Aura* aura = GetCaster()->GetAura(80035))
            if (GetExplTargetUnit()->HealthBelowPct(35))
                ApplyPct(damage, aura->GetEffect(EFFECT_0)->GetAmount());

        SetHitDamage(damage);
    }

    void HandleDummy(SpellEffIndex effIndex)
    {
        GetCaster()->CastCustomSpell(SPELL_WARRIOR_BLOODTHIRST_HEAL, SPELLVALUE_BASE_POINT0, GetEffectValue(), GetCaster(), true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_bloodthirst::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        OnEffectHit += SpellEffectFn(spell_warr_bloodthirst::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
    }
};



// 7384, 7887, 11584, 11585 - Overpower
class spell_warr_overpower : public SpellScript
{
    PrepareSpellScript(spell_warr_overpower);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({
            SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_1,
            SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_1,
            SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_2,
            SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_2
            });
    }

    void HandleEffect(SpellEffIndex /*effIndex*/)
    {
        uint32 spellId = 0;
        if (GetCaster()->HasAura(SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_1))
            spellId = SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_1;
        else if (GetCaster()->HasAura(SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_2))
            spellId = SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_2;

        if (!spellId)
            return;

        if (Player* target = GetHitPlayer())
            target->CastSpell(target, spellId, true, 0, 0, GetCaster()->GetGUID());
    }

    void HandleDamage(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_overpower::HandleEffect, EFFECT_0, SPELL_EFFECT_ANY);
        OnEffectHitTarget += SpellEffectFn(spell_warr_overpower::HandleDamage, EFFECT_2, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 5246 - Intimidating Shout
class spell_warr_intimidating_shout : public SpellScript
{
    PrepareSpellScript(spell_warr_intimidating_shout);

    void FilterTargets(std::list<WorldObject*>& unitList)
    {
        unitList.remove(GetExplTargetWorldObject());
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_intimidating_shout::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_intimidating_shout::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// -772 - Rend
class spell_warr_rend : public AuraScript
{
    PrepareAuraScript(spell_warr_rend);

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
    {
        if (Unit* caster = GetCaster())
        {
            int32 dotDamageRatio = sSpellMgr->AssertSpellInfo(SPELL_WARRIOR_REND)->GetEffect(EFFECT_0).CalcValue();

            // "If used while your target is above 75% health, Rend does 35% more damage."
            if (GetUnitOwner()->HasAuraState(AURA_STATE_HEALTH_ABOVE_75_PERCENT, GetSpellInfo(), caster))
                AddPct(dotDamageRatio, GetSpellInfo()->Effects[EFFECT_2].CalcValue(caster));

            float ap = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), dotDamageRatio);

            amount = int32(caster->ApplyEffectModifiers(GetSpellInfo(), aurEff->GetEffIndex(), ap));
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_rend::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// 47465 - Rend
class spell_warr_rend_direct_damage : public SpellScript
{
    PrepareSpellScript(spell_warr_rend_direct_damage);

    void HandleDamage(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_rend_direct_damage::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 64380, 65941 - Shattering Throw
class spell_warr_shattering_throw : public SpellScript
{
    PrepareSpellScript(spell_warr_shattering_throw);

    void HandleScript(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);

        // remove shields, will still display immune to damage part
        if (Unit* target = GetHitUnit())
            target->RemoveAurasWithMechanic(1 << MECHANIC_IMMUNE_SHIELD, AURA_REMOVE_BY_ENEMY_SPELL);
    }

    void HandleDamage(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_shattering_throw::HandleScript, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
        OnEffectHitTarget += SpellEffectFn(spell_warr_shattering_throw::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 12328, 18765, 35429 - Sweeping Strikes
class spell_warr_sweeping_strikes : public AuraScript
{
    PrepareAuraScript(spell_warr_sweeping_strikes);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_1, SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_2 });
    }

    bool Load() override
    {
        _procTarget = nullptr;
        return true;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* actor = eventInfo.GetActor();
        if (!actor)
        {
            return false;
        }

        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            switch (spellInfo->Id)
            {
            case SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_1:
            case SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_2:
            case SPELL_WARRIOR_WHIRLWIND_OFF:
                return false;
            default:
                break;
            }
        }

        _procTarget = eventInfo.GetActor()->SelectNearbyNoTotemTarget(eventInfo.GetProcTarget());
        return _procTarget != nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        if (DamageInfo* damageInfo = eventInfo.GetDamageInfo())
        {
            SpellInfo const* spellInfo = damageInfo->GetSpellInfo();
            int32 damage = damageInfo->GetDamageType() != DIRECT_DAMAGE ? damageInfo->GetUnmitigatedDamage() : damageInfo->GetDamage();

            GetTarget()->CastCustomSpell(_procTarget, SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_1, &damage, 0, 0, true, nullptr, aurEff);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warr_sweeping_strikes::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_warr_sweeping_strikes::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }

private:
    Unit* _procTarget = nullptr;
};

// 50720 - Vigilance
class spell_warr_vigilance : public AuraScript
{
    PrepareAuraScript(spell_warr_vigilance);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_WARRIOR_GLYPH_OF_VIGILANCE,
                SPELL_WARRIOR_VIGILANCE_PROC,
                SPELL_WARRIOR_VIGILANCE_REDIRECT_THREAT,
                SPELL_GEN_DAMAGE_REDUCTION_AURA,
                SPELL_PALADIN_BLESSING_OF_SANCTUARY,
                SPELL_PALADIN_GREATER_BLESSING_OF_SANCTUARY,
                SPELL_PRIEST_RENEWED_HOPE
            });
    }

    bool Load() override
    {
        _procTarget = nullptr;
        return true;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        target->CastSpell(target, SPELL_GEN_DAMAGE_REDUCTION_AURA, true);

        if (Unit* caster = GetCaster())
            target->CastSpell(caster, SPELL_WARRIOR_VIGILANCE_REDIRECT_THREAT, true);
    }

    void HandleAfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        //! WORKAROUND
        //! this glyph is a proc
        if (Unit* caster = GetCaster())
        {
            if (AuraEffect const* glyph = caster->GetAuraEffect(SPELL_WARRIOR_GLYPH_OF_VIGILANCE, EFFECT_0))
                GetTarget()->ModifyRedirectThreat(glyph->GetAmount());
        }
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (target->HasAura(SPELL_GEN_DAMAGE_REDUCTION_AURA) &&
            !(target->HasAura(SPELL_PALADIN_BLESSING_OF_SANCTUARY) ||
                target->HasAura(SPELL_PALADIN_GREATER_BLESSING_OF_SANCTUARY) ||
                target->HasAura(SPELL_PRIEST_RENEWED_HOPE)))
        {
            target->RemoveAurasDueToSpell(SPELL_GEN_DAMAGE_REDUCTION_AURA);
        }

        target->ResetRedirectThreat();
    }

    bool CheckProc(ProcEventInfo& /*eventInfo*/)
    {
        _procTarget = GetCaster();
        return _procTarget;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(_procTarget, SPELL_WARRIOR_VIGILANCE_PROC, true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warr_vigilance::HandleApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectApply += AuraEffectApplyFn(spell_warr_vigilance::HandleAfterApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        OnEffectRemove += AuraEffectRemoveFn(spell_warr_vigilance::HandleRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        DoCheckProc += AuraCheckProcFn(spell_warr_vigilance::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_warr_vigilance::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }

private:
    Unit* _procTarget;
};

// 50725 - Vigilance
class spell_warr_vigilance_trigger : public SpellScript
{
    PrepareSpellScript(spell_warr_vigilance_trigger);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARRIOR_TAUNT });
    }

    void HandleScript(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);

        // Remove Taunt cooldown
        if (Player* target = GetHitPlayer())
            target->RemoveSpellCooldown(SPELL_WARRIOR_TAUNT, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_vigilance_trigger::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 47498 - Devastate
class spell_warr_devastate : public SpellScript
{
    PrepareSpellScript(spell_warr_devastate);

    void HandleDamage(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Aura* aureff = GetExplTargetUnit()->GetAura(58567))
        {
            int32 addedDamages = (GetEffectValue() / 5) * aureff->GetStackAmount();
            ApplyPct(addedDamages, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));
            damage += addedDamages;
        }

        GetCaster()->AddAura(SPELL_WARRIOR_SUNDER_ARMOR, GetExplTargetUnit());

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_devastate::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 58387 - Glyph of Sunder Armor
class spell_warr_glyph_of_sunder_armor : public AuraScript
{
    PrepareAuraScript(spell_warr_glyph_of_sunder_armor);

    void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
    {
        if (!spellMod)
        {
            spellMod = new SpellModifier(aurEff->GetBase());
            spellMod->op = SpellModOp(aurEff->GetMiscValue());
            spellMod->type = SPELLMOD_FLAT;
            spellMod->spellId = GetId();
            spellMod->mask = GetSpellInfo()->Effects[aurEff->GetEffIndex()].SpellClassMask;
        }

        spellMod->value = aurEff->GetAmount();
    }

    void Register() override
    {
        DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_warr_glyph_of_sunder_armor::HandleEffectCalcSpellMod, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 47450 - Heroic Strike
class spell_warr_heroic_strike : public SpellScript
{
    PrepareSpellScript(spell_warr_heroic_strike);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_heroic_strike::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_war_damage_heroic_leap : public SpellScript
{
    PrepareSpellScript(spell_war_damage_heroic_leap);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_war_damage_heroic_leap::HandleHit, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 47450 - Heroic Throw
class spell_warr_heroic_throw : public SpellScript
{
    PrepareSpellScript(spell_warr_heroic_throw);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_heroic_throw::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 47486 - Mortal Strike
class spell_warr_mortal_strike : public SpellScript
{
    PrepareSpellScript(spell_warr_mortal_strike);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_mortal_strike::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_ap_to_hit_damage : public SpellScript
{
    PrepareSpellScript(spell_ap_to_hit_damage);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ap_to_hit_damage::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};


// 47520 - Cleave
class spell_warr_cleave : public SpellScript
{
    PrepareSpellScript(spell_warr_cleave);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_cleave::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 57823 - Revenge
class spell_warr_revenge : public SpellScript
{
    PrepareSpellScript(spell_warr_revenge);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_revenge::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 47502 - Thunder Clap
class spell_warr_thunder_clap : public SpellScript
{
    PrepareSpellScript(spell_warr_thunder_clap);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_thunder_clap::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 1680 - Whirlwind
class spell_warr_whirlwind : public SpellScript
{
    PrepareSpellScript(spell_warr_whirlwind);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_whirlwind::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Spell 28845 - Cheat Death
enum CheatDeath
{
    SPELL_CHEAT_DEATH_TRIGGER = 28846
};

class spell_warr_t3_prot_8p_bonus : public AuraScript
{
    PrepareAuraScript(spell_warr_t3_prot_8p_bonus);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActionTarget() && eventInfo.GetActionTarget()->GetHealthPct() <= 20.0f;
    }

    void HandleEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        if (Unit* target = eventInfo.GetActionTarget())
        {
            target->CastSpell(target, SPELL_CHEAT_DEATH_TRIGGER, true);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warr_t3_prot_8p_bonus::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_warr_t3_prot_8p_bonus::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 20230 - Retaliation
class spell_warr_retaliation : public AuraScript
{
    PrepareAuraScript(spell_warr_retaliation);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARRIOR_RETALIATION_DAMAGE });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetActor() || !eventInfo.GetProcTarget())
        {
            return false;
        }

        // check attack comes not from behind and warrior is not stunned
        return GetTarget()->isInFront(eventInfo.GetActor(), M_PI) && !GetTarget()->HasUnitState(UNIT_STATE_STUNNED);
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_WARRIOR_RETALIATION_DAMAGE, true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warr_retaliation::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_warr_retaliation::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 47488 - Shield Slam
class spell_warr_shield_slam : public SpellScript
{
    PrepareSpellScript(spell_warr_shield_slam);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_shield_slam::HandleHit, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 46968 - Shockwave
class spell_warr_shockwave : public SpellScript
{
    PrepareSpellScript(spell_warr_shockwave);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_shockwave::HandleHit, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_berserker_rage : public SpellScript
{
    PrepareSpellScript(spell_berserker_rage);

    void HandleProc()
    {
        if (GetCaster()->HasAura(20500) || GetCaster()->HasAura(20501)) {
            int32 amount = 11.0f + GetCaster()->ToPlayer()->GetMastery();
            GetCaster()->CastCustomSpell(200004, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_berserker_rage::HandleProc);
    }
};

class spell_rampage_proc_rage : public SpellScript
{
    PrepareSpellScript(spell_rampage_proc_rage);

    void HandleProc()
    {
        int32 amount = 11.0f + GetCaster()->ToPlayer()->GetMastery();
        GetCaster()->CastCustomSpell(200004, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_rampage_proc_rage::HandleProc);
    }
};


class spell_healing_deep_wound : public AuraScript
{
    PrepareAuraScript(spell_healing_deep_wound);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        LOG_ERROR("error", "Fueled by violence");
        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
        GetCaster()->CastCustomSpell(80003, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_healing_deep_wound::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_reset_overpower : public AuraScript
{
    PrepareAuraScript(spell_reset_overpower);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 spellId = eventInfo.GetSpellInfo()->Id;

        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        if (player->HasSpellCooldown(spellId))
            return;

        player->AddSpellCooldown(spellId, 0, 5500);
        player->RemoveSpellCooldown(7384, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_reset_overpower::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class spell_reset_shield_slam : public AuraScript
{
    PrepareAuraScript(spell_reset_shield_slam);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        player->RemoveSpellCooldown(47488, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_reset_shield_slam::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_thunderlord : public AuraScript
{
    PrepareAuraScript(spell_thunderlord);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(47437, -1000);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_thunderlord::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_ignore_pain : public SpellScript
{
    PrepareSpellScript(spell_ignore_pain);

    void OnCastSpell()
    {
        int32 ap = GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK);
        int32 value = round(ap * 3.5);
        GetCaster()->CastCustomSpell(80005, SPELLVALUE_BASE_POINT0, value, GetCaster(), true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_ignore_pain::OnCastSpell);
    }
};

class spell_ignore_pain_absorbe : public AuraScript
{
    PrepareAuraScript(spell_ignore_pain_absorbe);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 damage = eventInfo.GetDamageInfo()->GetDamage();
        uint32 maxDamageAbsorbe = aurEff->GetAmount();
        AuraEffect* protEff1 = GetCaster()->GetAuraEffect(80005, EFFECT_1);
        if ((damage + protEff1->GetAmount()) >= maxDamageAbsorbe) {
            GetCaster()->RemoveAura(80005);
            GetCaster()->RemoveAura(80004);
        }
        else {
            if (AuraEffect* protEff = GetCaster()->GetAuraEffect(80005, EFFECT_1))
                protEff->SetAmount(protEff->GetAmount() + damage);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_ignore_pain_absorbe::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_warr_unbridled_fury : public AuraScript
{
    PrepareAuraScript(spell_warr_unbridled_fury);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetCaster())
            return;

        if (Aura* recklessness = GetCaster()->GetAura(1719))
        {
            uint32 duration = recklessness->GetDuration();
            recklessness->SetDuration(duration + 2000);
        }
        else
        {
            GetCaster()->CastSpell(GetCaster(), 1719, TRIGGERED_FULL_MASK);
            GetCaster()->GetAura(1719)->SetDuration(4000);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warr_unbridled_fury::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


void AddSC_warrior_spell_scripts()
{
    RegisterSpellScript(spell_warr_mocking_blow);
    RegisterSpellScript(spell_warr_intervene);
    RegisterSpellScript(spell_warr_improved_spell_reflection);
    RegisterSpellAndAuraScriptPair(spell_warr_improved_spell_reflection_trigger, spell_warr_improved_spell_reflection_trigger_aura);
    RegisterSpellScript(spell_warr_victory_rush);
    RegisterSpellScript(spell_warr_bloodthirst);
    RegisterSpellScript(spell_warr_charge);
    RegisterSpellScript(spell_warr_charge_damage);
    RegisterSpellScript(spell_warr_intercept);
    RegisterSpellScript(spell_warr_concussion_blow);
    RegisterSpellScript(spell_warr_bladestorm);
    RegisterSpellScript(spell_warr_damage_shield);
    RegisterSpellScript(spell_warr_deep_wounds);
    RegisterSpellScript(spell_warr_execute);
    RegisterSpellScript(spell_warr_glyph_of_sunder_armor);
    RegisterSpellScript(spell_warr_intimidating_shout);
    RegisterSpellScript(spell_warr_last_stand);
    RegisterSpellScript(spell_warr_commanding_shout);
    RegisterSpellScript(spell_warr_overpower);
    RegisterSpellScript(spell_warr_rend);
    RegisterSpellScript(spell_warr_rend_direct_damage);
    RegisterSpellScript(spell_warr_retaliation);
    RegisterSpellScript(spell_warr_shattering_throw);
    RegisterSpellScript(spell_warr_sweeping_strikes);
    RegisterSpellScript(spell_warr_vigilance);
    RegisterSpellScript(spell_warr_vigilance_trigger);
    RegisterSpellScript(spell_warr_t3_prot_8p_bonus);
    RegisterSpellScript(spell_warr_heroic_throw);
    RegisterSpellScript(spell_warr_mortal_strike);
    RegisterSpellScript(spell_ap_to_hit_damage);
    RegisterSpellScript(spell_healing_deep_wound);
    RegisterSpellScript(spell_berserker_rage);
    RegisterSpellScript(spell_reset_overpower);
    RegisterSpellScript(spell_thunderlord);
    RegisterSpellScript(spell_ignore_pain);
    RegisterSpellScript(spell_ignore_pain_absorbe);
    RegisterSpellScript(spell_rampage_proc_rage);
    RegisterSpellScript(spell_warr_devastate);
    RegisterSpellScript(spell_war_damage_heroic_leap);
    RegisterSpellScript(spell_warr_unbridled_fury);
}
