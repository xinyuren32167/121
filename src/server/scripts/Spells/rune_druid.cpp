#include "PetDefines.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"
#include "Log.h"

enum DruidSpells
{
    //Forms
    FORM_AQUATIC_FORM = 1066,
    FORM_BEAR_FORM = 5487,
    FORM_DIRE_BEAR_FORM = 9634,
    FORM_CAT_FORM = 768,
    FORM_FLIGHT_FORM = 33943,
    FORM_SWIFT_FLIGHT_FORM = 40120,
    FORM_TRAVEL_FORM = 783,
    FORM_SWIFT_TRAVEL_FORM = 000,
    FORM_MOONKIN_FORM = 24858,

    //Incarnations
    SPELL_INCARNATION_AVATAR_OF_ASHAMANE = 00000,
    SPELL_INCARNATION_GUARDIAN_OF_URSOC = 00000,
    SPELL_INCARNATION_TREE_OF_LIFE = 00000,

    //Spells
    SPELL_ASTRAL_COMMUNION = 80534,
    SPELL_BARKSKIN = 22812,
    SPELL_BERSERK = 50334,
    SPELL_BERSERK_BEAR = 00000,
    SPELL_CELESTIAL_ALIGNMENT = 80531,
    SPELL_DASH = 33357,
    SPELL_FRENZIED_REGENERATION = 22842,
    SPELL_FULL_MOON = 80542,
    SPELL_INNERVATE = 29166,
    SPELL_IRONFUR = 000,
    SPELL_MOONFIRE = 48463,
    SPELL_MOONFALL = 80539,
    SPELL_REGROWTH = 48443,
    SPELL_REJUVENATION = 48441,
    SPELL_RIP = 49800,
    SPELL_STARFALL = 53201,
    SPELL_STARFIRE = 48465,
    SPELL_STARFIRE_AOE = 80506,
    SPELL_STARSURGE = 80527,
    SPELL_STELLAR_FLARE = 80528,
    SPELL_SUNFIRE = 80518,
    SPELL_TIGERS_FURY = 50213,
    SPELL_WARRIOR_OF_ELUNE = 80536,
    SPELL_WILD_GROWTH = 53251,
    SPELL_WRATH = 48461,

    //Talents
    SPELL_ECLIPSE_SOLAR = 80502,
    SPELL_ECLIPSE_LUNAR = 80505,
    SPELL_CELESTIAL_ALIGNMENT_ECLIPSE_SOLAR = 80532,
    SPELL_CELESTIAL_ALIGNMENT_ECLIPSE_LUNAR = 80533,

    //Runes
    RUNE_DRUID_LYCARAS_FLEETING_GLIMPSE_BARKSKIN = 700017,
    RUNE_DRUID_LYCARAS_FLEETING_GLIMPSE_TIGERS_FURY = 700018,
    RUNE_DRUID_LYCARAS_FLEETING_GLIMPSE_WILD_GROWTH = 700019,

    RUNE_DRUID_PROTECTIVE_SKIN_SHIELD = 700068,

    RUNE_DRUID_SKYSEC_HOLD_HEAL = 700088,

    RUNE_DRUID_RAMPANT_FEROCITY_DAMAGE = 700181,

    RUNE_DRUID_GUARDIANS_WRATH_DOT = 700218,

    RUNE_DRUID_PROTECTOR_OF_THE_GROVE_BUFF = 700274,

    RUNE_DRUID_PROTECTOR_OF_THE_PACK_BUFF = 700282,

    RUNE_DRUID_DREAM_OF_CENARIUS_DEBUFF = 700380,

    RUNE_DRUID_AUTUMN_LEAVES_HEAL = 700414,

    RUNE_DRUID_CULTIVATION_HEAL = 700422,

    RUNE_DRUID_GERMINATION_LISTENER = 700437,

    RUNE_DRUID_NURTURING_DORMANCY_LISTENER = 700436,

    RUNE_DRUID_SKIN_SHREDDER_DOT = 700468,

    RUNE_DRUID_IMPROVED_SUNFIRE_AOE = 700482,

    RUNE_DRUID_TSUNAMI_DEBUFF = 700514,

    RUNE_DRUID_NATURAL_SMOLDER_DOT = 700546,

    RUNE_DRUID_MERCILESS_CLAWS_DAMAGE = 700578,

    RUNE_DRUID_BURNING_ATTACKS_DAMAGE = 700596,

    RUNE_DRUID_BALANCE_OF_ALL_THINGS_ARCANE_BUFF = 700630,
    RUNE_DRUID_BALANCE_OF_ALL_THINGS_NATURE_BUFF = 700631,
    RUNE_DRUID_BALANCE_OF_ALL_THINGS_REMOVE_BUFF = 700632,

    RUNE_DRUID_LUNAR_SHRAPNEL_DAMAGE = 700652,

    RUNE_DRUID_AETHERIAL_KINDLING_LISTENER_MOONFIRE = 700666,
    RUNE_DRUID_AETHERIAL_KINDLING_LISTENER_SUNFIRE = 700667,

    RUNE_DRUID_STARLORD_BUFF = 700674,

    RUNE_DRUID_ARCANIC_SMOLDER_DOT = 700706,

    RUNE_DRUID_ASTRAL_SMOLDER_DOT = 700758,

    RUNE_DRUID_POWER_OF_GOLDRINN_DAMAGE = 700770,

    RUNE_DRUID_STARWEAVER_STARFALL_BUFF = 700790,
    RUNE_DRUID_STARWEAVER_STARSURGE_BUFF = 700791,

    RUNE_DRUID_ORBIT_BREAKER_LISTENER = 700810,

    RUNE_DRUID_SOLSTICE_BUFF = 700818,

    RUNE_DRUID_KNOWLEDGE_AGREEMENT_BUFF = 700832,
    RUNE_DRUID_KNOWLEDGE_AGREEMENT_AFTERBUFF = 700833,

    RUNE_DRUID_BALANCE_BUFF = 700866,

    RUNE_DRUID_FUNGAL_GROWTH_DOT = 700874,

    RUNE_DRUID_DRUID_OF_THE_FLAME_DOT = 700930,
};

