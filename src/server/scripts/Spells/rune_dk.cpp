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
};


class rune_permafrost : public AuraScript
{
    PrepareAuraScript(rune_permafrost);

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
        DoCheckProc += AuraCheckProcFn(rune_permafrost::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_permafrost::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_frozen_pulse : public AuraScript
{
    PrepareAuraScript(rune_frozen_pulse);


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
        DoCheckProc += AuraCheckProcFn(rune_frozen_pulse::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_frozen_pulse::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_blood_feast: public AuraScript
{
    PrepareAuraScript(rune_blood_feast);

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
        OnEffectAbsorb += AuraEffectAbsorbFn(rune_blood_feast::Absorb, EFFECT_0);
    }
};


class rune_blood_transfusion : public AuraScript
{
    PrepareAuraScript(rune_blood_transfusion);

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
        DoCheckProc += AuraCheckProcFn(rune_blood_transfusion::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_blood_transfusion::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

void AddSC_deathknight_perks_scripts()
{
    new rune_permafrost();
    new rune_frozen_pulse();
    new rune_blood_feast();
    new rune_blood_transfusion();
}
