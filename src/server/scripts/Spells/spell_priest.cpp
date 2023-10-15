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
  * Scripts for spells with SPELLFAMILY_PRIEST and SPELLFAMILY_GENERIC spells used by priest players.
  * Ordered alphabetically using scriptname.
  * Scriptnames of files in this file should be prefixed with "spell_pri_".
  */

#include "GridNotifiers.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"

enum PriestSpells
{
    SPELL_PRIEST_LIGHTWELL_RENEW = 48085,
    SPELL_PRIEST_LIGHTWELL_CHARGE = 59907,
    SPELL_PRIEST_AUTONEMENT = 81009,
    SPELL_PRIEST_AUTONEMENT_AURA = 81010,
    SPELL_PRIEST_AUTONEMENT_AOE = 81011,
    SPELL_PRIEST_AUTONEMENT_HEAL = 81012,
    SPELL_PRIEST_DEVOURING_PLAGUE = 48300,
    SPELL_PRIEST_DEVOURING_PLAGUE_EXTRA_DAMAGE = 63675,
    SPELL_PRIEST_DEVOURING_PLAGUE_HEAL = 75999,
    SPELL_PRIEST_DIVINE_AEGIS = 47753,
    SPELL_PRIEST_DIVINE_WORD = 81033,
    SPELL_PRIEST_DIVINE_WORD_AURA = 81034,
    SPELL_PRIEST_DIVINE_FAVOR_CHASTISE = 81035,
    SPELL_PRIEST_DIVINE_FAVOR_SANCTIFY = 81036,
    SPELL_PRIEST_DIVINE_FAVOR_SERENITY = 81037,
    SPELL_PRIEST_EMPOWERED_RENEW = 63544,
    SPELL_PRIEST_EMPYREAL_BLAZE_BUFF = 81028,
    SPELL_PRIEST_EMPYREAL_BLAZE_LISTENER = 81039,
    SPELL_PRIEST_FLASH_HEAL = 48071,
    SPELL_PRIEST_GLYPH_OF_CIRCLE_OF_HEALING = 55675,
    SPELL_PRIEST_GLYPH_OF_LIGHTWELL = 55673,
    SPELL_PRIEST_GLYPH_OF_PRAYER_OF_HEALING_HEAL = 56161,
    SPELL_PRIEST_GUARDIAN_SPIRIT_HEAL = 48153,
    SPELL_PRIEST_HOLY_FIRE = 48135,
    SPELL_PRIEST_HOLY_WORD_CHASTISE = 81026,
    SPELL_PRIEST_HOLY_WORD_SALVATION = 81032,
    SPELL_PRIEST_HOLY_WORD_SALVATION_MENDING = 81040,
    SPELL_PRIEST_HOLY_WORD_SANCTIFY = 81029,
    SPELL_PRIEST_HOLY_WORD_SERENITY = 81025,
    SPELL_PRIEST_ITEM_EFFICIENCY = 37595,
    SPELL_PRIEST_LEAP_OF_FAITH = 81003,
    SPELL_PRIEST_LEAP_OF_FAITH_PROC = 81004,
    SPELL_PRIEST_LEAP_OF_FAITH_GRAB = 81005,
    SPELL_PRIEST_LIGHTS_WRATH_BUFF = 81023,
    SPELL_PRIEST_LIGHTS_WRATH_AOE = 81024,
    SPELL_PRIEST_LIGHTWELL_CHARGES = 59907,
    SPELL_PRIEST_MANA_LEECH_PROC = 34650,
    SPELL_PRIEST_MASS_RESURRECTION = 81002,
    SPELL_PRIEST_PENANCE_R1 = 47540,
    SPELL_PRIEST_PENANCE_R1_DAMAGE = 47758,
    SPELL_PRIEST_PENANCE_R1_HEAL = 47757,
    SPELL_PRIEST_POWER_WORD_LIFE = 81006,
    SPELL_PRIEST_POWER_WORD_LIFE_LISTENER = 81007,
    SPELL_PRIEST_POWER_WORD_RADIANCE = 00000,
    SPELL_PRIEST_POWER_WORD_SHIELD = 48066,
    SPELL_PRIEST_PRAYER_OF_HEALING = 48072,
    SPELL_PRIEST_PRAYER_OF_MENDING = 48113,
    SPELL_PRIEST_PRAYER_OF_MENDING_HEAL = 48111,
    SPELL_PRIEST_PURGE_THE_WICKED = 81017,
    SPELL_PRIEST_PURGE_THE_WICKED_AOE = 81018,
    SPELL_PRIEST_RAPTURE = 81019,
    SPELL_PRIEST_REFLECTIVE_SHIELD_TRIGGERED = 33619,
    SPELL_PRIEST_REFLECTIVE_SHIELD_R1 = 33201,
    SPELL_PRIEST_RENEW = 48068,
    SPELL_PRIEST_SHADOW_WORD_DEATH = 48158,
    SPELL_PRIEST_SHADOW_WORD_DEATH_SELFDAMAGE = 32409,
    SPELL_PRIEST_SHADOW_WORD_DEATH_AURA = 48189,
    SPELL_PRIEST_SHADOW_WORD_PAIN = 48125,
    SPELL_PRIEST_SHADOWY_APPARITIONS_AOE = 81085,
    SPELL_PRIEST_SHADOWY_APPARITIONS_DAMAGE = 81086,
    SPELL_PRIEST_T9_HEALING_2P = 67201,
    SPELL_PRIEST_VAMPIRIC_TOUCH = 48160,
    SPELL_PRIEST_VAMPIRIC_TOUCH_DISPEL = 64085,
    SPELL_PRIEST_VAMPIRIC_TOUCH_HEAL = 81000,
    SPELL_PRIEST_VOID_BOLT = 81045,
    SPELL_PRIEST_VOID_ERUPTION = 81042,
    SPELL_PRIEST_VOID_ERUPTION_DAMAGE = 81043,
    SPELL_PRIEST_VOIDFORM = 81044,

    SPELL_GENERIC_ARENA_DAMPENING = 74410,
    SPELL_GENERIC_BATTLEGROUND_DAMPENING = 74411,
    SPELL_PRIEST_TWIN_DISCIPLINE_R1 = 47586,
    SPELL_PRIEST_SPIRITUAL_HEALING_R1 = 14898,
    SPELL_PRIEST_DIVINE_PROVIDENCE_R1 = 47562,

};

enum PriestSpellIcons
{
    PRIEST_ICON_ID_BORROWED_TIME = 2899,
    PRIEST_ICON_ID_EMPOWERED_RENEW_TALENT = 3021,
    PRIEST_ICON_ID_PAIN_AND_SUFFERING = 2874,
};

enum Mics
{
    PRIEST_LIGHTWELL_NPC_1 = 31897,
    PRIEST_LIGHTWELL_NPC_2 = 31896,
    PRIEST_LIGHTWELL_NPC_3 = 31895,
    PRIEST_LIGHTWELL_NPC_4 = 31894,
    PRIEST_LIGHTWELL_NPC_5 = 31893,
    PRIEST_LIGHTWELL_NPC_6 = 31883
};

