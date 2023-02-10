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

enum PaladinSpells
{
    SPELL_PALADIN_AURA_MASTERY = 31821,
    SPELL_PALADIN_AVENGERS_SHIELD = 48827,
    SPELL_PALADIN_AVENGING_WRATH = 31884,

    SPELL_PALADIN_BEACON_OF_LIGHT = 53563,
    SPELL_PALADIN_BEACON_OF_VIRTUE = 80048,
    SPELL_PALADIN_BEACON_OF_FAITH = 80049,

    SPELL_PALADIN_DIVINE_ILLUMINATION = 31842,
    SPELL_PALADIN_HAMMER_OF_WRATH = 48806,

    SPELL_PALADIN_HOLY_SHOCK = 48825,
    SPELL_PALADIN_HOLY_SHOCK_DAMAGE = 25912,
    SPELL_PALADIN_HOLY_SHOCK_HEAL = 25914,

    SPELL_PALADIN_LAY_ON_HANDS = 48788,
    SPELL_PALADIN_SERAPHIM = 80039,
    SPELL_PALADIN_TEMPLARS_VERDICT = 80046,
    SPELL_PALADIN_WORD_OF_GLORY = 80062,



    RUNE_PALADIN_TOUCH_OF_LIGHT_DAMAGE = 400046,
    RUNE_PALADIN_TOUCH_OF_LIGHT_HEAL = 400047,

    RUNE_PALADIN_JUSTICES_CLEMENCY_HEAL = 400054,

    RUNE_PALADIN_UNTOUCHABLE_JUDGEMENT_SHIELD = 400088,

    RUNE_PALADIN_VIRTUOUS_COMMAND_DAMAGE = 400108,

    RUNE_PALADIN_GOLDEN_PATH_HEAL = 400236,

    RUNE_PALADIN_CRUSADE_HASTE = 400244,

    RUNE_PALADIN_TITAN_OF_LIGHT_HEAL = 400288,

    RUNE_PALADIN_SOOTHING_AURA_HEAL = 400314,

    RUNE_PALADIN_CRUSADERS_REPRIEVE_HEAL = 400322,

    RUNE_PALADIN_AVENGING_LIGHT_DAMAGE = 400330,

    RUNE_PALADIN_RESPLENDENT_LIGHT_HEAL = 400338,

    RUNE_PALADIN_HOLY_REFLECTION_DAMAGE = 400400,

    RUNE_PALADIN_DIVINE_PERFECTION_HEAL = 400408,

    RUNE_PALADIN_LIGHT_OF_THE_TITANS_HEAL = 400452,

    RUNE_PALADIN_ANSWERED_PRAYER_COOLDOWN = 400479,

    RUNE_PALADIN_HAND_OF_THE_PROTECTOR_HEAL = 400478,

    RUNE_PALADIN_SHOCK_BARRIER_SHIELD = 400564,

    RUNE_PALADIN_POWER_OF_THE_SILVER_HAND_BUFF = 400600,

    RUNE_PALADIN_AVENGER_BUFF = 400614,

    RUNE_PALADIN_SAVED_BY_THE_LIGHT_SHIELD = 400726,
    RUNE_PALADIN_SAVED_BY_THE_LIGHT_DEBUFF = 400727,

