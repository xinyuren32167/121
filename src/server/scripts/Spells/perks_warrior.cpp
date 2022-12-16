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

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints;
    }

    int GetDamagePct()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (GetPerkAura())
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

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        Aura* aura = GetPerkAura();

        if (!aura)
            return;

        if (GetPerkAura())
        {
            int32 spellRage = eventInfo.GetSpellInfo()->ManaCost / 10;
            int32 rageAccumulated = GetPerkAura()->GetEffect(EFFECT_1)->GetAmount() + spellRage;

            if (spellRage <= 0)
                return;

            if (rageAccumulated >= GetRagePct())
            {
                GetCaster()->CastSpell(GetCaster(), 200093, TRIGGERED_FULL_MASK);
                GetPerkAura()->GetEffect(EFFECT_1)->SetAmount(GetRagePct() - rageAccumulated);
            }
            else
            {
                GetPerkAura()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
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

    Aura* GetPerkAura()
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


    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {

        Aura* aura = GetPerkAura();
        Unit* unit = GetCaster();
        Unit* target = GetTarget();

        if (!target)
            return;

        if (!aura)
            return;

       uint32 stacksAmount = unit->GetAura(200107)->GetStackAmount();
       uint32 requireStacks = aura->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;

       if (stacksAmount < requireStacks)
           return;

        int32 damage = aura->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints + 1;
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, 0);
        damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

        GetCaster()->CastCustomSpell(200106, SPELLVALUE_BASE_POINT0, damage, GetCaster(), TRIGGERED_FULL_MASK);
        GetCaster()->RemoveAura(200107);
     }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_tide_of_blood::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_vein_cutter : public AuraScript
{
    PrepareAuraScript(spell_vein_cutter);

    Aura* GetPerkAura()
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
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints;
    }

    int GetDamagePct()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (GetPerkAura())
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

void AddSC_warrior_perks_scripts()
{
    RegisterSpellScript(spell_cut_the_veins);
    RegisterSpellScript(spell_the_art_of_war);
    RegisterSpellScript(spell_tide_of_blood);
    RegisterSpellScript(spell_vein_cutter);
}
