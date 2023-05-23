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
    //Old Stuff
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
    SPELL_DRUID_KING_OF_THE_FOREST          = 80606,
    SPELL_DRUID_LIFEBLOOM_ENERGIZE          = 64372,
    SPELL_DRUID_LIFEBLOOM_FINAL_HEAL        = 80588,
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

    //New Stuff
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
    SPELL_DRUID_AVATAR_OF_ASHAMANE          = 80548,
    SPELL_DRUID_RIP                         = 80558,
    SPELL_DRUID_RIP_DOT                     = 49800,
    SPELL_DRUID_PRIMAL_WRATH                = 80551,
    SPELL_DRUID_MOONFIRE_CAT                = 80547,
    SPELL_DRUID_SOOTHE_CAT                  = 80554,
    SPELL_DRUID_REMOVE_CORRUPTION_CAT       = 80553,
    SPELL_DRUID_IRONFUR_BASE                = 80555,
    SPELL_DRUID_IRONFUR_ARMOR               = 80556,
    SPELL_DRUID_URSINE_ADEPT                = 80560,
    SPELL_DRUID_MOONFIRE_BEAR               = 80559,
    SPELL_DRUID_SOOTHE_BEAR                 = 80562,
    SPELL_DRUID_REMOVE_CORRUPTION_BEAR      = 80563,
    SPELL_DRUID_BRISTLING_FUR_PROC          = 80565,
    SPELL_DRUID_THRASH_BEAR                 = 80583,
    SPELL_DRUID_GUARDIAN_HEALTH             = 80569,
    SPELL_DRUID_GUARDIAN_OF_URSOC           = 80568,
    SPELL_DRUID_BERSERK_BEAR                = 80566,
    SPELL_DRUID_BEAR_FORM                   = 5487,
    SPELL_DRUID_DIRE_BEAR_FORM              = 9634,
    SPELL_DRUID_RAGE_SLEEPER                = 80570,
    SPELL_DRUID_RAGE_SLEEPER_BUFFS          = 80571,
    SPELL_DRUID_RAGE_SLEEPER_HEAL           = 80572,
    SPELL_DRUID_CENARION_WARD_HEAL          = 80575,
    SPELL_DRUID_TREE_FORM                   = 33891,
    SPELL_DRUID_EFFLORESCENCE               = 80577,
    SPELL_DRUID_EFFLORESCENCE_AURA          = 80578,
    SPELL_DRUID_EFFLORESCENCE_HEAL          = 80579,
    SPELL_DRUID_LIFEBLOOM_HOT               = 48451,
    SPELL_DRUID_REJUVENATION                = 48441,
    SPELL_DRUID_WILD_GROWTH                 = 53251,
    SPELL_DRUID_REGROWTH                    = 48443,
    SPELL_DRUID_NATURES_CURE                = 80573,
    SPELL_DRUID_YSERAS_GIFT                 = 80584,
    SPELL_DRUID_YSERAS_GIFT_SELF_HEAL       = 80585,
    SPELL_DRUID_YSERAS_GIFT_ALLY_HEAL       = 80586,
    SPELL_DRUID_FLOURISH                    = 80587,
    SPELL_DRUID_STELLAR_INNERVATION_R1      = 48516,
    SPELL_DRUID_STELLAR_INNERVATION_R2      = 48521,
    SPELL_DRUID_STELLAR_INNERVATION_R3      = 48525,
    SPELL_DRUID_POUNCING_STRIKES            = 80595,
    SPELL_DRUID_POUNCING_STRIKES_BUFF       = 80596,
    SPELL_DRUID_LEADER_OF_THE_PACK          = 17007,
    SPELL_DRUID_TASTE_FOR_BLOOD             = 80609,
    SPELL_DRUID_TASTE_FOR_BLOOD_PROC        = 80612,
    SPELL_DRUID_FAMILY_BEAR_HEAL            = 80665,
    SPELL_DRUID_FAMILY_BEAR_MANA            = 80664,
    SPELL_DRUID_FAMILY_BEAR_CD              = 80666,
    SPELL_DRUID_PACK_LEADER_HEAL            = 80613,
    SPELL_DRUID_PACK_LEADER_MANA            = 68285,
    SPELL_DRUID_PACK_LEADER_CD              = 80614,
    SPELL_DRUID_BRUTAL_SLASH                = 80507,
    SPELL_DRUID_SWIPE_CAT                   = 62078,
    SPELL_DRUID_NURTURING_PRESENCE_R1       = 80655,
    SPELL_DRUID_NURTURING_PRESENCE_R2       = 80656,
    SPELL_DRUID_MAUL                        = 48480,
    SPELL_DRUID_RAZE                        = 80520,

    // Rune Spell
    SPELL_DRUID_RADIANT_MOON_AURA           = 700910,
    SPELL_DRUID_APEX_PREDATORS_CRAVING_RUNE_BUFF = 701042,
    SPELL_DRUID_VERDANCY_EFFLORESCENCE_LISTENER = 701601,
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

