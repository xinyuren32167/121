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

    Aura* GetRuneAura()
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
        if (!GetRuneAura())
            return 0;

        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        if (damage) {
            GetCaster()->CastCustomSpellPct(100006, SPELLVALUE_BASE_POINT0,
                std::max(1, damage), GetProcPct(), false, false, false, 0, GetCaster());
        }
       
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_vampirism::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_second_wind : public AuraScript
{
    PrepareAuraScript(spell_second_wind);

    Aura* GetRuneAura()
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
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints + 1;
    }

    int GetDamageCheckSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    int GetHealSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
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

    Aura* GetRuneAura()
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
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
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

    Aura* GetRuneAura()
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
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
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

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        if (!GetCaster())
            return;

        int32 amount = int32(CalculatePct(GetCaster()->GetMaxHealth(), aurEff->GetAmount()));

        GetCaster()->CastCustomSpell(100078, SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_bulwark::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_pyretic_incantation : public AuraScript
{
    PrepareAuraScript(spell_pyretic_incantation);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100079))
            return GetCaster()->GetAura(100079);

        if (GetCaster()->HasAura(100080))
            return GetCaster()->GetAura(100080);

        if (GetCaster()->HasAura(100081))
            return GetCaster()->GetAura(100081);

        if (GetCaster()->HasAura(100082))
            return GetCaster()->GetAura(100082);

        if (GetCaster()->HasAura(100083))
            return GetCaster()->GetAura(100083);

        if (GetCaster()->HasAura(100084))
            return GetCaster()->GetAura(100084);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT)
            GetCaster()->AddAura(GetProcSpell(), GetCaster());

        if (eventInfo.GetHitMask() & PROC_EX_NORMAL_HIT && GetCaster()->HasAura(GetProcSpell()))
            GetCaster()->RemoveAura(GetProcSpell());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pyretic_incantation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_gutripper : public AuraScript
{
    PrepareAuraScript(spell_gutripper);

    int GetProcChance(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetProcTarget()->GetHealthPct() < 35)
            return 60;

        return 20;
    }

    void HandleProc(AuraEffect const*  aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetCaster())
            return;

        if(eventInfo.GetProcTarget() &&
            eventInfo.GetDamageInfo() &&
            eventInfo.GetDamageInfo()->GetDamage() > 0) {
            uint32 random = urand(1, 100);

            if (random <= GetProcChance(eventInfo))
            {
                int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
                GetCaster()->CastCustomSpell(100097, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_gutripper::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dying_breath : public AuraScript
{
    PrepareAuraScript(spell_dying_breath);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100098))
            return GetCaster()->GetAura(100098);

        if (GetCaster()->HasAura(100099))
            return GetCaster()->GetAura(100099);

        if (GetCaster()->HasAura(100100))
            return GetCaster()->GetAura(100100);

        if (GetCaster()->HasAura(100101))
            return GetCaster()->GetAura(100101);

        if (GetCaster()->HasAura(100102))
            return GetCaster()->GetAura(100102);

        if (GetCaster()->HasAura(100103))
            return GetCaster()->GetAura(100103);

        return nullptr;
    }

    int GetDamagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(100105))
        {
            int32 amount = int32(CalculatePct(GetTarget()->GetMaxHealth(), GetDamagePct()));

            GetCaster()->CastCustomSpell(GetProcSpell(), SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
            GetCaster()->AddAura(100105, GetCaster());
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dying_breath::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_ysera_s_gift : public AuraScript
{
    PrepareAuraScript(spell_ysera_s_gift);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100106))
            return GetCaster()->GetAura(100106);

        if (GetCaster()->HasAura(100107))
            return GetCaster()->GetAura(100107);

        if (GetCaster()->HasAura(100108))
            return GetCaster()->GetAura(100108);

        if (GetCaster()->HasAura(100109))
            return GetCaster()->GetAura(100109);

        if (GetCaster()->HasAura(100110))
            return GetCaster()->GetAura(100110);

        if (GetCaster()->HasAura(100111))
            return GetCaster()->GetAura(100111);

        return nullptr;
    }

    int GetProcPct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetSelfHealSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    int GetAllyHealSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 amount = int32(CalculatePct(GetCaster()->GetMaxHealth(), GetProcPct()));

        if (GetCaster()->GetHealth() < GetCaster()->GetMaxHealth())
            GetCaster()->CastCustomSpell(GetSelfHealSpell(), SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
        else
            GetCaster()->CastCustomSpell(GetAllyHealSpell(), SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_ysera_s_gift::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_ysera_s_gift_target : public SpellScript
{
    PrepareSpellScript(spell_ysera_s_gift_target);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        uint32 const maxTargets = 1;

        if (targets.size() > maxTargets)
        {
            targets.sort(Acore::HealthPctOrderPred());
            targets.resize(maxTargets);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ysera_s_gift_target::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

class spell_echo_of_light : public AuraScript
{
    PrepareAuraScript(spell_echo_of_light);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100114))
            return GetCaster()->GetAura(100114);

        if (GetCaster()->HasAura(100115))
            return GetCaster()->GetAura(100115);

        if (GetCaster()->HasAura(100116))
            return GetCaster()->GetAura(100116);

        if (GetCaster()->HasAura(100117))
            return GetCaster()->GetAura(100117);

        if (GetCaster()->HasAura(100118))
            return GetCaster()->GetAura(100118);

        if (GetCaster()->HasAura(100119))
            return GetCaster()->GetAura(100119);

        return nullptr;
    }

    int GetHealPct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        int32 totalTicks = sSpellMgr->AssertSpellInfo(GetProcSpell())->GetMaxTicks();
        int32 amount = int32(CalculatePct(eventInfo.GetHealInfo()->GetHeal(), GetHealPct()) / totalTicks);
        int32 maxAmount = int32(CalculatePct(eventInfo.GetProcTarget()->GetMaxHealth(), 30));

        if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(GetProcSpell(), 0))
        {
            int32 remainingTicks = totalTicks - protEff->GetTickNumber();
            int32 remainingAmount = protEff->GetAmount() * remainingTicks;
            int32 remainingAmountPerTick = remainingAmount / totalTicks;

            amount = (std::min<int32>(amount + remainingAmountPerTick, maxAmount));
        }
        eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), GetProcSpell(), SPELL_AURA_PERIODIC_HEAL, amount, TRIGGERED_IGNORE_AURA_SCALING);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_echo_of_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mittelschmerz_blood_magic : public AuraScript
{
    PrepareAuraScript(spell_mittelschmerz_blood_magic);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100121))
            return GetCaster()->GetAura(100121);

        if (GetCaster()->HasAura(100122))
            return GetCaster()->GetAura(100122);

        if (GetCaster()->HasAura(100123))
            return GetCaster()->GetAura(100123);

        if (GetCaster()->HasAura(100124))
            return GetCaster()->GetAura(100124);

        if (GetCaster()->HasAura(100125))
            return GetCaster()->GetAura(100125);

        if (GetCaster()->HasAura(100126))
            return GetCaster()->GetAura(100126);

        if (GetCaster()->HasAura(100127))
            return GetCaster()->GetAura(100127);

        if (GetCaster()->HasAura(100128))
            return GetCaster()->GetAura(100128);

        if (GetCaster()->HasAura(100129))
            return GetCaster()->GetAura(100129);

        if (GetCaster()->HasAura(100130))
            return GetCaster()->GetAura(100130);

        if (GetCaster()->HasAura(100131))
            return GetCaster()->GetAura(100131);

        if (GetCaster()->HasAura(100132))
            return GetCaster()->GetAura(100132);

        return nullptr;
    }

    int GetDamagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).Amplitude;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        uint32 damage = CalculatePct(GetCaster()->GetMaxHealth(), GetDamagePct());

        GetCaster()->DealDamage(GetCaster(), GetCaster(), damage);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mittelschmerz_blood_magic::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_azerite_veins : public AuraScript
{
    PrepareAuraScript(spell_azerite_veins);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100135))
            return GetCaster()->GetAura(100135);

        if (GetCaster()->HasAura(100136))
            return GetCaster()->GetAura(100136);

        if (GetCaster()->HasAura(100137))
            return GetCaster()->GetAura(100137);

        if (GetCaster()->HasAura(100138))
            return GetCaster()->GetAura(100138);

        if (GetCaster()->HasAura(100139))
            return GetCaster()->GetAura(100139);

        if (GetCaster()->HasAura(100140))
            return GetCaster()->GetAura(100140);

        return nullptr;
    }

    int GetHealPct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        int32 amount = int32(CalculatePct(GetCaster()->GetMaxHealth(), GetHealPct()));

        GetCaster()->CastCustomSpell(GetProcSpell(), SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_azerite_veins::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_killing_spree : public AuraScript
{
    PrepareAuraScript(spell_killing_spree);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (GetCaster()->GetInstanceId())
            return false;

        return true;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_killing_spree::CheckProc);
    }
};