class spell_pri_shadowfiend_scaling : public AuraScript
{
    PrepareAuraScript(spell_pri_shadowfiend_scaling);

    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: shadowfiend inherits 40% of resistance from owner and 35% of armor (guessed)
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            SpellSchoolMask schoolMask = SpellSchoolMask(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            int32 modifier = schoolMask == SPELL_SCHOOL_MASK_NORMAL ? 35 : 40;
            amount = CalculatePct(std::max<int32>(0, owner->GetResistance(schoolMask)), modifier);
        }
    }

    void CalculateStatAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: shadowfiend inherits 30% of intellect and 65% of stamina (guessed)
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            Stats stat = Stats(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            amount = CalculatePct(std::max<int32>(0, owner->GetStat(stat)), stat == STAT_STAMINA ? 65 : 30);
        }
    }

    void CalculateAPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: shadowfiend inherits 333% of SP as AP - 35.7% of damage increase per hit
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
            amount = CalculatePct(std::max<int32>(0, shadow), 300); // xinef: deacrased to 300, including 15% from self buff
        }
    }

    void CalculateSPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: shadowfiend inherits 30% of SP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
            amount = CalculatePct(std::max<int32>(0, shadow), 30);

            // xinef: Update appropriate player field
            if (owner->GetTypeId() == TYPEID_PLAYER)
                owner->SetUInt32Value(PLAYER_PET_SPELL_POWER, (uint32)amount);
        }
    }

    void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, aurEff->GetAuraType(), true, SPELL_BLOCK_TYPE_POSITIVE);
        if (aurEff->GetAuraType() == SPELL_AURA_MOD_ATTACK_POWER)
            GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ATTACK_POWER_PCT, true, SPELL_BLOCK_TYPE_POSITIVE);
        else if (aurEff->GetAuraType() == SPELL_AURA_MOD_STAT)
            GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, true, SPELL_BLOCK_TYPE_POSITIVE);
    }

    void Register() override
    {
        if (m_scriptSpellId != 35661)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_shadowfiend_scaling::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);

        if (m_scriptSpellId == 35661 || m_scriptSpellId == 35662)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_shadowfiend_scaling::CalculateStatAmount, EFFECT_ALL, SPELL_AURA_MOD_STAT);

        if (m_scriptSpellId == 35661)
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_shadowfiend_scaling::CalculateAPAmount, EFFECT_ALL, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_shadowfiend_scaling::CalculateSPAmount, EFFECT_ALL, SPELL_AURA_MOD_DAMAGE_DONE);
        }

        OnEffectApply += AuraEffectApplyFn(spell_pri_shadowfiend_scaling::HandleEffectApply, EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

// -34861 - Circle of Healing
class spell_pri_circle_of_healing : public SpellScript
{
    PrepareSpellScript(spell_pri_circle_of_healing);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_GLYPH_OF_CIRCLE_OF_HEALING });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::RaidCheck(GetCaster(), false));

        uint32 const maxTargets = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(GetCaster());

        if (targets.size() > maxTargets)
        {
            targets.sort(Acore::HealthPctOrderPred());
            targets.resize(maxTargets);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_circle_of_healing::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// -47509 - Divine Aegis
class spell_pri_divine_aegis : public AuraScript
{
    PrepareAuraScript(spell_pri_divine_aegis);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_DIVINE_AEGIS });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetProcTarget();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        int32 absorb = CalculatePct(int32(eventInfo.GetHealInfo()->GetHeal()), aurEff->GetAmount());

        // Multiple effects stack, so let's try to find this aura.
        if (AuraEffect const* aegis = eventInfo.GetProcTarget()->GetAuraEffect(SPELL_PRIEST_DIVINE_AEGIS, EFFECT_0))
            absorb += aegis->GetAmount();

        absorb = std::min(absorb, eventInfo.GetProcTarget()->getLevel() * 125);

        GetTarget()->CastCustomSpell(SPELL_PRIEST_DIVINE_AEGIS, SPELLVALUE_BASE_POINT0, absorb, eventInfo.GetProcTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_pri_divine_aegis::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_pri_divine_aegis::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 64844 - Divine Hymn
class spell_pri_divine_hymn : public SpellScript
{
    PrepareSpellScript(spell_pri_divine_hymn);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::RaidCheck(GetCaster(), false));
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_divine_hymn::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ALLY);
    }
};

// 55680 - Glyph of Prayer of Healing
class spell_pri_glyph_of_prayer_of_healing : public AuraScript
{
    PrepareAuraScript(spell_pri_glyph_of_prayer_of_healing);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_GLYPH_OF_PRAYER_OF_HEALING_HEAL });
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        HealInfo* healInfo = eventInfo.GetHealInfo();
        if (!healInfo || !healInfo->GetHeal())
        {
            return;
        }

        SpellInfo const* triggeredSpellInfo = sSpellMgr->AssertSpellInfo(SPELL_PRIEST_GLYPH_OF_PRAYER_OF_HEALING_HEAL);
        int32 heal = int32(CalculatePct(int32(healInfo->GetHeal()), aurEff->GetAmount()) / triggeredSpellInfo->GetMaxTicks());
        GetTarget()->CastCustomSpell(SPELL_PRIEST_GLYPH_OF_PRAYER_OF_HEALING_HEAL, SPELLVALUE_BASE_POINT0, heal, eventInfo.GetProcTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_glyph_of_prayer_of_healing::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 47788 - Guardian Spirit
class spell_pri_guardian_spirit : public AuraScript
{
    PrepareAuraScript(spell_pri_guardian_spirit);

    uint32 healPct;

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_GUARDIAN_SPIRIT_HEAL });
    }

    bool Load() override
    {
        healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue();
        return true;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* target = GetTarget();
        if (dmgInfo.GetDamage() < target->GetHealth())
            return;

        int32 healAmount = int32(target->CountPctFromMaxHealth(healPct));
        // remove the aura now, we don't want 40% healing bonus
        Remove(AURA_REMOVE_BY_ENEMY_SPELL);
        target->CastCustomSpell(target, SPELL_PRIEST_GUARDIAN_SPIRIT_HEAL, &healAmount, nullptr, nullptr, true);
        absorbAmount = dmgInfo.GetDamage();
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_guardian_spirit::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_pri_guardian_spirit::Absorb, EFFECT_1);
    }
};

// 64904 - Hymn of Hope
class spell_pri_hymn_of_hope : public SpellScript
{
    PrepareSpellScript(spell_pri_hymn_of_hope);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::PowerCheck(POWER_MANA, false));
        targets.remove_if(Acore::RaidCheck(GetCaster(), false));

        uint32 const maxTargets = 3;

        if (targets.size() > maxTargets)
        {
            targets.sort(Acore::PowerPctOrderPred(POWER_MANA));
            targets.resize(maxTargets);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_hymn_of_hope::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ALLY);
    }
};

// 37594 - Greater Heal Refund
class spell_pri_item_greater_heal_refund : public AuraScript
{
    PrepareAuraScript(spell_pri_item_greater_heal_refund);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_ITEM_EFFICIENCY });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (HealInfo* healInfo = eventInfo.GetHealInfo())
            if (Unit* healTarget = healInfo->GetTarget())
                if (eventInfo.GetHitMask() & PROC_EX_NO_OVERHEAL && healTarget->IsFullHealth())
                    return true;
        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(GetTarget(), SPELL_PRIEST_ITEM_EFFICIENCY, true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_pri_item_greater_heal_refund::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_pri_item_greater_heal_refund::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};


class spell_pri_lightwell : public SpellScript
{
    PrepareSpellScript(spell_pri_lightwell);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::ObjectGUIDCheck(GetCaster()->GetGUID(), true));
        targets.remove_if([&](WorldObject* target) -> bool
            {
                return !target->ToUnit() || target->ToUnit()->HealthAbovePct(50) || target->ToUnit()->HasAura(SPELL_PRIEST_LIGHTWELL_RENEW);
            });

        if (!targets.empty())
        {
            Acore::Containers::RandomResize(targets, 1);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_lightwell::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
    }
};

class spell_pri_lightwell_renew : public AuraScript
{
    PrepareAuraScript(spell_pri_lightwell_renew);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        Unit* target = GetUnitOwner();

        if (Aura* aura = caster->GetAura(SPELL_PRIEST_LIGHTWELL_CHARGE)) {
            if (aura->GetCharges() <= 0)
                aura->Remove();
            else
                aura->SetCharges(aura->GetCharges() - 1);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_pri_lightwell_renew::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};


// 8129 - Mana Burn
class spell_pri_mana_burn : public SpellScript
{
    PrepareSpellScript(spell_pri_mana_burn);

    void HandleAfterHit()
    {
        if (Unit* unitTarget = GetHitUnit())
            unitTarget->RemoveAurasWithMechanic((1 << MECHANIC_FEAR) | (1 << MECHANIC_POLYMORPH));
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_pri_mana_burn::HandleAfterHit);
    }
};

// 28305 - Mana Leech (Passive) (Priest Pet Aura)
class spell_pri_mana_leech : public AuraScript
{
    PrepareAuraScript(spell_pri_mana_leech);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_MANA_LEECH_PROC });
    }

    bool Load() override
    {
        _procTarget = nullptr;
        return true;
    }

    bool CheckProc(ProcEventInfo& /*eventInfo*/)
    {
        _procTarget = GetTarget()->GetOwner();
        return _procTarget;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(_procTarget, SPELL_PRIEST_MANA_LEECH_PROC, true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_pri_mana_leech::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_pri_mana_leech::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }

private:
    Unit* _procTarget;
};

