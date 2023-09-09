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
    SPELL_WARRIOR_BERSERKER_STANCE = 2458,
    SPELL_WARRIOR_BLOODTHIRST_HEAL = 23880,
    SPELL_WARRIOR_BLOODTHIRST = 23881,
    SPELL_WARRIOR_BLADESTORM = 46924,
    SPELL_WARRIOR_BLADESTORM_ENERGISE = 50623,
    SPELL_WARRIOR_BLADESTORM_WHIRLWIND = 50622,
    SPELL_WARRIOR_CHARGE_ENERGIZE = 34846,
    SPELL_WARRIOR_CHARGE_DAMAGE = 7921,
    SPELL_WARRIOR_INTERCEPT_DAMAGE = 20248,
    SPELL_WARRIOR_DAMAGE_SHIELD_DAMAGE = 59653,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_1 = 12162,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_2 = 12850,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_3 = 12868,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC = 12721,
    SPELL_WARRIOR_EXECUTE_DAMAGE = 20647,
    SPELL_WARRIOR_GLYPH_OF_EXECUTION = 58367,
    SPELL_WARRIOR_GLYPH_OF_VIGILANCE = 63326,
    SPELL_WARRIOR_HEROIC_LEAP_DAMAGE = 57794,
    SPELL_WARRIOR_IGNORE_PAIN = 80004,
    SPELL_WARRIOR_IMPENDING_VICTORY = 84515,
    SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_BUFF = 65156,
    SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_TALENT = 64976,
    SPELL_WARRIOR_LAST_STAND_BUFF = 12976,
    SPELL_WARRIOR_LAST_STAND_HEAL = 12977,
    SPELL_WARRIOR_COMMANDING_SHOUT_TRIGGERED = 47461,
    SPELL_WARRIOR_RAGING_BLOW = 80008,
    SPELL_WARRIOR_RECKLESSNESS = 1719,
    SPELL_WARRIOR_REND = 47465,
    SPELL_WARRIOR_RETALIATION_DAMAGE = 22858,
    SPELL_WARRIOR_SHIELD_CHARGE_DAMAGE = 84517,
    SPELL_WARRIOR_SHIELD_CHARGE_AOE = 84518,
    SPELL_WARRIOR_SKULLSPLITTER_EXTRA_DAMAGE = 84505,
    SPELL_WARRIOR_SLAM = 50783,
    SPELL_WARRIOR_SUNDER_ARMOR = 58567,
    SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_1 = 12723,
    SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_2 = 26654,
    SPELL_WARRIOR_TAUNT = 355,
    SPELL_WARRIOR_TITANS_GRIP = 46917,
    SPELL_WARRIOR_TITANS_GRIP_BUFF = 49152,
    SPELL_WARRIOR_TITANS_GRIP_SHIELD = 50483,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_1 = 46859,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_2 = 46860,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_1 = 64849,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_2 = 64850,
    SPELL_WARRIOR_VIGILANCE_PROC = 50725,
    SPELL_WARRIOR_VIGILANCE_REDIRECT_THREAT = 59665,
    SPELL_WARRIOR_WHIRLWIND_OFF = 44949,
    SPELL_WARRIOR_WHIRLWIND_ENERGIZE = 1684,
    SPELL_WARRIOR_VICTORY_RUSH = 34428,
    SPELL_WARRIOR_RAVAGER_AURA = 84540,
    SPELL_WARRIOR_RAVAGER_RAGE = 84541,
    SPELL_WARRIOR_COLOSSUS_SMASH = 80002,
    SPELL_WARRIOR_WARBREAKER = 84519,

    //Talents
    TALENT_WARRIOR_FUELED_BY_VIOLENCE_HEAL = 80003,
    TALENT_WARRIOR_POWERFUL_BLOODTHIRST = 80035,
    TALENT_WARRIOR_IMPROVED_BERSERKER_RAGE_R1 = 20500,
    TALENT_WARRIOR_IMPROVED_BERSERKER_RAGE_R2 = 20501,

    //Masteries
    MASTERY_WARRIOR_DEEP_WOUNDS_DOT = 200001,
    MASTERY_WARRIOR_UNSHACKLED_FURY = 200003,
    MASTERY_WARRIOR_UNSHACKLED_FURY_BUFF = 200004,
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

