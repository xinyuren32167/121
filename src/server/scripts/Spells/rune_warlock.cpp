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
    SPELL_WARLOCK_DEMONIC_CORE_BUFF = 83029,
    SPELL_WARLOCK_DOOM = 47867,
    SPELL_WARLOCK_DRAIN_LIFE = 47857,
    SPELL_WARLOCK_DRAIN_SOUL = 47855,
    SPELL_WARLOCK_MALEFIC_RAPTURE = 83020,
    SPELL_WARLOCK_MALEFIC_RAPTURE_DAMAGE = 83021,
    SPELL_WARLOCK_SHADOW_BOLT = 47809,
    SPELL_WARLOCK_SUMMON_DARK_GLARE = 83049,
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
    RUNE_WARLOCK_DREAD_TOUCH_BUFF = 800584,
    RUNE_WARLOCK_DREAD_TOUCH_DAMAGE = 800585,
    RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER = 800716,
    RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER_BUFF = 800717,
    RUNE_WARLOCK_THE_HOUNDMASTERS_STRATAGEM_LISTENER = 800774,
    RUNE_WARLOCK_THE_HOUNDMASTERS_STRATAGEM_DAMAGE = 800775,

    // Pet Scaling
    PET_SCALING_WARLOCK_DAMAGE_HASTE = 83205,
    PET_SCALING_WARLOCK_STAMINA_DAMAGEREDUC_ARMOR = 83206,
    PET_SCALING_WARLOCK_INTELLECT_ALLRES = 83207,


};

enum WarlockPets
{
    PET_WARLOCK_FELGUARD = 17252,

    // Runes
    PET_WARLOCK_INQUISITORS_EYE = 800000,
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
        Creature* summon = caster->SummonCreature(PET_WARLOCK_INQUISITORS_EYE, pos, TEMPSUMMON_TIMED_DESPAWN, aurEff->GetAmount(), 0, properties);

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

class rune_warl_soul_corruption_target : public SpellScript
{
    PrepareSpellScript(rune_warl_soul_corruption_target);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetTarget());

        if (!target || target->isDead())
            return;

        targets.remove(target);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_warl_soul_corruption_target::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
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
        int32 amount = CalculatePct(damage, aurEff->GetAmount());

        if (Aura* healAura = caster->GetAura(RUNE_WARLOCK_ACCRUED_VITALITY_HEAL))
        {
            int32 amountLeft = healAura->GetEffect(EFFECT_0)->GetAmount() * healAura->GetEffect(EFFECT_0)->GetRemaningTicks();
            amount += amountLeft;
            healAura->Remove();
        }

        amount /= 10;

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

        int32 spellSoulPower = eventInfo.GetSpellInfo()->CalcPowerCost(caster, eventInfo.GetSchoolMask());

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
       
        int32 procChance = aurEff->GetAmount();
        int32 procSpell = GetEffect(EFFECT_1)->GetAmount();
        int32 soulPower = GetEffect(EFFECT_2)->GetAmount();
      