// -49821 - Mind Sear
class spell_pri_mind_sear : public SpellScript
{
    PrepareSpellScript(spell_pri_mind_sear);

    void FilterTargets(std::list<WorldObject*>& unitList)
    {
        unitList.remove_if(Acore::ObjectGUIDCheck(GetCaster()->GetGuidValue(UNIT_FIELD_CHANNEL_OBJECT), true));
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_mind_sear::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// 47948 - Pain and Suffering (Proc)
class spell_pri_pain_and_suffering_proc : public SpellScript
{
    PrepareSpellScript(spell_pri_pain_and_suffering_proc);

    void HandleEffectScriptEffect(SpellEffIndex /*effIndex*/)
    {
        // Refresh Shadow Word: Pain and Vampiric Touch on target
        if (Unit* unitTarget = GetHitUnit())
        {
            if (AuraEffect* aur = unitTarget->GetAuraEffect(SPELL_PRIEST_SHADOW_WORD_PAIN, EFFECT_0))
            {
                aur->GetBase()->RefreshTimersWithMods();
                aur->ChangeAmount(aur->CalculateAmount(aur->GetCaster()), false);
            }
            if (AuraEffect* aur = unitTarget->GetAuraEffect(SPELL_PRIEST_VAMPIRIC_TOUCH, EFFECT_0))
            {
                aur->GetBase()->RefreshTimersWithMods();
                aur->ChangeAmount(aur->CalculateAmount(aur->GetCaster()), false);
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_pain_and_suffering_proc::HandleEffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// -47540 - Penance
class spell_pri_penance : public SpellScript
{
    PrepareSpellScript(spell_pri_penance);

    bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    bool Validate(SpellInfo const* spellInfo) override
    {
        SpellInfo const* firstRankSpellInfo = sSpellMgr->GetSpellInfo(SPELL_PRIEST_PENANCE_R1);
        if (!firstRankSpellInfo)
            return false;

        // can't use other spell than this penance due to spell_ranks dependency
        if (!spellInfo->IsRankOf(firstRankSpellInfo))
            return false;

        uint8 rank = spellInfo->GetRank();
        if (!sSpellMgr->GetSpellWithRank(SPELL_PRIEST_PENANCE_R1_DAMAGE, rank, true))
            return false;
        if (!sSpellMgr->GetSpellWithRank(SPELL_PRIEST_PENANCE_R1_HEAL, rank, true))
            return false;

        return true;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (Unit* unitTarget = GetHitUnit())
        {
            if (!unitTarget->IsAlive())
                return;

            uint8 rank = GetSpellInfo()->GetRank();

            if (caster->IsFriendlyTo(unitTarget))
                caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(SPELL_PRIEST_PENANCE_R1_HEAL, rank), false);
            else
            {
                caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(SPELL_PRIEST_PENANCE_R1_DAMAGE, rank), false);

                if (unitTarget->HasAura(SPELL_PRIEST_PURGE_THE_WICKED))
                    caster->CastSpell(unitTarget, SPELL_PRIEST_PURGE_THE_WICKED_AOE, TRIGGERED_FULL_MASK);
            }

        }
    }

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        if (Unit* target = GetExplTargetUnit())
        {
            if (!caster->IsFriendlyTo(target))
            {
                if (!caster->IsValidAttackTarget(target))
                    return SPELL_FAILED_BAD_TARGETS;

                if (!caster->isInFront(target))
                    return SPELL_FAILED_UNIT_NOT_INFRONT;
            }
        }
        else
            return SPELL_FAILED_BAD_TARGETS;
        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_penance::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        OnCheckCast += SpellCheckCastFn(spell_pri_penance::CheckCast);
    }
};

// -47540 - Penance Purge the Wicked
class spell_pri_penance_purge : public SpellScript
{
    PrepareSpellScript(spell_pri_penance_purge);

    bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* unitTarget = GetHitUnit();

        if (!unitTarget->IsAlive())
            return;

        if (unitTarget->HasAura(SPELL_PRIEST_PURGE_THE_WICKED))
            caster->CastSpell(unitTarget, SPELL_PRIEST_PURGE_THE_WICKED_AOE, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_penance_purge::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 48066 - Power Word: Shield
static int32 CalculateSpellAmount(Unit* caster, float ratio, int32 amount, SpellInfo const* spellInfo, const AuraEffect* aurEff)
{
    // ratio from sp bonus
    float bonus = ratio / 100;

    // Borrowed Time
    if (AuraEffect const* borrowedTime = caster->GetDummyAuraEffect(SPELLFAMILY_PRIEST, PRIEST_ICON_ID_BORROWED_TIME, EFFECT_1))
        bonus += CalculatePct(1.0f, borrowedTime->GetAmount());

    bonus *= caster->SpellBaseHealingBonusDone(spellInfo->GetSchoolMask());

    // Improved PW: Shield: its weird having a SPELLMOD_ALL_EFFECTS here but its blizzards doing :)
    // Improved PW: Shield is only applied at the spell healing bonus because it was already applied to the base value in CalculateSpellDamage
    bonus = caster->ApplyEffectModifiers(spellInfo, aurEff->GetEffIndex(), bonus);
    bonus *= caster->CalculateLevelPenalty(spellInfo);

    amount += int32(bonus);

    // Rapture
    if (AuraEffect const* rapture = caster->GetAuraEffect(SPELL_PRIEST_RAPTURE, EFFECT_2))
        AddPct(amount, rapture->GetAmount());

    // Twin Disciplines
    if (AuraEffect const* twinDisciplines = caster->GetAuraEffect(SPELL_AURA_ADD_PCT_MODIFIER, SPELLFAMILY_PRIEST, 0x400000, 0, 0, caster->GetGUID()))
        AddPct(amount, twinDisciplines->GetAmount());

    // Focused Power, xinef: apply positive modifier only
    if (int32 healModifier = caster->GetMaxPositiveAuraModifier(SPELL_AURA_MOD_HEALING_DONE_PERCENT))
        AddPct(amount, healModifier);

    // Arena - Dampening
    if (AuraEffect const* arenaDampening = caster->GetAuraEffect(SPELL_GENERIC_ARENA_DAMPENING, EFFECT_0))
    {
        AddPct(amount, arenaDampening->GetAmount());
    }
    // Battleground - Dampening
    else if (AuraEffect const* bgDampening = caster->GetAuraEffect(SPELL_GENERIC_BATTLEGROUND_DAMPENING, EFFECT_0))
    {
        AddPct(amount, bgDampening->GetAmount());
    }

    return amount;
};

class spell_pri_power_word_shield_aura : public AuraScript
{
    PrepareAuraScript(spell_pri_power_word_shield_aura);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_REFLECTIVE_SHIELD_TRIGGERED, SPELL_PRIEST_REFLECTIVE_SHIELD_R1 });
    }

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = false;
        float ratio = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(GetCaster());
        if (Unit* caster = GetCaster())
            amount = CalculateSpellAmount(caster, ratio, amount, GetSpellInfo(), aurEff);
    }

    void ReflectDamage(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* target = GetTarget();
        if (dmgInfo.GetAttacker() == target)
            return;

        if (Unit* owner = GetUnitOwner())
            if (AuraEffect* talentAurEff = owner->GetAuraEffectOfRankedSpell(SPELL_PRIEST_REFLECTIVE_SHIELD_R1, EFFECT_0))
            {
                int32 bp = CalculatePct(absorbAmount, talentAurEff->GetAmount());
                // xinef: prevents infinite loop!
                if (!dmgInfo.GetSpellInfo() || dmgInfo.GetSpellInfo()->Id != SPELL_PRIEST_REFLECTIVE_SHIELD_TRIGGERED)
                    target->CastCustomSpell(dmgInfo.GetAttacker(), SPELL_PRIEST_REFLECTIVE_SHIELD_TRIGGERED, &bp, nullptr, nullptr, true, nullptr, aurEff);
            }
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        Unit* target = GetUnitOwner();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_PRIEST_AUTONEMENT))
            return;

