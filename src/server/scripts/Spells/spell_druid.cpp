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
  * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
  * Ordered alphabetically using scriptname.
  * Scriptnames of files in this file should be prefixed with "spell_dru_".
  */

#include "Containers.h"
#include "GridNotifiers.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellScript.h"

enum DruidSpells
{
    SPELL_DRUID_GLYPH_OF_WILD_GROWTH        = 62970,
    SPELL_DRUID_NURTURING_INSTINCT_R1       = 47179,
    SPELL_DRUID_NURTURING_INSTINCT_R2       = 47180,
    SPELL_DRUID_FERAL_SWIFTNESS_R1          = 17002,
    SPELL_DRUID_FERAL_SWIFTNESS_R2          = 24866,
    SPELL_DRUID_FERAL_SWIFTNESS_PASSIVE_1   = 24867,
    SPELL_DRUID_FERAL_SWIFTNESS_PASSIVE_2   = 24864,
    SPELL_DRUID_BARKSKIN                    = 22812,
    SPELL_DRUID_GLYPH_OF_BARKSKIN           = 63057,
    SPELL_DRUID_GLYPH_OF_BARKSKIN_TRIGGER   = 63058,
    SPELL_DRUID_ENRAGE_MOD_DAMAGE           = 51185,
    SPELL_DRUID_GLYPH_OF_TYPHOON            = 62135,
    SPELL_DRUID_IDOL_OF_FERAL_SHADOWS       = 34241,
    SPELL_DRUID_IDOL_OF_WORSHIP             = 60774,
    SPELL_DRUID_INCREASED_MOONFIRE_DURATION = 38414,
    SPELL_DRUID_KING_OF_THE_JUNGLE          = 48492,
    SPELL_DRUID_LIFEBLOOM_ENERGIZE          = 64372,
    SPELL_DRUID_LIFEBLOOM_FINAL_HEAL        = 33778,
    SPELL_DRUID_LIVING_SEED_HEAL            = 48503,
    SPELL_DRUID_LIVING_SEED_PROC            = 48504,
    SPELL_DRUID_NATURES_SPLENDOR            = 57865,
    SPELL_DRUID_SURVIVAL_INSTINCTS          = 50322,
    //SPELL_DRUID_SAVAGE_ROAR                 = 62071,
    SPELL_DRUID_TIGER_S_FURY_ENERGIZE       = 51178,
    SPELL_DRUID_ITEM_T8_BALANCE_RELIC       = 64950,
    SPELL_DRUID_BEAR_FORM_PASSIVE           = 1178,
    SPELL_DRUID_DIRE_BEAR_FORM_PASSIVE      = 9635,
    SPELL_DRUID_ENRAGE                      = 5229,
    SPELL_DRUID_ENRAGED_DEFENSE             = 70725,
    SPELL_DRUID_ITEM_T10_FERAL_4P_BONUS     = 70726,

    SPELL_DRUID_FEROCIOUS_BITE              = 48577,
    SPELL_DRUID_PROWL                       = 5215,
    SPELL_DRUID_CAT_FORM                    = 768,
    SPELL_DRUID_THORNS_SLOW                 = 80500,
    SPELL_DRUID_MOONKIN_FORM                = 24858,
    SPELL_DRUID_WRATH                       = 48461,
    SPELL_DRUID_STARFIRE                    = 48465,
    SPELL_DRUID_STARFIRE_AOE                = 80506,
    SPELL_DRUID_ECLIPSE_BASE                = 80501,
    SPELL_DRUID_ECLIPSE_SOLAR_STACK         = 80503,
    SPELL_DRUID_ECLIPSE_SOLAR_BUFF          = 80502,
    SPELL_DRUID_ECLIPSE_LUNAR_STACK         = 80504,
    SPELL_DRUID_ECLIPSE_LUNAR_BUFF          = 80505,
    SPELL_DRUID_BERSERK_CAT                 = 50334,
    SPELL_DRUID_RAKE_STUN                   = 80509,
    SPELL_DRUID_BERSERK_CAT_CRIT            = 80508,
    SPELL_DRUID_BERSERK_COMBO_GEN           = 80510,
    SPELL_DRUID_SAVAGE_ROAR                 = 80511,
    SPELL_DRUID_STELLAR_FLARE_DISPEL        = 80529,
    SPELL_DRUID_CELESTIAL_ALIGNMENT         = 80531,
    SPELL_DRUID_ECLIPSE_SOLAR_ALIGNMENT     = 80532,
    SPELL_DRUID_ECLIPSE_LUNAR_ALIGNMENT     = 80533,
    SPELL_DRUID_FORCE_OF_NATURE             = 33831,
    SPELL_DRUID_STELLAR_FLARE               = 80528,
    SPELL_DRUID_ASTRAL_COMMUNION            = 80534,
    SPELL_DRUID_SHOOTING_STARS              = 80537,
    SPELL_DRUID_SHOOTING_STARS_PROC         = 80538,
    SPELL_DRUID_WILD_MUSHROOM               = 80142,
    SPELL_DRUID_MOON_FALL                   = 80539,
    SPELL_DRUID_NEW_MOON                    = 80540,
    SPELL_DRUID_NEW_MOON_AURA               = 80543,
    SPELL_DRUID_HALF_MOON                   = 80541,
    SPELL_DRUID_HALF_MOON_AURA              = 80544,
    SPELL_DRUID_FULL_MOON                   = 80542,
    SPELL_DRUID_FULL_MOON_AURA              = 80545,
    SPELL_DRUID_RADIANT_MOON_AURA           = 700910,
    SPELL_DRUID_AVATAR_OF_ASHAMANE          = 80548,
};

// 1178 - Bear Form (Passive)
// 9635 - Dire Bear Form (Passive)
class spell_dru_bear_form_passive : public AuraScript
{
    PrepareAuraScript(spell_dru_bear_form_passive);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_ENRAGE, SPELL_DRUID_ITEM_T10_FERAL_4P_BONUS });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (!GetUnitOwner()->HasAura(SPELL_DRUID_ENRAGE) || GetUnitOwner()->HasAura(SPELL_DRUID_ITEM_T10_FERAL_4P_BONUS))
        {
            return;
        }

        int32 mod = 0;
        switch (GetId())
        {
        case SPELL_DRUID_BEAR_FORM_PASSIVE:
            mod = -48;
            break;
        case SPELL_DRUID_DIRE_BEAR_FORM_PASSIVE:
            mod = -59;
            break;
        default:
            return;
        }

        amount += mod;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_bear_form_passive::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_BASE_RESISTANCE_PCT);
    }
};

// 70723 - Item - Druid T10 Balance 4P Bonus
class spell_dru_t10_balance_4p_bonus : public AuraScript
{
    PrepareAuraScript(spell_dru_t10_balance_4p_bonus);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActor() && eventInfo.GetProcTarget();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        uint32 triggered_spell_id = 71023;
        SpellInfo const* triggeredSpell = sSpellMgr->GetSpellInfo(triggered_spell_id);