        if (roll_chance_i(procChance))
        {
            caster->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
            caster->EnergizeBySpell(caster, GetSpellInfo()->Id, soulPower, POWER_ENERGY);
        }          
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
                        aura->GetEffect(EFFECT_0)->RecalculateAmount();
                }
        }
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(rune_warl_haunted_soul::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(rune_warl_haunted_soul::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_warl_doom_blossom : public AuraScript
{
    PrepareAuraScript(rune_warl_doom_blossom);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
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

class rune_warl_area_of_affliction_target : public SpellScript
{
    PrepareSpellScript(rune_warl_area_of_affliction_target);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetTarget());

        if (!target || target->isDead())
            return;

        targets.remove(target);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_warl_area_of_affliction_target::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
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

        if (!target->HasAura(RUNE_WARLOCK_DREAD_TOUCH_BUFF))
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_WARLOCK_DREAD_TOUCH_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_dread_touch::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_dread_touch::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_grand_warlocks_design : public AuraScript
{
    PrepareAuraScript(rune_warl_grand_warlocks_design);

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

        int32 spellSoulPower = eventInfo.GetSpellInfo()->CalcPowerCost(caster, eventInfo.GetSchoolMask());

        if (spellSoulPower <= 0)
            return;

        int32 powerAccumulated = GetEffect(EFFECT_2)->GetAmount() + spellSoulPower;
        int32 powerThreshold = aurEff->GetAmount();
        int32 cooldown = GetEffect(EFFECT_1)->GetAmount();

        if (Player* player = caster->ToPlayer())
        {
            while (powerAccumulated >= powerThreshold)
            {
                player->ModifySpellCooldown(SPELL_WARLOCK_SUMMON_DARK_GLARE, -cooldown);

                powerAccumulated -= powerThreshold;
            }
        }


        GetEffect(EFFECT_2)->SetAmount(powerAccumulated);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_grand_warlocks_design::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_grand_warlocks_design::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_demonic_knowledge : public AuraScript
{
    PrepareAuraScript(rune_warl_demonic_knowledge);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();

        if (!caster || caster->isDead())
            return;

        caster->CastCustomSpell(SPELL_WARLOCK_DEMONIC_CORE_BUFF, SPELLVALUE_AURA_CHARGE, 1, caster, true);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_demonic_knowledge::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_demonic_knowledge::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_dread_calling : public AuraScript
{
    PrepareAuraScript(rune_warl_dread_calling);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetActor();

        if (!caster || caster->isDead())
            return;

        int32 soulPower = std::min<int32>(caster->GetPower(POWER_ENERGY), 15);;

        if (soulPower <= 0)
            return;

        int32 powerThreshold = aurEff->GetAmount();
        int32 stackNbr = 0;

        if (Player* player = caster->ToPlayer())
        {
            while (soulPower >= powerThreshold)
            {
                stackNbr++;

                soulPower -= powerThreshold;
            }
        }

        if (stackNbr == 0)
            return;

        int32 procSpell = GetEffect(EFFECT_1)->GetAmount();

        if (caster->HasAura(procSpell))
            caster->RemoveAura(procSpell);

        Aura* buff = caster->AddAura(procSpell, caster);
        buff->SetStackAmount(stackNbr);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_dread_calling::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_dread_calling::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warl_immutable_hatred : public AuraScript
{
    PrepareAuraScript(rune_warl_immutable_hatred);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            Pet* pet = player->GetPet();

            if (!pet || pet->isDead())
                return;

            if (pet->GetEntry() == PET_WARLOCK_FELGUARD)
                caster->CastSpell(pet, RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER, TRIGGERED_FULL_MASK);
        }
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            Pet* pet = player->GetPet();

            if (!pet || pet->isDead())
                return;

            if (pet->GetEntry() != PET_WARLOCK_FELGUARD)
                return;

            if (pet->HasAura(RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER))
                return;

            caster->CastSpell(pet, RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER, TRIGGERED_FULL_MASK);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            Pet* pet = player->GetPet();

            if (!pet || pet->isDead())
                return;

            if (pet->HasAura(RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER))
                pet->RemoveAura(RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER);
        }
    }

    void Register()
    {
        OnEffectApply += AuraEffectApplyFn(rune_warl_immutable_hatred::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_warl_immutable_hatred::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_warl_immutable_hatred::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_warl_immutable_hatred_proc : public AuraScript
{
    PrepareAuraScript(rune_warl_immutable_hatred_proc);

    Aura* GetImmutableHatredAura(Unit* caster)
    {
        for (size_t i = 800704; i < 800710; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* pet = GetUnitOwner();

        if (!pet || pet->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        if (Aura* runeAura = GetImmutableHatredAura(caster))
        {
            int32 maxStack = runeAura->GetEffect(EFFECT_1)->GetAmount();
            int32 procSpell = runeAura->GetEffect(EFFECT_0)->GetAmount();

            if (!oldTarget || oldTarget != target)
            {
                if (Aura* listenerBuff = pet->GetAura(RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER_BUFF))
                    listenerBuff->SetStackAmount(1);
                else
                    pet->CastSpell(pet, RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER_BUFF, TRIGGERED_FULL_MASK);

                oldTarget = target;
                return;
            }

            if (oldTarget == target)
            {
                pet->CastSpell(pet, RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER_BUFF, TRIGGERED_FULL_MASK);

                if (Aura* listenerBuff = pet->GetAura(RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER_BUFF))
                    if (listenerBuff->GetStackAmount() == maxStack)
                    {
                        listenerBuff->Remove();
                        pet->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
                    }
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_immutable_hatred_proc::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_immutable_hatred_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
private:
    Unit* oldTarget;
};

class rune_warl_the_houndmasters_stratagem : public AuraScript
{
    PrepareAuraScript(rune_warl_the_houndmasters_stratagem);

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

        if (!target->HasAura(RUNE_WARLOCK_THE_HOUNDMASTERS_STRATAGEM_LISTENER))
            return;

        int32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
        caster->CastCustomSpell(RUNE_WARLOCK_THE_HOUNDMASTERS_STRATAGEM_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warl_the_houndmasters_stratagem::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warl_the_houndmasters_stratagem::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};



void AddSC_warlock_perks_scripts()
{
    RegisterSpellScript(rune_warl_inquisitors_gaze);
    RegisterSpellScript(rune_warl_inquisitors_gaze_damage);
    RegisterSpellScript(rune_warl_soul_corruption_target);
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
    RegisterSpellScript(rune_warl_area_of_affliction_target);
    RegisterSpellScript(rune_warl_tourmented_crescendo);
    RegisterSpellScript(rune_warl_dread_touch); 
    RegisterSpellScript(rune_warl_grand_warlocks_design);
    RegisterSpellScript(rune_warl_demonic_knowledge);
    RegisterSpellScript(rune_warl_dread_calling);
    RegisterSpellScript(rune_warl_immutable_hatred);
    RegisterSpellScript(rune_warl_immutable_hatred_proc);
    RegisterSpellScript(rune_warl_the_houndmasters_stratagem);

    


    
}
