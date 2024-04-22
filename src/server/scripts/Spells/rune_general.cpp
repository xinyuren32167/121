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

enum GeneralRuneSpells
{
    RUNE_GENERAL_DYING_BREATH_HEAL  = 100104,
    RUNE_GENERAL_DYING_BREATH_COOLDOWN  = 100105,
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
        if (GetCaster()->GetHealth() < GetCaster()->GetMaxHealth() && !GetCaster()->HasAura(GetDamageCheckSpell()) && GetCaster()->IsAlive())
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
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0) {
            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetProcPct()));
            GetCaster()->CastCustomSpell(100040, SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
        }
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

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetCaster())
            return;

        if (eventInfo.GetProcTarget() &&
            eventInfo.GetDamageInfo() &&
            eventInfo.GetDamageInfo()->GetDamage() > 0) {
            uint32 random = urand(1, 100);

            if (random <= GetProcChance(eventInfo))
            {
                int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
                GetCaster()->CastCustomSpellTrigger(100097, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), TRIGGERED_IGNORE_AURA_SCALING);
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

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* victim = GetTarget();

        if (!victim || victim->isDead())
            return;

        if (Aura* cooldown = victim->GetAura(RUNE_GENERAL_DYING_BREATH_COOLDOWN))
            return;

        int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
        uint32 allowedHealth = victim->CountPctFromMaxHealth(20);

        int32 healAmount = victim->CountPctFromMaxHealth(aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount());

        if ((remainingHealth < int32(allowedHealth)))   
        {
            victim->CastCustomSpell(RUNE_GENERAL_DYING_BREATH_HEAL, SPELLVALUE_BASE_POINT0, healAmount, victim, TRIGGERED_FULL_MASK);
            victim->CastSpell(victim, RUNE_GENERAL_DYING_BREATH_COOLDOWN);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dying_breath::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dying_breath::Absorb, EFFECT_0);
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

        if (!eventInfo.GetActor() || eventInfo.GetActor()->isDead())
            return;

        if (!eventInfo.GetProcTarget() || eventInfo.GetProcTarget()->isDead())
            return;

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

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetSpellInfo() && (eventInfo.GetSpellInfo()->Id == 47818 || eventInfo.GetSpellInfo()->Id == 42844
            || eventInfo.GetSpellInfo()->Id == 80001 || eventInfo.GetSpellInfo()->Id == 81522 || eventInfo.GetSpellInfo()->Id == 80012
            || eventInfo.GetSpellInfo()->Id == 42938))
            return false;

        return (GetCaster() && GetCaster()->IsAlive());
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 damage = GetCaster()->CountPctFromCurHealth(aurEff->GetAmount());
        GetCaster()->DealDamage(GetCaster(), GetCaster(), damage);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mittelschmerz_blood_magic::CheckProc);
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

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetHealInfo())
            return false;

        if (!eventInfo.GetHealInfo()->GetHeal() < 0)
            return false;

        return true;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_holy_aegis::CheckProc);
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

class spell_last_defender : public AuraScript
{
    PrepareAuraScript(spell_last_defender);

    uint32 findSizeThreath()
    {
        std::list<Unit*> targetList;

        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return 0;

        auto const& threatList = caster->getAttackers();
        auto threatListCopy = threatList;

        if (threatListCopy.empty())
            return 0;

        for (auto itr = threatListCopy.begin(); itr != threatListCopy.end(); ++itr)
        {
            Unit* treath = (*itr);

            float distance = caster->GetDistance(treath->GetPosition());
            if (distance <= 8.0f && treath->IsAlive()) {
                targetList.push_back(treath);
            }
        }

        return targetList.size();
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        uint32 spellId = aurEff->GetAmount();
        Unit* unit = GetCaster();

        if (!unit || unit->isDead())
            return;

        uint32 size = std::min<uint32>(findSizeThreath(), 10);

        if (size == 0) {
            unit->RemoveAura(spellId);
            return;
        }

        if (Aura* auraEff = unit->GetAura(spellId)) {
            uint32 currentStack = auraEff->GetStackAmount();
            if (currentStack != size)
                auraEff->SetStackAmount(size);
        }
        else {
            Aura* aura = unit->AddAura(spellId, unit);
            aura->SetStackAmount(size);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_last_defender::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};


class spell_juggling_balance : public AuraScript
{
    PrepareAuraScript(spell_juggling_balance);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* player = GetCaster();

        if (!player || player->isDead())
            return;

        float playerPowerPct = player->GetPowerPct(player->getPowerType());
        uint32 hasteAura = aurEff->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
        uint32 critAura = aurEff->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;

        if (playerPowerPct >= 50)
        {
            if (player->HasAura(critAura))
                player->RemoveAura(critAura);

            if (!player->HasAura(hasteAura))
                player->AddAura(hasteAura, player);
        }

        if (playerPowerPct < 50)
        {
            if (player->HasAura(hasteAura))
                player->RemoveAura(hasteAura);

            if (!player->HasAura(critAura))
                player->AddAura(critAura, player);
        }

    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        uint32 hasteAura = aurEff->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
        uint32 critAura = aurEff->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;

        if (GetCaster()->HasAura(hasteAura))
            GetCaster()->RemoveAura(hasteAura);

        if (GetCaster()->HasAura(critAura))
            GetCaster()->RemoveAura(critAura);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_juggling_balance::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_juggling_balance::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
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

class rune_general_multi_element_boost : public AuraScript
{
    PrepareAuraScript(rune_general_multi_element_boost);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_general_multi_element_boost::CheckProc);
    }
};

class rune_general_school_vampirism : public AuraScript
{
    PrepareAuraScript(rune_general_school_vampirism);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return false;

        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 healPct = aurEff->GetAmount();
        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, healPct);
        int32 value = std::max(1, amount);

        if (amount <= 0)
            return;

        int32 procSpell = GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        caster->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, value, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_general_school_vampirism::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_general_school_vampirism::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

void AddSC_generals_perks_scripts()
{
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
    RegisterSpellScript(rune_general_multi_element_boost);
    RegisterSpellScript(rune_general_school_vampirism);
}
