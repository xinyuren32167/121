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
    SPELL_PRIEST_APOTHEOSIS = 81030,
    SPELL_PRIEST_APOTHEOSIS_HOLY_FORM = 81031,
    SPELL_PRIEST_AUTONEMENT_AURA = 81010,
    SPELL_PRIEST_DEVOURING_PLAGUE = 48300,
    SPELL_PRIEST_FADE = 586,
    SPELL_PRIEST_FLASH_HEAL = 48071,
    SPELL_PRIEST_MIND_BLAST = 48127,
    SPELL_PRIEST_PAIN_SUPPRESSION = 33206,
    SPELL_PRIEST_PENANCE = 47540,
    SPELL_PRIEST_PENANCE_DAMAGE = 47666,
    SPELL_PRIEST_PENANCE_HEAL = 47750,
    SPELL_PRIEST_POWER_WORD_SHIELD = 48066,
    SPELL_PRIEST_POWER_WORD_SOLACE = 81016,
    SPELL_PRIEST_PRAYER_OF_MENDING = 48113,
    SPELL_PRIEST_PURGE_THE_WICKED = 81017,
    SPELL_PRIEST_RENEW = 48068,
    SPELL_PRIEST_SHADOW_WORD_DEATH = 48158,
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
    RUNE_PRIEST_ANSWERED_PRAYERS_LISTENER = 900492,
    RUNE_PRIEST_RENEWED_FAITH_HEAL = 900530,
    RUNE_PRIEST_PAINBREAKER_PSALM_DAMAGE = 900686,
    RUNE_PRIEST_BLESSED_RECOVERY_HOT = 900718,
    RUNE_PRIEST_DESPERATE_TIMES_HEAL = 900750,

    // Summons
    SUMMON_PRIEST_GOLDEN_APPARITION = 900001,
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

class rune_pri_revitalizing_prayers : public AuraScript
{
    PrepareAuraScript(rune_pri_revitalizing_prayers);

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

