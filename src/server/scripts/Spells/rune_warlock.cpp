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
    SPELL_WARLOCK_DRAIN_LIFE = 47857,
    SPELL_WARLOCK_UNENDING_RESOLVE = 83016,

    // Talents
    TALENT_WARLOCK_PLACEHOLDER = 00000,

    // Runes
    RUNE_WARLOCK_INQUISITORS_GAZE_DAMAGE = 800107,
    RUNE_WARLOCK_KAZAAKS_FINAL_CURSE_DAMAGE = 800186,
    RUNE_WARLOCK_ACCRUED_VITALITY_HEAL = 800204,
    RUNE_WARLOCK_CLAW_OF_ENDERETH_HEAL = 800218,
    RUNE_WARLOCK_FEL_SYNERGY_HEAL = 800328,
    RUNE_WARLOCK_RESOLUTE_BARRIER_DEBUFF = 800414,

    // Pet Scaling
    PET_SCALING_WARLOCK_DAMAGE_HASTE = 83205,
    PET_SCALING_WARLOCK_STAMINA_DAMAGEREDUC_ARMOR = 83206,
    PET_SCALING_WARLOCK_INTELLECT_ALLRES = 83207,

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
        std::vector<int> curses = { SPELL_WARLOCK_CURSE_OF_ELEMENTS, SPELL_WARLOCK_CURSE_OF_TONGUES, SPELL_WARLOCK_CURSE_OF_WEAKNESS };

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
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
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
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
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

class rune_warl_claw_of_endereth : public AuraScript
{
    PrepareAuraScript(rune_warl_claw_of_endereth);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 drainMultiplePct = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_DRAIN_LIFE)->GetEffect(EFFECT_0).CalcValueMultiplier(caster);
        int32 amount = damage * drainMultiplePct;
        ApplyPct(amount, aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_WARLOCK_CLAW_OF_ENDERETH_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_claw_of_endereth::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_claw_of_endereth::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_funerary_ceremony : public AuraScript
{
    PrepareAuraScript(rune_warl_funerary_ceremony);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_funerary_ceremony::CheckProc);
    }
};

class rune_warl_funerary_ceremony_apply : public AuraScript
{
    PrepareAuraScript(rune_warl_funerary_ceremony_apply);

    std::list <Unit*> FindTargets()
    {
        Player* caster = GetCaster()->ToPlayer();
        std::list <Unit*> targetAvailable;
        Position casterPos = caster->GetPosition();
        auto const& enemyList = caster->getAttackers();

        for (auto const& target : enemyList)
        {
            if (target->IsAlive())
            {
                Unit* dummy = target->ToUnit();

                if (target->HasAura(SPELL_WARLOCK_DRAIN_LIFE))
                    targetAvailable.push_back(dummy);
            }
        }
        return targetAvailable;
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        std::list <Unit*> targetList = FindTargets();

        if (targetList.size() <= 0)
            return;

        for (auto const& target : targetList)
            if (Aura* drainLife = target->GetAura(SPELL_WARLOCK_DRAIN_LIFE))
            {
                drainLife->GetEffect(EFFECT_0)->ResetTicks();
                drainLife->GetEffect(EFFECT_0)->CalculatePeriodic(caster);
            }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectRemoveFn(rune_warl_funerary_ceremony_apply::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

class rune_warl_everlasting_shadows : public AuraScript
{
    PrepareAuraScript(rune_warl_everlasting_shadows);

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

        int32 duration = aurEff->GetAmount();
        auto targetAuras = target->GetAppliedAuras();

        for (auto itr = targetAuras.begin(); itr != targetAuras.end(); ++itr)
        {
            if (Aura* aura = itr->second->GetBase())
            {
                if (aura->GetCaster() != caster)
                    continue;

                SpellInfo const* auraInfo = aura->GetSpellInfo();

                if (auraInfo->SpellFamilyName == SPELLFAMILY_WARLOCK && auraInfo->SpellFamilyFlags[2] & 0x80000000)
                {
                    int32 newDuration = std::min<int32>(aura->GetDuration() + duration, aura->GetMaxDuration());
                    aura->SetDuration(newDuration);
                }
            }
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_everlasting_shadows::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_everlasting_shadows::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_fel_synergy : public AuraScript
{
    PrepareAuraScript(rune_warl_fel_synergy);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 amount = eventInfo.GetHealInfo()->GetHeal();

        if (Player* player = caster->ToPlayer())
            if (Pet* pet = player->GetPet())
                caster->CastCustomSpell(RUNE_WARLOCK_FEL_SYNERGY_HEAL, SPELLVALUE_BASE_POINT0, amount, pet, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_fel_synergy::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_fel_synergy::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_grimoire_of_synergy : public AuraScript
{
    PrepareAuraScript(rune_warl_grimoire_of_synergy);

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
            if (Pet* pet = player->GetPet())
            {
                int32 procSpell = aurEff->GetAmount();

                caster->CastSpell(pet, procSpell, TRIGGERED_FULL_MASK);
            }
        }
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            if (Pet* pet = player->GetPet())
            {
                int32 procSpell = aurEff->GetAmount();

                if (pet->HasAura(procSpell))
                    return;

                caster->CastSpell(pet, procSpell, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_grimoire_of_synergy::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_grimoire_of_synergy::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_warl_grimoire_of_synergy::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_warl_grimoire_of_synergy_pet : public AuraScript
{
    PrepareAuraScript(rune_warl_grimoire_of_synergy_pet);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procSpell = aurEff->GetAmount();

        caster->CastSpell(caster, procSpell, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_grimoire_of_synergy_pet::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_grimoire_of_synergy_pet::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_resolute_barrier : public AuraScript
{
    PrepareAuraScript(rune_warl_resolute_barrier);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(RUNE_WARLOCK_RESOLUTE_BARRIER_DEBUFF))
            return;

        int32 damageTaken = eventInfo.GetDamageInfo()->GetDamage();
        int32 healthThreshold = GetEffect(EFFECT_1)->GetAmount();

        if (damageTaken < caster->CountPctFromMaxHealth(healthThreshold))
            return;

        if (Player* player = caster->ToPlayer())
        {
            player->ModifySpellCooldown(SPELL_WARLOCK_UNENDING_RESOLVE, -aurEff->GetAmount());
            caster->CastSpell(caster, RUNE_WARLOCK_RESOLUTE_BARRIER_DEBUFF, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_resolute_barrier::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_resolute_barrier::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};




void AddSC_warlock_perks_scripts()
{
    RegisterSpellScript(rune_warl_inquisitors_gaze);
    RegisterSpellScript(rune_warl_inquisitors_gaze_damage);
    RegisterSpellScript(rune_warl_sacrolashs_dark_strike);
    RegisterSpellScript(rune_warl_kazaaks_final_curse);
    RegisterSpellScript(rune_warl_accrued_vitality);
    RegisterSpellScript(rune_warl_claw_of_endereth);
    RegisterSpellScript(rune_warl_funerary_ceremony);
    RegisterSpellScript(rune_warl_funerary_ceremony_apply);
    RegisterSpellScript(rune_warl_everlasting_shadows);
    RegisterSpellScript(rune_warl_fel_synergy);
    RegisterSpellScript(rune_warl_grimoire_of_synergy);
    RegisterSpellScript(rune_warl_grimoire_of_synergy_pet);
    RegisterSpellScript(rune_warl_resolute_barrier);


    


}
