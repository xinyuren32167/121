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
    RUNE_SHIELD_PERMAFROST = 600028,
    RUNE_FROZEN_PUSLE = 600036,
    RUNE_ANTI_MAGIC_BARRIER_HEAL = 600065,
    RUNE_BLOOD_TRANSFUSION = 600094,
    RUNE_COIL_OF_DEVASTATION = 600363,
    RUNE_BLOOD_DRINKER_STAMINA = 600316,
    RUNE_DISEASE_AND_DECAY_PROC = 600135,
    RUNE_EXTRA_POTENT_DISEASES_FROST = 600341,
    RUNE_EXTRA_POTENT_DISEASES_BLOOD = 600342,
    RUNE_EXTRA_POTENT_DISEASES_VIRULENT = 600343,
    RUNE_CRIMSON_SCOURGE_PROC = 600167,
    RUNE_COLD_HEART_BUFF = 600350,
    RUNE_HEMOGLOBIN_PROC = 600351,
    RUNE_DK_AURA_OF_DECAY = 600358,
    RUNE_DK_AURA_OF_DECAY_DAMAGE = 600359,
    RUNE_DK_AURA_OF_DECAY_HEAL = 600360,
    RUNE_DK_AURA_OF_DECAY_HASTE = 600361,
    RUNE_DK_AURA_OF_DECAY_SANGUINE = 600362,
    RUNE_DK_GRIP_OF_THE_DEAD_SLOW = 600265,
    RUNE_DK_SHACKLES_OF_BRYNDAOR_RUNIC = 600364,
    RUNE_DK_ENDLESS_DEATH_PROC = 600408,
    RUNE_DK_THE_UNBEAUTIFUL_PROC = 600471,
    RUNE_DK_DRAW_BLOOD_PROC = 600534,
    RUNE_DK_DRAW_BLOOD_COOLDOWN = 600535,
    RUNE_DK_BLOOD_BATH_PROC = 600633,
    RUNE_DK_TORMENT_PROC = 600677,
    RUNE_DK_RELISH_IN_BLOOD_HEAL = 600690,
    RUNE_DK_DAMNATION_PROC = 600704,

    SPELL_DK_DEATH_AND_DECAY = 49938,
    SPELL_DK_FROST_FEVER = 55095,
    SPELL_DK_BLOOD_PLAGUE = 59879,
    SPELL_DK_VIRULENT_PLAGUE = 80332,
    TALENT_DK_DEATH_AND_RENEW = 87109,
    SPELL_DK_ICEBOUND_FORTITUDE = 48792,
    SPELL_DK_MIND_FREEZE = 47528,
    SPELL_DK_BONE_SHIELD = 49222,
    SPELL_DK_HYSTERIA = 49016,
    SPELL_DK_VAMPIRIC_BLOOD = 55233,
    SPELL_DK_BONESTORM = 80367,
};

class rune_dk_permafrost : public AuraScript
{
    PrepareAuraScript(rune_dk_permafrost);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
            return false;

        if (damageInfo->GetDamage() < 0)
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
        caster->CastCustomSpell(RUNE_SHIELD_PERMAFROST, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_IGNORE_AURA_SCALING);
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