class rune_druid_lycara_fleeting_glimpse : public AuraScript
{
    PrepareAuraScript(rune_druid_lycara_fleeting_glimpse);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->IsInCombat();
    }

    void HandleProc(AuraEffect const* aurEff)
    {
        Player* caster = GetCaster()->ToPlayer();

        if (!caster || caster->isDead())
            return;

        if (GetCaster()->HasAura(FORM_AQUATIC_FORM) || GetCaster()->HasAura(FORM_FLIGHT_FORM) || GetCaster()->HasAura(FORM_SWIFT_FLIGHT_FORM) || GetCaster()->HasAura(FORM_TRAVEL_FORM) || GetCaster()->HasAura(FORM_SWIFT_TRAVEL_FORM))
            return;

        if (GetCaster()->HasAura(FORM_BEAR_FORM) || GetCaster()->HasAura(FORM_DIRE_BEAR_FORM) || GetCaster()->HasAura(SPELL_INCARNATION_GUARDIAN_OF_URSOC))
        {
            caster->CastSpell(GetCaster(), RUNE_DRUID_LYCARAS_FLEETING_GLIMPSE_BARKSKIN, TRIGGERED_FULL_MASK);
            return;
        }

        if (GetCaster()->HasAura(FORM_CAT_FORM) || GetCaster()->HasAura(SPELL_INCARNATION_AVATAR_OF_ASHAMANE))
        {
            caster->CastSpell(GetCaster(), RUNE_DRUID_LYCARAS_FLEETING_GLIMPSE_TIGERS_FURY, TRIGGERED_FULL_MASK);
            return;
        }

        if (GetCaster()->HasAura(FORM_MOONKIN_FORM))
        {
            caster->CastSpell(GetCaster(), SPELL_STARFALL, TRIGGERED_FULL_MASK);
            return;
        }

        caster->CastSpell(GetCaster(), RUNE_DRUID_LYCARAS_FLEETING_GLIMPSE_WILD_GROWTH, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_lycara_fleeting_glimpse::CheckProc);
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_druid_lycara_fleeting_glimpse::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_druid_lycara_teachings : public AuraScript
{
    PrepareAuraScript(rune_druid_lycara_teachings);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 buffAura = GetAura()->GetEffect(EFFECT_0)->GetAmount();

        if (GetCaster()->HasAura(FORM_CAT_FORM) || GetCaster()->HasAura(SPELL_INCARNATION_AVATAR_OF_ASHAMANE))
            buffAura = GetAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        if (GetCaster()->HasAura(FORM_BEAR_FORM) || GetCaster()->HasAura(FORM_DIRE_BEAR_FORM) || GetCaster()->HasAura(SPELL_INCARNATION_GUARDIAN_OF_URSOC))
            buffAura = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;

        if (GetCaster()->HasAura(FORM_MOONKIN_FORM))
            buffAura = GetAura()->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
        GetCaster()->ToPlayer()->UpdateMastery();
        GetCaster()->ToPlayer()->UpdateVersatility();
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        for (size_t i = 700032; i < 700056; i++)
        {
            if (GetCaster()->HasAura(i))
                GetCaster()->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_lycara_teachings::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_lycara_teachings::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_lycara_teachings_cat : public AuraScript
{
    PrepareAuraScript(rune_druid_lycara_teachings_cat);

    Aura* GetRuneAura()
    {
        for (size_t i = 700026; i < 700032; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        for (size_t i = 700032; i < 700038; i++)
        {
            if (GetCaster()->HasAura(i))
                GetCaster()->RemoveAura(i);
        }

        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());

    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        for (size_t i = 700038; i < 700044; i++)
        {
            if (GetCaster()->HasAura(i))
                GetCaster()->RemoveAura(i);
        }

        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_lycara_teachings_cat::HandleProc, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_lycara_teachings_cat::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_lycara_teachings_bear : public AuraScript
{
    PrepareAuraScript(rune_druid_lycara_teachings_bear);

    Aura* GetRuneAura()
    {
        for (size_t i = 700026; i < 700032; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        for (size_t i = 700032; i < 700038; i++)
        {
            if (GetCaster()->HasAura(i))
                GetCaster()->RemoveAura(i);
        }

        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
        GetCaster()->ToPlayer()->UpdateVersatility();
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        for (size_t i = 700044; i < 700050; i++)
        {
            if (GetCaster()->HasAura(i))
                GetCaster()->RemoveAura(i);
        }

        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->AddAura(buffAura, GetCaster());
        GetCaster()->ToPlayer()->UpdateVersatility();
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_lycara_teachings_bear::HandleProc, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_lycara_teachings_bear::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_lycara_teachings_moonkin : public AuraScript
{
    PrepareAuraScript(rune_druid_lycara_teachings_moonkin);

    Aura* GetRuneAura()
    {
        for (size_t i = 700026; i < 700032; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        for (size_t i = 700032; i < 700038; i++)
        {
            if (GetCaster()->HasAura(i))
                GetCaster()->RemoveAura(i);
        }

        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
        GetCaster()->ToPlayer()->UpdateMastery();

    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        for (size_t i = 700050; i < 700056; i++)
        {
            if (GetCaster()->HasAura(i))
                GetCaster()->RemoveAura(i);
        }

        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->AddAura(buffAura, GetCaster());
        GetCaster()->ToPlayer()->UpdateMastery();
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_lycara_teachings_moonkin::HandleProc, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_lycara_teachings_moonkin::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_natural_wisdom : public AuraScript
{
    PrepareAuraScript(rune_druid_natural_wisdom);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700056; i < 700062; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!GetRuneAura(caster) || caster->isDead())
            return;

        if (GetUnitOwner() == caster)
            return;

        caster->AddAura(SPELL_INNERVATE, caster);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_natural_wisdom::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_protective_skin : public AuraScript
{
    PrepareAuraScript(rune_druid_protective_skin);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700062; i < 700068; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!GetRuneAura(caster) || caster->isDead())
            return;

        int32 healthPct = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();
        int32 amount = CalculatePct(caster->GetMaxHealth(), healthPct);

        GetCaster()->CastCustomSpell(RUNE_DRUID_PROTECTIVE_SKIN_SHIELD, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_protective_skin::HandleRemove, EFFECT_0, SPELL_AURA_REDUCE_PUSHBACK, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_skysec_hold : public AuraScript
{
    PrepareAuraScript(rune_druid_skysec_hold);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 maxHealth = caster->GetMaxHealth();
        float heal = int32(CalculatePct(maxHealth, aurEff->GetAmount()));
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_DRUID_SKYSEC_HOLD_HEAL)->GetMaxTicks();
        int32 amount = heal / maxTicks;

        GetCaster()->CastCustomSpell(RUNE_DRUID_SKYSEC_HOLD_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_skysec_hold::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_verdant_heart : public AuraScript
{
    PrepareAuraScript(rune_druid_verdant_heart);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700090; i < 700096; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!GetRuneAura(caster) || caster->isDead())
            return;

        int32 procSpell = GetRuneAura(caster)->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        caster->AddAura(procSpell, caster);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(SPELL_BARKSKIN) || GetCaster()->HasAura(SPELL_FRENZIED_REGENERATION))
            return;

        for (size_t i = 700096; i < 700102; i++)
        {
            if (GetCaster()->HasAura(i))
                GetCaster()->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_verdant_heart::HandleProc, EFFECT_0, SPELL_AURA_REDUCE_PUSHBACK, AURA_EFFECT_HANDLE_REAL);
        OnEffectApply += AuraEffectApplyFn(rune_druid_verdant_heart::HandleProc, EFFECT_2, SPELL_AURA_MOD_HEALING_PCT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_verdant_heart::HandleRemove, EFFECT_0, SPELL_AURA_REDUCE_PUSHBACK, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_verdant_heart::HandleRemove, EFFECT_2, SPELL_AURA_MOD_HEALING_PCT, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_natural_order : public AuraScript
{
    PrepareAuraScript(rune_druid_natural_order);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700102; i < 700108; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!GetRuneAura(caster) || caster->isDead())
            return;

        caster->AddAura(SPELL_FRENZIED_REGENERATION, caster);
        caster->AddAura(SPELL_IRONFUR, caster);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!GetRuneAura(caster) || caster->isDead())
            return;

        caster->AddAura(SPELL_FRENZIED_REGENERATION, caster);
        caster->AddAura(SPELL_IRONFUR, caster);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_natural_order::HandleProc, EFFECT_0, SPELL_AURA_REDUCE_PUSHBACK, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_natural_order::HandleRemove, EFFECT_0, SPELL_AURA_REDUCE_PUSHBACK, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_well_honed_instincts : public AuraScript
{
    PrepareAuraScript(rune_druid_well_honed_instincts);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        int32 debuffAura = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;

        if (!caster || caster->isDead() || caster->HasAura(debuffAura))
            return;

        int32 healthPct = caster->GetHealthPct();

        if (healthPct > aurEff->GetAmount())
            return;

        caster->AddAura(SPELL_FRENZIED_REGENERATION, caster);
        caster->AddAura(debuffAura, caster);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_well_honed_instincts::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_relentless_hunter : public AuraScript
{
    PrepareAuraScript(rune_druid_relentless_hunter);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();

        if (!caster || caster->isDead())
            return;

        caster->RemoveSpellCooldown(SPELL_DASH, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_relentless_hunter::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_iron_jaws : public AuraScript
{
    PrepareAuraScript(rune_druid_iron_jaws);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();

        if (!caster || caster->isDead())
            return;

        int32 procChance = GetCaster()->GetComboPoints() * aurEff->GetAmount();
        uint32 random = urand(1, 100);

        if (random > procChance)
            return;

        int32 procSpell = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_iron_jaws::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_rampant_ferocity : public SpellScript
{
    PrepareSpellScript(rune_druid_rampant_ferocity);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700174; i < 700180; i++)
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

        if (!GetRuneAura(caster))
            return;

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead() || !target->HasAura(SPELL_RIP))
                continue;

            Aura* ripAura = target->GetAura(SPELL_RIP);
            AuraEffect* ripEff = target->GetAuraEffect(SPELL_RIP, 0);

            int32 remainingTicks = ripAura->GetDuration() / ripEff->GetAmplitude();
            int32 remainingDamage = ripEff->GetAmount() * remainingTicks;
            int32 damagePct = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();
            int32 amount = int32(CalculatePct(remainingDamage, damagePct));

            GetCaster()->CastCustomSpell(RUNE_DRUID_RAMPANT_FEROCITY_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_druid_rampant_ferocity::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class rune_druid_guardians_wrath : public AuraScript
{
    PrepareAuraScript(rune_druid_guardians_wrath);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_DRUID_GUARDIANS_WRATH_DOT)->GetMaxTicks();
        int32 amount = damage / maxTicks;

        GetCaster()->CastCustomSpell(RUNE_DRUID_GUARDIANS_WRATH_DOT, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_guardians_wrath::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_guardians_wrath::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_tooth_and_claw : public SpellScript
{
    PrepareSpellScript(rune_druid_tooth_and_claw);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700220; i < 700226; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        if (!GetRuneAura(caster))
            return;

        if (!target || target->isDead())
            return;

        for (size_t i = 700226; i < 700232; i++)
        {
            if (!caster->HasAura(i))
                continue;

            int32 procSpell = caster->GetAura(i)->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;

            caster->AddAura(procSpell, target);
            return;
        }
    }


    void Register() override
    {
        OnHit += SpellHitFn(rune_druid_tooth_and_claw::HandleProc);
    }
};

class rune_druid_tooth_and_claw_raze : public SpellScript
{
    PrepareSpellScript(rune_druid_tooth_and_claw_raze);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700220; i < 700226; i++)
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

        if (!GetRuneAura(caster))
            return;

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead() || !target->HasAura(SPELL_RIP))
                continue;

            for (size_t i = 700226; i < 700232; i++)
            {
                if (caster->HasAura(i))
                {
                    int32 procSpell = caster->GetAura(i)->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;

                    caster->AddAura(procSpell, target);
                }
            }
        }
    }


    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_druid_tooth_and_claw_raze::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class rune_druid_protector_of_the_grove : public AuraScript
{
    PrepareAuraScript(rune_druid_protector_of_the_grove);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!eventInfo.GetHealInfo())
            return;

        float healDone = eventInfo.GetHealInfo()->GetHeal();

        if (healDone <= 0)
            return;

        float healStacking = CalculatePct(healDone, aurEff->GetAmount());

        if (healStacking < 1)
            return;

        if (!caster->HasAura(RUNE_DRUID_PROTECTOR_OF_THE_GROVE_BUFF))
            caster->AddAura(RUNE_DRUID_PROTECTOR_OF_THE_GROVE_BUFF, caster);

        AuraEffect* buffAura = caster->GetAura(RUNE_DRUID_PROTECTOR_OF_THE_GROVE_BUFF)->GetEffect(EFFECT_0);

        int32 maxAmount = CalculatePct(GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ARCANE), GetAura()->GetEffect(EFFECT_1)->GetAmount());
        int32 amount = buffAura->GetAmount();
        amount = std::min<int32>(maxAmount, amount + healStacking);

        buffAura->ChangeAmount(amount);

    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_protector_of_the_grove::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_protector_of_the_pack : public AuraScript
{
    PrepareAuraScript(rune_druid_protector_of_the_pack);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        float damageDone = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDone <= 0)
            return;

        float damageStacking = CalculatePct(damageDone, aurEff->GetAmount());

        if (!caster->HasAura(RUNE_DRUID_PROTECTOR_OF_THE_PACK_BUFF))
            caster->AddAura(RUNE_DRUID_PROTECTOR_OF_THE_PACK_BUFF, caster);

        AuraEffect* buffAura = caster->GetAura(RUNE_DRUID_PROTECTOR_OF_THE_PACK_BUFF)->GetEffect(EFFECT_0);

        int32 maxAmount = CalculatePct(GetCaster()->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_ALL), GetAura()->GetEffect(EFFECT_1)->GetAmount());
        int32 amount = buffAura->GetAmount();
        amount = std::min<int32>(maxAmount, amount + damageStacking);

        buffAura->ChangeAmount(amount);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_protector_of_the_pack::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_protector_of_the_pack::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_blood_mist : public AuraScript
{
    PrepareAuraScript(rune_druid_blood_mist);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        Player* player = GetCaster()->ToPlayer();
        int32 increasedDuration = aurEff->GetAmplitude();
        int32 baseDuration = aurEff->GetAmount();
        int32 berserkSpell = SPELL_BERSERK;

        if (player->HasSpell(SPELL_INCARNATION_AVATAR_OF_ASHAMANE))
            berserkSpell = SPELL_INCARNATION_AVATAR_OF_ASHAMANE;

        if (Aura* auraEff = player->GetAura(berserkSpell))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + increasedDuration, auraEff->GetMaxDuration() + 10000));

            auraEff->SetDuration(duration);
        }
        else
        {
            player->AddAura(berserkSpell, player);
            player->GetAura(berserkSpell)->SetDuration(baseDuration);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_blood_mist::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_abundance : public AuraScript
{
    PrepareAuraScript(rune_druid_abundance);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700350; i < 700356; i++)
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

        if (!GetRuneAura(caster))
            return;

        int32 procSpell = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();

        for (size_t i = 700356; i < 700362; i++)
        {
            if (i == procSpell)
                continue;

            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }

        if (Aura* buffAura = caster->GetAura(procSpell))
        {
            buffAura->ModStackAmount(1);
            return;
        }

        caster->AddAura(procSpell, caster);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetAura()->GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!GetRuneAura(caster))
        {
            for (size_t i = 700356; i < 700362; i++)
            {
                if (caster->HasAura(i))
                    caster->RemoveAura(i);
            }
            return;
        }

        int32 procSpell = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();

        for (size_t i = 700356; i < 700362; i++)
        {
            if (i == procSpell)
                continue;

            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }

        if (Aura* buffAura = caster->GetAura(procSpell))
            buffAura->ModStackAmount(-1);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_abundance::HandleProc, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_abundance::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_protective_growth : public AuraScript
{
    PrepareAuraScript(rune_druid_protective_growth);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700362; i < 700368; i++)
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

        if (GetAura()->GetOwner() != caster)
            return;

        if (!GetRuneAura(caster))
            return;

        int32 procSpell = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();

        for (size_t i = 700368; i < 700374; i++)
        {
            if (i == procSpell)
                continue;

            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }

        if (caster->HasAura(procSpell))
            return;

        caster->AddAura(procSpell, caster);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetAura()->GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetAura()->GetOwner() != caster)
            return;

        for (size_t i = 700368; i < 700374; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_protective_growth::HandleProc, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_protective_growth::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_dream_of_cenarius : public AuraScript
{
    PrepareAuraScript(rune_druid_dream_of_cenarius);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        int32 criticalChance = GetCaster()->GetUnitCriticalChance(BASE_ATTACK, GetCaster());
        uint32 random = urand(1, 100);

        return random <= criticalChance;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        int32 debuffAura = RUNE_DRUID_DREAM_OF_CENARIUS_DEBUFF;

        if (!caster || caster->isDead() || caster->HasAura(debuffAura))
            return;

        caster->CastSpell(caster, SPELL_REGROWTH, TRIGGERED_FULL_MASK);

        caster->AddAura(debuffAura, caster);
        caster->GetAura(debuffAura)->SetDuration(aurEff->GetAmount());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_dream_of_cenarius::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_dream_of_cenarius::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_improved_regrowth : public SpellScript
{
    PrepareSpellScript(rune_druid_improved_regrowth);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700382; i < 700388; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        Unit* target = GetExplTargetUnit();

        if (!target || !target->HasAura(SPELL_REGROWTH))
            return;

        Player* caster = GetCaster()->ToPlayer();

        if (!GetRuneAura(caster))
            return;

        int32 buff = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();

        caster->AddAura(buff, caster);
    }

    void HandleHitProc()
    {
        for (size_t i = 700388; i < 700394; i++)
        {
            if (GetCaster()->HasAura(i))
                GetCaster()->RemoveAura(i);
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(rune_druid_improved_regrowth::HandleProc);
        AfterHit += SpellHitFn(rune_druid_improved_regrowth::HandleHitProc);
    }
};

class rune_druid_luxuriant_soil : public SpellScript
{
    PrepareSpellScript(rune_druid_luxuriant_soil);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700400; i < 700406; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetGerminationAura(Unit* caster)
    {
        for (size_t i = 700400; i < 700406; i++)
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

        if (!GetRuneAura(caster))
            return;

        targets.sort(Acore::HealthPctOrderPred());

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (GetGerminationAura(caster))
            {
                if (target->isDead())
                    continue;

                if (Aura* rejuvAura = target->GetAura(SPELL_REJUVENATION))
                {
                    if (rejuvAura->GetStackAmount() > 1)
                        continue;
                }
            }
            else
            {
                if (target->isDead() || target->HasAura(SPELL_REJUVENATION))
                    continue;
            }


            caster->CastSpell(target, SPELL_REJUVENATION, TRIGGERED_FULL_MASK);
            return;
        }

        caster->CastSpell(caster, SPELL_REJUVENATION, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_druid_luxuriant_soil::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
    }
};

class rune_druid_autumn_leaves : public AuraScript
{
    PrepareAuraScript(rune_druid_autumn_leaves);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetHealInfo()->GetTarget();
        Player* caster = GetCaster()->ToPlayer();
        int32 healAmount = eventInfo.GetHealInfo()->GetHeal();
        int32 healPct = aurEff->GetAmount();

        if (!caster || caster->isDead())
            return;

        int32 HotQuantity = 0;
        auto targetAuras = target->GetAppliedAuras();

        for (auto itr = targetAuras.begin(); itr != targetAuras.end(); ++itr)
        {
            if (Aura* aura = itr->second->GetBase())
            {
                if (aura->GetCaster() != caster)
                    continue;

                SpellInfo const* auraInfo = aura->GetSpellInfo();

                if (auraInfo->SpellFamilyFlags[1] & 0x00400000 && auraInfo->SpellFamilyName == SPELLFAMILY_DRUID && auraInfo->Id != SPELL_REJUVENATION)
                    HotQuantity++;
            }
        }

        if (HotQuantity > 0)
            return;

        int32 amount = CalculatePct(healAmount, healPct);
        GetCaster()->CastCustomSpell(RUNE_DRUID_AUTUMN_LEAVES_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_autumn_leaves::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_autumn_leaves::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_cultivation : public AuraScript
{
    PrepareAuraScript(rune_druid_cultivation);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetHealInfo()->GetTarget();
        Player* caster = GetCaster()->ToPlayer();
        int32 HealthPct = target->GetHealthPct();
        int32 healthThreshold = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (!caster || caster->isDead())
            return;

        if (HealthPct > healthThreshold)
            return;

        int32 spellPower = caster->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_ALL);
        int32 amount = CalculatePct(spellPower, aurEff->GetAmount());

        if (AuraEffect* protEff = target->GetAuraEffect(RUNE_DRUID_CULTIVATION_HEAL, 0))
        {
            int32 totalTicks = sSpellMgr->AssertSpellInfo(RUNE_DRUID_CULTIVATION_HEAL)->GetMaxTicks();
            int32 remainingTicks = totalTicks - protEff->GetTickNumber();
            int32 remainingAmount = protEff->GetAmount() * remainingTicks;
            int32 remainingAmountPerTick = remainingAmount / totalTicks;

            amount += remainingAmountPerTick;
            target->RemoveAura(RUNE_DRUID_CULTIVATION_HEAL);
        }

        GetCaster()->CastCustomSpell(RUNE_DRUID_CULTIVATION_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_cultivation::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_cultivation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_germination : public SpellScript
{
    PrepareSpellScript(rune_druid_germination);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700424; i < 700430; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        Unit* target = GetExplTargetUnit();

        if (!target || !target->HasAura(SPELL_REJUVENATION))
            return;

        Player* caster = GetCaster()->ToPlayer();

        if (!GetRuneAura(caster))
            return;

        caster->AddAura(RUNE_DRUID_GERMINATION_LISTENER, target);
    }

    void HandleHitProc()
    {
        Unit* target = GetExplTargetUnit();

        if (!target || !target->HasAura(SPELL_REJUVENATION))
            return;

        Player* caster = GetCaster()->ToPlayer();

        if (!caster || caster->isDead())
            return;

        if (!target->HasAura(RUNE_DRUID_GERMINATION_LISTENER))
            return;

        target->RemoveAura(SPELL_REJUVENATION);
        caster->AddAura(SPELL_REJUVENATION, target);

        Aura* rejuvAura = target->GetAura(SPELL_REJUVENATION);
        rejuvAura->SetStackAmount(2);
        // rejuvAura->RefreshDuration();
        // rejuvAura->GetEffect(EFFECT_0)->ResetTicks();
        target->RemoveAura(RUNE_DRUID_GERMINATION_LISTENER);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(rune_druid_germination::HandleProc);
        AfterHit += SpellHitFn(rune_druid_germination::HandleHitProc);
    }
};

class rune_druid_nurturing_dormancy : public AuraScript
{
    PrepareAuraScript(rune_druid_nurturing_dormancy);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetHealInfo()->GetTarget();
        Player* caster = GetCaster()->ToPlayer();
        int32 HealthPct = target->GetHealthPct();
        int32 healthThreshold = 99;

        if (!caster || caster->isDead())
            return;

        if (HealthPct <= healthThreshold)
            return;

        Aura* rejuvAura = target->GetAura(SPELL_REJUVENATION);

        if (!target->HasAura(RUNE_DRUID_NURTURING_DORMANCY_LISTENER))
            caster->AddAura(RUNE_DRUID_NURTURING_DORMANCY_LISTENER, target);

        Aura* listenerAura = target->GetAura(RUNE_DRUID_NURTURING_DORMANCY_LISTENER);
        int32 increasedAmount = listenerAura->GetEffect(EFFECT_0)->GetAmount();
        int32 maxIncrease = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (increasedAmount >= maxIncrease)
            return;

        int32 durationIncrease = aurEff->GetAmount();

        if ((increasedAmount + durationIncrease) > maxIncrease)
            durationIncrease = maxIncrease - increasedAmount;

        int32 duration = rejuvAura->GetDuration();
        rejuvAura->SetDuration(duration + durationIncrease);
        rejuvAura->GetEffect(EFFECT_0)->ResetTicks();

        listenerAura->GetEffect(EFFECT_0)->SetAmount(increasedAmount + durationIncrease);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_nurturing_dormancy::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_nurturing_dormancy::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_nurturing_dormancy_listener : public AuraScript
{
    PrepareAuraScript(rune_druid_nurturing_dormancy_listener);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700430; i < 700436; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* target = GetAura()->GetUnitOwner();
        Unit* caster = GetAura()->GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        if (target->HasAura(RUNE_DRUID_NURTURING_DORMANCY_LISTENER))
            target->RemoveAura(RUNE_DRUID_NURTURING_DORMANCY_LISTENER);

        caster->AddAura(RUNE_DRUID_NURTURING_DORMANCY_LISTENER, target);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* target = GetAura()->GetUnitOwner();

        if (!target || target->isDead())
            return;

        if (target->HasAura(RUNE_DRUID_NURTURING_DORMANCY_LISTENER))
            target->RemoveAura(RUNE_DRUID_NURTURING_DORMANCY_LISTENER);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_nurturing_dormancy_listener::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_nurturing_dormancy_listener::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_skin_shredder : public AuraScript
{
    PrepareAuraScript(rune_druid_skin_shredder);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_DRUID_SKIN_SHREDDER_DOT)->GetMaxTicks();
        int32 amount = damage / maxTicks;

        GetCaster()->CastCustomSpell(RUNE_DRUID_SKIN_SHREDDER_DOT, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_skin_shredder::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_skin_shredder::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_improved_sunfire_proc : public AuraScript
{
    PrepareAuraScript(rune_druid_improved_sunfire_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        GetCaster()->CastSpell(victim, RUNE_DRUID_IMPROVED_SUNFIRE_AOE, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_improved_sunfire_proc::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_improved_sunfire_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_improved_sunfire : public SpellScript
{
    PrepareSpellScript(rune_druid_improved_sunfire);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead())
                continue;

            caster->AddAura(SPELL_SUNFIRE, target);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_druid_improved_sunfire::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class rune_druid_tsunami : public SpellScript
{
    PrepareSpellScript(rune_druid_tsunami);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700508; i < 700514; i++)
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

        if (!GetRuneAura(caster))
            return;

        int32 debuffSpell = GetRuneAura(caster)->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead())
                continue;

            caster->AddAura(debuffSpell, target);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_druid_tsunami::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
    }
};

class rune_druid_natural_smolder : public AuraScript
{
    PrepareAuraScript(rune_druid_natural_smolder);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_DRUID_NATURAL_SMOLDER_DOT)->GetMaxTicks();
        int32 amount = damage / maxTicks;

        if (AuraEffect* protEff = victim->GetAuraEffect(RUNE_DRUID_NATURAL_SMOLDER_DOT, 0))
        {
            int32 remainingTicks = maxTicks - protEff->GetTickNumber();
            int32 remainingAmount = protEff->GetAmount() * remainingTicks;
            int32 remainingAmountPerTick = remainingAmount / maxTicks;

            amount += remainingAmountPerTick;
            victim->RemoveAura(RUNE_DRUID_NATURAL_SMOLDER_DOT);
        }

        GetCaster()->CastCustomSpell(RUNE_DRUID_NATURAL_SMOLDER_DOT, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_natural_smolder::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_natural_smolder::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_invigorating_wounds : public AuraScript
{
    PrepareAuraScript(rune_druid_invigorating_wounds);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 powerIncrease = 0;

        if (GetCaster()->HasAura(FORM_BEAR_FORM) || GetCaster()->HasAura(FORM_DIRE_BEAR_FORM) || GetCaster()->HasAura(SPELL_INCARNATION_GUARDIAN_OF_URSOC))
        {
            powerIncrease = GetAura()->GetEffect(EFFECT_1)->GetAmount();
            caster->SetPower(POWER_RAGE, caster->GetPower(POWER_RAGE) + powerIncrease);
            return;
        }

        if (GetCaster()->HasAura(FORM_CAT_FORM) || GetCaster()->HasAura(SPELL_INCARNATION_AVATAR_OF_ASHAMANE))
        {
            powerIncrease = aurEff->GetAmount();
            caster->SetPower(POWER_ENERGY, caster->GetPower(POWER_ENERGY) + powerIncrease);
            return;
        }

        if (GetCaster()->HasAura(FORM_MOONKIN_FORM))
        {
            powerIncrease = GetAura()->GetEffect(EFFECT_2)->GetAmount();
            caster->SetPower(POWER_RUNIC_POWER, caster->GetPower(POWER_RUNIC_POWER) + powerIncrease);
            return;
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_invigorating_wounds::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_trail_of_blood : public AuraScript
{
    PrepareAuraScript(rune_druid_trail_of_blood);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();
        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!caster->HasAura(FORM_CAT_FORM) && !caster->HasAura(SPELL_INCARNATION_AVATAR_OF_ASHAMANE))
            return;

        if (!target || target->isDead())
            return;

        int32 bleedQuantity = 0;
        auto targetAuras = target->GetAppliedAuras();

        for (auto itr = targetAuras.begin(); itr != targetAuras.end(); ++itr)
        {
            if (Aura* aura = itr->second->GetBase())
            {
                if (GetCaster()->GetGUID() != aura->GetCasterGUID())
                    continue;

                SpellInfo const* auraInfo = aura->GetSpellInfo();

                if (auraInfo->SpellFamilyFlags[2] & 0x01000000 && auraInfo->SpellFamilyName == SPELLFAMILY_DRUID)
                    bleedQuantity++;
            }
        }

        if (bleedQuantity <= 0)
            return;

        caster->AddAura(aurEff->GetAmount(), target);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_trail_of_blood::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_trail_of_blood::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_merciless_claws : public AuraScript
{
    PrepareAuraScript(rune_druid_merciless_claws);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 bleedQuantity = 0;
        auto targetAuras = target->GetAppliedAuras();
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

        if (bleedQuantity <= 0)
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, aurEff->GetAmount());

        GetCaster()->CastCustomSpell(RUNE_DRUID_MERCILESS_CLAWS_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_merciless_claws::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_merciless_claws::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_burning_attacks : public AuraScript
{
    PrepareAuraScript(rune_druid_burning_attacks);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 amount = 0;
        auto targetAuras = target->GetAppliedAuras();

        for (auto itr = targetAuras.begin(); itr != targetAuras.end(); ++itr)
        {
            if (Aura* aura = itr->second->GetBase())
            {
                if (GetCaster()->GetGUID() != aura->GetCasterGUID())
                    continue;

                SpellInfo const* auraInfo = aura->GetSpellInfo();

                if (auraInfo->SpellFamilyFlags[2] & 0x03000000 && auraInfo->SpellFamilyName == SPELLFAMILY_DRUID)
                {
                    AuraEffect* auraEff = aura->GetEffect(EFFECT_0);
                    int32 totalTicks = sSpellMgr->AssertSpellInfo(aura->GetId())->GetMaxTicks();
                    int32 remainingTicks = totalTicks - auraEff->GetTickNumber();
                    int32 remainingAmount = auraEff->GetAmount() * remainingTicks;

                    amount += remainingAmount;
                }
            }
        }

        if (amount <= 0)
            return;

        amount = CalculatePct(amount, aurEff->GetAmount());

        GetCaster()->CastCustomSpell(RUNE_DRUID_BURNING_ATTACKS_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_burning_attacks::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_burning_attacks::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_astral_generation : public AuraScript
{
    PrepareAuraScript(rune_druid_astral_generation);

    void HandleProc(AuraEffect const* aurEff)
    {
        Player* caster = GetCaster()->ToPlayer();

        if (!caster || caster->isDead())
            return;

        if (GetCaster()->HasAura(FORM_MOONKIN_FORM))
        {
            int32 powerIncrease = aurEff->GetAmount();
            caster->SetPower(POWER_RUNIC_POWER, caster->GetPower(POWER_RUNIC_POWER) + powerIncrease);
        }
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_druid_astral_generation::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_druid_natures_grace : public AuraScript
{
    PrepareAuraScript(rune_druid_natures_grace);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700610; i < 700616; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        int32 buffAura = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();
        int32 duration = GetRuneAura(caster)->GetEffect(EFFECT_1)->GetAmount();

        caster->AddAura(buffAura, caster);
        caster->GetAura(buffAura)->SetDuration(duration);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_natures_grace::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_balance_of_all_things_buffs : public AuraScript
{
    PrepareAuraScript(rune_druid_balance_of_all_things_buffs);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700624; i < 700630; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        int32 stackAmount = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();

        if (caster->HasAura(SPELL_ECLIPSE_SOLAR) || caster->HasAura(SPELL_CELESTIAL_ALIGNMENT_ECLIPSE_SOLAR))
        {
            caster->AddAura(RUNE_DRUID_BALANCE_OF_ALL_THINGS_NATURE_BUFF, caster);
            caster->GetAura(RUNE_DRUID_BALANCE_OF_ALL_THINGS_NATURE_BUFF)->SetStackAmount(stackAmount);
            caster->AddAura(RUNE_DRUID_BALANCE_OF_ALL_THINGS_REMOVE_BUFF, caster);
        }

        if (caster->HasAura(SPELL_ECLIPSE_LUNAR) || caster->HasAura(SPELL_CELESTIAL_ALIGNMENT_ECLIPSE_LUNAR))
        {
            caster->AddAura(RUNE_DRUID_BALANCE_OF_ALL_THINGS_ARCANE_BUFF, caster);
            caster->GetAura(RUNE_DRUID_BALANCE_OF_ALL_THINGS_ARCANE_BUFF)->SetStackAmount(stackAmount);
            caster->AddAura(RUNE_DRUID_BALANCE_OF_ALL_THINGS_REMOVE_BUFF, caster);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_balance_of_all_things_buffs::HandleProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_balance_of_all_things_listener : public AuraScript
{
    PrepareAuraScript(rune_druid_balance_of_all_things_listener);

    void HandleProc(AuraEffect const* aurEff)
    {
        Player* caster = GetCaster()->ToPlayer();

        if (!caster || caster->isDead())
        {
            GetAura()->Remove();
            return;
        }

        if (Aura* natureBuff = caster->GetAura(RUNE_DRUID_BALANCE_OF_ALL_THINGS_NATURE_BUFF))
        {
            if (natureBuff->GetStackAmount() == 1)
            {
                natureBuff->Remove();

                if (!caster->HasAura(RUNE_DRUID_BALANCE_OF_ALL_THINGS_ARCANE_BUFF))
                {
                    GetAura()->Remove();
                    return;
                }
            }
            natureBuff->ModStackAmount(-1);
        }

        if (Aura* arcaneBuff = caster->GetAura(RUNE_DRUID_BALANCE_OF_ALL_THINGS_ARCANE_BUFF))
        {
            if (arcaneBuff->GetStackAmount() == 1)
            {
                arcaneBuff->Remove();
                GetAura()->Remove();
                return;
            }
            arcaneBuff->ModStackAmount(-1);
        }
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_druid_balance_of_all_things_listener::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_druid_lunar_shrapnel : public AuraScript
{
    PrepareAuraScript(rune_druid_lunar_shrapnel);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        if (!target->HasAura(SPELL_MOONFIRE))
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, aurEff->GetAmount());

        GetCaster()->CastCustomSpell(RUNE_DRUID_LUNAR_SHRAPNEL_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_lunar_shrapnel::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_lunar_shrapnel::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_lunar_rain : public AuraScript
{
    PrepareAuraScript(rune_druid_lunar_rain);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 rand = urand(0, 1);

        if (rand == 0)
            caster->CastSpell(target, SPELL_MOONFIRE, TRIGGERED_FULL_MASK);
        else
            caster->CastSpell(target, SPELL_SUNFIRE, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_lunar_rain::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_lunar_rain::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_aetherial_kindling : public AuraScript
{
    PrepareAuraScript(rune_druid_aetherial_kindling);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        if (!target->HasAura(SPELL_MOONFIRE) && !target->HasAura(SPELL_SUNFIRE))
            return;

        int32 maxIncrease = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        int32 increase = aurEff->GetAmount();

        if (Aura* moonfire = target->GetAura(SPELL_MOONFIRE))
        {
            Aura* listener = target->GetAura(RUNE_DRUID_AETHERIAL_KINDLING_LISTENER_MOONFIRE);
            int32 increasedAmount = listener->GetEffect(EFFECT_0)->GetAmount();

            if (increasedAmount < maxIncrease)
            {
                if ((increasedAmount + increase) > maxIncrease)
                    increase = maxIncrease - increasedAmount;

                int32 duration = moonfire->GetDuration();
                moonfire->SetDuration(duration + increase);
                moonfire->GetEffect(EFFECT_0)->ResetTicks();

                listener->GetEffect(EFFECT_0)->SetAmount(increasedAmount + increase);
            }
        }

        if (Aura* sunfire = target->GetAura(SPELL_SUNFIRE))
        {
            Aura* listener = target->GetAura(RUNE_DRUID_AETHERIAL_KINDLING_LISTENER_SUNFIRE);
            int32 increasedAmount = listener->GetEffect(EFFECT_0)->GetAmount();

            if (increasedAmount >= maxIncrease)
                return;

            if ((increasedAmount + increase) > maxIncrease)
                increase = maxIncrease - increasedAmount;

            int32 duration = sunfire->GetDuration();
            sunfire->SetDuration(duration + increase);
            sunfire->GetEffect(EFFECT_0)->ResetTicks();

            listener->GetEffect(EFFECT_0)->SetAmount(increasedAmount + increase);
        }

    };

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_aetherial_kindling::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_aetherial_kindling::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_aetherial_kindling_listener : public AuraScript
{
    PrepareAuraScript(rune_druid_aetherial_kindling_listener);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700660; i < 700666; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetAura()->GetCaster();
        Unit* target = GetAura()->GetOwner()->ToUnit();

        if (!target || target->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        if (GetAura()->GetId() == SPELL_MOONFIRE)
            caster->AddAura(RUNE_DRUID_AETHERIAL_KINDLING_LISTENER_MOONFIRE, target);

        if (GetAura()->GetId() == SPELL_SUNFIRE)
            caster->AddAura(RUNE_DRUID_AETHERIAL_KINDLING_LISTENER_SUNFIRE, target);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* target = GetAura()->GetOwner()->ToUnit();

        if (!target || target->isDead())
            return;

        if (GetAura()->GetId() == SPELL_MOONFIRE && target->HasAura(RUNE_DRUID_AETHERIAL_KINDLING_LISTENER_MOONFIRE))
            target->RemoveAura(RUNE_DRUID_AETHERIAL_KINDLING_LISTENER_MOONFIRE);

        if (GetAura()->GetId() == SPELL_SUNFIRE && target->HasAura(RUNE_DRUID_AETHERIAL_KINDLING_LISTENER_SUNFIRE))
            target->RemoveAura(RUNE_DRUID_AETHERIAL_KINDLING_LISTENER_SUNFIRE);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_aetherial_kindling_listener::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_aetherial_kindling_listener::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_starlord : public AuraScript
{
    PrepareAuraScript(rune_druid_starlord);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 duration = aurEff->GetAmount();
        int32 maxStack = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (!caster->HasAura(RUNE_DRUID_STARLORD_BUFF))
        {
            caster->AddAura(RUNE_DRUID_STARLORD_BUFF, caster);
            caster->GetAura(RUNE_DRUID_STARLORD_BUFF)->SetDuration(duration);
            return;
        }

        Aura* casterAura = caster->GetAura(RUNE_DRUID_STARLORD_BUFF);

        if (casterAura->GetStackAmount() == maxStack)
            return;

        duration = casterAura->GetDuration();
        casterAura->ModStackAmount(1);
        casterAura->SetDuration(duration);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_starlord::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_balance_of_power : public AuraScript
{
    PrepareAuraScript(rune_druid_balance_of_power);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        if (!target->HasAura(SPELL_MOONFIRE) && !target->HasAura(SPELL_SUNFIRE) && !target->HasAura(SPELL_STELLAR_FLARE))
            return;

        int32 increase = 0;
        int32 spellId = eventInfo.GetSpellInfo()->Id;

        if (spellId == SPELL_STARFIRE_AOE)
            increase = aurEff->GetAmount();

        if (spellId == SPELL_WRATH)
            increase = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (Aura* moonfire = target->GetAura(SPELL_MOONFIRE))
        {
            int32 duration = moonfire->GetDuration();
            moonfire->SetDuration(duration + increase);
            moonfire->GetEffect(EFFECT_0)->ResetTicks();
        }

        if (Aura* sunfire = target->GetAura(SPELL_SUNFIRE))
        {
            int32 duration = sunfire->GetDuration();
            sunfire->SetDuration(duration + increase);
            sunfire->GetEffect(EFFECT_0)->ResetTicks();
        }

        if (Aura* stellarFlare = target->GetAura(SPELL_STELLAR_FLARE))
        {
            int32 duration = stellarFlare->GetDuration();
            stellarFlare->SetDuration(duration + increase);
            stellarFlare->GetEffect(EFFECT_0)->ResetTicks();
        }
    };

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_balance_of_power::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_balance_of_power::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_nightsong_regalia_remove : public SpellScript
{
    PrepareSpellScript(rune_druid_nightsong_regalia_remove);

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 700694; i < 700700; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(rune_druid_nightsong_regalia_remove::HandleAfterCast);
    }
};

class rune_druid_arcanic_smolder : public AuraScript
{
    PrepareAuraScript(rune_druid_arcanic_smolder);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_DRUID_ARCANIC_SMOLDER_DOT)->GetMaxTicks();
        int32 amount = damage / maxTicks;

        if (AuraEffect* protEff = victim->GetAuraEffect(RUNE_DRUID_ARCANIC_SMOLDER_DOT, 0))
        {
            int32 remainingTicks = maxTicks - protEff->GetTickNumber();
            int32 remainingAmount = protEff->GetAmount() * remainingTicks;
            int32 remainingAmountPerTick = remainingAmount / maxTicks;

            amount += remainingAmountPerTick;
            victim->RemoveAura(RUNE_DRUID_ARCANIC_SMOLDER_DOT);
        }

        GetCaster()->CastCustomSpell(RUNE_DRUID_ARCANIC_SMOLDER_DOT, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_arcanic_smolder::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_arcanic_smolder::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_downing_sun_and_rising_moon : public AuraScript
{
    PrepareAuraScript(rune_druid_downing_sun_and_rising_moon);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();

        if (!caster || caster->isDead())
            return;

        int32 spellId = eventInfo.GetSpellInfo()->Id;
        int32 buffAura = 0;

        if (spellId == SPELL_WRATH)
            buffAura = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (spellId == SPELL_STARFIRE)
            buffAura = aurEff->GetAmount();

        if (buffAura == 0)
            return;

        caster->CastSpell(caster, buffAura, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_downing_sun_and_rising_moon::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_astral_gift : public AuraScript
{
    PrepareAuraScript(rune_druid_astral_gift);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetCaster()->ToPlayer())
        {
            target->ModifySpellCooldown(SPELL_ASTRAL_COMMUNION, -aurEff->GetAmount());
            target->ModifySpellCooldown(SPELL_CELESTIAL_ALIGNMENT, -aurEff->GetAmount());
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_astral_gift::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_astral_smolder : public AuraScript
{
    PrepareAuraScript(rune_druid_astral_smolder);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_DRUID_ASTRAL_SMOLDER_DOT)->GetMaxTicks();
        int32 amount = damage / maxTicks;

        if (AuraEffect* protEff = victim->GetAuraEffect(RUNE_DRUID_ASTRAL_SMOLDER_DOT, 0))
        {
            int32 remainingTicks = maxTicks - protEff->GetTickNumber();
            int32 remainingAmount = protEff->GetAmount() * remainingTicks;
            int32 remainingAmountPerTick = remainingAmount / maxTicks;

            amount += remainingAmountPerTick;
            victim->RemoveAura(RUNE_DRUID_ASTRAL_SMOLDER_DOT);
        }

        GetCaster()->CastCustomSpell(RUNE_DRUID_ASTRAL_SMOLDER_DOT, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_astral_smolder::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_astral_smolder::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_power_of_goldrinn : public AuraScript
{
    PrepareAuraScript(rune_druid_power_of_goldrinn);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetDamageInfo()->GetVictim();
        Player* caster = GetCaster()->ToPlayer();

        if (!target || target->isDead())
            return;

        int32 spellPower = std::max<int32>(caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ARCANE), caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_NATURE));
        int32 amount = CalculatePct(spellPower, aurEff->GetAmount());

        GetCaster()->CastCustomSpell(RUNE_DRUID_POWER_OF_GOLDRINN_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_power_of_goldrinn::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_starweaver : public AuraScript
{
    PrepareAuraScript(rune_druid_starweaver);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();

        if (!caster || caster->isDead())
            return;

        int32 spellId = eventInfo.GetSpellInfo()->Id;
        int32 buffAura = 0;
        int32 procChance = 0;

        if (spellId == SPELL_STARSURGE)
        {
            buffAura = RUNE_DRUID_STARWEAVER_STARFALL_BUFF;
            procChance = aurEff->GetAmount();
        }

        if (spellId == SPELL_STARFALL)
        {
            buffAura = RUNE_DRUID_STARWEAVER_STARSURGE_BUFF;
            procChance = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        }

        if (buffAura == 0)
            return;

        int32 random = urand(1, 100);

        if (random > procChance)
            return;

        caster->CastSpell(caster, buffAura, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_starweaver::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_orbit_breaker : public AuraScript
{
    PrepareAuraScript(rune_druid_orbit_breaker);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
        Unit* caster = eventInfo.GetDamageInfo()->GetAttacker();

        if (!victim || victim->isDead())
            return;

        if (!caster->HasAura(RUNE_DRUID_ORBIT_BREAKER_LISTENER))
        {
            caster->AddAura(RUNE_DRUID_ORBIT_BREAKER_LISTENER, caster);
            return;
        }

        Aura* listener = caster->GetAura(RUNE_DRUID_ORBIT_BREAKER_LISTENER);

        listener->ModStackAmount(1);
        int32 maxStack = aurEff->GetAmount();
        int32 currentStack = listener->GetStackAmount();

        if (currentStack < maxStack)
            return;

        listener->Remove();
        caster->CastSpell(victim, SPELL_FULL_MOON, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_orbit_breaker::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_orbit_breaker::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_solstice : public AuraScript
{
    PrepareAuraScript(rune_druid_solstice);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700812; i < 700818; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        int32 duration = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();
        caster->AddAura(RUNE_DRUID_SOLSTICE_BUFF, caster);
        caster->GetAura(RUNE_DRUID_SOLSTICE_BUFF)->SetDuration(duration);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_druid_solstice::HandleProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_knowledge_agreement : public AuraScript
{
    PrepareAuraScript(rune_druid_knowledge_agreement);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();

        if (!caster || caster->isDead())
            return;

        if (!GetCaster()->HasAura(FORM_MOONKIN_FORM) || !caster->HasAura(SPELL_CELESTIAL_ALIGNMENT))
            return;

        int32 spellAstralPower = eventInfo.GetSpellInfo()->ManaCost / 10;

        if (spellAstralPower <= 0)
            return;

        int32 astralPowerAccumulated = GetAura()->GetEffect(EFFECT_2)->GetAmount() + spellAstralPower;
        int32 astralPowerThreshold = aurEff->GetAmount();

        while (astralPowerAccumulated > astralPowerThreshold)
        {
            if (!caster->HasAura(RUNE_DRUID_KNOWLEDGE_AGREEMENT_BUFF))
                caster->AddAura(RUNE_DRUID_KNOWLEDGE_AGREEMENT_BUFF, caster);
            else
                caster->GetAura(RUNE_DRUID_KNOWLEDGE_AGREEMENT_BUFF)->ModStackAmount(1);

            astralPowerAccumulated -= astralPowerThreshold;
        }

        GetAura()->GetEffect(EFFECT_2)->SetAmount(astralPowerAccumulated);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_knowledge_agreement::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_knowledge_agreement::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_knowledge_agreement_remove : public AuraScript
{
    PrepareAuraScript(rune_druid_knowledge_agreement_remove);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 700826; i < 700832; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetAura()->GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(RUNE_DRUID_KNOWLEDGE_AGREEMENT_BUFF))
            return;

        if (!GetRuneAura(caster))
        {
            caster->RemoveAura(RUNE_DRUID_KNOWLEDGE_AGREEMENT_BUFF);
            return;
        }

        int32 duration = GetRuneAura(caster)->GetEffect(EFFECT_1)->GetAmount();
        int32 stackAmount = caster->GetAura(RUNE_DRUID_KNOWLEDGE_AGREEMENT_BUFF)->GetStackAmount();

        caster->AddAura(RUNE_DRUID_KNOWLEDGE_AGREEMENT_AFTERBUFF, caster);

        Aura* afterBuff = caster->GetAura(RUNE_DRUID_KNOWLEDGE_AGREEMENT_AFTERBUFF);
        afterBuff->SetStackAmount(stackAmount);
        afterBuff->SetDuration(duration);

        caster->RemoveAura(RUNE_DRUID_KNOWLEDGE_AGREEMENT_BUFF);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_druid_knowledge_agreement_remove::HandleRemove, EFFECT_0, SPELL_AURA_MOD_MELEE_RANGED_HASTE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_druid_primordial_arcanic_pulsar : public AuraScript
{
    PrepareAuraScript(rune_druid_primordial_arcanic_pulsar);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();

        if (!caster || caster->isDead())
            return;

        if (!GetCaster()->HasAura(FORM_MOONKIN_FORM))
            return;

        int32 spellAstralPower = eventInfo.GetSpellInfo()->ManaCost / 10;

        if (spellAstralPower <= 0)
            return;

        int32 astralPowerAccumulated = GetAura()->GetEffect(EFFECT_2)->GetAmount() + spellAstralPower;
        int32 celestialDuration = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        int32 astralPowerThreshold = aurEff->GetAmount();

        if (astralPowerAccumulated < astralPowerThreshold)
        {
            GetAura()->GetEffect(EFFECT_2)->SetAmount(astralPowerAccumulated);
            return;
        }

        if (Aura* celestial = caster->GetAura(SPELL_CELESTIAL_ALIGNMENT))
        {
            int32 duration = celestial->GetDuration();
            celestialDuration = celestialDuration / 2;
            celestial->SetDuration(duration + celestialDuration);
        }
        else
        {
            caster->AddAura(SPELL_CELESTIAL_ALIGNMENT, caster);
            caster->GetAura(SPELL_CELESTIAL_ALIGNMENT)->SetDuration(celestialDuration);
        }
        astralPowerAccumulated -= astralPowerThreshold;

        GetAura()->GetEffect(EFFECT_2)->SetAmount(astralPowerAccumulated);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_primordial_arcanic_pulsar::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_primordial_arcanic_pulsar::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_power_of_elune : public AuraScript
{
    PrepareAuraScript(rune_druid_power_of_elune);

    void HandleProc(AuraEffect const* aurEff)
    {
        Player* caster = GetCaster()->ToPlayer();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_CELESTIAL_ALIGNMENT))
            return;

        int32 powerIncrease = aurEff->GetAmount();
        caster->SetPower(POWER_RUNIC_POWER, caster->GetPower(POWER_RUNIC_POWER) + powerIncrease);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_druid_power_of_elune::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_druid_balance : public AuraScript
{
    PrepareAuraScript(rune_druid_balance);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();

        if (!caster || caster->isDead())
            return;

        if (!GetCaster()->HasAura(FORM_MOONKIN_FORM))
            return;

        int32 powerIncrease = aurEff->GetAmount();
        int32 maxTicks = sSpellMgr->GetSpellInfo(SPELL_ASTRAL_COMMUNION)->GetMaxTicks();
        int32 amount = powerIncrease / maxTicks;

        GetCaster()->CastCustomSpell(RUNE_DRUID_BALANCE_BUFF, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_balance::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_fungal_growth : public AuraScript
{
    PrepareAuraScript(rune_druid_fungal_growth);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_DRUID_FUNGAL_GROWTH_DOT)->GetMaxTicks();
        int32 amount = damage / maxTicks;

        if (AuraEffect* protEff = victim->GetAuraEffect(RUNE_DRUID_FUNGAL_GROWTH_DOT, 0))
        {
            int32 remainingTicks = maxTicks - protEff->GetTickNumber();
            int32 remainingAmount = protEff->GetAmount() * remainingTicks;
            int32 remainingAmountPerTick = remainingAmount / maxTicks;

            amount += remainingAmountPerTick;
            victim->RemoveAura(RUNE_DRUID_FUNGAL_GROWTH_DOT);
        }

        GetCaster()->CastCustomSpell(RUNE_DRUID_FUNGAL_GROWTH_DOT, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_fungal_growth::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_fungal_growth::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_call_of_elune : public AuraScript
{
    PrepareAuraScript(rune_druid_call_of_elune);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetCaster()->ToPlayer())
        {
            target->ModifySpellCooldown(SPELL_WARRIOR_OF_ELUNE, -aurEff->GetAmount());
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_call_of_elune::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_moon_friend : public AuraScript
{
    PrepareAuraScript(rune_druid_moon_friend);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetCaster()->ToPlayer())
        {
            target->ModifySpellCooldown(SPELL_MOONFALL, -aurEff->GetAmount());
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_druid_moon_friend::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_druid_druid_of_the_flame : public AuraScript
{
    PrepareAuraScript(rune_druid_druid_of_the_flame);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        if (!caster->HasAura(FORM_CAT_FORM) && !caster->HasAura(SPELL_INCARNATION_AVATAR_OF_ASHAMANE))
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_DRUID_DRUID_OF_THE_FLAME_DOT)->GetMaxTicks();
        int32 amount = damage / maxTicks;

        if (AuraEffect* protEff = victim->GetAuraEffect(RUNE_DRUID_DRUID_OF_THE_FLAME_DOT, 0))
        {
            int32 remainingTicks = maxTicks - protEff->GetTickNumber();
            int32 remainingAmount = protEff->GetAmount() * remainingTicks;
            int32 remainingAmountPerTick = remainingAmount / maxTicks;

            amount += remainingAmountPerTick;
            victim->RemoveAura(RUNE_DRUID_DRUID_OF_THE_FLAME_DOT);
        }

        caster->CastCustomSpell(RUNE_DRUID_DRUID_OF_THE_FLAME_DOT, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_druid_druid_of_the_flame::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_druid_druid_of_the_flame::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};



void AddSC_druid_rune_scripts()
{
    RegisterSpellScript(rune_druid_lycara_fleeting_glimpse);
    RegisterSpellScript(rune_druid_lycara_teachings);
    RegisterSpellScript(rune_druid_lycara_teachings_cat);
    RegisterSpellScript(rune_druid_lycara_teachings_bear);
    RegisterSpellScript(rune_druid_lycara_teachings_moonkin);
    RegisterSpellScript(rune_druid_natural_wisdom);
    RegisterSpellScript(rune_druid_protective_skin);
    RegisterSpellScript(rune_druid_skysec_hold);
    RegisterSpellScript(rune_druid_verdant_heart);
    RegisterSpellScript(rune_druid_natural_order);
    RegisterSpellScript(rune_druid_well_honed_instincts);
    RegisterSpellScript(rune_druid_relentless_hunter);
    RegisterSpellScript(rune_druid_iron_jaws);
    RegisterSpellScript(rune_druid_rampant_ferocity);
    RegisterSpellScript(rune_druid_guardians_wrath);
    RegisterSpellScript(rune_druid_tooth_and_claw);
    RegisterSpellScript(rune_druid_protector_of_the_grove);
    RegisterSpellScript(rune_druid_protector_of_the_pack);
    RegisterSpellScript(rune_druid_blood_mist);
    RegisterSpellScript(rune_druid_abundance);
    RegisterSpellScript(rune_druid_protective_growth);
    RegisterSpellScript(rune_druid_dream_of_cenarius);
    RegisterSpellScript(rune_druid_improved_regrowth);
    RegisterSpellScript(rune_druid_luxuriant_soil);
    RegisterSpellScript(rune_druid_autumn_leaves);
    RegisterSpellScript(rune_druid_cultivation);
    RegisterSpellScript(rune_druid_germination);
    RegisterSpellScript(rune_druid_nurturing_dormancy);
    RegisterSpellScript(rune_druid_nurturing_dormancy_listener);
    RegisterSpellScript(rune_druid_skin_shredder);
    RegisterSpellScript(rune_druid_improved_sunfire_proc);
    RegisterSpellScript(rune_druid_improved_sunfire);
    RegisterSpellScript(rune_druid_tsunami);
    RegisterSpellScript(rune_druid_natural_smolder);
    RegisterSpellScript(rune_druid_invigorating_wounds);
    RegisterSpellScript(rune_druid_trail_of_blood);
    RegisterSpellScript(rune_druid_merciless_claws);
    RegisterSpellScript(rune_druid_burning_attacks);
    RegisterSpellScript(rune_druid_astral_generation);
    RegisterSpellScript(rune_druid_natures_grace);
    RegisterSpellScript(rune_druid_balance_of_all_things_buffs);
    RegisterSpellScript(rune_druid_balance_of_all_things_listener);
    RegisterSpellScript(rune_druid_lunar_shrapnel);
    RegisterSpellScript(rune_druid_lunar_rain);
    RegisterSpellScript(rune_druid_aetherial_kindling);
    RegisterSpellScript(rune_druid_aetherial_kindling_listener);
    RegisterSpellScript(rune_druid_starlord);
    RegisterSpellScript(rune_druid_balance_of_power);
    RegisterSpellScript(rune_druid_nightsong_regalia_remove);
    RegisterSpellScript(rune_druid_arcanic_smolder);
    RegisterSpellScript(rune_druid_downing_sun_and_rising_moon);
    RegisterSpellScript(rune_druid_astral_gift);
    RegisterSpellScript(rune_druid_astral_smolder);
    RegisterSpellScript(rune_druid_power_of_goldrinn);
    RegisterSpellScript(rune_druid_starweaver);
    RegisterSpellScript(rune_druid_orbit_breaker);
    RegisterSpellScript(rune_druid_solstice);
    RegisterSpellScript(rune_druid_knowledge_agreement);
    RegisterSpellScript(rune_druid_knowledge_agreement_remove);
    RegisterSpellScript(rune_druid_primordial_arcanic_pulsar);
    RegisterSpellScript(rune_druid_power_of_elune);
    RegisterSpellScript(rune_druid_balance);
    RegisterSpellScript(rune_druid_fungal_growth);
    RegisterSpellScript(rune_druid_call_of_elune);
    RegisterSpellScript(rune_druid_moon_friend);
    RegisterSpellScript(rune_druid_druid_of_the_flame);




    
}
