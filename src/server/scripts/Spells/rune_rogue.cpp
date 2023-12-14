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

enum RogueSpells
{
    // Spell
    SPELL_ROGUE_EVASION = 26669,
    SPELL_ROGUE_GARROTE = 82002,
    SPELL_ROGUE_RUPTURE = 48672,
    SPELL_ROGUE_SHADOW_DANCE = 51713,
    SPELL_ROGUE_SHADOWSTEP = 36554,
    SPELL_ROGUE_SLICE_AND_DICE = 6774,

    // Poisons
    POISON_ROGUE_AMPLIFYING_POISON = 82005,
    POISON_ROGUE_AMPLIFYING_POISON_AURA = 82006,
    POISON_ROGUE_ANESTHETIC_POISON = 57982,
    POISON_ROGUE_ATROPHIC_POISON = 82003,
    POISON_ROGUE_ATROPHIC_POISON_AURA = 82004,
    POISON_ROGUE_DEADLY_POISON = 57973,
    POISON_ROGUE_DEADLY_POISON_AURA = 57970,
    POISON_ROGUE_INSTANT_POISON = 57968,
    POISON_ROGUE_NUMBING_POISON = 5761,
    POISON_ROGUE_NUMBING_POISON_AURA = 5760,
    POISON_ROGUE_VAMPIRIC_POISON = 82007,
    POISON_ROGUE_VAMPIRIC_POISON_AURA = 82008,
    POISON_ROGUE_WOUND_POISON = 57978,
    POISON_ROGUE_WOUND_POISON_AURA = 57975,

    // Talents
    TALENT_ROGUE_PLACEHOLDER = 00000,

    // Runes
    RUNE_ROGUE_INTERNAL_BLEEDING_DOT = 1100358,
    RUNE_ROGUE_REPLICATING_SHADOWS_DAMAGE = 1100390,
    RUNE_ROGUE_LINGERING_SHADOW_BUFF = 1100440,
    RUNE_ROGUE_LINGERING_SHADOW_DAMAGE = 1100441,
};

class rune_rog_venom_rush : public AuraScript
{
    PrepareAuraScript(rune_rog_venom_rush);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return false;

        if (!eventInfo.GetDamageInfo())
            return false;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return false;

        if (target->HasAura(POISON_ROGUE_NUMBING_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_AMPLIFYING_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_ATROPHIC_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_DEADLY_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_VAMPIRIC_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_WOUND_POISON_AURA))
            return true;
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_venom_rush::CheckProc);
    }
};

class rune_rog_improved_backstab : public AuraScript
{
    PrepareAuraScript(rune_rog_improved_backstab);

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

        if (target->HasInArc(M_PI, caster))
            return;

