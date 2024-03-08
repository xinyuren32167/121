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
    SPELL_ROGUE_ADRENALINE_RUSH = 13750,
    SPELL_ROGUE_BACKSTAB = 82001,
    SPELL_ROGUE_BETWEEN_THE_EYES = 82072,
    SPELL_ROGUE_BLACK_POWDER = 82041,
    SPELL_ROGUE_BLADE_FLURRY = 13877,
    SPELL_ROGUE_BLADE_RUSH = 82028,
    SPELL_ROGUE_CAPTAIN_STRIKE = 82083,
    SPELL_ROGUE_COMBAT_ECSTASY = 82073,
    SPELL_ROGUE_DEATHMARK = 82025,
    SPELL_ROGUE_DREADBLADES = 82080,
    SPELL_ROGUE_EVASION = 26669,
    SPELL_ROGUE_EVISCERATE = 48668,
    SPELL_ROGUE_FEINT = 48659,
    SPELL_ROGUE_GARROTE = 82002,
    SPELL_ROGUE_GHOSTLY_STRIKE = 14278,
    SPELL_ROGUE_GRAPPLING_HOOK = 82084,
    SPELL_ROGUE_KEEP_IT_ROLLING = 82098,
    SPELL_ROGUE_KILLING_SPREE = 51690,
    SPELL_ROGUE_KILLING_SPREE_MAIN_HAND = 57841,
    SPELL_ROGUE_KILLING_SPREE_OFF_HAND = 57842,
    SPELL_ROGUE_KINGSBANE = 82037,
    SPELL_ROGUE_MARKED_FOR_DEATH = 82022,
    SPELL_ROGUE_RIPOSTE = 82064,
    SPELL_ROGUE_ROLL_THE_BONES = 82091,
    SPELL_ROGUE_ROLL_THE_BONES_GRAND_MELEE = 82092,
    SPELL_ROGUE_ROLL_THE_BONES_BROADSIDE = 82093,
    SPELL_ROGUE_ROLL_THE_BONES_RUTHLESS_PRECISION = 82094,
    SPELL_ROGUE_ROLL_THE_BONES_BURIED_TREASURE = 82095,
    SPELL_ROGUE_ROLL_THE_BONES_SKULL_AND_CROSSBONES = 82096,
    SPELL_ROGUE_ROLL_THE_BONES_TRUE_BEARING = 82097,
    SPELL_ROGUE_RUPTURE = 48672,
    SPELL_ROGUE_SCIMITAR_RUSH = 82075,
    SPELL_ROGUE_SEA_OF_STRIKES = 82087,
    SPELL_ROGUE_SHADOW_BLADES = 82042,
    SPELL_ROGUE_SHADOW_DANCE = 51713,
    SPELL_ROGUE_SHADOWSTEP = 36554,
    SPELL_ROGUE_SHADOWSTIKE = 82016,
    SPELL_ROGUE_SLICE_AND_DICE = 6774,
    SPELL_ROGUE_SPRINT = 11305,
    SPELL_ROGUE_SYMBOLS_OF_DEATH = 82040,
    SPELL_ROGUE_VAMPIRIC_BURST = 82050,
    SPELL_ROGUE_VANISH = 26889,
    
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
    TALENT_ROGUE_OPPORTUNITY = 82085,
    TALENT_ROGUE_OPPORTUNITY_BUFF = 82086,
    TALENT_ROGUE_OPPORTUNITY_BUFF_AMBUSH = 82189,

    // Runes
    RUNE_ROGUE_INTERNAL_BLEEDING_DOT = 1100358,
    RUNE_ROGUE_REPLICATING_SHADOWS_DAMAGE = 1100390,
    RUNE_ROGUE_LINGERING_SHADOW_BUFF = 1100440,
    RUNE_ROGUE_LINGERING_SHADOW_DAMAGE = 1100441,
    RUNE_ROGUE_CLOAKED_IN_SHADOWS_SHIELD = 1100642,
    RUNE_ROGUE_ZOLDYCK_RECIPE_DAMAGE = 1100710,
    RUNE_ROGUE_LEECHING_POISON_HEAL = 1100718,
    RUNE_ROGUE_VEILTOUCHED_SHADOW_DAMAGE = 1100736,
    RUNE_ROGUE_VEILTOUCHED_NATURE_DAMAGE = 1100737,
    RUNE_ROGUE_VICIOUS_VENOMS_DAMAGE = 1100848,
    RUNE_ROGUE_DOOMBLADE_DOT = 1100856,
    RUNE_ROGUE_BLINDSIDE_BUFF = 1100880,
    RUNE_ROGUE_SHADOWED_FINISHERS_DAMAGE = 1101170,
    RUNE_ROGUE_WEAPONMASTER_PROC = 1101250,
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

        return false;
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

        if (target->HasAura(GetAura()->GetEffect(EFFECT_0)->GetAmount()))
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

        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastSpell(GetCaster(), GetAura()->GetEffect(EFFECT_0)->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_venomous_wounds::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_venomous_wounds::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
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

class rune_rog_recuperator : public AuraScript
{
    PrepareAuraScript(rune_rog_recuperator);

    void HandleEffectPeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_ROGUE_SLICE_AND_DICE))
            return;

        caster->CastSpell(caster, aurEff->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_rog_recuperator::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_rog_cloaked_in_shadows : public AuraScript
{
    PrepareAuraScript(rune_rog_cloaked_in_shadows);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 amount = CalculatePct(caster->GetMaxHealth(), aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_ROGUE_CLOAKED_IN_SHADOWS_SHIELD, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_cloaked_in_shadows::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_cloaked_in_shadows::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_zoldyck_recipe : public AuraScript
{
    PrepareAuraScript(rune_rog_zoldyck_recipe);

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

        int32 healthPct = target->GetHealthPct();
        int32 healthThreshold = GetEffect(EFFECT_1)->GetAmount();

        if (healthPct > healthThreshold)
            return;

        int32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_ROGUE_ZOLDYCK_RECIPE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_zoldyck_recipe::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_zoldyck_recipe::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_leeching_poison : public AuraScript
{
    PrepareAuraScript(rune_rog_leeching_poison);

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

        caster->CastCustomSpell(RUNE_ROGUE_LEECHING_POISON_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_leeching_poison::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_leeching_poison::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_veiltouched : public AuraScript
{
    PrepareAuraScript(rune_rog_veiltouched);

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

        int32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_ROGUE_VEILTOUCHED_SHADOW_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
        caster->CastCustomSpell(RUNE_ROGUE_VEILTOUCHED_NATURE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_veiltouched::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_veiltouched::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_seal_fate : public AuraScript
{
    PrepareAuraScript(rune_rog_seal_fate);

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

        caster->AddComboPoints(target, aurEff->GetAmount());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_seal_fate::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_seal_fate::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_alacrity : public AuraScript
{
    PrepareAuraScript(rune_rog_alacrity);

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

        int32 procSpell = GetEffect(EFFECT_1)->GetAmount();
        caster->AddAura(procSpell, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_alacrity::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_alacrity::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_relentless_strikes : public AuraScript
{
    PrepareAuraScript(rune_rog_relentless_strikes);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 energy = caster->GetComboPoints() * aurEff->GetAmount();

        caster->EnergizeBySpell(caster, GetSpellInfo()->Id, energy, POWER_ENERGY);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_relentless_strikes::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_relentless_strikes::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_restless_blades : public AuraScript
{
    PrepareAuraScript(rune_rog_restless_blades);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = caster->GetComboPoints() * aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
        {
            player->ModifySpellCooldown(SPELL_ROGUE_ADRENALINE_RUSH, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_BETWEEN_THE_EYES, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_BLADE_FLURRY, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_BLADE_RUSH, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_CAPTAIN_STRIKE, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_COMBAT_ECSTASY, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_DREADBLADES, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_GHOSTLY_STRIKE, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_GRAPPLING_HOOK, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_KEEP_IT_ROLLING, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_KILLING_SPREE, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_MARKED_FOR_DEATH, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_RIPOSTE, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_ROLL_THE_BONES, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_SEA_OF_STRIKES, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_SCIMITAR_RUSH, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_SPRINT, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_VANISH, -cooldown);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_restless_blades::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_restless_blades::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_float_like_a_butterfly : public AuraScript
{
    PrepareAuraScript(rune_rog_float_like_a_butterfly);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = caster->GetComboPoints() * aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
        {
            player->ModifySpellCooldown(SPELL_ROGUE_EVASION, -cooldown);
            player->ModifySpellCooldown(SPELL_ROGUE_FEINT, -cooldown);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_float_like_a_butterfly::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_float_like_a_butterfly::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_poison_bomb : public AuraScript
{
    PrepareAuraScript(rune_rog_poison_bomb);

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

        int32 procChance = caster->GetComboPoints() * aurEff->GetAmount();

        if (!roll_chance_i(procChance))
            return;

        int32 procSpell = GetEffect(EFFECT_1)->GetAmount();
        caster->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_poison_bomb::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_poison_bomb::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_vicious_venoms : public AuraScript
{
    PrepareAuraScript(rune_rog_vicious_venoms);

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

        caster->CastCustomSpell(RUNE_ROGUE_VICIOUS_VENOMS_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_vicious_venoms::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_vicious_venoms::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_doomblade : public AuraScript
{
    PrepareAuraScript(rune_rog_doomblade);

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
        amount /= 8;

        if (Aura* doombladeDot = target->GetAura(RUNE_ROGUE_DOOMBLADE_DOT))
            if (AuraEffect* dot = doombladeDot->GetEffect(EFFECT_0))
            {
                int32 remainingDamage = dot->GetAmount() * dot->GetRemaningTicks();
                amount += remainingDamage / 8;
                target->RemoveAura(dot->GetBase());
            }

        caster->CastCustomSpell(RUNE_ROGUE_DOOMBLADE_DOT, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_doomblade::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_doomblade::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_blindside : public AuraScript
{
    PrepareAuraScript(rune_rog_blindside);

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

        int32 procChance = aurEff->GetAmount();
        int32 healthThreshold = GetEffect(EFFECT_1)->GetAmount();

        if (target->GetHealthPct() < healthThreshold)
            procChance *= 2;

        if (!roll_chance_i(procChance))
            return;

        caster->AddAura(RUNE_ROGUE_BLINDSIDE_BUFF, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_blindside::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_blindside::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_advancing_death : public AuraScript
{
    PrepareAuraScript(rune_rog_advancing_death);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_ROGUE_DEATHMARK, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_advancing_death::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_advancing_death::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_everlasting_bane : public AuraScript
{
    PrepareAuraScript(rune_rog_everlasting_bane);

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

        int32 durationIncrease = aurEff->GetAmount();

        if (Aura* kingsbane = target->GetAura(SPELL_ROGUE_KINGSBANE))
        {
            int32 duration = std::min<int32>(kingsbane->GetDuration() + durationIncrease, kingsbane->GetMaxDuration());
            kingsbane->SetDuration(duration);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_everlasting_bane::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_everlasting_bane::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_brigands_blitz : public AuraScript
{
    PrepareAuraScript(rune_rog_brigands_blitz);

    void HandleApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procSpell = GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1100954; i < 1100960; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_rog_brigands_blitz::HandleApplyEffect, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(rune_rog_brigands_blitz::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_rog_improved_combat_ecstasy : public AuraScript
{
    PrepareAuraScript(rune_rog_improved_combat_ecstasy);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        caster->AddComboPoints(aurEff->GetAmount());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_improved_combat_ecstasy::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_improved_combat_ecstasy::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_ecstasy_filled_attacks : public AuraScript
{
    PrepareAuraScript(rune_rog_ecstasy_filled_attacks);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_ROGUE_COMBAT_ECSTASY, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_ecstasy_filled_attacks::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_ecstasy_filled_attacks::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_ghostly_cut : public AuraScript
{
    PrepareAuraScript(rune_rog_ghostly_cut);

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
        int32 combo = caster->GetComboPoints();

        caster->AddAura(procSpell, caster);
        caster->GetAura(procSpell)->SetStackAmount(combo);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_ghostly_cut::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_ghostly_cut::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_echoing_spree : public AuraScript
{
    PrepareAuraScript(rune_rog_echoing_spree);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetSpellInfo())
            return false;

        return eventInfo.GetSpellInfo()->Id == SPELL_ROGUE_KILLING_SPREE_MAIN_HAND || eventInfo.GetSpellInfo()->Id == SPELL_ROGUE_KILLING_SPREE_OFF_HAND;
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_echoing_spree::CheckProc);
    }
};

class rune_rog_rapid_spree : public AuraScript
{
    PrepareAuraScript(rune_rog_rapid_spree);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_ROGUE_KILLING_SPREE, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_rapid_spree::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_rapid_spree::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_quick_riposte_proc : public AuraScript
{
    PrepareAuraScript(rune_rog_quick_riposte_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        caster->AddComboPoints(aurEff->GetAmount());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_quick_riposte_proc::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_quick_riposte_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_vampiric_riposte : public AuraScript
{
    PrepareAuraScript(rune_rog_vampiric_riposte);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_ROGUE_VAMPIRIC_BURST, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_vampiric_riposte::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_vampiric_riposte::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_rog_ghostly_hand : public AuraScript
{
    PrepareAuraScript(rune_rog_ghostly_hand);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_ROGUE_GHOSTLY_STRIKE, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_ghostly_hand::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_ghostly_hand::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_vampiric_hand : public AuraScript
{
    PrepareAuraScript(rune_rog_vampiric_hand);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return false;

        return target->HasAura(POISON_ROGUE_VAMPIRIC_POISON_AURA);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_vampiric_hand::CheckProc);
    }
};

class rune_rog_shadowed_finishers : public AuraScript
{
    PrepareAuraScript(rune_rog_shadowed_finishers);

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

        caster->CastCustomSpell(RUNE_ROGUE_SHADOWED_FINISHERS_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_shadowed_finishers::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_shadowed_finishers::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_finality : public AuraScript
{
    PrepareAuraScript(rune_rog_finality);

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

        if (spellID == SPELL_ROGUE_EVISCERATE)
            procSpell = aurEff->GetAmount();

        if (spellID == SPELL_ROGUE_RUPTURE)
            procSpell = GetEffect(EFFECT_1)->GetAmount();

        if (spellID == 0)
            return;

        caster->AddAura(procSpell, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_finality::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_finality::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_weaponmaster : public AuraScript
{
    PrepareAuraScript(rune_rog_weaponmaster);

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

        int32 spellID = eventInfo.GetSpellInfo()->Id;

        caster->CastCustomSpell(RUNE_ROGUE_WEAPONMASTER_PROC, SPELLVALUE_BASE_POINT0, spellID, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_weaponmaster::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_weaponmaster::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_weaponmaster_proc : public AuraScript
{
    PrepareAuraScript(rune_rog_weaponmaster_proc);

    void HandleEffectPeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        caster->CastSpell(target, aurEff->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_rog_weaponmaster_proc::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_rog_premeditating_shadows : public AuraScript
{
    PrepareAuraScript(rune_rog_premeditating_shadows);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 duration = aurEff->GetAmount();
        int32 combo = GetEffect(EFFECT_1)->GetAmount();

        if (caster->HasAura(SPELL_ROGUE_SLICE_AND_DICE))
            caster->AddComboPoints(combo);
        else
        {
            caster->AddAura(SPELL_ROGUE_SLICE_AND_DICE, caster);
            caster->GetAura(SPELL_ROGUE_SLICE_AND_DICE)->SetDuration(duration);
        } 
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_premeditating_shadows::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_premeditating_shadows::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_stiletto_staccato : public AuraScript
{
    PrepareAuraScript(rune_rog_stiletto_staccato);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_ROGUE_SHADOW_BLADES, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_stiletto_staccato::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_stiletto_staccato::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_inevitability : public AuraScript
{
    PrepareAuraScript(rune_rog_inevitability);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 duration = aurEff->GetAmount();

        if (Aura* symbol = caster->GetAura(SPELL_ROGUE_SYMBOLS_OF_DEATH))
            symbol->SetDuration(symbol->GetDuration() + duration);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_inevitability::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_inevitability::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_greenskins_wickers : public AuraScript
{
    PrepareAuraScript(rune_rog_greenskins_wickers);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return false;

        int32 combo = caster->GetComboPoints();
        int32 chance = GetEffect(EFFECT_0)->GetAmount() * combo;

        return roll_chance_i(chance);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_greenskins_wickers::CheckProc);
    }
};

class rune_rog_ace_up_your_sleeve : public AuraScript
{
    PrepareAuraScript(rune_rog_ace_up_your_sleeve);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return false;

        int32 combo = caster->GetComboPoints();
        int32 chance = GetEffect(EFFECT_0)->GetAmount() * combo;

        return roll_chance_i(chance);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_ace_up_your_sleeve::CheckProc);
    }
};

class rune_rog_count_the_odds : public AuraScript
{
    PrepareAuraScript(rune_rog_count_the_odds);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procChance = aurEff->GetAmount();
        int32 duration = GetEffect(EFFECT_1)->GetAmount();

        if (caster->HasAura(SPELL_ROGUE_SHADOW_DANCE))
        {
            procChance *= 2;
            duration *= 2;
        }

        if (!roll_chance_i(procChance))
            return;

        std::vector<int> rolltheBonesBuffs;

        if (!caster->HasAura(SPELL_ROGUE_ROLL_THE_BONES_GRAND_MELEE))
            rolltheBonesBuffs.push_back(SPELL_ROGUE_ROLL_THE_BONES_GRAND_MELEE);

        if (!caster->HasAura(SPELL_ROGUE_ROLL_THE_BONES_BROADSIDE))
            rolltheBonesBuffs.push_back(SPELL_ROGUE_ROLL_THE_BONES_BROADSIDE);

        if (!caster->HasAura(SPELL_ROGUE_ROLL_THE_BONES_RUTHLESS_PRECISION))
            rolltheBonesBuffs.push_back(SPELL_ROGUE_ROLL_THE_BONES_RUTHLESS_PRECISION);

        if (!caster->HasAura(SPELL_ROGUE_ROLL_THE_BONES_BURIED_TREASURE))
            rolltheBonesBuffs.push_back(SPELL_ROGUE_ROLL_THE_BONES_BURIED_TREASURE);

        if (!caster->HasAura(SPELL_ROGUE_ROLL_THE_BONES_SKULL_AND_CROSSBONES))
            rolltheBonesBuffs.push_back(SPELL_ROGUE_ROLL_THE_BONES_SKULL_AND_CROSSBONES);

        if (!caster->HasAura(SPELL_ROGUE_ROLL_THE_BONES_TRUE_BEARING))
            rolltheBonesBuffs.push_back(SPELL_ROGUE_ROLL_THE_BONES_TRUE_BEARING);

        if (rolltheBonesBuffs.size() <= 0)
            return;

        uint32 rand = urand(0, rolltheBonesBuffs.size() - 1);

        caster->CastCustomSpell(rolltheBonesBuffs[rand], SPELLVALUE_AURA_DURATION, duration, caster, TRIGGERED_FULL_MASK);

    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_count_the_odds::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_count_the_odds::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rog_fan_the_hammer : public AuraScript
{
    PrepareAuraScript(rune_rog_fan_the_hammer);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return false;

        return caster->HasAura(TALENT_ROGUE_OPPORTUNITY_BUFF) || caster->HasAura(TALENT_ROGUE_OPPORTUNITY_BUFF_AMBUSH);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_fan_the_hammer::CheckProc);
    }
};

class rune_rog_quick_draw : public AuraScript
{
    PrepareAuraScript(rune_rog_quick_draw);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(TALENT_ROGUE_OPPORTUNITY_BUFF) && !caster->HasAura(TALENT_ROGUE_OPPORTUNITY_BUFF_AMBUSH))
            return;

        caster->AddComboPoints(aurEff->GetAmount());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_quick_draw::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_rog_quick_draw::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
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
    RegisterSpellScript(rune_rog_recuperator);
    RegisterSpellScript(rune_rog_cloaked_in_shadows);
    RegisterSpellScript(rune_rog_zoldyck_recipe);
    RegisterSpellScript(rune_rog_leeching_poison);
    RegisterSpellScript(rune_rog_veiltouched);
    RegisterSpellScript(rune_rog_seal_fate);
    RegisterSpellScript(rune_rog_alacrity);
    RegisterSpellScript(rune_rog_relentless_strikes);
    RegisterSpellScript(rune_rog_restless_blades);
    RegisterSpellScript(rune_rog_float_like_a_butterfly);
    RegisterSpellScript(rune_rog_poison_bomb);
    RegisterSpellScript(rune_rog_vicious_venoms);
    RegisterSpellScript(rune_rog_doomblade);
    RegisterSpellScript(rune_rog_blindside);
    RegisterSpellScript(rune_rog_advancing_death);
    RegisterSpellScript(rune_rog_everlasting_bane);
    RegisterSpellScript(rune_rog_brigands_blitz);
    RegisterSpellScript(rune_rog_improved_combat_ecstasy);
    RegisterSpellScript(rune_rog_ecstasy_filled_attacks);
    RegisterSpellScript(rune_rog_ghostly_cut);
    RegisterSpellScript(rune_rog_echoing_spree);
    RegisterSpellScript(rune_rog_rapid_spree);
    RegisterSpellScript(rune_rog_quick_riposte_proc);
    RegisterSpellScript(rune_rog_vampiric_riposte);
    RegisterSpellScript(rune_rog_ghostly_hand);
    RegisterSpellScript(rune_rog_vampiric_hand);
    RegisterSpellScript(rune_rog_shadowed_finishers);
    RegisterSpellScript(rune_rog_finality);
    RegisterSpellScript(rune_rog_weaponmaster);
    RegisterSpellScript(rune_rog_weaponmaster_proc);
    RegisterSpellScript(rune_rog_premeditating_shadows);
    RegisterSpellScript(rune_rog_stiletto_staccato); 
    RegisterSpellScript(rune_rog_inevitability);
    RegisterSpellScript(rune_rog_greenskins_wickers);
    RegisterSpellScript(rune_rog_ace_up_your_sleeve);
    RegisterSpellScript(rune_rog_count_the_odds);
    RegisterSpellScript(rune_rog_fan_the_hammer);
    RegisterSpellScript(rune_rog_quick_draw);
}
