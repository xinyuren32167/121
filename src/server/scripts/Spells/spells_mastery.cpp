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

// Mage
class spell_icicle_ice_lance : public SpellScript
{
    PrepareSpellScript(spell_icicle_ice_lance);

    Aura* GetBuffAura()
    {
        if (GetCaster()->HasAura(300106))
            return GetCaster()->GetAura(300106);

        return nullptr;
    }

    void HandleProc()
    {
        if (GetBuffAura())
        {
            GetCaster()->CastSpell(GetExplTargetUnit(), 300108, TRIGGERED_FULL_MASK);
            GetExplTargetUnit()->GetAura(300108)->SetDuration(GetBuffAura()->GetStackAmount() * 200);
            GetCaster()->RemoveAura(GetBuffAura());
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_icicle_ice_lance::HandleProc);
    }
};

class spell_icicle_ice_lance_aura : public AuraScript
{
    PrepareAuraScript(spell_icicle_ice_lance_aura);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300105))
            return GetCaster()->GetAura(300105);

        return nullptr;
    }

    int GetProcPct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        if (GetCaster()->ToPlayer())
        {
            float pct = GetCaster()->ToPlayer()->GetMastery() + GetProcPct();
            int32 amount = int32(CalculatePct(GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST), pct));

            GetCaster()->CastCustomSpell(300107, SPELLVALUE_BASE_POINT0, amount, GetTarget(), true);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_icicle_ice_lance_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_icicle_frostbolt : public SpellScript
{
    PrepareSpellScript(spell_icicle_frostbolt);

    int GetStackIcicle()
    {
        return GetCaster()->GetAura(300106)->GetStackAmount();
    }

    int GetProcSpell()
    {
        return GetCaster()->GetAura(300106)->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    int GetProcPct()
    {
        return GetCaster()->GetAura(300105)->GetEffect(EFFECT_0)->GetAmount();
    }

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        if (player->HasAura(300106)) {
            if (GetStackIcicle() == 5) {
                float pct = player->GetMastery() + GetProcPct();
                int32 amount = int32(CalculatePct(player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST), pct));
                player->CastCustomSpell(GetProcSpell(), SPELLVALUE_BASE_POINT0, amount, GetExplTargetUnit(), true);
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_icicle_frostbolt::HandleProc);
    }
};

class spell_mastery_ignite : public AuraScript
{
    PrepareAuraScript(spell_mastery_ignite);

    int GetDamagePct()
    {
        return GetCaster()->GetAura(300109)->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0) {
            float pct = GetDamagePct() + GetCaster()->ToPlayer()->GetMastery();
            int32 totalTicks = sSpellMgr->AssertSpellInfo(300110)->GetMaxTicks();
            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), pct) / totalTicks);
         
            eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), 300110, SPELL_AURA_PERIODIC_DAMAGE, amount, TRIGGERED_IGNORE_AURA_SCALING);
        }
      
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_ignite::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_savant : public SpellScript
{
    PrepareSpellScript(spell_mastery_savant);

    int GetManaPct()
    {
        return GetCaster()->GetAura(300111)->GetEffect(EFFECT_1)->GetAmount();
    }

    int GetDamagePct()
    {
        return GetCaster()->GetAura(300111)->GetEffect(EFFECT_2)->GetAmount();
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;

        int32 manaPct = GetManaPct() + caster->ToPlayer()->GetMastery();
        int32 dmgPct = GetDamagePct() + caster->ToPlayer()->GetMastery();
        caster->CastCustomSpell(caster, 300112, &manaPct, &manaPct, &dmgPct, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_savant::HandleCast);
    }
};

class spell_mastery_savant_on_remove : public AuraScript
{
    PrepareAuraScript(spell_mastery_savant_on_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetCaster()->RemoveAura(300112);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_mastery_savant_on_remove::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};



// Warrior
class spell_mastery_deep_wounds : public AuraScript
{
    PrepareAuraScript(spell_mastery_deep_wounds);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 amount = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount());
        eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), 200001, SPELL_AURA_PERIODIC_DAMAGE, amount);
    }

    void HandleBuff(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        float amount = aurEff->GetAmount() + GetCaster()->ToPlayer()->GetMastery();
        GetCaster()->CastCustomSpell(200002, SPELLVALUE_BASE_POINT0, amount, GetTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_deep_wounds::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectProc += AuraEffectProcFn(spell_mastery_deep_wounds::HandleBuff, EFFECT_1, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_unshackled_fury : public AuraScript
{
    PrepareAuraScript(spell_mastery_unshackled_fury);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();

        if (!caster)
            return;

        int32 amount = aurEff->GetAmount() + caster->GetMastery();
        caster->CastCustomSpell(200004, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_unshackled_fury::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_critical_block : public SpellScript
{
    PrepareSpellScript(spell_mastery_critical_block);

    int GetEffectAmount(SpellEffIndex effect)
    {
        return GetCaster()->GetAura(200005)->GetEffect(effect)->GetAmount();
    }

    void HandleCast()
    {
        int32 blockAmount = GetEffectAmount(EFFECT_0) + GetCaster()->ToPlayer()->GetMastery();
        int32 critBlockChance = GetEffectAmount(EFFECT_1) + GetCaster()->ToPlayer()->GetMastery() + GetCaster()->GetFloatValue(PLAYER_CRIT_PERCENTAGE);
        int32 powerAmount = GetEffectAmount(EFFECT_2) + GetCaster()->ToPlayer()->GetMastery();
        GetCaster()->RemoveAura(200006);
        GetCaster()->CastCustomSpell(GetCaster(), 200006, &blockAmount, &critBlockChance, &powerAmount, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_critical_block::HandleCast);
    }
};

class spell_mastery_critical_block_on_remove : public AuraScript
{
    PrepareAuraScript(spell_mastery_critical_block_on_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetCaster()->RemoveAura(100750);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_mastery_critical_block_on_remove::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_spells_mastery_scripts()
{
    RegisterSpellScript(spell_icicle_ice_lance);
    RegisterSpellScript(spell_icicle_frostbolt);
    RegisterSpellScript(spell_icicle_ice_lance_aura);
    RegisterSpellScript(spell_mastery_ignite);
    RegisterSpellScript(spell_mastery_savant);
    RegisterSpellScript(spell_mastery_deep_wounds);
    RegisterSpellScript(spell_mastery_unshackled_fury);
    RegisterSpellScript(spell_mastery_critical_block);
    RegisterSpellScript(spell_mastery_savant_on_remove);
    RegisterSpellScript(spell_mastery_critical_block_on_remove);
}