        int32 procSpell = aurEff->GetAmount();
        caster->AddAura(procSpell, target);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_improved_backstab::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_improved_backstab::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_prey_on_the_weak : public AuraScript
{
    PrepareAuraScript(rune_rog_prey_on_the_weak);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 1100172; i < 1100178; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        int32 procSpell = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();

        caster->AddAura(procSpell, target);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(rune_rog_prey_on_the_weak::HandleApplyEffect, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_rog_prison_break : public AuraScript
{
    PrepareAuraScript(rune_rog_prison_break);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procChance = caster->GetComboPoints() * aurEff->GetAmount();

        if (!roll_chance_i(procChance))
            return;

        if (Player* player = caster->ToPlayer())
            player->RemoveSpellCooldown(SPELL_ROGUE_EVASION, true);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_prison_break::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_prison_break::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_eviscerating_dance : public AuraScript
{
    PrepareAuraScript(rune_rog_eviscerating_dance);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procChance = caster->GetComboPoints() * aurEff->GetAmount();

        if (!roll_chance_i(procChance))
            return;

        int32 cooldown = GetEffect(EFFECT_1)->GetAmount();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_ROGUE_SHADOW_DANCE, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_eviscerating_dance::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_eviscerating_dance::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_internal_bleeding : public AuraScript
{
    PrepareAuraScript(rune_rog_internal_bleeding);

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

        int32 damageRatio = caster->GetComboPoints() * aurEff->GetAmount();
        int32 amount = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);

        if (amount <= 0)
            return;

        caster->CastCustomSpell(RUNE_ROGUE_INTERNAL_BLEEDING_DOT, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_internal_bleeding::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_internal_bleeding::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_venomous_wounds : public AuraScript
{
    PrepareAuraScript(rune_rog_venomous_wounds);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return false;

        if (!eventInfo.GetDamageInfo())
            return false;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return false;

        if (target->HasAura(POISON_ROGUE_NUMBING_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_AMPLIFYING_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_ATROPHIC_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_DEADLY_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_VAMPIRIC_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_WOUND_POISON_AURA))
            return true;
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_venomous_wounds::CheckProc);
    }
};

class rune_rog_scent_of_blood : public AuraScript
{
    PrepareAuraScript(rune_rog_scent_of_blood);

    int32 GetRuptureTargetNbr(Unit* caster)
    {
        auto const& threatList = caster->getAttackers();
        int32 targetNbr = 0;

        for (auto const& threat : threatList)
            if (threat->IsAlive())
            {
                if (threat->HasAura(SPELL_ROGUE_RUPTURE))
                    targetNbr++;
            }

        return targetNbr;
    }

    void HandleEffectPeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 maxTargets = GetEffect(EFFECT_1)->GetAmount();
        int32 targetNbr = std::min<int32>(GetRuptureTargetNbr(caster), maxTargets);
        int32 procSpell = aurEff->GetAmount();

        if (targetNbr == 0)
        {
            if (caster->HasAura(procSpell))
                caster->RemoveAura(procSpell);

            return;
        }

        if (Aura* buff = caster->GetAura(procSpell))
        {
            if (targetNbr == buff->GetStackAmount())
                return;
            else
                buff->SetStackAmount(targetNbr);
        }
        else
        {
            caster->AddAura(procSpell, caster);
            caster->GetAura(procSpell)->SetStackAmount(targetNbr);
        }
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1100378; i < 1100384; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_rog_scent_of_blood::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        AfterEffectRemove += AuraEffectRemoveFn(rune_rog_scent_of_blood::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_rog_replicating_shadows : public AuraScript
{
    PrepareAuraScript(rune_rog_replicating_shadows);

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

        if (amount <= 0)
            return;

        caster->CastCustomSpell(RUNE_ROGUE_REPLICATING_SHADOWS_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_replicating_shadows::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_replicating_shadows::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_blackjack : public AuraScript
{
    PrepareAuraScript(rune_rog_blackjack);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 1100398; i < 1100404; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        if (Aura* runeAura = GetRuneAura(caster))
        {
            int32 procSpell = runeAura->GetEffect(EFFECT_0)->GetAmount();
            caster->AddAura(procSpell, target);
        }
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(rune_rog_blackjack::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_rog_lingering_shadow : public AuraScript
{
    PrepareAuraScript(rune_rog_lingering_shadow);

    void HandleEffectPeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Aura* buff = caster->GetAura(RUNE_ROGUE_LINGERING_SHADOW_BUFF))
            buff->ModStackAmount(-1);
    }

    void OnAfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(RUNE_ROGUE_LINGERING_SHADOW_BUFF))
            caster->RemoveAura(RUNE_ROGUE_LINGERING_SHADOW_BUFF);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_rog_lingering_shadow::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        AfterEffectRemove += AuraEffectRemoveFn(rune_rog_lingering_shadow::OnAfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_rog_lingering_shadow_proc : public AuraScript
{
    PrepareAuraScript(rune_rog_lingering_shadow_proc);

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

        if (amount <= 0)
            return;

        caster->CastCustomSpell(RUNE_ROGUE_LINGERING_SHADOW_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_lingering_shadow_proc::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_lingering_shadow_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_dance_macabre : public AuraScript
{
    PrepareAuraScript(rune_rog_dance_macabre);

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

        for (auto const& id : spellUsed)
            if (id == spellID)
                return;

        ModStackAmount(1);
        spellUsed.emplace_back(spellID);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_dance_macabre::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_dance_macabre::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
private:
    std::list<int> spellUsed;
};

class rune_rog_deepening_shadows : public AuraScript
{
    PrepareAuraScript(rune_rog_deepening_shadows);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = GetEffect(EFFECT_0)->GetAmount() * caster->GetComboPoints();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_ROGUE_SHADOW_DANCE, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_deepening_shadows::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_deepening_shadows::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_vampiric_strike : public AuraScript
{
    PrepareAuraScript(rune_rog_vampiric_strike);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return false;

        if (!eventInfo.GetDamageInfo())
            return false;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return false;

        return target->HasAura(POISON_ROGUE_VAMPIRIC_POISON_AURA);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_vampiric_strike::CheckProc);
    }
};

class rune_rog_slice_and_strike : public AuraScript
{
    PrepareAuraScript(rune_rog_slice_and_strike);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 duration = GetEffect(EFFECT_0)->GetAmount();

        if (Aura* slice = caster->GetAura(SPELL_ROGUE_SLICE_AND_DICE))
            slice->SetDuration(slice->GetDuration() + duration);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_slice_and_strike::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_slice_and_strike::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};



void AddSC_rogue_perks_scripts()
{
    RegisterSpellScript(rune_rog_venom_rush);
    RegisterSpellScript(rune_rog_improved_backstab);
    RegisterSpellScript(rune_rog_prey_on_the_weak);
    RegisterSpellScript(rune_rog_prison_break);
    RegisterSpellScript(rune_rog_eviscerating_dance);
    RegisterSpellScript(rune_rog_internal_bleeding);
    RegisterSpellScript(rune_rog_venomous_wounds);
    RegisterSpellScript(rune_rog_scent_of_blood);
    RegisterSpellScript(rune_rog_replicating_shadows);
    RegisterSpellScript(rune_rog_blackjack);
    RegisterSpellScript(rune_rog_lingering_shadow);
    RegisterSpellScript(rune_rog_lingering_shadow_proc);
    RegisterSpellScript(rune_rog_dance_macabre);
    RegisterSpellScript(rune_rog_deepening_shadows);
    RegisterSpellScript(rune_rog_vampiric_strike);
    RegisterSpellScript(rune_rog_slice_and_strike);





}

