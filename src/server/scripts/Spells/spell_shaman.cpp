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
  * Scripts for spells with SPELLFAMILY_SHAMAN and SPELLFAMILY_GENERIC spells used by shaman players.
  * Ordered alphabetically using scriptname.
  * Scriptnames of files in this file should be prefixed with "spell_sha_".
  */

#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "Unit.h"

enum ShamanSpells
{
    SPELL_SHAMAN_GLYPH_OF_FERAL_SPIRIT = 63271,
    // Spells
    SPELL_SHAMAN_ANCESTRAL_AWAKENING_PROC = 52752,
    SPELL_SHAMAN_ANCESTRAL_GUIDANCE = 84010,
    SPELL_SHAMAN_ANCESTRAL_GUIDANCE_AOE = 84011,
    SPELL_SHAMAN_ANCESTRAL_GUIDANCE_HEAL = 84012,
    SPELL_SHAMAN_ANCESTRAL_PROTECTION_HEAL = 84081,
    SPELL_SHAMAN_ANCESTRAL_VISION = 84006,
    SPELL_SHAMAN_ASCENDANCE_FIRE = 84019,
    SPELL_SHAMAN_ASCENDANCE_WATER_AOE_HEAL = 84042,
    SPELL_SHAMAN_ASCENDANCE_WATER_HEAL = 84043,
    SPELL_SHAMAN_ASTRAL_SHIFT = 84007,
    SPELL_SHAMAN_BIND_SIGHT = 6277,
    SPELL_SHAMAN_BLESSING_OF_THE_ETERNALS_R1 = 51554,
    SPELL_SHAMAN_CAPACITOR_TOTEM = 84069,
    SPELL_SHAMAN_CHAIN_LIGHTNING = 49271,
    SPELL_SHAMAN_CHAIN_LIGHTNING_OVERLOAD = 84056,
    SPELL_SHAMAN_CLOUDBURST_TOTEM = 84082,
    SPELL_SHAMAN_CRASH_LIGHTNING_AURA = 84033,
    SPELL_SHAMAN_CRASH_LIGHTNING_DAMAGE = 84034,
    SPELL_SHAMAN_CLEANSING_TOTEM_EFFECT = 52025,
    SPELL_SHAMAN_DOWNPOUR = 84046,
    SPELL_SHAMAN_DOWNPOUR_LISTENER = 84059,
    SPELL_SHAMAN_EARTH_SHIELD_HEAL = 379,
    SPELL_SHAMAN_EARTHLIVING_WEAPON = 51994,
    SPELL_SHAMAN_ELEMENTAL_BLAST = 84022,
    SPELL_SHAMAN_ELEMENTAL_BLAST_CRIT = 84023,
    SPELL_SHAMAN_ELEMENTAL_BLAST_HASTE = 84024,
    SPELL_SHAMAN_ELEMENTAL_BLAST_MASTERY = 84025,
    SPELL_SHAMAN_ELEMENTAL_MASTERY = 16166,
    SPELL_SHAMAN_EXHAUSTION = 57723,
    SPELL_SHAMAN_FERAL_SPIRIT = 51533,
    SPELL_SHAMAN_FIRE_NOVA = 84000,
    SPELL_SHAMAN_FIRE_NOVA_DAMAGE = 84001,
    SPELL_SHAMAN_FLAME_SHOCK = 49233,
    SPELL_SHAMAN_FLAMETONGUE_WEAPON_AURA = 58792,
    SPELL_SHAMAN_FROST_SHOCK = 49236,
    SPELL_SHAMAN_GLYPH_OF_EARTH_SHIELD = 63279,
    SPELL_SHAMAN_GLYPH_OF_HEALING_STREAM_TOTEM = 55456,
    SPELL_SHAMAN_GLYPH_OF_MANA_TIDE = 55441,
    SPELL_SHAMAN_GLYPH_OF_THUNDERSTORM = 62132,
    SPELL_SHAMAN_GUST_OF_WIND = 84009,
    SPELL_SHAMAN_HEALING_STREAM_TOTEM_AURA = 58764,
    SPELL_SHAMAN_HEALING_SURGE = 84004,
    SPELL_SHAMAN_HEALING_TIDE_TOTEM = 84076,
    SPELL_SHAMAN_ICEFURY = 84016,
    SPELL_SHAMAN_ICEFURY_BUFF = 84017,
    SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD = 23552,
    SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD_DAMAGE = 27635,
    SPELL_SHAMAN_ITEM_MANA_SURGE = 23571,
    SPELL_SHAMAN_LAVA_BEAM = 84021,
    SPELL_SHAMAN_LAVA_BEAM_OVERLOAD = 84058,
    SPELL_SHAMAN_LAVA_BURST = 60043,
    SPELL_SHAMAN_LAVA_FLOWS_R1 = 51480,
    SPELL_SHAMAN_LAVA_FLOWS_TRIGGERED_R1 = 64694,
    SPELL_SHAMAN_LAVA_LASH = 60103,
    SPELL_SHAMAN_LIGHTNING_BOLT = 49238,
    SPELL_SHAMAN_LIGHTNING_BOLT_OVERLOAD = 84057,
    SPELL_SHAMAN_LIGHTNING_SHIELD = 49281,
    SPELL_SHAMAN_MANA_SPRING_TOTEM_ENERGIZE = 52032,
    SPELL_SHAMAN_MANA_TIDE_TOTEM = 16190,
    SPELL_SHAMAN_OUTBURST_HEAL = 84083,
    SPELL_SHAMAN_RIPTIDE = 61301,
    SPELL_SHAMAN_SATED = 57724,
    SPELL_SHAMAN_STORM_EARTH_AND_FIRE = 51483,
    SPELL_SHAMAN_STORMSTRIKE = 17364,
    SPELL_SHAMAN_STORMKEEPER = 84018,
    SPELL_SHAMAN_STORMKEEPER_LISTENER = 84054,
    SPELL_SHAMAN_TOTEM_EARTHBIND_EARTHGRAB = 64695,
    SPELL_SHAMAN_TOTEM_EARTHBIND_TOTEM = 6474,
    SPELL_SHAMAN_TOTEM_EARTHEN_POWER = 59566,
    SPELL_SHAMAN_TOTEM_HEALING_STREAM_HEAL = 52042,
    SPELL_SHAMAN_WINDFURY_WEAPON_AURA = 33757,

    // Mastery
    MASTERY_SHAMAN_ELEMENTAL_OVERLOAD = 1000000,
    MASTERY_SHAMAN_ELEMENTAL_OVERLOAD_BUFF = 1000001,

    // Spirit Master
    SPELL_SHAMAN_SPIRIT_OF_FIRE = 84089,
    SPELL_SHAMAN_SPIRIT_OF_FIRE_PASSIVE = 84136,
    SPELL_SHAMAN_SPIRIT_OF_STORM = 84090,
    SPELL_SHAMAN_SPIRIT_OF_STORM_HASTE = 84096,
    SPELL_SHAMAN_SPIRIT_OF_STORM_PASSIVE = 84137,
    SPELL_SHAMAN_SPIRIT_OF_EARTH = 84091,
    SPELL_SHAMAN_SPIRIT_OF_EARTH_PASSIVE = 84138,
    SPELL_SHAMAN_SPIRIT_OF_WATER = 84092,
    SPELL_SHAMAN_SPIRIT_OF_WATER_PASSIVE = 84139,
    SPELL_SHAMAN_SPIRIT_OF_STORM_PROC = 84095,
    SPELL_SHAMAN_SPIRIT_OF_WATER_SHIELD = 84098,
    SPELL_SHAMAN_SPIRIT_OF_WATER_REGEN = 84100,
    SPELL_SHAMAN_INVOKE_ESSENCE = 84102,
    SPELL_SHAMAN_INVOKE_ESSENCE_FIRE = 84103,
    SPELL_SHAMAN_INVOKE_ESSENCE_EARTH = 84104,
    SPELL_SHAMAN_INVOKE_ESSENCE_WATER = 84105,
    SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_FIRE = 84107,
    SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_STORM = 84108,
    SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_STORM_ECHO = 84109,
    SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_EARTH = 84110,
    SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_EARTH_GRIP = 84111,
    SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_WATER = 84112,
    SPELL_SHAMAN_FOCUS_THINE_FOE_STORM = 84117,
    SPELL_SHAMAN_FOCUS_THINE_FOE_EARTH = 84118,
    SPELL_SHAMAN_FOCUS_THINE_FOE_WATER = 84119,
    SPELL_SHAMAN_FIERY_STAMP_PROC = 84122,
    SPELL_SHAMAN_TECTONIC_SHIFT_DEBUFF = 84124,
    SPELL_SHAMAN_WATERY_GRAVE_HEAL = 84126,
    SPELL_SHAMAN_WATERY_GRAVE_BUBBLE = 84127,
    SPELL_SHAMAN_OVERCHARGE_FIRE = 84129,
    SPELL_SHAMAN_OVERCHARGE_STORM = 84130,
    SPELL_SHAMAN_OVERCHARGE_EARTH = 84131,
    SPELL_SHAMAN_OVERCHARGE_WATER = 84132,
    SPELL_SHAMAN_SEAMLESS_WATER_PROC = 84152,
    SPELL_SHAMAN_SEAMLESS_WATER_STACKS = 84153,
    SPELL_SHAMAN_WATER_BENDING_PROC = 84166,
    SPELL_SHAMAN_FIRE_PROFICIENCY_STACKS = 84170,
    SPELL_SHAMAN_FIRE_PROFICIENCY_PROC = 84171,
    SPELL_SHAMAN_REGENERATIVE_AURA_HEAL = 84208,
    SPELL_SHAMAN_ACCUMULATION_HEAL = 84224,
    SPELL_SHAMAN_SEEPING_LIFE_FORCE_HEAL = 84231,
    SPELL_SHAMAN_PATH_OF_THE_ASCENDANT = 84242,
    SPELL_SHAMAN_ASCENDANT_EARTH = 84113,
    SPELL_SHAMAN_ASCENDENCE_AIR = 84035,
    SPELL_SHAMAN_ASCENDENCE_WATER = 84040,
    SPELL_SHAMAN_STORMBRAND_TOTEM_PROC = 84245,
    SPELL_SHAMAN_SPIRIT_WEAPON_NATURE = 84247,
    SPELL_SHAMAN_SPIRIT_WEAPON_FIRE = 84248,
    SPELL_SHAMAN_SPIRIT_WEAPON_FROST = 84249,

    // Passive
    SPELL_SHAMAN_MAELSTROM_WEAPON = 84053,

    // Talents
    TALENT_SHAMAN_SEAMLESS_WATER = 84149,
    TALENT_SHAMAN_TIDAL_WAVES_BUFF = 53390,
    TALENT_SHAMAN_STORM_PROFICIENCY_PROC = 84175,

    // Runes
    RUNE_SHAMAN_GUARDIANS_CUDGEL_DEBUFF = 1000428,
    RUNE_SHAMAN_IMPROVED_EARTHLIVING_WEAPON_HEAL = 1000454,
    RUNE_SHAMAN_LEGACY_OF_THE_FROST_WITCH_LISTENER = 1000816,
    RUNE_SHAMAN_DEADLY_STAMP_LISTENER = 1001166,
    RUNE_SHAMAN_PROPAGATING_FIRE_LISTENER = 1001192,
    RUNE_SHAMAN_PROPAGATING_FIRE_DAMAGE = 1001193,
    RUNE_SHAMAN_LINGERING_STORM_BUFF = 1001310,
    RUNE_SHAMAN_EARTHS_GRASP_SLOW = 1001328,

    // Summons
    SUMMON_SHAMAN_EARTH_ELEMENTAL = 15352,
    SUMMON_SHAMAN_FIRE_ELEMENTAL = 15438,
    SUMMON_SHAMAN_STORM_ELEMENTAL = 400408,
    SUMMON_SHAMAN_FERAL_SPIRITS = 29264,

    // Sets
    T1_SHAMAN_ELEM_2PC = 99000,
    T1_SHAMAN_ELEM_2PC_BUFF = 99001,
    T1_SHAMAN_ELEM_2PC_EARTHQUAKEBUFF = 99002,
    T1_SHAMAN_ENH_4PC = 99102,
    T1_SHAMAN_ENH_4PC_BUFF = 99103,
    T1_SHAMAN_SPIRIT_2PC = 99300,
    T1_SHAMAN_SPIRIT_2PC_BUFF_FIRE = 99301,
    T1_SHAMAN_SPIRIT_2PC_BUFF_STORM = 99302,
    T1_SHAMAN_SPIRIT_2PC_BUFF_EARTH = 99303,
    T1_SHAMAN_SPIRIT_2PC_BUFF_WATER = 99304,
    T1_SHAMAN_SPIRIT_4PC = 99305,
    T1_SHAMAN_SPIRIT_4PC_BUFF_FIRE = 99306,
    T1_SHAMAN_SPIRIT_4PC_BUFF_STORM = 99307,
    T1_SHAMAN_SPIRIT_4PC_BUFF_EARTH = 99308,
    T1_SHAMAN_SPIRIT_4PC_BUFF_WATER = 99309,
};

enum ShamanSpellIcons
{
    SHAMAN_ICON_ID_RESTORATIVE_TOTEMS = 338,
    SHAMAN_ICON_ID_SHAMAN_LAVA_FLOW = 3087
};

class spell_sha_totem_of_wrath : public SpellScript
{
    PrepareSpellScript(spell_sha_totem_of_wrath);

    void HandleAfterCast()
    {
        if (AuraEffect* aurEff = GetCaster()->GetAuraEffect(63280, EFFECT_0))
            if (Creature* totem = GetCaster()->GetMap()->GetCreature(GetCaster()->m_SummonSlot[1]))   // Fire totem summon slot
                if (SpellInfo const* totemSpell = sSpellMgr->GetSpellInfo(totem->m_spells[0]))
                {
                    int32 bp0 = CalculatePct(totemSpell->Effects[EFFECT_0].CalcValue(), aurEff->GetAmount());
                    int32 bp1 = CalculatePct(totemSpell->Effects[EFFECT_1].CalcValue(), aurEff->GetAmount());
                    GetCaster()->CastCustomSpell(GetCaster(), 63283, &bp0, &bp1, nullptr, true, nullptr, aurEff);
                }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_sha_totem_of_wrath::HandleAfterCast);
    }
};

class spell_sha_spirit_walk : public SpellScript
{
    PrepareSpellScript(spell_sha_spirit_walk);

    SpellCastResult CheckCast()
    {
        if (Unit* owner = GetCaster()->GetOwner())
            if (GetCaster()->IsWithinDist(owner, GetSpellInfo()->GetMaxRange(GetSpellInfo()->IsPositive())))
                return SPELL_CAST_OK;

        return SPELL_FAILED_OUT_OF_RANGE;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_sha_spirit_walk::CheckCast);
    }
};

class spell_sha_maelstrom_on_cast : public SpellScript
{
    PrepareSpellScript(spell_sha_maelstrom_on_cast);

    void HandleProc()
    {
        Unit* caster = GetCaster();
        int32 maelstromAmount = GetSpellInfo()->GetEffect(EFFECT_2).CalcValue(caster);

        if (GetSpellInfo()->Id == SPELL_SHAMAN_FROST_SHOCK)
            if (Aura* icefury = caster->GetAura(SPELL_SHAMAN_ICEFURY_BUFF))
                maelstromAmount = icefury->GetEffect(EFFECT_1)->GetAmount();

        if (GetSpellInfo()->Id == SPELL_SHAMAN_LIGHTNING_BOLT)
            if (Aura* stormkeeper = caster->GetAura(SPELL_SHAMAN_STORMKEEPER_LISTENER))
                stormkeeper->ModStackAmount(-1);

        caster->ModifyPower(POWER_RUNIC_POWER, maelstromAmount, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_sha_maelstrom_on_cast::HandleProc);
    }
};

class spell_sha_t10_restoration_4p_bonus : public AuraScript
{
    PrepareAuraScript(spell_sha_t10_restoration_4p_bonus);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActor() && eventInfo.GetProcTarget();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        uint32 triggered_spell_id = 70809;
        SpellInfo const* triggeredSpell = sSpellMgr->GetSpellInfo(triggered_spell_id);

