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

enum PriestSpells
{
    // Spells
    SPELL_PRIEST_FADE = 586,
    SPELL_PRIEST_FLASH_HEAL = 48071,

    // Talents
    TALENT_PRIEST_ = 00000,

    // Runes
    RUNE_PRIEST_ARMOR_OF_FAITH_SHIELD = 900148,
    RUNE_PRIEST_BINDING_HEALS_HEAL = 900192,
};

class rune_pri_faded : public AuraScript
{
    PrepareAuraScript(rune_pri_faded);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 debuffSpell = aurEff->GetAmount();

        if (caster->HasAura(debuffSpell))
            return;

        caster->AddAura(SPELL_PRIEST_FADE, caster);
        caster->AddAura(debuffSpell, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_faded::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_faded::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_armor_of_faith : public AuraScript
{
    PrepareAuraScript(rune_pri_armor_of_faith);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetHealInfo() && eventInfo.GetHitMask();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;

        if (spellID == SPELL_PRIEST_FLASH_HEAL && eventInfo.GetHitMask() != PROC_EX_CRITICAL_HIT)
            return;

        int32 heal = eventInfo.GetHealInfo()->GetHeal();

        if (heal <= 0)
            return;

        int32 amount = CalculatePct(heal, aurEff->GetAmount());

        if (Aura* shield = target->GetAura(RUNE_PRIEST_ARMOR_OF_FAITH_SHIELD))
            amount += shield->GetEffect(EFFECT_0)->GetAmount();

        caster->CastCustomSpell(RUNE_PRIEST_ARMOR_OF_FAITH_SHIELD, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_armor_of_faith::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_armor_of_faith::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_protective_light : public AuraScript
{
    PrepareAuraScript(rune_pri_protective_light);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetHealInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        if (caster != target)
            return;

        int32 procSpell = aurEff->GetAmount();

        caster->AddAura(procSpell, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_protective_light::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_protective_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_unwavering_will : public AuraScript
{
    PrepareAuraScript(rune_pri_unwavering_will);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 healthThreshold = aurEff->GetAmount();
        int32 buffSpell = GetEffect(EFFECT_1)->GetAmount();

        if (caster->GetHealthPct() >= healthThreshold)
        {
            if (!caster->HasAura(buffSpell))
                caster->AddAura(buffSpell, caster);
        }
        else
            if (caster->HasAura(buffSpell))
                caster->RemoveAura(buffSpell);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_pri_unwavering_will::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_pri_binding_heals : public AuraScript
{
    PrepareAuraScript(rune_pri_binding_heals);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        if (caster == target)
            return;

        int32 heal = eventInfo.GetHealInfo()->GetHeal();
        int32 amount = CalculatePct(heal, aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_PRIEST_BINDING_HEALS_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_binding_heals::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_binding_heals::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};



void AddSC_priest_perks_scripts()
{
    RegisterSpellScript(rune_pri_faded);
    RegisterSpellScript(rune_pri_armor_of_faith);
    RegisterSpellScript(rune_pri_protective_light);
    RegisterSpellScript(rune_pri_unwavering_will);
    RegisterSpellScript(rune_pri_binding_heals);



    
}