    bool AtLeastTwoRuneAvailable(Player* player)
    {
        uint8 count = 0;

        for (uint8 i = 0; i < MAX_RUNES; ++i)
            if (player->GetRuneCooldown(i) == 0)
                count++;

        return count <= 2;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return false;

        return AtLeastTwoRuneAvailable(player);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        int32 amount = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount());
        caster->CastCustomSpell(RUNE_FROZEN_PUSLE, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
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

        if (Aura* aura = GetRuneBloodFeast(caster))
        {
            int32 healPct = aura->GetEffect(EFFECT_0)->GetAmount();
            int32 healAmount = CalculatePct(absorbAmount, healPct);

            caster->CastCustomSpell(RUNE_ANTI_MAGIC_BARRIER_HEAL, SPELLVALUE_BASE_POINT0, healAmount, caster, TRIGGERED_FULL_MASK);
        }   
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
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
            return false;

        if (damageInfo->GetDamage() < 0)
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
            return;

        Unit* caster = GetCaster();

        int32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
        caster->CastCustomSpell(RUNE_BLOOD_TRANSFUSION, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
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
        caster->CastCustomSpell(RUNE_BLOOD_TRANSFUSION, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_IGNORE_AURA_SCALING);
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
        player->RemoveSpellCooldown(SPELL_DK_DEATH_AND_DECAY);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_dk_crismon_scourge::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_shadow_disease : public AuraScript //Chilling decay
{
    PrepareAuraScript(rune_dk_shadow_disease);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetActionTarget())
            return false;

        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();

        if(!target->HasAura(SPELL_DK_FROST_FEVER))
            GetCaster()->AddAura(SPELL_DK_FROST_FEVER, target);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_shadow_disease::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_shadow_disease::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_soul_drinker : public AuraScript
{
    PrepareAuraScript(rune_dk_soul_drinker);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        HealInfo* healInfo = eventInfo.GetHealInfo();

        if (!eventInfo.GetActionTarget())
            return false;

        if (!healInfo || !healInfo->GetHeal())
            return false;

        if (healInfo->GetHeal() < 0)
            return false;

        if ((eventInfo.GetActionTarget()->GetMaxHealth() - eventInfo.GetActionTarget()->GetHealth()) > eventInfo.GetHealInfo()->GetHeal())
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        uint32 healingAmount = eventInfo.GetHealInfo()->GetHeal();
        int32 increasingMaximumHealth = CalculatePct(healingAmount, aurEff->GetAmount());
        int32 maximumAmount = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        int32 maximumHealthAllowable = CalculatePct(caster->GetMaxHealth(), maximumAmount);
        uint32 currentHeal = caster->GetHealthPct();

        if (Aura* aura = caster->GetAura(RUNE_BLOOD_DRINKER_STAMINA))
        {
            int32 currentAmount = aura->GetEffect(EFFECT_0)->GetAmount();
            aura->GetEffect(EFFECT_0)->ChangeAmount(std::min(currentAmount + increasingMaximumHealth, maximumHealthAllowable));
        }
        else
        {
            caster->CastCustomSpell(RUNE_BLOOD_DRINKER_STAMINA, SPELLVALUE_BASE_POINT0, increasingMaximumHealth, caster, TRIGGERED_FULL_MASK);
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
        for (size_t i = 600228; i < 600234; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        if (Aura* aura = GetRuneAura(GetCaster()))
            GetCaster()->CastSpell(GetExplTargetDest()->GetPositionX(), GetExplTargetDest()->GetPositionY(), GetExplTargetDest()->GetPositionZ(), aura->GetEffect(EFFECT_0)->GetAmount(), TRIGGERED_FULL_MASK);
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
        for (size_t i = 600240; i < 600246; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        if (Aura* aura = GetRuneAura(GetCaster()))
            GetCaster()->CastSpell(GetExplTargetDest()->GetPositionX(), GetExplTargetDest()->GetPositionY(), GetExplTargetDest()->GetPositionZ(), aura->GetEffect(EFFECT_0)->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_dk_sanguine_ground::HandleProc);
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
                target->ModifySpellCooldown(SPELL_DK_DEATH_AND_DECAY, aurEff->GetAmount());
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
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
            return false;

        if (damageInfo->GetDamage() < 0)
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 amount = (CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount())) / 4;
        GetCaster()->CastCustomSpell(RUNE_COIL_OF_DEVASTATION, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_coil_of_devastation::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_coil_of_devastation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_disease_and_decay : public AuraScript
{
    PrepareAuraScript(rune_dk_disease_and_decay);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetActionTarget())
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastSpell(eventInfo.GetActionTarget()->GetPositionX(), eventInfo.GetActionTarget()->GetPositionY(), eventInfo.GetActionTarget()->GetPositionZ(), RUNE_DISEASE_AND_DECAY_PROC, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_disease_and_decay::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_disease_and_decay::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_repeated_application : public AuraScript
{
    PrepareAuraScript(rune_dk_repeated_application);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActionTarget();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Unit* target = eventInfo.GetActionTarget())
        {
            if (Aura* bloodPlague = target->GetAura(SPELL_DK_BLOOD_PLAGUE, GetCaster()->GetGUID()))
            {
                bloodPlague->SetDuration(bloodPlague->GetDuration() + aurEff->GetAmount());
                bloodPlague->GetEffect(EFFECT_1)->ResetTicks();
            }
            if (Aura* frostFever = target->GetAura(SPELL_DK_FROST_FEVER, GetCaster()->GetGUID()))
            {
                frostFever->SetDuration(frostFever->GetDuration() + aurEff->GetAmount());
                frostFever->GetEffect(EFFECT_0)->ResetTicks();
            }
            if (Aura* virulentPlague = target->GetAura(SPELL_DK_VIRULENT_PLAGUE, GetCaster()->GetGUID()))
            {
                virulentPlague->SetDuration(virulentPlague->GetDuration() + aurEff->GetAmount());
                virulentPlague->GetEffect(EFFECT_1)->ResetTicks();
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_repeated_application::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_repeated_application::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_extra_potent_diseases : public AuraScript
{
    PrepareAuraScript(rune_dk_extra_potent_diseases);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();

        if (!target || target->isDead())
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 damageAmount = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount());
            if (spellInfo->Id == SPELL_DK_FROST_FEVER)
                GetCaster()->CastCustomSpell(RUNE_EXTRA_POTENT_DISEASES_FROST, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
            else if (spellInfo->Id == SPELL_DK_BLOOD_PLAGUE)
                GetCaster()->CastCustomSpell(RUNE_EXTRA_POTENT_DISEASES_BLOOD, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
            else if (spellInfo->Id == SPELL_DK_VIRULENT_PLAGUE)
                GetCaster()->CastCustomSpell(RUNE_EXTRA_POTENT_DISEASES_VIRULENT, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_extra_potent_diseases::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_extra_potent_diseases::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_crimson_scourge : public AuraScript
{
    PrepareAuraScript(rune_dk_crimson_scourge);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster() && GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        caster->RemoveSpellCooldown(SPELL_DK_DEATH_AND_DECAY, true);
        caster->CastSpell(caster, RUNE_CRIMSON_SCOURGE_PROC, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_crimson_scourge::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_crimson_scourge::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_cold_heart : public SpellScript
{
    PrepareSpellScript(rune_dk_cold_heart);

    void HandleProc()
    {
        if (Aura* aura = GetCaster()->GetAura(RUNE_COLD_HEART_BUFF))
            aura->Remove();
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_dk_cold_heart::HandleProc);
    }
};

class rune_dk_hemoglobin : public AuraScript
{
    PrepareAuraScript(rune_dk_hemoglobin);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        HealInfo* healInfo = eventInfo.GetHealInfo();

        if (!healInfo || !healInfo->GetHeal())
            return false;

        if (healInfo->GetHeal() < 0)
            return false;

        return GetCaster() && GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastSpell(GetCaster()->GetPositionX(), GetCaster()->GetPositionY(), GetCaster()->GetPositionZ(), RUNE_HEMOGLOBIN_PROC, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_hemoglobin::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_hemoglobin::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_aura_of_decay : public AuraScript
{
    PrepareAuraScript(rune_dk_aura_of_decay);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        if (!target)
            return;

        target->removeSpell(SPELL_DK_DEATH_AND_DECAY, SPEC_MASK_ALL, false);
        target->learnSpell(RUNE_DK_AURA_OF_DECAY);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        if (!target)
            return;

        target->removeSpell(RUNE_DK_AURA_OF_DECAY, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_DK_DEATH_AND_DECAY);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_dk_aura_of_decay::HandleLearn, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_dk_aura_of_decay::HandleUnlearn, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_dk_aura_of_decay_periodic : public AuraScript
{
    PrepareAuraScript(rune_dk_aura_of_decay_periodic);

    Aura* GetPhearomonesAura(Unit* caster)
    {
        for (size_t i = 600228; i < 600234; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    Aura* GetSanguineAura(Unit* caster)
    {
        for (size_t i = 600240; i < 600246; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    void PeriodicTick(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (caster->IsInCombat())
        {
            caster->CastSpell(caster, RUNE_DK_AURA_OF_DECAY_DAMAGE, TRIGGERED_FULL_MASK);

            if (caster->HasAura(TALENT_DK_DEATH_AND_RENEW))
                caster->CastSpell(caster, RUNE_DK_AURA_OF_DECAY_HEAL, TRIGGERED_FULL_MASK);

            if (Aura* phearomones = GetPhearomonesAura(caster))
                caster->CastCustomSpell(RUNE_DK_AURA_OF_DECAY_HASTE, SPELLVALUE_BASE_POINT0, phearomones->GetEffect(EFFECT_1)->GetAmount(), caster, TRIGGERED_FULL_MASK);

            if (Aura* sanguine = GetSanguineAura(caster))
            {
                int32 amount = sanguine->GetEffect(EFFECT_1)->GetAmount();
                caster->CastCustomSpell(caster, RUNE_DK_AURA_OF_DECAY_SANGUINE, &amount, &amount, nullptr, true, nullptr);
            } 
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_dk_aura_of_decay_periodic::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_dk_grip_of_the_dead : public AuraScript
{
    PrepareAuraScript(rune_dk_grip_of_the_dead);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetActionTarget())
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetActionTarget()->HasAura(RUNE_DK_GRIP_OF_THE_DEAD_SLOW))
            GetCaster()->CastCustomSpell(RUNE_DK_GRIP_OF_THE_DEAD_SLOW, SPELLVALUE_AURA_DURATION, aurEff->GetAmount(), eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_grip_of_the_dead::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_grip_of_the_dead::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_shackles_of_bryndaor : public AuraScript
{
    PrepareAuraScript(rune_dk_shackles_of_bryndaor);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        HealInfo* healInfo = eventInfo.GetHealInfo();

        if (!healInfo || !healInfo->GetHeal())
            return false;

        if (healInfo->GetHeal() < 0)
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        int32 threshold = caster->CountPctFromMaxHealth(aurEff->GetAmount());
        int32 healAmount = eventInfo.GetHealInfo()->GetHeal();

        if (healAmount >= threshold)
            caster->CastSpell(caster, RUNE_DK_SHACKLES_OF_BRYNDAOR_RUNIC, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_shackles_of_bryndaor::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_shackles_of_bryndaor::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_adaptation : public AuraScript
{
    PrepareAuraScript(rune_dk_adaptation);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster() && GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 buffId = GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
        GetCaster()->CastCustomSpell(buffId, SPELLVALUE_AURA_STACK, GetAura()->GetStackAmount(), GetCaster(), TRIGGERED_FULL_MASK);
        GetAura()->Remove();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_adaptation::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_adaptation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_endless_death : public AuraScript
{
    PrepareAuraScript(rune_dk_endless_death);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        uint32 chance = CalculatePct(caster->GetUnitParryChance(), aurEff->GetAmount());

        if (roll_chance_i(chance))
            caster->CastSpell(eventInfo.GetActor(), RUNE_DK_ENDLESS_DEATH_PROC, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_dk_endless_death::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_heart_of_ice : public AuraScript
{
    PrepareAuraScript(rune_dk_heart_of_ice);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Aura* aura = GetCaster()->GetAura(SPELL_DK_ICEBOUND_FORTITUDE))
            aura->SetDuration(aura->GetDuration() + aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_dk_heart_of_ice::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_unholy_endurance : public AuraScript
{
    PrepareAuraScript(rune_dk_unholy_endurance);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600439; i < 600445; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    void HandleEffectApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Aura* aura = GetRuneAura(GetCaster()))
            GetCaster()->AddAura(aura->GetEffect(EFFECT_1)->GetAmount(), GetCaster());
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (Aura* runeAura = GetRuneAura(GetCaster()))
            if (Aura* aura = GetCaster()->GetAura(runeAura->GetEffect(EFFECT_1)->GetAmount()))
                aura->Remove();
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_dk_unholy_endurance::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_dk_unholy_endurance::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_dk_coldthirst : public SpellScript
{
    PrepareSpellScript(rune_dk_coldthirst);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600451; i < 600457; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = GetRuneAura(caster))
        {
            int32 runic = aura->GetEffect(EFFECT_0)->GetAmount();
            int32 reduction = aura->GetEffect(EFFECT_1)->GetAmount();

            caster->EnergizeBySpell(caster, SPELL_DK_MIND_FREEZE, runic, POWER_RUNIC_POWER);
            caster->ModifySpellCooldown(SPELL_DK_MIND_FREEZE, reduction);
        }
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_dk_coldthirst::HandleProc);
    }
};

class rune_dk_the_unbeautiful : public AuraScript
{
    PrepareAuraScript(rune_dk_the_unbeautiful);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600464; i < 600470; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Aura* aura = GetRuneAura(GetCaster()))
            if (roll_chance_i(aura->GetEffect(EFFECT_0)->GetAmount()))
                GetCaster()->CastSpell(GetCaster(), RUNE_DK_THE_UNBEAUTIFUL_PROC, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_dk_the_unbeautiful::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_enfeeble : public AuraScript
{
    PrepareAuraScript(rune_dk_enfeeble);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600472; i < 600478; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Aura* aura = GetRuneAura(GetCaster()->GetCharmerOrOwner()))
            GetCaster()->CastSpell(eventInfo.GetActionTarget(), aura->GetEffect(EFFECT_0)->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_dk_enfeeble::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_rune_mastery : public AuraScript
{
    PrepareAuraScript(rune_dk_rune_mastery);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        uint32 runeSpent = 0;

        if (SpellInfo const* spell = eventInfo.GetSpellInfo())
        {
            SpellRuneCostEntry const* src = sSpellRuneCostStore.LookupEntry(spell->RuneCostID);
            if (src)
            {
                runeSpent = src->RuneCost[RUNE_BLOOD];
            }
        }

        if (runeSpent < 0)
            return false;

        return GetCaster()->IsAlive();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_rune_mastery::CheckProc);
    }
};

class rune_dk_draw_blood : public AuraScript
{
    PrepareAuraScript(rune_dk_draw_blood);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* victim = GetTarget();
        int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
        uint32 allowedHealth = victim->CountPctFromMaxHealth(30.f);

        int32 damageAmount = CalculatePct(victim->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount());

        if ((remainingHealth < int32(allowedHealth)) && !victim->ToPlayer()->HasSpellCooldown(RUNE_DK_DRAW_BLOOD_COOLDOWN))
        {
            victim->CastCustomSpell(RUNE_DK_DRAW_BLOOD_PROC, SPELLVALUE_BASE_POINT0, damageAmount, victim, TRIGGERED_FULL_MASK);
            victim->CastSpell(victim, RUNE_DK_DRAW_BLOOD_COOLDOWN);
            victim->ToPlayer()->AddSpellCooldown(RUNE_DK_DRAW_BLOOD_COOLDOWN, 0, 180000);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(rune_dk_draw_blood::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(rune_dk_draw_blood::Absorb, EFFECT_0);
    }
};

class rune_dk_might_of_the_lich_king : public AuraScript
{
    PrepareAuraScript(rune_dk_might_of_the_lich_king);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        // min pct of hp is stored in effect 0 of talent spell
        int32 remainingHp = int32(GetTarget()->GetHealth() - dmgInfo.GetDamage());
        int32 minHp = int32(GetTarget()->CountPctFromMaxHealth(30));

        // Damage that would take you below [effect0] health or taken while you are at [effect0]
        if (remainingHp < minHp)
            absorbAmount = CalculatePct(dmgInfo.GetDamage(), aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount());
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(rune_dk_might_of_the_lich_king::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(rune_dk_might_of_the_lich_king::Absorb, EFFECT_0);
    }
};

class rune_dk_parasitic_blood : public AuraScript
{
    PrepareAuraScript(rune_dk_parasitic_blood);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        HealInfo* healInfo = eventInfo.GetHealInfo();

        if (!healInfo || !healInfo->GetHeal())
            return false;

        if (healInfo->GetHeal() < 0)
            return false;

        if (healInfo->GetHealer() != GetCaster())
            return false;

        return GetCaster()->IsAlive();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_parasitic_blood::CheckProc);
    }
};

class rune_dk_insatiable_blade : public SpellScript
{
    PrepareSpellScript(rune_dk_insatiable_blade);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600579; i < 600585; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        Unit* caster = GetCaster();
        if (Aura* rune = GetRuneAura(caster))
        {
            if (Aura* aura = caster->GetAura(SPELL_DK_BONE_SHIELD))
                aura->ModStackAmount(5);
            else
                caster->CastCustomSpell(SPELL_DK_BONE_SHIELD, SPELLVALUE_AURA_STACK, 5, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_dk_insatiable_blade::HandleProc);
    }
};

class rune_dk_collective_hysteria : public AuraScript
{
    PrepareAuraScript(rune_dk_collective_hysteria);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600609; i < 600615; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        if(Aura* aura = GetRuneAura(caster))
            if (GetAura()->GetOwner()->ToUnit() != caster)
            {
                caster->CastSpell(caster, SPELL_DK_HYSTERIA, TRIGGERED_FULL_MASK);
                if (Aura* hysteria = caster->GetAura(SPELL_DK_HYSTERIA))
                    aura->GetEffect(EFFECT_0)->ChangeAmount(aura->GetEffect(EFFECT_0)->GetAmount() / 2);
            }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_dk_collective_hysteria::HandleApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_dk_red_thirst : public AuraScript
{
    PrepareAuraScript(rune_dk_red_thirst);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 spellRunic = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));

            if (spellRunic <= 0)
                return;

            int32 runicAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRunic;
            int32 runicThreshold = aurEff->GetAmount();

            if (runicAccumulated >= runicThreshold)
            {
                GetCaster()->ToPlayer()->ModifySpellCooldown(SPELL_DK_VAMPIRIC_BLOOD, -1000);
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(runicAccumulated - runicThreshold);
            }
            else
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(runicAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_dk_red_thirst::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_blood_bath : public AuraScript
{
    PrepareAuraScript(rune_dk_blood_bath);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(SPELL_DK_VAMPIRIC_BLOOD))
            return false;

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
            return false;

        if (damageInfo->GetDamage() < 0)
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());

        aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(damage);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_blood_bath::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_blood_bath::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_blood_bath_expire : public AuraScript
{
    PrepareAuraScript(rune_dk_blood_bath_expire);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600627; i < 600633; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (Aura* runeAura = GetRuneAura(GetCaster()))
        {
            GetCaster()->CastCustomSpell(RUNE_DK_BLOOD_BATH_PROC, SPELLVALUE_BASE_POINT0, runeAura->GetEffect(EFFECT_1)->GetAmount(), GetCaster(), TRIGGERED_FULL_MASK);
            runeAura->GetEffect(EFFECT_1)->SetAmount(0);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_dk_blood_bath_expire::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_HEALING_PCT, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_dk_in_service_of_gorefiend : public AuraScript
{
    PrepareAuraScript(rune_dk_in_service_of_gorefiend);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Aura* vampiric = GetCaster()->GetAura(SPELL_DK_VAMPIRIC_BLOOD))
            vampiric->SetDuration(vampiric->GetDuration() + aurEff->GetAmount());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_in_service_of_gorefiend::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_in_service_of_gorefiend::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_in_service_of_gorefiend_runic : public SpellScript
{
    PrepareSpellScript(rune_dk_in_service_of_gorefiend_runic);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600635; i < 600641; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    void HandleProc()
    {
        if (Aura* rune = GetRuneAura(GetCaster()))
            GetCaster()->EnergizeBySpell(GetCaster(), SPELL_DK_VAMPIRIC_BLOOD, rune->GetEffect(EFFECT_1)->GetAmount(), POWER_RUNIC_POWER);
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_dk_in_service_of_gorefiend_runic::HandleProc);
    }
};

class rune_dk_hardened_blood : public AuraScript
{
    PrepareAuraScript(rune_dk_hardened_blood);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Aura* vampiric = GetCaster()->GetAura(SPELL_DK_VAMPIRIC_BLOOD))
            vampiric->SetDuration(vampiric->GetDuration() + 2000);
        else
        {
            if(Aura* aura = GetCaster()->AddAura(SPELL_DK_VAMPIRIC_BLOOD, GetCaster()))
                aura->SetDuration(4000);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_dk_hardened_blood::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_perserverance_of_the_ebon_blade : public AuraScript
{
    PrepareAuraScript(rune_dk_perserverance_of_the_ebon_blade);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600659; i < 600665; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (Aura* runeAura = GetRuneAura(GetCaster()))
            GetCaster()->CastSpell(GetCaster(), runeAura->GetEffect(EFFECT_0)->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_dk_perserverance_of_the_ebon_blade::HandleEffectRemove, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_dk_torment : public AuraScript
{
    PrepareAuraScript(rune_dk_torment);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(SPELL_DK_BONESTORM))
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));
            if (spellRage <= 0)
                return;

            int32 rageAccumulated = GetAura()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
            GetAura()->GetEffect(EFFECT_1)->ChangeAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_torment::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_torment::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_torment_expire : public AuraScript
{
    PrepareAuraScript(rune_dk_torment_expire);

    Aura* GetRuneAura()
    {
        for (size_t i = 600671; i < 600677; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }
        return nullptr;
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (Aura* runeAura = GetRuneAura())
        {
            int32 threshold = runeAura->GetEffect(EFFECT_0)->GetAmount();
            int32 rageAccumulated = runeAura->GetEffect(EFFECT_1)->GetAmount();
            runeAura->GetEffect(EFFECT_1)->SetAmount(0);
            int32 calculatedStack = static_cast<int32>(3 * (rageAccumulated / threshold));
            if (calculatedStack > 0)
                GetCaster()->CastCustomSpell(RUNE_DK_TORMENT_PROC, SPELLVALUE_BASE_POINT0, calculatedStack, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_dk_torment_expire::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_dk_relish_in_blood: public AuraScript
{
    PrepareAuraScript(rune_dk_relish_in_blood);

    Aura* GetRuneAura()
    {
        for (size_t i = 600684; i < 600690; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }
        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if(Aura* boneShield = GetCaster()->GetAura(SPELL_DK_BONE_SHIELD))
            if (Aura* rune = GetRuneAura())
            {
                Unit* caster = GetCaster();
                int32 heal = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), rune->GetEffect(EFFECT_0)->GetAmount());
                heal *= boneShield->GetStackAmount();
                caster->CastCustomSpell(RUNE_DK_RELISH_IN_BLOOD_HEAL, SPELLVALUE_BASE_POINT0, heal, caster, TRIGGERED_FULL_MASK);
                caster->EnergizeBySpell(caster, SPELL_DK_DEATH_AND_DECAY, 100, POWER_RUNIC_POWER);
            }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_dk_relish_in_blood::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_dk_damnation : public AuraScript
{
    PrepareAuraScript(rune_dk_damnation);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
            return false;

        if (damageInfo->GetDamage() < 0)
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());

        GetCaster()->CastCustomSpell(RUNE_DK_DAMNATION_PROC, SPELLVALUE_BASE_POINT0, (damage / 8), eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_dk_damnation::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_dk_damnation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

void AddSC_deathknight_perks_scripts()
{
    RegisterSpellScript(rune_dk_permafrost);
    RegisterSpellScript(rune_dk_frozen_pulse);
    RegisterSpellScript(rune_dk_blood_feast);
    RegisterSpellScript(rune_dk_blood_transfusion);
    RegisterSpellScript(rune_dk_crismon_scourge);
    RegisterSpellScript(rune_dk_shadow_disease);
    RegisterSpellScript(rune_dk_phearomones);
    RegisterSpellScript(rune_dk_sanguine_ground);
    RegisterSpellScript(rune_dk_quick_decay);
    RegisterSpellScript(rune_dk_coil_of_devastation);
    RegisterSpellScript(rune_dk_clenching_grasp);
    RegisterSpellScript(rune_dk_soul_drinker);
    RegisterSpellScript(rune_dk_blood_barrier);
    RegisterSpellScript(rune_dk_disease_and_decay);
    RegisterSpellScript(rune_dk_repeated_application);
    RegisterSpellScript(rune_dk_extra_potent_diseases);
    RegisterSpellScript(rune_dk_crimson_scourge);
    RegisterSpellScript(rune_dk_cold_heart);
    RegisterSpellScript(rune_dk_hemoglobin);
    RegisterSpellScript(rune_dk_aura_of_decay);
    RegisterSpellScript(rune_dk_aura_of_decay_periodic);
    RegisterSpellScript(rune_dk_grip_of_the_dead);
    RegisterSpellScript(rune_dk_shackles_of_bryndaor);
    RegisterSpellScript(rune_dk_adaptation);
    RegisterSpellScript(rune_dk_endless_death);
    RegisterSpellScript(rune_dk_heart_of_ice);
    RegisterSpellScript(rune_dk_unholy_endurance);
    RegisterSpellScript(rune_dk_coldthirst);
    RegisterSpellScript(rune_dk_the_unbeautiful);
    RegisterSpellScript(rune_dk_enfeeble);
    RegisterSpellScript(rune_dk_rune_mastery);
    RegisterSpellScript(rune_dk_draw_blood);
    RegisterSpellScript(rune_dk_might_of_the_lich_king);
    RegisterSpellScript(rune_dk_parasitic_blood);
    RegisterSpellScript(rune_dk_insatiable_blade);
    RegisterSpellScript(rune_dk_collective_hysteria);
    RegisterSpellScript(rune_dk_red_thirst);
    RegisterSpellScript(rune_dk_blood_bath);
    RegisterSpellScript(rune_dk_blood_bath_expire);
    RegisterSpellScript(rune_dk_in_service_of_gorefiend_runic);
    RegisterSpellScript(rune_dk_in_service_of_gorefiend);
    RegisterSpellScript(rune_dk_hardened_blood);
    RegisterSpellScript(rune_dk_perserverance_of_the_ebon_blade);
    RegisterSpellScript(rune_dk_torment);
    RegisterSpellScript(rune_dk_torment_expire);
    RegisterSpellScript(rune_dk_relish_in_blood);
    RegisterSpellScript(rune_dk_damnation);
}