class spell_dru_nurturing_presence : public AuraScript
{
    PrepareAuraScript(spell_dru_nurturing_presence);

    void AfterApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetTarget()->ToPlayer())
            player->addSpell(GetSpellInfo()->GetRank() == 1 ? SPELL_DRUID_NURTURING_PRESENCE_R1 : SPELL_DRUID_NURTURING_PRESENCE_R2, SPEC_MASK_ALL, false, true);
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetTarget()->ToPlayer())
            player->removeSpell(GetSpellInfo()->GetRank() == 1 ? SPELL_DRUID_NURTURING_PRESENCE_R1 : SPELL_DRUID_NURTURING_PRESENCE_R2, SPEC_MASK_ALL, true);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_nurturing_presence::AfterApply, EFFECT_0, SPELL_AURA_MOD_SPELL_HEALING_OF_STAT_PERCENT, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_nurturing_presence::AfterRemove, EFFECT_0, SPELL_AURA_MOD_SPELL_HEALING_OF_STAT_PERCENT, AURA_EFFECT_HANDLE_REAL);
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
            /*if (AuraEffect const* bramblesEff = owner->GetAuraEffect(SPELL_AURA_ADD_FLAT_MODIFIER, SPELLFAMILY_DRUID, 53, 2))
                AddPct(amount, bramblesEff->GetAmount());*/
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
        return ValidateSpellInfo({ SPELL_DRUID_KING_OF_THE_FOREST, SPELL_DRUID_ENRAGE_MOD_DAMAGE, SPELL_DRUID_ENRAGED_DEFENSE, SPELL_DRUID_ITEM_T10_FERAL_4P_BONUS });
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
        if (AuraEffect const* aurEff = target->GetAuraEffectOfRankedSpell(SPELL_DRUID_KING_OF_THE_FOREST, EFFECT_0))
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
        GetTarget()->CastCustomSpell(SPELL_DRUID_LIVING_SEED_PROC, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), TRIGGERED_FULL_MASK);
        
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
        GetTarget()->CastCustomSpell(SPELL_DRUID_LIVING_SEED_HEAL, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetTarget(), TRIGGERED_FULL_MASK);
        
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
class spell_dru_feral_tenacity : public AuraScript
{
    PrepareAuraScript(spell_dru_feral_tenacity);

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
        Unit* target = GetTarget();
        // reduces all damage taken while Stunned in Cat Form
        if (target->GetShapeshiftForm() == FORM_CAT || target->GetShapeshiftForm() == FORM_BEAR || target->GetShapeshiftForm() == FORM_DIREBEAR && GetTarget()->HasUnitFlag(UNIT_FLAG_STUNNED) && GetTarget()->HasAuraWithMechanic(1 << MECHANIC_STUN))
            absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_feral_tenacity::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_feral_tenacity::Absorb, EFFECT_1);
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
/*class spell_dru_tiger_s_fury : public SpellScript
{
    PrepareSpellScript(spell_dru_tiger_s_fury);

    void OnHit()
    {
        if (AuraEffect const* aurEff = GetHitUnit()->GetAuraEffectOfRankedSpell(SPELL_DRUID_KING_OF_THE_JUNGLE, EFFECT_0))
            GetHitUnit()->CastCustomSpell(SPELL_DRUID_TIGER_S_FURY_ENERGIZE, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetHitUnit(), true);
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_dru_tiger_s_fury::OnHit);
    }
};*/

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

    Aura* GetGroupGrowthRuneAura(Unit* caster)
    {
        for (size_t i = 701750; i < 701756; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::RaidCheck(GetCaster(), false));

        uint32 maxTargets = 5;
        maxTargets += GetCaster()->HasAura(SPELL_DRUID_GLYPH_OF_WILD_GROWTH) ? 1 : 0;

        if (GetGroupGrowthRuneAura(GetCaster()))
            maxTargets += GetGroupGrowthRuneAura(GetCaster())->GetEffect(EFFECT_1)->GetAmount();
            
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

class spell_dru_wild_growth_periodic : public AuraScript
{
    PrepareAuraScript(spell_dru_wild_growth_periodic);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Player* caster = GetCaster()->ToPlayer();

        int32 amount = aurEff->GetAmount();
        float drop = 20.0f;

        // Rune - Unstoppable Growth
        for (size_t i = 701756; i < 701760; i++)
        {
            if (AuraEffect* eff = caster->GetAuraEffect(i, 0))
                AddPct(drop, -eff->GetAmount());
        }

        int32 const finalAmount = amount * (1 - drop * 0.01f);
        GetAura()->GetEffect(EFFECT_0)->ChangeAmount(std::max(1, finalAmount));
    }

    void Register() override
    {        
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_wild_growth_periodic::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
    }
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
        Unit* caster = GetCaster();
        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(SPELL_DRUID_FEROCIOUS_BITE);
        int32 damageRatio = caster->GetComboPoints() * GetEffectValue();
        int32 damage = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);
        int32 energy = caster->GetPower(POWER_ENERGY);
        int32 consumption = spell->GetEffect(EFFECT_2).CalcValue(caster);

        if (caster->HasAura(SPELL_DRUID_APEX_PREDATORS_CRAVING_RUNE_BUFF))
        {
            int32 bonusDamage = consumption * (spell->GetEffect(EFFECT_1).CalcValue(caster));
            damage += int32(CalculatePct(damage, bonusDamage));
        }
        else if (energy > 0)
        {
            int32 bonusPercent = std::min<int32>(energy, consumption);
            int32 bonusDamage = bonusPercent * (spell->GetEffect(EFFECT_1).CalcValue(caster));
            damage += int32(CalculatePct(damage, bonusDamage));

            caster->ModifyPower(POWER_ENERGY, -bonusPercent);
        }

        if (Aura* runeAura = GetTasteForBloodRuneAura(caster))
        {
            int32 bleedQuantity = 0;
            auto targetAuras = GetExplTargetUnit()->GetAppliedAuras();
            for (auto itr = targetAuras.begin(); itr != targetAuras.end(); ++itr)
            {
                if (Aura* aura = itr->second->GetBase())
                {
                    if (caster->GetGUID() != aura->GetCasterGUID())
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
            damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
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

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (caster->HasAura(SPELL_DRUID_BERSERK_CAT))
            return;

        if (caster->HasAura(SPELL_DRUID_POUNCING_STRIKES))
            caster->AddAura(SPELL_DRUID_POUNCING_STRIKES_BUFF, caster);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (caster->HasAura(SPELL_DRUID_PROWL))
            caster->RemoveAura(SPELL_DRUID_PROWL);

        if (caster->HasAura(SPELL_DRUID_POUNCING_STRIKES))
            caster->RemoveAura(SPELL_DRUID_POUNCING_STRIKES_BUFF);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_prowl::HandleApply, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_prowl::HandleRemove, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
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
        if (caster->HasAura(SPELL_DRUID_STELLAR_INNERVATION_R1) && caster->HasAura(SPELL_DRUID_ECLIPSE_SOLAR_BUFF))
            astralPower += CalculatePct(astralPower, 33);
        else if (caster->HasAura(SPELL_DRUID_STELLAR_INNERVATION_R2) && caster->HasAura(SPELL_DRUID_ECLIPSE_SOLAR_BUFF))
            astralPower += CalculatePct(astralPower, 66);
        else if (caster->HasAura(SPELL_DRUID_STELLAR_INNERVATION_R3) && caster->HasAura(SPELL_DRUID_ECLIPSE_SOLAR_BUFF))
            astralPower += CalculatePct(astralPower, 99);
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
        if (caster->HasAura(SPELL_DRUID_STELLAR_INNERVATION_R1) && caster->HasAura(SPELL_DRUID_ECLIPSE_LUNAR_BUFF))
            astralPower += CalculatePct(astralPower, 33);
        else if (caster->HasAura(SPELL_DRUID_STELLAR_INNERVATION_R2) && caster->HasAura(SPELL_DRUID_ECLIPSE_LUNAR_BUFF))
            astralPower += CalculatePct(astralPower, 66);
        else if (caster->HasAura(SPELL_DRUID_STELLAR_INNERVATION_R3) && caster->HasAura(SPELL_DRUID_ECLIPSE_LUNAR_BUFF))
            astralPower += CalculatePct(astralPower, 99);
        caster->ModifyPower(POWER_RUNIC_POWER, astralPower);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_starfire::HandleCast);
    }
};

