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

class spell_pyretic_incantation : public AuraScript
{
    PrepareAuraScript(spell_pyretic_incantation);

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints;
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

    Aura* GetPerkAura()
    {
        if (GetCaster()->HasAura(100091))
            return GetCaster()->GetAura(100091);

        if (GetCaster()->HasAura(100092))
            return GetCaster()->GetAura(100092);

        if (GetCaster()->HasAura(100093))
            return GetCaster()->GetAura(100093);

        if (GetCaster()->HasAura(100094))
            return GetCaster()->GetAura(100094);

        if (GetCaster()->HasAura(100095))
            return GetCaster()->GetAura(100095);

        if (GetCaster()->HasAura(100096))
            return GetCaster()->GetAura(100096);

        return nullptr;
    }

    int GetDamagePct()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    int GetProcChance(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetProcTarget()->GetHealthPct() < 35)
            return 60;

        return 20;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        uint32 random = urand(1, 100);

        if (random <= GetProcChance(eventInfo))
        {
            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetDamagePct()));

            GetCaster()->CastCustomSpell(GetProcSpell(), SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true);
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

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
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

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetSelfHealSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    int GetAllyHealSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;
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

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
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

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).Amplitude;
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

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
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

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).Amplitude;
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

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).Amplitude;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        int32 maxTicks = sSpellMgr->AssertSpellInfo(GetProcSpell())->GetMaxTicks();
        uint32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetDamagePct()) / maxTicks;

        if (AuraEffect* protEff = GetCaster()->GetAuraEffect(GetProcSpell(), 0))
        {
            int32 remainingTicks = maxTicks - protEff->GetTickNumber();
            int32 remainingAmount = protEff->GetAmount() * remainingTicks;
            int32 remainingAmountPerTick = remainingAmount / maxTicks;

            amount += remainingAmountPerTick;
        }

         GetCaster()->CastDelayedSpellWithPeriodicAmount(GetCaster(), GetProcSpell(), SPELL_AURA_PERIODIC_DAMAGE, amount,  TRIGGERED_IGNORE_AURA_SCALING);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_shadow_pact::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_ysera_s_gift : public AuraScript
{
    PrepareAuraScript(spell_ysera_s_gift);

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetSelfHealSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    int GetAllyHealSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;
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
}