class spell_holy_aegis : public AuraScript
{
    PrepareAuraScript(spell_holy_aegis);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100211))
            return GetCaster()->GetAura(100211);

        if (GetCaster()->HasAura(100212))
            return GetCaster()->GetAura(100212);

        if (GetCaster()->HasAura(100213))
            return GetCaster()->GetAura(100213);

        if (GetCaster()->HasAura(100214))
            return GetCaster()->GetAura(100214);

        if (GetCaster()->HasAura(100215))
            return GetCaster()->GetAura(100215);

        if (GetCaster()->HasAura(100216))
            return GetCaster()->GetAura(100216);

        return nullptr;
    }

    int GetShieldPct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).Amplitude;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 absorb = int32(CalculatePct(eventInfo.GetHealInfo()->GetHeal(), GetShieldPct()));

        if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(GetProcSpell(), 0))
        {
            int32 maxShield = CalculatePct(eventInfo.GetProcTarget()->GetMaxHealth(), 30);
            protEff->SetAmount(std::min<int32>(protEff->GetAmount() + absorb, maxShield));
            protEff->GetBase()->RefreshDuration();
        }
        else
            GetTarget()->CastCustomSpell(GetProcSpell(), SPELLVALUE_BASE_POINT0, absorb, eventInfo.GetProcTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_holy_aegis::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_shadow_pact : public AuraScript
{
    PrepareAuraScript(spell_shadow_pact);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100218))
            return GetCaster()->GetAura(100218);

        if (GetCaster()->HasAura(100219))
            return GetCaster()->GetAura(100219);

        if (GetCaster()->HasAura(100220))
            return GetCaster()->GetAura(100220);

        if (GetCaster()->HasAura(100221))
            return GetCaster()->GetAura(100221);

        if (GetCaster()->HasAura(100222))
            return GetCaster()->GetAura(100222);

        if (GetCaster()->HasAura(100223))
            return GetCaster()->GetAura(100223);

        return nullptr;
    }

    int GetDamagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).Amplitude;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        if (!GetCaster())
            return;

        if (!GetRuneAura())
            return;

        int32 maxTicks = sSpellMgr->AssertSpellInfo(GetProcSpell())->GetMaxTicks();

        uint32 amountHealOrDamage = eventInfo.GetDamageInfo() ? eventInfo.GetDamageInfo()->GetDamage() : eventInfo.GetHealInfo()->GetHeal();
        uint32 amount = CalculatePct(amountHealOrDamage, GetDamagePct()) / maxTicks;

        if (amount < 0)
            return;

        if (AuraEffect* protEff = GetCaster()->GetAuraEffect(GetProcSpell(), 0))
        {
            int32 remainingTicks = maxTicks - protEff->GetTickNumber();
            int32 remainingAmount = protEff->GetAmount() * remainingTicks;
            int32 remainingAmountPerTick = remainingAmount / maxTicks;
            amount += remainingAmountPerTick;
        }

        GetCaster()->CastDelayedSpellWithPeriodicAmount(GetCaster(), GetProcSpell(), SPELL_AURA_PERIODIC_DAMAGE, amount, TRIGGERED_IGNORE_AURA_SCALING);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_shadow_pact::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rampart : public AuraScript
{
    PrepareAuraScript(spell_rampart);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100232))
            return GetCaster()->GetAura(100232);

        if (GetCaster()->HasAura(100233))
            return GetCaster()->GetAura(100233);

        if (GetCaster()->HasAura(100234))
            return GetCaster()->GetAura(100234);

        if (GetCaster()->HasAura(100235))
            return GetCaster()->GetAura(100235);

        if (GetCaster()->HasAura(100236))
            return GetCaster()->GetAura(100236);

        if (GetCaster()->HasAura(100237))
            return GetCaster()->GetAura(100237);

        return nullptr;
    }

    int GetDamageCheckSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    int GetProcAmount()
    {
        return GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        if (!GetCaster()->HasAura(GetDamageCheckSpell()) && !GetCaster()->HasAura(GetProcSpell()))
            GetCaster()->CastCustomSpell(GetProcSpell(), SPELLVALUE_BASE_POINT0, GetProcAmount(), GetCaster(), true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_rampart::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_rampart_remove : public AuraScript
{
    PrepareAuraScript(spell_rampart_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster()->HasAura(100238))
            GetCaster()->AddAura(100238, GetCaster());
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_rampart_remove::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_last_defender : public SpellScript
{
    PrepareSpellScript(spell_last_defender);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100240))
            return GetCaster()->GetAura(100240);

        if (GetCaster()->HasAura(100241))
            return GetCaster()->GetAura(100241);

        if (GetCaster()->HasAura(100242))
            return GetCaster()->GetAura(100242);

        if (GetCaster()->HasAura(100243))
            return GetCaster()->GetAura(100243);

        if (GetCaster()->HasAura(100244))
            return GetCaster()->GetAura(100244);

        if (GetCaster()->HasAura(100245))
            return GetCaster()->GetAura(100245);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    void FindTargets(std::list<WorldObject*>& targets)
    {
        if (!GetCaster())
            return;

        if (!GetRuneAura())
            return;

        if (targets.size() > 0)
        {
            if (!GetCaster()->HasAura(GetProcSpell()))
                GetCaster()->AddAura(GetProcSpell(), GetCaster());

            Aura* aura = GetCaster()->GetAura(GetProcSpell());
            uint32 amount = (std::max<int32>(targets.size(), 10));

            if (aura->GetStackAmount() != amount) 
                aura->SetStackAmount(amount);
        }
        else if (GetCaster()->HasAura(GetProcSpell()))
            GetCaster()->RemoveAura(GetProcSpell());
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_last_defender::FindTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class spell_juggling_balance : public AuraScript
{
    PrepareAuraScript(spell_juggling_balance);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100253))
            return GetCaster()->GetAura(100253);

        if (GetCaster()->HasAura(100254))
            return GetCaster()->GetAura(100254);

        if (GetCaster()->HasAura(100255))
            return GetCaster()->GetAura(100255);

        if (GetCaster()->HasAura(100256))
            return GetCaster()->GetAura(100256);

        if (GetCaster()->HasAura(100257))
            return GetCaster()->GetAura(100257);

        if (GetCaster()->HasAura(100258))
            return GetCaster()->GetAura(100258);

        return nullptr;
    }

    int GetHasteProc()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    int GetCritProc()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* player = GetCaster();

        if (!player || !GetRuneAura())
            return;

        float playerPowerPct = player->GetPowerPct(player->getPowerType());     

        if (playerPowerPct >= 50)
        {
            if (player->HasAura(GetCritProc()))
                player->RemoveAura(GetCritProc());

            if (!player->HasAura(GetHasteProc()))
                player->AddAura(GetHasteProc(), player);
        }

        if (playerPowerPct < 50)
        {
            if (player->HasAura(GetHasteProc()))
                player->RemoveAura(GetHasteProc());

            if (!player->HasAura(GetCritProc()))
                player->AddAura(GetCritProc(), player);
        }

    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_juggling_balance::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_mana_filled_wounds : public AuraScript
{
    PrepareAuraScript(spell_mana_filled_wounds);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(100271))
            return GetCaster()->GetAura(100271);

        if (GetCaster()->HasAura(100272))
            return GetCaster()->GetAura(100272);

        if (GetCaster()->HasAura(100273))
            return GetCaster()->GetAura(100273);

        if (GetCaster()->HasAura(100274))
            return GetCaster()->GetAura(100274);

        if (GetCaster()->HasAura(100275))
            return GetCaster()->GetAura(100275);

        if (GetCaster()->HasAura(100276))
            return GetCaster()->GetAura(100276);

        return nullptr;
    }

    int GetProcPct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).Amplitude;
    }

    int GetCheckSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(GetCheckSpell()))
        {
            int32 amount = int32(CalculatePct(GetCaster()->GetCreateMana(), GetProcPct()));
            GetCaster()->CastCustomSpell(GetProcSpell(), SPELLVALUE_BASE_POINT0, amount, GetCaster(), true, nullptr);
            GetCaster()->AddAura(GetCheckSpell(), GetCaster());
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mana_filled_wounds::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
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
    RegisterSpellScript(spell_pyretic_incantation);
    RegisterSpellScript(spell_gutripper);
    RegisterSpellScript(spell_dying_breath);
    RegisterSpellScript(spell_ysera_s_gift);
    RegisterSpellScript(spell_ysera_s_gift_target);
    RegisterSpellScript(spell_echo_of_light);
    RegisterSpellScript(spell_mittelschmerz_blood_magic);
    RegisterSpellScript(spell_azerite_veins);
    RegisterSpellScript(spell_killing_spree);
    RegisterSpellScript(spell_holy_aegis);
    RegisterSpellScript(spell_shadow_pact);
    RegisterSpellScript(spell_rampart);
    RegisterSpellScript(spell_rampart_remove);
    RegisterSpellScript(spell_last_defender);
    RegisterSpellScript(spell_juggling_balance);
    RegisterSpellScript(spell_mana_filled_wounds);
}