        caster->AddAura(SPELL_PRIEST_AUTONEMENT_AURA, target);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_power_word_shield_aura::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        AfterEffectAbsorb += AuraEffectAbsorbFn(spell_pri_power_word_shield_aura::ReflectDamage, EFFECT_0);
        OnEffectApply += AuraEffectApplyFn(spell_pri_power_word_shield_aura::HandleProc, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

class spell_pri_power_word_shield : public SpellScript
{
    PrepareSpellScript(spell_pri_power_word_shield);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();
        if (!target)
            return SPELL_FAILED_BAD_TARGETS;

        if (AuraEffect* aurEff = target->GetAuraEffect(SPELL_AURA_SCHOOL_ABSORB, (SpellFamilyNames)GetSpellInfo()->SpellFamilyName, GetSpellInfo()->SpellIconID, EFFECT_0))
        {
            int32 newAmount = GetSpellInfo()->Effects[EFFECT_0].CalcValue(caster, nullptr, nullptr);
            float ratio = GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster, nullptr, nullptr);
            newAmount = CalculateSpellAmount(caster, ratio, newAmount, GetSpellInfo(), aurEff);

            if (aurEff->GetAmount() > newAmount)
                return SPELL_FAILED_AURA_BOUNCED;
        }

        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_pri_power_word_shield::CheckCast);
    }
};

//// 48066 - Power Word: Shield
//class spell_pri_power_word_shield_atonement : public AuraScript
//{
//    PrepareAuraScript(spell_pri_power_word_shield_atonement);
//
//    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
//    {
//        Unit* caster = GetCaster();
//        Unit* target = GetUnitOwner();
//
//        if (!caster || caster->isDead())
//            return;
//        if (!caster->HasAura(SPELL_PRIEST_AUTONEMENT))
//            return;
//        
//        caster->AddAura(SPELL_PRIEST_AUTONEMENT_AURA, target);
//    }
//
//    void Register() override
//    {
//        OnEffectApply += AuraEffectApplyFn(spell_pri_power_word_shield_atonement::HandleProc, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
//    }
//};

// 33110 - Prayer of Mending Heal
class spell_pri_prayer_of_mending_heal : public SpellScript
{
    PrepareSpellScript(spell_pri_prayer_of_mending_heal);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_PRIEST_T9_HEALING_2P,
                SPELL_PRIEST_TWIN_DISCIPLINE_R1,
                SPELL_PRIEST_SPIRITUAL_HEALING_R1,
                SPELL_PRIEST_DIVINE_PROVIDENCE_R1
            });
    }

    void HandleHeal(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetOriginalCaster())
        {
            int32 heal = GetEffectValue();
            if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_PRIEST_T9_HEALING_2P, EFFECT_0))
            {
                AddPct(heal, aurEff->GetAmount());
            }
            if (AuraEffect* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_PRIEST_TWIN_DISCIPLINE_R1, EFFECT_0))
            {
                AddPct(heal, aurEff->GetAmount());
            }
            if (AuraEffect* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_PRIEST_SPIRITUAL_HEALING_R1, EFFECT_0))
            {
                AddPct(heal, aurEff->GetAmount());
            }
            if (AuraEffect* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_PRIEST_DIVINE_PROVIDENCE_R1, EFFECT_0))
            {
                AddPct(heal, aurEff->GetAmount());
            }
            SetEffectValue(heal);
        }
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_pri_prayer_of_mending_heal::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
    }
};


// -32379 - Shadow Word: Death
class spell_pri_shadow_word_death : public SpellScript
{
    PrepareSpellScript(spell_pri_shadow_word_death);

    void HandleDamage(SpellEffIndex effIndex)
    {
        Unit* target = GetHitUnit();
        int32 damage = GetEffectValue();

        ApplyPct(damage, GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW));

        if (!target)
            return;

        damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
        damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

        int32 targetHealthPct = target->GetHealthPct();

        if (target->HealthBelowPct(20))
            damage *= GetSpellInfo()->GetEffect(EFFECT_1).BonusMultiplier;
        else if (target->HealthBelowPct(50))
            damage *= GetSpellInfo()->GetEffect(EFFECT_1).DamageMultiplier;

        SetHitDamage(damage);

        GetCaster()->CastCustomSpell(SPELL_PRIEST_SHADOW_WORD_DEATH_AURA, SPELLVALUE_BASE_POINT0, damage, target, TRIGGERED_FULL_MASK);
    }

    /*void HandleReflect()
    {
        Unit* target = GetHitUnit();
        int32 damage = GetHitDamage();

        if (!target)
            return;

        if (target->IsAlive())
            GetCaster()->CastCustomSpell(SPELL_PRIEST_SHADOW_WORD_DEATH_SELFDAMAGE, SPELLVALUE_BASE_POINT0, damage, GetCaster(), TRIGGERED_FULL_MASK);
    }*/

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_shadow_word_death::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        // AfterHit += SpellHitFn(spell_pri_shadow_word_death::HandleReflect);
    }
};

// 48189 - Shadow Word: Death 
class spell_pri_shadow_word_death_after_damage : public AuraScript
{
    PrepareAuraScript(spell_pri_shadow_word_death_after_damage);

    Aura* GetDeathTapAura()
    {
        for (size_t i = 81076; i < 81079; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetProcTarget();

        if (!target || !caster)
            return;

        if (GetDeathTapAura())
        {
            int32 rand = urand(1, 100);

            if (rand > GetDeathTapAura()->GetEffect(EFFECT_0)->GetAmount())
                return;

            caster->ToPlayer()->RemoveSpellCooldown(SPELL_PRIEST_SHADOW_WORD_DEATH, true);
        }

        GetAura()->GetEffect(EFFECT_0)->SetAmount(0);
        GetAura()->Remove();
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        int32 amount = aurEff->GetAmount();

        if (amount <= 0)
            return;

        if (GetUnitOwner()->isDead())
            return;

        caster->CastCustomSpell(SPELL_PRIEST_SHADOW_WORD_DEATH_SELFDAMAGE, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_shadow_word_death_after_damage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_pri_shadow_word_death_after_damage::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// -34914 - Vampiric Touch
class spell_pri_vampiric_touch : public AuraScript
{
    PrepareAuraScript(spell_pri_vampiric_touch);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_PRIEST_VAMPIRIC_TOUCH_DISPEL });
    }

    void HandleDispel(DispelInfo* dispelInfo)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = dispelInfo->GetDispeller())
                caster->CastSpell(target, SPELL_PRIEST_VAMPIRIC_TOUCH_DISPEL, TRIGGERED_FULL_MASK);
    }

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        Unit* target = GetOwner()->ToUnit();
        int32 damage = aurEff->GetAmount();
        int32 healPct = GetSpellInfo()->GetEffect(EFFECT_2).CalcValue(caster);

        damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, EFFECT_0);
        damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

        int32 heal = int32(CalculatePct(damage, healPct));

        GetCaster()->CastCustomSpell(SPELL_PRIEST_VAMPIRIC_TOUCH_HEAL, SPELLVALUE_BASE_POINT0, heal, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        AfterDispel += AuraDispelFn(spell_pri_vampiric_touch::HandleDispel);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_vampiric_touch::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// 605 - Mind Control
class spell_pri_mind_control : public AuraScript
{
    PrepareAuraScript(spell_pri_mind_control);

    void HandleApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = GetTarget())
            {
                uint32 duration = static_cast<uint32>(GetDuration());
                caster->SetInCombatWith(target, duration);
                target->SetInCombatWith(caster, duration);
            }
        }
    }

    void HandleRemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = GetTarget())
            {
                caster->SetCombatTimer(0);
                target->SetCombatTimer(0);
            }
        }
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_pri_mind_control::HandleApplyEffect, EFFECT_0, SPELL_AURA_MOD_POSSESS, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_pri_mind_control::HandleRemoveEffect, EFFECT_0, SPELL_AURA_MOD_POSSESS, AURA_EFFECT_HANDLE_REAL);
    }
};

// 19243 - Desperate Prayer
class spell_pri_desperate_prayer : public AuraScript
{
    PrepareAuraScript(spell_pri_desperate_prayer);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = GetUnitOwner()->CountPctFromMaxHealth(amount);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_desperate_prayer::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_HEALTH);
    }
};

// 48300 - Devouring Plague
class spell_pri_devouring_plague : public SpellScript
{
    PrepareSpellScript(spell_pri_devouring_plague);

