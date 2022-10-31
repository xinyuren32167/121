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


class spell_tempest_barrier : public SpellScript
{
    PrepareSpellScript(spell_tempest_barrier);

    int ShieldPct()
    {
        auto playerAuras = GetCaster()->GetAppliedAuras();
        int shieldPct = 0;

        for (auto itr = playerAuras.begin(); itr != playerAuras.end(); ++itr)
        {
            if (Aura* aura = itr->second->GetBase())
            {
                SpellInfo const* auraInfo = aura->GetSpellInfo();

                if (auraInfo->SpellFamilyFlags[2] & 0x00002000)
                {
                    shieldPct += aura->GetEffect(EFFECT_0)->GetAmount();
                }
            }
        }

        return shieldPct;
    }

    void HandleProc()
    {

        int playerMaxHealth = GetCaster()->GetMaxHealth();

        int32 absorb = int32(CalculatePct(playerMaxHealth, ShieldPct()));

        GetCaster()->CastCustomSpell(300135, SPELLVALUE_BASE_POINT0, absorb, GetCaster(), true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_tempest_barrier::HandleProc);
    }
};

class spell_cryo_freeze : public AuraScript
{
    PrepareAuraScript(spell_cryo_freeze);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        PreventDefaultAction();

        if (GetCaster()->HasAura(300019))
        {
            GetCaster()->CastSpell(GetCaster(), 300013, TRIGGERED_IGNORE_CASTER_AURAS);
        }
        if (GetCaster()->HasAura(300020))
        {
            GetCaster()->CastSpell(GetCaster(), 300014, TRIGGERED_IGNORE_CASTER_AURAS);
        }
        if (GetCaster()->HasAura(300021))
        {
            GetCaster()->CastSpell(GetCaster(), 300015, TRIGGERED_IGNORE_CASTER_AURAS);
        }
        if (GetCaster()->HasAura(300022))
        {
            GetCaster()->CastSpell(GetCaster(), 300016, TRIGGERED_IGNORE_CASTER_AURAS);
        }
        if (GetCaster()->HasAura(300023))
        {
            GetCaster()->CastSpell(GetCaster(), 300017, TRIGGERED_IGNORE_CASTER_AURAS);
        }
        if (GetCaster()->HasAura(300024))
        {
            GetCaster()->CastSpell(GetCaster(), 300018, TRIGGERED_IGNORE_CASTER_AURAS);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_cryo_freeze::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class spell_diverted_energy : public AuraScript
{
    PrepareAuraScript(spell_diverted_energy);

    uint32 absorbPct;

    int HealPct()
    {
        auto playerAuras = GetCaster()->GetAppliedAuras();
        int healPct = 0;

        for (auto itr = playerAuras.begin(); itr != playerAuras.end(); ++itr)
        {
            if (Aura* aura = itr->second->GetBase())
            {
                SpellInfo const* auraInfo = aura->GetSpellInfo();

                if (auraInfo->SpellFamilyFlags[2] & 0x00000080)
                {
                    healPct = aura->GetEffect(EFFECT_0)->GetAmount();
                    break;
                }
            }
        }

        return healPct;
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        int32 heal = int32(CalculatePct(absorbAmount, HealPct()));

        GetCaster()->CastCustomSpell(300142, SPELLVALUE_BASE_POINT0, heal, GetCaster(), true);
    }

    void Register() override
    {
        AfterEffectAbsorb += AuraEffectAbsorbFn(spell_diverted_energy::Absorb, EFFECT_0);
        OnEffectManaShield += AuraEffectManaShieldFn(spell_diverted_energy::Absorb, EFFECT_0);
    }
};

class spell_spiritual_armor : public SpellScript
{
    PrepareSpellScript(spell_spiritual_armor);

    void HandleProc()
    {
        if (GetCaster()->HasAura(300037))
            GetCaster()->CastSpell(GetCaster(), 300031, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300038))
            GetCaster()->CastSpell(GetCaster(), 300032, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300039))
            GetCaster()->CastSpell(GetCaster(), 300033, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300040))
            GetCaster()->CastSpell(GetCaster(), 300034, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300041))
            GetCaster()->CastSpell(GetCaster(), 300035, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300042))
            GetCaster()->CastSpell(GetCaster(), 300036, TRIGGERED_IGNORE_CASTER_AURAS);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_spiritual_armor::HandleProc);
    }
};

