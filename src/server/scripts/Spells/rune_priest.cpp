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

enum PriestSpells
{
    // Spells
    SPELL_PRIEST_DEVOURING_PLAGUE = 48300,
    SPELL_PRIEST_FADE = 586,
    SPELL_PRIEST_FLASH_HEAL = 48071,
    SPELL_PRIEST_MIND_BLAST = 48127,
    SPELL_PRIEST_PENANCE = 47540,
    SPELL_PRIEST_POWER_WORD_SHIELD = 48066,
    SPELL_PRIEST_POWER_WORD_SOLACE = 81016,
    SPELL_PRIEST_PURGE_THE_WICKED = 81017,
    SPELL_PRIEST_RENEW = 48068,
    SPELL_PRIEST_SHADOW_WORD_PAIN = 48125,
    SPELL_PRIEST_SMITE = 48123,
    SPELL_PRIEST_VAMPIRIC_TOUCH = 48160,

    // Talents
    TALENT_PRIEST_SHADOWY_APPARITIONS_DAMAGE = 81086,

    // Runes
    RUNE_PRIEST_ARMOR_OF_FAITH_SHIELD = 900148,
    RUNE_PRIEST_BINDING_HEALS_HEAL = 900192,
    RUNE_PRIEST_BURNING_VEHEMENCE_DAMAGE = 900212,
    RUNE_PRIEST_INSIDIOUS_IRE_DAMAGE = 900258,
    RUNE_PRIEST_PSYCHIC_LINK_DAMAGE = 900278,
    RUNE_PRIEST_EXPIATION_DAMAGE = 900310,
};

class rune_pri_faded : public AuraScript
{
    PrepareAuraScript(rune_pri_faded);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 debuffSpell = aurEff->GetAmount();

        if (caster->HasAura(debuffSpell))
            return;

