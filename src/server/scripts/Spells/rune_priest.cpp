#include "PetDefines.h"
#include "Player.h"
#include "Pet.h"
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
    SPELL_PRIEST_GUARDIAN_SPIRIT = 47788,
    SPELL_PRIEST_HOLY_BINDING = 86202,
    SPELL_PRIEST_HOLY_BURST_BUFF = 86281,
    SPELL_PRIEST_HOLY_MIGHT = 86208,
    SPELL_PRIEST_HOLY_MIGHT_STRENGTH = 86209,
    SPELL_PRIEST_HOLY_MIGHT_AGILITY = 86210,
    SPELL_PRIEST_HOLY_MIGHT_INTELLECT = 86211,
    SPELL_PRIEST_HOLY_MIGHT_SPIRIT = 86212,
    SPELL_PRIEST_INSANITY_ENERGIZE = 81093,
    SPELL_PRIEST_MIND_BLAST = 48127,
    SPELL_PRIEST_MIND_FLAY = 48156,
    SPELL_PRIEST_MIND_DAMAGE = 48156,
    SPELL_PRIEST_MIND_SPIKE = 81048,
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
    SPELL_PRIEST_UPHEAVAL = 86219,
    SPELL_PRIEST_VAMPIRIC_TOUCH = 48160,
    SPELL_PRIEST_VOID_TORRENT = 81049,

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
    RUNE_PRIEST_POWERFUL_SHADOWFIEND_DAMAGE = 900556,
    RUNE_PRIEST_PUTREFYING_CLAWS_DOT = 900588,
    RUNE_PRIEST_PUTREFYING_CLAWS_DAMAGE = 900589,
    RUNE_PRIEST_PAINBREAKER_PSALM_DAMAGE = 900686,
    RUNE_PRIEST_BLESSED_RECOVERY_HOT = 900718,
    RUNE_PRIEST_DESPERATE_TIMES_HEAL = 900750,
    RUNE_PRIEST_PHYRINXS_EMBRACE_HEAL = 901038,
    RUNE_PRIEST_GOLDEN_APPARITION_VISUAL = 901039,
    RUNE_PRIEST_TRAIL_OF_LIGHT_HEAL = 901046,
    RUNE_PRIEST_TRAIL_OF_LIGHT_LISTENER = 901047,
    RUNE_PRIEST_IDOL_OF_CTHUN_MIND_FLAY = 901224,
    RUNE_PRIEST_IDOL_OF_CTHUN_MIND_FLAY_DAMAGE = 901225,
    RUNE_PRIEST_IDOL_OF_CTHUN_MIND_SEAR = 901226,
    RUNE_PRIEST_IDOL_OF_CTHUN_MIND_SEAR_DAMAGE = 901227,
    RUNE_PRIEST_UNFURLING_DARKNESS_DEBUFF = 901282,
    RUNE_PRIEST_IDOL_OF_NZOTH_ECHOING = 901302,
    RUNE_PRIEST_IDOL_OF_NZOTH_PERIODIC = 901303,
    RUNE_PRIEST_IDOL_OF_NZOTH_DAMAGE = 901304,
    RUNE_PRIEST_VELENS_APPRENTICE_DAMAGE = 901508,
    RUNE_PRIEST_FATE_MIRROR_DAMAGE = 901564,
    RUNE_PRIEST_FATE_MIRROR_HEAL = 901565,
    RUNE_PRIEST_LIGHTS_PROTECTION_DOT = 901629,

    // Summons
    SUMMON_PRIEST_SHADOW_FIEND = 19668,
    SUMMON_PRIEST_GOLDEN_APPARITION = 900001,
    SUMMON_PRIEST_IDOL_OF_CTHUN_VOID_TENDRIL = 900002,
    SUMMON_PRIEST_IDOL_OF_CTHUN_VOID_LASHER = 900003,
    SUMMON_PRIEST_IDOL_YOGG_SARON_THING_FROM_BEYOND = 900004,
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

class rune_pri_powerful_shadowfiend : public AuraScript
{
    PrepareAuraScript(rune_pri_powerful_shadowfiend);

