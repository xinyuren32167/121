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
        if (GetCaster()->HasSpellCooldown(GetAura()->GetId()))
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 procSpell = 0;
        float apPct = 0;
        float spPct = 0;

        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            procSpell = 400046;
            apPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_1).DamageMultiplier;
            spPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_1).BonusMultiplier;
        }
        else if (eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0)
        {
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
        Unit* target = GetTarget();

        if (!target)
            return;

        GetCaster()->CastSpell(target, 48827, TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_fortified_empyrean_legacy::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_fires_of_justice : public AuraScript
{
    PrepareAuraScript(rune_pal_fires_of_justice);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->SetPower(POWER_ENERGY, GetCaster()->GetPower(POWER_ENERGY) + 1);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_fires_of_justice::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_sanctification : public AuraScript
{
    PrepareAuraScript(rune_pal_sanctification);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->SetPower(POWER_ENERGY, GetCaster()->GetPower(POWER_ENERGY) + 1);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_sanctification::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_golden_path : public AuraScript
{
    PrepareAuraScript(rune_pal_golden_path);

    void HandleProc(AuraEffect const* aurEff)
    {
        uint32 procSpell = 400236;


        float ap = int32(CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount()));
        float sp = int32(CalculatePct(GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY), aurEff->GetAmount()));
        int32 amount = std::max<int32>(0, int32(ap + sp));

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, GetTarget(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_pal_golden_path::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_pal_crusade : public AuraScript
{
    PrepareAuraScript(rune_pal_crusade);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(31884);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 holyPowerCost = eventInfo.GetSpellInfo()->ManaCost;
        Aura* crusadeAura = GetCaster()->GetAura(400244);

        if (holyPowerCost <= 0 || holyPowerCost > 5 || !crusadeAura)
            return;

        int32 maxStacks = aurEff->GetAmount();
        int32 currentStacks = crusadeAura->GetStackAmount();

        if (currentStacks == maxStacks)
            return;

        currentStacks += holyPowerCost;

        if (currentStacks > maxStacks)
            currentStacks = maxStacks;

        if (crusadeAura)
            crusadeAura->SetStackAmount(currentStacks);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_crusade::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_crusade::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_crusade_avenger_check : public AuraScript
{
    PrepareAuraScript(rune_pal_crusade_avenger_check);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(400238))
            return GetCaster()->GetAura(400238);

        if (GetCaster()->HasAura(400239))
            return GetCaster()->GetAura(400239);

        if (GetCaster()->HasAura(400240))
            return GetCaster()->GetAura(400240);

        if (GetCaster()->HasAura(400241))
            return GetCaster()->GetAura(400241);

        if (GetCaster()->HasAura(400242))
            return GetCaster()->GetAura(400242);

        if (GetCaster()->HasAura(400243))
            return GetCaster()->GetAura(400243);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->AddAura(400244, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(400244))
            GetCaster()->RemoveAura(400244);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_pal_crusade_avenger_check::HandleProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_pal_crusade_avenger_check::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pal_paragon_of_light : public AuraScript
{
    PrepareAuraScript(rune_pal_paragon_of_light);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Aura* auraEff = GetCaster()->GetAura(31884))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + 2000, auraEff->GetMaxDuration() + 5000));

            auraEff->SetDuration(duration);
        }
        else
        {
            GetCaster()->AddAura(31884, GetCaster());
            GetCaster()->GetAura(31884)->SetDuration(aurEff->GetAmount());
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_paragon_of_light::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_verdict : public AuraScript
{
    PrepareAuraScript(rune_pal_verdict);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(31884, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_verdict::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_lights_adrenaline : public AuraScript
{
    PrepareAuraScript(rune_pal_lights_adrenaline);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(400264))
            return GetCaster()->GetAura(400264);

        if (GetCaster()->HasAura(400265))
            return GetCaster()->GetAura(400265);

        if (GetCaster()->HasAura(400266))
            return GetCaster()->GetAura(400266);

        if (GetCaster()->HasAura(400267))
            return GetCaster()->GetAura(400267);

        if (GetCaster()->HasAura(400268))
            return GetCaster()->GetAura(400268);

        if (GetCaster()->HasAura(400269))
            return GetCaster()->GetAura(400269);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->AddAura(GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(400270))
            GetCaster()->RemoveAura(400270);

        if (GetCaster()->HasAura(400271))
            GetCaster()->RemoveAura(400271);

        if (GetCaster()->HasAura(400272))
            GetCaster()->RemoveAura(400272);

        if (GetCaster()->HasAura(400273))
            GetCaster()->RemoveAura(400273);

        if (GetCaster()->HasAura(400274))
            GetCaster()->RemoveAura(400274);

        if (GetCaster()->HasAura(400275))
            GetCaster()->RemoveAura(400275);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_pal_lights_adrenaline::HandleProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_pal_lights_adrenaline::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pal_titan_of_light : public AuraScript
{
    PrepareAuraScript(rune_pal_titan_of_light);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 procSpell = 400288;
        float heal = eventInfo.GetHealInfo()->GetHeal();
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target)
            return;

        int32 amount = (int32(CalculatePct(heal, aurEff->GetAmount()))) / 8;

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_titan_of_light::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_titan_of_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_soothing_aura : public AuraScript
{
    PrepareAuraScript(rune_pal_soothing_aura);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetHealInfo() || eventInfo.GetHealInfo()->GetHeal() <= 0)
            return false;

        if (!GetCaster()->HasAura(31821))
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 procSpell = 400314;
        float heal = eventInfo.GetHealInfo()->GetHeal();

        int32 amount = int32(CalculatePct(heal, aurEff->GetAmount()));

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_soothing_aura::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_soothing_aura::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_crusaders_reprieve : public AuraScript
{
    PrepareAuraScript(rune_pal_crusaders_reprieve);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 procSpell = 400322;
        float damage = eventInfo.GetDamageInfo()->GetDamage();

        int32 amount = int32(CalculatePct(damage, aurEff->GetAmount()));

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_crusaders_reprieve::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_crusaders_reprieve::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_avenging_light : public AuraScript
{
    PrepareAuraScript(rune_pal_avenging_light);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 procSpell = 400330;
        float heal = eventInfo.GetHealInfo()->GetHeal();
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target)
            return;

        int32 amount = int32(CalculatePct(heal, aurEff->GetAmount()));

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_avenging_light::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_avenging_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_resplendent_light : public AuraScript
{
    PrepareAuraScript(rune_pal_resplendent_light);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 procSpell = 400338;
        float heal = eventInfo.GetHealInfo()->GetHeal();
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target)
            return;

        int32 amount = int32(CalculatePct(heal, aurEff->GetAmount()));

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_resplendent_light::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_resplendent_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_holy_reflection : public AuraScript
{
    PrepareAuraScript(rune_pal_holy_reflection);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        LOG_ERROR("error", "reflection proc");
        if (!eventInfo.GetDamageInfo() || eventInfo.GetDamageInfo()->GetUnmitigatedDamage() <= 0)
            return false;
        LOG_ERROR("error", "reflection damage = {}", eventInfo.GetDamageInfo()->GetUnmitigatedDamage());
        if (!GetCaster()->HasAura(642))
            return false;
        LOG_ERROR("error", "reflection has damage + divine");
        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 procSpell = 400400;
        float damage = eventInfo.GetDamageInfo()->GetUnmitigatedDamage();
        LOG_ERROR("error", "damage = {}", damage);
        int32 amount = int32(CalculatePct(damage, aurEff->GetAmount()));
        LOG_ERROR("error", "amount = {}", amount);
        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_holy_reflection::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_holy_reflection::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_divine_perfection : public AuraScript
{
    PrepareAuraScript(rune_pal_divine_perfection);

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* victim = GetTarget();
        int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
        int32 cooldown = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        int32 healSpell = 400408;
        int32 healPct = aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount();

        if (remainingHealth <= 0 && !victim->ToPlayer()->HasSpellCooldown(healSpell))
        {
            absorbAmount = dmgInfo.GetDamage();

            int32 healAmount = int32(victim->CountPctFromMaxHealth(healPct));
            victim->CastCustomSpell(healSpell, SPELLVALUE_BASE_POINT0, healAmount, victim, true, nullptr, aurEff);
            victim->ToPlayer()->AddSpellCooldown(healSpell, 0, cooldown);
            victim->AddAura(642, victim);
            victim->GetAura(642)->SetDuration(4000);
        }
        else
            absorbAmount = 0;
    }

    void Register() override
    {
        OnEffectAbsorb += AuraEffectAbsorbFn(rune_pal_divine_perfection::Absorb, EFFECT_0);
    }
};

class rune_pal_punishment : public AuraScript
{
    PrepareAuraScript(rune_pal_punishment);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        LOG_ERROR("error", "punishment  proc");
        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastSpell(GetTarget(), 80046, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_punishment::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_punishment::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
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
    RegisterSpellScript(rune_pal_fires_of_justice);
    RegisterSpellScript(rune_pal_sanctification);
    RegisterSpellScript(rune_pal_golden_path);
    RegisterSpellScript(rune_pal_crusade);
    RegisterSpellScript(rune_pal_crusade_avenger_check);
    RegisterSpellScript(rune_pal_paragon_of_light);
    RegisterSpellScript(rune_pal_verdict);
    RegisterSpellScript(rune_pal_lights_adrenaline);
    RegisterSpellScript(rune_pal_titan_of_light);
    RegisterSpellScript(rune_pal_soothing_aura);
    RegisterSpellScript(rune_pal_crusaders_reprieve);
    RegisterSpellScript(rune_pal_avenging_light);
    RegisterSpellScript(rune_pal_resplendent_light);
    RegisterSpellScript(rune_pal_holy_reflection);
    RegisterSpellScript(rune_pal_divine_perfection);
    RegisterSpellScript(rune_pal_punishment);
}