        int32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()) / triggeredSpell->GetMaxTicks();
        eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(GetTarget(), triggered_spell_id, SPELL_AURA_PERIODIC_DAMAGE, amount, EFFECT_0);

        //GetTarget()->CastCustomSpell(triggered_spell_id, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dru_t10_balance_4p_bonus::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dru_t10_balance_4p_bonus::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// -33872 - Nurturing Instinct
class spell_dru_nurturing_instinct : public AuraScript
{
    PrepareAuraScript(spell_dru_nurturing_instinct);

    void AfterApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetTarget()->ToPlayer())
            player->addSpell(GetSpellInfo()->GetRank() == 1 ? SPELL_DRUID_NURTURING_INSTINCT_R1 : SPELL_DRUID_NURTURING_INSTINCT_R2, SPEC_MASK_ALL, false, true);
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetTarget()->ToPlayer())
            player->removeSpell(GetSpellInfo()->GetRank() == 1 ? SPELL_DRUID_NURTURING_INSTINCT_R1 : SPELL_DRUID_NURTURING_INSTINCT_R2, SPEC_MASK_ALL, true);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_nurturing_instinct::AfterApply, EFFECT_0, SPELL_AURA_MOD_SPELL_HEALING_OF_STAT_PERCENT, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_nurturing_instinct::AfterRemove, EFFECT_0, SPELL_AURA_MOD_SPELL_HEALING_OF_STAT_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

/* 5487 - Bear Form
   9634 - Dire Bear Form */
class spell_dru_feral_swiftness : public AuraScript
{
    PrepareAuraScript(spell_dru_feral_swiftness);

    void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetTarget()->ToPlayer())
            if (uint8 rank = player->HasTalent(SPELL_DRUID_FERAL_SWIFTNESS_R1, player->GetActiveSpec()) ? 1 : (player->HasTalent(SPELL_DRUID_FERAL_SWIFTNESS_R2, player->GetActiveSpec()) ? 2 : 0))
                player->CastSpell(player, rank == 1 ? SPELL_DRUID_FERAL_SWIFTNESS_PASSIVE_1 : SPELL_DRUID_FERAL_SWIFTNESS_PASSIVE_2, true, nullptr, aurEff, GetCasterGUID());
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_FERAL_SWIFTNESS_PASSIVE_1);
        GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_FERAL_SWIFTNESS_PASSIVE_2);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_feral_swiftness::AfterApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_feral_swiftness::AfterRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 16864 - Omen of Clarity
class spell_dru_omen_of_clarity : public AuraScript
{
    PrepareAuraScript(spell_dru_omen_of_clarity);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo)
        {
            return true;
        }

        // Prevent passive spells to proc. (I.e shapeshift passives & passive talents)
        if (spellInfo->IsPassive())
        {
            return false;
        }

        if (eventInfo.GetTypeMask() & PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS)
        {
            return spellInfo->HasAttribute(SPELL_ATTR0_ON_NEXT_SWING) || spellInfo->HasAttribute(SPELL_ATTR0_ON_NEXT_SWING_NO_DAMAGE);
        }

        // Non-damaged/Non-healing spells - only druid abilities
        if (!spellInfo->HasAttribute(SpellCustomAttributes(SPELL_ATTR0_CU_DIRECT_DAMAGE | SPELL_ATTR0_CU_NO_INITIAL_THREAT)))
        {
            if (spellInfo->SpellFamilyName == SPELLFAMILY_DRUID)
            {
                // Exclude shapeshifting
                return !spellInfo->HasAura(SPELL_AURA_MOD_SHAPESHIFT);
            }

            return false;
        }

        return true;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dru_omen_of_clarity::CheckProc);
    }
};

// 50419 - Brambles
class spell_dru_brambles_treant : public AuraScript
{
    PrepareAuraScript(spell_dru_brambles_treant);

    bool CheckProc(ProcEventInfo&  /*eventInfo*/)
    {
        if (Player* player = GetUnitOwner()->GetSpellModOwner())
        {
            int32 amount = 0;
            if (player->HasAura(SPELL_DRUID_BARKSKIN, player->GetGUID()))
                player->ApplySpellMod(SPELL_DRUID_BARKSKIN, SPELLMOD_CHANCE_OF_SUCCESS, amount);

            return roll_chance_i(amount);
        }

        return false;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: chance of success stores proper amount of damage increase
        // xinef: little hack because GetSpellModOwner will return nullptr pointer at this point (early summoning stage)
        if (GetUnitOwner()->IsSummon())
            if (Unit* owner = GetUnitOwner()->ToTempSummon()->GetSummonerUnit())
                if (Player* player = owner->GetSpellModOwner())
                    player->ApplySpellMod(SPELL_DRUID_BARKSKIN, SPELLMOD_CHANCE_OF_SUCCESS, amount);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_brambles_treant::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        DoCheckProc += AuraCheckProcFn(spell_dru_brambles_treant::CheckProc);
    }
};

// 22812 - Barkskin
class spell_dru_barkskin : public AuraScript
{
    PrepareAuraScript(spell_dru_barkskin);

    void AfterApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetUnitOwner()->HasAura(SPELL_DRUID_GLYPH_OF_BARKSKIN, GetUnitOwner()->GetGUID()))
            GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_DRUID_GLYPH_OF_BARKSKIN_TRIGGER, true);
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetUnitOwner()->RemoveAurasDueToSpell(SPELL_DRUID_GLYPH_OF_BARKSKIN_TRIGGER, GetUnitOwner()->GetGUID());
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_barkskin::AfterApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_barkskin::AfterRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

/* 35669 - Serverside - Druid Pet Scaling 01
   35670 - Serverside - Druid Pet Scaling 02
   35671 - Serverside - Druid Pet Scaling 03
   35672 - Serverside - Druid Pet Scaling 04 */
class spell_dru_treant_scaling : public AuraScript
{
    PrepareAuraScript(spell_dru_treant_scaling);

    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: treant inherits 40% of resistance from owner and 35% of armor (guessed)
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            SpellSchoolMask schoolMask = SpellSchoolMask(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            int32 modifier = schoolMask == SPELL_SCHOOL_MASK_NORMAL ? 35 : 40;
            amount = CalculatePct(std::max<int32>(0, owner->GetResistance(schoolMask)), modifier);
        }
    }

    void CalculateStatAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: treant inherits 30% of intellect / stamina (guessed)
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            Stats stat = Stats(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            amount = CalculatePct(std::max<int32>(0, owner->GetStat(stat)), 30);
        }
    }

    void CalculateAPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: treant inherits 105% of SP as AP - 15% of damage increase per hit
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 nature = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_NATURE);
            amount = CalculatePct(std::max<int32>(0, nature), 105);

            // xinef: brambles talent
            if (AuraEffect const* bramblesEff = owner->GetAuraEffect(SPELL_AURA_ADD_FLAT_MODIFIER, SPELLFAMILY_DRUID, 53, 2))
                AddPct(amount, bramblesEff->GetAmount());
        }
    }

    void CalculateSPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: treant inherits 15% of SP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 nature = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_NATURE);
            amount = CalculatePct(std::max<int32>(0, nature), 15);

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
        if (m_scriptSpellId != 35669)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_treant_scaling::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);

        if (m_scriptSpellId == 35669 || m_scriptSpellId == 35670)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_treant_scaling::CalculateStatAmount, EFFECT_ALL, SPELL_AURA_MOD_STAT);

        if (m_scriptSpellId == 35669)
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_treant_scaling::CalculateAPAmount, EFFECT_ALL, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_treant_scaling::CalculateSPAmount, EFFECT_ALL, SPELL_AURA_MOD_DAMAGE_DONE);
        }

        OnEffectApply += AuraEffectApplyFn(spell_dru_treant_scaling::HandleEffectApply, EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

// -1850 - Dash
class spell_dru_dash : public AuraScript
{
    PrepareAuraScript(spell_dru_dash);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // do not set speed if not in cat form
        if (GetUnitOwner()->GetShapeshiftForm() != FORM_CAT)
            amount = 0;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_dash::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED);
    }
};