    void HandleExtraDamage(SpellMissInfo missInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        if (target->isDead() || !target->HasAura(SPELL_PRIEST_DEVOURING_PLAGUE))
            return;

        Aura* devouring = target->GetAura(SPELL_PRIEST_DEVOURING_PLAGUE);
        AuraEffect* devouringEff = devouring->GetEffect(EFFECT_0);

        int32 healPct = GetSpellInfo()->GetEffect(EFFECT_2).CalcValue(caster);
        int32 remainingTicks = devouring->GetDuration() / devouringEff->GetAmplitude();
        uint32 damage = devouringEff->GetAmount();
        damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), damage, DOT);

        int32 amount = damage * (remainingTicks + 1);
        int32 heal = int32(CalculatePct(amount, healPct));

        caster->CastCustomSpell(target, SPELL_PRIEST_DEVOURING_PLAGUE_EXTRA_DAMAGE, &amount, nullptr, nullptr, true, nullptr, devouringEff);
        caster->CastCustomSpell(caster, SPELL_PRIEST_DEVOURING_PLAGUE_HEAL, &heal, nullptr, nullptr, true, nullptr, devouringEff);
    }

    void HandleHealing()
    {
        Unit* caster = GetCaster();
        int32 damage = GetHitDamage();
        int32 healPct = GetSpellInfo()->GetEffect(EFFECT_2).CalcValue(caster);

        int32 heal = int32(CalculatePct(damage, healPct));

        GetCaster()->CastCustomSpell(SPELL_PRIEST_DEVOURING_PLAGUE_HEAL, SPELLVALUE_BASE_POINT0, heal, caster, TRIGGERED_FULL_MASK);

        if (Aura* voidform = caster->GetAura(SPELL_PRIEST_VOIDFORM))
        {
            int32 durationIncrease = voidform->GetEffect(EFFECT_0)->GetAmplitude();
            voidform->SetDuration(voidform->GetDuration() + durationIncrease);
        }
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_pri_devouring_plague::HandleExtraDamage);
        OnHit += SpellHitFn(spell_pri_devouring_plague::HandleHealing);
    }
};

// 48300 - Devouring Plague
class spell_pri_devouring_plague_heal : public AuraScript
{
    PrepareAuraScript(spell_pri_devouring_plague_heal);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        int32 healPct = GetSpellInfo()->GetEffect(EFFECT_2).CalcValue(caster);
        int32 heal = int32(CalculatePct(aurEff->GetAmount(), healPct));

        GetCaster()->CastCustomSpell(SPELL_PRIEST_DEVOURING_PLAGUE_HEAL, SPELLVALUE_BASE_POINT0, heal, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_devouring_plague_heal::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// 81001 - Mass Resurrection
class spell_pri_mass_resurrection : public SpellScript
{
    PrepareSpellScript(spell_pri_mass_resurrection);

    std::list <Unit*> FindTargets()
    {
        Player* caster = GetCaster()->ToPlayer();
        std::list <Unit*> targetAvailable;
        auto const& allyList = caster->GetGroup()->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* player = ObjectAccessor::FindPlayer(target.guid);
            if (player)
                if (player->isDead())
                {
                    Unit* dummy = player->ToUnit();
                    if (dummy)
                        targetAvailable.push_back(dummy);
                }
        }
        return targetAvailable;
    }

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player->GetGroup())
            return;

        for (auto const& target : FindTargets())
        {
            GetCaster()->CastSpell(target, SPELL_PRIEST_MASS_RESURRECTION, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_pri_mass_resurrection::HandleProc);
    }
};

// 81003 - Leap of Faith
class spell_pri_leap_of_faith : public SpellScript
{
    PrepareSpellScript(spell_pri_leap_of_faith);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();

        //if (target->GetTypeId() != TYPEID_PLAYER)
        //    return SPELL_FAILED_TARGET_NOT_PLAYER;

        if (caster->HasUnitState(UNIT_STATE_JUMPING) || caster->HasUnitMovementFlag(MOVEMENTFLAG_FALLING))
            return SPELL_FAILED_MOVING;

        return SPELL_CAST_OK;
    }

    void HandleBaseDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        Unit* baseTarget = GetExplTargetUnit();
        Creature* targetCreature = GetHitCreature();

        if (caster != target)
        {
            caster->CastSpell(target, SPELL_PRIEST_LEAP_OF_FAITH_PROC, true);
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(1766); // Rogue kick
            if (!target->IsImmunedToSpellEffect(spellInfo, EFFECT_0))
                target->InterruptNonMeleeSpells(true);
        }
        else
            baseTarget->CastSpell(caster, SPELL_PRIEST_LEAP_OF_FAITH_PROC, true);
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        float casterZ = GetCaster()->GetPositionZ(); // for Ring of Valor
        WorldLocation gripPos = *GetExplTargetDest();
        if (Unit* target = GetHitUnit())
            if (!target->HasAuraType(SPELL_AURA_DEFLECT_SPELLS) || target->HasUnitState(UNIT_STATE_STUNNED)) // Deterrence
            {
                if (target != GetCaster())
                {
                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(1766); // Rogue kick
                    if (!target->IsImmunedToSpellEffect(spellInfo, EFFECT_0))
                        target->InterruptNonMeleeSpells(false, 0, false);
                }

                if (target->GetMapId() == 618) // for Ring of Valor
                    gripPos.m_positionZ = std::max(casterZ + 0.2f, 28.5f);

                target->CastSpell(gripPos.GetPositionX(), gripPos.GetPositionY(), gripPos.GetPositionZ(), SPELL_PRIEST_LEAP_OF_FAITH_GRAB, true);
            }
    }

    void Register() override
    {
        if (m_scriptSpellId == SPELL_PRIEST_LEAP_OF_FAITH)
        {
            OnCheckCast += SpellCheckCastFn(spell_pri_leap_of_faith::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_pri_leap_of_faith::HandleBaseDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
        else
            OnEffectHitTarget += SpellEffectFn(spell_pri_leap_of_faith::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 81006 - Power Word: Life
class spell_pri_power_word_life : public SpellScript
{
    PrepareSpellScript(spell_pri_power_word_life);

    void HandleHeal(SpellEffIndex effIndex)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();
        int32 heal = GetEffectValue();

        ApplyPct(heal, GetCaster()->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_HOLY));

        if (!target)
            return;

        heal = caster->SpellHealingBonusDone(target, GetSpellInfo(), uint32(heal), SPELL_DIRECT_DAMAGE, effIndex);
        heal = target->SpellHealingBonusTaken(caster, GetSpellInfo(), uint32(heal), SPELL_DIRECT_DAMAGE);

        int32 healthThreshold = GetSpellInfo()->GetEffect(EFFECT_2).CalcValue(caster);
        int32 healIncrease = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(caster);

        if (target->HealthBelowPct(healthThreshold))
        {
            ApplyPct(heal, healIncrease);
            caster->AddAura(SPELL_PRIEST_POWER_WORD_LIFE_LISTENER, caster);
        }

        SetHitHeal(heal);
    }

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (!caster->HasAura(SPELL_PRIEST_POWER_WORD_LIFE_LISTENER))
            return;

        caster->ToPlayer()->ModifySpellCooldown(SPELL_PRIEST_POWER_WORD_LIFE, -GetSpellInfo()->GetEffect(EFFECT_1).Amplitude);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_power_word_life::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        AfterHit += SpellHitFn(spell_pri_power_word_life::HandleProc);
    }
};

// 81008 - Void Shift
class spell_pri_void_shift : public SpellScript
{
    PrepareSpellScript(spell_pri_void_shift);

    void HandleProc(SpellEffIndex effIndex)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();

        int32 minPct = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(caster);
        int32 targetHealthPct = std::max<int32>(target->GetHealthPct(), minPct);
        int32 casterHealthPct = std::max<int32>(caster->GetHealthPct(), minPct);

        caster->SetHealth(CalculatePct(targetHealthPct, caster->GetMaxHealth()));
        target->SetHealth(CalculatePct(casterHealthPct, target->GetMaxHealth()));
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_void_shift::HandleProc, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 81009 - Atonement
class spell_pri_atonement : public AuraScript
{
    PrepareAuraScript(spell_pri_atonement);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetProcTarget();

        if (!caster || caster->isDead())
            return;

        if (!target || target->isDead())
            return;

        if (!eventInfo.GetSpellInfo())
            return;

        if (eventInfo.GetSpellInfo()->Id == SPELL_PRIEST_FLASH_HEAL || eventInfo.GetSpellInfo()->Id == SPELL_PRIEST_RENEW)
        {
            caster->AddAura(SPELL_PRIEST_AUTONEMENT_AURA, target);
            return;
        }

        if (!eventInfo.GetDamageInfo() || eventInfo.GetDamageInfo()->GetDamage() <= 0)
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, aurEff->GetAmount());
        GetAura()->GetEffect(EFFECT_1)->SetAmount(amount);

        caster->CastSpell(caster, SPELL_PRIEST_AUTONEMENT_AOE, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_atonement::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 81011 - Atonement AOE
class spell_pri_atonement_heal : public SpellScript
{
    PrepareSpellScript(spell_pri_atonement_heal);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 amount = caster->GetAura(SPELL_PRIEST_AUTONEMENT)->GetEffect(EFFECT_1)->GetAmount();

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead() || !target->HasAura(SPELL_PRIEST_AUTONEMENT_AURA))
                continue;

            amount = GetCaster()->SpellHealingBonusDone(target, GetSpellInfo(), uint32(amount), SPELL_DIRECT_DAMAGE, amount);
            amount = target->SpellHealingBonusTaken(GetCaster(), GetSpellInfo(), uint32(amount), SPELL_DIRECT_DAMAGE);

            caster->CastCustomSpell(SPELL_PRIEST_AUTONEMENT_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_atonement_heal::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// 81013 - Power Word: Radiance
class spell_pri_power_word_radiance : public SpellScript
{
    PrepareSpellScript(spell_pri_power_word_radiance);

    void HandleProc(SpellEffIndex effIndex)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();

        int32 durationPct = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(caster);

        if (Aura* atonement = target->GetAura(SPELL_PRIEST_AUTONEMENT_AURA))
        {
            int32 duration = CalculatePct(atonement->GetMaxDuration(), durationPct);

            if (atonement->GetDuration() > duration)
                return;

            atonement->SetDuration(duration);
            return;
        }

        caster->AddAura(SPELL_PRIEST_AUTONEMENT_AURA, target);

        Aura* atonement = target->GetAura(SPELL_PRIEST_AUTONEMENT_AURA);
        int32 duration = CalculatePct(atonement->GetMaxDuration(), durationPct);
        atonement->SetDuration(duration);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_power_word_radiance::HandleProc, EFFECT_0, SPELL_EFFECT_HEAL);
    }
};

// 81015 - Power Word: Barrier
class spell_pri_power_word_barrier : public SpellScript
{
    PrepareSpellScript(spell_pri_power_word_barrier);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.clear();
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_power_word_barrier::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// 81018 - Purge the Wicked AOE
class spell_pri_purge_the_wicked : public SpellScript
{
    PrepareSpellScript(spell_pri_purge_the_wicked);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead() || target->HasAura(SPELL_PRIEST_PURGE_THE_WICKED))
                continue;

            if (target->HasAura(SPELL_PRIEST_PURGE_THE_WICKED) && target->GetAura(SPELL_PRIEST_PURGE_THE_WICKED)->GetDuration() > 5000)
                continue;

            caster->CastSpell(target, SPELL_PRIEST_PURGE_THE_WICKED, true);
        }

        caster->AddAura(SPELL_PRIEST_PURGE_THE_WICKED, GetExplTargetUnit());
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_purge_the_wicked::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// 81019 - Rapture
class spell_pri_rapture : public SpellScript
{
    PrepareSpellScript(spell_pri_rapture);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();

        player->RemoveSpellCooldown(SPELL_PRIEST_POWER_WORD_SHIELD, true);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_pri_rapture::HandleProc);
    }
};

