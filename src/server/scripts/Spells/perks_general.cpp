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



void AddSC_generals_perks_scripts()
{
    RegisterSpellScript(spell_vampirism);
    RegisterSpellScript(spell_second_wind);
}
