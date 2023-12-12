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
    SPELL_ROGUE_SHADOW_DANCE = 51713,

    // Poisons
    POISON_ROGUE_AMPLIFYING_POISON = 82005,
    POISON_ROGUE_AMPLIFYING_POISON_AURA = 82006,
    POISON_ROGUE_ANESTHETIC_POISON = 57982,
    POISON_ROGUE_ATROPHIC_POISON = 82003,
    POISON_ROGUE_DEADLY_POISON = 57973,
    POISON_ROGUE_DEADLY_POISON_AURA = 57970,
    POISON_ROGUE_INSTANT_POISON = 57968,
    POISON_ROGUE_NUMBING_POISON = 5761,
    POISON_ROGUE_NUMBING_POISON_AURA = 5760,
    POISON_ROGUE_VAMPIRIC_POISON = 82007,
    POISON_ROGUE_VAMPIRIC_POISON_AURA = 82008,
    POISON_ROGUE_WOUND_POISON = 57978,
    POISON_ROGUE_WOUND_POISON = 57975,

    // Talents
    TALENT_ROGUE_PLACEHOLDER = 00000,

    // Runes
    RUNE_ROGUE_PLACEHOLDER = 00000,
};

class rune_rog_improved_backstab : public AuraScript
{
    PrepareAuraScript(rune_rog_improved_backstab);

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

        if (target->HasAura())
            return true;

        if (target->HasAura(POISON_ROGUE_DEADLY_POISON_AURA))
            return true;

        if (target->HasAura(POISON_ROGUE_VAMPIRIC_POISON_AURA))
            return true;
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_rog_improved_backstab::CheckProc);
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



void AddSC_rogue_perks_scripts()
{
    RegisterSpellScript(rune_rog_improved_backstab);
    RegisterSpellScript(rune_rog_prey_on_the_weak);
    RegisterSpellScript(rune_rog_prison_break);
    RegisterSpellScript(rune_rog_eviscerating_dance);

    



}

