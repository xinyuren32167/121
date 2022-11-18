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


class spell_vampirism : public AuraScript
{
    PrepareAuraScript(spell_vampirism);

    Aura* GetPerkAura()
    {
        if (GetCaster()->HasAura(100000))
            return GetCaster()->GetAura(100000);

        if (GetCaster()->HasAura(100001))
            return GetCaster()->GetAura(100001);

        if (GetCaster()->HasAura(100002))
            return GetCaster()->GetAura(100002);

        if (GetCaster()->HasAura(100003))
            return GetCaster()->GetAura(100003);

        if (GetCaster()->HasAura(100004))
            return GetCaster()->GetAura(100004);

        if (GetCaster()->HasAura(100005))
            return GetCaster()->GetAura(100005);

        return nullptr;
    }

    int GetProcPct()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetProcPct()));

        GetCaster()->CastCustomSpell(100006, SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_vampirism::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_second_wind : public AuraScript
{
    PrepareAuraScript(spell_second_wind);

    Aura* GetPerkAura()
    {
        if (GetCaster()->HasAura(100019))
            return GetCaster()->GetAura(100019);

        if (GetCaster()->HasAura(100020))
            return GetCaster()->GetAura(100020);

        if (GetCaster()->HasAura(100021))
            return GetCaster()->GetAura(100021);

        if (GetCaster()->HasAura(100022))
            return GetCaster()->GetAura(100022);

        if (GetCaster()->HasAura(100023))
            return GetCaster()->GetAura(100023);

        if (GetCaster()->HasAura(100024))
            return GetCaster()->GetAura(100024);

        return nullptr;
    }

    int GetProcPct()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints + 1;
    }

    int GetDamageCheckSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    int GetHealSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        if (GetCaster()->GetHealth() < GetCaster()->GetMaxHealth() && !GetCaster()->HasAura(GetDamageCheckSpell()))
        {
            int32 amount = int32(CalculatePct(GetCaster()->GetMaxHealth(), GetProcPct()));

            GetCaster()->CastCustomSpell(GetHealSpell(), SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_second_wind::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_medic_now : public AuraScript
{
    PrepareAuraScript(spell_medic_now);

    Aura* GetPerkAura()
    {
        if (GetCaster()->HasAura(100034))
            return GetCaster()->GetAura(100034);

        if (GetCaster()->HasAura(100035))
            return GetCaster()->GetAura(100035);

        if (GetCaster()->HasAura(100036))
            return GetCaster()->GetAura(100036);

        if (GetCaster()->HasAura(100037))
            return GetCaster()->GetAura(100037);

        if (GetCaster()->HasAura(100038))
            return GetCaster()->GetAura(100038);

        if (GetCaster()->HasAura(100039))
            return GetCaster()->GetAura(100039);

        return nullptr;
    }

    int GetProcPct()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetProcPct()));

        GetCaster()->CastCustomSpell(100040, SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_medic_now::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_overwhelming_power : public SpellScript
{
    PrepareSpellScript(spell_overwhelming_power);

    Aura* GetPerkAura()
    {
        if (GetCaster()->HasAura(100041))
            return GetCaster()->GetAura(100041);

        if (GetCaster()->HasAura(100042))
            return GetCaster()->GetAura(100042);

        if (GetCaster()->HasAura(100043))
            return GetCaster()->GetAura(100043);

        if (GetCaster()->HasAura(100044))
            return GetCaster()->GetAura(100044);

        if (GetCaster()->HasAura(100045))
            return GetCaster()->GetAura(100045);

        if (GetCaster()->HasAura(100046))
            return GetCaster()->GetAura(100046);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    void HandleProc()
    {
        if (!GetCaster()->HasAura(GetProcSpell()))
            GetCaster()->AddAura(GetProcSpell(), GetCaster());
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_overwhelming_power::HandleProc);
    }
};

class spell_overwhelming_power_stack_remove : public AuraScript
{
    PrepareAuraScript(spell_overwhelming_power_stack_remove);

    Aura* GetBuffAura()
    {
        if (GetCaster()->HasAura(100047))
            return GetCaster()->GetAura(100047);

        return nullptr;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (GetBuffAura())
        {
            int stackAmount = GetBuffAura()->GetStackAmount();

            if (stackAmount > 0)
                GetBuffAura()->ModStackAmount(-1);
        }
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        if (GetBuffAura())
        {
            int stackAmount = GetBuffAura()->GetStackAmount();

            if (stackAmount > 0)
                GetBuffAura()->ModStackAmount(-1);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_overwhelming_power_stack_remove::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectProc += AuraEffectProcFn(spell_overwhelming_power_stack_remove::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_bulwark : public AuraScript
{
    PrepareAuraScript(spell_bulwark);

    Aura* GetPerkAura()
    {
        if (GetCaster()->HasAura(100072))
            return GetCaster()->GetAura(100072);

        if (GetCaster()->HasAura(100073))
            return GetCaster()->GetAura(100073);

        if (GetCaster()->HasAura(100074))
            return GetCaster()->GetAura(100074);

        if (GetCaster()->HasAura(100075))
            return GetCaster()->GetAura(100075);

        if (GetCaster()->HasAura(100076))
            return GetCaster()->GetAura(100076);

        if (GetCaster()->HasAura(100077))
            return GetCaster()->GetAura(100077);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    int GetProcPct()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 amount = int32(CalculatePct(GetCaster()->GetMaxHealth(), GetProcPct()));

        GetCaster()->CastCustomSpell(GetProcSpell(), SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_bulwark::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

void AddSC_generals_perks_scripts()
{
    RegisterSpellScript(spell_vampirism);
    RegisterSpellScript(spell_second_wind);
    RegisterSpellScript(spell_medic_now);
    RegisterSpellScript(spell_overwhelming_power);
    RegisterSpellScript(spell_overwhelming_power_stack_remove);
    RegisterSpellScript(spell_bulwark);
}