//class spell_warr_intervene : public SpellScript
//{
//    PrepareSpellScript(spell_warr_intervene);
//
//    bool Validate(SpellInfo const* /*spellInfo*/) override
//    {
//        return ValidateSpellInfo({ SPELL_WARRIOR_INTERVENE_TRIGGER });
//    }
//
//    void HandleApplyAura(SpellEffIndex /*effIndex*/)
//    {
//        if (Unit* target = GetHitUnit())
//            target->CastSpell((Unit*)nullptr, SPELL_WARRIOR_INTERVENE_TRIGGER, true);
//    }
//
//    void Register() override
//    {
//        OnEffectHitTarget += SpellEffectFn(spell_warr_intervene::HandleApplyAura, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
//    }
//};

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

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(GetEffectValue()));

        caster->CastCustomSpell(caster, SPELL_WARRIOR_LAST_STAND_BUFF, &healthModSpellBasePoints0, nullptr, nullptr, true, nullptr);
        caster->CastCustomSpell(caster, SPELL_WARRIOR_LAST_STAND_HEAL, &healthModSpellBasePoints0, nullptr, nullptr, true, nullptr);
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
                SPELL_WARRIOR_CHARGE_ENERGIZE
            });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        int32 chargeBasePoints0 = GetEffectValue();
        Unit* caster = GetCaster();
        //caster->CastCustomSpell(caster, SPELL_WARRIOR_CHARGE_ENERGIZE, &chargeBasePoints0, nullptr, nullptr, true);

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