class spell_spiritual_armor_remove : public AuraScript
{

    PrepareAuraScript(spell_spiritual_armor_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster()->HasAura(300031))
            GetCaster()->RemoveAura(300031);

        if (GetCaster()->HasAura(300032))
            GetCaster()->RemoveAura(300032);

        if (GetCaster()->HasAura(300033))
            GetCaster()->RemoveAura(300033);

        if (GetCaster()->HasAura(300034))
            GetCaster()->RemoveAura(300034);

        if (GetCaster()->HasAura(300035))
            GetCaster()->RemoveAura(300035);

        if (GetCaster()->HasAura(300036))
            GetCaster()->RemoveAura(300036);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_spiritual_armor_remove::OnRemove, EFFECT_0, SPELL_AURA_MOD_RESISTANCE, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_triune_armor : public SpellScript
{
    PrepareSpellScript(spell_triune_armor);

    bool HasPerkTriuneArmor()
    {
        return (GetCaster()->HasAura(300143) ||
            GetCaster()->HasAura(300144) ||
            GetCaster()->HasAura(300145) ||
            GetCaster()->HasAura(300146) ||
            GetCaster()->HasAura(300147) ||
            GetCaster()->HasAura(300148));
    }

    void HandleProc()
    {
        if (HasPerkTriuneArmor())
        {
            GetCaster()->RemoveAura(43024);
            GetCaster()->RemoveAura(43046);
            GetCaster()->RemoveAura(43008);

            GetCaster()->CastSpell(GetCaster(), 300149, TRIGGERED_FULL_MASK);
            GetCaster()->CastSpell(GetCaster(), 300150, TRIGGERED_FULL_MASK);
            GetCaster()->CastSpell(GetCaster(), 300151, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_triune_armor::HandleProc);
    }
};

class spell_arcanic_gems : public SpellScript
{
    PrepareSpellScript(spell_arcanic_gems);

    void HandleProc()
    {
        if (GetCaster()->HasAura(300043))
            GetCaster()->CastSpell(GetCaster(), 300152, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300044))
            GetCaster()->CastSpell(GetCaster(), 300153, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300045))
            GetCaster()->CastSpell(GetCaster(), 300154, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300046))
            GetCaster()->CastSpell(GetCaster(), 300155, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300047))
            GetCaster()->CastSpell(GetCaster(), 300156, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300048))
            GetCaster()->CastSpell(GetCaster(), 300157, TRIGGERED_IGNORE_CASTER_AURAS);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_arcanic_gems::HandleProc);
    }
};