class spell_dru_nature_balance : public AuraScript
{
    PrepareAuraScript(spell_dru_nature_balance);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        uint32 amount = aurEff->GetAmount() * 10;
        uint32 maxAmount = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();


        if (caster->IsInCombat())
            caster->ModifyPower(POWER_RUNIC_POWER, amount, true);
        else
            if (caster->GetPower(POWER_RUNIC_POWER) <= maxAmount)
                caster->ModifyPower(POWER_RUNIC_POWER, amount, true);
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->SetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER + POWER_RUNIC_POWER, -10.f);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->SetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER + POWER_RUNIC_POWER, 3.f);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_nature_balance::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_nature_balance::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_nature_balance::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
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
        Unit* caster = GetCaster();

        caster->AddAura(SPELL_DRUID_BERSERK_CAT_CRIT, caster);

        if (caster->HasAura(SPELL_DRUID_POUNCING_STRIKES_BUFF))
            caster->RemoveAura(SPELL_DRUID_POUNCING_STRIKES_BUFF);
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

        if (caster->HasAura(SPELL_DRUID_BERSERK_CAT) || caster->HasAura(SPELL_DRUID_POUNCING_STRIKES_BUFF))
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

    void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
            return;

        GetCaster()->CastSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, TRIGGERED_FULL_MASK);
    }

    void HandleDispel(DispelInfo* dispelInfo)
    {
        GetCaster()->CastSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_new::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        AfterDispel += AuraDispelFn(spell_dru_lifebloom_new::HandleDispel);
    }
};

