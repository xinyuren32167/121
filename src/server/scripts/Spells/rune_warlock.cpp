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

enum WarlockSpells
{
    // Spells
    SPELL_WARLOCK_CURSE_OF_ELEMENTS = 47865,
    SPELL_WARLOCK_CURSE_OF_TONGUES = 11719,
    SPELL_WARLOCK_CURSE_OF_WEAKNESS = 50511,
    SPELL_WARLOCK_DOOM = 47867,

    // Talents
    TALENT_WARLOCK_PLACEHOLDER = 00000,

    // Runes
    RUNE_WARLOCK_INQUISITORS_GAZE_DAMAGE = 800107,
    RUNE_WARLOCK_KAZAAKS_FINAL_CURSE_DAMAGE = 800186,
    RUNE_WARLOCK_ACCRUED_VITALITY_HEAL = 800204,

    // Summons
    SUMMON_WARLOCK_INQUISITORS_EYE = 800000,
};

class rune_warl_inquisitors_gaze : public AuraScript
{
    PrepareAuraScript(rune_warl_inquisitors_gaze);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (eventInfo.GetSpellInfo()->Id == RUNE_WARLOCK_INQUISITORS_GAZE_DAMAGE)
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        Position pos = caster->GetNearPosition(urand(1, 8), urand(1, 8));
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = caster->SummonCreature(SUMMON_WARLOCK_INQUISITORS_EYE, pos, TEMPSUMMON_TIMED_DESPAWN, aurEff->GetAmount(), 0, properties);

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
        DoCheckProc += AuraCheckProcFn(rune_warl_inquisitors_gaze::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_inquisitors_gaze::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_inquisitors_gaze_damage : public AuraScript
{
    PrepareAuraScript(rune_warl_inquisitors_gaze_damage);

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

        int32 insanityAmount = GetEffect(EFFECT_0)->GetAmount();

        player->CastSpell(target, RUNE_WARLOCK_INQUISITORS_GAZE_DAMAGE, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_warl_inquisitors_gaze_damage::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_warl_sacrolashs_dark_strike : public AuraScript
{
    PrepareAuraScript(rune_warl_sacrolashs_dark_strike);

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

        int32 durationIncrease = aurEff->GetAmount();
        std::vector<int> curses = {SPELL_WARLOCK_CURSE_OF_ELEMENTS, SPELL_WARLOCK_CURSE_OF_TONGUES, SPELL_WARLOCK_CURSE_OF_WEAKNESS};

        for (auto const& curse : curses)
            if (Aura* curseAura = target->GetAura(curse))
            {
                int32 duration = curseAura->GetDuration();
                duration = std::min<int32>(duration, curseAura->GetMaxDuration());
                curseAura->SetDuration(duration + durationIncrease);
            }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_sacrolashs_dark_strike::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_sacrolashs_dark_strike::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_kazaaks_final_curse : public AuraScript
{
    PrepareAuraScript(rune_warl_kazaaks_final_curse);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 targetNbr = 0;

        if (Player* player = caster->ToPlayer())
        {            
            Pet* pet = player->GetPet();

            if (pet && pet->IsAlive())
                targetNbr++;

            std::vector<Unit*> summonedUnits = player->GetSummonedUnits();
            targetNbr += summonedUnits.size();
        }

        if (targetNbr == 0)
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 damageIncreasePct = aurEff->GetAmount() * targetNbr;
        int32 amount = CalculatePct(damage, damageIncreasePct);

        caster->CastCustomSpell(RUNE_WARLOCK_KAZAAKS_FINAL_CURSE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_kazaaks_final_curse::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_kazaaks_final_curse::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_accrued_vitality : public AuraScript
{
    PrepareAuraScript(rune_warl_accrued_vitality);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, aurEff->GetAmount()) / 10;

        if (Aura* healAura = caster->GetAura(RUNE_WARLOCK_ACCRUED_VITALITY_HEAL))
        {
            int32 amountLeft = healAura->GetEffect(EFFECT_0)->GetAmount() * healAura->GetEffect(EFFECT_0)->GetRemaningTicks() / 10;
            amount += amountLeft;
            healAura->Remove();
        }

        caster->CastCustomSpell(RUNE_WARLOCK_ACCRUED_VITALITY_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_accrued_vitality::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_accrued_vitality::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};



void AddSC_warlock_perks_scripts()
{
    RegisterSpellScript(rune_warl_inquisitors_gaze);
    RegisterSpellScript(rune_warl_inquisitors_gaze_damage);
    RegisterSpellScript(rune_warl_sacrolashs_dark_strike);
    RegisterSpellScript(rune_warl_kazaaks_final_curse);
    RegisterSpellScript(rune_warl_accrued_vitality);

    
    


}