        HealInfo* healInfo = eventInfo.GetHealInfo();

        if (!healInfo || !triggeredSpell)
        {
            return;
        }

        int32 amount = CalculatePct(healInfo->GetHeal(), aurEff->GetAmount()) / triggeredSpell->GetMaxTicks();
        eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(GetTarget(), triggered_spell_id, SPELL_AURA_PERIODIC_HEAL, amount, EFFECT_0);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_t10_restoration_4p_bonus::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_t10_restoration_4p_bonus::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sha_totemic_mastery : public AuraScript
{
    PrepareAuraScript(spell_sha_totemic_mastery);

    void HandlePeriodic(AuraEffect const*  /*aurEff*/)
    {
        PreventDefaultAction();

        for (uint8 i = SUMMON_SLOT_TOTEM; i < MAX_TOTEM_SLOT; ++i)
            if (!GetTarget()->m_SummonSlot[i])
                return;

        GetTarget()->CastSpell(GetTarget(), 38437, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_totemic_mastery::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// 51533 - Feral Spirit
class spell_sha_feral_spirit : public SpellScript
{
    PrepareSpellScript(spell_sha_feral_spirit);

    Aura* GetElementalSpiritsAura(Unit* caster)
    {
        for (size_t i = 1000862; i < 1000868; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 duration = caster->CalcSpellDuration(GetSpellInfo());

        if (GetElementalSpiritsAura(caster))
        {
            for (Unit::ControlSet::iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
            {
                if ((*itr)->GetEntry() == SUMMON_SHAMAN_FERAL_SPIRITS && (*itr)->IsAlive())
                {
                    int32 rand = urand(1, 3);
                    SpellEffIndex spellEffect;

                    // Rand for element type, 1 = Fire, 2 = Frost and 3 = Lightning
                    if (rand == 1)
                        spellEffect = EFFECT_0;
                    else if (rand == 2)
                        spellEffect = EFFECT_1;
                    else if (rand == 3)
                        spellEffect = EFFECT_2;

                    if (!spellEffect)
                        continue;

                    int32 buff1 = GetElementalSpiritsAura(caster)->GetEffect(spellEffect)->GetAmount();
                    int32 buff2 = GetElementalSpiritsAura(caster)->GetSpellInfo()->GetEffect(spellEffect).TriggerSpell;

                    caster->CastCustomSpell(buff1, SPELLVALUE_AURA_DURATION, duration, (*itr), TRIGGERED_FULL_MASK);
                    caster->CastCustomSpell(buff2, SPELLVALUE_AURA_DURATION, duration, (*itr), TRIGGERED_FULL_MASK);

                    // need to add the morphs
                }
            }
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_sha_feral_spirit::HandleProc);
    }
};

class spell_sha_feral_spirit_scaling : public AuraScript
{
    PrepareAuraScript(spell_sha_feral_spirit_scaling);

    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: feral spirit inherits 40% of resistance from owner and 35% of armor
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            SpellSchoolMask schoolMask = SpellSchoolMask(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            int32 modifier = schoolMask == SPELL_SCHOOL_MASK_NORMAL ? 35 : 40;
            amount = CalculatePct(std::max<int32>(0, owner->GetResistance(schoolMask)), modifier);
        }
    }

    void CalculateStatAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: by default feral spirit inherits 30% of stamina
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            Stats stat = Stats(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            amount = CalculatePct(std::max<int32>(0, owner->GetStat(stat)), 30);
        }
    }

    void CalculateAPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: by default feral spirit inherits 30% of AP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 modifier = 30;
            if (AuraEffect const* gofsEff = owner->GetAuraEffect(SPELL_SHAMAN_GLYPH_OF_FERAL_SPIRIT, EFFECT_0))
                modifier += gofsEff->GetAmount();

            amount = CalculatePct(std::max<int32>(0, owner->GetTotalAttackPowerValue(BASE_ATTACK)), modifier);
        }
    }

    void CalculateSPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: by default feral spirit inherits 30% of AP as SP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 modifier = 30;
            if (AuraEffect const* gofsEff = owner->GetAuraEffect(SPELL_SHAMAN_GLYPH_OF_FERAL_SPIRIT, EFFECT_0))
                modifier += gofsEff->GetAmount();

            amount = CalculatePct(std::max<int32>(0, owner->GetTotalAttackPowerValue(BASE_ATTACK)), modifier);

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

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        isPeriodic = true;
        amplitude = 1 * IN_MILLISECONDS;
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
        if (m_scriptSpellId == 35675)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_feral_spirit_scaling::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);

        if (m_scriptSpellId == 35674)
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_feral_spirit_scaling::CalculateStatAmount, EFFECT_ALL, SPELL_AURA_MOD_STAT);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_feral_spirit_scaling::CalculateAPAmount, EFFECT_ALL, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_feral_spirit_scaling::CalculateSPAmount, EFFECT_ALL, SPELL_AURA_MOD_DAMAGE_DONE);
        }

        OnEffectApply += AuraEffectApplyFn(spell_sha_feral_spirit_scaling::HandleEffectApply, EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_sha_feral_spirit_scaling::CalcPeriodic, EFFECT_ALL, SPELL_AURA_ANY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_feral_spirit_scaling::HandlePeriodic, EFFECT_ALL, SPELL_AURA_ANY);
    }
};

class spell_sha_fire_elemental_scaling : public AuraScript
{
    PrepareAuraScript(spell_sha_fire_elemental_scaling);

    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: fire elemental inherits 40% of resistance from owner and 35% of armor
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            SpellSchoolMask schoolMask = SpellSchoolMask(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            int32 modifier = schoolMask == SPELL_SCHOOL_MASK_NORMAL ? 35 : 40;
            amount = CalculatePct(std::max<int32>(0, owner->GetResistance(schoolMask)), modifier);
        }
    }

    void CalculateStatAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: fire elemental inherits 30% of intellect / stamina
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            Stats stat = Stats(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            amount = CalculatePct(std::max<int32>(0, owner->GetStat(stat)), 30);
        }
    }

    void CalculateAPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: fire elemental inherits 300% / 150% of SP as AP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 fire = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
            amount = CalculatePct(std::max<int32>(0, fire), (GetUnitOwner()->GetEntry() == NPC_FIRE_ELEMENTAL ? 300 : 150));
        }
    }

    void CalculateSPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: fire elemental inherits 100% of SP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 fire = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
            amount = CalculatePct(std::max<int32>(0, fire), 100);

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
        if (m_scriptSpellId != 35665 && m_scriptSpellId != 65225)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_fire_elemental_scaling::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);

        if (m_scriptSpellId == 35666 || m_scriptSpellId == 65226)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_fire_elemental_scaling::CalculateStatAmount, EFFECT_ALL, SPELL_AURA_MOD_STAT);

        if (m_scriptSpellId == 35665 || m_scriptSpellId == 65225)
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_fire_elemental_scaling::CalculateAPAmount, EFFECT_ALL, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_fire_elemental_scaling::CalculateSPAmount, EFFECT_ALL, SPELL_AURA_MOD_DAMAGE_DONE);
        }

        OnEffectApply += AuraEffectApplyFn(spell_sha_fire_elemental_scaling::HandleEffectApply, EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_earth_elemental_scaling : public AuraScript
{
    PrepareAuraScript(spell_sha_earth_elemental_scaling);

    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            amount = CalculatePct(std::max<int32>(0, owner->GetResistance(SPELL_SCHOOL_MASK_NORMAL)), amount);
        }
    }

    void CalculateStatAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            amount = CalculatePct(std::max<int32>(0, owner->GetStat(STAT_STAMINA)), amount);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_earth_elemental_scaling::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_earth_elemental_scaling::CalculateStatAmount, EFFECT_ALL, SPELL_AURA_MOD_STAT);
    }
};

// 52759 - Ancestral Awakening (Proc)
class spell_sha_ancestral_awakening_proc : public SpellScript
{
    PrepareSpellScript(spell_sha_ancestral_awakening_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_ANCESTRAL_AWAKENING_PROC });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.size() < 2)
            return;

        targets.sort(Acore::HealthPctOrderPred());

        WorldObject* target = targets.front();
        targets.clear();
        targets.push_back(target);
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        int32 damage = GetEffectValue();
        if (GetHitUnit())
            GetCaster()->CastCustomSpell(GetHitUnit(), SPELL_SHAMAN_ANCESTRAL_AWAKENING_PROC, &damage, nullptr, nullptr, true);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_ancestral_awakening_proc::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
        OnEffectHitTarget += SpellEffectFn(spell_sha_ancestral_awakening_proc::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

//// 51474 - Astral Shift
//class spell_sha_astral_shift : public AuraScript
//{
//    PrepareAuraScript(spell_sha_astral_shift);
//
//    uint32 absorbPct;
//
//    bool Load() override
//    {
//        absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
//        return true;
//    }
//
//    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
//    {
//        // Set absorbtion amount to unlimited
//        amount = -1;
//    }
//
//    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
//    {
//        // reduces all damage taken while stun, fear or silence
//        if (GetTarget()->GetUnitFlags() & (UNIT_FLAG_FLEEING | UNIT_FLAG_SILENCED) || (GetTarget()->GetUnitFlags() & (UNIT_FLAG_STUNNED) && GetTarget()->HasAuraWithMechanic(1 << MECHANIC_STUN)))
//            absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
//    }
//
//    void Register() override
//    {
//        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_astral_shift::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
//        OnEffectAbsorb += AuraEffectAbsorbFn(spell_sha_astral_shift::Absorb, EFFECT_0);
//    }
//};

// 2825 - Bloodlust
class spell_sha_bloodlust : public SpellScript
{
    PrepareSpellScript(spell_sha_bloodlust);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_SATED, SPELL_SHAMAN_EXHAUSTION });
    }

    void RemoveInvalidTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::UnitAuraCheck(true, SPELL_SHAMAN_SATED));
        targets.remove_if(Acore::UnitAuraCheck(true, SPELL_SHAMAN_EXHAUSTION));
    }

    void ApplyDebuff()
    {
        if (Unit* target = GetHitUnit())
            target->CastSpell(target, SPELL_SHAMAN_SATED, true);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust::RemoveInvalidTargets, EFFECT_ALL, TARGET_UNIT_CASTER_AREA_RAID);
        AfterHit += SpellHitFn(spell_sha_bloodlust::ApplyDebuff);
    }
};

//55459 - Chain Heal
class spell_sha_chain_heal : public SpellScript
{
    PrepareSpellScript(spell_sha_chain_heal);

    bool Load() override
    {
        firstHeal = true;
        riptide = false;
        return true;
    }