// 7921 - Charge Damage
//class spell_warr_charge_damage : public SpellScript
//{
//    PrepareSpellScript(spell_warr_charge_damage);
//
//    void HandleDamage(SpellEffIndex effIndex)
//    {
//        int32 damage = GetEffectValue();
//        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));
//
//        if (Unit* target = GetHitUnit())
//        {
//            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
//            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
//        }
//
//        SetHitDamage(damage);
//    }
//
//    void Register() override
//    {
//        OnEffectHitTarget += SpellEffectFn(spell_warr_charge_damage::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
//    }
//};

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
        return ValidateSpellInfo({ SPELL_WARRIOR_EXECUTE_DAMAGE, SPELL_WARRIOR_GLYPH_OF_EXECUTION });
    }

    void SendMiss(SpellMissInfo missInfo)
    {
        if (missInfo != SPELL_MISS_NONE)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetHitUnit())
                {
                    caster->SendSpellMiss(target, SPELL_WARRIOR_EXECUTE_DAMAGE, missInfo);
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
            int32 spellCost = spellInfo->CalcPowerCost(caster, SpellSchoolMask(spellInfo->SchoolMask));
            int32 maxRage = spellInfo->GetEffect(EFFECT_1).CalcValue(caster) - spellCost;
            int32 rageAmount = caster->GetPower(POWER_RAGE) - spellCost;
            int32 rage = spellCost;

            rage += std::min<int32>(maxRage, rageAmount);

            float amount = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), GetEffectValue());
            int32 minimumRageKept = 0;

            // Sudden Death rage save
            if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_AURA_PROC_TRIGGER_SPELL, SPELLFAMILY_GENERIC, WARRIOR_ICON_ID_SUDDEN_DEATH, EFFECT_0))
                minimumRageKept = aurEff->GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster);

            int32 newRage = std::max<int32>(minimumRageKept, caster->GetPower(POWER_RAGE) - rage);
            caster->SetPower(POWER_RAGE, newRage);

            // Glyph of Execution bonus
            if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_WARRIOR_GLYPH_OF_EXECUTION, EFFECT_0))
                rage += aurEff->GetAmount() * 10;

            amount *= rage / 10;

            caster->CastCustomSpell(SPELL_WARRIOR_EXECUTE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
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

//50622 - 46924 - Bladestorm Damages
class spell_warr_bladestorm : public AuraScript
{
    PrepareAuraScript(spell_warr_bladestorm);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        caster->CastSpell(caster, SPELL_WARRIOR_BLADESTORM_WHIRLWIND, TRIGGERED_FULL_MASK);
        caster->CastSpell(caster, SPELL_WARRIOR_BLADESTORM_ENERGISE, TRIGGERED_FULL_MASK);
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

        if (Aura* aura = GetCaster()->GetAura(TALENT_WARRIOR_POWERFUL_BLOODTHIRST))
            if (GetExplTargetUnit()->HealthBelowPct(35))
                ApplyPct(damage, aura->GetEffect(EFFECT_0)->GetAmount());

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_bloodthirst::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
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

//// -772 - Rend
//class spell_warr_rend : public AuraScript
//{
//    PrepareAuraScript(spell_warr_rend);
//
//    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
//    {
//        if (Unit* caster = GetCaster())
//        {
//            int32 dotDamageRatio = sSpellMgr->AssertSpellInfo(SPELL_WARRIOR_REND)->GetEffect(EFFECT_0).CalcValue();
//
//            // "If used while your target is above 75% health, Rend does 35% more damage."
//            if (GetUnitOwner()->HasAuraState(AURA_STATE_HEALTH_ABOVE_75_PERCENT, GetSpellInfo(), caster))
//                AddPct(dotDamageRatio, GetSpellInfo()->Effects[EFFECT_2].CalcValue(caster));
//
//            float ap = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), dotDamageRatio);
//
//            amount = int32(caster->ApplyEffectModifiers(GetSpellInfo(), aurEff->GetEffIndex(), ap));
//        }
//    }
//
//    void Register() override
//    {
//        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_rend::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
//    }
//};
//
//// 47465 - Rend
//class spell_warr_rend_direct_damage : public SpellScript
//{
//    PrepareSpellScript(spell_warr_rend_direct_damage);
//
//    void HandleDamage(SpellEffIndex effIndex)
//    {
//        int32 damage = GetEffectValue();
//        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));
//
//        if (Unit* target = GetHitUnit())
//        {
//            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
//            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
//        }
//        SetHitDamage(damage);
//    }
//
//    void Register() override
//    {
//        OnEffectHitTarget += SpellEffectFn(spell_warr_rend_direct_damage::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
//    }
//};

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
            int32 addedDamages = (GetEffectValue() / 2) * aureff->GetStackAmount();
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

//class spell_warr_heroic_leap : public SpellScript
//{
//    PrepareSpellScript(spell_warr_heroic_leap);
//
//    void HandleHit()
//    {
//        Unit* caster = GetCaster();
//
//        caster->CastSpell(caster, SPELL_WARRIOR_HEROIC_LEAP_DAMAGE, TRIGGERED_FULL_MASK);
//    }
//
//    void Register() override
//    {
//        AfterHit += SpellHitFn(spell_warr_heroic_leap::HandleHit);
//    }
//};

//class spell_war_damage_heroic_leap : public SpellScript
//{
//    PrepareSpellScript(spell_war_damage_heroic_leap);
//
//    void HandleHit(SpellEffIndex effIndex)
//    {
//        int32 damage = GetEffectValue();
//        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));
//
//        if (Unit* target = GetHitUnit())
//        {
//            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
//            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
//        }
//        SetHitDamage(damage);
//    }
//
//    void Register() override
//    {
//        OnEffectHitTarget += SpellEffectFn(spell_war_damage_heroic_leap::HandleHit, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
//    }
//};

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

        /*if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }*/
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ap_to_hit_damage::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};


// 47520 - Cleave
//class spell_warr_cleave : public SpellScript
//{
//    PrepareSpellScript(spell_warr_cleave);
//
//    void HandleHit(SpellEffIndex effIndex)
//    {
//        int32 damage = GetEffectValue();
//        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));
//
//        if (Unit* target = GetHitUnit())
//        {
//            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
//            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
//        }
//        SetHitDamage(damage);
//    }
//
//    void Register() override
//    {
//        OnEffectHitTarget += SpellEffectFn(spell_warr_cleave::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
//    }
//};

// 57823 - Revenge
//class spell_warr_revenge : public SpellScript
//{
//    PrepareSpellScript(spell_warr_revenge);
//
//    void HandleHit(SpellEffIndex effIndex)
//    {
//        int32 damage = GetEffectValue();
//        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));
//
//        if (Unit* target = GetHitUnit())
//        {
//            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
//            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
//        }
//        SetHitDamage(damage);
//    }
//
//    void Register() override
//    {
//        OnEffectHitTarget += SpellEffectFn(spell_warr_revenge::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
//    }
//};

// 47502 - Thunder Clap
//class spell_warr_thunder_clap : public SpellScript
//{
//    PrepareSpellScript(spell_warr_thunder_clap);
//
//    void HandleHit(SpellEffIndex effIndex)
//    {
//        int32 damage = GetEffectValue();
//        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));
//
//        if (Unit* target = GetHitUnit())
//        {
//            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
//            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
//        }
//        SetHitDamage(damage);
//    }
//
//    void Register() override
//    {
//        OnEffectHitTarget += SpellEffectFn(spell_warr_thunder_clap::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
//    }
//};

// 1680 - Whirlwind
class spell_warr_whirlwind : public SpellScript
{
    PrepareSpellScript(spell_warr_whirlwind);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        int32 baseRage = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(caster);
        int32 ragePerTarget = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(caster);
        int32 targetQte = targets.size();

        if (targetQte == 0)
            return;

        int32 rageIncrease = (ragePerTarget * targetQte) + baseRage;

        caster->CastCustomSpell(SPELL_WARRIOR_WHIRLWIND_ENERGIZE, SPELLVALUE_BASE_POINT0, rageIncrease, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_whirlwind::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
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
//class spell_warr_retaliation : public AuraScript
//{
//    PrepareAuraScript(spell_warr_retaliation);
//
//    bool Validate(SpellInfo const* /*spellInfo*/) override
//    {
//        return ValidateSpellInfo({ SPELL_WARRIOR_RETALIATION_DAMAGE });
//    }
//
//    bool CheckProc(ProcEventInfo& eventInfo)
//    {
//        if (!eventInfo.GetActor() || !eventInfo.GetProcTarget())
//        {
//            return false;
//        }
//
//        // check attack comes not from behind and warrior is not stunned
//        return GetTarget()->isInFront(eventInfo.GetActor(), M_PI) && !GetTarget()->HasUnitState(UNIT_STATE_STUNNED);
//    }
//
//    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
//    {
//        if (!GetCaster() || !GetCaster()->IsAlive())
//            return;
//
//        if (!eventInfo.GetProcTarget() || !eventInfo.GetProcTarget()->IsAlive())
//            return;
//
//        PreventDefaultAction();
//        GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_WARRIOR_RETALIATION_DAMAGE, true, nullptr, aurEff);
//    }
//
//    void Register() override
//    {
//        DoCheckProc += AuraCheckProcFn(spell_warr_retaliation::CheckProc);
//        OnEffectProc += AuraEffectProcFn(spell_warr_retaliation::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
//    }
//};

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

// 18499 - Berserker Rage
class spell_berserker_rage : public SpellScript
{
    PrepareSpellScript(spell_berserker_rage);

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (!caster->HasAura(TALENT_WARRIOR_IMPROVED_BERSERKER_RAGE_R1) && !caster->HasAura(TALENT_WARRIOR_IMPROVED_BERSERKER_RAGE_R2))
            return;

        if (Aura* mastery = caster->GetAura(MASTERY_WARRIOR_UNSHACKLED_FURY))
        {
            int32 amount = mastery->GetEffect(EFFECT_0)->GetAmount() + GetCaster()->ToPlayer()->GetMastery();
            GetCaster()->CastCustomSpell(MASTERY_WARRIOR_UNSHACKLED_FURY_BUFF, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_berserker_rage::HandleProc);
    }
};

// 80000 - Rampage
class spell_rampage_proc_rage : public SpellScript
{
    PrepareSpellScript(spell_rampage_proc_rage);

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (Aura* mastery = caster->GetAura(MASTERY_WARRIOR_UNSHACKLED_FURY))
        {
            int32 amount = mastery->GetEffect(EFFECT_0)->GetAmount() + GetCaster()->ToPlayer()->GetMastery();
            GetCaster()->CastCustomSpell(MASTERY_WARRIOR_UNSHACKLED_FURY_BUFF, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_rampage_proc_rage::HandleProc);
    }
};

// 2687 - Bloodrage
class spell_bloodrage : public SpellScript
{
    PrepareSpellScript(spell_bloodrage);

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (Aura* mastery = caster->GetAura(MASTERY_WARRIOR_UNSHACKLED_FURY))
        {
            int32 amount = mastery->GetEffect(EFFECT_0)->GetAmount() + GetCaster()->ToPlayer()->GetMastery();
            GetCaster()->CastCustomSpell(MASTERY_WARRIOR_UNSHACKLED_FURY_BUFF, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_bloodrage::HandleProc);
    }
};

// 1719 - Recklessness	
class spell_recklessness_enrage : public SpellScript
{
    PrepareSpellScript(spell_recklessness_enrage);

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (Aura* mastery = caster->GetAura(MASTERY_WARRIOR_UNSHACKLED_FURY))
        {
            int32 amount = mastery->GetEffect(EFFECT_0)->GetAmount() + GetCaster()->ToPlayer()->GetMastery();
            GetCaster()->CastCustomSpell(MASTERY_WARRIOR_UNSHACKLED_FURY_BUFF, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_recklessness_enrage::HandleProc);
    }
};

// 12292 - Death Wish	
class spell_death_wish_enrage : public SpellScript
{
    PrepareSpellScript(spell_death_wish_enrage);

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (Aura* mastery = caster->GetAura(MASTERY_WARRIOR_UNSHACKLED_FURY))
        {
            int32 amount = mastery->GetEffect(EFFECT_0)->GetAmount() + GetCaster()->ToPlayer()->GetMastery();
            GetCaster()->CastCustomSpell(MASTERY_WARRIOR_UNSHACKLED_FURY_BUFF, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_death_wish_enrage::HandleProc);
    }
};

class spell_healing_deep_wound : public AuraScript
{
    PrepareAuraScript(spell_healing_deep_wound);

    bool HandleCheck(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetDamageInfo()->GetAttacker();

        if (!caster || !caster->IsAlive())
            return;

        int32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());

        caster->CastCustomSpell(TALENT_WARRIOR_FUELED_BY_VIOLENCE_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_healing_deep_wound::HandleCheck);
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

        if (!player || !player->IsAlive())
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

        if (!player || !player->IsAlive())
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
        Player* target = GetTarget()->ToPlayer();

        if (!target || !target->IsAlive())
            return;

        if (target)
            target->ModifySpellCooldown(47437, -1000);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_thunderlord::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 80004 - Ignore Pain
class spell_warr_ignore_pain : public AuraScript
{
    PrepareAuraScript(spell_warr_ignore_pain);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        absorbedAmount = 0;
    }

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* caster = GetTarget();

        int32 maxShieldAmount = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetAura()->GetEffect(EFFECT_1)->GetAmount());
        int32 shieldPct = GetAura()->GetEffect(EFFECT_2)->GetAmount();

        if (absorbedAmount == maxShieldAmount)
        {
            GetAura()->Remove();
            return;
        }

        int32 damage = CalculatePct(dmgInfo.GetDamage(), shieldPct);
        int32 amount = std::min<int32>(damage, maxShieldAmount - absorbedAmount);

        absorbAmount = amount;
        absorbedAmount += amount;
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warr_ignore_pain::HandleApply, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_ignore_pain::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_warr_ignore_pain::Absorb, EFFECT_0);
    }
private:
    int32 absorbedAmount = 0;
};