    Unit* GetShadowFiend(Unit* caster)
    {
        Unit* controlledUnit = nullptr;
        if (Player* player = caster->ToPlayer())
            for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                if (Unit* pet = (*itr))
                    if (pet->IsAlive() &&
                        pet->GetOwnerGUID() == player->GetGUID() &&
                        pet->GetEntry() == SUMMON_PRIEST_SHADOW_FIEND &&
                        pet->IsWithinDist(player, 100.0f, false))
                        controlledUnit = pet;

        return controlledUnit;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = GetShadowFiend(caster);

        caster->AddAura(aurEff->GetAmount(), shadowFiend);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = GetShadowFiend(caster);

        for (size_t i = 900550; i < 900556; i++)
            if (shadowFiend->HasAura(i))
                shadowFiend->RemoveAura(i);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_pri_powerful_shadowfiend::HandleApply, EFFECT_1, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_pri_powerful_shadowfiend::HandleRemove, EFFECT_1, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pri_powerful_shadowfiend_damage : public AuraScript
{
    PrepareAuraScript(rune_pri_powerful_shadowfiend_damage);

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

        caster->CastCustomSpell(RUNE_PRIEST_POWERFUL_SHADOWFIEND_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_powerful_shadowfiend_damage::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_powerful_shadowfiend_damage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_fiend_friend : public AuraScript
{
    PrepareAuraScript(rune_pri_fiend_friend);

    Unit* GetShadowFiend(Unit* caster)
    {
        Unit* controlledUnit = nullptr;
        if (Player* player = caster->ToPlayer())
            for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                if (Unit* pet = (*itr))
                    if (pet->IsAlive() &&
                        pet->GetOwnerGUID() == player->GetGUID() &&
                        pet->GetEntry() == SUMMON_PRIEST_SHADOW_FIEND &&
                        pet->IsWithinDist(player, 100.0f, false))
                        controlledUnit = pet;

        return controlledUnit;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = GetShadowFiend(caster);

        if (Creature* shadowF = shadowFiend->ToCreature())
            shadowF->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_pri_fiend_friend::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pri_rabid_shadows : public AuraScript
{
    PrepareAuraScript(rune_pri_rabid_shadows);

    Unit* GetShadowFiend(Unit* caster)
    {
        Unit* controlledUnit = nullptr;
        if (Player* player = caster->ToPlayer())
            for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                if (Unit* pet = (*itr))
                    if (pet->IsAlive() &&
                        pet->GetOwnerGUID() == player->GetGUID() &&
                        pet->GetEntry() == SUMMON_PRIEST_SHADOW_FIEND &&
                        pet->IsWithinDist(player, 100.0f, false))
                        controlledUnit = pet;

        return controlledUnit;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = GetShadowFiend(caster);

        caster->AddAura(aurEff->GetAmount(), shadowFiend);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = GetShadowFiend(caster);

        for (size_t i = 900570; i < 900576; i++)
            if (shadowFiend->HasAura(i))
                shadowFiend->RemoveAura(i);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_pri_rabid_shadows::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_pri_rabid_shadows::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pri_putrefying_claws : public AuraScript
{
    PrepareAuraScript(rune_pri_putrefying_claws);

    Unit* GetShadowFiend(Unit* caster)
    {
        Unit* controlledUnit = nullptr;
        if (Player* player = caster->ToPlayer())
            for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                if (Unit* pet = (*itr))
                    if (pet->IsAlive() &&
                        pet->GetOwnerGUID() == player->GetGUID() &&
                        pet->GetEntry() == SUMMON_PRIEST_SHADOW_FIEND &&
                        pet->IsWithinDist(player, 100.0f, false))
                        controlledUnit = pet;

        return controlledUnit;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = GetShadowFiend(caster);

        if(shadowFiend) {
            caster->AddAura(aurEff->GetAmount(), shadowFiend);
        }

    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = GetShadowFiend(caster);

        if (shadowFiend) {
            for (size_t i = 900582; i < 900588; i++)
                if (shadowFiend->HasAura(i))
                    shadowFiend->RemoveAura(i);

        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_pri_putrefying_claws::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_pri_putrefying_claws::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pri_putrefying_claws_damage : public AuraScript
{
    PrepareAuraScript(rune_pri_putrefying_claws_damage);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        if (Aura* dot = target->GetAura(RUNE_PRIEST_PUTREFYING_CLAWS_DOT))
            if (dot->GetStackAmount() == 5)
                caster->CastSpell(target, RUNE_PRIEST_PUTREFYING_CLAWS_DAMAGE, TRIGGERED_FULL_MASK);

        caster->CastSpell(target, RUNE_PRIEST_PUTREFYING_CLAWS_DOT, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_putrefying_claws_damage::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_putrefying_claws_damage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_essence_devourer : public AuraScript
{
    PrepareAuraScript(rune_pri_essence_devourer);

    Unit* GetShadowFiend(Unit* caster)
    {
        Unit* controlledUnit = nullptr;
        if (Player* player = caster->ToPlayer())
            for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                if (Unit* pet = (*itr))
                    if (pet->IsAlive() &&
                        pet->GetOwnerGUID() == player->GetGUID() &&
                        pet->GetEntry() == SUMMON_PRIEST_SHADOW_FIEND &&
                        pet->IsWithinDist(player, 100.0f, false))
                        controlledUnit = pet;

        return controlledUnit;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = GetShadowFiend(caster);

        caster->AddAura(aurEff->GetAmount(), shadowFiend);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = GetShadowFiend(caster);

        for (size_t i = 900596; i < 900602; i++)
            if (shadowFiend->HasAura(i))
                shadowFiend->RemoveAura(i);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_pri_essence_devourer::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_pri_essence_devourer::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pri_essence_devourer_proc : public AuraScript
{
    PrepareAuraScript(rune_pri_essence_devourer_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 procSpell = aurEff->GetAmount();

            if (!player->GetGroup())
            {
                caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
                return;
            }

            auto const& allyList = player->GetGroup()->GetMemberSlots();

            if (allyList.size() > 0)
                for (auto const& target : allyList)
                {
                    Player* alliedPlayer = ObjectAccessor::FindPlayer(target.guid);

                    caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
                    return;
                }
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_essence_devourer_proc::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_essence_devourer_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_shadowflame_prism : public AuraScript
{
    PrepareAuraScript(rune_pri_shadowflame_prism);

    Unit* GetShadowFiend(Unit* caster)
    {
        Unit* controlledUnit = nullptr;
        if (Player* player = caster->ToPlayer())
            for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                if (Unit* pet = (*itr))
                    if (pet->IsAlive() &&
                        pet->GetOwnerGUID() == player->GetGUID() &&
                        pet->GetEntry() == SUMMON_PRIEST_SHADOW_FIEND &&
                        pet->IsWithinDist(player, 100.0f, false))
                        controlledUnit = pet;

        return controlledUnit;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = GetShadowFiend(caster);

        caster->AddAura(aurEff->GetAmount(), shadowFiend);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_shadowflame_prism::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_shadowflame_prism::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_shadowflame_prism_proc : public AuraScript
{
    PrepareAuraScript(rune_pri_shadowflame_prism_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* shadowFiend = eventInfo.GetDamageInfo()->GetAttacker();

        if (!shadowFiend || shadowFiend->isDead())
            return;

        int32 procSpell = aurEff->GetAmount();

        if (shadowFiend && shadowFiend->IsAlive())
            caster->CastSpell(shadowFiend, procSpell, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_shadowflame_prism_proc::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_shadowflame_prism_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
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
                    Creature* summon = caster->SummonCreature(SUMMON_PRIEST_GOLDEN_APPARITION, pos, TEMPSUMMON_MANUAL_DESPAWN, 30000, 0, properties);

                    if (!summon)
                        continue;

                    caster->AddAura(RUNE_PRIEST_GOLDEN_APPARITION_VISUAL, summon);
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

class rune_pri_make_amends : public AuraScript
{
    PrepareAuraScript(rune_pri_make_amends);

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

            if (eventInfo.GetHealInfo()->GetHeal() <= 0)
                return;
        }
        else if (eventInfo.GetDamageInfo())
        {
            target = caster;

            if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
                return;
        }

        if (!target || target->isDead())
            return;

        if (Aura* atonement = target->GetAura(SPELL_PRIEST_AUTONEMENT_AURA))
        {
            int32 duration = atonement->GetDuration();
            duration = std::min<int32>(duration + aurEff->GetAmount(), atonement->GetMaxDuration());
            atonement->SetDuration(duration);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_pri_make_amends::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_phyrinxs_embrace : public AuraScript
{
    PrepareAuraScript(rune_pri_phyrinxs_embrace);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 heal = eventInfo.GetHealInfo()->GetHeal();
        int32 amount = CalculatePct(heal, aurEff->GetAmount());
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            Group* group = player->GetGroup();

            if (group)
            {
                auto const& allyList = player->GetGroup()->GetMemberSlots();

                for (auto const& ally : allyList)
                {
                    Player* alliedPlayer = ObjectAccessor::FindPlayer(ally.guid);

                    if (alliedPlayer->GetGUID() == target->GetGUID())
                        continue;

                    if (alliedPlayer->GetGUID() == caster->GetGUID())
                        continue;

                    if (alliedPlayer->isDead() || !alliedPlayer->HasAura(SPELL_PRIEST_GUARDIAN_SPIRIT))
                        continue;

                    caster->CastCustomSpell(RUNE_PRIEST_PHYRINXS_EMBRACE_HEAL, SPELLVALUE_BASE_POINT0, amount, alliedPlayer, TRIGGERED_FULL_MASK);
                }
            }

            if (caster->HasAura(SPELL_PRIEST_GUARDIAN_SPIRIT) && target->GetGUID() != caster->GetGUID())
                caster->CastCustomSpell(RUNE_PRIEST_PHYRINXS_EMBRACE_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_phyrinxs_embrace::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_phyrinxs_embrace::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_trail_of_light : public AuraScript
{
    PrepareAuraScript(rune_pri_trail_of_light);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 heal = eventInfo.GetHealInfo()->GetHeal();
        int32 amount = CalculatePct(heal, aurEff->GetAmount());
        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        if (target->HasAura(RUNE_PRIEST_TRAIL_OF_LIGHT_LISTENER))
            return;

        Unit* oldTarget = target;

        if (Player* player = caster->ToPlayer())
        {
            Group* group = player->GetGroup();

            if (group)
            {
                auto const& allyList = player->GetGroup()->GetMemberSlots();

                for (auto const& ally : allyList)
                {
                    Player* alliedPlayer = ObjectAccessor::FindPlayer(ally.guid);

                    if (alliedPlayer->HasAura(RUNE_PRIEST_TRAIL_OF_LIGHT_LISTENER))
                        oldTarget = alliedPlayer;
                }

                if (group->IsMember(target->GetGUID()))
                {
                    caster->AddAura(RUNE_PRIEST_TRAIL_OF_LIGHT_LISTENER, target);

                    if (oldTarget != target)
                        oldTarget->RemoveAura(RUNE_PRIEST_TRAIL_OF_LIGHT_LISTENER);
                }

                if (oldTarget == target || oldTarget->isDead())
                    return;

                caster->CastCustomSpell(RUNE_PRIEST_TRAIL_OF_LIGHT_HEAL, SPELLVALUE_BASE_POINT0, amount, oldTarget, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_trail_of_light::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_trail_of_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_screams_of_the_void : public AuraScript
{
    PrepareAuraScript(rune_pri_screams_of_the_void);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        auto const& threatList = caster->getAttackers();

        for (auto const& targets : threatList)
            if (targets->IsAlive())
            {
                if (Aura* vampiric = targets->GetAura(SPELL_PRIEST_VAMPIRIC_TOUCH))
                {
                    vampiric->GetEffect(EFFECT_0)->ResetTicks();
                    vampiric->GetEffect(EFFECT_0)->CalculatePeriodic(caster);
                }

                if (Aura* swPain = targets->GetAura(SPELL_PRIEST_SHADOW_WORD_PAIN))
                {
                    swPain->GetEffect(EFFECT_0)->ResetTicks();
                    swPain->GetEffect(EFFECT_0)->CalculatePeriodic(caster);
                }
            }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        auto const& threatList = caster->getAttackers();

        for (auto const& targets : threatList)
            if (targets->IsAlive())
            {
                if (Aura* vampiric = targets->GetAura(SPELL_PRIEST_VAMPIRIC_TOUCH))
                {
                    vampiric->GetEffect(EFFECT_0)->ResetTicks();
                    vampiric->GetEffect(EFFECT_0)->CalculatePeriodic(caster);
                }

                if (Aura* swPain = targets->GetAura(SPELL_PRIEST_SHADOW_WORD_PAIN))
                {
                    swPain->GetEffect(EFFECT_0)->ResetTicks();
                    swPain->GetEffect(EFFECT_0)->CalculatePeriodic(caster);
                }
            }
    }

    void Register()
    {
        AfterEffectApply += AuraEffectApplyFn(rune_pri_screams_of_the_void::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(rune_pri_screams_of_the_void::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pri_idol_of_cthun : public AuraScript
{
    PrepareAuraScript(rune_pri_idol_of_cthun);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetProcTarget();

        if (!target || target->isDead())
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;

        if (spellID != SPELL_PRIEST_MIND_FLAY && spellID != SPELL_PRIEST_MIND_SPIKE && spellID != SPELL_PRIEST_VOID_TORRENT)
            return;

        auto const& threatList = caster->getAttackers();
        int32 targetNbr = 0;

        for (auto const& targets : threatList)
            if (targets->IsAlive())
            {
                if (target == targets)
                    continue;

                int32 distance = target->GetDistance(targets);

                if (distance < 10)
                    targetNbr++;
            }

        int32 summonedCreature = SUMMON_PRIEST_IDOL_OF_CTHUN_VOID_TENDRIL;

        if (targetNbr >= 2)
            summonedCreature = SUMMON_PRIEST_IDOL_OF_CTHUN_VOID_LASHER;

        Position pos = caster->GetNearPosition(urand(1, 8), urand(1, 8));
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = caster->SummonCreature(summonedCreature, pos, TEMPSUMMON_TIMED_DESPAWN, 16000, 0, properties);

        if (!summon)
            return;

        summon->SetOwnerGUID(caster->GetGUID());
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        summon->ClearUnitState(UNIT_STATE_ROOT);
        summon->SetControlled(true, UNIT_STATE_ROOT);
        summon->SetTarget(target->GetGUID());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_idol_of_cthun::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_idol_of_cthun::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_idol_of_cthun_damage : public AuraScript
{
    PrepareAuraScript(rune_pri_idol_of_cthun_damage);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* pet = GetCaster();

        if (!pet || pet->isDead())
            return;

        Unit* player = pet->GetOwner();

        if (!player || player->isDead())
            return;

        Unit* target = ObjectAccessor::GetUnit(*pet, pet->GetTarget());

        if (!target || target->isDead())
            return;

        int32 spellID = GetSpellInfo()->Id;
        int32 procSpell = GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
        int32 insanityAmount = GetEffect(EFFECT_0)->GetAmount();

        player->CastCustomSpell(SPELL_PRIEST_INSANITY_ENERGIZE, SPELLVALUE_BASE_POINT0, insanityAmount, player, TRIGGERED_FULL_MASK);
        player->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_pri_idol_of_cthun_damage::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_pri_mental_decay : public AuraScript
{
    PrepareAuraScript(rune_pri_mental_decay);

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

        if (!eventInfo.GetSpellInfo())
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;
        int32 durationIncrease = aurEff->GetAmount();

        if (spellID == SPELL_PRIEST_MIND_SPIKE)
            durationIncrease *= 2;

        if (Aura* vampiric = target->GetAura(SPELL_PRIEST_VAMPIRIC_TOUCH))
        {
            int32 duration = std::min<int32>(vampiric->GetDuration() + durationIncrease, vampiric->GetMaxDuration());
            vampiric->SetDuration(duration);
            vampiric->GetEffect(EFFECT_0)->ResetTicks();
        }

        if (Aura* swPain = target->GetAura(SPELL_PRIEST_SHADOW_WORD_PAIN))
        {
            int32 duration = std::min<int32>(swPain->GetDuration() + durationIncrease, swPain->GetMaxDuration());
            swPain->SetDuration(duration);
            swPain->GetEffect(EFFECT_0)->ResetTicks();
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_mental_decay::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_mental_decay::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_dark_evangelism : public AuraScript
{
    PrepareAuraScript(rune_pri_dark_evangelism);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_dark_evangelism::CheckProc);
    }
};

class rune_pri_unfurling_darkness : public AuraScript
{
    PrepareAuraScript(rune_pri_unfurling_darkness);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return false;

        return !caster->HasAura(RUNE_PRIEST_UNFURLING_DARKNESS_DEBUFF);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_unfurling_darkness::CheckProc);
    }
};

class rune_pri_idol_of_nzoth : public AuraScript
{
    PrepareAuraScript(rune_pri_idol_of_nzoth);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        int32 procChance = GetEffect(EFFECT_1)->GetAmount();

        if (!roll_chance_i(procChance))
            return;

        caster->AddAura(RUNE_PRIEST_IDOL_OF_NZOTH_PERIODIC, target);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        int32 stack = aurEff->GetAmount();

        while (stack > 0)
        {
            caster->CastSpell(target, RUNE_PRIEST_IDOL_OF_NZOTH_DAMAGE, TRIGGERED_FULL_MASK);
            stack--;
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        if (target->HasAura(RUNE_PRIEST_IDOL_OF_NZOTH_PERIODIC))
            target->RemoveAura(RUNE_PRIEST_IDOL_OF_NZOTH_PERIODIC);
    }

    void Register()
    {
        AfterEffectApply += AuraEffectApplyFn(rune_pri_idol_of_nzoth::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        OnEffectProc += AuraEffectProcFn(rune_pri_idol_of_nzoth::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_pri_idol_of_nzoth::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pri_idol_of_nzoth_periodic : public AuraScript
{
    PrepareAuraScript(rune_pri_idol_of_nzoth_periodic);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        if (Aura* echoing = target->GetAura(RUNE_PRIEST_IDOL_OF_NZOTH_ECHOING))
        {
            caster->CastSpell(target, RUNE_PRIEST_IDOL_OF_NZOTH_DAMAGE, TRIGGERED_FULL_MASK);
            echoing->ModStackAmount(-1);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_pri_idol_of_nzoth_periodic::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_pri_whispering_shadows : public AuraScript
{
    PrepareAuraScript(rune_pri_whispering_shadows);

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

        caster->CastSpell(target, SPELL_PRIEST_VAMPIRIC_TOUCH, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_whispering_shadows::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_whispering_shadows::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_idol_of_yogg_saron : public AuraScript
{
    PrepareAuraScript(rune_pri_idol_of_yogg_saron);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 901366; i < 901372; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetTarget();

        if (!target || target->isDead())
            return;

        int32 stacks = GetStackAmount();

        if (!GetRuneAura(caster))
        {
            ModStackAmount(-stacks);
            return;
        }

        if (stacks < GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount())
            return;

        Position pos = caster->GetNearPosition(urand(1, 5), urand(1, 5));
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = caster->SummonCreature(SUMMON_PRIEST_IDOL_YOGG_SARON_THING_FROM_BEYOND, pos, TEMPSUMMON_TIMED_DESPAWN, 20500, 0, properties);

        if (!summon)
            return;

        summon->SetOwnerGUID(caster->GetGUID());
        summon->SetReactState(REACT_DEFENSIVE);
        summon->ClearUnitState(UNIT_STATE_ROOT);
        summon->SetControlled(true, UNIT_STATE_ROOT);
        summon->SetTarget(target->GetGUID());

        ModStackAmount(-stacks);
    }

    void Register()
    {
        AfterEffectApply += AuraEffectApplyFn(rune_pri_idol_of_yogg_saron::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

class rune_pri_ancient_madness : public AuraScript
{
    PrepareAuraScript(rune_pri_ancient_madness);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 901410; i < 901416; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Aura* runeAura = GetRuneAura(caster))
        {
            int32 buffAura = runeAura->GetEffect(EFFECT_0)->GetAmount();
            int32 stackAmount = runeAura->GetEffect(EFFECT_2)->GetAmount();

            if (caster->HasAura(buffAura))
                caster->RemoveAura(buffAura);

            caster->AddAura(buffAura, caster);
            caster->GetAura(buffAura)->SetStackAmount(stackAmount);
        }
    }

    void Register()
    {
        AfterEffectApply += AuraEffectApplyFn(rune_pri_ancient_madness::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_pri_ancient_madness_periodic : public AuraScript
{
    PrepareAuraScript(rune_pri_ancient_madness_periodic);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procSpell = GetEffect(EFFECT_0)->GetAmount();
        int32 stackReduction = aurEff->GetAmount();

        if (Aura* buffAura = caster->GetAura(procSpell))
            buffAura->ModStackAmount(-stackReduction);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_pri_ancient_madness_periodic::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_pri_holymancy : public AuraScript
{
    PrepareAuraScript(rune_pri_holymancy);

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
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_PRIEST_HOLY_BINDING, -cooldown);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_holymancy::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_holymancy::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_accretion : public AuraScript
{
    PrepareAuraScript(rune_pri_accretion);

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
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_PRIEST_UPHEAVAL, -cooldown);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_accretion::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_accretion::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_velens_apprentice : public AuraScript
{
    PrepareAuraScript(rune_pri_velens_apprentice);

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

        caster->CastCustomSpell(RUNE_PRIEST_VELENS_APPRENTICE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_velens_apprentice::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_velens_apprentice::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_velens_apprentice_targeting : public SpellScript
{
    PrepareSpellScript(rune_pri_velens_apprentice_targeting);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 901502; i < 901508; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetExplTargetUnit();
        targets.remove(target);

        if (!GetRuneAura(caster))
            return;

        int32 maxTargets = GetRuneAura(caster)->GetEffect(EFFECT_1)->GetAmount();

        if (targets.size() > maxTargets)
            targets.resize(maxTargets);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_pri_velens_apprentice_targeting::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class rune_pri_velens_blessing : public AuraScript
{
    PrepareAuraScript(rune_pri_velens_blessing);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procSpell = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
        {
            auto const& allyList = player->GetGroup()->GetMemberSlots();

            if (caster->HasAura(SPELL_PRIEST_HOLY_MIGHT))
                caster->AddAura(procSpell, caster);

            if (player->GetGroup() && allyList.size() > 0)
                for (auto const& target : allyList)
                {
                    Player* alliedPlayer = ObjectAccessor::FindPlayer(target.guid);

                    if (alliedPlayer->isDead())
                        continue;

                    if (alliedPlayer->HasAura(SPELL_PRIEST_HOLY_MIGHT_STRENGTH))
                        caster->AddAura(procSpell, alliedPlayer);

                    if (alliedPlayer->HasAura(SPELL_PRIEST_HOLY_MIGHT_AGILITY))
                        caster->AddAura(procSpell, alliedPlayer);

                    if (alliedPlayer->HasAura(SPELL_PRIEST_HOLY_MIGHT_INTELLECT))
                        caster->AddAura(procSpell, alliedPlayer);

                    if (alliedPlayer->HasAura(SPELL_PRIEST_HOLY_MIGHT_SPIRIT))
                        caster->AddAura(procSpell, alliedPlayer);
                }
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_velens_blessing::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_velens_blessing::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_velens_blessing_proc : public AuraScript
{
    PrepareAuraScript(rune_pri_velens_blessing_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* owner = GetUnitOwner();

        if (!owner || owner->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 procSpell = aurEff->GetAmount();
        caster->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_velens_blessing_proc::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_velens_blessing_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_pri_fate_mirror : public AuraScript
{
    PrepareAuraScript(rune_pri_fate_mirror);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* owner = GetUnitOwner();

        if (!owner || owner->isDead())
            return;

        int32 amount = 0;
        int32 procSpell = 0;
        Unit* target;

        if (eventInfo.GetDamageInfo())
        {
            amount = eventInfo.GetDamageInfo()->GetDamage();
            procSpell = RUNE_PRIEST_FATE_MIRROR_DAMAGE;
            target = eventInfo.GetDamageInfo()->GetVictim();
        }

        if (eventInfo.GetHealInfo())
        {
            amount = eventInfo.GetHealInfo()->GetHeal();
            procSpell = RUNE_PRIEST_FATE_MIRROR_HEAL;
            target = eventInfo.GetHealInfo()->GetTarget();
        }

        if (amount <= 0 || !target || procSpell == 0)
            return;

        ApplyPct(amount, aurEff->GetAmount());
        owner->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_pri_fate_mirror::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_pri_fate_mirror::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_pri_lights_protection : public AuraScript
{
    PrepareAuraScript(spell_pri_lights_protection);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 901622; i < 901628; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Aura* runeAura = GetRuneAura(caster))
        {
            int32 amount = CalculatePct(dmgInfo.GetDamage(), runeAura->GetEffect(EFFECT_0)->GetAmount());
            absorbAmount = amount;

            if (Aura* dot = caster->GetAura(RUNE_PRIEST_LIGHTS_PROTECTION_DOT))
                amount += dot->GetEffect(EFFECT_0)->GetAmount() * dot->GetEffect(EFFECT_0)->GetRemaningTicks();

            amount /= 10;
            caster->CastCustomSpell(RUNE_PRIEST_LIGHTS_PROTECTION_DOT, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_pri_lights_protection::Absorb, EFFECT_0);
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
    RegisterSpellScript(rune_pri_powerful_shadowfiend);
    RegisterSpellScript(rune_pri_powerful_shadowfiend_damage);
    RegisterSpellScript(rune_pri_fiend_friend);
    RegisterSpellScript(rune_pri_rabid_shadows);
    RegisterSpellScript(rune_pri_putrefying_claws);
    RegisterSpellScript(rune_pri_putrefying_claws_damage);
    RegisterSpellScript(rune_pri_essence_devourer);
    RegisterSpellScript(rune_pri_essence_devourer_proc);
    RegisterSpellScript(rune_pri_shadowflame_prism);
    RegisterSpellScript(rune_pri_shadowflame_prism_proc);
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
    RegisterSpellScript(rune_pri_make_amends);
    RegisterSpellScript(rune_pri_phyrinxs_embrace);
    RegisterSpellScript(rune_pri_trail_of_light);
    RegisterSpellScript(rune_pri_screams_of_the_void);
    RegisterSpellScript(rune_pri_idol_of_cthun);
    RegisterSpellScript(rune_pri_idol_of_cthun_damage);
    RegisterSpellScript(rune_pri_mental_decay); 
    RegisterSpellScript(rune_pri_dark_evangelism);
    RegisterSpellScript(rune_pri_unfurling_darkness);
    RegisterSpellScript(rune_pri_idol_of_nzoth);
    RegisterSpellScript(rune_pri_idol_of_nzoth_periodic);
    RegisterSpellScript(rune_pri_whispering_shadows);
    RegisterSpellScript(rune_pri_idol_of_yogg_saron);
    RegisterSpellScript(rune_pri_ancient_madness);
    RegisterSpellScript(rune_pri_ancient_madness_periodic);
    RegisterSpellScript(rune_pri_holymancy);
    RegisterSpellScript(rune_pri_accretion);
    RegisterSpellScript(rune_pri_velens_apprentice);
    RegisterSpellScript(rune_pri_velens_apprentice_targeting);
    RegisterSpellScript(rune_pri_velens_blessing);
    RegisterSpellScript(rune_pri_velens_blessing_proc);
    RegisterSpellScript(rune_pri_fate_mirror);
    RegisterSpellScript(spell_pri_lights_protection);





}