class spell_unstable_magic : public AuraScript
{
    PrepareAuraScript(spell_unstable_magic);

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), 50));

        GetCaster()->CastCustomSpell(300062, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_unstable_magic::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_icicle_ice_lance : public SpellScript
{
    PrepareSpellScript(spell_icicle_ice_lance);

    Aura* GetIcicleAura()
    {
        auto playerAuras = GetCaster()->GetAppliedAuras();


        for (auto itr = playerAuras.begin(); itr != playerAuras.end(); ++itr)
            if (Aura* aura = itr->second->GetBase())
            {
                SpellInfo const* auraInfo = aura->GetSpellInfo();
                if (auraInfo->SpellFamilyFlags[2] & 0x00000800)
                    return aura;
            }
        return nullptr;
    }

    int GetAuraTriggerSpell()
    {
        auto playerAuras = GetCaster()->GetAppliedAuras();


        for (auto itr = playerAuras.begin(); itr != playerAuras.end(); ++itr)
            if (Aura* aura = itr->second->GetBase())
            {
                SpellInfo const* auraInfo = aura->GetSpellInfo();
                if (auraInfo->SpellFamilyFlags[2] & 0x00000800)
                    return auraInfo->GetEffect(EFFECT_0).TriggerSpell;
            }
        return 0;
    }

    void HandleProc()
    {
        Aura* IcicleAura = GetIcicleAura();

        if (IcicleAura)
        {
            int triggerSpell = IcicleAura->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

            for (int i = 0; i < IcicleAura->GetStackAmount(); i++)
            {
                GetCaster()->CastSpell(GetExplTargetUnit(), triggerSpell);
            }
            GetCaster()->RemoveAura(IcicleAura);
        }
    }


    void Register() override
    {
        OnCast += SpellCastFn(spell_icicle_ice_lance::HandleProc);
    }
};

class spell_icicle_frostbolt : public SpellScript
{
    PrepareSpellScript(spell_icicle_frostbolt);

    int GetStackIcicle()
    {
        auto playerAuras = GetCaster()->GetAppliedAuras();


        for (auto itr = playerAuras.begin(); itr != playerAuras.end(); ++itr)
            if (Aura* aura = itr->second->GetBase())
            {
                SpellInfo const* auraInfo = aura->GetSpellInfo();
                if (auraInfo->SpellFamilyFlags[2] & 0x00000800)
                    return aura->GetStackAmount();
            }
        return 0;
    }

    int GetAuraTriggerSpell()
    {
        auto playerAuras = GetCaster()->GetAppliedAuras();


        for (auto itr = playerAuras.begin(); itr != playerAuras.end(); ++itr)
            if (Aura* aura = itr->second->GetBase())
            {
                SpellInfo const* auraInfo = aura->GetSpellInfo();
                if (auraInfo->SpellFamilyFlags[2] & 0x00000800)
                    return auraInfo->GetEffect(EFFECT_0).TriggerSpell;
            }
        return 0;
    }

    void HandleProc()
    {
        int triggerSpell = GetAuraTriggerSpell();
        int stackAmount = GetStackIcicle();

        if (triggerSpell > 0 && stackAmount == 5)
        {
            GetCaster()->CastSpell(GetExplTargetUnit(), triggerSpell);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_icicle_frostbolt::HandleProc);
    }
};

class spell_slick_ice : public AuraScript
{
    PrepareAuraScript(spell_slick_ice);

    bool HandleProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster()->HasAura(12472));
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_slick_ice::HandleProc);
    }
};

class spell_improved_brain_freeze : public AuraScript
{

    PrepareAuraScript(spell_improved_brain_freeze);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster()->HasAura(300164))
            GetCaster()->CastSpell(GetCaster(), 300170, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300165))
            GetCaster()->CastSpell(GetCaster(), 300171, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300166))
            GetCaster()->CastSpell(GetCaster(), 300172, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300167))
            GetCaster()->CastSpell(GetCaster(), 300173, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300168))
            GetCaster()->CastSpell(GetCaster(), 300174, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300169))
            GetCaster()->CastSpell(GetCaster(), 300175, TRIGGERED_IGNORE_CASTER_AURAS);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_improved_brain_freeze::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_improved_brain_freeze_remove : public SpellScript
{

    PrepareSpellScript(spell_improved_brain_freeze_remove);

    void HandleProc()
    {
        if (GetCaster()->HasAura(300170))
            GetCaster()->RemoveAura(300170);

        if (GetCaster()->HasAura(300171))
            GetCaster()->RemoveAura(300171);

        if (GetCaster()->HasAura(300172))
            GetCaster()->RemoveAura(300172);

        if (GetCaster()->HasAura(300173))
            GetCaster()->RemoveAura(300173);

        if (GetCaster()->HasAura(300174))
            GetCaster()->RemoveAura(300174);

        if (GetCaster()->HasAura(300175))
            GetCaster()->RemoveAura(300175);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_improved_brain_freeze_remove::HandleProc);
    }
};

class spell_lady_vashj_s_grasp_remove : public AuraScript
{
    PrepareAuraScript(spell_lady_vashj_s_grasp_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster()->HasAura(300212))
            GetCaster()->RemoveAura(300212);