//class spell_ignore_pain : public SpellScript
//{
//    PrepareSpellScript(spell_ignore_pain);
//
//    void OnCastSpell()
//    {
//        Unit* caster = GetCaster();
//
//        int32 ap = GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK);
//        int32 ratio = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(caster);
//        int32 value = CalculatePct(ap, ratio);
//
//        GetCaster()->CastCustomSpell(SPELL_WARRIOR_IGNORE_PAIN_LISTENER, SPELLVALUE_BASE_POINT0, value, GetCaster(), true);
//    }
//
//    void Register() override
//    {
//        OnCast += SpellCastFn(spell_ignore_pain::OnCastSpell);
//    }
//};
//
//class spell_ignore_pain_absorbe : public AuraScript
//{
//    PrepareAuraScript(spell_ignore_pain_absorbe);
//
//    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
//    {
//        Unit* caster = GetCaster();
//
//        if (!caster || !caster->IsAlive())
//            return;
//
//        uint32 damage = eventInfo.GetDamageInfo()->GetDamage();
//        uint32 maxDamageAbsorbe = aurEff->GetAmount();
//        AuraEffect* protEff1 = caster->GetAuraEffect(SPELL_WARRIOR_IGNORE_PAIN_LISTENER, EFFECT_1);
//
//        if ((damage + protEff1->GetAmount()) >= maxDamageAbsorbe)
//        {
//            caster->RemoveAura(SPELL_WARRIOR_IGNORE_PAIN_LISTENER);
//            caster->RemoveAura(SPELL_WARRIOR_IGNORE_PAIN);
//        }
//        else if (AuraEffect* protEff = caster->GetAuraEffect(SPELL_WARRIOR_IGNORE_PAIN_LISTENER, EFFECT_1))
//            protEff->SetAmount(protEff->GetAmount() + damage);
//
//    }
//
//    void Register() override
//    {
//        OnEffectProc += AuraEffectProcFn(spell_ignore_pain_absorbe::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
//    }
//};