class spell_dru_swiftmend : public SpellScript
{
    PrepareSpellScript(spell_dru_swiftmend);

    Aura* GetVerdantInfusionRuneAura(Unit* caster)
    {
        for (size_t i = 701672; i < 701678; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

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
        if (GetVerdantInfusionRuneAura(GetCaster()))
            return;

        AuraEffect* aura = GetAuraEffect();

        if (aura) {
            GetExplTargetUnit()->RemoveAura(aura->GetId(), aura->GetCasterGUID());
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_swiftmend::HandleCast);
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
        Unit* caster = GetCaster(); 
        Aura* improvedAura = caster->GetAura(SPELL_DRUID_AVATAR_OF_ASHAMANE);
        int32 duration = improvedAura->GetDuration();

        caster->CastSpell(caster, SPELL_DRUID_CAT_FORM, TRIGGERED_FULL_MASK);

        caster->CastSpell(caster, SPELL_DRUID_BERSERK_CAT, TRIGGERED_FULL_MASK);
        Aura* catAura = caster->GetAura(SPELL_DRUID_BERSERK_CAT);
        catAura->SetDuration(duration);             
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_avatar_of_ashamane::HandleApply, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_primal_wrath : public SpellScript
{
    PrepareSpellScript(spell_dru_primal_wrath);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;


        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_PRIMAL_WRATH);
        uint32 durationAmount = value->GetEffect(EFFECT_1).CalcValue(caster);

        uint8 comboPoint = caster->ToPlayer()->GetComboPoints();
        int32 duration = durationAmount + (durationAmount * comboPoint);

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead())
                continue;

            caster->AddAura(SPELL_DRUID_RIP_DOT, target);
            target->GetAura(SPELL_DRUID_RIP_DOT)->SetDuration(duration);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_primal_wrath::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class spell_dru_feline_adept : public AuraScript
{
    PrepareAuraScript(spell_dru_feline_adept);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();
        target->learnSpell(SPELL_DRUID_MOONFIRE_CAT);
        target->learnSpell(SPELL_DRUID_SOOTHE_CAT);
        target->learnSpell(SPELL_DRUID_REMOVE_CORRUPTION_CAT);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();
        target->removeSpell(SPELL_DRUID_MOONFIRE_CAT, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_DRUID_SOOTHE_CAT, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_DRUID_REMOVE_CORRUPTION_CAT, SPEC_MASK_ALL, false);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_feline_adept::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_feline_adept::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_ironfur : public SpellScript
{
    PrepareSpellScript(spell_dru_ironfur);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_IRONFUR_BASE);
        uint32 armorPercent = value->GetEffect(EFFECT_0).CalcValue(caster);
        int32 increaseDuration = value->GetEffect(EFFECT_1).CalcValue(caster);
        int32 armor = CalculatePct(caster->GetStat(STAT_AGILITY), armorPercent);

        if (Aura* aura = caster->GetAura(SPELL_DRUID_IRONFUR_ARMOR))
        {
            if (caster->HasAura(SPELL_DRUID_URSINE_ADEPT))
            {
                int32 remainingDuration = aura->GetDuration();
                caster->CastCustomSpell(SPELL_DRUID_IRONFUR_ARMOR, SPELLVALUE_BASE_POINT0, armor, caster, TRIGGERED_FULL_MASK);
                aura->SetDuration(remainingDuration + increaseDuration);
            }
            else
            {
                caster->RemoveAura(SPELL_DRUID_IRONFUR_ARMOR);
                caster->CastCustomSpell(SPELL_DRUID_IRONFUR_ARMOR, SPELLVALUE_BASE_POINT0, armor, caster, TRIGGERED_FULL_MASK);
            }
        }
        else
        {
            caster->CastCustomSpell(SPELL_DRUID_IRONFUR_ARMOR, SPELLVALUE_BASE_POINT0, armor, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnCast += SpellCastFn(spell_dru_ironfur::HandleCast);
    }
};

class spell_dru_rip_new : public SpellScript
{
    PrepareSpellScript(spell_dru_rip_new);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        if (!caster || !caster->IsAlive())
            return;

        Unit* target = GetExplTargetUnit();
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_RIP);
        uint32 durationIncrease = value->GetEffect(EFFECT_0).CalcValue(caster);
        uint8 comboPoints = caster->ToPlayer()->GetComboPoints();
        int32 duration = durationIncrease + (durationIncrease * comboPoints);

        caster->CastSpell(target, SPELL_DRUID_RIP_DOT, TRIGGERED_FULL_MASK);
        target->GetAura(SPELL_DRUID_RIP_DOT)->SetDuration(duration);
    }

    void Register()
    {
        OnCast += SpellCastFn(spell_dru_rip_new::HandleCast);
    }
};

class spell_dru_ursine_adept : public AuraScript
{
    PrepareAuraScript(spell_dru_ursine_adept);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();
        target->learnSpell(SPELL_DRUID_MOONFIRE_BEAR);
        target->learnSpell(SPELL_DRUID_SOOTHE_BEAR);
        target->learnSpell(SPELL_DRUID_REMOVE_CORRUPTION_BEAR);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();
        target->removeSpell(SPELL_DRUID_MOONFIRE_BEAR, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_DRUID_SOOTHE_BEAR, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_DRUID_REMOVE_CORRUPTION_BEAR, SPEC_MASK_ALL, false);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_ursine_adept::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_ursine_adept::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_bristling_fur : public AuraScript
{
    PrepareAuraScript(spell_dru_bristling_fur);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (!caster || !caster->IsAlive())
            return;

        if (!eventInfo.GetDamageInfo() || !eventInfo.GetDamageInfo()->GetDamage())
            return;
        
        int32 damageTaken = eventInfo.GetDamageInfo()->GetDamage();
        int32 baseRageAmount = aurEff->GetAmount();
        int32 healthAmount = CalculatePct(caster->GetMaxHealth(), 1);
        int32 rageAmount = (damageTaken / healthAmount) * baseRageAmount;

        if (damageTaken > healthAmount)
            caster->CastCustomSpell(SPELL_DRUID_BRISTLING_FUR_PROC, SPELLVALUE_BASE_POINT0, rageAmount, caster, TRIGGERED_FULL_MASK);

        if (AuraEffect const* kingForest = caster->GetAuraEffectOfRankedSpell(SPELL_DRUID_KING_OF_THE_FOREST, EFFECT_0))
        {
            caster->CastCustomSpell(SPELL_DRUID_ENRAGE_MOD_DAMAGE, SPELLVALUE_BASE_POINT0, kingForest->GetAmount(), caster, true);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_bristling_fur::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dru_pulverize : public SpellScript
{
    PrepareSpellScript(spell_dru_pulverize);

    SpellCastResult CheckCast()
    {
        Unit* target = GetExplTargetUnit();
        if (!target || !target->IsAlive())
            return SPELL_FAILED_SPELL_UNAVAILABLE;

        if (!target->HasAura(SPELL_DRUID_THRASH_BEAR) || target->GetAura(SPELL_DRUID_THRASH_BEAR)->GetStackAmount() < 2)
            return SPELL_FAILED_SPELL_UNAVAILABLE;

        return SPELL_CAST_OK;
    }

    void HandleCast()
    {    
        Unit* target = GetExplTargetUnit();

        Aura* aura = target->GetAura(SPELL_DRUID_THRASH_BEAR);
        int32 stackAmount = aura->GetStackAmount();

        if (stackAmount == 2)
            target->RemoveAura(SPELL_DRUID_THRASH_BEAR);
        else
            GetCaster()->SetAuraStack(SPELL_DRUID_THRASH_BEAR, target, stackAmount += -2);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dru_pulverize::CheckCast);
        OnCast += SpellCastFn(spell_dru_pulverize::HandleCast);
    }
};

class spell_dru_guardian_of_ursoc_health : public SpellScript
{
    PrepareSpellScript(spell_dru_guardian_of_ursoc_health);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_GUARDIAN_OF_URSOC);
        uint32 healthIncrease = value->GetEffect(EFFECT_0).CalcValue(caster);
        int32 bp0 = caster->CountPctFromMaxHealth(healthIncrease);
        caster->CastCustomSpell(caster, SPELL_DRUID_GUARDIAN_HEALTH, &bp0, nullptr, nullptr, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_guardian_of_ursoc_health::HandleCast);
    }
};

class spell_dru_guardian_of_ursoc : public AuraScript
{
    PrepareAuraScript(spell_dru_guardian_of_ursoc);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        Aura* improvedAura = caster->GetAura(SPELL_DRUID_GUARDIAN_OF_URSOC);
        int32 duration = improvedAura->GetDuration();

