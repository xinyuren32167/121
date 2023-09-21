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

enum ShamanSpells
{
    //Spells
    SPELL_SHAMAN_FIRE_ELEMENTAL = 2894,
    SPELL_SHAMAN_LAVA_BURST = 60043,
    SPELL_SHAMAN_STORM_ELEMENTAL = 84084,

    //Talents
    TALENT_SHAMAN_BLABLA = 00000,

    //Runes
    RUNE_SHAMAN_VOLCANIC_INFERNO_DOT = 1000178,
};

class rune_sha_tidebringer : public AuraScript
{
    PrepareAuraScript(rune_sha_tidebringer);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procSpell = GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
        int32 maxCharges = aurEff->GetAmount();

        if (Aura* proc = caster->GetAura(procSpell))
        {
            int32 currentCharges = proc->GetCharges();
            proc->Remove();
            caster->AddAura(procSpell, caster);        
        }
        else
        {
            caster->AddAura(procSpell, caster);
            caster->GetAura(procSpell)->ModCharges(-1);
        }
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_sha_tidebringer::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_sha_lava_surge : public AuraScript
{
    PrepareAuraScript(rune_sha_lava_surge);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
            player->RemoveSpellCooldown(SPELL_SHAMAN_LAVA_BURST, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_sha_lava_surge::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_sha_fiery_demise : public AuraScript
{
    PrepareAuraScript(rune_sha_fiery_demise);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            player->ModifySpellCooldown(SPELL_SHAMAN_FIRE_ELEMENTAL, -aurEff->GetAmount());
            player->ModifySpellCooldown(SPELL_SHAMAN_STORM_ELEMENTAL, -aurEff->GetAmount());
        }

    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_sha_fiery_demise::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_volcanic_inferno : public AuraScript
{
    PrepareAuraScript(rune_sha_volcanic_inferno);

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

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();

        if (damage == 0)
            return;

        float amount = CalculatePct(damage, aurEff->GetAmount());
        int32 totalTick = sSpellMgr->AssertSpellInfo(RUNE_SHAMAN_VOLCANIC_INFERNO_DOT)->GetMaxTicks();
        amount /= totalTick;

        caster->CastCustomSpell(RUNE_SHAMAN_VOLCANIC_INFERNO_DOT, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_volcanic_inferno::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_volcanic_inferno::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};



void AddSC_shaman_perks_scripts()
{
    RegisterSpellScript(rune_sha_tidebringer);
    RegisterSpellScript(rune_sha_lava_surge);
    RegisterSpellScript(rune_sha_fiery_demise);
    RegisterSpellScript(rune_sha_volcanic_inferno);



}
