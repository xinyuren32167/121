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


enum DeathKnightSpells
{
    SPELL_SHIELD_PERMAFROST = 600029,
    SPELL_FROZEN_PUSLE = 600037,
    SPELL_ANTI_MAGIC_BARRIER_HEAL = 600066,
    SPELL_BLOOD_TRANSFUSION = 600095,
    SPELL_BLOOD_COIL_OF_DEVASTATION = 600285,
    SPELL_BLOOD_DRINKER_STAMINA = 600316,


    SPELL_DEATH_AND_DECAY = 49938,
    SPELL_FROST_FEVER_DOT = 55095,
};


class rune_dk_permafrost : public AuraScript
{
    PrepareAuraScript(rune_dk_permafrost);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
            return;

        Unit* caster = GetCaster();

        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
        caster->CastCustomSpell(SPELL_SHIELD_PERMAFROST, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_IGNORE_AURA_SCALING);

    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_permafrost::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_permafrost::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_dk_frozen_pulse : public AuraScript
{
    PrepareAuraScript(rune_dk_frozen_pulse);


    bool AtLeastTwoRuneAvailable(Player* player) {

        uint8 count = 0;

        for (uint8 i = 0; i < MAX_RUNES; ++i)
            if (player->GetRuneCooldown(i) == 0)
                count++;

        return count >= 2;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster)
            return false;

        Player* player = caster->ToPlayer();

        if (!player)
            return false;

        return AtLeastTwoRuneAvailable(player);
    }


    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        int32 amount = int32(CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount()));
        caster->CastCustomSpell(SPELL_FROZEN_PUSLE, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_IGNORE_AURA_SCALING);

    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_frozen_pulse::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_frozen_pulse::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_blood_feast: public AuraScript
{
    PrepareAuraScript(rune_dk_blood_feast);

    Aura* GetRuneBloodFeast(Unit* caster)
    {
        for (size_t i = 600059; i < 600065; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        if (absorbAmount <= 0)
            return;

        Unit* caster = GetCaster();

        Aura* aura = GetRuneBloodFeast(caster);

        if (!aura)
            return;

        int32 amount = aura->GetEffect(EFFECT_0)->GetAmount();
        int32 finalAmount = int32(CalculatePct(absorbAmount, amount));

        caster->CastCustomSpell(SPELL_ANTI_MAGIC_BARRIER_HEAL, SPELLVALUE_BASE_POINT0, finalAmount, caster, TRIGGERED_IGNORE_AURA_SCALING);
    }

    void Register() override
    {
        OnEffectAbsorb += AuraEffectAbsorbFn(rune_dk_blood_feast::Absorb, EFFECT_0);
    }
};


class rune_dk_blood_transfusion : public AuraScript
{
    PrepareAuraScript(rune_dk_blood_transfusion);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
            return;

        Unit* caster = GetCaster();

        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
        caster->CastCustomSpell(SPELL_BLOOD_TRANSFUSION, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_IGNORE_AURA_SCALING);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_blood_transfusion::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_blood_transfusion::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_dk_blood_barrier : public AuraScript
{
    PrepareAuraScript(rune_dk_blood_barrier);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetHealInfo()->GetHeal() <= 0)
            return;

        Unit* caster = GetCaster();

        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
        caster->CastCustomSpell(SPELL_BLOOD_TRANSFUSION, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_IGNORE_AURA_SCALING);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_blood_barrier::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_blood_barrier::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_dk_crismon_scourge : public AuraScript
{
    PrepareAuraScript(rune_dk_crismon_scourge);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        player->RemoveSpellCooldown(SPELL_DEATH_AND_DECAY);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_dk_crismon_scourge::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_shadow_desease : public AuraScript
{
    PrepareAuraScript(rune_dk_shadow_desease);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if(!target->HasAura(SPELL_FROST_FEVER_DOT))
            player->AddAura(SPELL_FROST_FEVER_DOT, target);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_shadow_desease::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_shadow_desease::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_dk_soul_drinker : public AuraScript
{
    PrepareAuraScript(rune_dk_soul_drinker);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetHealInfo()->GetHeal()  <= 0)
            return;

        Unit* caster = GetCaster();


        uint32 healingAmount = eventInfo.GetHealInfo()->GetHeal();

        int32 increasingMaximumHealth = int32(CalculatePct(healingAmount, aurEff->GetAmount()));

        int32 maximumAmount = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();

        int32 maximumHealthAllowable = int32(CalculatePct(caster->GetMaxHealth(), maximumAmount));

        uint32 currentHeal = caster->GetHealthPct();

        bool isOverHealing = currentHeal >= 100.f;

        if (isOverHealing)
        {
            if (Aura* aura = caster->GetAura(SPELL_BLOOD_DRINKER_STAMINA))
            {
                int32 currentAmount = aura->GetEffect(EFFECT_0)->GetAmount();
                aura->GetEffect(EFFECT_0)->ChangeAmount(std::min(currentAmount + increasingMaximumHealth, maximumHealthAllowable));
                caster->SetFullHealth();
            }
            else
            {
                caster->CastCustomSpell(SPELL_BLOOD_DRINKER_STAMINA, SPELLVALUE_BASE_POINT0, increasingMaximumHealth, caster, TRIGGERED_IGNORE_AURA_SCALING);
                caster->SetFullHealth();
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_soul_drinker::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_soul_drinker::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_phearomones : public SpellScript
{
    PrepareSpellScript(rune_dk_phearomones);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600228; i < 600233; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }


    void HandleProc()
    {
        Aura* aura = GetRuneAura(GetCaster());
        if (aura)
        {
            uint32 spellId = aura->GetEffect(EFFECT_0)->GetAmount();
            Position pos = GetExplTargetDest()->GetPosition();
            GetCaster()->CastSpell(GetExplTargetDest()->GetPositionX(), GetExplTargetDest()->GetPositionY(), GetExplTargetDest()->GetPositionZ(), spellId, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_dk_phearomones::HandleProc);
    }
};


class rune_dk_sanguine_ground : public SpellScript
{
    PrepareSpellScript(rune_dk_sanguine_ground);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600240; i < 600245; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }


    void HandleProc()
    {
        Aura* aura = GetRuneAura(GetCaster());
        if (aura)
        {
            uint32 spellId = aura->GetEffect(EFFECT_0)->GetAmount();
            Position pos = GetExplTargetDest()->GetPosition();
            GetCaster()->CastSpell(GetExplTargetDest()->GetPositionX(), GetExplTargetDest()->GetPositionY(), GetExplTargetDest()->GetPositionZ(), spellId, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_dk_sanguine_ground::HandleProc);
    }
};


class rune_dk_grip_of_the_dead : public SpellScript
{
    PrepareSpellScript(rune_dk_grip_of_the_dead);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600259; i < 600264; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }


    void HandleProc()
    {
        Aura* aura = GetRuneAura(GetCaster());
        if (aura)
        {
            uint32 spellId = aura->GetEffect(EFFECT_0)->GetAmount();
            Position pos = GetExplTargetDest()->GetPosition();
            GetCaster()->CastSpell(GetExplTargetDest()->GetPositionX(), GetExplTargetDest()->GetPositionY(), GetExplTargetDest()->GetPositionZ(), spellId, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_dk_grip_of_the_dead::HandleProc);
    }
};


class rune_dk_clenching_grasp : public SpellScript
{
    PrepareSpellScript(rune_dk_clenching_grasp);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600298; i < 600303; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }


    void HandleProc()
    {
        Aura* aura = GetRuneAura(GetCaster());
        if (aura)
        {
            uint32 spellId = aura->GetEffect(EFFECT_0)->GetAmount();
            GetCaster()->CastSpell(GetCaster()->GetVictim(), spellId, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_dk_clenching_grasp::HandleProc);
    }
};

class rune_dk_quick_decay : public AuraScript
{
    PrepareAuraScript(rune_dk_quick_decay);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
            if (Player* target = GetTarget()->ToPlayer())
                target->ModifySpellCooldown(SPELL_DEATH_AND_DECAY, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_dk_quick_decay::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_dk_coil_of_devastation : public AuraScript
{
    PrepareAuraScript(rune_dk_coil_of_devastation);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
            return;

        Unit* caster = GetCaster();

        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
        caster->CastCustomSpell(SPELL_BLOOD_COIL_OF_DEVASTATION, SPELLVALUE_BASE_POINT0, amount, caster->GetVictim(), TRIGGERED_IGNORE_AURA_SCALING);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_coil_of_devastation::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_coil_of_devastation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};




void AddSC_deathknight_perks_scripts()
{
    RegisterSpellScript(rune_dk_permafrost);
    RegisterSpellScript(rune_dk_frozen_pulse);
    RegisterSpellScript(rune_dk_blood_feast);
    RegisterSpellScript(rune_dk_blood_transfusion);
    RegisterSpellScript(rune_dk_crismon_scourge);
    RegisterSpellScript(rune_dk_shadow_desease);
    RegisterSpellScript(rune_dk_phearomones);
    RegisterSpellScript(rune_dk_sanguine_ground);
    RegisterSpellScript(rune_dk_quick_decay);
    RegisterSpellScript(rune_dk_coil_of_devastation);
    RegisterSpellScript(rune_dk_grip_of_the_dead);
    RegisterSpellScript(rune_dk_clenching_grasp);
    RegisterSpellScript(rune_dk_soul_drinker);
    RegisterSpellScript(rune_dk_blood_barrier);
}