        if (Player* player = caster->ToPlayer())
        {
            int32 duration = aurEff->GetAmount();
            caster->AddAura(SPELL_PRIEST_RENEW, target);
            target->GetAura(SPELL_PRIEST_RENEW)->SetDuration(duration);
        }

    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_revitalizing_prayers::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_revitalizing_prayers::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_epiphany : public AuraScript
{
    PrepareAuraScript(rune_pri_epiphany);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
            player->RemoveSpellCooldown(SPELL_PRIEST_PRAYER_OF_MENDING, true);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_epiphany::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_epiphany::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_answered_prayers : public AuraScript
{
    PrepareAuraScript(rune_pri_answered_prayers);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        caster->CastSpell(caster, RUNE_PRIEST_ANSWERED_PRAYERS_LISTENER, TRIGGERED_FULL_MASK);

        if (Aura* listener = caster->GetAura(RUNE_PRIEST_ANSWERED_PRAYERS_LISTENER))
        {
            int32 threshold = aurEff->GetAmount();

            if (listener->GetStackAmount() < threshold)
                return;

            int32 duration = GetEffect(EFFECT_1)->GetAmount();

            if (Aura* apotheosisAura = caster->GetAura(SPELL_PRIEST_APOTHEOSIS_HOLY_FORM))
            {
                duration /= 2;
                duration += apotheosisAura->GetDuration();
                apotheosisAura->SetDuration(duration);
            }
            else
            {
                caster->AddAura(SPELL_PRIEST_APOTHEOSIS_HOLY_FORM, caster);
                caster->GetAura(SPELL_PRIEST_APOTHEOSIS_HOLY_FORM)->SetDuration(duration);
            }
            listener->Remove();
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_answered_prayers::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_answered_prayers::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_lasting_renovation : public AuraScript
{
    PrepareAuraScript(rune_pri_lasting_renovation);

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

        if (Aura* renew = target->GetAura(SPELL_PRIEST_RENEW))
        {
            int32 duration = renew->GetDuration() + aurEff->GetAmount();
            renew->SetDuration(duration);
            renew->GetEffect(EFFECT_0)->ResetTicks();
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_lasting_renovation::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_lasting_renovation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_renewed_faith : public AuraScript
{
    PrepareAuraScript(rune_pri_renewed_faith);

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

        if (!target->HasAura(SPELL_PRIEST_RENEW))
            return;

        int32 heal = eventInfo.GetHealInfo()->GetHeal();
        int32 amount = CalculatePct(heal, aurEff->GetAmount());

        if (amount == 0)
            return;

        caster->CastCustomSpell(RUNE_PRIEST_RENEWED_FAITH_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_renewed_faith::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_renewed_faith::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_catharstick : public AuraScript
{
    PrepareAuraScript(rune_pri_catharstick);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
        int32 healthThreshold = CalculatePct(caster->GetMaxHealth(), GetEffect(EFFECT_1)->GetAmount());
        int32 amount = std::min<int32>(GetEffect(EFFECT_2)->GetAmount() + damage, healthThreshold);

        GetEffect(EFFECT_2)->SetAmount(amount);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_catharstick::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_catharstick::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_deathspeaker : public AuraScript
{
    PrepareAuraScript(rune_pri_deathspeaker);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
            player->RemoveSpellCooldown(SPELL_PRIEST_SHADOW_WORD_DEATH, true);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_deathspeaker::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_deathspeaker::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_pri_painbreaker_psalm : public AuraScript
{
    PrepareAuraScript(rune_pri_painbreaker_psalm);

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

        if (!target->HasAura(SPELL_PRIEST_SHADOW_WORD_PAIN) && !target->HasAura(SPELL_PRIEST_VAMPIRIC_TOUCH))
            return;

        int32 amount = 0;

        if (Aura* swPain = target->GetAura(SPELL_PRIEST_SHADOW_WORD_PAIN))
        {
            int32 remainingTick = swPain->GetEffect(EFFECT_0)->GetRemaningTicks();
            amount += swPain->GetEffect(EFFECT_0)->GetAmount() * remainingTick;
            swPain->Remove();
        }

        if (Aura* vampiric = target->GetAura(SPELL_PRIEST_VAMPIRIC_TOUCH))
        {
            int32 remainingTick = vampiric->GetEffect(EFFECT_0)->GetRemaningTicks();
            amount += vampiric->GetEffect(EFFECT_0)->GetAmount() * remainingTick;
            vampiric->Remove();
        }

        if (amount == 0)
            return;

        ApplyPct(amount, aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_PRIEST_PAINBREAKER_PSALM_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_painbreaker_psalm::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_painbreaker_psalm::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_blessed_recovery : public AuraScript
{
    PrepareAuraScript(rune_pri_blessed_recovery);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
        amount /= 6;

        caster->CastCustomSpell(RUNE_PRIEST_BLESSED_RECOVERY_HOT, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_blessed_recovery::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_blessed_recovery::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_twist_of_fate : public AuraScript
{
    PrepareAuraScript(rune_pri_twist_of_fate);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = nullptr;

        if (eventInfo.GetHealInfo())
        {
            target = eventInfo.GetHealInfo()->GetTarget();

            if (eventInfo.GetHealInfo()->GetHeal() <= 0)
                return;
        }
        else if (eventInfo.GetDamageInfo())
        {
            target = eventInfo.GetDamageInfo()->GetVictim();

            if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
                return;
        }

        if (!target || target->isDead())
            return;

        int32 healthThreshold = aurEff->GetAmount();

        if (target->GetHealthPct() > healthThreshold)
            return;

        int32 procSpell = GetEffect(EFFECT_1)->GetAmount();

        caster->AddAura(procSpell, caster);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pri_twist_of_fate::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_desperate_times : public AuraScript
{
    PrepareAuraScript(rune_pri_desperate_times);

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

        int32 healthThreshold = GetEffect(EFFECT_1)->GetAmount();

        if (target->GetHealthPct() > healthThreshold)
            return;

        int32 amount = CalculatePct(eventInfo.GetHealInfo()->GetHeal(), aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_PRIEST_DESPERATE_TIMES_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_desperate_times::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_desperate_times::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_golden_apparitions : public AuraScript
{
    PrepareAuraScript(rune_pri_golden_apparitions);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        int32 apparitionNbr = aurEff->GetAmount();

        if (eventInfo.GetHitMask() == PROC_EX_CRITICAL_HIT)
            apparitionNbr = GetEffect(EFFECT_1)->GetAmount();

        if (Player* player = caster->ToPlayer())
        {
            auto const& allyList = player->GetGroup()->GetMemberSlots();

            for (auto const& target : allyList)
            {
                Player* alliedPlayer = ObjectAccessor::FindPlayer(target.guid);

                if (alliedPlayer->isDead() || !alliedPlayer->HasAura(SPELL_PRIEST_RENEW))
                    continue;

                int32 apparition = apparitionNbr;

                while (apparition > 0)
                {
                    Position pos = caster->GetNearPosition(urand(1, 5), urand(1, 5));
                    SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
                    Creature* summon = caster->SummonCreature(SUMMON_PRIEST_GOLDEN_APPARITION, pos, TEMPSUMMON_MANUAL_DESPAWN, GetSpellInfo()->GetDuration(), 0, properties);

                    if (!summon)
                        continue;

                    summon->SetOwnerGUID(caster->GetGUID());
                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    summon->SetReactState(REACT_PASSIVE);
                    summon->SetTarget(alliedPlayer->GetGUID());

                    apparition--;
                }
            }
        }

    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pri_golden_apparitions::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// Golden Apparition Pet behaviour
class npc_pri_golden_apparitions : public CreatureScript
{
    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 900756; i < 900762; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

public:
    npc_pri_golden_apparitions() : CreatureScript("npc_pri_golden_apparitions") { }

    struct spell_pri_golden_apparitionsAI : public ScriptedAI
    {
        spell_pri_golden_apparitionsAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 update = 250;

        void Reset() override
        {
            me->CombatStop(true);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(uint32 diff) override
        {
            if (update >= 250) {
                if (Unit* target = ObjectAccessor::GetCreature(*me, me->GetTarget()))
                {
                    Position pos = target->GetPosition();
                    me->GetMotionMaster()->MovePoint(0, pos);
                }
                update = 0;
            }

            update += diff;
        }

        void MovementInform(uint32 /*type*/, uint32 id) override
        {
            Unit* caster = me->GetOwner();

            /*if (Aura* rune = GetRuneAura(caster))
                if (id == 0) {
                    if (Unit* target = ObjectAccessor::GetCreature(*me, me->GetTarget()))
                    {
                        int32 procSpell = rune->GetEffect(EFFECT_2)->GetAmount();
                        me->CastSpell(target, procSpell, TRIGGERED_FULL_MASK, nullptr, nullptr, me->GetOwnerGUID());
                    }

                    me->DespawnOrUnsummon();
                }*/
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new spell_pri_golden_apparitionsAI(creature);
    }
};

class rune_pri_protector_of_the_frail : public AuraScript
{
    PrepareAuraScript(rune_pri_protector_of_the_frail);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_PRIEST_PAIN_SUPPRESSION, -cooldown);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_protector_of_the_frail::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_protector_of_the_frail::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_weal_and_woe : public AuraScript
{
    PrepareAuraScript(rune_pri_weal_and_woe);

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
        int32 procSpell = 0;

        if (spellID == SPELL_PRIEST_PENANCE_HEAL)
            procSpell = aurEff->GetAmount();
        else if (spellID == SPELL_PRIEST_PENANCE_DAMAGE)
            procSpell = GetEffect(EFFECT_1)->GetAmount();

        caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_weal_and_woe::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_weal_and_woe::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_contemptuous_homily : public AuraScript
{
    PrepareAuraScript(rune_pri_contemptuous_homily);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = nullptr;

        if (eventInfo.GetHealInfo())
        {
            target = eventInfo.GetHealInfo()->GetTarget();

            if (eventInfo.GetHealInfo()->GetHeal() <= 0)
                return;
        }
        else if (eventInfo.GetDamageInfo())
        {
            target = eventInfo.GetDamageInfo()->GetVictim();

            if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
                return;
        }

        if (!target || target->isDead())
            return;

        int32 durationIncrease = aurEff->GetAmount();

        if (Aura* swPain = target->GetAura(SPELL_PRIEST_SHADOW_WORD_PAIN))
        {
            int32 duration = swPain->GetDuration() + durationIncrease;
            swPain->SetDuration(duration);
            swPain->GetEffect(EFFECT_0)->ResetTicks();
        }

        if (Aura* renew = target->GetAura(SPELL_PRIEST_RENEW))
        {
            int32 duration = renew->GetDuration() + durationIncrease;
            renew->SetDuration(duration);
            renew->GetEffect(EFFECT_0)->ResetTicks();
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pri_contemptuous_homily::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_blaze_of_light : public AuraScript
{
    PrepareAuraScript(rune_pri_blaze_of_light);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = nullptr;
        int32 procSpell = 0;

        if (eventInfo.GetHealInfo())
        {
            target = eventInfo.GetHealInfo()->GetTarget();
            procSpell = GetEffect(EFFECT_1)->GetAmount();

            if (eventInfo.GetHealInfo()->GetHeal() <= 0)
                return;
        }
        else if (eventInfo.GetDamageInfo())
        {
            target = eventInfo.GetDamageInfo()->GetVictim();
            procSpell = aurEff->GetAmount();

            if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
                return;
        }

        if (!target || target->isDead())
            return;

        if (procSpell == 0)
            return;

        caster->AddAura(procSpell, target);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pri_blaze_of_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_contrition : public AuraScript
{
    PrepareAuraScript(rune_pri_contrition);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procSpell = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
        {
            auto const& allyList = player->GetGroup()->GetMemberSlots();

            if (caster->HasAura(SPELL_PRIEST_AUTONEMENT_AURA))
                caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);

            if (!player->GetGroup())
                return;

            if (allyList.size() <= 0)
                return;

            for (auto const& target : allyList)
            {
                Player* alliedPlayer = ObjectAccessor::FindPlayer(target.guid);

                if (alliedPlayer->isDead() || !alliedPlayer->HasAura(SPELL_PRIEST_AUTONEMENT_AURA))
                    continue;

                caster->CastSpell(alliedPlayer, procSpell, TRIGGERED_FULL_MASK); 
            }
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pri_contrition::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
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
    RegisterSpellScript(rune_pri_revitalizing_prayers);
    RegisterSpellScript(rune_pri_epiphany);
    RegisterSpellScript(rune_pri_answered_prayers);
    RegisterSpellScript(rune_pri_lasting_renovation);
    RegisterSpellScript(rune_pri_renewed_faith);
    RegisterSpellScript(rune_pri_catharstick);
    RegisterSpellScript(rune_pri_deathspeaker);
    RegisterSpellScript(rune_pri_painbreaker_psalm);
    RegisterSpellScript(rune_pri_blessed_recovery);
    RegisterSpellScript(rune_pri_twist_of_fate);
    RegisterSpellScript(rune_pri_desperate_times);
    RegisterSpellScript(rune_pri_golden_apparitions);
    RegisterSpellScript(rune_pri_protector_of_the_frail);
    RegisterSpellScript(rune_pri_weal_and_woe);
    RegisterSpellScript(rune_pri_contemptuous_homily);
    RegisterSpellScript(rune_pri_blaze_of_light);
    RegisterSpellScript(rune_pri_contrition);


    
    


    new npc_pri_golden_apparitions();
}