// 5229 - Enrage
class spell_dru_enrage : public AuraScript
{
    PrepareAuraScript(spell_dru_enrage);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_KING_OF_THE_JUNGLE, SPELL_DRUID_ENRAGE_MOD_DAMAGE, SPELL_DRUID_ENRAGED_DEFENSE, SPELL_DRUID_ITEM_T10_FERAL_4P_BONUS });
    }

    void RecalculateBaseArmor()
    {
        Unit::AuraEffectList const& auras = GetTarget()->GetAuraEffectsByType(SPELL_AURA_MOD_BASE_RESISTANCE_PCT);
        for (Unit::AuraEffectList::const_iterator i = auras.begin(); i != auras.end(); ++i)
        {
            SpellInfo const* spellInfo = (*i)->GetSpellInfo();
            // Dire- / Bear Form (Passive)
            if (spellInfo->SpellFamilyName == SPELLFAMILY_DRUID && spellInfo->SpellFamilyFlags.HasFlag(0x0, 0x0, 0x2))
            {
                (*i)->RecalculateAmount();
            }
        }
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (AuraEffect const* aurEff = target->GetAuraEffectOfRankedSpell(SPELL_DRUID_KING_OF_THE_JUNGLE, EFFECT_0))
        {
            target->CastCustomSpell(SPELL_DRUID_ENRAGE_MOD_DAMAGE, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), target, true);
        }

        // Item - Druid T10 Feral 4P Bonus
        if (target->HasAura(SPELL_DRUID_ITEM_T10_FERAL_4P_BONUS))
        {
            target->CastSpell(target, SPELL_DRUID_ENRAGED_DEFENSE, true);
        }

        RecalculateBaseArmor();
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_ENRAGE_MOD_DAMAGE);
        GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_ENRAGED_DEFENSE);

        RecalculateBaseArmor();
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_enrage::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_enrage::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE, AURA_EFFECT_HANDLE_REAL);
    }
};

// 54846 - Glyph of Starfire
class spell_dru_glyph_of_starfire : public SpellScript
{
    PrepareSpellScript(spell_dru_glyph_of_starfire);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_INCREASED_MOONFIRE_DURATION, SPELL_DRUID_NATURES_SPLENDOR });
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (Unit* unitTarget = GetHitUnit())
            if (AuraEffect const* aurEff = unitTarget->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_DRUID, 0x00000002, 0, 0, caster->GetGUID()))
            {
                Aura* aura = aurEff->GetBase();

                uint32 countMin = aura->GetMaxDuration();
                uint32 countMax = aura->GetSpellInfo()->GetMaxDuration() + 9000;
                if (caster->HasAura(SPELL_DRUID_INCREASED_MOONFIRE_DURATION))
                    countMax += 3000;
                if (caster->HasAura(SPELL_DRUID_NATURES_SPLENDOR))
                    countMax += 3000;

                if (countMin < countMax)
                {
                    aura->SetDuration(uint32(aura->GetDuration() + 3000));
                    aura->SetMaxDuration(countMin + 3000);
                }
            }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_glyph_of_starfire::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

/* 34246 - Increased Lifebloom Periodic
   60779 - Idol of Lush Moss */
class spell_dru_idol_lifebloom : public AuraScript
{
    PrepareAuraScript(spell_dru_idol_lifebloom);

    void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
    {
        if (!spellMod)
        {
            spellMod = new SpellModifier(GetAura());
            spellMod->op = SPELLMOD_DOT;
            spellMod->type = SPELLMOD_FLAT;
            spellMod->spellId = GetId();
            spellMod->mask = GetSpellInfo()->Effects[aurEff->GetEffIndex()].SpellClassMask;
        }
        spellMod->value = aurEff->GetAmount() / 7;
    }

    void Register() override
    {
        DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_idol_lifebloom::HandleEffectCalcSpellMod, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 29166 - Innervate
class spell_dru_innervate : public AuraScript
{
    PrepareAuraScript(spell_dru_innervate);

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* caster = GetCaster())
            amount = int32(CalculatePct(caster->GetCreatePowers(POWER_MANA), amount) / aurEff->GetTotalTicks());
        else
            amount = 0;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_innervate::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
    }
};

// -5570 - Insect Swarm
class spell_dru_insect_swarm : public AuraScript
{
    PrepareAuraScript(spell_dru_insect_swarm);

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* caster = GetCaster())
            if (AuraEffect const* relicAurEff = caster->GetAuraEffect(SPELL_DRUID_ITEM_T8_BALANCE_RELIC, EFFECT_0))
                amount += relicAurEff->GetAmount() / aurEff->GetTotalTicks();
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_insect_swarm::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// -33763 - Lifebloom
class spell_dru_lifebloom : public AuraScript
{
    PrepareAuraScript(spell_dru_lifebloom);