// 81021 - Evangelism
class spell_pri_evangelism : public SpellScript
{
    PrepareSpellScript(spell_pri_evangelism);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 durationIncrease = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(caster);

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead())
                continue;

            if (Aura* atonement = target->GetAura(SPELL_PRIEST_AUTONEMENT_AURA))
                atonement->SetDuration(atonement->GetDuration() + durationIncrease);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_evangelism::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// 81022 - Light's Wrath
class spell_pri_light_wrath : public SpellScript
{
    PrepareSpellScript(spell_pri_light_wrath);

    void HandleProc()
    {
        Unit* caster = GetCaster();

        caster->CastSpell(caster, SPELL_PRIEST_LIGHTS_WRATH_AOE, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_pri_light_wrath::HandleProc);
    }
};

// 81024 - Light's Wrath AOE
class spell_pri_light_wrath_check : public SpellScript
{
    PrepareSpellScript(spell_pri_light_wrath_check);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 atonementAmount = 0;

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead())
                continue;

            if (target->HasAura(SPELL_PRIEST_AUTONEMENT_AURA))
                atonementAmount++;
        }

        if (atonementAmount == 0)
            return;

        caster->AddAura(SPELL_PRIEST_LIGHTS_WRATH_BUFF, caster);
        caster->GetAura(SPELL_PRIEST_LIGHTS_WRATH_BUFF)->SetStackAmount(atonementAmount);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_light_wrath_check::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// 81025 - Holy Word: Serenity / 48071 - Flash Heal / 6064 - Heal
class spell_pri_holy_word_serenity_cooldown : public SpellScript
{
    PrepareSpellScript(spell_pri_holy_word_serenity_cooldown);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        int32 cooldownReduction = sSpellMgr->AssertSpellInfo(SPELL_PRIEST_HOLY_WORD_SERENITY)->GetEffect(EFFECT_1).CalcValue(player);

        player->ModifySpellCooldown(SPELL_PRIEST_HOLY_WORD_SERENITY, -cooldownReduction);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_pri_holy_word_serenity_cooldown::HandleProc);
    }
};

// 81026 - Holy Word: Chastise / 48123 - Smite
class spell_pri_holy_word_chastise_cooldown : public SpellScript
{
    PrepareSpellScript(spell_pri_holy_word_chastise_cooldown);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        int32 cooldownReduction = sSpellMgr->AssertSpellInfo(SPELL_PRIEST_HOLY_WORD_CHASTISE)->GetEffect(EFFECT_2).CalcValue(player);

        player->ModifySpellCooldown(SPELL_PRIEST_HOLY_WORD_CHASTISE, -cooldownReduction);

        if (player->HasAura(SPELL_PRIEST_DIVINE_FAVOR_CHASTISE))
            player->ToPlayer()->RemoveSpellCooldown(SPELL_PRIEST_HOLY_FIRE, true);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_pri_holy_word_chastise_cooldown::HandleProc);
    }
};

// 48135 - Holy Fire
class spell_pri_holy_fire : public SpellScript
{
    PrepareSpellScript(spell_pri_holy_fire);

    void HandleProc(SpellMissInfo missInfo)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();

        if (target->HasAura(SPELL_PRIEST_HOLY_FIRE) && caster->HasAura(SPELL_PRIEST_EMPYREAL_BLAZE_BUFF))
        {
            Aura* holyFire = target->GetAura(SPELL_PRIEST_HOLY_FIRE);
            int32 duration = holyFire->GetDuration();

            caster->AddAura(SPELL_PRIEST_EMPYREAL_BLAZE_LISTENER, target);
            target->GetAura(SPELL_PRIEST_EMPYREAL_BLAZE_LISTENER)->GetEffect(EFFECT_0)->SetAmount(duration);
        }
    }

    void HandleAfter()
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();

        if (target->HasAura(SPELL_PRIEST_HOLY_FIRE) && target->HasAura(SPELL_PRIEST_EMPYREAL_BLAZE_LISTENER))
        {
            Aura* holyFire = target->GetAura(SPELL_PRIEST_HOLY_FIRE);
            Aura* listenerAura = target->GetAura(SPELL_PRIEST_EMPYREAL_BLAZE_LISTENER);

            int32 duration = listenerAura->GetEffect(EFFECT_0)->GetAmount() + holyFire->GetMaxDuration();
            listenerAura->Remove();

            holyFire->SetMaxDuration(duration);
            holyFire->RefreshDuration();
        }
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_pri_holy_fire::HandleProc);
        AfterHit += SpellHitFn(spell_pri_holy_fire::HandleAfter);
    }
};

// 81027 - Empyreal Blaze
class spell_pri_empyreal_blaze : public SpellScript
{
    PrepareSpellScript(spell_pri_empyreal_blaze);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();

        player->RemoveSpellCooldown(SPELL_PRIEST_HOLY_FIRE, true);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_pri_empyreal_blaze::HandleProc);
    }
};