        if (GetCaster()->HasAura(300213))
            GetCaster()->RemoveAura(300213);

        if (GetCaster()->HasAura(300214))
            GetCaster()->RemoveAura(300214);

        if (GetCaster()->HasAura(300215))
            GetCaster()->RemoveAura(300215);

        if (GetCaster()->HasAura(300216))
            GetCaster()->RemoveAura(300216);

        if (GetCaster()->HasAura(300217))
            GetCaster()->RemoveAura(300217);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_lady_vashj_s_grasp_remove::OnRemove, EFFECT_0, SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_forzen_veins : public AuraScript
{
    PrepareAuraScript(spell_forzen_veins);

    bool HandleProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster()->HasAura(12472));
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (GetCaster()->HasAura(12472))
        {
            if (AuraEffect* protEff = GetCaster()->GetAuraEffect(12472, EFFECT_0))
            {
                uint32 duration = protEff->GetBase()->GetDuration();
                if (duration < protEff->GetBase()->GetMaxDuration() + 10000)
                    protEff->GetBase()->SetDuration(protEff->GetBase()->GetDuration() + aurEff->GetAmount());
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_forzen_veins::HandleProc);
        OnEffectProc += AuraEffectProcFn(spell_forzen_veins::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_impulsive_veins : public AuraScript
{
    PrepareAuraScript(spell_impulsive_veins);

    bool HandleProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster()->HasAura(12472));
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (GetCaster()->HasAura(12472))
        {
            if (Player* target = GetTarget()->ToPlayer())
                target->ModifySpellCooldown(12472, -aurEff->GetAmount());
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_impulsive_veins::HandleProc);
        OnEffectProc += AuraEffectProcFn(spell_impulsive_veins::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_flurry : public SpellScript
{
    PrepareSpellScript(spell_flurry);

    void HandleProc()
    {
        if (GetCaster()->HasAura(300366))
            GetCaster()->CastSpell(GetCaster(), 300372, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300367))
            GetCaster()->CastSpell(GetCaster(), 300373, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300368))
            GetCaster()->CastSpell(GetCaster(), 300374, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300369))
            GetCaster()->CastSpell(GetCaster(), 300375, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300370))
            GetCaster()->CastSpell(GetCaster(), 300376, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300371))
            GetCaster()->CastSpell(GetCaster(), 300377, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_flurry::HandleProc);
    }
};

class spell_flurry_proc : public SpellScript
{
    PrepareSpellScript(spell_flurry_proc);

    bool HasPerkFlurryProc()
    {
        return (GetCaster()->HasAura(300372) ||
            GetCaster()->HasAura(300373) ||
            GetCaster()->HasAura(300374) ||
            GetCaster()->HasAura(300375) ||
            GetCaster()->HasAura(300376) ||
            GetCaster()->HasAura(300377));
    }

    void HandleProc()
    {
        if (HasPerkFlurryProc())
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
    }

    void HandleHitProc()
    {
        if (GetCaster()->HasAura(300372))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300372);
        }

        if (GetCaster()->HasAura(300373))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300373);
        }

        if (GetCaster()->HasAura(300374))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300374);
        }

        if (GetCaster()->HasAura(300375))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300375);
        }

        if (GetCaster()->HasAura(300376))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300376);
        }

        if (GetCaster()->HasAura(300377))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300377);
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_flurry_proc::HandleProc);
        AfterHit += SpellHitFn(spell_flurry_proc::HandleHitProc);
    }
};

class spell_brain_fire : public SpellScript
{
    PrepareSpellScript(spell_brain_fire);

    void HandleProc()
    {
        if (GetCaster()->HasAura(300378))
            GetCaster()->CastSpell(GetCaster(), 300384, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300379))
            GetCaster()->CastSpell(GetCaster(), 300385, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300380))
            GetCaster()->CastSpell(GetCaster(), 300386, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300381))
            GetCaster()->CastSpell(GetCaster(), 300387, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300382))
            GetCaster()->CastSpell(GetCaster(), 300388, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300383))
            GetCaster()->CastSpell(GetCaster(), 300389, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_brain_fire::HandleProc);
    }
};