    RUNE_PALADIN_SIGN_OF_FAITH_HEAL = 400726,
    RUNE_PALADIN_SIGN_OF_FAITH_DEBUFF = 400727,
};

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
            procSpell = RUNE_PALADIN_TOUCH_OF_LIGHT_DAMAGE;
            apPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_1).DamageMultiplier;
            spPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_1).BonusMultiplier;
        }
        else if (eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0)
        {
            procSpell = RUNE_PALADIN_TOUCH_OF_LIGHT_HEAL;
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

        float damage = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_PALADIN_JUSTICES_CLEMENCY_HEAL, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
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
            caster->RemoveSpellCooldown(SPELL_PALADIN_HAMMER_OF_WRATH, true);
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

        float damage = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_PALADIN_UNTOUCHABLE_JUDGEMENT_SHIELD, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);

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
        if (Aura* auraEff = GetCaster()->GetAura(SPELL_PALADIN_AVENGING_WRATH))
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

        float damage = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_PALADIN_VIRTUOUS_COMMAND_DAMAGE, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
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

        GetCaster()->CastSpell(target, SPELL_PALADIN_AVENGERS_SHIELD, TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD);
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
        float ap = int32(CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount()));
        float sp = int32(CalculatePct(GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY), aurEff->GetAmount()));
        int32 amount = std::max<int32>(0, int32(ap + sp));

        GetCaster()->CastCustomSpell(RUNE_PALADIN_GOLDEN_PATH_HEAL, SPELLVALUE_BASE_POINT0, amount, GetTarget(), TRIGGERED_FULL_MASK);
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
        return GetCaster()->HasAura(SPELL_PALADIN_AVENGING_WRATH);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 holyPowerCost = eventInfo.GetSpellInfo()->ManaCost;
        Aura* crusadeAura = GetCaster()->GetAura(RUNE_PALADIN_CRUSADE_HASTE);

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

        GetCaster()->AddAura(RUNE_PALADIN_CRUSADE_HASTE, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(RUNE_PALADIN_CRUSADE_HASTE))
            GetCaster()->RemoveAura(RUNE_PALADIN_CRUSADE_HASTE);
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
        if (Aura* auraEff = GetCaster()->GetAura(SPELL_PALADIN_AVENGING_WRATH))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + 2000, auraEff->GetMaxDuration() + 5000));

            auraEff->SetDuration(duration);
        }
        else
        {
            GetCaster()->AddAura(SPELL_PALADIN_AVENGING_WRATH, GetCaster());
            GetCaster()->GetAura(SPELL_PALADIN_AVENGING_WRATH)->SetDuration(aurEff->GetAmount());
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
            target->ModifySpellCooldown(SPELL_PALADIN_AVENGING_WRATH, -aurEff->GetAmount());
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
        float heal = eventInfo.GetHealInfo()->GetHeal();
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target)
            return;

        int32 amount = (int32(CalculatePct(heal, aurEff->GetAmount()))) / 8;

        GetCaster()->CastCustomSpell(RUNE_PALADIN_TITAN_OF_LIGHT_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
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

        if (!GetCaster()->HasAura(SPELL_PALADIN_AURA_MASTERY))
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        float heal = eventInfo.GetHealInfo()->GetHeal();
        int32 amount = int32(CalculatePct(heal, aurEff->GetAmount()));

        GetCaster()->CastCustomSpell(RUNE_PALADIN_SOOTHING_AURA_HEAL, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
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
        float damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = int32(CalculatePct(damage, aurEff->GetAmount()));

        GetCaster()->CastCustomSpell(RUNE_PALADIN_CRUSADERS_REPRIEVE_HEAL, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
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
        float heal = eventInfo.GetHealInfo()->GetHeal();
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target)
            return;

        int32 amount = int32(CalculatePct(heal, aurEff->GetAmount()));

        GetCaster()->CastCustomSpell(RUNE_PALADIN_AVENGING_LIGHT_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
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
        float heal = eventInfo.GetHealInfo()->GetHeal();
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target)
            return;

        int32 amount = int32(CalculatePct(heal, aurEff->GetAmount()));

        GetCaster()->CastCustomSpell(RUNE_PALADIN_RESPLENDENT_LIGHT_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
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
        float damage = eventInfo.GetDamageInfo()->GetUnmitigatedDamage();
        LOG_ERROR("error", "damage = {}", damage);
        int32 amount = int32(CalculatePct(damage, aurEff->GetAmount()));
        LOG_ERROR("error", "amount = {}", amount);
        GetCaster()->CastCustomSpell(RUNE_PALADIN_HOLY_REFLECTION_DAMAGE, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
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
        int32 healPct = aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount();

        if (remainingHealth <= 0 && !victim->ToPlayer()->HasSpellCooldown(RUNE_PALADIN_DIVINE_PERFECTION_HEAL))
        {
            absorbAmount = dmgInfo.GetDamage();
            int32 healAmount = int32(victim->CountPctFromMaxHealth(healPct));

            victim->CastCustomSpell(RUNE_PALADIN_DIVINE_PERFECTION_HEAL, SPELLVALUE_BASE_POINT0, healAmount, victim, true, nullptr, aurEff);
            victim->ToPlayer()->AddSpellCooldown(RUNE_PALADIN_DIVINE_PERFECTION_HEAL, 0, cooldown);

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
        GetCaster()->CastSpell(GetTarget(), SPELL_PALADIN_TEMPLARS_VERDICT, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_punishment::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_punishment::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_light_of_the_titans : public AuraScript
{
    PrepareAuraScript(rune_pal_light_of_the_titans);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target)
            return;

        float ap = int32(CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount()));
        float sp = int32(CalculatePct(GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY), aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount()));
        int32 amount = (ap + sp) / 15;

        GetCaster()->CastCustomSpell(RUNE_PALADIN_LIGHT_OF_THE_TITANS_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_light_of_the_titans::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_light_of_the_titans::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_answered_prayer : public AuraScript
{
    PrepareAuraScript(rune_pal_answered_prayer);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return !GetCaster()->ToPlayer()->HasSpellCooldown(RUNE_PALADIN_ANSWERED_PRAYER_COOLDOWN);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        int32 remainingHealth = caster->GetHealth() - eventInfo.GetDamageInfo()->GetDamage();
        int32 cooldown = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        int32 healthTreshold = aurEff->GetAmount();

        if (remainingHealth > int32(caster->CountPctFromMaxHealth(healthTreshold)))
            return;

        caster->CastSpell(caster, SPELL_PALADIN_WORD_OF_GLORY, TRIGGERED_FULL_MASK);

        caster->ToPlayer()->AddSpellCooldown(RUNE_PALADIN_ANSWERED_PRAYER_COOLDOWN, 0, cooldown);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_answered_prayer::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_answered_prayer::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_hand_of_the_protector : public AuraScript
{
    PrepareAuraScript(rune_pal_hand_of_the_protector);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target)
            return;

        float healthPct = target->GetHealthPct();

        if (healthPct == 100)
            return;

        int32 increasePct = (100 - healthPct) * aurEff->GetAmount();
        int32 amount = int32(CalculatePct(eventInfo.GetHealInfo()->GetHeal(), increasePct));

        GetCaster()->CastCustomSpell(RUNE_PALADIN_HAND_OF_THE_PROTECTOR_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_hand_of_the_protector::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_hand_of_the_protector::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_ashes_to_ashes : public AuraScript
{
    PrepareAuraScript(rune_pal_ashes_to_ashes);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(400486))
            return GetCaster()->GetAura(400486);

        if (GetCaster()->HasAura(400487))
            return GetCaster()->GetAura(400487);

        if (GetCaster()->HasAura(400488))
            return GetCaster()->GetAura(400488);

        if (GetCaster()->HasAura(400489))
            return GetCaster()->GetAura(400489);

        if (GetCaster()->HasAura(400490))
            return GetCaster()->GetAura(400490);

        if (GetCaster()->HasAura(400491))
            return GetCaster()->GetAura(400491);

        return nullptr;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetRuneAura();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Aura* auraEff = GetCaster()->GetAura(SPELL_PALADIN_SERAPHIM))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + 1000, auraEff->GetMaxDuration() + 5000));

            auraEff->SetDuration(duration);
        }
        else
        {
            GetCaster()->AddAura(SPELL_PALADIN_SERAPHIM, GetCaster());
            GetCaster()->GetAura(SPELL_PALADIN_SERAPHIM)->SetDuration(GetRuneAura()->GetEffect(EFFECT_0)->GetAmount());
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_ashes_to_ashes::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_ashes_to_ashes::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_tirions_devotion : public AuraScript
{
    PrepareAuraScript(rune_pal_tirions_devotion);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_PALADIN_LAY_ON_HANDS, -aurEff->GetAmount());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_tirions_devotion::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_tirions_devotion::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_empyreal_ward : public SpellScript
{
    PrepareSpellScript(rune_pal_empyreal_ward);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(400504))
            return GetCaster()->GetAura(400504);

        if (GetCaster()->HasAura(400505))
            return GetCaster()->GetAura(400505);

        if (GetCaster()->HasAura(400506))
            return GetCaster()->GetAura(400506);

        if (GetCaster()->HasAura(400507))
            return GetCaster()->GetAura(400507);

        if (GetCaster()->HasAura(400508))
            return GetCaster()->GetAura(400508);

        if (GetCaster()->HasAura(400509))
            return GetCaster()->GetAura(400509);

        return nullptr;
    }

    void HandleProc()
    {
        if (!GetRuneAura())
            return;

        int32 procSpell = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->CastSpell(GetCaster(), procSpell, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_pal_empyreal_ward::HandleProc);
    }
};

class rune_pal_echoing_hands_freedom : public AuraScript
{
    PrepareAuraScript(rune_pal_echoing_hands_freedom);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(400516))
            GetCaster()->AddAura(400522, GetTarget());

        if (GetCaster()->HasAura(400517))
            GetCaster()->AddAura(400523, GetTarget());

        if (GetCaster()->HasAura(400518))
            GetCaster()->AddAura(400524, GetTarget());

        if (GetCaster()->HasAura(400519))
            GetCaster()->AddAura(400525, GetTarget());

        if (GetCaster()->HasAura(400520))
            GetCaster()->AddAura(400526, GetTarget());

        if (GetCaster()->HasAura(400521))
            GetCaster()->AddAura(400527, GetTarget());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_pal_echoing_hands_freedom::HandleRemove, EFFECT_2, SPELL_AURA_MOD_INCREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pal_echoing_hands_protection : public AuraScript
{
    PrepareAuraScript(rune_pal_echoing_hands_protection);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(400516))
            GetCaster()->AddAura(400528, GetTarget());

        if (GetCaster()->HasAura(400517))
            GetCaster()->AddAura(400529, GetTarget());

        if (GetCaster()->HasAura(400518))
            GetCaster()->AddAura(400530, GetTarget());

        if (GetCaster()->HasAura(400519))
            GetCaster()->AddAura(400531, GetTarget());

        if (GetCaster()->HasAura(400520))
            GetCaster()->AddAura(400532, GetTarget());

        if (GetCaster()->HasAura(400521))
            GetCaster()->AddAura(400533, GetTarget());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_pal_echoing_hands_protection::HandleRemove, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pal_shock_trooper : public AuraScript
{
    PrepareAuraScript(rune_pal_shock_trooper);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_PALADIN_HOLY_SHOCK, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_shock_trooper::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_avenging_shock : public AuraScript
{
    PrepareAuraScript(rune_pal_avenging_shock);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_PALADIN_AVENGING_WRATH, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_avenging_shock::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_shock_barrier : public AuraScript
{
    PrepareAuraScript(rune_pal_shock_barrier);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target)
            return;

        int32 amount = int32(CalculatePct(eventInfo.GetHealInfo()->GetHeal(), aurEff->GetAmount()));

        GetCaster()->CastCustomSpell(RUNE_PALADIN_SHOCK_BARRIER_SHIELD, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_shock_barrier::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_shock_barrier::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_dynamic : public AuraScript
{
    PrepareAuraScript(rune_pal_dynamic);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Aura* auraEff = GetCaster()->GetAura(SPELL_PALADIN_DIVINE_ILLUMINATION))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + 2000, auraEff->GetMaxDuration() + 5000));

            auraEff->SetDuration(duration);
        }
        else
        {
            GetCaster()->AddAura(SPELL_PALADIN_DIVINE_ILLUMINATION, GetCaster());
            GetCaster()->GetAura(SPELL_PALADIN_DIVINE_ILLUMINATION)->SetDuration(aurEff->GetAmount());
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_dynamic::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_imbued_infusions : public AuraScript
{
    PrepareAuraScript(rune_pal_imbued_infusions);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(400576))
            return GetCaster()->GetAura(400576);

        if (GetCaster()->HasAura(400577))
            return GetCaster()->GetAura(400577);

        if (GetCaster()->HasAura(400578))
            return GetCaster()->GetAura(400578);

        if (GetCaster()->HasAura(400579))
            return GetCaster()->GetAura(400579);

        if (GetCaster()->HasAura(400580))
            return GetCaster()->GetAura(400580);

        if (GetCaster()->HasAura(400581))
            return GetCaster()->GetAura(400581);

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_PALADIN_HOLY_SHOCK, -GetRuneAura()->GetEffect(EFFECT_0)->GetAmount());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_pal_imbued_infusions::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pal_power_of_the_silver_hand_check : public AuraScript
{
    PrepareAuraScript(rune_pal_power_of_the_silver_hand_check);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();

        if (GetCaster()->HasAura(procSpell))
            return;

        GetCaster()->CastSpell(GetCaster(), procSpell, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_power_of_the_silver_hand_check::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_power_of_the_silver_hand : public AuraScript
{
    PrepareAuraScript(rune_pal_power_of_the_silver_hand);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetHealInfo() && !eventInfo.GetDamageInfo())
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 amountPct = aurEff->GetAmount();
        int32 amount = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();

        if (eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0)
        {
            int32 healAmount = int32(CalculatePct(eventInfo.GetHealInfo()->GetHeal(), amountPct));
            amount += healAmount;
        }
        else if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damageAmount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), amountPct));
            amount += damageAmount;
        }

        aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(amount);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 amount = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        int32 maxAmount = GetCaster()->CountPctFromMaxHealth(50);

        amount = std::min<int32>(amount, maxAmount);

        GetCaster()->CastCustomSpell(RUNE_PALADIN_POWER_OF_THE_SILVER_HAND_BUFF, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_power_of_the_silver_hand::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_power_of_the_silver_hand::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_pal_power_of_the_silver_hand::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pal_veneration : public AuraScript
{
    PrepareAuraScript(rune_pal_veneration);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_PALADIN_HAMMER_OF_WRATH, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_veneration::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_pal_avenger : public AuraScript
{
    PrepareAuraScript(rune_pal_avenger);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(400608))
            return GetCaster()->GetAura(400608);

        if (GetCaster()->HasAura(400609))
            return GetCaster()->GetAura(400609);

        if (GetCaster()->HasAura(400610))
            return GetCaster()->GetAura(400610);

        if (GetCaster()->HasAura(400611))
            return GetCaster()->GetAura(400611);

        if (GetCaster()->HasAura(400612))
            return GetCaster()->GetAura(400612);

        if (GetCaster()->HasAura(400613))
            return GetCaster()->GetAura(400613);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        int32 amount = GetRuneAura()->GetEffect(EFFECT_1)->GetAmount();

        GetCaster()->CastCustomSpell(RUNE_PALADIN_AVENGER_BUFF, SPELLVALUE_BASE_POINT0, -amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(RUNE_PALADIN_AVENGER_BUFF))
            GetCaster()->RemoveAura(RUNE_PALADIN_AVENGER_BUFF);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_pal_avenger::HandleProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_pal_avenger::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pal_protection_of_tyr : public AuraScript
{
    PrepareAuraScript(rune_pal_protection_of_tyr);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(400616))
            return GetCaster()->GetAura(400616);

        if (GetCaster()->HasAura(400617))
            return GetCaster()->GetAura(400617);

        if (GetCaster()->HasAura(400618))
            return GetCaster()->GetAura(400618);

        if (GetCaster()->HasAura(400619))
            return GetCaster()->GetAura(400619);

        if (GetCaster()->HasAura(400620))
            return GetCaster()->GetAura(400620);

        if (GetCaster()->HasAura(400621))
            return GetCaster()->GetAura(400621);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        int32 procSpell = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(procSpell, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(400622))
            GetCaster()->RemoveAura(400622);

        if (GetCaster()->HasAura(400623))
            GetCaster()->RemoveAura(400623);

        if (GetCaster()->HasAura(400624))
            GetCaster()->RemoveAura(400624);

        if (GetCaster()->HasAura(400625))
            GetCaster()->RemoveAura(400625);

        if (GetCaster()->HasAura(400626))
            GetCaster()->RemoveAura(400626);

        if (GetCaster()->HasAura(400627))
            GetCaster()->RemoveAura(400627);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_pal_protection_of_tyr::HandleProc, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_pal_protection_of_tyr::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pal_awakening : public AuraScript
{
    PrepareAuraScript(rune_pal_awakening);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 fullDuration = aurEff->GetAmount();
        int32 increasedDuration = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();

        if (Aura* auraEff = GetCaster()->GetAura(SPELL_PALADIN_AVENGING_WRATH))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + increasedDuration, auraEff->GetMaxDuration() + 5000));

            auraEff->SetDuration(duration);
        }
        else
        {
            GetCaster()->AddAura(SPELL_PALADIN_AVENGING_WRATH, GetCaster());
            GetCaster()->GetAura(SPELL_PALADIN_AVENGING_WRATH)->SetDuration(fullDuration);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_awakening::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_tower_of_radiance : public AuraScript
{
    PrepareAuraScript(rune_pal_tower_of_radiance);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetHealInfo()->GetTarget();

        if (!victim)
            return;

        if (victim->HasAura(SPELL_PALADIN_BEACON_OF_LIGHT) || victim->HasAura(SPELL_PALADIN_BEACON_OF_VIRTUE) || victim->HasAura(SPELL_PALADIN_BEACON_OF_FAITH))
            GetCaster()->SetPower(POWER_ENERGY, GetCaster()->GetPower(POWER_ENERGY) + 1);
        else
        {
            int32 healthPct = victim->GetHealthPct();
            uint32 random = urand(1, 100);

            if (random <= healthPct)
                GetCaster()->SetPower(POWER_ENERGY, GetCaster()->GetPower(POWER_ENERGY) + 1);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_pal_tower_of_radiance::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_saved_by_the_light : public AuraScript
{
    PrepareAuraScript(rune_pal_saved_by_the_light);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim->HasAura(SPELL_PALADIN_BEACON_OF_LIGHT) || !victim->HasAura(SPELL_PALADIN_BEACON_OF_VIRTUE) || !victim->HasAura(SPELL_PALADIN_BEACON_OF_FAITH))
            return false;

        if (victim->HasAura(RUNE_PALADIN_SAVED_BY_THE_LIGHT_DEBUFF))
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
        int32 remainingHealth = victim->GetHealth() - eventInfo.GetDamageInfo()->GetDamage();
        int32 healhPct = aurEff->GetAmount();

        if (remainingHealth > victim->CountPctFromMaxHealth(healhPct))
            return;

        float ap = int32(CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount()));
        float sp = int32(CalculatePct(GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY), aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount()));
        int32 amount = std::max<int32>(0, int32(ap + sp));

        GetCaster()->CastCustomSpell(RUNE_PALADIN_SAVED_BY_THE_LIGHT_SHIELD, SPELLVALUE_BASE_POINT0, amount, GetTarget(), TRIGGERED_FULL_MASK);
        GetCaster()->AddAura(RUNE_PALADIN_SAVED_BY_THE_LIGHT_DEBUFF, victim);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_saved_by_the_light::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_saved_by_the_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pal_sign_of_faith : public AuraScript
{
    PrepareAuraScript(rune_pal_sign_of_faith);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim->HasAura(SPELL_PALADIN_BEACON_OF_LIGHT) || !victim->HasAura(SPELL_PALADIN_BEACON_OF_VIRTUE) || !victim->HasAura(SPELL_PALADIN_BEACON_OF_FAITH))
            return false;

        if (victim->HasAura(RUNE_PALADIN_SAVED_BY_THE_LIGHT_DEBUFF))
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
        int32 remainingHealth = victim->GetHealth() - eventInfo.GetDamageInfo()->GetDamage();
        int32 healhPct = aurEff->GetAmount();

        if (remainingHealth > victim->CountPctFromMaxHealth(healhPct))
            return;

        float ap = int32(CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount()));
        float sp = int32(CalculatePct(GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY), aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount()));
        int32 amount = std::max<int32>(0, int32(ap + sp));

        GetCaster()->CastCustomSpell(RUNE_PALADIN_SAVED_BY_THE_LIGHT_SHIELD, SPELLVALUE_BASE_POINT0, amount, GetTarget(), TRIGGERED_FULL_MASK);
        GetCaster()->AddAura(RUNE_PALADIN_SAVED_BY_THE_LIGHT_DEBUFF, victim);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_pal_sign_of_faith::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pal_sign_of_faith::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
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
    RegisterSpellScript(rune_pal_light_of_the_titans);
    RegisterSpellScript(rune_pal_answered_prayer);
    RegisterSpellScript(rune_pal_hand_of_the_protector);
    RegisterSpellScript(rune_pal_ashes_to_ashes);
    RegisterSpellScript(rune_pal_tirions_devotion);
    RegisterSpellScript(rune_pal_empyreal_ward);
    RegisterSpellScript(rune_pal_echoing_hands_freedom);
    RegisterSpellScript(rune_pal_echoing_hands_protection);
    RegisterSpellScript(rune_pal_shock_trooper);
    RegisterSpellScript(rune_pal_avenging_shock);
    RegisterSpellScript(rune_pal_shock_barrier);
    RegisterSpellScript(rune_pal_dynamic);
    RegisterSpellScript(rune_pal_imbued_infusions);
    RegisterSpellScript(rune_pal_power_of_the_silver_hand_check);
    RegisterSpellScript(rune_pal_power_of_the_silver_hand);
    RegisterSpellScript(rune_pal_veneration);
    RegisterSpellScript(rune_pal_avenger);
    RegisterSpellScript(rune_pal_protection_of_tyr);
    RegisterSpellScript(rune_pal_awakening);
    RegisterSpellScript(rune_pal_tower_of_radiance);
    RegisterSpellScript(rune_pal_saved_by_the_light);
    RegisterSpellScript(rune_pal_sign_of_faith);
}