// 81025 - Holy Word: Sanctify / 48072 - Prayer of Healing / 48068 - Renew
class spell_pri_holy_word_sanctify_cooldown : public SpellScript
{
    PrepareSpellScript(spell_pri_holy_word_sanctify_cooldown);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        int32 cooldownReduction = 0;

        if (GetSpellInfo()->Id == SPELL_PRIEST_PRAYER_OF_HEALING)
            cooldownReduction = sSpellMgr->AssertSpellInfo(SPELL_PRIEST_HOLY_WORD_SANCTIFY)->GetEffect(EFFECT_1).CalcValue(player);

        if (GetSpellInfo()->Id == SPELL_PRIEST_RENEW)
            cooldownReduction = sSpellMgr->AssertSpellInfo(SPELL_PRIEST_HOLY_WORD_SANCTIFY)->GetEffect(EFFECT_2).CalcValue(player);

        player->ModifySpellCooldown(SPELL_PRIEST_HOLY_WORD_SANCTIFY, -cooldownReduction);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_pri_holy_word_sanctify_cooldown::HandleProc);
    }
};

// 81030 - Apotheosis
class spell_pri_apotheosis : public SpellScript
{
    PrepareSpellScript(spell_pri_apotheosis);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();

        player->RemoveSpellCooldown(SPELL_PRIEST_HOLY_WORD_CHASTISE, true);
        player->RemoveSpellCooldown(SPELL_PRIEST_HOLY_WORD_SANCTIFY, true);
        player->RemoveSpellCooldown(SPELL_PRIEST_HOLY_WORD_SERENITY, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_pri_apotheosis::HandleProc);
    }
};

// 81032 - Holy Word: Salvation
class spell_pri_holy_word_salvation : public SpellScript
{
    PrepareSpellScript(spell_pri_holy_word_salvation);

    void HandleProc(SpellEffIndex effIndex)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();

        caster->CastSpell(target, SPELL_PRIEST_RENEW, true);
        caster->CastCustomSpell(SPELL_PRIEST_PRAYER_OF_MENDING, SPELLVALUE_AURA_CHARGE, 2, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_pri_holy_word_salvation::HandleProc, EFFECT_0, SPELL_EFFECT_HEAL);
    }
};

// 81032 - Holy Word: Salvation / 81025 - Holy Word: Serenity / 81029 - Holy Word: Sanctify / 81026 - Holy Word: Chastise
class spell_pri_holy_word_salvation_cooldown : public SpellScript
{
    PrepareSpellScript(spell_pri_holy_word_salvation_cooldown);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        int32 cooldownReduction = sSpellMgr->AssertSpellInfo(SPELL_PRIEST_HOLY_WORD_SALVATION)->GetEffect(EFFECT_1).CalcValue(player);

        player->ModifySpellCooldown(SPELL_PRIEST_HOLY_WORD_SALVATION, -cooldownReduction);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_pri_holy_word_salvation_cooldown::HandleProc);
    }
};

// 81034 - Divine Word Aura
class spell_pri_divine_word : public AuraScript
{
    PrepareAuraScript(spell_pri_divine_word);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;


        int32 spellID = eventInfo.GetSpellInfo()->Id;

        if (spellID == SPELL_PRIEST_HOLY_WORD_CHASTISE)
            caster->AddAura(SPELL_PRIEST_DIVINE_FAVOR_CHASTISE, caster);

        if (spellID == SPELL_PRIEST_HOLY_WORD_SANCTIFY) {
            if (Unit* selectedUnit = ObjectAccessor::GetUnit(*caster, caster->GetTarget()))
                caster->AddAura(SPELL_PRIEST_DIVINE_FAVOR_SANCTIFY, selectedUnit);
            else
                caster->AddAura(SPELL_PRIEST_DIVINE_FAVOR_SANCTIFY, caster);
        }

        if (spellID == SPELL_PRIEST_HOLY_WORD_SERENITY)
            caster->AddAura(SPELL_PRIEST_DIVINE_FAVOR_SERENITY, caster);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_divine_word::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 81042 - Void Eruption 
class spell_pri_void_eruption : public SpellScript
{
    PrepareSpellScript(spell_pri_void_eruption);

    void HandleProc()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(SPELL_PRIEST_VOIDFORM))
            caster->CastSpell(target, SPELL_PRIEST_VOID_BOLT, TRIGGERED_FULL_MASK);
        else
        {
            caster->AddAura(SPELL_PRIEST_VOIDFORM, caster);
            caster->CastSpell(target, SPELL_PRIEST_VOID_ERUPTION_DAMAGE, TRIGGERED_FULL_MASK);
            caster->CastSpell(target, SPELL_PRIEST_VOID_ERUPTION_DAMAGE, TRIGGERED_FULL_MASK);
            caster->ToPlayer()->ModifySpellCooldown(SPELL_PRIEST_VOID_ERUPTION, -120000);
        }
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_pri_void_eruption::HandleProc);
    }
};

// 81044 - Voidform 
class spell_pri_void_eruption_cooldown : public AuraScript
{
    PrepareAuraScript(spell_pri_void_eruption_cooldown);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* player = GetCaster()->ToPlayer();

        if (player->HasSpellCooldown(SPELL_PRIEST_VOID_ERUPTION))
        {
            int32 cooldownChange = 6000 - player->GetSpellCooldownDelay(SPELL_PRIEST_VOID_ERUPTION);
            player->ModifySpellCooldown(SPELL_PRIEST_VOID_ERUPTION, cooldownChange);
        }
        else
        {
            player->ModifySpellCooldown(SPELL_PRIEST_VOID_ERUPTION, 6000);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* player = GetCaster()->ToPlayer();

        player->ToPlayer()->SendCooldownEvent(sSpellMgr->AssertSpellInfo(SPELL_PRIEST_VOID_ERUPTION));
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_pri_void_eruption_cooldown::HandleProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_pri_void_eruption_cooldown::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
    }
};

// All Insanity Generation EFFECT_1 On Cast
class spell_pri_insanity_on_cast : public SpellScript
{
    PrepareSpellScript(spell_pri_insanity_on_cast);

    void HandleProc()
    {
        Unit* caster = GetCaster();

        int32 insanityAmount = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(caster);

        caster->SetPower(POWER_RUNIC_POWER, caster->GetPower(POWER_RUNIC_POWER) + insanityAmount, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_pri_insanity_on_cast::HandleProc);
    }
};

// All Insanity Generation EFFECT_1 On Periodic
class spell_pri_insanity_on_periodic : public AuraScript
{
    PrepareAuraScript(spell_pri_insanity_on_periodic);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        int32 insanityAmount = aurEff->GetAmount();