    bool Validate(SpellInfo const* /*spell*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, SPELL_DRUID_LIFEBLOOM_ENERGIZE });
    }

    void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        // Final heal only on duration end
        if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
            return;

        // final heal
        int32 stack = GetStackAmount();
        int32 healAmount = aurEff->GetAmount();
        SpellInfo const* finalHeal = sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL);

        if (Unit* caster = GetCaster())
        {
            healAmount = caster->SpellHealingBonusDone(GetTarget(), finalHeal, healAmount, HEAL, aurEff->GetEffIndex(), 0.0f, stack);
            healAmount = GetTarget()->SpellHealingBonusTaken(caster, finalHeal, healAmount, HEAL, stack);
            // restore mana
            int32 returnmana = (GetSpellInfo()->ManaCostPercentage * caster->GetCreateMana() / 100) * stack / 2;
            caster->CastCustomSpell(caster, SPELL_DRUID_LIFEBLOOM_ENERGIZE, &returnmana, nullptr, nullptr, true, nullptr, aurEff, GetCasterGUID());
        }
        GetTarget()->CastCustomSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, nullptr, nullptr, true, nullptr, aurEff, GetCasterGUID());
    }

    void HandleDispel(DispelInfo* dispelInfo)
    {
        if (Unit* target = GetUnitOwner())
        {
            if (GetEffect(EFFECT_1))
            {
                Unit* caster = GetCaster();
                int32 healAmount = GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster ? caster : target, 0, target) * dispelInfo->GetRemovedCharges();
                SpellInfo const* finalHeal = sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL);
                if (caster)
                {
                    // healing with bonus
                    healAmount = caster->SpellHealingBonusDone(target, finalHeal, healAmount, HEAL, EFFECT_1, 0.0f, dispelInfo->GetRemovedCharges());
                    healAmount = target->SpellHealingBonusTaken(caster, finalHeal, healAmount, HEAL, dispelInfo->GetRemovedCharges());

                    // mana amount
                    int32 mana = CalculatePct(caster->GetCreateMana(), GetSpellInfo()->ManaCostPercentage) * dispelInfo->GetRemovedCharges() / 2;
                    caster->CastCustomSpell(caster, SPELL_DRUID_LIFEBLOOM_ENERGIZE, &mana, nullptr, nullptr, true, nullptr, nullptr, GetCasterGUID());
                }
                target->CastCustomSpell(target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, nullptr, nullptr, true, nullptr, nullptr, GetCasterGUID());
            }
        }
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterDispel += AuraDispelFn(spell_dru_lifebloom::HandleDispel);
    }
};

// -48496 - Living Seed
class spell_dru_living_seed : public AuraScript
{
    PrepareAuraScript(spell_dru_living_seed);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_LIVING_SEED_PROC });
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        if (!eventInfo.GetHealInfo() || !eventInfo.GetProcTarget())
        {
            return;
        }

        int32 amount = CalculatePct(eventInfo.GetHealInfo()->GetHeal(), aurEff->GetAmount());
        GetTarget()->CastCustomSpell(SPELL_DRUID_LIVING_SEED_PROC, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_living_seed::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 48504 - Living Seed
class spell_dru_living_seed_proc : public AuraScript
{
    PrepareAuraScript(spell_dru_living_seed_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_LIVING_SEED_HEAL });
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        GetTarget()->CastCustomSpell(SPELL_DRUID_LIVING_SEED_HEAL, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 69366 - Moonkin Form (Passive)
class spell_dru_moonkin_form_passive : public AuraScript
{
    PrepareAuraScript(spell_dru_moonkin_form_passive);

    uint32 absorbPct;

    bool Load() override
    {
        absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
        return true;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        // reduces all damage taken while Stunned in Moonkin Form
        if (GetTarget()->GetUnitFlags() & (UNIT_FLAG_STUNNED) && GetTarget()->HasAuraWithMechanic(1 << MECHANIC_STUN))
            absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_moonkin_form_passive::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_moonkin_form_passive::Absorb, EFFECT_0);
    }
};

// 48391 - Owlkin Frenzy
class spell_dru_owlkin_frenzy : public AuraScript
{
    PrepareAuraScript(spell_dru_owlkin_frenzy);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = CalculatePct(GetUnitOwner()->GetCreatePowers(POWER_MANA), amount);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_owlkin_frenzy::CalculateAmount, EFFECT_2, SPELL_AURA_PERIODIC_ENERGIZE);
    }
};

// -16972 - Predatory Strikes
class spell_dru_predatory_strikes : public AuraScript
{
    PrepareAuraScript(spell_dru_predatory_strikes);

    void UpdateAmount(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->UpdateAttackPowerAndDamage();
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_predatory_strikes::UpdateAmount, EFFECT_ALL, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_predatory_strikes::UpdateAmount, EFFECT_ALL, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
    }
};

// 33851 - Primal Tenacity
class spell_dru_primal_tenacity : public AuraScript
{
    PrepareAuraScript(spell_dru_primal_tenacity);

    uint32 absorbPct;

    bool Load() override
    {
        absorbPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
        return true;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        // reduces all damage taken while Stunned in Cat Form
        if (GetTarget()->GetShapeshiftForm() == FORM_CAT && GetTarget()->HasUnitFlag(UNIT_FLAG_STUNNED) && GetTarget()->HasAuraWithMechanic(1 << MECHANIC_STUN))
            absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_primal_tenacity::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_primal_tenacity::Absorb, EFFECT_1);
    }
};

// -1079 - Rip
class spell_dru_rip : public AuraScript
{
    PrepareAuraScript(spell_dru_rip);

    bool Load() override
    {
        Unit* caster = GetCaster();
        return caster && caster->GetTypeId() == TYPEID_PLAYER;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {


        canBeRecalculated = false;

        if (Unit* caster = GetCaster())
        {
            // 0.01 * $AP * cp
            uint8 cp = caster->ToPlayer()->GetComboPoints();

            // Idol of Feral Shadows. Can't be handled as SpellMod due its dependency from CPs
            if (AuraEffect const* idol = caster->GetAuraEffect(SPELL_DRUID_IDOL_OF_FERAL_SHADOWS, EFFECT_0))
                amount += cp * idol->GetAmount();
            // Idol of Worship. Can't be handled as SpellMod due its dependency from CPs
            else if (AuraEffect const* idol2 = caster->GetAuraEffect(SPELL_DRUID_IDOL_OF_WORSHIP, EFFECT_0))
                amount += cp * idol2->GetAmount();

            amount += int32(CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), cp));
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rip::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// 62606 - Savage Defense
class spell_dru_savage_defense : public AuraScript
{
    PrepareAuraScript(spell_dru_savage_defense);

    uint32 absorbPct;

    bool Load() override
    {
        absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
        return true;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& /*dmgInfo*/, uint32& absorbAmount)
    {
        absorbAmount = uint32(CalculatePct(GetTarget()->GetTotalAttackPowerValue(BASE_ATTACK), absorbPct));
        aurEff->SetAmount(0);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_savage_defense::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_savage_defense::Absorb, EFFECT_0);
    }
};

// 52610 - Savage Roar
class spell_dru_savage_roar : public SpellScript
{
    PrepareSpellScript(spell_dru_savage_roar);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        if (caster->GetShapeshiftForm() != FORM_CAT)
            return SPELL_FAILED_ONLY_SHAPESHIFT;

        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar::CheckCast);
    }
};

class spell_dru_savage_roar_aura : public AuraScript
{
    PrepareAuraScript(spell_dru_savage_roar_aura);