    Aura* GetFlowOfTheTidesAura(Unit* caster)
    {
        for (size_t i = 1000018; i < 1000024; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleHeal(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetFlowOfTheTidesAura(caster))
        {
            if (firstHeal)
            {
                // Check if the target has Riptide
                if (Aura* aurEff = GetHitUnit()->GetAura(SPELL_SHAMAN_RIPTIDE))
                {
                    riptide = true;
                    // Consume it
                    GetHitUnit()->RemoveAura(aurEff);
                }
                firstHeal = false;
            }

            // Riptide increases the Chain Heal effect by Rune%
            if (riptide)
            {
                float healIncreasePct = GetFlowOfTheTidesAura(caster)->GetEffect(EFFECT_1)->GetAmount();
                int32 healing = GetHitHeal();
                SetHitHeal(AddPct(healing, healIncreasePct));
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_chain_heal::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
    }

    bool firstHeal;
    bool riptide;
};

// 8171 - Cleansing Totem (Pulse)
class spell_sha_cleansing_totem_pulse : public SpellScript
{
    PrepareSpellScript(spell_sha_cleansing_totem_pulse);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_CLEANSING_TOTEM_EFFECT });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        int32 bp = 1;
        if (GetCaster() && GetHitUnit() && GetOriginalCaster())
            GetCaster()->CastCustomSpell(GetHitUnit(), SPELL_SHAMAN_CLEANSING_TOTEM_EFFECT, nullptr, &bp, nullptr, true, nullptr, nullptr, GetOriginalCaster()->GetGUID());
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_cleansing_totem_pulse::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 49284 - Earth Shield
class spell_sha_earth_shield : public AuraScript
{
    PrepareAuraScript(spell_sha_earth_shield);

    Aura* GetPulsatingEarthAura(Unit* caster)
    {
        for (size_t i = 1000936; i < 1000942; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetEarthenHarmonyAura(Unit* caster)
    {
        for (size_t i = 1000948; i < 1000954; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        // Cast Earthen Harmony's damage reduction
        if (GetEarthenHarmonyAura(caster))
        {
            int32 procSpell = GetEarthenHarmonyAura(caster)->GetEffect(EFFECT_2)->GetAmount();
            caster->AddAura(procSpell, target);
        }
    }

    bool CheckProc(ProcEventInfo&  /*eventInfo*/)
    {
        return !GetTarget()->HasSpellCooldown(SPELL_SHAMAN_EARTH_SHIELD_HEAL);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo&  /*eventInfo*/)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(GetTarget(), SPELL_SHAMAN_EARTH_SHIELD_HEAL, TRIGGERED_FULL_MASK);
        GetTarget()->AddSpellCooldown(SPELL_SHAMAN_EARTH_SHIELD_HEAL, 0, 3000);
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        if (GetPulsatingEarthAura(caster))
        {
            int32 procSpell = GetPulsatingEarthAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            caster->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
        }

        // Check to remove Earthen Harmony's damage reduction
        if (!GetEarthenHarmonyAura(caster))
        {
            for (size_t i = 1000954; i < 1000960; i++)
            {
                if (target->HasAura(i))
                    target->RemoveAura(i);
            }
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        // remove Earthen Harmony's damage reduction from target
        for (size_t i = 1000954; i < 1000960; i++)
        {
            if (target->HasAura(i))
                target->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_earth_shield::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        DoCheckProc += AuraCheckProcFn(spell_sha_earth_shield::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_earth_shield::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earth_shield::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_earth_shield::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 379 - Earth Shield Heal
class spell_sha_earth_shield_heal : public SpellScript
{
    PrepareSpellScript(spell_sha_earth_shield_heal);

    Aura* GetEarthenHarmonyAura(Unit* caster)
    {
        for (size_t i = 1000948; i < 1000954; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetHitUnit();

        if (!target || target->isDead())
            return;

        int32 heal = GetHitHeal();

        // apply heal increase from Earthen Harmony Rune
        if (GetEarthenHarmonyAura(caster))
        {
            int32 healIncrease = GetEarthenHarmonyAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            int32 healthThreshold = GetEarthenHarmonyAura(caster)->GetEffect(EFFECT_1)->GetAmount();

            if (target->GetHealthPct() < healthThreshold)
                AddPct(heal, healIncrease);
        }

        SetHitHeal(heal);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_earth_shield_heal::HandleDummy, EFFECT_0, SPELL_EFFECT_HEAL);
    }
};

// 6474 - Earthbind Totem - Fix Talent: Earthen Power
class spell_sha_earthbind_totem : public AuraScript
{
    PrepareAuraScript(spell_sha_earthbind_totem);

    Aura* GetFrozenPowerAura()
    {
        for (size_t i = 84049; i < 84051; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_TOTEM_EARTHBIND_TOTEM, SPELL_SHAMAN_TOTEM_EARTHEN_POWER });
    }

    void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
    {
        if (!GetCaster())
            return;

        if (Player* owner = GetCaster()->GetCharmerOrOwnerPlayerOrPlayerItself())
            if (Aura* aura = GetFrozenPowerAura())
                if (AuraEffect* effect = aura->GetEffect(EFFECT_0)) {
                    if (roll_chance_i(effect->GetBaseAmount()))
                        GetTarget()->CastSpell((Unit*)nullptr, SPELL_SHAMAN_TOTEM_EARTHEN_POWER, true);
                }
    }

    void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster())
            return;
        Player* owner = GetCaster()->GetCharmerOrOwnerPlayerOrPlayerItself();
        if (!owner)
            return;
        // Storm, Earth and Fire
        if (AuraEffect* aurEff = owner->GetAuraEffectOfRankedSpell(SPELL_SHAMAN_STORM_EARTH_AND_FIRE, EFFECT_1))
        {
            if (roll_chance_i(aurEff->GetAmount()))
                GetCaster()->CastSpell(GetCaster(), SPELL_SHAMAN_TOTEM_EARTHBIND_EARTHGRAB, false);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earthbind_totem::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        OnEffectApply += AuraEffectApplyFn(spell_sha_earthbind_totem::Apply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class EarthenPowerTargetSelector
{
public:
    EarthenPowerTargetSelector() { }

    bool operator() (WorldObject* target)
    {
        if (!target->ToUnit())
            return true;

        if (!target->ToUnit()->HasAuraWithMechanic(1 << MECHANIC_SNARE))
            return true;

        return false;
    }
};

// 59566 - Earthen Power
class spell_sha_earthen_power : public SpellScript
{
    PrepareSpellScript(spell_sha_earthen_power);

    void FilterTargets(std::list<WorldObject*>& unitList)
    {
        unitList.remove_if(EarthenPowerTargetSelector());
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_earthen_power::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
    }
};

// -51940 - Earthliving Weapon (Passive)
class spell_sha_earthliving_weapon : public AuraScript
{
    PrepareAuraScript(spell_sha_earthliving_weapon);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_BLESSING_OF_THE_ETERNALS_R1 });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();
        auto chance = sSpellMgr->AssertSpellInfo(SPELL_SHAMAN_EARTHLIVING_WEAPON)->GetEffect(EFFECT_1).CalcValue(caster);

        if (!caster || !eventInfo.GetProcTarget())
            return false;

        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
            if (spellInfo->Id == SPELL_SHAMAN_EARTH_SHIELD_HEAL)
                return false;

        if (AuraEffect const* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_SHAMAN_BLESSING_OF_THE_ETERNALS_R1, EFFECT_1, caster->GetGUID()))
            if (eventInfo.GetProcTarget()->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT))
                chance += aurEff->GetAmount();

        return roll_chance_i(chance);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_earthliving_weapon::CheckProc);
    }
};

// 52000 - Earthliving Weapon Heal
class spell_sha_earthliving_weapon_hot : public AuraScript
{
    PrepareAuraScript(spell_sha_earthliving_weapon_hot);

    Aura* GetImprovedEarthlibingWeaponAura(Unit* caster)
    {
        for (size_t i = 1000448; i < 1000454; i++)
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

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        if (GetImprovedEarthlibingWeaponAura(caster))
        {
            int32 heal = aurEff->GetAmount();
            int32 healPct = GetImprovedEarthlibingWeaponAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            int32 healthThreshold = GetImprovedEarthlibingWeaponAura(caster)->GetEffect(EFFECT_2)->GetAmount();

            if (target->GetHealthPct() < healthThreshold)
            {
                int32 amount = CalculatePct(heal, healPct);

                caster->CastCustomSpell(RUNE_SHAMAN_IMPROVED_EARTHLIVING_WEAPON_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earthliving_weapon_hot::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
    }
};

//// -1535 - Fire Nova
//class spell_sha_fire_nova : public SpellScript
//{
//    PrepareSpellScript(spell_sha_fire_nova);
//
//    bool Validate(SpellInfo const* spellInfo) override
//    {
//        SpellInfo const* firstRankSpellInfo = sSpellMgr->GetSpellInfo(SPELL_SHAMAN_FIRE_NOVA_R1);
//        if (!firstRankSpellInfo || !spellInfo->IsRankOf(firstRankSpellInfo))
//            return false;
//
//        uint8 rank = spellInfo->GetRank();
//        if (!sSpellMgr->GetSpellWithRank(SPELL_SHAMAN_FIRE_NOVA_TRIGGERED_R1, rank, true))
//            return false;
//        return true;
//    }
//
//    SpellCastResult CheckFireTotem()
//    {
//        // fire totem
//        Unit* caster = GetCaster();
//        if (Creature* totem = caster->GetMap()->GetCreature(caster->m_SummonSlot[1]))
//        {
//            if (!caster->IsWithinDistInMap(totem, caster->GetSpellMaxRangeForTarget(totem, GetSpellInfo())))
//                return SPELL_FAILED_OUT_OF_RANGE;
//            return SPELL_CAST_OK;
//        }
//        else
//        {
//            SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_HAVE_FIRE_TOTEM);
//            return SPELL_FAILED_CUSTOM_ERROR;
//        }
//    }
//
//    void HandleDummy(SpellEffIndex /*effIndex*/)
//    {
//        Unit* caster = GetCaster();
//        if (Creature* totem = caster->GetMap()->GetCreature(caster->m_SummonSlot[1]))
//        {
//            uint8 rank = GetSpellInfo()->GetRank();
//            if (totem->IsTotem())
//                caster->CastSpell(totem, sSpellMgr->GetSpellWithRank(SPELL_SHAMAN_FIRE_NOVA_TRIGGERED_R1, rank), true);
//        }
//    }
//
//    void Register() override
//    {
//        OnCheckCast += SpellCheckCastFn(spell_sha_fire_nova::CheckFireTotem);
//        OnEffectHitTarget += SpellEffectFn(spell_sha_fire_nova::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
//    }
//};

// -8050 - Flame Shock
class spell_sha_flame_shock : public AuraScript
{
    PrepareAuraScript(spell_sha_flame_shock);

    void HandleDispel(DispelInfo* /*dispelInfo*/)
    {
        if (Unit* caster = GetCaster())
            // Lava Flows
            if (AuraEffect const* aurEff = caster->GetDummyAuraEffect(SPELLFAMILY_SHAMAN, SHAMAN_ICON_ID_SHAMAN_LAVA_FLOW, EFFECT_0))
            {
                if (SpellInfo const* firstRankSpellInfo = sSpellMgr->GetSpellInfo(SPELL_SHAMAN_LAVA_FLOWS_R1))
                    if (!aurEff->GetSpellInfo()->IsRankOf(firstRankSpellInfo))
                        return;

                uint8 rank = aurEff->GetSpellInfo()->GetRank();
                caster->CastSpell(caster, sSpellMgr->GetSpellWithRank(SPELL_SHAMAN_LAVA_FLOWS_TRIGGERED_R1, rank), true);
            }
    }

    void Register() override
    {
        AfterDispel += AuraDispelFn(spell_sha_flame_shock::HandleDispel);
    }
};

//// 58761 - Healing Stream Totem
//class spell_sha_healing_stream_totem : public SpellScript
//{
//    PrepareSpellScript(spell_sha_healing_stream_totem);
//
//    bool Validate(SpellInfo const* /*spellInfo*/) override
//    {
//        return ValidateSpellInfo({ SPELL_SHAMAN_GLYPH_OF_HEALING_STREAM_TOTEM, SPELL_SHAMAN_TOTEM_HEALING_STREAM_HEAL });
//    }
//
//    void HandleDummy(SpellEffIndex effIndex)
//    {
//        int32 damage = GetEffectValue();
//        SpellInfo const* triggeringSpell = GetTriggeringSpell();
//        if (Unit* target = GetHitUnit())
//            if (Unit* caster = GetCaster())
//            {
//                if (Unit* owner = caster->GetOwner())
//                {
//                    if (triggeringSpell)
//                        damage = int32(owner->SpellHealingBonusDone(target, triggeringSpell, damage, HEAL, effIndex));
//
//                    // Restorative Totems
//                    if (AuraEffect* dummy = owner->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_SHAMAN, SHAMAN_ICON_ID_RESTORATIVE_TOTEMS, 1))
//                        AddPct(damage, dummy->GetAmount());
//
//                    damage = int32(target->SpellHealingBonusTaken(owner, triggeringSpell, damage, HEAL));
//                }
//                caster->CastCustomSpell(target, SPELL_SHAMAN_TOTEM_HEALING_STREAM_HEAL, &damage, 0, 0, true, 0, 0, GetOriginalCaster()->GetGUID());
//            }
//    }
//
//    void Register() override
//    {
//        OnEffectHitTarget += SpellEffectFn(spell_sha_healing_stream_totem::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
//    }
//};

// 58760 - Healing Stream Totem Heal
class spell_sha_healing_stream_totem_target : public SpellScript
{
    PrepareSpellScript(spell_sha_healing_stream_totem_target);

    Aura* GetLivingStreamAura(Unit* caster)
    {
        for (size_t i = 1000246; i < 1000252; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetFlowingStreamsAura(Unit* caster)
    {
        for (size_t i = 1000264; i < 1000270; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleBeforeCast()
    {
        Unit* totem = GetCaster();

        if (!totem || totem->isDead())
            return;

        Unit* caster = totem->GetOwner();

        if (!caster || caster->isDead())
            return;

        if (GetLivingStreamAura(caster))
        {
            int32 procSpell = GetLivingStreamAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
        }
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::RaidCheck(GetCaster(), false));

        uint32 const maxTargets = sSpellMgr->AssertSpellInfo(SPELL_SHAMAN_HEALING_STREAM_TOTEM_AURA)->GetEffect(EFFECT_1).CalcValue(GetCaster());

        if (targets.size() > maxTargets)
        {
            targets.sort(Acore::HealthPctOrderPred());
            targets.resize(maxTargets);
        }
    }

    void HandleHit()
    {
        Unit* totem = GetCaster();

        if (!totem || totem->isDead())
            return;

        Unit* caster = totem->GetOwner();

        if (!caster || caster->isDead())
            return;

        if (GetFlowingStreamsAura(caster))
        {
            int32 procChance = GetFlowingStreamsAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            if (roll_chance_i(procChance))
                caster->CastSpell(caster, SPELL_SHAMAN_HEALING_SURGE, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_sha_healing_stream_totem_target::HandleBeforeCast);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_healing_stream_totem_target::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        OnHit += SpellHitFn(spell_sha_healing_stream_totem_target::HandleHit);
    }
};

// 32182 - Heroism
class spell_sha_heroism : public SpellScript
{
    PrepareSpellScript(spell_sha_heroism);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_EXHAUSTION, SPELL_SHAMAN_SATED });
    }

    void RemoveInvalidTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::UnitAuraCheck(true, SPELL_SHAMAN_EXHAUSTION));
        targets.remove_if(Acore::UnitAuraCheck(true, SPELL_SHAMAN_SATED));
    }

    void ApplyDebuff()
    {
        if (Unit* target = GetHitUnit())
            target->CastSpell(target, SPELL_SHAMAN_EXHAUSTION, true);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism::RemoveInvalidTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
        AfterHit += SpellHitFn(spell_sha_heroism::ApplyDebuff);
    }
};

// 23551 - Lightning Shield
class spell_sha_item_lightning_shield : public AuraScript
{
    PrepareAuraScript(spell_sha_item_lightning_shield);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD });
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD, true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sha_item_lightning_shield::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 23552 - Lightning Shield
class spell_sha_item_lightning_shield_trigger : public AuraScript
{
    PrepareAuraScript(spell_sha_item_lightning_shield_trigger);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_ITEM_MANA_SURGE });
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(GetTarget(), SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD_DAMAGE, true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sha_item_lightning_shield_trigger::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 23572 - Mana Surge
class spell_sha_item_mana_surge : public AuraScript
{
    PrepareAuraScript(spell_sha_item_mana_surge);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD_DAMAGE });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() != nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        int32 mana = eventInfo.GetSpellInfo()->CalcPowerCost(GetTarget(), eventInfo.GetSchoolMask());
        int32 damage = CalculatePct(mana, 35);

        GetTarget()->CastCustomSpell(SPELL_SHAMAN_ITEM_MANA_SURGE, SPELLVALUE_BASE_POINT0, damage, GetTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_item_mana_surge::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_item_mana_surge::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 70811 - Item - Shaman T10 Elemental 2P Bonus
class spell_sha_item_t10_elemental_2p_bonus : public AuraScript
{
    PrepareAuraScript(spell_sha_item_t10_elemental_2p_bonus);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_ELEMENTAL_MASTERY });
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_SHAMAN_ELEMENTAL_MASTERY, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sha_item_t10_elemental_2p_bonus::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 60103 - Lava Lash
class spell_sha_lava_lash : public SpellScript
{
    PrepareSpellScript(spell_sha_lava_lash)

        bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    void HandleDummy(SpellEffIndex effIndex)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            int32 hitDamage = GetHitDamage();

            if (Item* castItem = caster->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
            {
                int32 itemId = castItem->GetGUID();
                if (castItem->GetEnchantmentId(EnchantmentSlot(TEMP_ENCHANTMENT_SLOT)) == SPELL_SHAMAN_FLAMETONGUE_WEAPON_AURA)
                {
                    int32 flametongueIncrease = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(caster);
                    // Damage is increased by 100% if your off-hand weapon is enchanted with Flametongue.
                    if (caster->GetAuraEffect(58792, 0))
                        AddPct(hitDamage, flametongueIncrease);
                }
            }

            SetHitDamage(hitDamage);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_lava_lash::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 52031, 52033, 52034, 52035, 52036, 58778, 58779, 58780 - Mana Spring Totem
class spell_sha_mana_spring_totem : public SpellScript
{
    PrepareSpellScript(spell_sha_mana_spring_totem);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_MANA_SPRING_TOTEM_ENERGIZE });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        int32 damage = GetEffectValue();
        if (Unit* target = GetHitUnit())
            if (Unit* caster = GetCaster())
                if (target->getPowerType() == POWER_MANA)
                    caster->CastCustomSpell(target, SPELL_SHAMAN_MANA_SPRING_TOTEM_ENERGIZE, &damage, 0, 0, true, 0, 0, GetOriginalCaster()->GetGUID());
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_mana_spring_totem::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 16190 - Mana Tide Totem
class spell_sha_mana_tide_totem : public SpellScript
{
    PrepareSpellScript(spell_sha_mana_tide_totem);

    Aura* GetSpiritwalkersTidalTotemAura(Unit* caster)
    {
        for (size_t i = 1001142; i < 1001148; i++)
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

        if (GetSpiritwalkersTidalTotemAura(caster))
        {
            int32 procSpell = GetSpiritwalkersTidalTotemAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            caster->AddAura(procSpell, caster);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_sha_mana_tide_totem::HandleCast);
    }
};

//// 39610 - Mana Tide Totem Energize
//class spell_sha_mana_tide_totem_energize : public SpellScript
//{
//    PrepareSpellScript(spell_sha_mana_tide_totem_energize);
//
//    bool Validate(SpellInfo const* /*spellInfo*/) override
//    {
//        return ValidateSpellInfo({ SPELL_SHAMAN_GLYPH_OF_MANA_TIDE, SPELL_SHAMAN_MANA_TIDE_TOTEM });
//    }
//
//    void HandleDummy(SpellEffIndex /*effIndex*/)
//    {
//        if (Unit* caster = GetCaster())
//            if (Unit* unitTarget = GetHitUnit())
//            {
//                if (unitTarget->getPowerType() == POWER_MANA)
//                {
//                    int32 effValue = GetEffectValue();
//                    // Glyph of Mana Tide
//                    if (Unit* owner = caster->GetOwner())
//                        if (AuraEffect* dummy = owner->GetAuraEffect(SPELL_SHAMAN_GLYPH_OF_MANA_TIDE, 0))
//                            effValue += dummy->GetAmount();
//                    // Regenerate 6% of Total Mana Every 3 secs
//                    int32 effBasePoints0 = int32(CalculatePct(unitTarget->GetMaxPower(POWER_MANA), effValue));
//                    caster->CastCustomSpell(unitTarget, SPELL_SHAMAN_MANA_TIDE_TOTEM, &effBasePoints0, nullptr, nullptr, true, nullptr, nullptr, GetOriginalCaster()->GetGUID());
//                }
//            }
//    }
//
//    void Register() override
//    {
//        OnEffectHitTarget += SpellEffectFn(spell_sha_mana_tide_totem_energize::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
//    }
//};

// 6495 - Sentry Totem
class spell_sha_sentry_totem : public AuraScript
{
    PrepareAuraScript(spell_sha_sentry_totem);

    bool Validate(SpellInfo const* /*spell*/) override
    {
        return ValidateSpellInfo({ SPELL_SHAMAN_BIND_SIGHT });
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
            if (caster->GetTypeId() == TYPEID_PLAYER)
                caster->ToPlayer()->StopCastingBindSight();
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_sha_sentry_totem::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// -51490 - Thunderstorm
class spell_sha_thunderstorm : public SpellScript
{
    PrepareSpellScript(spell_sha_thunderstorm);

    void HandleKnockBack(SpellEffIndex effIndex)
    {
        // Glyph of Thunderstorm
        if (GetCaster()->HasAura(SPELL_SHAMAN_GLYPH_OF_THUNDERSTORM))
            PreventHitDefaultEffect(effIndex);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_thunderstorm::HandleKnockBack, EFFECT_2, SPELL_EFFECT_KNOCK_BACK);
    }
};

// -16257 - SpellName
class spell_sha_flurry_proc : public AuraScript
{
    PrepareAuraScript(spell_sha_flurry_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        // Should not proc from Windfury Attack, Stormstrike and Lava Lash
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            constexpr std::array<uint32, 2> spellIcons = { SPELL_SHAMAN_STORMSTRIKE, SPELL_SHAMAN_LAVA_LASH };
            const auto found = std::find(std::begin(spellIcons), std::end(spellIcons), spellInfo->Id);

            if ((spellInfo->SpellFamilyName == SPELLFAMILY_SHAMAN && (spellInfo->SpellFamilyFlags[0] & 0x00800000) != 0) || found != std::end(spellIcons))
            {
                return false;
            }
        }

        return true;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_flurry_proc::CheckProc);
    }
};

// 84000 - Fire Nova
class spell_sha_fire_nova : public SpellScript
{
    PrepareSpellScript(spell_sha_fire_nova);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead() || !target->HasAura(SPELL_SHAMAN_FLAME_SHOCK))
                continue;

            caster->CastSpell(target, SPELL_SHAMAN_FIRE_NOVA_DAMAGE, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_fire_nova::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// 58799 - Frostbrand Weapon Damage
class spell_sha_frostbrand_weapon : public SpellScript
{
    PrepareSpellScript(spell_sha_frostbrand_weapon);

    void HandleDamage(SpellEffIndex effIndex)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();
        int32 damage = GetEffectValue();

        if (!target || target->isDead())
            return;

        damage += CalculatePct(caster->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask()), GetSpellInfo()->GetEffect(effIndex).BonusMultiplier);
        damage += CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), GetSpellInfo()->GetEffect(effIndex).DamageMultiplier);
        damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
        damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_frostbrand_weapon::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_sha_gust_of_wind : public SpellScript
{
    PrepareSpellScript(spell_sha_gust_of_wind);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        float speedxy = 200 / 10.0f;
        float speedz = 75 / 10.0f;

        caster->JumpTo(speedxy, speedz, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_gust_of_wind::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// All Maelstrom Generation EFFECT_2 On Hit
class spell_sha_maelstrom_on_hit : public SpellScript
{
    PrepareSpellScript(spell_sha_maelstrom_on_hit);

    void HandleProc(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        int32 maelstromAmount = GetSpellInfo()->GetEffect(EFFECT_2).CalcValue(caster);

        caster->ModifyPower(POWER_RUNIC_POWER, maelstromAmount, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_maelstrom_on_hit::HandleProc, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 84005 - Ancestral Vision
class spell_sha_ancestral_vision : public SpellScript
{
    PrepareSpellScript(spell_sha_ancestral_vision);

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
            GetCaster()->CastSpell(target, SPELL_SHAMAN_ANCESTRAL_VISION, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_sha_ancestral_vision::HandleProc);
    }
};

// 84010 - Ancestral Guidance
class spell_sha_ancestral_guidance : public AuraScript
{
    PrepareAuraScript(spell_sha_ancestral_guidance);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActor() && eventInfo.GetProcTarget();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetProcTarget();

        if (!target || target->isDead())
            return;

        int32 amountPct = aurEff->GetAmount();
        int32 amount = 0;

        if (eventInfo.GetDamageInfo())
            amount += eventInfo.GetDamageInfo()->GetDamage();

        if (eventInfo.GetHealInfo())
            amount += eventInfo.GetHealInfo()->GetHeal();

        if (amount == 0)
            return;

        ApplyPct(amount, amountPct);
        GetEffect(EFFECT_1)->SetAmount(amount);

        caster->CastSpell(caster, SPELL_SHAMAN_ANCESTRAL_GUIDANCE_AOE, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_ancestral_guidance::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_ancestral_guidance::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 84011 - Ancestral Guidance targets
class spell_sha_ancestral_guidance_healing : public SpellScript
{
    PrepareSpellScript(spell_sha_ancestral_guidance_healing);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (Aura* ancestralGuidance = caster->GetAura(SPELL_SHAMAN_ANCESTRAL_GUIDANCE))
        {
            uint32 const maxTargets = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(caster);
            int32 amount = ancestralGuidance->GetEffect(EFFECT_1)->GetAmount();

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

                caster->CastCustomSpell(SPELL_SHAMAN_ANCESTRAL_GUIDANCE_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_ancestral_guidance_healing::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// 84036 - Ascendance (Air) Buff
class spell_sha_ascendance_air_aura : public AuraScript
{
    PrepareAuraScript(spell_sha_ascendance_air_aura);

    Aura* GetOathOfTheFarSeerAura(Unit* caster)
    {
        for (size_t i = 1000436; i < 1000442; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetOathOfTheFarSeerAura(caster))
            caster->AddAura(GetOathOfTheFarSeerAura(caster)->GetEffect(EFFECT_1)->GetAmount(), caster);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1000442; i < 1000448; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_ascendance_air_aura::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_ascendance_air_aura::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

// 84019 - Ascendance (Flame)
class spell_sha_ascendance_flame : public SpellScript
{
    PrepareSpellScript(spell_sha_ascendance_flame);

    void HandleProc()
    {
        Unit* caster = GetCaster();
        auto const& threatList = caster->getAttackers();

        if (threatList.empty())
            return;

        for (auto itr = threatList.begin(); itr != threatList.end(); ++itr)
        {
            Unit* treath = (*itr);

            if (Aura* flameShock = treath->GetAura(SPELL_SHAMAN_FLAME_SHOCK))
            {
                GetCaster()->CastSpell(treath, SPELL_SHAMAN_LAVA_BURST, TRIGGERED_FULL_MASK);
                GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_SHAMAN_LAVA_BURST);
                flameShock->RefreshDuration();
                flameShock->GetEffect(EFFECT_1)->ResetTicks();
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_sha_ascendance_flame::HandleProc);
    }
};

// 84019 - Ascendance (Flame)
class spell_sha_ascendance_flame_aura : public AuraScript
{
    PrepareAuraScript(spell_sha_ascendance_flame_aura);

    Aura* GetOathOfTheFarSeerAura(Unit* caster)
    {
        for (size_t i = 1000436; i < 1000442; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetOathOfTheFarSeerAura(caster))
            caster->AddAura(GetOathOfTheFarSeerAura(caster)->GetEffect(EFFECT_1)->GetAmount(), caster);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1000442; i < 1000448; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_ascendance_flame_aura::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_ascendance_flame_aura::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

// 49271 - Chain Lightning
class spell_sha_chain_lightning : public SpellScript
{
    PrepareSpellScript(spell_sha_chain_lightning);

    void HandleCast()
    {
        if (Aura* ascendance = GetCaster()->GetAura(SPELL_SHAMAN_ASCENDANCE_FIRE))
            GetCaster()->CastSpell(GetExplTargetUnit(), SPELL_SHAMAN_LAVA_BEAM, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_sha_chain_lightning::HandleCast);
    }
};

// 84026 - Stormbringer
class spell_sha_stormbringer : public AuraScript
{
    PrepareAuraScript(spell_sha_stormbringer);

    Aura* GetStormblastAura(Unit* caster)
    {
        for (size_t i = 1000830; i < 1000836; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActor();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = eventInfo.GetActor()->ToPlayer())
            player->RemoveSpellCooldown(SPELL_SHAMAN_STORMSTRIKE, true);

        // Stormblast Rune proc
        if (GetStormblastAura(caster))
        {
            int32 procSpell = GetStormblastAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            caster->AddAura(procSpell, caster);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_stormbringer::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_stormbringer::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 84032 - Crash Lightning
class spell_sha_crash_lightning : public SpellScript
{
    PrepareSpellScript(spell_sha_crash_lightning);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        uint32 const maxTargets = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(GetCaster());

        if (targets.size() >= maxTargets)
            GetCaster()->AddAura(SPELL_SHAMAN_CRASH_LIGHTNING_AURA, GetCaster());
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_crash_lightning::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
    }
};

// 84033 - Crash Lightning Aura
class spell_sha_crash_lightning_proc : public AuraScript
{
    PrepareAuraScript(spell_sha_crash_lightning_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActor();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();

        caster->CastSpell(caster, SPELL_SHAMAN_CRASH_LIGHTNING_DAMAGE, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_crash_lightning_proc::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_crash_lightning_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 84040 - Ascendance (Water)
class spell_sha_ascendance_water : public SpellScript
{
    PrepareSpellScript(spell_sha_ascendance_water);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        int32 targetNumber = targets.size();

        if (targetNumber == 0)
            return;

        int32 SpRation = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(caster);
        int32 totalAmount = CalculatePct(caster->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_ALL), SpRation);
        int32 amount = totalAmount / targetNumber;

        GetCaster()->CastCustomSpell(SPELL_SHAMAN_ASCENDANCE_WATER_AOE_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_ascendance_water::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// 84041 - Ascendance (Water) Aura
class spell_sha_ascendance_water_proc : public AuraScript
{
    PrepareAuraScript(spell_sha_ascendance_water_proc);

    Aura* GetOathOfTheFarSeerAura(Unit* caster)
    {
        for (size_t i = 1000436; i < 1000442; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetOathOfTheFarSeerAura(caster))
            caster->AddAura(GetOathOfTheFarSeerAura(caster)->GetEffect(EFFECT_1)->GetAmount(), caster);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetHealInfo()->GetTarget();
        Unit* caster = eventInfo.GetHealInfo()->GetHealer();
        int32 healAmount = eventInfo.GetHealInfo()->GetHeal();
        int32 healPct = aurEff->GetAmount();

        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (!caster || caster->isDead())
            return;

        if (!target || target->isDead())
            return;

        int32 amount = CalculatePct(healAmount, healPct);
        GetCaster()->CastCustomSpell(SPELL_SHAMAN_ASCENDANCE_WATER_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1000442; i < 1000448; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_ascendance_water_proc::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        DoCheckProc += AuraCheckProcFn(spell_sha_ascendance_water_proc::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_ascendance_water_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_ascendance_water_proc::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 84046 - Downpour
class spell_sha_downpour : public SpellScript
{
    PrepareSpellScript(spell_sha_downpour);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.size() > 6)
            targets.resize(6);

        targetsSize = targets.size();
    }

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();
        int32 cooldownPerTarget = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(caster);
        caster->ToPlayer()->SetSpellCooldown(SPELL_SHAMAN_DOWNPOUR, -30000);
        caster->ToPlayer()->SetSpellCooldown(SPELL_SHAMAN_DOWNPOUR, (targetsSize * cooldownPerTarget));
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_downpour::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        AfterCast += SpellCastFn(spell_sha_downpour::HandleAfterCast);
    }


private:
    uint32 targetsSize;
};

// 51474 - Improved Astral Shift
class spell_sha_improved_astral_shift : public AuraScript
{
    PrepareAuraScript(spell_sha_improved_astral_shift);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetSpellInfo())
            return false;

        const SpellInfo* procSpell = eventInfo.GetSpellInfo();

        return (procSpell->GetAllEffectsMechanicMask() & ((1 << MECHANIC_SILENCE) | (1 << MECHANIC_STUN) | (1 << MECHANIC_FEAR)));
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        Unit* caster = GetCaster();
        int32 duration = aurEff->GetAmount();

        if (!caster || caster->isDead())
            return;

        if (Aura* auraEff = caster->GetAura(SPELL_SHAMAN_ASTRAL_SHIFT))
        {
            duration += auraEff->GetDuration();
            auraEff->SetDuration(duration);
        }
        else
        {
            caster->CastCustomSpell(SPELL_SHAMAN_ASTRAL_SHIFT, SPELLVALUE_AURA_DURATION, duration, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_improved_astral_shift::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_improved_astral_shift::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 49236 - Frost Shock / 84051 - Frozen Power Aura
class spell_sha_frozen_power_aura : public AuraScript
{
    PrepareAuraScript(spell_sha_frozen_power_aura);

    Aura* GetFrozenPowerAura()
    {
        for (size_t i = 84049; i < 84051; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetUnitOwner();

        if (!caster || !target)
            return;

        if (!GetFrozenPowerAura())
            return;

        if (AuraEffect* aurEff = GetFrozenPowerAura()->GetEffect(EFFECT_1))
        {
            int32 procSpell = aurEff->GetAmount();
            caster->AddAura(procSpell, target);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        for (size_t i = 84051; i < 84052; i++)
        {
            if (GetUnitOwner()->HasAura(i))
                GetUnitOwner()->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_frozen_power_aura::HandleApply, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_frozen_power_aura::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
    }
};

// 84018 - Stormkeeper
class spell_sha_stormkeeper : public AuraScript
{
    PrepareAuraScript(spell_sha_stormkeeper);

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->AddAura(SPELL_SHAMAN_STORMKEEPER_LISTENER, caster);
        caster->GetAura(SPELL_SHAMAN_STORMKEEPER_LISTENER)->SetStackAmount(2);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return (eventInfo.GetSpellInfo());
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetActionTarget();
        int32 spellID = eventInfo.GetSpellInfo()->Id;

        if (!caster || !target)
            return;

        // doesn't proc on overload versions
        if (spellID == SPELL_SHAMAN_LIGHTNING_BOLT_OVERLOAD || spellID == SPELL_SHAMAN_CHAIN_LIGHTNING_OVERLOAD || spellID == SPELL_SHAMAN_LAVA_BEAM_OVERLOAD)
            return;

        if (spellID == SPELL_SHAMAN_LIGHTNING_BOLT)
            spellID = SPELL_SHAMAN_LIGHTNING_BOLT_OVERLOAD;
        else if (spellID == SPELL_SHAMAN_CHAIN_LIGHTNING)
            spellID = SPELL_SHAMAN_CHAIN_LIGHTNING_OVERLOAD;
        else if (spellID == SPELL_SHAMAN_LAVA_BEAM)
            spellID = SPELL_SHAMAN_LAVA_BEAM_OVERLOAD;

        if (!caster->HasAura(SPELL_SHAMAN_STORMKEEPER_LISTENER))
            return;

        // damage + maelstrom nerf
        caster->AddAura(MASTERY_SHAMAN_ELEMENTAL_OVERLOAD_BUFF, caster);
        // caster->CastSpell(target, spellID, TRIGGERED_FULL_MASK);
        caster->RemoveAura(MASTERY_SHAMAN_ELEMENTAL_OVERLOAD_BUFF);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_stormkeeper::HandleApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        DoCheckProc += AuraCheckProcFn(spell_sha_stormkeeper::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_stormkeeper::HandleEffectProc, EFFECT_2, SPELL_AURA_DUMMY);
    }
};

// 84054 - Stormkeeper Listener
class spell_sha_stormkeeper_remove : public AuraScript
{
    PrepareAuraScript(spell_sha_stormkeeper_remove);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(SPELL_SHAMAN_STORMKEEPER))
            GetCaster()->RemoveAura(SPELL_SHAMAN_STORMKEEPER);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_stormkeeper_remove::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_lava_burst : public SpellScript
{
    PrepareSpellScript(spell_sha_lava_burst);

    void HandleDamage(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        int32 damage = GetHitDamage();

        if (caster->HasAura(SPELL_SHAMAN_ASCENDANCE_FIRE))
        {
            float crit_chance = caster->GetFloatValue(static_cast<uint16>(PLAYER_SPELL_CRIT_PERCENTAGE1) + SPELL_SCHOOL_FIRE);
            AddPct(damage, crit_chance);
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_lava_burst::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_sha_elemental_blast : public SpellScript
{
    PrepareSpellScript(spell_sha_elemental_blast);

    struct Ratings
    {
        uint32 value;
        uint32 spellId;
    };

    void HandleProc()
    {
        Unit* caster = GetCaster();

        Ratings arr[] = {
           { caster->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + static_cast<uint16>(CR_CRIT_MELEE)),
                    SPELL_SHAMAN_ELEMENTAL_BLAST_CRIT },
           { caster->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + static_cast<uint16>(CR_HIT_MELEE)),
                    SPELL_SHAMAN_ELEMENTAL_BLAST_MASTERY },
           { caster->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + static_cast<uint16>(CR_HASTE_MELEE)),
                    SPELL_SHAMAN_ELEMENTAL_BLAST_HASTE },
        };

        int size = sizeof(arr) / sizeof(arr[0]);

        auto highestRating = std::max_element(arr, arr + sizeof(arr) / sizeof(arr[0]), [](const Ratings& a, const Ratings& b) {
            return a.value < b.value;
            });

        caster->AddAura(highestRating->spellId, caster);
    }

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // Remove T1 2pc Buff
        if (caster->HasAura(T1_SHAMAN_ELEM_2PC_BUFF))
            caster->RemoveAura(T1_SHAMAN_ELEM_2PC_BUFF);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_sha_elemental_blast::HandleProc);
        AfterHit += SpellHitFn(spell_sha_elemental_blast::HandleAfterHit);
    }
};

class spell_sha_spirit_link : public AuraScript
{
    PrepareAuraScript(spell_sha_spirit_link);

    uint8 GetMemberGroupCountAliveAndAtDistance(Group* group) {

        Unit* caster = GetCaster();

        auto const& allyList = group->GetMemberSlots();

        uint8 count = 0;
        for (auto const& target : allyList)
            if (Player* member = ObjectAccessor::FindPlayer(target.guid)) {
                float distance = member->GetDistance(caster->GetPosition());
                if (member->IsAlive() && distance <= 10.0f)
                    count += 1;
            }


        return count;
    }

    void OnPeriodic(AuraEffect const* aurEff)
    {
        Unit* owner = GetCaster()->GetOwner();

        if (!owner)
            return;

        Group* group = owner->ToPlayer()->GetGroup();

        if (!group)
            return;

        uint32 totalHealth = 0;

        auto const& allyList = group->GetMemberSlots();

        for (auto const& target : allyList)
            if (Player* member = ObjectAccessor::FindPlayer(target.guid))
                if (member->IsAlive())
                    totalHealth += member->GetHealthPct();

        uint32 pctAmount = totalHealth / GetMemberGroupCountAliveAndAtDistance(group);

        for (auto const& target : allyList)
            if (Player* member = ObjectAccessor::FindPlayer(target.guid)) {
                float distance = member->GetDistance(GetCaster()->GetPosition());
                uint32 amount = member->CountPctFromMaxHealth(pctAmount);

                if (member->IsAlive() && distance <= 10.0f)
                    member->SetHealth(amount);
            }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_spirit_link::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_shaman_totem_ancestral_protection : public AuraScript
{
    PrepareAuraScript(spell_shaman_totem_ancestral_protection);

public:
    spell_shaman_totem_ancestral_protection()
    {
        healPct = 0;
    }

private:
    uint32 healPct;

    bool Load() override
    {
        healPct = GetSpellInfo()->Effects[EFFECT_2].CalcValue(GetCaster());
        return true;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {

        Unit* victim = GetTarget();
        Unit* caster = GetCaster();

        int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
        uint32 allowedHealth = victim->CountPctFromMaxHealth(1);

        if (remainingHealth <= 0)
        {
            int32 healAmount = int32(victim->CountPctFromMaxHealth(healPct));
            victim->CastCustomSpell(SPELL_SHAMAN_ANCESTRAL_PROTECTION_HEAL, SPELLVALUE_BASE_POINT0, healAmount, victim, true, nullptr, aurEff);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_shaman_totem_ancestral_protection::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_shaman_totem_ancestral_protection::Absorb, EFFECT_1);
    }
};

class spell_sha_summon_cloudburst_totem : public AuraScript
{
    PrepareAuraScript(spell_sha_summon_cloudburst_totem);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }


    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(83);
        int32 duration = GetSpellInfo()->GetDuration();
        Position pos = caster->GetNearPosition(3.f, 0);
        caster->GetMap()->SummonCreature(400406, pos, properties, duration + 1000, caster, GetSpellInfo()->Id);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;

        int32 totalHealingAmount = aurEff->GetAmount();

        float pct = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        totalHealingAmount = CalculatePct(totalHealingAmount, pct);

        Creature* creature = caster->FindNearestCreature(400406, 100.f);

        if (creature && caster) {
            creature->CastCustomSpell(SPELL_SHAMAN_OUTBURST_HEAL, SPELLVALUE_BASE_POINT0, totalHealingAmount, caster, true, nullptr, nullptr, caster->GetGUID());
            creature->DespawnOrUnsummon();
        }
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 healingAmount = eventInfo.GetHealInfo()->GetHeal();
        int32 currentAmount = aurEff->GetAmount();
        aurEff->GetBase()->GetEffect(EFFECT_0)->SetAmount(healingAmount + currentAmount);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_summon_cloudburst_totem::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_summon_cloudburst_totem::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectApply += AuraEffectApplyFn(spell_sha_summon_cloudburst_totem::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_summon_cloudburst_totem::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }

private:
    TempSummon* summon;
};

class spell_sha_spirit_of_fire : public AuraScript
{
    PrepareAuraScript(spell_sha_spirit_of_fire);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, SPELL_SHAMAN_SPIRIT_OF_FIRE_PASSIVE, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(SPELL_SHAMAN_SPIRIT_OF_FIRE_PASSIVE))
            GetCaster()->RemoveAura(SPELL_SHAMAN_SPIRIT_OF_FIRE_PASSIVE);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_spirit_of_fire::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_spirit_of_fire::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_spirit_of_storm : public AuraScript
{
    PrepareAuraScript(spell_sha_spirit_of_storm);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, SPELL_SHAMAN_SPIRIT_OF_STORM_HASTE, TRIGGERED_FULL_MASK);
        caster->CastSpell(caster, SPELL_SHAMAN_SPIRIT_OF_STORM_PASSIVE, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        caster->RemoveAura(SPELL_SHAMAN_SPIRIT_OF_STORM_HASTE);
        caster->RemoveAura(SPELL_SHAMAN_SPIRIT_OF_STORM_PASSIVE);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_spirit_of_storm::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_spirit_of_storm::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_spirit_of_storm_proc : public AuraScript
{
    PrepareAuraScript(spell_sha_spirit_of_storm_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetSpellInfo() && (eventInfo.GetSpellInfo()->Id == SPELL_SHAMAN_SPIRIT_OF_STORM_PROC || eventInfo.GetSpellInfo()->Id == SPELL_SHAMAN_FOCUS_THINE_FOE_STORM))
            return false;
        return true;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_spirit_of_storm_proc::CheckProc);
    }
};

class spell_sha_spirit_of_water : public AuraScript
{
    PrepareAuraScript(spell_sha_spirit_of_water);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        amount = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), amount);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetSpellInfo() && eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0 && eventInfo.GetActor()->GetGUID() == GetCaster()->GetGUID() && GetCaster()->IsAlive())
            return true;
        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        uint32 healAmount = eventInfo.GetHealInfo()->GetHeal();
        uint32 thresholdPct = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        uint32 shieldThreshold = caster->CountPctFromMaxHealth(thresholdPct);

        if (Aura* aura = caster->GetAura(SPELL_SHAMAN_SPIRIT_OF_WATER_SHIELD))
        {
            uint32 currentShield = aura->GetEffect(EFFECT_0)->GetAmount();
            if (currentShield >= shieldThreshold)
                return;
            aura->GetEffect(EFFECT_0)->ChangeAmount(currentShield + healAmount);
            aura->RefreshDuration();
        }
        else
            caster->CastCustomSpell(SPELL_SHAMAN_SPIRIT_OF_WATER_SHIELD, SPELLVALUE_BASE_POINT0, healAmount, caster, TRIGGERED_FULL_MASK);
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, SPELL_SHAMAN_SPIRIT_OF_WATER_REGEN, TRIGGERED_FULL_MASK);
        caster->CastSpell(caster, SPELL_SHAMAN_SPIRIT_OF_WATER_PASSIVE, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        caster->RemoveAura(SPELL_SHAMAN_SPIRIT_OF_WATER_REGEN);
        caster->RemoveAura(SPELL_SHAMAN_SPIRIT_OF_WATER_PASSIVE);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_spirit_of_water::CheckProc);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_spirit_of_water::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_HEALING_DONE);
        OnEffectProc += AuraEffectProcFn(spell_sha_spirit_of_water::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
        OnEffectApply += AuraEffectApplyFn(spell_sha_spirit_of_water::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_spirit_of_water::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_spirit_master_teacher : public AuraScript
{
    PrepareAuraScript(spell_sha_spirit_master_teacher);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->learnSpell(SPELL_SHAMAN_SPIRIT_OF_FIRE);
        target->learnSpell(SPELL_SHAMAN_SPIRIT_OF_STORM);
        target->learnSpell(SPELL_SHAMAN_SPIRIT_OF_EARTH);
        target->learnSpell(SPELL_SHAMAN_SPIRIT_OF_WATER);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_SHAMAN_SPIRIT_OF_FIRE, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_SHAMAN_SPIRIT_OF_STORM, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_SHAMAN_SPIRIT_OF_EARTH, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_SHAMAN_SPIRIT_OF_WATER, SPEC_MASK_ALL, false);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_spirit_master_teacher::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_spirit_master_teacher::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_invoke_essence : public SpellScript
{
    PrepareSpellScript(spell_sha_invoke_essence);

    Aura* GetSpellWardingEarthAura(Unit* caster)
    {
        for (size_t i = 1001378; i < 1001384; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetSolidBarrierAura(Unit* caster)
    {
        for (size_t i = 1001390; i < 1001396; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();
        ShapeshiftForm form = caster->GetShapeshiftForm();

        if (caster->HasAura(SPELL_SHAMAN_PATH_OF_THE_ASCENDANT) && caster->HasAura(SPELL_SHAMAN_ASCENDANT_EARTH))
        {
            caster->CastSpell(target, SPELL_SHAMAN_INVOKE_ESSENCE_FIRE, TRIGGERED_FULL_MASK);
            caster->CastSpell(caster, SPELL_SHAMAN_INVOKE_ESSENCE_EARTH, TRIGGERED_FULL_MASK);
            caster->CastSpell(caster, SPELL_SHAMAN_INVOKE_ESSENCE_WATER, TRIGGERED_FULL_MASK);
            // Spell Warding Earth Rune Magic damage reduction cast.
            if (GetSpellWardingEarthAura(caster))
            {
                int32 procSpell = GetSpellWardingEarthAura(caster)->GetEffect(EFFECT_0)->GetAmount();
                caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
            }
            // Solid Barrier Rune Armor increase cast.
            if (GetSolidBarrierAura(caster))
            {
                int32 procSpell = GetSolidBarrierAura(caster)->GetEffect(EFFECT_0)->GetAmount();
                caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
            }
        }
        else if (form == FORM_SPIRIT_OF_FIRE)
            caster->CastSpell(target, SPELL_SHAMAN_INVOKE_ESSENCE_FIRE, TRIGGERED_FULL_MASK);
        else if (form == FORM_SPIRIT_OF_EARTH)
        {
            caster->CastSpell(caster, SPELL_SHAMAN_INVOKE_ESSENCE_EARTH, TRIGGERED_FULL_MASK);
            // Spell Warding Earth Rune Magic damage reduction cast.
            if (GetSpellWardingEarthAura(caster))
            {
                int32 procSpell = GetSpellWardingEarthAura(caster)->GetEffect(EFFECT_0)->GetAmount();
                caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
            }
        }
        else if (form == FORM_SPIRIT_OF_WATER)
        {
            caster->CastSpell(caster, SPELL_SHAMAN_INVOKE_ESSENCE_WATER, TRIGGERED_FULL_MASK);
            // Solid Barrier Rune Armor increase cast.
            if (GetSolidBarrierAura(caster))
            {
                int32 procSpell = GetSolidBarrierAura(caster)->GetEffect(EFFECT_0)->GetAmount();
                caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
            }
        }

    }

    void Register()
    {
        OnCast += SpellCastFn(spell_sha_invoke_essence::HandleProc);
    }
};

class spell_sha_invoke_essence_fire : public AuraScript
{
    PrepareAuraScript(spell_sha_invoke_essence_fire);

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 maxStacks = sSpellMgr->AssertSpellInfo(SPELL_SHAMAN_INVOKE_ESSENCE)->GetEffect(EFFECT_1).CalcValue(caster);

        if (GetStackAmount() > maxStacks)
            SetStackAmount(maxStacks);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_invoke_essence_fire::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

class spell_sha_invoke_essence_earth : public AuraScript
{
    PrepareAuraScript(spell_sha_invoke_essence_earth);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1001384; i < 1001390; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_invoke_essence_earth::HandleRemove, EFFECT_0, SPELL_AURA_MOD_RESISTANCE_PCT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_invoke_essence_water : public AuraScript
{
    PrepareAuraScript(spell_sha_invoke_essence_water);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1001396; i < 1001402; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_invoke_essence_water::HandleRemove, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_fury_of_the_elements : public SpellScript
{
    PrepareSpellScript(spell_sha_fury_of_the_elements);

    void HandleProc()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();
        ShapeshiftForm form = caster->GetShapeshiftForm();

        if (caster->HasAura(SPELL_SHAMAN_PATH_OF_THE_ASCENDANT) && caster->HasAura(SPELL_SHAMAN_ASCENDANT_EARTH))
        {
            caster->CastSpell(target, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_FIRE, TRIGGERED_FULL_MASK);
            caster->CastSpell(caster, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_STORM, TRIGGERED_FULL_MASK);
            caster->CastSpell(caster, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_EARTH, TRIGGERED_FULL_MASK);
            caster->CastSpell(caster, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_WATER, TRIGGERED_FULL_MASK);

            // Add T1 2pc bonus
            if (caster->HasAura(T1_SHAMAN_SPIRIT_2PC))
            {
                if (caster->HasAura(T1_SHAMAN_SPIRIT_4PC))
                {
                    caster->AddAura(T1_SHAMAN_SPIRIT_4PC_BUFF_FIRE, caster);
                    caster->AddAura(T1_SHAMAN_SPIRIT_4PC_BUFF_STORM, caster);
                    caster->AddAura(T1_SHAMAN_SPIRIT_4PC_BUFF_EARTH, caster);
                    caster->AddAura(T1_SHAMAN_SPIRIT_4PC_BUFF_WATER, caster);
                }
                else
                {
                    caster->AddAura(T1_SHAMAN_SPIRIT_2PC_BUFF_FIRE, caster);
                    caster->AddAura(T1_SHAMAN_SPIRIT_2PC_BUFF_STORM, caster);
                    caster->AddAura(T1_SHAMAN_SPIRIT_2PC_BUFF_EARTH, caster);
                    caster->AddAura(T1_SHAMAN_SPIRIT_2PC_BUFF_WATER, caster);
                }
            }
        }
        else if (form == FORM_SPIRIT_OF_FIRE)
        {
            caster->CastSpell(target, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_FIRE, TRIGGERED_FULL_MASK);

            // Add T1 2pc bonus
            if (caster->HasAura(T1_SHAMAN_SPIRIT_2PC))
                if (caster->HasAura(T1_SHAMAN_SPIRIT_4PC))
                    caster->AddAura(T1_SHAMAN_SPIRIT_4PC_BUFF_FIRE, caster);
                else
                    caster->AddAura(T1_SHAMAN_SPIRIT_2PC_BUFF_FIRE, caster);
        }
        else if (form == FORM_SPIRIT_OF_STORM)
        {
            caster->CastSpell(caster, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_STORM, TRIGGERED_FULL_MASK);

            // Add T1 2pc bonus
            if (caster->HasAura(T1_SHAMAN_SPIRIT_2PC))
                if (caster->HasAura(T1_SHAMAN_SPIRIT_4PC))
                    caster->AddAura(T1_SHAMAN_SPIRIT_4PC_BUFF_STORM, caster);
                else
                    caster->AddAura(T1_SHAMAN_SPIRIT_2PC_BUFF_STORM, caster);
        }
        else if (form == FORM_SPIRIT_OF_EARTH)
        {
            caster->CastSpell(caster, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_EARTH, TRIGGERED_FULL_MASK);

            // Add T1 2pc bonus
            if (caster->HasAura(T1_SHAMAN_SPIRIT_2PC))
                if (caster->HasAura(T1_SHAMAN_SPIRIT_4PC))
                    caster->AddAura(T1_SHAMAN_SPIRIT_4PC_BUFF_EARTH, caster);
                else
                    caster->AddAura(T1_SHAMAN_SPIRIT_2PC_BUFF_EARTH, caster);
        }
        else if (form == FORM_SPIRIT_OF_WATER)
        {
            caster->CastSpell(caster, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_WATER, TRIGGERED_FULL_MASK);

            // Add T1 2pc bonus
            if (caster->HasAura(T1_SHAMAN_SPIRIT_2PC))
                if (caster->HasAura(T1_SHAMAN_SPIRIT_4PC))
                    caster->AddAura(T1_SHAMAN_SPIRIT_4PC_BUFF_WATER, caster);
                else
                    caster->AddAura(T1_SHAMAN_SPIRIT_2PC_BUFF_WATER, caster);
        }
    }

    void Register()
    {
        OnCast += SpellCastFn(spell_sha_fury_of_the_elements::HandleProc);
    }
};

class spell_sha_fury_of_the_elements_fire : public SpellScript
{
    PrepareSpellScript(spell_sha_fury_of_the_elements_fire);

    Aura* GetFireExpertAura(Unit* caster)
    {
        for (size_t i = 1001298; i < 1001304; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleHit(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        int32 damage = GetHitDamage();

        if (Unit* target = GetHitUnit())
        {
            if (auto* aurEff = target->GetAura(SPELL_SHAMAN_INVOKE_ESSENCE_FIRE, caster->GetGUID()))
            {
                int32 stackAmount = aurEff->GetStackAmount();
                uint32 damagePct = GetSpellInfo()->GetEffect(EFFECT_1).CalcValue(caster);

                if (!GetFireExpertAura(caster))
                    aurEff->Remove();

                damagePct *= stackAmount;
                AddPct(damage, damagePct);
            }
        }

        if (GetFireExpertAura(caster))
        {
            int32 damagePct = 100 - GetFireExpertAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            ApplyPct(damage, damagePct);
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_fury_of_the_elements_fire::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_sha_fury_of_the_elements_storm : public SpellScript
{
    PrepareSpellScript(spell_sha_fury_of_the_elements_storm);

    Aura* GetLingeringStormAura(Unit* caster)
    {
        for (size_t i = 1001304; i < 1001310; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleBeforeCast()
    {
        Unit* caster = GetCaster();
        ShapeshiftForm form = caster->GetShapeshiftForm();

        if (GetLingeringStormAura(caster))
        {
            int32 buffSpell = GetLingeringStormAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            if (caster->HasAura(buffSpell))
                caster->RemoveAura(buffSpell);
        }
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_STORM);
        uint32 targetSize = spell->GetEffect(EFFECT_1).CalcValue(caster);

        if (targets.size() >= targetSize)
        {
            caster->CastSpell(caster, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_STORM_ECHO, TRIGGERED_FULL_MASK);
        }

        // Grants Fire Expert Rune damage buff, x% per target hit.
        if (GetLingeringStormAura(caster))
        {
            int32 buffSpell = GetLingeringStormAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            caster->CastSpell(caster, buffSpell, TRIGGERED_FULL_MASK);
            caster->GetAura(buffSpell)->SetStackAmount(targets.size());
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_sha_fury_of_the_elements_storm::HandleBeforeCast);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_fury_of_the_elements_storm::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_sha_fury_of_the_elements_earth : public SpellScript
{
    PrepareSpellScript(spell_sha_fury_of_the_elements_earth);

    Aura* GetEarthsGraspAura(Unit* caster)
    {
        for (size_t i = 1001316; i < 1001322; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void FindTargets(std::list<WorldObject*>& targets)
    {
        Unit* initialTarget = GetCaster();
        Position targetPos = initialTarget->GetPosition();
        if (targets.size() > 0)
        {
            for (auto const& target : targets)
                if (Creature* creatureTarget = target->ToCreature())
                {
                    if (CreatureTemplate const* cinfo = sObjectMgr->GetCreatureTemplate(creatureTarget->GetEntry()))
                        if (cinfo->MechanicImmuneMask & 32)
                            return;

                    if (!creatureTarget->isWorldBoss() || !creatureTarget->IsDungeonBoss())
                    {
                        float distance = target->GetDistance(initialTarget);
                        if (distance <= 8)
                            if (Unit* unit = target->ToUnit())
                                unit->CastSpell(initialTarget, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS_EARTH_GRIP, TRIGGERED_FULL_MASK);
                    }
                }

            if (GetEarthsGraspAura(initialTarget))
            {
                int32 buffSpell = GetEarthsGraspAura(initialTarget)->GetEffect(EFFECT_0)->GetAmount();

                for (auto const& object : targets)
                {
                    Unit* target = object->ToUnit();

                    if (target->isDead())
                        continue;

                    initialTarget->CastSpell(target, buffSpell, TRIGGERED_FULL_MASK);
                    initialTarget->CastSpell(target, RUNE_SHAMAN_EARTHS_GRASP_SLOW, TRIGGERED_FULL_MASK);
                }
            }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_fury_of_the_elements_earth::FindTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_sha_ascendance_earth : public AuraScript
{
    PrepareAuraScript(spell_sha_ascendance_earth);

    Aura* GetOathOfTheFarSeerAura(Unit* caster)
    {
        for (size_t i = 1000436; i < 1000442; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetOathOfTheFarSeerAura(caster))
            caster->AddAura(GetOathOfTheFarSeerAura(caster)->GetEffect(EFFECT_1)->GetAmount(), caster);
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = GetUnitOwner()->CountPctFromMaxHealth(amount);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1000442; i < 1000448; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_ascendance_earth::HandleApply, EFFECT_2, SPELL_AURA_MOD_INCREASE_HEALTH, AURA_EFFECT_HANDLE_REAL);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_ascendance_earth::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_INCREASE_HEALTH);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_ascendance_earth::HandleRemove, EFFECT_2, SPELL_AURA_MOD_INCREASE_HEALTH, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_focus_thine_foe : public AuraScript
{
    PrepareAuraScript(spell_sha_focus_thine_foe);

    Aura* GetPropagatingFireAura(Unit* caster)
    {
        for (size_t i = 1001186; i < 1001192; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetRockSkinAura(Unit* caster)
    {
        for (size_t i = 1001208; i < 1001214; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetProtectiveFocusAura(Unit* caster)
    {
        for (size_t i = 1001232; i < 1001238; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetFocusOfTheElementsAura(Unit* caster)
    {
        for (size_t i = 1001262; i < 1001268; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        ShapeshiftForm form = caster->GetShapeshiftForm();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        if (form == FORM_SPIRIT_OF_FIRE)
        {
            if (GetPropagatingFireAura(caster))
            {
                int32 damage = 0;
                int32 damagePct = GetPropagatingFireAura(caster)->GetEffect(EFFECT_0)->GetAmount();

                auto targetAuras = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);

                targetAuras.remove_if([&](AuraEffect const* effect) -> bool
                    {
                        return effect->GetCasterGUID() != GetCaster()->GetGUID();
                    });

                for (auto itj = targetAuras.begin(); itj != targetAuras.end(); ++itj) {
                    if (AuraEffect const* effect = (*itj))
                    {
                        if (effect->GetBase()->GetId() == GetId())
                            continue;

                        int32 remainingTicks = effect->GetRemaningTicks();
                        int32 amount = remainingTicks * effect->GetAmount();
                        damage += amount;
                        effect->GetBase()->Remove();
                    }
                }
                int32 amount = CalculatePct(damage, damagePct);
                caster->CastCustomSpell(RUNE_SHAMAN_PROPAGATING_FIRE_LISTENER, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
                target->GetAura(RUNE_SHAMAN_PROPAGATING_FIRE_LISTENER)->SetDuration(GetDuration());
            }
        }

        if (form == FORM_SPIRIT_OF_EARTH)
        {
            if (GetRockSkinAura(caster))
            {
                int32 procSpell = GetRockSkinAura(caster)->GetEffect(EFFECT_0)->GetAmount();
                target->AddAura(procSpell, caster);
            }
        }

    }

    void OnPeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetTarget();

        if (!target || target->isDead())
            return;

        ShapeshiftForm form = caster->GetShapeshiftForm();

        if (caster->HasAura(SPELL_SHAMAN_PATH_OF_THE_ASCENDANT) && caster->HasAura(SPELL_SHAMAN_ASCENDANT_EARTH))
        {
            caster->CastSpell(target, SPELL_SHAMAN_FOCUS_THINE_FOE_STORM, TRIGGERED_FULL_MASK);
            caster->CastSpell(target, SPELL_SHAMAN_FOCUS_THINE_FOE_EARTH, TRIGGERED_FULL_MASK);
            caster->CastSpell(caster, SPELL_SHAMAN_FOCUS_THINE_FOE_WATER, TRIGGERED_FULL_MASK);
        }
        else if (form == FORM_SPIRIT_OF_STORM)
            caster->CastSpell(target, SPELL_SHAMAN_FOCUS_THINE_FOE_STORM, TRIGGERED_FULL_MASK);
        else if (form == FORM_SPIRIT_OF_EARTH)
            caster->CastSpell(target, SPELL_SHAMAN_FOCUS_THINE_FOE_EARTH, TRIGGERED_FULL_MASK);
        else if (form == FORM_SPIRIT_OF_WATER)
            caster->CastSpell(caster, SPELL_SHAMAN_FOCUS_THINE_FOE_WATER, TRIGGERED_FULL_MASK);
        else if (form == FORM_SPIRIT_OF_FIRE)
        {
            if (Aura* propagatingFireListener = target->GetAura(RUNE_SHAMAN_PROPAGATING_FIRE_LISTENER))
            {
                int32 amount = propagatingFireListener->GetEffect(EFFECT_0)->GetAmount();
                caster->CastCustomSpell(RUNE_SHAMAN_PROPAGATING_FIRE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        ShapeshiftForm form = caster->GetShapeshiftForm();

        // remove Rock Skin Rune Buffs
        for (size_t i = 1001214; i < 1001220; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }

        // Manual remove of Lingering Storm Rune buff
        if (caster->HasAura(RUNE_SHAMAN_LINGERING_STORM_BUFF))
            caster->RemoveAura(RUNE_SHAMAN_LINGERING_STORM_BUFF);

        if (GetProtectiveFocusAura(caster))
        {
            int32 procSpell = 0;

            if (form == FORM_SPIRIT_OF_FIRE)
                procSpell = GetProtectiveFocusAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            else if (form == FORM_SPIRIT_OF_STORM)
                procSpell = GetProtectiveFocusAura(caster)->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
            else if (form == FORM_SPIRIT_OF_EARTH)
                procSpell = GetProtectiveFocusAura(caster)->GetEffect(EFFECT_1)->GetAmount();
            else if (form == FORM_SPIRIT_OF_WATER)
                procSpell = GetProtectiveFocusAura(caster)->GetEffect(EFFECT_2)->GetAmount();

            if (procSpell != 0)
                caster->AddAura(procSpell, caster);
        }

        if (GetFocusOfTheElementsAura(caster))
        {
            int32 procSpell = 0;

            if (form == FORM_SPIRIT_OF_FIRE)
                procSpell = GetFocusOfTheElementsAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            else if (form == FORM_SPIRIT_OF_STORM)
                procSpell = GetFocusOfTheElementsAura(caster)->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
            else if (form == FORM_SPIRIT_OF_EARTH)
                procSpell = GetFocusOfTheElementsAura(caster)->GetEffect(EFFECT_1)->GetAmount();
            else if (form == FORM_SPIRIT_OF_WATER)
                procSpell = GetFocusOfTheElementsAura(caster)->GetEffect(EFFECT_2)->GetAmount();

            if (procSpell != 0)
                caster->AddAura(procSpell, caster);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_focus_thine_foe::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_focus_thine_foe::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_focus_thine_foe::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_focus_thine_foe_elemental : public AuraScript
{
    PrepareAuraScript(spell_sha_focus_thine_foe_elemental);

    void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->GetShapeshiftForm() == FORM_SPIRIT_OF_FIRE)
        {
            Unit* target = GetTarget();

            if (!target)
                return;

            uint32 damage = aurEff->GetAmount();
            uint32 damagePct = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();

            if (target->HasAura(SPELL_SHAMAN_FLAME_SHOCK) && target->HasAura(SPELL_SHAMAN_INVOKE_ESSENCE_FIRE))
            {
                damagePct *= 2;
                uint32 damageIncrease = CalculatePct(damage, damagePct);
                damage += damageIncrease;
                GetAura()->GetEffect(EFFECT_0)->ChangeAmount(damage);
                GetAura()->GetEffect(EFFECT_0)->RecalculateAmount(GetCaster());
            }
            else if (target->HasAura(SPELL_SHAMAN_FLAME_SHOCK) || target->HasAura(SPELL_SHAMAN_INVOKE_ESSENCE_FIRE))
            {
                uint32 damageIncrease = CalculatePct(damage, damagePct);
                damage += damageIncrease;
                GetAura()->GetEffect(EFFECT_0)->ChangeAmount(damage);
                GetAura()->GetEffect(EFFECT_0)->RecalculateAmount(GetCaster());
            }
        }
    }

    void OnPeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->GetShapeshiftForm() == FORM_SPIRIT_OF_STORM)
            caster->CastSpell(GetTarget(), SPELL_SHAMAN_FOCUS_THINE_FOE_STORM, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_focus_thine_foe_elemental::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_focus_thine_foe_elemental::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

class spell_sha_fiery_stamp : public AuraScript
{
    PrepareAuraScript(spell_sha_fiery_stamp);

    Aura* GetDeadlyStampAura(Unit* caster)
    {
        for (size_t i = 1001160; i < 1001166; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        // cast Deadly Stamp Rune Listener
        if (GetDeadlyStampAura(caster))
            caster->AddAura(RUNE_SHAMAN_DEADLY_STAMP_LISTENER, target);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return false;

        if (!eventInfo.GetActor())
            return false;

        if (eventInfo.GetActor()->GetGUID() == GetCaster()->GetGUID() && GetCaster()->GetShapeshiftForm() == FORM_SPIRIT_OF_FIRE)
            return true;
        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastSpell(GetTarget(), SPELL_SHAMAN_FIERY_STAMP_PROC, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // remove Deadly Stamp Rune listener
        if (caster->HasAura(RUNE_SHAMAN_DEADLY_STAMP_LISTENER))
            caster->RemoveAura(RUNE_SHAMAN_DEADLY_STAMP_LISTENER);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_fiery_stamp::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        DoCheckProc += AuraCheckProcFn(spell_sha_fiery_stamp::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_fiery_stamp::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_fiery_stamp::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_tectonic_shift : public SpellScript
{
    PrepareSpellScript(spell_sha_tectonic_shift);

    void HandleProc()
    {
        Unit* caster = GetCaster();
        ShapeshiftForm form = caster->GetShapeshiftForm();
        if (form == FORM_SPIRIT_OF_EARTH)
            caster->CastSpell(caster, SPELL_SHAMAN_TECTONIC_SHIFT_DEBUFF, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnCast += SpellCastFn(spell_sha_tectonic_shift::HandleProc);
    }
};

class spell_sha_watery_grave : public AuraScript
{
    PrepareAuraScript(spell_sha_watery_grave);

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* victim = GetTarget();

        if (!victim || victim->isDead())
            return;

        int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
        int32 healPct = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (victim->GetShapeshiftForm() == FORM_SPIRIT_OF_WATER)
            healPct = GetAura()->GetEffect(EFFECT_2)->GetAmount();

        if (remainingHealth <= 0)
        {
            absorbAmount = dmgInfo.GetDamage();
            int32 healAmount = int32(victim->CountPctFromMaxHealth(healPct));

            victim->CastCustomSpell(SPELL_SHAMAN_WATERY_GRAVE_HEAL, SPELLVALUE_BASE_POINT0, healAmount, victim, true, nullptr, aurEff);
            victim->CastSpell(victim, SPELL_SHAMAN_WATERY_GRAVE_BUBBLE, TRIGGERED_FULL_MASK);
        }
        else
            absorbAmount = 0;
    }

    void Register() override
    {
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_sha_watery_grave::Absorb, EFFECT_0);
    }
};

class spell_sha_overcharge : public SpellScript
{
    PrepareSpellScript(spell_sha_overcharge);

    void HandleProc()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();
        ShapeshiftForm form = caster->GetShapeshiftForm();

        if (caster->HasAura(SPELL_SHAMAN_PATH_OF_THE_ASCENDANT) && caster->HasAura(SPELL_SHAMAN_ASCENDANT_EARTH))
        {
            caster->CastSpell(caster, SPELL_SHAMAN_OVERCHARGE_FIRE, TRIGGERED_FULL_MASK);
            caster->CastSpell(caster, SPELL_SHAMAN_OVERCHARGE_STORM, TRIGGERED_FULL_MASK);
            caster->CastSpell(caster, SPELL_SHAMAN_OVERCHARGE_EARTH, TRIGGERED_FULL_MASK);
            caster->CastSpell(caster, SPELL_SHAMAN_OVERCHARGE_WATER, TRIGGERED_FULL_MASK);
        }
        else if (form == FORM_SPIRIT_OF_FIRE)
            caster->CastSpell(caster, SPELL_SHAMAN_OVERCHARGE_FIRE, TRIGGERED_FULL_MASK);
        else if (form == FORM_SPIRIT_OF_STORM)
            caster->CastSpell(caster, SPELL_SHAMAN_OVERCHARGE_STORM, TRIGGERED_FULL_MASK);
        else if (form == FORM_SPIRIT_OF_EARTH)
            caster->CastSpell(caster, SPELL_SHAMAN_OVERCHARGE_EARTH, TRIGGERED_FULL_MASK);
        else if (form == FORM_SPIRIT_OF_WATER)
            caster->CastSpell(caster, SPELL_SHAMAN_OVERCHARGE_WATER, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnCast += SpellCastFn(spell_sha_overcharge::HandleProc);
    }
};

class spell_sha_unshakable_earth : public AuraScript
{
    PrepareAuraScript(spell_sha_unshakable_earth);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (GetCaster()->GetShapeshiftForm() == FORM_SPIRIT_OF_EARTH)
            return true;
        return false;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_unshakable_earth::CheckProc);
    }
};

class spell_sha_seamless_water : public AuraScript
{
    PrepareAuraScript(spell_sha_seamless_water);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (GetCaster()->GetShapeshiftForm() == FORM_SPIRIT_OF_WATER && !GetCaster()->HasAura(SPELL_SHAMAN_SEAMLESS_WATER_PROC))
            return true;
        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        uint32 requiredStacks = aurEff->GetAmount();
        if (caster->GetShapeshiftForm() == FORM_SPIRIT_OF_WATER || caster->GetAuraEffectOfRankedSpell(TALENT_SHAMAN_SEAMLESS_WATER, EFFECT_0))
        {
            if (Aura* stackAura = caster->GetAura(SPELL_SHAMAN_SEAMLESS_WATER_STACKS))
            {
                uint32 currentStacks = stackAura->GetStackAmount();

                if (currentStacks < requiredStacks)
                    caster->AddAura(SPELL_SHAMAN_SEAMLESS_WATER_STACKS, caster);
                else if (currentStacks == requiredStacks)
                {
                    caster->RemoveAura(SPELL_SHAMAN_SEAMLESS_WATER_STACKS);
                    caster->AddAura(SPELL_SHAMAN_SEAMLESS_WATER_PROC, caster);
                }
            }
            else
                caster->AddAura(SPELL_SHAMAN_SEAMLESS_WATER_STACKS, caster);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(SPELL_SHAMAN_SEAMLESS_WATER_STACKS))
            GetCaster()->RemoveAura(SPELL_SHAMAN_SEAMLESS_WATER_STACKS);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_seamless_water::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_seamless_water::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_seamless_water::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_water_bending : public AuraScript
{
    PrepareAuraScript(spell_sha_water_bending);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return false;

        if (GetCaster()->GetShapeshiftForm() == FORM_SPIRIT_OF_WATER && eventInfo.GetActionTarget() == GetCaster())
            return true;

        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        int32 heal = eventInfo.GetHealInfo()->GetHeal();
        uint32 healPct = aurEff->GetAmount();
        uint32 healAmount = CalculatePct(heal, healPct);
        if (heal && caster->IsAlive())
            caster->CastCustomSpell(SPELL_SHAMAN_WATER_BENDING_PROC, SPELLVALUE_BASE_POINT0, healAmount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_water_bending::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_water_bending::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sha_fire_proficiency : public AuraScript
{
    PrepareAuraScript(spell_sha_fire_proficiency);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return false;

        if (GetCaster()->GetShapeshiftForm() == FORM_SPIRIT_OF_FIRE)
            return true;
        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        uint32 requiredStacks = aurEff->GetAmount();
        int32 damage = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), 10) + CalculatePct(caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE), 10);
        if (Aura* stackAura = caster->GetAura(SPELL_SHAMAN_FIRE_PROFICIENCY_STACKS))
        {
            uint32 currentStacks = stackAura->GetStackAmount();

            if (currentStacks < requiredStacks)
                caster->AddAura(SPELL_SHAMAN_FIRE_PROFICIENCY_STACKS, caster);
            else if (currentStacks == requiredStacks)
            {
                caster->RemoveAura(SPELL_SHAMAN_FIRE_PROFICIENCY_STACKS);
                caster->CastCustomSpell(eventInfo.GetActionTarget(), SPELL_SHAMAN_FIRE_PROFICIENCY_PROC, &damage, &damage, nullptr, true);
            }
        }
        else
            caster->AddAura(SPELL_SHAMAN_FIRE_PROFICIENCY_STACKS, caster);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_fire_proficiency::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_fire_proficiency::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sha_earth_proficiency : public AuraScript
{
    PrepareAuraScript(spell_sha_earth_proficiency);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (GetCaster()->GetShapeshiftForm() == FORM_SPIRIT_OF_EARTH)
            return true;
        return false;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_earth_proficiency::CheckProc);
    }
};

class spell_sha_regenerative_aura : public AuraScript
{
    PrepareAuraScript(spell_sha_regenerative_aura);


    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->GetShapeshiftForm() == FORM_NONE)
            caster->CastSpell(caster, SPELL_SHAMAN_REGENERATIVE_AURA_HEAL, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sha_regenerative_aura::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_sha_accumulation : public AuraScript
{
    PrepareAuraScript(spell_sha_accumulation);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage && caster->IsAlive())
            {
                int32 damagePct = aurEff->GetAmount();
                int32 damageAmount = CalculatePct(damage, damagePct);;
                int32 accumulated = GetAura()->GetEffect(EFFECT_1)->GetAmount();
                uint32 threshold = caster->CountPctFromMaxHealth(50);
                if (accumulated < threshold)
                    GetAura()->GetEffect(EFFECT_1)->ChangeAmount(accumulated + damageAmount);
            }
        }

        if (eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0)
        {
            int32 heal = eventInfo.GetHealInfo()->GetHeal();
            if (heal && caster->IsAlive())
            {
                int32 accumulated = GetAura()->GetEffect(EFFECT_1)->GetAmount();
                caster->CastCustomSpell(SPELL_SHAMAN_ACCUMULATION_HEAL, SPELLVALUE_BASE_POINT0, accumulated, caster, TRIGGERED_FULL_MASK);
                GetAura()->GetEffect(EFFECT_1)->SetAmount(0);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sha_accumulation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sha_seeping_life_force : public AuraScript
{
    PrepareAuraScript(spell_sha_seeping_life_force);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage)
            {
                int32 healPct = aurEff->GetAmount();
                int32 healAmount = CalculatePct(damage, healPct);
                GetCaster()->CastCustomSpell(SPELL_SHAMAN_SEEPING_LIFE_FORCE_HEAL, SPELLVALUE_BASE_POINT0, healAmount, GetCaster(), TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sha_seeping_life_force::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sha_stormbrand_totem : public AuraScript
{
    PrepareAuraScript(spell_sha_stormbrand_totem);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(83);
        int32 duration = GetSpellInfo()->GetDuration();
        Position pos = caster->GetNearPosition(3.f, 0);
        caster->GetMap()->SummonCreature(400407, pos, properties, duration + 1000, caster, GetSpellInfo()->Id);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (caster->isDead() || !caster)
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        float damagePct = aurEff->GetAmount();

        if (caster->GetShapeshiftForm() == FORM_SPIRIT_OF_STORM)
            damagePct = 100.f;

        int32 damageAmount = CalculatePct(damage, damagePct);

        Creature* summon = caster->FindNearestCreature(400407, 100.f);

        if (summon && caster) {
            summon->CastCustomSpell(SPELL_SHAMAN_STORMBRAND_TOTEM_PROC, SPELLVALUE_BASE_POINT0, damageAmount, caster, true, nullptr, nullptr, caster->GetGUID());
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_sha_stormbrand_totem::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_sha_stormbrand_totem::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectApply += AuraEffectApplyFn(spell_sha_stormbrand_totem::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }

};

class spell_sha_spirit_weapons : public AuraScript
{
    PrepareAuraScript(spell_sha_spirit_weapons);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            Unit* caster = GetCaster();

            if (!caster || caster->isDead())
                return;

            if (!eventInfo.GetActionTarget() || eventInfo.GetActionTarget()->isDead())
                return;

            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            int32 damagePct = aurEff->GetAmount();
            int32 damageAmount = CalculatePct(damage, damagePct);
            caster->CastCustomSpell(SPELL_SHAMAN_SPIRIT_WEAPON_NATURE, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
            caster->CastCustomSpell(SPELL_SHAMAN_SPIRIT_WEAPON_FIRE, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
            caster->CastCustomSpell(SPELL_SHAMAN_SPIRIT_WEAPON_FROST, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sha_spirit_weapons::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

//84004 - Healing Surge
class spell_sha_healing_surge : public SpellScript
{
    PrepareSpellScript(spell_sha_healing_surge);

    Aura* GetRefreshingWatersAura(Unit* caster)
    {
        for (size_t i = 1000294; i < 1000300; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleHeal(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetHitUnit();

        if (!target || target->isDead())
            return;

        int32 heal = GetHitHeal();

        if (heal == 0)
            return;

        // Refreshing Waters Rune effects
        if (GetRefreshingWatersAura(caster) && target == caster)
        {
            int32 healPct = GetRefreshingWatersAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            AddPct(heal, healPct);
        }

        SetHitHeal(heal);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_sha_healing_surge::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
    }
};

// 2645 - Ghost Wolf
class spell_sha_ghost_wolf : public SpellScript
{
    PrepareSpellScript(spell_sha_ghost_wolf);

    Aura* GetThunderousPawsAura(Unit* caster)
    {
        for (size_t i = 1000368; i < 1000374; i++)
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

        // Thunderous Paws Rune effects
        if (GetThunderousPawsAura(caster))
        {
            int32 buffSpell = GetThunderousPawsAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            int32 preventSpell = GetThunderousPawsAura(caster)->GetEffect(EFFECT_1)->GetAmount();

            if (!caster->HasAura(preventSpell))
            {
                caster->AddAura(buffSpell, caster);
                caster->AddAura(preventSpell, caster);
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_sha_ghost_wolf::HandleCast);
    }
};

// 84064 - Capacitor Totem Stun
class spell_sha_capacitor_totem_stun : public SpellScript
{
    PrepareSpellScript(spell_sha_capacitor_totem_stun);

    Aura* GetStaticChargeAura(Unit* caster)
    {
        for (size_t i = 1000416; i < 1000422; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* totem = GetCaster();

        if (!totem)
            return;

        Unit* caster = totem->GetOwner();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            // Static Charge Rune effects
            if (GetStaticChargeAura(caster))
            {
                int32 targetNbr = std::min<uint32>(targets.size(), GetStaticChargeAura(caster)->GetEffect(EFFECT_1)->GetAmount());
                int32 cooldown = GetStaticChargeAura(caster)->GetEffect(EFFECT_0)->GetAmount() * targetNbr;
                player->ModifySpellCooldown(SPELL_SHAMAN_CAPACITOR_TOTEM, -cooldown);
            }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_capacitor_totem_stun::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// 84072 - Capacitor Totem Aura
class spell_sha_capacitor_totem_aura : public AuraScript
{
    PrepareAuraScript(spell_sha_capacitor_totem_aura);

    Aura* GetGuardiansCudgelAura(Unit* caster)
    {
        for (size_t i = 1000422; i < 1000428; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* totem = GetCaster();

        if (!totem)
            return;

        Position totemPos = totem->GetPosition();
        Unit* caster = totem->GetOwner();

        if (!caster || caster->isDead())
            return;

        if (GetGuardiansCudgelAura(caster))
        {
            if (caster->HasAura(RUNE_SHAMAN_GUARDIANS_CUDGEL_DEBUFF))
                return;

            int32 procChance = GetGuardiansCudgelAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            if (!roll_chance_i(procChance))
                return;


            SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(81);
            caster->GetMap()->SummonCreature(400402, totemPos, properties, 3000, caster, GetSpellInfo()->Id);
            caster->AddAura(RUNE_SHAMAN_GUARDIANS_CUDGEL_DEBUFF, caster);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_capacitor_totem_aura::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

// 53817 - Maelstrom Weapon (Buff)
class spell_sha_maelstrom_weapon_buff : public AuraScript
{
    PrepareAuraScript(spell_sha_maelstrom_weapon_buff);

    Aura* GetLegacyOfTheFrostWitchAura(Unit* caster)
    {
        for (size_t i = 1000804; i < 1000810; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetWitchDoctorsAncestryAura(Unit* caster)
    {
        for (size_t i = 1000818; i < 1000824; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Aura* maelstromWeaponAura = caster->GetAura(SPELL_SHAMAN_MAELSTROM_WEAPON))
        {
            int32 maxStacks = maelstromWeaponAura->GetEffect(EFFECT_1)->GetAmount();

            if (GetStackAmount() > maxStacks)
                SetStackAmount(maxStacks);
        }

        if (Player* player = caster->ToPlayer())
        {
            if (GetWitchDoctorsAncestryAura(player))
            {
                int32 cooldown = GetWitchDoctorsAncestryAura(player)->GetEffect(EFFECT_1)->GetAmount();
                player->ModifySpellCooldown(SPELL_SHAMAN_FERAL_SPIRIT, -cooldown);
            }
        }
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            // Legacy of the Frost Witch rune
            if (GetLegacyOfTheFrostWitchAura(player))
            {
                int32 stackThreshold = GetLegacyOfTheFrostWitchAura(player)->GetEffect(EFFECT_0)->GetAmount();
                int32 stackAmount = GetStackAmount();
                int32 procSpell = GetLegacyOfTheFrostWitchAura(player)->GetEffect(EFFECT_1)->GetAmount();

                if (Aura* LegacyFrostWitchBuff = player->GetAura(RUNE_SHAMAN_LEGACY_OF_THE_FROST_WITCH_LISTENER))
                {
                    stackAmount += LegacyFrostWitchBuff->GetStackAmount();

                    if (stackAmount < stackThreshold)
                        LegacyFrostWitchBuff->SetStackAmount(stackAmount);
                    else
                    {
                        player->AddAura(procSpell, player);
                        player->RemoveSpellCooldown(SPELL_SHAMAN_STORMSTRIKE, true);
                        stackAmount -= stackThreshold;

                        if (stackAmount == 0)
                            LegacyFrostWitchBuff->Remove();
                        else
                            LegacyFrostWitchBuff->SetStackAmount(stackAmount);
                    }
                }
                else
                {
                    caster->AddAura(RUNE_SHAMAN_LEGACY_OF_THE_FROST_WITCH_LISTENER, caster);
                    caster->GetAura(RUNE_SHAMAN_LEGACY_OF_THE_FROST_WITCH_LISTENER)->SetStackAmount(stackAmount);
                }
            }

            // T1 4pc buff handling
            if (player->HasAura(T1_SHAMAN_ENH_4PC))
            {
                int32 stackAmount = GetStackAmount();

                caster->CastCustomSpell(T1_SHAMAN_ENH_4PC_BUFF, SPELLVALUE_AURA_STACK, stackAmount, player, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_maelstrom_weapon_buff::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_maelstrom_weapon_buff::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_riptide : public AuraScript
{
    PrepareAuraScript(spell_sha_riptide);

    Aura* GetUndercurrentAura(Unit* caster)
    {
        for (size_t i = 1000998; i < 1001004; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetAura()->GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetUndercurrentAura(caster))
        {
            int32 procSpell = GetUndercurrentAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            for (size_t i = 1001004; i < 1001010; i++)
            {
                if (i == procSpell)
                    continue;

                if (caster->HasAura(i))
                    caster->RemoveAura(i);
            }

            if (Aura* buffAura = caster->GetAura(procSpell))
                buffAura->ModStackAmount(1);
            else
                caster->AddAura(procSpell, caster);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetAura()->GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetUndercurrentAura(caster))
        {
            int32 procSpell = GetUndercurrentAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            for (size_t i = 1001004; i < 1001010; i++)
            {
                if (i == procSpell)
                    continue;

                if (caster->HasAura(i))
                    caster->RemoveAura(i);
            }

            if (Aura* buffAura = caster->GetAura(procSpell))
                buffAura->ModStackAmount(-1);
        }

        if (!GetUndercurrentAura(caster))
        {
            for (size_t i = 1001004; i < 1001010; i++)
            {
                if (caster->HasAura(i))
                    caster->RemoveAura(i);
            }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_riptide::HandleProc, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_riptide::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
    }
};

// 84037 - Healing Rain
class spell_sha_healing_rain : public SpellScript
{
    PrepareSpellScript(spell_sha_healing_rain);

    Aura* GetOverflowingShoresAura(Unit* caster)
    {
        for (size_t i = 1001052; i < 1001058; i++)
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

        // Overflowing Shores Rune effects
        if (GetOverflowingShoresAura(caster))
        {
            int32 procSpell = GetOverflowingShoresAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            Position pos = GetExplTargetDest()->GetPosition();

            caster->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), procSpell, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_sha_healing_rain::HandleCast);
    }
};

// 84037 - Healing Rain Periodic
class spell_sha_healing_rain_periodic : public AuraScript
{
    PrepareAuraScript(spell_sha_healing_rain_periodic);

    Aura* GetAcidRainAura(Unit* caster)
    {
        for (size_t i = 1001064; i < 1001070; i++)
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

        Unit* target = GetTarget();

        if (!target || target->isDead())
            return;

        // Acid Rain Rune effects
        if (GetAcidRainAura(caster))
        {
            int32 procSpell = GetAcidRainAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            caster->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_healing_rain_periodic::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 84078 - Healing Tide Totem Heal
class spell_sha_healing_tide_totem_heal : public SpellScript
{
    PrepareSpellScript(spell_sha_healing_tide_totem_heal);

    Aura* GetTideTurnerAura(Unit* caster)
    {
        for (size_t i = 1001088; i < 1001094; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* totem = GetCaster();

        if (!totem || totem->isDead())
            return;

        Unit* caster = totem->GetOwner();

        if (!caster || caster->isDead())
            return;

        if (GetTideTurnerAura(caster))
        {
            std::list<WorldObject*> tideTurnerList = targets;
            tideTurnerList.sort(Acore::HealthPctOrderPred());
            tideTurnerList.resize(1);
            int32 buffAura = GetTideTurnerAura(caster)->GetEffect(EFFECT_1)->GetAmount();

            for (auto const& object : tideTurnerList)
            {
                Unit* target = object->ToUnit();

                caster->AddAura(buffAura, target);
            }
        }
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* totem = GetCaster();

        if (!totem || totem->isDead())
            return;

        Unit* caster = totem->GetOwner();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetHitUnit();

        if (!target || target->isDead())
            return;

        int32 heal = GetHitHeal();

        if (GetTideTurnerAura(caster))
        {
            int32 procSpell = GetTideTurnerAura(caster)->GetEffect(EFFECT_1)->GetAmount();
            int32 healPct = GetTideTurnerAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            if (target->HasAura(procSpell))
                AddPct(heal, healPct);
        }

        SetHitHeal(heal);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_healing_tide_totem_heal::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        OnEffectHitTarget += SpellEffectFn(spell_sha_healing_tide_totem_heal::HandleDummy, EFFECT_0, SPELL_EFFECT_HEAL);
    }
private:
    Unit* lowestHealthTarget;
};

// Tidal Waves Buff : 53390, script on Healing Wave : 49273 and Healing Surge : 84004
class spell_sha_tidal_wave_consumed : public SpellScript
{
    PrepareSpellScript(spell_sha_tidal_wave_consumed);

    Aura* GetFlashFloodAura(Unit* caster)
    {
        for (size_t i = 1001124; i < 1001130; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetWaterTotemMasteryAura(Unit* caster)
    {
        for (size_t i = 1001118; i < 1001124; i++)
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

        if (!caster->HasAura(TALENT_SHAMAN_TIDAL_WAVES_BUFF))
            return;

        if (GetFlashFloodAura(caster))
        {
            int32 procSpell = GetFlashFloodAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            caster->AddAura(procSpell, caster);
        }

        if (Player* player = caster->ToPlayer())
        {
            if (GetWaterTotemMasteryAura(caster))
            {
                int32 cooldown = GetWaterTotemMasteryAura(player)->GetEffect(EFFECT_0)->GetAmount();
                player->ModifySpellCooldown(SPELL_SHAMAN_CLOUDBURST_TOTEM, -cooldown);
                player->ModifySpellCooldown(SPELL_SHAMAN_HEALING_TIDE_TOTEM, -cooldown);
                player->ModifySpellCooldown(SPELL_SHAMAN_MANA_TIDE_TOTEM, -cooldown);
            }
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_sha_tidal_wave_consumed::HandleBeforeCast);
    }
};

class spell_sha_storm_proficiency : public AuraScript
{
    PrepareAuraScript(spell_sha_storm_proficiency);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (GetCaster()->GetShapeshiftForm() == FORM_SPIRIT_OF_STORM)
            if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
            {
                if (eventInfo.GetActionTarget() != GetCaster())
                    GetCaster()->CastSpell(eventInfo.GetActionTarget(), TALENT_SHAMAN_STORM_PROFICIENCY_PROC, TRIGGERED_FULL_MASK);
            }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sha_storm_proficiency::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sha_ascendancy_morphs : public AuraScript
{
    PrepareAuraScript(spell_sha_ascendancy_morphs);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        uint32 spellId = m_scriptSpellId;
        uint32 morphId = 0;

        if (spellId == SPELL_SHAMAN_ASCENDANCE_FIRE)
            morphId = 50016;
        else if (spellId == SPELL_SHAMAN_ASCENDENCE_AIR)
            morphId = 50015;
        else if (spellId == SPELL_SHAMAN_ASCENDENCE_WATER)
            morphId = 50017;
        else
            morphId = 50023;

        caster->SetDisplayId(morphId);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        GetCaster()->DeMorph();

        if (ShapeshiftForm form = caster->GetShapeshiftForm())
            if (form == FORM_SPIRIT_OF_FIRE)
                caster->SetDisplayId(32756);
            else if (form == FORM_SPIRIT_OF_STORM)
                caster->SetDisplayId(32757);
            else if (form == FORM_SPIRIT_OF_EARTH)
                caster->SetDisplayId(32758);
            else if (form == FORM_SPIRIT_OF_WATER)
                caster->SetDisplayId(32755);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_ascendancy_morphs::HandleApply, EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sha_ascendancy_morphs::HandleRemove, EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sha_liquid_magma_eruption : public SpellScript
{
    PrepareSpellScript(spell_sha_liquid_magma_eruption);

    void FindTargets(std::list<WorldObject*>& targets)
    {
        if (Unit* caster = GetCaster()->GetCharmerOrOwnerOrSelf())
        {
            if (targets.size() > 0)
            {
                uint8 count = 0;
                for (auto const& target : targets)
                    if (Unit* unit = target->ToUnit())
                        if (unit->IsAlive())
                        {
                            count += 1;

                            if (count < 3)
                                caster->AddAura(SPELL_SHAMAN_FLAME_SHOCK, unit);
                        }
            }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_liquid_magma_eruption::FindTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// 49231 - Earth Shock
class spell_sha_earth_shock : public SpellScript
{
    PrepareSpellScript(spell_sha_earth_shock);

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // Remove T1 2pc Buff
        if (caster->HasAura(T1_SHAMAN_ELEM_2PC_BUFF))
            caster->RemoveAura(T1_SHAMAN_ELEM_2PC_BUFF);
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_sha_earth_shock::HandleAfterHit);
    }
};

// 84014 - Earthquake
class spell_sha_earthquake : public AuraScript
{
    PrepareAuraScript(spell_sha_earthquake);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        // Remove T1 2pc Buff + add Earthquake only Buff
        if (caster->HasAura(T1_SHAMAN_ELEM_2PC_BUFF))
        {
            caster->RemoveAura(T1_SHAMAN_ELEM_2PC_BUFF);
            caster->AddAura(T1_SHAMAN_ELEM_2PC_EARTHQUAKEBUFF, caster);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_sha_earthquake::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};



void AddSC_shaman_spell_scripts()
{
    RegisterSpellScript(spell_sha_totem_of_wrath);
    RegisterSpellScript(spell_sha_spirit_walk);
    RegisterSpellScript(spell_sha_t10_restoration_4p_bonus);
    RegisterSpellScript(spell_sha_totemic_mastery);
    RegisterSpellScript(spell_sha_feral_spirit);
    RegisterSpellScript(spell_sha_feral_spirit_scaling);
    RegisterSpellScript(spell_sha_fire_elemental_scaling);
    RegisterSpellScript(spell_sha_ancestral_awakening_proc);
    //RegisterSpellScript(spell_sha_astral_shift);
    RegisterSpellScript(spell_sha_bloodlust);
    RegisterSpellScript(spell_sha_chain_heal);
    RegisterSpellScript(spell_sha_cleansing_totem_pulse);
    RegisterSpellScript(spell_sha_earth_shield);
    RegisterSpellScript(spell_sha_earth_shield_heal);
    RegisterSpellScript(spell_sha_earthbind_totem);
    RegisterSpellScript(spell_sha_earthen_power);
    RegisterSpellScript(spell_sha_earthliving_weapon);
    RegisterSpellScript(spell_sha_earthliving_weapon_hot);
    RegisterSpellScript(spell_sha_fire_nova);
    RegisterSpellScript(spell_sha_flame_shock);
    //RegisterSpellScript(spell_sha_healing_stream_totem);
    RegisterSpellScript(spell_sha_healing_stream_totem_target);
    RegisterSpellScript(spell_sha_heroism);
    RegisterSpellScript(spell_sha_item_lightning_shield);
    RegisterSpellScript(spell_sha_item_lightning_shield_trigger);
    RegisterSpellScript(spell_sha_item_mana_surge);
    RegisterSpellScript(spell_sha_item_t10_elemental_2p_bonus);
    RegisterSpellScript(spell_sha_lava_lash);
    RegisterSpellScript(spell_sha_mana_spring_totem);
    RegisterSpellScript(spell_sha_mana_tide_totem);
    //RegisterSpellScript(spell_sha_mana_tide_totem_energize);
    RegisterSpellScript(spell_sha_sentry_totem);
    RegisterSpellScript(spell_sha_thunderstorm);
    RegisterSpellScript(spell_sha_flurry_proc);
    RegisterSpellScript(spell_sha_frostbrand_weapon);
    RegisterSpellScript(spell_sha_maelstrom_on_cast);
    RegisterSpellScript(spell_sha_maelstrom_on_hit);
    RegisterSpellScript(spell_sha_earth_elemental_scaling);
    RegisterSpellScript(spell_sha_ancestral_vision);
    RegisterSpellScript(spell_sha_ancestral_guidance);
    RegisterSpellScript(spell_sha_ancestral_guidance_healing);
    RegisterSpellScript(spell_sha_ascendance_air_aura);
    RegisterSpellScript(spell_sha_ascendance_flame);
    RegisterSpellScript(spell_sha_ascendance_flame_aura);
    RegisterSpellScript(spell_sha_elemental_blast);
    RegisterSpellScript(spell_sha_stormbringer);
    RegisterSpellScript(spell_sha_crash_lightning);
    RegisterSpellScript(spell_sha_crash_lightning_proc);
    RegisterSpellScript(spell_sha_ascendance_water);
    RegisterSpellScript(spell_sha_ascendance_water_proc);
    RegisterSpellScript(spell_sha_downpour);
    RegisterSpellScript(spell_sha_improved_astral_shift);
    RegisterSpellScript(spell_sha_frozen_power_aura);
    RegisterSpellScript(spell_sha_stormkeeper);
    RegisterSpellScript(spell_sha_stormkeeper_remove);
    RegisterSpellScript(spell_sha_gust_of_wind);
    RegisterSpellScript(spell_sha_lava_burst);
    RegisterSpellScript(spell_sha_spirit_link);
    RegisterSpellScript(spell_shaman_totem_ancestral_protection);
    RegisterSpellScript(spell_sha_summon_cloudburst_totem);
    RegisterSpellScript(spell_sha_spirit_of_storm);
    RegisterSpellScript(spell_sha_spirit_of_storm_proc);
    RegisterSpellScript(spell_sha_spirit_of_water);
    RegisterSpellScript(spell_sha_spirit_master_teacher);
    RegisterSpellScript(spell_sha_invoke_essence);
    RegisterSpellScript(spell_sha_invoke_essence_fire);
    RegisterSpellScript(spell_sha_invoke_essence_earth);
    RegisterSpellScript(spell_sha_invoke_essence_water);
    RegisterSpellScript(spell_sha_fury_of_the_elements);
    RegisterSpellScript(spell_sha_fury_of_the_elements_fire);
    RegisterSpellScript(spell_sha_fury_of_the_elements_storm);
    RegisterSpellScript(spell_sha_fury_of_the_elements_earth);
    RegisterSpellScript(spell_sha_ascendance_earth);
    RegisterSpellScript(spell_sha_focus_thine_foe);
    RegisterSpellScript(spell_sha_focus_thine_foe_elemental);
    RegisterSpellScript(spell_sha_fiery_stamp);
    RegisterSpellScript(spell_sha_tectonic_shift);
    RegisterSpellScript(spell_sha_watery_grave);
    RegisterSpellScript(spell_sha_overcharge);
    RegisterSpellScript(spell_sha_spirit_of_fire);
    RegisterSpellScript(spell_sha_unshakable_earth);
    RegisterSpellScript(spell_sha_seamless_water);
    RegisterSpellScript(spell_sha_water_bending);
    RegisterSpellScript(spell_sha_fire_proficiency);
    RegisterSpellScript(spell_sha_earth_proficiency);
    RegisterSpellScript(spell_sha_regenerative_aura);
    RegisterSpellScript(spell_sha_accumulation);
    RegisterSpellScript(spell_sha_seeping_life_force);
    RegisterSpellScript(spell_sha_stormbrand_totem);
    RegisterSpellScript(spell_sha_spirit_weapons);
    RegisterSpellScript(spell_sha_healing_surge);
    RegisterSpellScript(spell_sha_ghost_wolf);
    RegisterSpellScript(spell_sha_capacitor_totem_stun);
    RegisterSpellScript(spell_sha_capacitor_totem_aura);
    RegisterSpellScript(spell_sha_maelstrom_weapon_buff);
    RegisterSpellScript(spell_sha_riptide);
    RegisterSpellScript(spell_sha_healing_rain);
    RegisterSpellScript(spell_sha_healing_rain_periodic);
    RegisterSpellScript(spell_sha_healing_tide_totem_heal);
    RegisterSpellScript(spell_sha_tidal_wave_consumed);
    RegisterSpellScript(spell_sha_storm_proficiency);
    RegisterSpellScript(spell_sha_ascendancy_morphs);
    RegisterSpellScript(spell_sha_chain_lightning);
    RegisterSpellScript(spell_sha_liquid_magma_eruption);
    RegisterSpellScript(spell_sha_earth_shock);
    RegisterSpellScript(spell_sha_earthquake);





}
