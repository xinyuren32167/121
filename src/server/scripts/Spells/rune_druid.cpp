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
    SPELL_RIP = 49800,
    SPELL_STARFALL = 53201,
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




    
}