class spell_ice_spike : public SpellScript
{

    PrepareSpellScript(spell_ice_spike);

    int GetProcRate()
    {
        if (GetCaster()->HasAura(300402))
            return 21;

        if (GetCaster()->HasAura(300403))
            return 37;

        if (GetCaster()->HasAura(300404))
            return 53;

        if (GetCaster()->HasAura(300405))
            return 69;

        if (GetCaster()->HasAura(300406))
            return 85;

        if (GetCaster()->HasAura(300407))
            return 101;

        return 0;
    }

    void HandleProc()
    {
        if (GetCaster()->HasAura(57761))
        {
            if (GetCaster()->HasAura(300402) || GetCaster()->HasAura(300403) || GetCaster()->HasAura(300404) || GetCaster()->HasAura(300405) || GetCaster()->HasAura(300406) || GetCaster()->HasAura(300407))
            {
                uint32 random = urand(0, 100);
                if (random < GetProcRate())
                    GetCaster()->CastSpell(GetExplTargetUnit(), 300408);
            }
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_ice_spike::HandleProc);
    }
};

class spell_powerful_water_elemental : public AuraScript
{
    PrepareAuraScript(spell_powerful_water_elemental);

    Aura* GetPerkAura(Unit* player)
    {
        if (player->HasAura(300415))
            return player->GetAura(300415);

        if (player->HasAura(300416))
            return player->GetAura(300416);

        if (player->HasAura(300417))
            return player->GetAura(300417);

        if (player->HasAura(300418))
            return player->GetAura(300418);

        if (player->HasAura(300419))
            return player->GetAura(300419);

        if (player->HasAura(300420))
            return player->GetAura(300420);

        return nullptr;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* player = GetCaster()->GetCharmerOrOwner();

        if (!player || !GetPerkAura(player))
            return;

        float crit = player->GetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + static_cast<uint8>(SPELL_SCHOOL_FROST));
        float haste = player->GetFloatValue(UNIT_MOD_CAST_SPEED);
        float spellPowerPct = GetPerkAura(player)->GetEffect(EFFECT_0)->GetAmount();
        float spellPowerRaw = player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST);
        float spellPowerFinal = CalculatePct(std::max<int32>(0, spellPowerRaw), spellPowerPct);

        GetCaster()->SetFloatValue(UNIT_MOD_CAST_SPEED, haste);
        GetCaster()->CastCustomSpell(300422, SPELLVALUE_BASE_POINT0, crit - 5, GetCaster(), true);
        GetCaster()->CastCustomSpell(51040, SPELLVALUE_BASE_POINT0, spellPowerFinal, GetCaster(), true);

    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_powerful_water_elemental::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class spell_cone_of_blizzard : public AuraScript
{
    PrepareAuraScript(spell_cone_of_blizzard);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->RemoveSpellCooldown(42931, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_cone_of_blizzard::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_arcane_prodigy : public AuraScript
{
    PrepareAuraScript(spell_arcane_prodigy);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(12042, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_arcane_prodigy::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_time_lord : public AuraScript
{
    PrepareAuraScript(spell_time_lord);

    bool HandleProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster()->HasAura(12042));
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_time_lord::HandleProc);
    }
};

class spell_arcano_shatter : public AuraScript
{
    PrepareAuraScript(spell_arcano_shatter);

    bool HasPerkArcanoShatter()
    {
        return (GetCaster()->HasAura(300486) ||
            GetCaster()->HasAura(300487) ||
            GetCaster()->HasAura(300488) ||
            GetCaster()->HasAura(300489) ||
            GetCaster()->HasAura(300490) ||
            GetCaster()->HasAura(300491));
    }

    Aura* GetPerkAura()
    {
        if (HasPerkArcanoShatter())
        {
            if (GetCaster()->HasAura(300486))
                return GetCaster()->GetAura(300486);

            if (GetCaster()->HasAura(300487))
                return GetCaster()->GetAura(300487);

            if (GetCaster()->HasAura(300488))
                return GetCaster()->GetAura(300488);

            if (GetCaster()->HasAura(300489))
                return GetCaster()->GetAura(300489);

            if (GetCaster()->HasAura(300490))
                return GetCaster()->GetAura(300490);

            if (GetCaster()->HasAura(300491))
                return GetCaster()->GetAura(300491);
        }
        else
            return nullptr;
    }

    int GetTriggerSpell()
    {
        if (GetPerkAura())
            return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
        else
            return 0;
    }

    void HandleEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetTriggerSpell() != 0)
            GetCaster()->CastSpell(GetCaster(), GetTriggerSpell(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_arcano_shatter::HandleEffectRemove, EFFECT_0, SPELL_AURA_OBS_MOD_POWER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_equipose : public AuraScript
{
    PrepareAuraScript(spell_equipose);

    Aura* GetPerkAura(Unit* player)
    {
        if (player->HasAura(300504))
            return player->GetAura(300504);

        if (player->HasAura(300505))
            return player->GetAura(300505);

        if (player->HasAura(300506))
            return player->GetAura(300506);

        if (player->HasAura(300507))
            return player->GetAura(300507);

        if (player->HasAura(300508))
            return player->GetAura(300508);

        if (player->HasAura(300509))
            return player->GetAura(300509);

        return nullptr;
    }

    int GetDamageProc()
    {
        return GetPerkAura(GetCaster())->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
    }

    int GetManaReductionProc()
    {
        return GetPerkAura(GetCaster())->GetSpellInfo()->GetEffect(EFFECT_2).TriggerSpell;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* player = GetCaster();

        if (!player || !GetPerkAura(player))
            return;

        float playerManaPct = player->GetPowerPct(POWER_MANA);

        if (playerManaPct >= 70)
        {
            player->RemoveAura(GetManaReductionProc());

            if (!player->HasAura(GetDamageProc()))
                player->CastSpell(player, GetDamageProc(), TRIGGERED_FULL_MASK);
        }

        if (playerManaPct < 70)
        {
            player->RemoveAura(GetDamageProc());

            if (!player->HasAura(GetManaReductionProc()))
                player->CastSpell(player, GetManaReductionProc(), TRIGGERED_FULL_MASK);
        }

    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_equipose::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class spell_touch_of_the_magi : public AuraScript
{
    PrepareAuraScript(spell_touch_of_the_magi);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return (!GetTarget()->HasAura(300528));
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        GetCaster()->CastSpell(GetCaster(), 300528, TRIGGERED_FULL_MASK);
        GetCaster()->CastSpell(GetCaster(), 300530, TRIGGERED_FULL_MASK);
        GetCaster()->CastSpell(procInfo.GetActionTarget(), 300531);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_touch_of_the_magi::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_touch_of_the_magi::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_touch_of_the_magi_debuff : public AuraScript
{
    PrepareAuraScript(spell_touch_of_the_magi_debuff);

    void TriggerDamage(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (DamageInfo* damageInfo = eventInfo.GetDamageInfo())
        {
            int32 amount = eventInfo.GetDamageInfo()->GetDamage();

            if (AuraEffect* protEff = GetCaster()->GetAuraEffect(300528, 0))
            {
                protEff->SetAmount(protEff->GetAmount() + amount);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_touch_of_the_magi_debuff::TriggerDamage, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_touch_of_the_magi_explosion : public AuraScript
{
    PrepareAuraScript(spell_touch_of_the_magi_explosion);

    Aura* GetPerkAura(Unit* player)
    {
        if (player->HasAura(300522))
            return player->GetAura(300522);

        if (player->HasAura(300523))
            return player->GetAura(300523);

        if (player->HasAura(300524))
            return player->GetAura(300524);

        if (player->HasAura(300525))
            return player->GetAura(300525);

        if (player->HasAura(300526))
            return player->GetAura(300526);

        if (player->HasAura(300527))
            return player->GetAura(300527);

        return nullptr;
    }

    int DamagePct(Unit* player)
    {
        return GetPerkAura(player)->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* player = GetCaster()->ToPlayer();
        int damage = 0;

        if (AuraEffect* protEff = player->GetAuraEffect(300528, 0))
            damage = protEff->GetAmount();

        int damageFinal = int(CalculatePct(damage, DamagePct(player)));

        GetTarget()->CastCustomSpell(300529, SPELLVALUE_BASE_POINT0, damageFinal, GetTarget(), true);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_touch_of_the_magi_explosion::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_nether_precision : public AuraScript
{
    PrepareAuraScript(spell_nether_precision);

    Aura* GetPerkAura()
    {
        if (GetCaster()->HasAura(300538))
            return GetCaster()->GetAura(300538);

        if (GetCaster()->HasAura(300539))
            return GetCaster()->GetAura(300539);

        if (GetCaster()->HasAura(300540))
            return GetCaster()->GetAura(300540);

        if (GetCaster()->HasAura(300541))
            return GetCaster()->GetAura(300541);

        if (GetCaster()->HasAura(300542))
            return GetCaster()->GetAura(300542);

        if (GetCaster()->HasAura(300543))
            return GetCaster()->GetAura(300543);

        return nullptr;
    }

    int GetTriggeredSpell()
    {
        return GetPerkAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetPerkAura())
            GetCaster()->AddAura(GetTriggeredSpell(),GetCaster());
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_nether_precision::OnRemove, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_resonance : public SpellScript
{
    PrepareSpellScript(spell_resonance);

    Aura* GetPerkAura()
    {
        if (GetCaster()->HasAura(300562))
            return GetCaster()->GetAura(300562);

        if (GetCaster()->HasAura(300563))
            return GetCaster()->GetAura(300563);

        if (GetCaster()->HasAura(300564))
            return GetCaster()->GetAura(300564);

        if (GetCaster()->HasAura(300565))
            return GetCaster()->GetAura(300565);

        if (GetCaster()->HasAura(300566))
            return GetCaster()->GetAura(300566);

        if (GetCaster()->HasAura(300567))
            return GetCaster()->GetAura(300567);

        return nullptr;
    }

    void HandleProc()
    {
        if (GetPerkAura())
            GetCaster()->RemoveAura(GetPerkAura());
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_resonance::HandleProc);
    }
};


void AddSC_mage_perks_scripts()
{
    RegisterSpellScript(spell_tempest_barrier);
    RegisterSpellScript(spell_cryo_freeze);
    RegisterSpellScript(spell_diverted_energy);
    RegisterSpellScript(spell_spiritual_armor);
    RegisterSpellScript(spell_spiritual_armor_remove);
    RegisterSpellScript(spell_triune_armor);
    RegisterSpellScript(spell_arcanic_gems);
    RegisterSpellScript(spell_unstable_magic);
    RegisterSpellScript(spell_icicle_ice_lance);
    RegisterSpellScript(spell_icicle_frostbolt);
    RegisterSpellScript(spell_slick_ice);
    RegisterSpellScript(spell_improved_brain_freeze);
    RegisterSpellScript(spell_improved_brain_freeze_remove);
    RegisterSpellScript(spell_lady_vashj_s_grasp_remove);
    RegisterSpellScript(spell_forzen_veins);
    RegisterSpellScript(spell_impulsive_veins);
    RegisterSpellScript(spell_flurry);
    RegisterSpellScript(spell_flurry_proc);
    RegisterSpellScript(spell_brain_fire);
    RegisterSpellScript(spell_ice_spike);
    RegisterSpellScript(spell_powerful_water_elemental);
    RegisterSpellScript(spell_cone_of_blizzard);
    RegisterSpellScript(spell_arcane_prodigy);
    RegisterSpellScript(spell_time_lord);
    RegisterSpellScript(spell_arcano_shatter);
    RegisterSpellScript(spell_equipose);
    RegisterSpellScript(spell_touch_of_the_magi);
    RegisterSpellScript(spell_touch_of_the_magi_debuff);
    RegisterSpellScript(spell_touch_of_the_magi_explosion);
    RegisterSpellScript(spell_nether_precision);
    RegisterSpellScript(spell_resonance);
}


