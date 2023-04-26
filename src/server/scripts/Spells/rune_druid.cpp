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
    SPELL_BARKSKIN = 22812,
    SPELL_BERSERK = 50334,
    SPELL_BERSERK_BEAR = 00000,
    SPELL_DASH = 33357,
    SPELL_FRENZIED_REGENERATION = 22842,
    SPELL_INNERVATE = 29166,
    SPELL_IRONFUR = 000,
    SPELL_REGROWTH = 48443,
    SPELL_REJUVENATION = 48441,
    SPELL_RIP = 49800,
    SPELL_STARFALL = 53201,
    SPELL_SUNFIRE = 80518,
    SPELL_TIGERS_FURY = 50213,
    SPELL_WILD_GROWTH = 53251,

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





    
}