        caster->AddAura(SPELL_PRIEST_FADE, caster);
        caster->AddAura(debuffSpell, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_faded::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_faded::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_armor_of_faith : public AuraScript
{
    PrepareAuraScript(rune_pri_armor_of_faith);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetHealInfo() && eventInfo.GetHitMask();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;

        if (spellID == SPELL_PRIEST_FLASH_HEAL && eventInfo.GetHitMask() != PROC_EX_CRITICAL_HIT)
            return;

        int32 heal = eventInfo.GetHealInfo()->GetHeal();

        if (heal <= 0)
            return;

        int32 amount = CalculatePct(heal, aurEff->GetAmount());

        if (Aura* shield = target->GetAura(RUNE_PRIEST_ARMOR_OF_FAITH_SHIELD))
            amount += shield->GetEffect(EFFECT_0)->GetAmount();

        caster->CastCustomSpell(RUNE_PRIEST_ARMOR_OF_FAITH_SHIELD, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_armor_of_faith::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_armor_of_faith::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_protective_light : public AuraScript
{
    PrepareAuraScript(rune_pri_protective_light);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetHealInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        if (caster != target)
            return;

        int32 procSpell = aurEff->GetAmount();

        caster->AddAura(procSpell, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_protective_light::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_protective_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_unwavering_will : public AuraScript
{
    PrepareAuraScript(rune_pri_unwavering_will);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 healthThreshold = aurEff->GetAmount();
        int32 buffSpell = GetEffect(EFFECT_1)->GetAmount();

        if (caster->GetHealthPct() >= healthThreshold)
        {
            if (!caster->HasAura(buffSpell))
                caster->AddAura(buffSpell, caster);
        }
        else
            if (caster->HasAura(buffSpell))
                caster->RemoveAura(buffSpell);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_pri_unwavering_will::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_pri_binding_heals : public AuraScript
{
    PrepareAuraScript(rune_pri_binding_heals);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        if (caster == target)
            return;

        int32 heal = eventInfo.GetHealInfo()->GetHeal();
        int32 amount = CalculatePct(heal, aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_PRIEST_BINDING_HEALS_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_binding_heals::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_binding_heals::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_burning_vehemence : public AuraScript
{
    PrepareAuraScript(rune_pri_burning_vehemence);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_PRIEST_BURNING_VEHEMENCE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_burning_vehemence::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_burning_vehemence::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_burning_vehemence_target : public SpellScript
{
    PrepareSpellScript(rune_pri_burning_vehemence_target);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetTarget());

        targets.remove(target);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_pri_burning_vehemence_target::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class rune_pri_shadowy_insight : public AuraScript
{
    PrepareAuraScript(rune_pri_shadowy_insight);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
            player->RemoveSpellCooldown(SPELL_PRIEST_MIND_BLAST, true);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_shadowy_insight::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_shadowy_insight::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_pri_insidious_ire : public AuraScript
{
    PrepareAuraScript(rune_pri_insidious_ire);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        if (!target->HasAura(SPELL_PRIEST_DEVOURING_PLAGUE))
            return;

        if (!target->HasAura(SPELL_PRIEST_SHADOW_WORD_PAIN))
            return;

        if (!target->HasAura(SPELL_PRIEST_VAMPIRIC_TOUCH))
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_PRIEST_INSIDIOUS_IRE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_insidious_ire::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_insidious_ire::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_psychic_link : public AuraScript
{
    PrepareAuraScript(rune_pri_psychic_link);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetSpellInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;

        if (spellID == TALENT_PRIEST_SHADOWY_APPARITIONS_DAMAGE || spellID == SPELL_PRIEST_SHADOW_WORD_PAIN || spellID == SPELL_PRIEST_VAMPIRIC_TOUCH)
            return;

        auto const& threatList = caster->getAttackers();
        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, aurEff->GetAmount());

        for (auto const& targets : threatList)
            if (targets->IsAlive())
            {
                if (!targets->HasAura(SPELL_PRIEST_VAMPIRIC_TOUCH))
                    continue;

                if (targets == target)
                    continue;

                float distance = targets->GetDistance(target->GetPosition());

                if (distance > 40)
                    continue;

                caster->CastCustomSpell(RUNE_PRIEST_PSYCHIC_LINK_DAMAGE, SPELLVALUE_BASE_POINT0, amount, targets, TRIGGERED_FULL_MASK);
            }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_psychic_link::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_psychic_link::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_expiation : public AuraScript
{
    PrepareAuraScript(rune_pri_expiation);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        if (!target->HasAura(SPELL_PRIEST_SHADOW_WORD_PAIN) && !target->HasAura(SPELL_PRIEST_PURGE_THE_WICKED))
            return;

        int32 durationConsumed = aurEff->GetAmount();
        int32 amount = 0;

        if (Aura* swPain = target->GetAura(SPELL_PRIEST_SHADOW_WORD_PAIN))
        {
            int32 remainingTick = swPain->GetEffect(EFFECT_0)->GetRemaningTicks();
            int32 multi = std::min<int32>(durationConsumed / 2000, remainingTick);
            amount += swPain->GetEffect(EFFECT_0)->GetAmount() * multi;

            if (swPain->GetDuration() < durationConsumed)
                swPain->Remove();
            else
                swPain->SetDuration(swPain->GetDuration() - durationConsumed);
        }

        if (Aura* purge = target->GetAura(SPELL_PRIEST_PURGE_THE_WICKED))
        {
            int32 remainingTick = purge->GetEffect(EFFECT_0)->GetRemaningTicks();
            int32 multi = std::min<int32>(durationConsumed / 2000, remainingTick);
            amount += purge->GetEffect(EFFECT_0)->GetAmount() * multi;

            if (purge->GetDuration() < durationConsumed)
                purge->Remove();
            else
                purge->SetDuration(purge->GetDuration() - durationConsumed);
        }

        if (amount == 0)
            return;

        caster->CastCustomSpell(RUNE_PRIEST_EXPIATION_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_expiation::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_expiation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_void_shield : public AuraScript
{
    PrepareAuraScript(rune_pri_void_shield);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Aura* shield = caster->GetAura(SPELL_PRIEST_POWER_WORD_SHIELD))
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            int32 increase = CalculatePct(damage, aurEff->GetAmount());
            int32 amount = shield->GetEffect(EFFECT_0)->GetAmount();
            LOG_ERROR("error", "amount = {}", amount);
            shield->GetEffect(EFFECT_0)->ChangeAmount(amount + increase);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_void_shield::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_void_shield::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_train_of_thought : public AuraScript
{
    PrepareAuraScript(rune_pri_train_of_thought);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;

        if (Player* player = caster->ToPlayer())
        {
            if (spellID == SPELL_PRIEST_FLASH_HEAL || spellID == SPELL_PRIEST_RENEW)
            {
                int32 cooldown = aurEff->GetAmount();
                player->ModifySpellCooldown(SPELL_PRIEST_POWER_WORD_SHIELD, -cooldown);
            }
            else if (spellID == SPELL_PRIEST_SMITE || spellID == SPELL_PRIEST_POWER_WORD_SOLACE)
            {
                int32 cooldown = GetAura()->GetEffect(EFFECT_1)->GetAmount();
                player->ModifySpellCooldown(SPELL_PRIEST_PENANCE, -cooldown);                
            }
        }     
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_train_of_thought::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_train_of_thought::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};



void AddSC_priest_perks_scripts()
{
    RegisterSpellScript(rune_pri_faded);
    RegisterSpellScript(rune_pri_armor_of_faith);
    RegisterSpellScript(rune_pri_protective_light);
    RegisterSpellScript(rune_pri_unwavering_will);
    RegisterSpellScript(rune_pri_binding_heals);
    RegisterSpellScript(rune_pri_burning_vehemence);
    RegisterSpellScript(rune_pri_burning_vehemence_target);
    RegisterSpellScript(rune_pri_shadowy_insight);
    RegisterSpellScript(rune_pri_insidious_ire);
    RegisterSpellScript(rune_pri_psychic_link);
    RegisterSpellScript(rune_pri_expiation);
    RegisterSpellScript(rune_pri_void_shield);
    RegisterSpellScript(rune_pri_train_of_thought);


    
    
}

