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
    SPELL_WARLOCK_AGONY = 83010,
    SPELL_WARLOCK_CORRUPTION = 47813,
    SPELL_WARLOCK_CURSE_OF_ELEMENTS = 47865,
    SPELL_WARLOCK_CURSE_OF_TONGUES = 11719,
    SPELL_WARLOCK_CURSE_OF_WEAKNESS = 50511,
    SPELL_WARLOCK_DOOM = 47867,
    SPELL_WARLOCK_DRAIN_LIFE = 47857,
    SPELL_WARLOCK_DRAIN_SOUL = 47855,
    SPELL_WARLOCK_MALEFIC_RAPTURE = 83020,
    SPELL_WARLOCK_MALEFIC_RAPTURE_DAMAGE = 83021,
    SPELL_WARLOCK_SHADOW_BOLT = 47809,
    SPELL_WARLOCK_UNENDING_RESOLVE = 83016,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION = 47843,

    // Talents
    TALENT_WARLOCK_PLACEHOLDER = 00000,

    // Runes
    RUNE_WARLOCK_INQUISITORS_GAZE_DAMAGE = 800107,
    RUNE_WARLOCK_KAZAAKS_FINAL_CURSE_DAMAGE = 800186,
    RUNE_WARLOCK_ACCRUED_VITALITY_HEAL = 800204,
    RUNE_WARLOCK_CLAW_OF_ENDERETH_HEAL = 800218,
    RUNE_WARLOCK_FEL_SYNERGY_HEAL = 800328,
    RUNE_WARLOCK_RESOLUTE_BARRIER_DEBUFF = 800414,
    RUNE_WARLOCK_STONE_OF_LIFE_HOT = 800476,
    RUNE_WARLOCK_SEIZED_VITALITY_DEBUFF = 800484,
    RUNE_WARLOCK_SEIZED_VITALITY_HEAL = 800485,
    RUNE_WARLOCK_AREA_OF_AFFLICTION_DAMAGE = 800534,
    RUNE_WARLOCK_TORMENTED_CRESCENDO_BUFF = 800572,

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
                    aura->GetEffect(EFFECT_0)->ResetTicks();
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

class rune_warl_soul_conduit : public AuraScript
{
    PrepareAuraScript(rune_warl_soul_conduit);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();

        if (!caster || caster->isDead())
            return;

        if (eventInfo.GetSpellInfo()->PowerType != POWER_ENERGY)
            return;

        int32 spellSoulPower = eventInfo.GetSpellInfo()->CalcPowerCost(caster, eventInfo.GetSchoolMask()) / 10;

        if (spellSoulPower <= 0)
            return;

        int32 procChance = aurEff->GetAmount();
        int32 amount = 0;

        for (size_t i = 0; i < spellSoulPower; i++)
            if (roll_chance_i(procChance))
                amount++;

        if (amount == 0)
            return;

        caster->EnergizeBySpell(caster, GetSpellInfo()->Id, amount, POWER_ENERGY);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_soul_conduit::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_soul_conduit::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_pandemic_invocation : public AuraScript
{
    PrepareAuraScript(rune_warl_pandemic_invocation);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetActionTarget();

        if (!target || target->isDead())
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;
        int32 durationThreshold = GetEffect(EFFECT_1)->GetAmount();

        if (Aura* dotAura = target->GetAura(spellID))
            if (dotAura->GetDuration() > durationThreshold)
                return;

        int32 procSpell = GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        caster->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);

        int32 procChance = aurEff->GetAmount();
        int32 soulPower = GetEffect(EFFECT_2)->GetAmount();

