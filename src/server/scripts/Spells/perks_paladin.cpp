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

class rune_pal_touch_of_light : public AuraScript
{
    PrepareAuraScript(rune_pal_touch_of_light);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        LOG_ERROR("error", "touch of light");
        if (!eventInfo.GetDamageInfo() && !eventInfo.GetHealInfo())
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 procSpell = 0;
        float apPct = 0;
        float spPct = 0;
        LOG_ERROR("error", "{} {}", eventInfo.GetDamageInfo()->GetDamage(), eventInfo.GetHealInfo()->GetHeal());
        if (eventInfo.GetDamageInfo())
        {
            procSpell = 400046;
            apPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_1).DamageMultiplier;
            spPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_1).BonusMultiplier;
        }
        else if (eventInfo.GetHealInfo())
        {
            procSpell = 400047;
            apPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_2).DamageMultiplier;
            spPct = aurEff->GetSpellInfo()->GetEffect(EFFECT_2).BonusMultiplier;
        }
        LOG_ERROR("error", "{} {} {}", procSpell, apPct, spPct);
        float ap = CalculatePct(int32(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK)), apPct);
        float sp = CalculatePct(int32(GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY)), spPct);
        int32 amount = std::max<int32>(0, int32(ap + sp));
        LOG_ERROR("error", "{} {} {}", ap, sp, amount);
        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
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
        LOG_ERROR("error", "justice clemency");
        if (!eventInfo.GetDamageInfo())
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
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

void AddSC_paladin_perks_scripts()
{
    RegisterSpellScript(rune_pal_touch_of_light);
    RegisterSpellScript(rune_pal_justice_clemency);
    RegisterSpellScript(rune_pal_fist_of_justice);
}