        if (!caster->HasAura(SPELL_DRUID_BEAR_FORM) || !caster->HasAura(SPELL_DRUID_DIRE_BEAR_FORM))
        {
            if (caster->HasSpell(SPELL_DRUID_DIRE_BEAR_FORM))
                caster->CastSpell(caster, SPELL_DRUID_DIRE_BEAR_FORM, TRIGGERED_FULL_MASK);
            else
                caster->CastSpell(caster, SPELL_DRUID_BEAR_FORM, TRIGGERED_FULL_MASK);
        }

        caster->CastSpell(caster, SPELL_DRUID_BERSERK_BEAR, TRIGGERED_FULL_MASK);
        Aura* catAura = caster->GetAura(SPELL_DRUID_BERSERK_BEAR);
        catAura->SetDuration(duration);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_guardian_of_ursoc::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_rage_sleeper : public AuraScript
{
    PrepareAuraScript(spell_dru_rage_sleeper);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, SPELL_DRUID_RAGE_SLEEPER_BUFFS, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        caster->RemoveAura(SPELL_DRUID_RAGE_SLEEPER_BUFFS);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_rage_sleeper::HandleApply, EFFECT_0, SPELL_AURA_DAMAGE_SHIELD, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_rage_sleeper::HandleRemove, EFFECT_0, SPELL_AURA_DAMAGE_SHIELD, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_rage_sleeper_leech : public AuraScript
{
    PrepareAuraScript(spell_dru_rage_sleeper_leech);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage)
            {
                int32 healPct = aurEff->GetAmount();
                int32 healAmount = CalculatePct(damage, healPct);
                GetCaster()->CastCustomSpell(SPELL_DRUID_RAGE_SLEEPER_HEAL, SPELLVALUE_BASE_POINT0, healAmount, GetCaster(), TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_rage_sleeper_leech::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dru_cenarion_ward : public AuraScript
{
    PrepareAuraScript(spell_dru_cenarion_ward);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = GetTarget();
        target->CastSpell(target, SPELL_DRUID_CENARION_WARD_HEAL, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_cenarion_ward::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dru_tree_of_life : public AuraScript
{
    PrepareAuraScript(spell_dru_tree_of_life);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, SPELL_DRUID_TREE_FORM, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        caster->RemoveAura(SPELL_DRUID_TREE_FORM);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_tree_of_life::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_tree_of_life::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_efflorescence : public SpellScript
{
    PrepareSpellScript(spell_dru_efflorescence);

    void HandleSummon(SpellEffIndex effIndex)
    { 

        WorldLocation const* dest = GetHitDest();
        Position pos = dest->GetPosition();

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_EFFLORESCENCE);
        uint32 duration = value->GetDuration();

        std::vector<Unit*> summonedUnits = GetCaster()->ToPlayer()->GetSummonedUnits();

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (unit->HasAura(SPELL_DRUID_EFFLORESCENCE_AURA))
                unit->ToCreature()->DespawnOrUnsummon();
        }
       
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = GetCaster()->SummonCreature(500507, pos, TEMPSUMMON_TIMED_DESPAWN, duration, 0, properties);

        if (!summon)
            return;

        summon->SetOwnerGUID(GetCaster()->GetGUID());
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        summon->SetReactState(REACT_PASSIVE);

        summon->AddAura(SPELL_DRUID_EFFLORESCENCE_AURA, summon);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_dru_efflorescence::HandleSummon, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_dru_efflorescence_trigger : public AuraScript
{
    PrepareAuraScript(spell_dru_efflorescence_trigger);

    void OnPeriodic(AuraEffect const* /*aurEff*/)
    {
        Unit* master = GetUnitOwner()->GetOwner();

        master->CastSpell(GetCaster(), SPELL_DRUID_EFFLORESCENCE_HEAL, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_efflorescence_trigger::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_dru_efflorescence_target_select : public SpellScript
{
    PrepareSpellScript(spell_dru_efflorescence_target_select);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_EFFLORESCENCE);
        uint32 maxTargets = value->GetEffect(EFFECT_0).CalcValue(caster);

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead())
                continue;

            caster->AddAura(SPELL_DRUID_VERDANCY_EFFLORESCENCE_LISTENER, target);
        }

        if (targets.size() > maxTargets)
        {
            targets.sort(Acore::HealthPctOrderPred());
            targets.resize(maxTargets);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_efflorescence_target_select::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

class spell_dru_overgrowth : public SpellScript
{
    PrepareSpellScript(spell_dru_overgrowth);

    void HandleCast()
    {
        Unit* target = GetExplTargetUnit();
        Unit* caster = GetCaster();

        caster->CastSpell(target, SPELL_DRUID_LIFEBLOOM_HOT, TRIGGERED_FULL_MASK);
        caster->AddAura(SPELL_DRUID_REJUVENATION, target);
        caster->AddAura(SPELL_DRUID_WILD_GROWTH, target);
        caster->AddAura(SPELL_DRUID_REGROWTH, target);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_overgrowth::HandleCast);
    }
};

class spell_dru_dryad_adept : public AuraScript
{
    PrepareAuraScript(spell_dru_dryad_adept);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();
        target->learnSpell(SPELL_DRUID_NATURES_CURE);
        target->learnSpell(SPELL_DRUID_YSERAS_GIFT);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();
        target->removeSpell(SPELL_DRUID_NATURES_CURE, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_DRUID_YSERAS_GIFT, SPEC_MASK_ALL, false);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_dryad_adept::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_dryad_adept::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_druid_yseras_gift : public AuraScript
{
    PrepareAuraScript(spell_druid_yseras_gift);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        int32 procPct = caster->GetAura(SPELL_DRUID_YSERAS_GIFT)->GetEffect(EFFECT_0)->GetAmount();
        int32 amount = int32(CalculatePct(GetCaster()->GetMaxHealth(), procPct));

        if (caster->GetHealth() < caster->GetMaxHealth())
            caster->CastCustomSpell(SPELL_DRUID_YSERAS_GIFT_SELF_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        else
            caster->CastCustomSpell(SPELL_DRUID_YSERAS_GIFT_ALLY_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_druid_yseras_gift::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_druid_yseras_gift_target : public SpellScript
{
    PrepareSpellScript(spell_druid_yseras_gift_target);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (!GetCaster()->ToPlayer()->GetGroup())
            return;

        uint32 const maxTargets = 1;

        if (targets.size() > maxTargets)
        {
            targets.sort(Acore::HealthPctOrderPred());
            targets.resize(maxTargets);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_druid_yseras_gift_target::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

class spell_dru_flourish : public SpellScript
{
    PrepareSpellScript(spell_dru_flourish);

    std::list <Unit*> FindTargets()
    {
        Player* caster = GetCaster()->ToPlayer();
        std::list <Unit*> targetAvailable;
        Position casterPos = caster->GetPosition();
        auto const& allyList = caster->GetGroup()->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* player = ObjectAccessor::FindPlayer(target.guid);
            if (player)
                if (!player->isDead())
                {
                    Unit* dummy = player->ToUnit();
                    float distance = player->GetDistance(casterPos);
                    if (dummy && distance <= 60)
                        targetAvailable.push_back(dummy);
                }
        }
        return targetAvailable;
    }

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (!caster->GetGroup())
            return;

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DRUID_FLOURISH);
        uint32 durationIncrease = value->GetEffect(EFFECT_0).CalcValue(caster);

        for (auto const& target : FindTargets())
        {
            auto targetAuras = target->GetAppliedAuras();
            for (auto itr = targetAuras.begin(); itr != targetAuras.end(); ++itr)
            {
                if (Aura* aura = itr->second->GetBase())
                {
                    if (caster->GetGUID() != aura->GetCasterGUID())
                        continue;

                    SpellInfo const* auraInfo = aura->GetSpellInfo();

                    if (auraInfo->SpellFamilyFlags[1] & 0x00400000 && auraInfo->SpellFamilyName == SPELLFAMILY_DRUID)
                    {
                        int32 currentDuration = aura->GetDuration();
                        aura->SetDuration(currentDuration += durationIncrease);
                        aura->GetEffect(EFFECT_0)->ResetTicks();
                        aura->GetEffect(EFFECT_0)->CalculatePeriodic(caster);
                    }
                }
            }
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dru_flourish::HandleCast);
    }
};

class spell_dru_moonglow : public AuraScript
{
    PrepareAuraScript(spell_dru_moonglow);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (!caster || !caster->IsAlive())
            return;

        if (!caster->HasAura(SPELL_DRUID_MOONKIN_FORM))
            return;

        int32 powerAmount = aurEff->GetAmount();
        caster->ModifyPower(POWER_RUNIC_POWER, powerAmount);     
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_moonglow::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dru_pack_leader : public AuraScript
{
    PrepareAuraScript(spell_dru_pack_leader);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (!caster || !caster->IsAlive())
            return;

        int32 ap = caster->GetTotalAttackPowerValue(BASE_ATTACK);
        int32 healAmount = aurEff->GetAmount();
        int32 heal = CalculatePct(ap, healAmount);
        int32 mana = CalculatePct(caster->GetMaxPower(Powers(POWER_MANA)), healAmount * 2);

        if (!caster->HasAura(SPELL_DRUID_PACK_LEADER_CD))
        {
            caster->CastSpell(caster, SPELL_DRUID_PACK_LEADER_HEAL, true);
            caster->CastCustomSpell(SPELL_DRUID_PACK_LEADER_MANA, SPELLVALUE_BASE_POINT0, mana, caster, TRIGGERED_IGNORE_GCD);
            caster->CastSpell(caster, SPELL_DRUID_PACK_LEADER_CD, true);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_pack_leader::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dru_family_bear : public AuraScript
{
    PrepareAuraScript(spell_dru_family_bear);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (!caster || !caster->IsAlive())
            return;

        int32 maxHealth = caster->GetMaxHealth();
        int32 healAmount = aurEff->GetAmount();
        int32 heal = CalculatePct(maxHealth, healAmount);
        int32 mana = CalculatePct(caster->GetMaxPower(Powers(POWER_MANA)), healAmount * 2);

        if (!caster->HasAura(SPELL_DRUID_FAMILY_BEAR_CD))
        {
            caster->CastSpell(caster, SPELL_DRUID_FAMILY_BEAR_HEAL, true);
            caster->CastCustomSpell(SPELL_DRUID_FAMILY_BEAR_MANA, SPELLVALUE_BASE_POINT0, mana, caster, TRIGGERED_IGNORE_GCD);
            caster->CastSpell(caster, SPELL_DRUID_FAMILY_BEAR_CD, true);
        }          
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_family_bear::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dru_feral_frenzy : public SpellScript
{
    PrepareSpellScript(spell_dru_feral_frenzy);

    void HandleHit()
    {
        Unit* caster = GetCaster();

        if (!caster || !caster->IsAlive())
            return;

        if (AuraEffect const* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_DRUID_TASTE_FOR_BLOOD, EFFECT_0))
        {
            int32 amount = aurEff->GetAmount();
            caster->CastCustomSpell(SPELL_DRUID_TASTE_FOR_BLOOD_PROC, SPELLVALUE_BASE_POINT0, amount, GetExplTargetUnit(), true);
        }        
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dru_feral_frenzy::HandleHit);
    }
};

class spell_dru_brutal_slash : public AuraScript
{
    PrepareAuraScript(spell_dru_brutal_slash);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();
        target->removeSpell(SPELL_DRUID_SWIPE_CAT, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_DRUID_BRUTAL_SLASH);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();
        target->removeSpell(SPELL_DRUID_BRUTAL_SLASH, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_DRUID_SWIPE_CAT);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_brutal_slash::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_brutal_slash::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_raze : public AuraScript
{
    PrepareAuraScript(spell_dru_raze);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_DRUID_MAUL, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_DRUID_RAZE);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_DRUID_RAZE, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_DRUID_MAUL);        
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_raze::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_raze::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dru_replacer_ursoc : public AuraScript
{
    PrepareAuraScript(spell_dru_replacer_ursoc);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_DRUID_BERSERK_BEAR, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_DRUID_GUARDIAN_OF_URSOC);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_DRUID_GUARDIAN_OF_URSOC, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_DRUID_BERSERK_BEAR);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_replacer_ursoc::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_replacer_ursoc::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_druid_spell_scripts()
{
    RegisterSpellScript(spell_dru_bear_form_passive);
    RegisterSpellScript(spell_dru_t10_balance_4p_bonus);
    RegisterSpellScript(spell_dru_nurturing_instinct);
    RegisterSpellScript(spell_dru_feral_swiftness);
    RegisterSpellScript(spell_dru_omen_of_clarity);
    //RegisterSpellScript(spell_dru_brambles_treant);
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
    RegisterSpellScript(spell_dru_feral_tenacity);
    RegisterSpellScript(spell_dru_rip);
    RegisterSpellScript(spell_dru_savage_defense);
    RegisterSpellScript(spell_dru_savage_roar);
    //RegisterSpellAndAuraScriptPair(spell_dru_savage_roar, spell_dru_savage_roar_aura);
    //RegisterSpellScript(spell_dru_starfall_aoe);
    //RegisterSpellScript(spell_dru_starfall_dummy);
    RegisterSpellAndAuraScriptPair(spell_dru_survival_instincts, spell_dru_survival_instincts_aura);
    RegisterSpellScript(spell_dru_swift_flight_passive);
    //RegisterSpellScript(spell_dru_tiger_s_fury);
    RegisterSpellScript(spell_dru_typhoon);
    RegisterSpellScript(spell_dru_t10_restoration_4p_bonus);
    RegisterSpellScript(spell_dru_wild_growth);
    RegisterSpellScript(spell_dru_wild_growth_periodic);
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
    RegisterSpellScript(spell_dru_swiftmend);
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
    RegisterSpellScript(spell_dru_primal_wrath);
    RegisterSpellScript(spell_dru_feline_adept);
    RegisterSpellScript(spell_dru_ironfur);
    RegisterSpellScript(spell_dru_rip_new);
    RegisterSpellScript(spell_dru_ursine_adept);
    RegisterSpellScript(spell_dru_bristling_fur);
    RegisterSpellScript(spell_dru_pulverize);
    RegisterSpellScript(spell_dru_guardian_of_ursoc_health);
    RegisterSpellScript(spell_dru_guardian_of_ursoc);
    RegisterSpellScript(spell_dru_rage_sleeper);
    RegisterSpellScript(spell_dru_rage_sleeper_leech);
    RegisterSpellScript(spell_dru_cenarion_ward);
    RegisterSpellScript(spell_dru_tree_of_life);
    RegisterSpellScript(spell_dru_efflorescence);
    RegisterSpellScript(spell_dru_efflorescence_trigger);
    RegisterSpellScript(spell_dru_efflorescence_target_select);
    RegisterSpellScript(spell_dru_overgrowth);
    RegisterSpellScript(spell_dru_dryad_adept);
    RegisterSpellScript(spell_druid_yseras_gift);
    RegisterSpellScript(spell_druid_yseras_gift_target);
    RegisterSpellScript(spell_dru_flourish);
    RegisterSpellScript(spell_dru_nature_balance);
    RegisterSpellScript(spell_dru_moonglow);
    RegisterSpellScript(spell_dru_pack_leader);
    RegisterSpellScript(spell_dru_family_bear);
    RegisterSpellScript(spell_dru_feral_frenzy);
    RegisterSpellScript(spell_dru_brutal_slash);
    RegisterSpellScript(spell_dru_nurturing_presence);
    RegisterSpellScript(spell_dru_replacer_ursoc);
}