            if (roll_chance_i(procChance))
                caster->EnergizeBySpell(caster, GetSpellInfo()->Id, soulPower, POWER_ENERGY);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_pandemic_invocation::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_pandemic_invocation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_stone_of_life : public AuraScript
{
    PrepareAuraScript(rune_warl_stone_of_life);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 amount = caster->CountPctFromMaxHealth(aurEff->GetAmount());
        amount /= 8;

        caster->CastCustomSpell(RUNE_WARLOCK_STONE_OF_LIFE_HOT, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_stone_of_life::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_stone_of_life::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_seized_vitality : public AuraScript
{
    PrepareAuraScript(rune_warl_seized_vitality);

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
        
        caster->CastCustomSpell(RUNE_WARLOCK_SEIZED_VITALITY_DEBUFF, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_seized_vitality::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_seized_vitality::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_seized_vitality_heal : public AuraScript
{
    PrepareAuraScript(rune_warl_seized_vitality_heal);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 amount = aurEff->GetAmount();

        caster->CastCustomSpell(RUNE_WARLOCK_SEIZED_VITALITY_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_warl_seized_vitality_heal::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_warl_haunted_soul : public AuraScript
{
    PrepareAuraScript(rune_warl_haunted_soul);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        auto const& enemyList = caster->getAttackers();
        int32 hauntedEnemiesNbr = 0;

        for (auto const& enemy : enemyList)
        {
            if (enemy->isDead())
                continue;

            auto targetAuras = enemy->GetAppliedAuras();

            for (auto itr = targetAuras.begin(); itr != targetAuras.end(); ++itr)
                if (Aura* aura = itr->second->GetBase())
                {
                    if (aura->GetCaster() != caster)
                        continue;

                    SpellInfo const* auraInfo = aura->GetSpellInfo();

                    if (auraInfo->SpellFamilyName == SPELLFAMILY_WARLOCK && auraInfo->SpellFamilyFlags[2] & 0x80000000)
                    {
                        aura->GetEffect(EFFECT_0)->CalculatePeriodic(caster);
                        aura->GetEffect(EFFECT_0)->ResetTicks();
                    }
                }
        }
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(rune_warl_haunted_soul::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_warl_doom_blossom : public AuraScript
{
    PrepareAuraScript(rune_warl_doom_blossom);

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

        if (!target->HasAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION))
            return;

        int32 procSpell = aurEff->GetAmount();
        caster->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_doom_blossom::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_doom_blossom::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_area_of_affliction : public AuraScript
{
    PrepareAuraScript(rune_warl_area_of_affliction);

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

        caster->CastCustomSpell(RUNE_WARLOCK_AREA_OF_AFFLICTION_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_area_of_affliction::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_area_of_affliction::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_tourmented_crescendo : public AuraScript
{
    PrepareAuraScript(rune_warl_tourmented_crescendo);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        if (!target->HasAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION) || !target->HasAura(SPELL_WARLOCK_AGONY) || !target->HasAura(SPELL_WARLOCK_CORRUPTION))
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;
        int32 procChance = 0;

        if (spellID == SPELL_WARLOCK_SHADOW_BOLT)
            procChance = aurEff->GetAmount();

        if (spellID == SPELL_WARLOCK_DRAIN_SOUL)
            procChance = GetEffect(EFFECT_1)->GetAmount();

        if (!roll_chance_i(procChance))
            return;

        caster->CastSpell(caster, RUNE_WARLOCK_TORMENTED_CRESCENDO_BUFF, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_tourmented_crescendo::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_tourmented_crescendo::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_dread_touch : public AuraScript
{
    PrepareAuraScript(rune_warl_dread_touch);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        if (!target->HasAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION) && !target->HasAura(SPELL_WARLOCK_AGONY) || !target->HasAura(SPELL_WARLOCK_CORRUPTION))
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;
        int32 procChance = 0;

        if (spellID == SPELL_WARLOCK_MALEFIC_RAPTURE_DAMAGE)
        {
            procChance = aurEff->GetAmount();
        }
            

        if (spellID == SPELL_WARLOCK_DRAIN_SOUL)
            procChance = GetEffect(EFFECT_1)->GetAmount();

        if (!roll_chance_i(procChance))
            return;

        caster->CastSpell(caster, RUNE_WARLOCK_TORMENTED_CRESCENDO_BUFF, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_dread_touch::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_dread_touch::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
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
    RegisterSpellScript(rune_warl_soul_conduit);
    RegisterSpellScript(rune_warl_pandemic_invocation);
    RegisterSpellScript(rune_warl_stone_of_life);
    RegisterSpellScript(rune_warl_seized_vitality);
    RegisterSpellScript(rune_warl_seized_vitality_heal);
    RegisterSpellScript(rune_warl_haunted_soul);
    RegisterSpellScript(rune_warl_doom_blossom);
    RegisterSpellScript(rune_warl_area_of_affliction);
    RegisterSpellScript(rune_warl_tourmented_crescendo);

    
    
    
    
}