    bool Validate(SpellInfo const* /*spell*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_SAVAGE_ROAR });
    }

    void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        target->CastSpell(target, SPELL_DRUID_SAVAGE_ROAR, true, nullptr, aurEff, GetCasterGUID());
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_SAVAGE_ROAR);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_savage_roar_aura::AfterApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_savage_roar_aura::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// -50294 - Starfall
class spell_dru_starfall_aoe : public SpellScript
{
    PrepareSpellScript(spell_dru_starfall_aoe);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove(GetExplTargetUnit());
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_starfall_aoe::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// -50286 - Starfall
class spell_dru_starfall_dummy : public SpellScript
{
    PrepareSpellScript(spell_dru_starfall_dummy);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Acore::Containers::RandomResize(targets, 2);
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_starfall_dummy::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_dru_starfall_dummy::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 61336 - Survival Instincts
class spell_dru_survival_instincts : public SpellScript
{
    PrepareSpellScript(spell_dru_survival_instincts);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        if (!caster->IsInFeralForm())
            return SPELL_FAILED_ONLY_SHAPESHIFT;

        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dru_survival_instincts::CheckCast);
    }
};

class spell_dru_survival_instincts_aura : public AuraScript
{
    PrepareAuraScript(spell_dru_survival_instincts_aura);

    bool Validate(SpellInfo const* /*spell*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_SURVIVAL_INSTINCTS });
    }

    void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        int32 bp0 = target->CountPctFromMaxHealth(aurEff->GetAmount());
        target->CastCustomSpell(target, SPELL_DRUID_SURVIVAL_INSTINCTS, &bp0, nullptr, nullptr, true);
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_SURVIVAL_INSTINCTS);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_survival_instincts_aura::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_survival_instincts_aura::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
    }
};

// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive : public AuraScript
{
    PrepareAuraScript(spell_dru_swift_flight_passive);

    bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (caster->Has310Flyer(false))
                amount = 310;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_swift_flight_passive::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
    }
};

// -5217 - Tiger's Fury
class spell_dru_tiger_s_fury : public SpellScript
{
    PrepareSpellScript(spell_dru_tiger_s_fury);

    void OnHit()
    {
        if (AuraEffect const* aurEff = GetHitUnit()->GetAuraEffectOfRankedSpell(SPELL_DRUID_KING_OF_THE_JUNGLE, EFFECT_1))
            GetHitUnit()->CastCustomSpell(SPELL_DRUID_TIGER_S_FURY_ENERGIZE, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetHitUnit(), true);
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_dru_tiger_s_fury::OnHit);
    }
};

// -61391 - Typhoon
class spell_dru_typhoon : public SpellScript
{
    PrepareSpellScript(spell_dru_typhoon);

    void HandleKnockBack(SpellEffIndex effIndex)
    {
        // Glyph of Typhoon
        if (GetCaster()->HasAura(SPELL_DRUID_GLYPH_OF_TYPHOON))
            PreventHitDefaultEffect(effIndex);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_typhoon::HandleKnockBack, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
    }
};

// 70691 - Rejuvenation
class spell_dru_t10_restoration_4p_bonus : public SpellScript
{
    PrepareSpellScript(spell_dru_t10_restoration_4p_bonus);

    bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (!GetCaster()->ToPlayer()->GetGroup())
        {
            targets.clear();
            targets.push_back(GetCaster());
        }
        else
        {
            targets.remove(GetExplTargetUnit());
            std::list<Unit*> tempTargets;
            for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                if ((*itr)->isType(TYPEMASK_UNIT | TYPEMASK_PLAYER) && GetCaster()->IsInRaidWith((*itr)->ToUnit()) && !(*itr)->ToUnit()->GetAuraEffect(SPELL_AURA_PERIODIC_HEAL, SPELLFAMILY_DRUID, 64, EFFECT_0))
                    tempTargets.push_back((*itr)->ToUnit());

            if (tempTargets.empty())
            {
                targets.clear();
                FinishCast(SPELL_FAILED_DONT_REPORT);
                return;
            }

            tempTargets.sort(Acore::HealthPctOrderPred());
            targets.clear();
            targets.push_back(tempTargets.front());
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_t10_restoration_4p_bonus::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// -48438 - Wild Growth
class spell_dru_wild_growth : public SpellScript
{
    PrepareSpellScript(spell_dru_wild_growth);

    bool Validate(SpellInfo const* spellInfo) override
    {
        if (spellInfo->Effects[EFFECT_2].IsEffect() || spellInfo->Effects[EFFECT_2].CalcValue() <= 0)
            return false;
        return true;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::RaidCheck(GetCaster(), false));

        uint32 const maxTargets = GetCaster()->HasAura(SPELL_DRUID_GLYPH_OF_WILD_GROWTH) ? 6 : 5;

        if (targets.size() > maxTargets)
        {
            targets.sort(Acore::HealthPctOrderPred());
            targets.resize(maxTargets);
        }

        _targets = targets;
    }

    void SetTargets(std::list<WorldObject*>& targets)
    {
        targets = _targets;
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth::SetTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
    }

private:
    std::list<WorldObject*> _targets;
};

// -50334 - Berserk
class spell_dru_berserk : public SpellScript
{
    PrepareSpellScript(spell_dru_berserk);

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();

        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
            // Remove tiger fury / mangle(bear)
            const uint32 TigerFury[6] = { 5217, 6793, 9845, 9846, 50212, 50213 };
            const uint32 DireMaul[6] = { 33878, 33986, 33987, 48563, 48564 };

            // remove aura
            for (auto& i : TigerFury)
                caster->RemoveAurasDueToSpell(i);

            // reset dire bear maul cd
            for (auto& i : DireMaul)
                caster->ToPlayer()->RemoveSpellCooldown(i, true);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dru_berserk::HandleAfterCast);
    }
};

// 24905 - Moonkin Form (Passive)
class spell_dru_moonkin_form_passive_proc : public AuraScript
{
    PrepareAuraScript(spell_dru_moonkin_form_passive_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            return !spellInfo->IsAffectingArea();
        }

        return false;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dru_moonkin_form_passive_proc::CheckProc);
    }
};

class spell_dru_ferocious_bite : public SpellScript
{
    PrepareSpellScript(spell_dru_ferocious_bite);

    Aura* GetTasteForBloodRuneAura(Unit* caster)
    {
        for (size_t i = 700168; i < 700174; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleHit(SpellEffIndex effIndex)
    {
        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(SPELL_DRUID_FEROCIOUS_BITE);
        int32 damageRatio = GetCaster()->GetComboPoints() * GetEffectValue();
        int32 damage = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);
        int32 energy = GetCaster()->GetPower(POWER_ENERGY);
        int32 consumption = spell->GetEffect(EFFECT_2).CalcValue(GetCaster());

        if (energy > 0)
        {
            int32 bonusPercent = std::min<int32>(energy, consumption);
            int32 bonusDamage = bonusPercent * (sSpellMgr->AssertSpellInfo(SPELL_DRUID_FEROCIOUS_BITE)->GetEffect(EFFECT_1).CalcValue());
            damage += int32(CalculatePct(damage, bonusDamage));

            GetCaster()->ModifyPower(POWER_ENERGY, -bonusPercent);
        }

        if (Aura* runeAura = GetTasteForBloodRuneAura(GetCaster()))
        {
            int32 bleedQuantity = 0;
            auto targetAuras = GetExplTargetUnit()->GetAppliedAuras();
            for (auto itr = targetAuras.begin(); itr != targetAuras.end(); ++itr)
            {
                if (Aura* aura = itr->second->GetBase())
                {
                    if (GetCaster()->GetGUID() != aura->GetCasterGUID())
                        continue;

                    SpellInfo const* auraInfo = aura->GetSpellInfo();

                    if (auraInfo->SpellFamilyFlags[2] & 0x01000000 && auraInfo->SpellFamilyName == SPELLFAMILY_DRUID)
                    {
                        bleedQuantity++;
                    }
                }
            }

            if (bleedQuantity >= 0)
            {
                int32 percentIncrease = runeAura->GetEffect(EFFECT_0)->GetAmount() * bleedQuantity;
                damage += int32(CalculatePct(damage, percentIncrease));
            }
        }

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_ferocious_bite::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_dru_maim : public SpellScript
{
    PrepareSpellScript(spell_dru_maim);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damageRatio = GetCaster()->GetComboPoints() * GetEffectValue();
        int32 damage = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_maim::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_dru_prowl : public AuraScript
{
    PrepareAuraScript(spell_dru_prowl);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(SPELL_DRUID_PROWL))
            GetCaster()->RemoveAura(SPELL_DRUID_PROWL);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_prowl::HandleRemove, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
    }
};


class spell_dru_thorns : public AuraScript
{
    PrepareAuraScript(spell_dru_thorns);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetCaster() || !GetCaster()->IsAlive())
            return;

        Unit* target = eventInfo.GetProcTarget();

        GetCaster()->CastSpell(target, SPELL_DRUID_THORNS_SLOW, true, nullptr, aurEff, GetCasterGUID());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_thorns::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dru_wrath : public SpellScript
{
    PrepareSpellScript(spell_dru_wrath);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster->HasAura(SPELL_DRUID_MOONKIN_FORM))
            return;

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_WRATH);
        uint32 astralPower = value->GetEffect(EFFECT_1).CalcValue(caster);
        caster->ModifyPower(POWER_RUNIC_POWER, astralPower);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_wrath::HandleCast);
    }
};

class spell_dru_starfire : public SpellScript
{
    PrepareSpellScript(spell_dru_starfire);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        Unit* target = GetExplTargetUnit();

        caster->CastSpell(target, SPELL_DRUID_STARFIRE_AOE);

        if (!caster->HasAura(SPELL_DRUID_MOONKIN_FORM))
            return;

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_STARFIRE);
        uint32 astralPower = value->GetEffect(EFFECT_1).CalcValue(caster);
        caster->ModifyPower(POWER_RUNIC_POWER, astralPower);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_starfire::HandleCast);
    }
};

class spell_dru_eclipse : public AuraScript
{

    PrepareAuraScript(spell_dru_eclipse);

    void handleBuff(Unit* caster, uint32 buffSpellId, uint32 stackSpellId, uint32 maxStack)
    {
        if (Aura* aurff = caster->GetAura(stackSpellId))
        {
            aurff->ModStackAmount(1);
            if (aurff->GetStackAmount() >= maxStack) {
                caster->AddAura(buffSpellId, caster);
                caster->RemoveAura(stackSpellId);
            }
        }
        else
            caster->AddAura(stackSpellId, caster);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (caster->HasAura(SPELL_DRUID_CELESTIAL_ALIGNMENT))
            return;

        uint32 maxSolarStack = aurEff->GetAmount();
        uint32 maxLunarStack = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        uint32 spellId = eventInfo.GetProcSpell()->GetSpellInfo()->Id;

        if (spellId == SPELL_DRUID_WRATH && !caster->HasAura(SPELL_DRUID_ECLIPSE_LUNAR_STACK) && !caster->HasAura(SPELL_DRUID_ECLIPSE_LUNAR_BUFF))
            handleBuff(caster, SPELL_DRUID_ECLIPSE_SOLAR_BUFF, SPELL_DRUID_ECLIPSE_SOLAR_STACK, maxSolarStack);
        if (spellId == SPELL_DRUID_STARFIRE && !caster->HasAura(SPELL_DRUID_ECLIPSE_SOLAR_STACK) && !caster->HasAura(SPELL_DRUID_ECLIPSE_SOLAR_BUFF))
            handleBuff(caster, SPELL_DRUID_ECLIPSE_LUNAR_BUFF, SPELL_DRUID_ECLIPSE_LUNAR_STACK, maxLunarStack);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_eclipse::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dru_force_of_nature : public SpellScript
{
    PrepareSpellScript(spell_dru_force_of_nature);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster->HasAura(SPELL_DRUID_MOONKIN_FORM))
            return;


        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_FORCE_OF_NATURE);
        uint32 astralPower = value->GetEffect(EFFECT_1).CalcValue(caster);
        caster->ModifyPower(POWER_RUNIC_POWER, astralPower);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_force_of_nature::HandleCast);
    }
};

class spell_dru_berserk_cat : public AuraScript
{
    PrepareAuraScript(spell_dru_berserk_cat);


    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->AddAura(SPELL_DRUID_BERSERK_CAT_CRIT, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->RemoveAura(SPELL_DRUID_BERSERK_CAT_CRIT);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_berserk_cat::HandleApply, EFFECT_1, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_berserk_cat::HandleRemove, EFFECT_1, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_berserk_combo_adder : public SpellScript
{
    PrepareSpellScript(spell_dru_berserk_combo_adder);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster || !caster->IsAlive())
            return;

        if (!caster->HasAura(SPELL_DRUID_BERSERK_CAT))
            return;

        int32 baseProc = 20;
        int32 procChance = GetCaster()->GetComboPoints() * baseProc;

        if (roll_chance_f(procChance))
        {
            caster->AddAura(SPELL_DRUID_BERSERK_COMBO_GEN, caster);
        }
    }

    void HandleAfter()
    {
        if (GetCaster()->HasAura(SPELL_DRUID_BERSERK_COMBO_GEN))
        {
            int32 comboAmount = sSpellMgr->AssertSpellInfo(SPELL_DRUID_BERSERK_CAT)->GetEffect(EFFECT_1).CalcValue();
            GetCaster()->RemoveAura(SPELL_DRUID_BERSERK_COMBO_GEN);
            Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetTarget());

            GetCaster()->AddComboPoints(target, 2);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_berserk_combo_adder::HandleCast);
        AfterCast += SpellCastFn(spell_dru_berserk_combo_adder::HandleAfter);
    }
};

class spell_dru_rake : public SpellScript
{
    PrepareSpellScript(spell_dru_rake);

    void HandleCast()
    {
        if (!GetCaster() || !GetCaster()->IsAlive())
            return;

        Unit* caster = GetCaster();

        if (caster->HasAura(SPELL_DRUID_BERSERK_CAT))
            caster->CastSpell(GetExplTargetUnit(), SPELL_DRUID_RAKE_STUN, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_rake::HandleCast);
    }
};

class spell_dru_cat_form : public AuraScript
{
    PrepareAuraScript(spell_dru_cat_form);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(SPELL_DRUID_SAVAGE_ROAR))
            GetCaster()->RemoveAura(SPELL_DRUID_SAVAGE_ROAR);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_cat_form::HandleRemove, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_lifebloom_new : public AuraScript
{
    PrepareAuraScript(spell_dru_lifebloom_new);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->CastSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_new::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_switftmend : public SpellScript
{
    PrepareSpellScript(spell_dru_switftmend);

    AuraEffect* GetAuraEffect()
    {
        Unit::AuraEffectList const& RejorRegr = GetExplTargetUnit()->GetAuraEffectsByType(SPELL_AURA_PERIODIC_HEAL);
        AuraEffect* forcedTargetAura = nullptr;
        AuraEffect* targetAura = nullptr;
        for (Unit::AuraEffectList::const_iterator i = RejorRegr.begin(); i != RejorRegr.end(); ++i)
        {
            if ((*i)->GetSpellInfo()->SpellFamilyName == SPELLFAMILY_DRUID
                && (*i)->GetSpellInfo()->SpellFamilyFlags[2] == 0x80000000)
            {
                if (GetCaster()->GetGUID() == (*i)->GetCasterGUID())
                {
                    if (!forcedTargetAura || (*i)->GetBase()->GetDuration() < forcedTargetAura->GetBase()->GetDuration())
                        forcedTargetAura = *i;
                }
                else if (!targetAura || (*i)->GetBase()->GetDuration() < targetAura->GetBase()->GetDuration())
                    targetAura = *i;
            }
        }

        if (forcedTargetAura)
            targetAura = forcedTargetAura;

        return targetAura;
    }

    void HandleCast()
    {
        AuraEffect* aura = GetAuraEffect();

        if (aura) {
            GetExplTargetUnit()->RemoveAura(aura->GetId(), aura->GetCasterGUID());
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_switftmend::HandleCast);
    }
};

class spell_dru_wild_charge : public SpellScript
{
    PrepareSpellScript(spell_dru_wild_charge);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        Unit* target = GetExplTargetUnit();
        Position targetPos = GetExplTargetUnit()->GetPosition();

        if (!GetCaster() || !GetCaster()->IsAlive())
            return;

        if (!target || !target->IsAlive())
            return;

        caster->GetMotionMaster()->MoveJump(targetPos, 15.0f, 15.0f);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_wild_charge::HandleCast);
    }
};

class spell_dru_tiger_dash : public AuraScript
{
    PrepareAuraScript(spell_dru_tiger_dash);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->RemoveAura(80516);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_tiger_dash::HandleProc, EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_prowl_check : public SpellScript
{
    PrepareSpellScript(spell_dru_prowl_check);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();

        if (caster->IsInCombat() && !caster->HasAura(SPELL_DRUID_AVATAR_OF_ASHAMANE))
            return SPELL_FAILED_TARGET_IN_COMBAT;

        return SPELL_CAST_OK;
    } 

    void HandleCast()
    {
        if (!GetCaster() || !GetCaster()->IsAlive())
            return;

        if (GetCaster()->HasAura(SPELL_DRUID_CAT_FORM))
            return;

        GetCaster()->CastSpell(GetCaster(), SPELL_DRUID_CAT_FORM);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dru_prowl_check::CheckCast);
        OnCast += SpellCastFn(spell_dru_prowl_check::HandleCast);
    }
};

class spell_dru_stellar_flare_power : public SpellScript
{
    PrepareSpellScript(spell_dru_stellar_flare_power);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster->HasAura(SPELL_DRUID_MOONKIN_FORM))
            return;

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_STELLAR_FLARE);
        uint32 astralPower = value->GetEffect(EFFECT_2).CalcValue(caster);
        caster->ModifyPower(POWER_RUNIC_POWER, astralPower);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_stellar_flare_power::HandleCast);
    }
};

class spell_dru_stellar_flare_dispel : public AuraScript
{
    PrepareAuraScript(spell_dru_stellar_flare_dispel);

    void HandleDispel(DispelInfo* dispelInfo)
    {
        Unit* target = dispelInfo->GetDispeller();

        GetCaster()->CastSpell(target, SPELL_DRUID_STELLAR_FLARE_DISPEL);
    }

    void Register() override
    {
        OnDispel += AuraDispelFn(spell_dru_stellar_flare_dispel::HandleDispel);
    }
};

class spell_dru_celestial_alignment : public AuraScript
{
    PrepareAuraScript(spell_dru_celestial_alignment);


    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->AddAura(SPELL_DRUID_ECLIPSE_LUNAR_ALIGNMENT, GetCaster());
        GetCaster()->AddAura(SPELL_DRUID_ECLIPSE_SOLAR_ALIGNMENT, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->RemoveAura(SPELL_DRUID_ECLIPSE_LUNAR_ALIGNMENT);
        GetCaster()->RemoveAura(SPELL_DRUID_ECLIPSE_SOLAR_ALIGNMENT);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_celestial_alignment::HandleApply, EFFECT_0, SPELL_AURA_MOD_MELEE_RANGED_HASTE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_celestial_alignment::HandleRemove, EFFECT_0, SPELL_AURA_MOD_MELEE_RANGED_HASTE, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_astral_communion_power : public SpellScript
{
    PrepareSpellScript(spell_dru_astral_communion_power);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster->HasAura(SPELL_DRUID_MOONKIN_FORM))
            return;

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_ASTRAL_COMMUNION);
        uint32 astralPower = value->GetEffect(EFFECT_0).CalcValue(caster);
        caster->ModifyPower(POWER_RUNIC_POWER, astralPower);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_astral_communion_power::HandleCast);
    }
};

class spell_dru_wild_mushroom : public SpellScript
{
    PrepareSpellScript(spell_dru_wild_mushroom);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(SPELL_DRUID_WILD_MUSHROOM);
        uint32 astralPowerAmount = spell->GetEffect(EFFECT_1).CalcValue(caster);
        int32 astralPowerBase = spell->GetEffect(EFFECT_1).CalcValue(caster);
        int32 astralPowerLimit = spell->GetEffect(EFFECT_2).CalcValue(caster);

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead() || astralPowerLimit > 20)
                continue;

            caster->ModifyPower(POWER_RUNIC_POWER, astralPowerAmount);
            astralPowerLimit += astralPowerBase;
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_mushroom::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class spell_dru_shooting_stars : public AuraScript
{
    PrepareAuraScript(spell_dru_shooting_stars);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        if (!GetCaster() || !GetCaster()->IsAlive())
            return;

        Unit* caster = GetCaster();

        Unit* target = procInfo.GetActionTarget();

        if (!target || !target->IsAlive())
            return;

        int32 procChance = GetAura()->GetEffect(EFFECT_0)->GetAmount();

        if (procInfo.GetHitMask() == PROC_EX_CRITICAL_HIT)
        {
            procChance = procChance * 2;
        }

        if (roll_chance_f(procChance))
        {
            caster->CastSpell(target, SPELL_DRUID_SHOOTING_STARS_PROC, true);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_shooting_stars::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dru_shooting_stars_power : public SpellScript
{
    PrepareSpellScript(spell_dru_shooting_stars_power);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster->HasAura(SPELL_DRUID_MOONKIN_FORM))
            return;

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_SHOOTING_STARS_PROC);
        uint32 astralPower = value->GetEffect(EFFECT_1).CalcValue(caster);
        caster->ModifyPower(POWER_RUNIC_POWER, astralPower);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_shooting_stars_power::HandleCast);
    }
};

class spell_dru_moon_fall : public SpellScript
{
    PrepareSpellScript(spell_dru_moon_fall);

    void HandlePower(uint32 castSpellId)
    {
        Unit* caster = GetCaster();

        if (!caster->HasAura(SPELL_DRUID_MOONKIN_FORM))
            return;

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(castSpellId);
        uint32 astralPower = value->GetEffect(EFFECT_1).CalcValue(caster);
        caster->ModifyPower(POWER_RUNIC_POWER, astralPower);
    }

    void HandleCast()
    {
        Unit* target = GetExplTargetUnit();
        Unit* caster = GetCaster();


        if (caster->HasAura(SPELL_DRUID_HALF_MOON_AURA))
        {
            caster->CastSpell(target, SPELL_DRUID_HALF_MOON, TRIGGERED_FULL_MASK);
            HandlePower(SPELL_DRUID_HALF_MOON);
        }
        else if (caster->HasAura(SPELL_DRUID_FULL_MOON_AURA) || caster->HasAura(SPELL_DRUID_RADIANT_MOON_AURA))
        {
            caster->CastSpell(target, SPELL_DRUID_FULL_MOON, TRIGGERED_FULL_MASK);
            HandlePower(SPELL_DRUID_FULL_MOON);
        }
        else
        {
            caster->CastSpell(target, SPELL_DRUID_NEW_MOON, TRIGGERED_FULL_MASK);
            HandlePower(SPELL_DRUID_NEW_MOON);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_moon_fall::HandleCast);
    }
};

class spell_dru_moon_fall_aura : public SpellScript
{
    PrepareSpellScript(spell_dru_moon_fall_aura);

    Aura* GetRadiantMoonlightRuneAura(Unit* caster)
    {
        for (size_t i = 700904; i < 700910; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (caster->HasAura(SPELL_DRUID_NEW_MOON_AURA))
        {
            caster->RemoveAura(SPELL_DRUID_NEW_MOON_AURA);
            caster->AddAura(SPELL_DRUID_HALF_MOON_AURA, caster);
        }
        else if (caster->HasAura(SPELL_DRUID_HALF_MOON_AURA))
        {
            caster->RemoveAura(SPELL_DRUID_HALF_MOON_AURA);
            caster->AddAura(SPELL_DRUID_FULL_MOON_AURA, caster);
        }
        else if (caster->HasAura(SPELL_DRUID_FULL_MOON_AURA))
        {
            if (GetRadiantMoonlightRuneAura(caster))
            {
                caster->RemoveAura(SPELL_DRUID_FULL_MOON_AURA);
                caster->AddAura(SPELL_DRUID_RADIANT_MOON_AURA, caster);
            }
            else
            {
                caster->RemoveAura(SPELL_DRUID_FULL_MOON_AURA);
                caster->AddAura(SPELL_DRUID_NEW_MOON_AURA, caster);
            }
        }
        else
        {
            caster->RemoveAura(SPELL_DRUID_RADIANT_MOON_AURA);
            caster->AddAura(SPELL_DRUID_NEW_MOON_AURA, caster);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_moon_fall_aura::HandleCast);
    }
};

class spell_dru_avatar_of_ashamane : public AuraScript
{
    PrepareAuraScript(spell_dru_avatar_of_ashamane);


    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->AddAura(SPELL_DRUID_BERSERK_CAT, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->RemoveAura(SPELL_DRUID_CAT_FORM);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_avatar_of_ashamane::HandleApply, EFFECT_0, SPELL_AURA_MOD_MELEE_RANGED_HASTE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_avatar_of_ashamane::HandleRemove, EFFECT_0, SPELL_AURA_MOD_MELEE_RANGED_HASTE, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_druid_spell_scripts()
{
    RegisterSpellScript(spell_dru_bear_form_passive);
    RegisterSpellScript(spell_dru_t10_balance_4p_bonus);
    RegisterSpellScript(spell_dru_nurturing_instinct);
    RegisterSpellScript(spell_dru_feral_swiftness);
    RegisterSpellScript(spell_dru_omen_of_clarity);
    RegisterSpellScript(spell_dru_brambles_treant);
    RegisterSpellScript(spell_dru_barkskin);
    RegisterSpellScript(spell_dru_treant_scaling);
    RegisterSpellScript(spell_dru_berserk);
    RegisterSpellScript(spell_dru_dash);
    RegisterSpellScript(spell_dru_enrage);
    RegisterSpellScript(spell_dru_glyph_of_starfire);
    RegisterSpellScript(spell_dru_idol_lifebloom);
    RegisterSpellScript(spell_dru_innervate);
    RegisterSpellScript(spell_dru_insect_swarm);
    //RegisterSpellScript(spell_dru_lifebloom);
    RegisterSpellScript(spell_dru_living_seed);
    RegisterSpellScript(spell_dru_living_seed_proc);
    RegisterSpellScript(spell_dru_moonkin_form_passive);
    RegisterSpellScript(spell_dru_owlkin_frenzy);
    RegisterSpellScript(spell_dru_predatory_strikes);
    RegisterSpellScript(spell_dru_primal_tenacity);
    RegisterSpellScript(spell_dru_rip);
    RegisterSpellScript(spell_dru_savage_defense);
    RegisterSpellScript(spell_dru_savage_roar);
    //RegisterSpellAndAuraScriptPair(spell_dru_savage_roar, spell_dru_savage_roar_aura);
    //RegisterSpellScript(spell_dru_starfall_aoe);
    //RegisterSpellScript(spell_dru_starfall_dummy);
    RegisterSpellAndAuraScriptPair(spell_dru_survival_instincts, spell_dru_survival_instincts_aura);
    RegisterSpellScript(spell_dru_swift_flight_passive);
    RegisterSpellScript(spell_dru_tiger_s_fury);
    RegisterSpellScript(spell_dru_typhoon);
    RegisterSpellScript(spell_dru_t10_restoration_4p_bonus);
    RegisterSpellScript(spell_dru_wild_growth);
    RegisterSpellScript(spell_dru_moonkin_form_passive_proc);
    RegisterSpellScript(spell_dru_ferocious_bite);
    RegisterSpellScript(spell_dru_maim);
    RegisterSpellScript(spell_dru_prowl);
    RegisterSpellScript(spell_dru_thorns);
    RegisterSpellScript(spell_dru_wrath);
    RegisterSpellScript(spell_dru_starfire);
    RegisterSpellScript(spell_dru_force_of_nature);
    RegisterSpellScript(spell_dru_berserk_cat);
    RegisterSpellScript(spell_dru_rake);
    RegisterSpellScript(spell_dru_eclipse);
    RegisterSpellScript(spell_dru_berserk_combo_adder);
    RegisterSpellScript(spell_dru_cat_form);
    RegisterSpellScript(spell_dru_lifebloom_new);
    RegisterSpellScript(spell_dru_switftmend);
    RegisterSpellScript(spell_dru_wild_charge);
    RegisterSpellScript(spell_dru_tiger_dash);
    RegisterSpellScript(spell_dru_prowl_check);
    RegisterSpellScript(spell_dru_stellar_flare_power);
    RegisterSpellScript(spell_dru_stellar_flare_dispel);
    RegisterSpellScript(spell_dru_celestial_alignment);
    RegisterSpellScript(spell_dru_astral_communion_power);
    RegisterSpellScript(spell_dru_wild_mushroom);
    RegisterSpellScript(spell_dru_shooting_stars);
    RegisterSpellScript(spell_dru_shooting_stars_power);
    RegisterSpellScript(spell_dru_moon_fall);
    RegisterSpellScript(spell_dru_moon_fall_aura);
    RegisterSpellScript(spell_dru_avatar_of_ashamane);
}
