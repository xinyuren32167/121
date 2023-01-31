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

class rune_pal_inner_grace : public AuraScript
{
    PrepareAuraScript(rune_pal_inner_grace);

    void HandleProc(AuraEffect const* aurEff)
    {
        GetCaster()->SetPower(POWER_ENERGY, GetCaster()->GetPower(POWER_ENERGY) + 1);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_pal_inner_grace::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_pal_touch_of_light : public AuraScript
{
    PrepareAuraScript(rune_pal_touch_of_light);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo() && !eventInfo.GetHealInfo())
            return false;

        if (GetCaster()->HasSpellCooldown(GetAura()->GetId()))
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 procSpell = 0;
        float apPct = 0;
        float spPct = 0;

        if (eventInfo.GetDamageInfo())
        {
            if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
                return;

            procSpell = 400046;
            apPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_1).DamageMultiplier;
            spPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_1).BonusMultiplier;
        }
        else if (eventInfo.GetHealInfo())
        {
            if (eventInfo.GetHealInfo()->GetHeal() <= 0)
                return;

            procSpell = 400047;
            apPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_2).DamageMultiplier;
            spPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_2).BonusMultiplier;
        }
        float ap = GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK) * apPct;
        float sp = GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY) * spPct;
        int32 amount = std::max<int32>(0, int32(ap + sp));

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
        GetCaster()->AddSpellCooldown(GetAura()->GetId(), 0, 1000);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_touch_of_light::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_touch_of_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_justice_clemency : public AuraScript
{
    PrepareAuraScript(rune_pal_justice_clemency);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
            return;

        uint32 procSpell = 400054;
        float damage = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_justice_clemency::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_justice_clemency::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_fist_of_justice : public AuraScript
{
    PrepareAuraScript(rune_pal_fist_of_justice);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(48806, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_fist_of_justice::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_pal_untouchable_judgement : public AuraScript
{
    PrepareAuraScript(rune_pal_untouchable_judgement);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
            return;

        uint32 procSpell = 400088;
        float damage = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);

    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_untouchable_judgement::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_untouchable_judgement::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_zealots_paragon : public AuraScript
{
    PrepareAuraScript(rune_pal_zealots_paragon);

    bool HandleProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster()->HasAura(31884));
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Aura* auraEff = GetCaster()->GetAura(31884))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + aurEff->GetAmount(), auraEff->GetMaxDuration() + 5000));

            auraEff->SetDuration(duration);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_zealots_paragon::HandleProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_zealots_paragon::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_virtuous_command : public AuraScript
{
    PrepareAuraScript(rune_pal_virtuous_command);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
            return;

        uint32 procSpell = 400108;
        float damage = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_virtuous_command::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_virtuous_command::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_fortified_empyrean_legacy : public AuraScript
{
    PrepareAuraScript(rune_pal_fortified_empyrean_legacy);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        LOG_ERROR("error", "fortified proc");
        uint32 procSpell = 48827;
        Unit* target = GetTarget();

        if (!target)
            return;
        LOG_ERROR("error", "fortified proc check");
        GetCaster()->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);

        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(48827, true);
        LOG_ERROR("error", "finished");
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_fortified_empyrean_legacy::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

void AddSC_paladin_perks_scripts()
{
    RegisterSpellScript(rune_pal_inner_grace);
    RegisterSpellScript(rune_pal_touch_of_light);
    RegisterSpellScript(rune_pal_justice_clemency);
    RegisterSpellScript(rune_pal_fist_of_justice);
    RegisterSpellScript(rune_pal_untouchable_judgement);
    RegisterSpellScript(rune_pal_zealots_paragon);
    RegisterSpellScript(rune_pal_virtuous_command);
    RegisterSpellScript(rune_pal_fortified_empyrean_legacy);
}