class spell_warr_unbridled_fury : public AuraScript
{
    PrepareAuraScript(spell_warr_unbridled_fury);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || !caster->IsAlive())
            return;

        if (Aura* recklessness = caster->GetAura(SPELL_WARRIOR_RECKLESSNESS))
        {
            int32 duration = GetAura()->GetEffect(EFFECT_1)->GetAmount();
            duration += recklessness->GetDuration();
            recklessness->SetDuration(duration);
        }
        else
        {
            int32 duration = aurEff->GetAmount();
            caster->AddAura(SPELL_WARRIOR_RECKLESSNESS, caster);
            if (Aura* recklessness = caster->GetAura(SPELL_WARRIOR_RECKLESSNESS))
                recklessness->SetDuration(duration);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warr_unbridled_fury::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 84504 - Skullsplitter
class spell_warr_skullsplitter : public SpellScript
{
    PrepareSpellScript(spell_warr_skullsplitter);

    void HandleApplyAura(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        int32 amount = 0;

        if (!caster || !target)
            return;

        if (Aura* rend = target->GetAura(SPELL_WARRIOR_REND))
        {
            if (rend->GetCaster() == caster)
            {
                int32 remainingTicks = rend->GetEffect(EFFECT_0)->GetRemaningTicks();
                int32 effectAmount = rend->GetEffect(EFFECT_0)->GetAmount();
                amount += remainingTicks * effectAmount;
                rend->Remove();
            }
        }

        if (Aura* deepWounds = target->GetAura(MASTERY_WARRIOR_DEEP_WOUNDS_DOT))
        {
            if (deepWounds->GetCaster() == caster)
            {
                int32 remainingTicks = deepWounds->GetEffect(0)->GetRemaningTicks();
                int32 effectAmount = deepWounds->GetEffect(0)->GetAmount();
                amount += remainingTicks * effectAmount;
                deepWounds->Remove();
            }
        }

        if (amount == 0)
            return;

        caster->CastCustomSpell(SPELL_WARRIOR_SKULLSPLITTER_EXTRA_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_skullsplitter::HandleApplyAura, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 84506 - Berserker's Way
class spell_warr_berserkers_way : public AuraScript
{
    PrepareAuraScript(spell_warr_berserkers_way);

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* caster = GetUnitOwner()->ToPlayer();

        caster->learnSpell(SPELL_WARRIOR_BERSERKER_STANCE);
        caster->learnSpell(SPELL_WARRIOR_RAGING_BLOW);
        caster->learnSpell(SPELL_WARRIOR_TITANS_GRIP);
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* caster = GetUnitOwner()->ToPlayer();

        caster->removeSpell(SPELL_WARRIOR_BERSERKER_STANCE, SPEC_MASK_ALL, false);
        caster->removeSpell(SPELL_WARRIOR_RAGING_BLOW, SPEC_MASK_ALL, false);
        caster->removeSpell(SPELL_WARRIOR_TITANS_GRIP, SPEC_MASK_ALL, false);
        caster->removeSpell(SPELL_WARRIOR_TITANS_GRIP_BUFF, SPEC_MASK_ALL, false);
        caster->removeSpell(SPELL_WARRIOR_TITANS_GRIP_SHIELD, SPEC_MASK_ALL, false);
        caster->SetCanTitanGrip(false);
        caster->AutoUnequipOffhandIfNeed();
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warr_berserkers_way::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_warr_berserkers_way::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 32216 - Victorious
class spell_warr_victorious : public AuraScript
{
    PrepareAuraScript(spell_warr_victorious);

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* caster = GetUnitOwner()->ToPlayer();

        if (!caster->HasSpell(SPELL_WARRIOR_IMPENDING_VICTORY))
            return;

        caster->RemoveSpellCooldown(SPELL_WARRIOR_IMPENDING_VICTORY, true);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warr_victorious::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

// 84516 - Shield Charge
class spell_warr_shield_charge : public SpellScript
{
    PrepareSpellScript(spell_warr_shield_charge);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        caster->CastSpell(target, SPELL_WARRIOR_SHIELD_CHARGE_DAMAGE);
        caster->CastSpell(target, SPELL_WARRIOR_SHIELD_CHARGE_AOE);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warr_shield_charge::HandleDummy, EFFECT_0, SPELL_EFFECT_CHARGE);
    }
};

class spell_warr_ravager : public SpellScript
{
    PrepareSpellScript(spell_warr_ravager);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        Position pos = target->GetPosition();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = GetCaster()->SummonCreature(600611, pos, TEMPSUMMON_TIMED_DESPAWN, 12000, 0, properties);

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
        OnEffectHitTarget += SpellEffectFn(spell_warr_ravager::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

struct npc_pet_ravager : public ScriptedAI
{
    npc_pet_ravager(Creature* creature) : ScriptedAI(creature) { }

    void InitializeAI() override
    {
        _events.Reset();
        _events.ScheduleEvent(1, 0);
        _events.ScheduleEvent(2, 0);
        me->CombatStop(true);
        me->AttackStop();
        me->SetReactState(REACT_PASSIVE);
    }


    void EnterCombat(Unit*) override
    {
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                if (Unit* owner = me->ToTempSummon()->GetSummonerUnit()) {
                    me->CastSpell(me, SPELL_WARRIOR_RAVAGER_AURA, true, nullptr, nullptr, owner->GetGUID());
                }
                break;
            case 2:
                if (Unit* owner = me->ToTempSummon()->GetSummonerUnit()) {
                    owner->CastSpell(owner, SPELL_WARRIOR_RAVAGER_RAGE);
                    _events.ScheduleEvent(2, 2000);
                }
                break;
            default:
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
};

class spell_warr_impending_victory_replacer : public AuraScript
{
    PrepareAuraScript(spell_warr_impending_victory_replacer);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_WARRIOR_VICTORY_RUSH, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_WARRIOR_IMPENDING_VICTORY);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_WARRIOR_IMPENDING_VICTORY, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_WARRIOR_VICTORY_RUSH);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warr_impending_victory_replacer::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_warr_impending_victory_replacer::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_warr_warbreaker_replacer : public AuraScript
{
    PrepareAuraScript(spell_warr_warbreaker_replacer);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_WARRIOR_COLOSSUS_SMASH, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_WARRIOR_WARBREAKER);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_WARRIOR_WARBREAKER, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_WARRIOR_COLOSSUS_SMASH);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warr_warbreaker_replacer::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_warr_warbreaker_replacer::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_warrior_spell_scripts()
{
    RegisterSpellScript(spell_warr_mocking_blow);
    //RegisterSpellScript(spell_warr_intervene);
    RegisterSpellScript(spell_warr_improved_spell_reflection);
    RegisterSpellAndAuraScriptPair(spell_warr_improved_spell_reflection_trigger, spell_warr_improved_spell_reflection_trigger_aura);
    RegisterSpellScript(spell_warr_victory_rush);
    RegisterSpellScript(spell_warr_bloodthirst);
    RegisterSpellScript(spell_warr_charge);
    //RegisterSpellScript(spell_warr_charge_damage);
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
    //RegisterSpellScript(spell_warr_rend);
    //RegisterSpellScript(spell_warr_rend_direct_damage);
    //RegisterSpellScript(spell_warr_retaliation);
    RegisterSpellScript(spell_warr_shattering_throw);
    RegisterSpellScript(spell_warr_sweeping_strikes);
    RegisterSpellScript(spell_warr_vigilance);
    RegisterSpellScript(spell_warr_vigilance_trigger);
    RegisterSpellScript(spell_warr_t3_prot_8p_bonus);
    RegisterSpellScript(spell_warr_heroic_throw);
    RegisterSpellScript(spell_warr_mortal_strike);
    RegisterSpellScript(spell_ap_to_hit_damage);
    RegisterSpellScript(spell_warr_whirlwind);
    RegisterSpellScript(spell_healing_deep_wound);
    RegisterSpellScript(spell_berserker_rage);
    RegisterSpellScript(spell_rampage_proc_rage);
    RegisterSpellScript(spell_bloodrage);
    RegisterSpellScript(spell_recklessness_enrage);
    RegisterSpellScript(spell_death_wish_enrage);
    RegisterSpellScript(spell_reset_overpower);
    RegisterSpellScript(spell_thunderlord);
    RegisterSpellScript(spell_warr_ignore_pain);
    //RegisterSpellScript(spell_ignore_pain_absorbe);
    RegisterSpellScript(spell_warr_devastate);
    //RegisterSpellScript(spell_war_damage_heroic_leap);
    //RegisterSpellScript(spell_warr_heroic_leap);
    RegisterSpellScript(spell_warr_unbridled_fury);
    RegisterSpellScript(spell_warr_skullsplitter);
    RegisterSpellScript(spell_warr_berserkers_way);
    RegisterSpellScript(spell_warr_victorious);
    RegisterSpellScript(spell_warr_shield_charge);
    RegisterSpellScript(spell_warr_impending_victory_replacer);
    RegisterSpellScript(spell_warr_ravager);
    RegisterSpellScript(spell_warr_warbreaker_replacer);
    RegisterCreatureAI(npc_pet_ravager);
}

