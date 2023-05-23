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
#include "Object.h"

enum Masteries
{

};

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
            if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(300110, 0)) {
                int32 remainingTicks = totalTicks - protEff->GetTickNumber();
                int32 remainingAmount = protEff->GetAmount() * remainingTicks / totalTicks;
                amount += remainingAmount;
            }
            GetCaster()->CastCustomSpellTrigger(300110, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), TRIGGERED_IGNORE_AURA_SCALING);
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
        GetCaster()->CastCustomSpell(200002, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
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


//Paladin
class spell_mastery_lightbringer : public AuraScript
{
    PrepareAuraScript(spell_mastery_lightbringer);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        int32 defaultValue = aurEff->GetAmount();
        float mastery = GetCaster()->ToPlayer()->GetMastery();

        if (!procInfo.GetHealInfo())
            return;
        if (procInfo.GetHealInfo()->GetSpellInfo()->Id == 400001)
            return;

        int32 healAmount = procInfo.GetHealInfo()->GetHeal();

        int32 effectiveness = 0;
        int32 effectiveValue = defaultValue + mastery;

        Unit* target = procInfo.GetActionTarget();
        if (!target)
            return;
        Position targetPos = target->GetPosition();
        float distance = GetCaster()->GetDistance(targetPos);

        if (distance >= 40)
            effectiveness = 0;
        if (distance <= 10)
            effectiveness = 100; 
        if (distance > 10)
            effectiveness = (40 - distance) * 3.33;

        int32 finalAmount = CalculatePct(effectiveValue, effectiveness);
        int32 finalHeal = ApplyPct(healAmount, finalAmount);

        GetCaster()->CastCustomSpell(400001, SPELLVALUE_BASE_POINT0, finalHeal, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_lightbringer::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_divine_bulwark : public SpellScript
{
    PrepareSpellScript(spell_mastery_divine_bulwark);

    void HandleCast()
    {
        float mastery = GetCaster()->ToPlayer()->GetMastery();
        int32 attackPower = (GetCaster()->GetAura(400002)->GetEffect(EFFECT_0)->GetAmount()) + mastery;
        int32 blockRating = (GetCaster()->GetAura(400002)->GetEffect(EFFECT_2)->GetAmount()) + mastery;

        GetCaster()->CastCustomSpell(GetCaster(), 400005, &attackPower, &blockRating, nullptr, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_divine_bulwark::HandleCast);
    }
};

class spell_mastery_divine_bulwark_consec : public SpellScript
{
    PrepareSpellScript(spell_mastery_divine_bulwark_consec);

    void HandleCast()
    {
        if (!GetCaster()->HasAura(400002))
            return;

        float mastery = GetCaster()->ToPlayer()->GetMastery();
        int32 damageReduction = (GetCaster()->GetAura(400002)->GetEffect(EFFECT_1)->GetAmount()) + mastery;

        GetCaster()->CastCustomSpell(400003, SPELLVALUE_BASE_POINT0, damageReduction, GetCaster(), true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_divine_bulwark_consec::HandleCast);
    }
};

class spell_mastery_divine_bulwark_selection : public AuraScript
{
    PrepareAuraScript(spell_mastery_divine_bulwark_selection);

    void HandleScriptEffect(AuraEffect* aurEff)
    {
        Creature* creature = GetCaster()->FindNearestCreature(500502, 30);
        if (!creature)
            return;

        Position player = GetCaster()->GetPosition();
        float distance = creature->GetDistance(player);

        if (const SpellInfo* info = sSpellMgr->GetSpellInfo(48819))
        {
            float radius = info->Effects[EFFECT_0].CalcRadius(GetCaster());

            if (distance > radius)
                GetCaster()->RemoveAura(400003);
            else
                GetCaster()->AddAura(400003, GetCaster());
        }
    }

    void Register() override
    {
        OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_mastery_divine_bulwark_selection::HandleScriptEffect, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_mastery_hand_of_light : public SpellScript
{
    PrepareSpellScript(spell_mastery_hand_of_light);

    void HandleCast()
    {
        float mastery = GetCaster()->ToPlayer()->GetMastery();
        int32 holyPower = (GetCaster()->GetAura(400006)->GetEffect(EFFECT_0)->GetAmount()) + mastery;

        GetCaster()->CastCustomSpell(400007, SPELLVALUE_BASE_POINT0, holyPower, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_hand_of_light::HandleCast);
    }
};


//Hunter
class spell_mastery_master_of_beasts : public SpellScript
{
    PrepareSpellScript(spell_mastery_master_of_beasts);

    void HandleCast()
    {
        float mastery = GetCaster()->ToPlayer()->GetMastery();
        int32 damageBonus = (GetCaster()->GetAura(500003)->GetEffect(EFFECT_0)->GetAmount()) + mastery;

        GetCaster()->CastCustomSpell(500000, SPELLVALUE_BASE_POINT0, damageBonus, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_master_of_beasts::HandleCast);
    }
};

class spell_mastery_sniper_training : public SpellScript
{
    PrepareSpellScript(spell_mastery_sniper_training);

    void HandleCast()
    {
        float mastery = GetCaster()->ToPlayer()->GetMastery();
        int32 rangeBonus = (GetCaster()->GetAura(500004)->GetEffect(EFFECT_0)->GetAmount()) + mastery;
        int32 damageBonus = (GetCaster()->GetAura(500004)->GetEffect(EFFECT_1)->GetAmount()) + mastery;

        GetCaster()->CastCustomSpell(GetCaster(), 500001, &rangeBonus, &damageBonus, nullptr, true, nullptr, nullptr);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_sniper_training::HandleCast);
    }
};

//Druid
class spell_mastery_astral_invocation : public AuraScript
{
    PrepareAuraScript(spell_mastery_astral_invocation);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleCast(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        float mastery = GetCaster()->ToPlayer()->GetMastery();
        int32 natureDamageBonus = (GetCaster()->GetAura(700000)->GetEffect(EFFECT_0)->GetAmount()) + mastery;
        int32 arcaneDamageBonus = (GetCaster()->GetAura(700000)->GetEffect(EFFECT_1)->GetAmount()) + mastery;
        Unit* target = procInfo.GetActionTarget();

        int32 schoolMask = procInfo.GetSpellInfo()->GetSchoolMask();

        if (schoolMask & SPELL_SCHOOL_MASK_NATURE && target->HasAura(80518))
        {
            int32 rawDamage = procInfo.GetDamageInfo()->GetDamage();
            int32 pctDamage = CalculatePct(rawDamage, natureDamageBonus);           
            GetCaster()->CastCustomSpell(700002, SPELLVALUE_BASE_POINT0, std::max(1,pctDamage), target, TRIGGERED_FULL_MASK);
        }

        if (schoolMask & SPELL_SCHOOL_MASK_ARCANE && target->HasAura(48463))
        {
            int32 rawDamage = procInfo.GetDamageInfo()->GetDamage();
            int32 pctDamage = CalculatePct(rawDamage, arcaneDamageBonus);
            GetCaster()->CastCustomSpell(700003, SPELLVALUE_BASE_POINT0, std::max(1, pctDamage), target, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_astral_invocation::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_astral_invocation::HandleCast, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_razor_claws: public SpellScript
{
    PrepareSpellScript(spell_mastery_razor_claws);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        float mastery = caster->ToPlayer()->GetMastery();
        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(700004);
        int32 damageBonus = spell->GetEffect(EFFECT_0).CalcValue(caster) + mastery;

        caster->CastCustomSpell(caster, 700005, &damageBonus, &damageBonus, nullptr, true, nullptr, nullptr);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_razor_claws::HandleCast);
    }
};

class spell_mastery_natures_guardian : public SpellScript
{
    PrepareSpellScript(spell_mastery_natures_guardian);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        float mastery = caster->ToPlayer()->GetMastery();
        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(700007);
        int32 damageBonus = spell->GetEffect(EFFECT_0).CalcValue(caster) + mastery;

        caster->CastCustomSpell(caster, 700005, &damageBonus, &damageBonus, nullptr, true, nullptr, nullptr);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_natures_guardian::HandleCast);
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
    RegisterSpellScript(spell_mastery_lightbringer);
    RegisterSpellScript(spell_mastery_divine_bulwark);
    RegisterSpellScript(spell_mastery_divine_bulwark_consec);
    RegisterSpellScript(spell_mastery_divine_bulwark_selection);
    RegisterSpellScript(spell_mastery_hand_of_light);
    RegisterSpellScript(spell_mastery_master_of_beasts);
    RegisterSpellScript(spell_mastery_sniper_training);
    RegisterSpellScript(spell_mastery_astral_invocation);
    RegisterSpellScript(spell_mastery_razor_claws);
    RegisterSpellScript(spell_mastery_natures_guardian);
}