        caster->SetPower(POWER_RUNIC_POWER, caster->GetPower(POWER_RUNIC_POWER) + insanityAmount, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_insanity_on_periodic::HandleProc, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 81076 - Death Tap
//class spell_pri_death_tap : public AuraScript
//{
//    PrepareAuraScript(spell_pri_death_tap);
//
//    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
//    {
//        Unit* caster = GetCaster();
//        Unit* target = eventInfo.GetProcTarget();
//
//        if (!target || !caster)
//            return;
//
//        if (Aura* deathAura = target->GetAura(SPELL_PRIEST_SHADOW_WORD_DEATH_AURA))
//        {
//            int32 rand = urand(1, 100);
//
//            if (rand > aurEff->GetAmount())
//                return;
//
//            caster->ToPlayer()->RemoveSpellCooldown(SPELL_PRIEST_SHADOW_WORD_DEATH, true);
//            deathAura->GetEffect(EFFECT_0)->SetAmount(0);
//        }
//    }
//
//    void Register()
//    {
//        OnEffectProc += AuraEffectProcFn(spell_pri_death_tap::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
//    }
//};

// 81083 - Shadowy Apparitions
class spell_pri_shadowy_apparitions : public AuraScript
{
    PrepareAuraScript(spell_pri_shadowy_apparitions);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetProcTarget();

        if (!target || !caster)
            return;

        int32 shadowNbr = aurEff->GetAmount();

        if (eventInfo.GetHitMask() == PROC_EX_CRITICAL_HIT)
            shadowNbr = GetEffect(EFFECT_1)->GetAmount();

        GetEffect(EFFECT_2)->SetAmount(shadowNbr);
        caster->CastSpell(caster, SPELL_PRIEST_SHADOWY_APPARITIONS_AOE, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_shadowy_apparitions::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 81085 - Shadowy Apparitions Aoe
class spell_pri_shadowy_apparitions_aoe : public SpellScript
{
    PrepareSpellScript(spell_pri_shadowy_apparitions_aoe);

    Aura* GetTalentAura()
    {
        for (size_t i = 81083; i < 81085; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!GetTalentAura())
            return;

        int32 shadowNbr = GetTalentAura()->GetEffect(EFFECT_2)->GetAmount();

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead() || !target->HasAura(SPELL_PRIEST_VAMPIRIC_TOUCH))
                continue;

            int32 shadow = shadowNbr;

            while (shadow > 0)
            {
                Position pos = caster->GetNearPosition(urand(1, 5), urand(1, 5));
                SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
                Creature* summon = caster->SummonCreature(900000, pos, TEMPSUMMON_MANUAL_DESPAWN, GetSpellInfo()->GetDuration(), 0, properties);

                if (!summon)
                    return;

                summon->SetOwnerGUID(caster->GetGUID());
                summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                summon->SetReactState(REACT_PASSIVE);
                summon->SetTarget(target->GetGUID());

                shadow--;
            }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_shadowy_apparitions_aoe::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// Shadowy Apparition Pet
class npc_pri_shadowy_apparitions : public CreatureScript
{
public:
    npc_pri_shadowy_apparitions() : CreatureScript("npc_pri_shadowy_apparitions") { }

    struct spell_pri_shadowy_apparitionsAI : public ScriptedAI
    {
        spell_pri_shadowy_apparitionsAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 update = 250;

        void Reset() override
        {
            me->CombatStop(true);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(uint32 diff) override
        {
            if (update >= 250) {
                if (Unit* target = ObjectAccessor::GetCreature(*me, me->GetTarget()))
                {
                    Position pos = target->GetPosition();
                    me->GetMotionMaster()->MovePoint(0, pos);
                }
                update = 0;
            }

            update += diff;
        }

        void MovementInform(uint32 /*type*/, uint32 id) override {
            if (id == 0) {
                if (Unit* target = ObjectAccessor::GetCreature(*me, me->GetTarget()))
                    me->CastSpell(target, SPELL_PRIEST_SHADOWY_APPARITIONS_DAMAGE, TRIGGERED_FULL_MASK, nullptr, nullptr, me->GetOwnerGUID());

                me->DespawnOrUnsummon();
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new spell_pri_shadowy_apparitionsAI(creature);
    }
};

class spell_pri_prayer_of_mending : public AuraScript
{
    PrepareAuraScript(spell_pri_prayer_of_mending);

    Unit* findNearestTarget()
    {
        Unit* caster = GetCaster();
        std::list<Unit*> nearMembers;
        Group* group = caster->ToPlayer()->GetGroup();

        if (!group)
            return nullptr;

        for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
            if (Player* Target = itr->GetSource())
            {
                if (!caster->IsWithinDistInMap(Target, 20.f))
                    continue;

                // IsHostileTo check duel and controlled by enemy
                if (Target->IsAlive() && !caster->IsHostileTo(Target))
                    nearMembers.push_back(Target);
            }

        if (!nearMembers.empty())
            nearMembers.sort(Acore::HealthPctOrderPred());

        return nearMembers.front();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint8 charges = aurEff->GetBase()->GetCharges() - 1;
        PreventDefaultAction();

        Unit* caster = GetCaster();
        Unit* target = GetTarget();


        caster->CastSpell(target, SPELL_PRIEST_PRAYER_OF_MENDING_HEAL);

        Unit* nextTarget = findNearestTarget();

        if (nextTarget && charges > 0) {
            target->RemoveAura(SPELL_PRIEST_PRAYER_OF_MENDING);
            target->CastSpell(nextTarget, 41637 /*Dummy visual effect triggered by main spell cast*/, true);
            target->CastCustomSpell(SPELL_PRIEST_PRAYER_OF_MENDING, SPELLVALUE_AURA_CHARGE, charges, nextTarget,true, nullptr, nullptr, caster->GetGUID());
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pri_prayer_of_mending::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_pri_holy_blossom : public SpellScript
{
    PrepareSpellScript(spell_pri_holy_blossom);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::RaidCheck(GetCaster(), false));

        uint32 const maxTargets = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(GetCaster());

        if (targets.size() > maxTargets)
        {
            targets.sort(Acore::HealthPctOrderPred());
            targets.resize(maxTargets);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_holy_blossom::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

void AddSC_priest_spell_scripts()
{
    RegisterSpellScript(spell_pri_shadowfiend_scaling);
    RegisterSpellScript(spell_pri_circle_of_healing);
    RegisterSpellScript(spell_pri_divine_aegis);
    RegisterSpellScript(spell_pri_divine_hymn);
    RegisterSpellScript(spell_pri_glyph_of_prayer_of_healing);
    RegisterSpellScript(spell_pri_guardian_spirit);
    RegisterSpellScript(spell_pri_hymn_of_hope);
    RegisterSpellScript(spell_pri_item_greater_heal_refund);
    RegisterSpellScript(spell_pri_lightwell);
    RegisterSpellScript(spell_pri_lightwell_renew);
    RegisterSpellScript(spell_pri_mana_burn);
    RegisterSpellScript(spell_pri_mana_leech);
    RegisterSpellScript(spell_pri_mind_sear);
    RegisterSpellScript(spell_pri_pain_and_suffering_proc);
    RegisterSpellScript(spell_pri_penance);
    RegisterSpellScript(spell_pri_penance_purge);
    RegisterSpellAndAuraScriptPair(spell_pri_power_word_shield, spell_pri_power_word_shield_aura);
    RegisterSpellScript(spell_pri_prayer_of_mending_heal);
    RegisterSpellScript(spell_pri_shadow_word_death);
    RegisterSpellScript(spell_pri_shadow_word_death_after_damage);
    RegisterSpellScript(spell_pri_vampiric_touch);
    RegisterSpellScript(spell_pri_mind_control);
    RegisterSpellScript(spell_pri_desperate_prayer);
    RegisterSpellScript(spell_pri_devouring_plague);
    RegisterSpellScript(spell_pri_devouring_plague_heal);
    RegisterSpellScript(spell_pri_mass_resurrection);
    RegisterSpellScript(spell_pri_leap_of_faith);
    RegisterSpellScript(spell_pri_power_word_life);
    RegisterSpellScript(spell_pri_void_shift);
    RegisterSpellScript(spell_pri_atonement);
    RegisterSpellScript(spell_pri_atonement_heal);
    RegisterSpellScript(spell_pri_power_word_radiance);
    RegisterSpellScript(spell_pri_power_word_barrier);
    RegisterSpellScript(spell_pri_purge_the_wicked);
    RegisterSpellScript(spell_pri_rapture);
    RegisterSpellScript(spell_pri_evangelism);
    RegisterSpellScript(spell_pri_light_wrath);
    RegisterSpellScript(spell_pri_light_wrath_check);
    RegisterSpellScript(spell_pri_holy_word_serenity_cooldown);
    RegisterSpellScript(spell_pri_holy_word_chastise_cooldown);
    RegisterSpellScript(spell_pri_holy_fire);
    RegisterSpellScript(spell_pri_empyreal_blaze);
    RegisterSpellScript(spell_pri_holy_word_sanctify_cooldown);
    RegisterSpellScript(spell_pri_apotheosis);
    RegisterSpellScript(spell_pri_holy_word_salvation);
    RegisterSpellScript(spell_pri_holy_word_salvation_cooldown);
    RegisterSpellScript(spell_pri_divine_word);
    RegisterSpellScript(spell_pri_void_eruption);
    RegisterSpellScript(spell_pri_void_eruption_cooldown);
    RegisterSpellScript(spell_pri_insanity_on_cast);
    RegisterSpellScript(spell_pri_insanity_on_periodic);
    //RegisterSpellScript(spell_pri_death_tap);
    RegisterSpellScript(spell_pri_shadowy_apparitions);
    RegisterSpellScript(spell_pri_shadowy_apparitions_aoe);
    RegisterSpellScript(spell_pri_prayer_of_mending);
    RegisterSpellScript(spell_pri_holy_blossom);
    new npc_pri_shadowy_apparitions();
}
