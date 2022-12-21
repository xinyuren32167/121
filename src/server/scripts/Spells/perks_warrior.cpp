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


class spell_cut_the_veins : public AuraScript
{
    PrepareAuraScript(spell_cut_the_veins);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200038))
            return GetCaster()->GetAura(200038);

        if (GetCaster()->HasAura(200039))
            return GetCaster()->GetAura(200039);

        if (GetCaster()->HasAura(200040))
            return GetCaster()->GetAura(200040);

        if (GetCaster()->HasAura(200041))
            return GetCaster()->GetAura(200041);

        if (GetCaster()->HasAura(200042))
            return GetCaster()->GetAura(200042);

        if (GetCaster()->HasAura(200043))
            return GetCaster()->GetAura(200043);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints;
    }

    int GetDamagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (GetRuneAura())
        {
            int32 totalTicks = sSpellMgr->AssertSpellInfo(GetProcSpell())->GetMaxTicks();
            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetDamagePct()) / totalTicks);
            int32 maxAmount = int32(CalculatePct(GetCaster()->GetMaxHealth(), 50));

            if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(GetProcSpell(), 0))
            {
                int32 remainingTicks = totalTicks - protEff->GetTickNumber();
                int32 remainingAmount = protEff->GetAmount() * remainingTicks;
                int32 remainingAmountPerTick = remainingAmount / totalTicks;

                amount = (std::min<int32>(amount + remainingAmountPerTick, maxAmount));
            }
            eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), GetProcSpell(), SPELL_AURA_PERIODIC_DAMAGE, amount, TRIGGERED_IGNORE_AURA_SCALING);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_cut_the_veins::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_the_art_of_war : public AuraScript
{
    PrepareAuraScript(spell_the_art_of_war);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200087))
            return GetCaster()->GetAura(200087);

        if (GetCaster()->HasAura(200088))
            return GetCaster()->GetAura(200088);

        if (GetCaster()->HasAura(200089))
            return GetCaster()->GetAura(200089);

        if (GetCaster()->HasAura(200090))
            return GetCaster()->GetAura(200090);

        if (GetCaster()->HasAura(200091))
            return GetCaster()->GetAura(200091);

        if (GetCaster()->HasAura(200092))
            return GetCaster()->GetAura(200092);

        return nullptr;
    }

    int GetRagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        Aura* aura = GetRuneAura();

        if (!aura)
            return;

        if (GetRuneAura())
        {
            int32 spellRage = eventInfo.GetSpellInfo()->ManaCost / 10;
            int32 rageAccumulated = GetRuneAura()->GetEffect(EFFECT_1)->GetAmount() + spellRage;

            if (spellRage <= 0)
                return;

            if (rageAccumulated >= GetRagePct())
            {
                GetCaster()->CastSpell(GetCaster(), 200093, TRIGGERED_FULL_MASK);
                GetRuneAura()->GetEffect(EFFECT_1)->SetAmount(GetRagePct() - rageAccumulated);
            }
            else
            {
                GetRuneAura()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_the_art_of_war::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_tide_of_blood : public AuraScript
{
    PrepareAuraScript(spell_tide_of_blood);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200100))
            return GetCaster()->GetAura(200100);

        if (GetCaster()->HasAura(200101))
            return GetCaster()->GetAura(200101);

        if (GetCaster()->HasAura(200102))
            return GetCaster()->GetAura(200102);

        if (GetCaster()->HasAura(200103))
            return GetCaster()->GetAura(200103);

        if (GetCaster()->HasAura(200104))
            return GetCaster()->GetAura(200104);

        if (GetCaster()->HasAura(200105))
            return GetCaster()->GetAura(200105);

        return nullptr;
    }


    void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
    {

        Aura* aura = GetRuneAura();
        Unit* caster = GetCaster();
        Unit* target = GetTarget();

        if (!target)
            return;

        if (!aura)
            return;

        Aura* auraStack = caster->GetAura(200107);

        if (!auraStack)
            return;

        uint32 stacksAmount = auraStack->GetStackAmount();
        uint32 requireStacks = aura->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;

        if (stacksAmount < requireStacks)
            return;

        int32 damage = aura->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints + 1;
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, 0);
        damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

        caster->CastCustomSpell(200106, SPELLVALUE_BASE_POINT0, damage, caster, TRIGGERED_FULL_MASK);
        caster->RemoveAura(200107);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_tide_of_blood::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_vein_cutter : public AuraScript
{
    PrepareAuraScript(spell_vein_cutter);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200108))
            return GetCaster()->GetAura(200108);

        if (GetCaster()->HasAura(200109))
            return GetCaster()->GetAura(200109);

        if (GetCaster()->HasAura(200110))
            return GetCaster()->GetAura(200110);

        if (GetCaster()->HasAura(200111))
            return GetCaster()->GetAura(200111);

        if (GetCaster()->HasAura(200112))
            return GetCaster()->GetAura(200112);

        if (GetCaster()->HasAura(200113))
            return GetCaster()->GetAura(200113);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints;
    }

    int GetDamagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (GetRuneAura())
        {
            int32 totalTicks = sSpellMgr->AssertSpellInfo(GetProcSpell())->GetMaxTicks();
            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetDamagePct()) / totalTicks);
            int32 maxAmount = int32(CalculatePct(GetCaster()->GetMaxHealth(), 50));

            if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(GetProcSpell(), 0))
            {
                int32 remainingTicks = totalTicks - protEff->GetTickNumber();
                int32 remainingAmount = protEff->GetAmount() * remainingTicks;
                int32 remainingAmountPerTick = remainingAmount / totalTicks;

                amount = (std::min<int32>(amount + remainingAmountPerTick, maxAmount));
            }
            eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), GetProcSpell(), SPELL_AURA_PERIODIC_DAMAGE, amount, TRIGGERED_IGNORE_AURA_SCALING);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_vein_cutter::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_battlelord : public AuraScript
{
    PrepareAuraScript(spell_battlelord);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* caster = GetTarget()->ToPlayer())
        {
            caster->RemoveSpellCooldown(47486, true);
            caster->RemoveSpellCooldown(47520, true);
            /*caster->ModifySpellCooldown(47486, -aurEff->GetAmount());
            caster->ModifySpellCooldown(47520, -aurEff->GetAmount());*/
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_battlelord::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_tactician : public AuraScript
{
    PrepareAuraScript(spell_tactician);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 spellRage = eventInfo.GetSpellInfo()->ManaCost / 10;
        float procPctPerRagePoint = aurEff->GetSpellInfo()->Effects[EFFECT_0].DamageMultiplier;

        if (spellRage <= 0)
            return;

        float procChance = spellRage * procPctPerRagePoint;
        uint32 random = urand(1, 100);

        if (random <= procChance)
            GetCaster()->ToPlayer()->ModifySpellCooldown(7384, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_tactician::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dreadnaught : public SpellScript
{
    PrepareSpellScript(spell_dreadnaught);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dreadnaught::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_blood_and_thunder : public SpellScript
{
    PrepareSpellScript(spell_blood_and_thunder);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200193))
            return GetCaster()->GetAura(200193);

        if (GetCaster()->HasAura(200194))
            return GetCaster()->GetAura(200194);

        if (GetCaster()->HasAura(200195))
            return GetCaster()->GetAura(200195);

        if (GetCaster()->HasAura(200196))
            return GetCaster()->GetAura(200196);

        if (GetCaster()->HasAura(200197))
            return GetCaster()->GetAura(200197);

        if (GetCaster()->HasAura(200198))
            return GetCaster()->GetAura(200198);

        return nullptr;
    }

    void FindTargets(std::list<WorldObject*>& targets)
    {
        if (!GetRuneAura())
            return;

        if (targets.size() > 0)
        {
            for (auto const& target : targets)
                if (Unit* unit = target->ToUnit())
                    GetCaster()->CastSpell(unit, 200237, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_blood_and_thunder::FindTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_anger_management : public AuraScript
{
    PrepareAuraScript(spell_anger_management);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 spellRage = eventInfo.GetSpellInfo()->ManaCost / 10;
        int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount() + spellRage;
        int32 rageThreshold = aurEff->GetAmount();

        if (spellRage <= 0)
            return;

        LOG_ERROR("error", "{} , {} , {}", spellRage, rageAccumulated, rageThreshold);

        if (rageAccumulated >= rageThreshold)
        {
            GetCaster()->ToPlayer()->ModifySpellCooldown(46924, -aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount());
            aurEff->GetBase()->GetEffect(EFFECT_2)->SetAmount(rageAccumulated - rageThreshold);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_2)->SetAmount(rageAccumulated);
        LOG_ERROR("error", "{}", aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_anger_management::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_blood_dance : public AuraScript
{
    PrepareAuraScript(spell_blood_dance);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetProcTarget();

        if (!target)
            return;

        if (!target->HasAura(47465))
            return;

        AuraEffect* rendEff = target->GetAuraEffect(47465, 0);
        int32 tickDamage = rendEff->GetAmount();
        int32 remainingTicks = rendEff->GetSpellInfo()->GetMaxTicks() - rendEff->GetTickNumber();
        int32 remainingDamage = tickDamage * remainingTicks;

        int32 amount = CalculatePct(remainingDamage, aurEff->GetAmount());

        GetCaster()->CastCustomSpell(200217, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_blood_dance::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_keep_spinnin : public AuraScript
{
    PrepareAuraScript(spell_keep_spinnin);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->ModifySpellCooldown(46924, -aurEff->GetBase()->GetEffect(EFFECT_0)->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_keep_spinnin::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sweeping_rage : public AuraScript
{
    PrepareAuraScript(spell_sweeping_rage);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(12328))
            return;

        int32 spellRage = eventInfo.GetSpellInfo()->ManaCost / 10;

        if (spellRage <= 0)
            return;

        int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
        int32 rageThreshold = aurEff->GetAmount();
        LOG_ERROR("error", "{} , {} , {}", spellRage, rageAccumulated, rageThreshold);
        if (rageAccumulated >= rageThreshold)
        {
            GetCaster()->CastSpell(GetCaster(), 200244, TRIGGERED_FULL_MASK);
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
        LOG_ERROR("error", "{}", aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sweeping_rage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sweeping_rage_proc : public AuraScript
{
    PrepareAuraScript(spell_sweeping_rage_proc);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->HasAura(200244))
            return;

        int32 stackAmount = GetCaster()->GetAura(200244)->GetStackAmount();

        GetCaster()->RemoveAura(200244);
        GetCaster()->CastSpell(GetCaster(), 200245, TRIGGERED_FULL_MASK);
        GetCaster()->GetAura(200245)->SetStackAmount(stackAmount);
        LOG_ERROR("error", "{}", GetCaster()->GetAura(200245)->GetStackAmount());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_sweeping_rage_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_collateral_damage : public AuraScript
{
    PrepareAuraScript(spell_collateral_damage);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(12328);
    }
   
    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_collateral_damage::CheckProc);
    }
};

class spell_collateral_damage_proc : public AuraScript
{
    PrepareAuraScript(spell_collateral_damage_proc);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200246))
            return GetCaster()->GetAura(200246);

        if (GetCaster()->HasAura(200247))
            return GetCaster()->GetAura(200247);

        if (GetCaster()->HasAura(200248))
            return GetCaster()->GetAura(200248);

        if (GetCaster()->HasAura(200249))
            return GetCaster()->GetAura(200249);

        if (GetCaster()->HasAura(200250))
            return GetCaster()->GetAura(200250);

        if (GetCaster()->HasAura(200251))
            return GetCaster()->GetAura(200251);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->HasAura(200252))
            return;

        int32 stackAmount = GetCaster()->GetAura(200252)->GetStackAmount();
        int32 damageAmount = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();
        LOG_ERROR("error", "{} , {}", stackAmount, damageAmount);
        GetCaster()->RemoveAura(200252);
        GetCaster()->CastCustomSpell(200253, SPELLVALUE_BASE_POINT0, damageAmount, GetCaster(), TRIGGERED_FULL_MASK);
        GetCaster()->GetAura(200253)->SetStackAmount(stackAmount);
        LOG_ERROR("error", "{}", GetCaster()->GetAura(200253)->GetStackAmount());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_collateral_damage_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_vicious_comtempt : public AuraScript
{
    PrepareAuraScript(spell_vicious_comtempt);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(23881, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_vicious_comtempt::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_charging_for_blood : public AuraScript
{
    PrepareAuraScript(spell_charging_for_blood);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(23881, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_charging_for_blood::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

void AddSC_warrior_perks_scripts()
{
    RegisterSpellScript(spell_cut_the_veins);
    RegisterSpellScript(spell_the_art_of_war);
    RegisterSpellScript(spell_tide_of_blood);
    RegisterSpellScript(spell_vein_cutter);
    RegisterSpellScript(spell_battlelord);
    RegisterSpellScript(spell_tactician);
    RegisterSpellScript(spell_dreadnaught);
    RegisterSpellScript(spell_blood_and_thunder);
    RegisterSpellScript(spell_anger_management);
    RegisterSpellScript(spell_blood_dance);
    RegisterSpellScript(spell_keep_spinnin);
    RegisterSpellScript(spell_sweeping_rage);
    RegisterSpellScript(spell_sweeping_rage_proc);
    RegisterSpellScript(spell_collateral_damage);
    RegisterSpellScript(spell_collateral_damage_proc);
    RegisterSpellScript(spell_vicious_comtempt);
    RegisterSpellScript(spell_charging_for_blood);
}
