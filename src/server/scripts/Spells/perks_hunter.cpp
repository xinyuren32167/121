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

enum HunterSpells
{

    SPELL_HUNTER_ASPECT_OF_THE_BEAST = 13161,
    SPELL_HUNTER_ASPECT_OF_THE_DRAGONHAWK = 5118,
    SPELL_HUNTER_ASPECT_OF_THE_HAWK = 27044,
    SPELL_HUNTER_ASPECT_OF_THE_MONKEY = 13163,

    SPELL_HUNTER_EXPLOSIVE_TRAP = 80138,
    SPELL_HUNTER_FREEZING_TRAP = 80139,
    SPELL_HUNTER_FROST_TRAP = 13809,
    SPELL_HUNTER_IMMOLATION_TRAP = 49056,
    SPELL_HUNTER_SNAKE_TRAP = 34600,


    SPELL_HUNTER_DISENGAGE = 781,
    SPELL_HUNTER_EXHILARATION = 80161,
    SPELL_HUNTER_FEIGN_DEATH = 5384,

    SPELL_HUNTER_KILL_SHOT = 61006,
    SPELL_HUNTER_SERPENT_STING = 49001,

    RUNE_HUNTER_BULLSEYE_BUFF = 500044,

    RUNE_HUNTER_50CAL_DAMAGE = 500052,

    RUNE_HUNTER_SNIPER_TRAINING_CHECK = 500066,

    RUNE_HUNTER_SERPENT_TOUCH_DAMAGE = 500074,

    RUNE_HUNTER_POISON_INJECTION_DAMAGE = 500094,

    RUNE_HUNTER_REJUVENATING_WIND_HOT = 500258,

    RUNE_HUNTER_REST_IN_PEACE_HEAL = 500314,

    RUNE_HUNTER_KILLER_INSTINCT_DAMAGE = 500366,
};

class rune_hunter_exposed_weakness : public AuraScript
{
    PrepareAuraScript(rune_hunter_exposed_weakness);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_KILL_SHOT, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_exposed_weakness::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_hunter_bullseye : public AuraScript
{
    PrepareAuraScript(rune_hunter_bullseye);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        int32 healthPct = victim->GetHealthPct();

        if (healthPct > GetAura()->GetEffect(EFFECT_1)->GetAmount())
            return;

        GetCaster()->CastSpell(GetCaster(), RUNE_HUNTER_BULLSEYE_BUFF, TRIGGERED_FULL_MASK);

        Aura* buff = GetCaster()->GetAura(RUNE_HUNTER_BULLSEYE_BUFF);
        int32 maxStack = aurEff->GetAmount();

        if (int32 currentStack = buff->GetStackAmount() > maxStack)
            buff->SetStackAmount(maxStack);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_bullseye::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_bullseye::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_50cal : public AuraScript
{
    PrepareAuraScript(rune_hunter_50cal);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_HUNTER_50CAL_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_50cal::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_50cal::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_sniper_training : public AuraScript
{
    PrepareAuraScript(rune_hunter_sniper_training);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Aura* movementCheck = GetCaster()->GetAura(RUNE_HUNTER_SNIPER_TRAINING_CHECK);

        if (!movementCheck)
            return;

        int32 currentStacks = movementCheck->GetStackAmount();

        if (currentStacks < aurEff->GetAmount())
            return;

        int32 buffAura = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;

        if (GetCaster()->HasAura(buffAura))
            return;

        GetCaster()->CastSpell(GetCaster(), buffAura, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_sniper_training::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class rune_hunter_serpent_touch : public AuraScript
{
    PrepareAuraScript(rune_hunter_serpent_touch);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        if (!victim->HasAura(SPELL_HUNTER_SERPENT_STING))
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_HUNTER_SERPENT_TOUCH_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_serpent_touch::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_serpent_touch::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_poison_injection : public SpellScript
{
    PrepareSpellScript(rune_hunter_poison_injection);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500082))
            return GetCaster()->GetAura(500082);

        if (GetCaster()->HasAura(500083))
            return GetCaster()->GetAura(500083);

        if (GetCaster()->HasAura(500084))
            return GetCaster()->GetAura(500084);

        if (GetCaster()->HasAura(500085))
            return GetCaster()->GetAura(500085);

        if (GetCaster()->HasAura(500086))
            return GetCaster()->GetAura(500086);

        if (GetCaster()->HasAura(500087))
            return GetCaster()->GetAura(500087);

        return nullptr;
    }

    void HandleProc()
    {
        if (!GetRuneAura())
            return;

        Unit* victim = GetExplTargetUnit();

        if (!victim)
            return;

        int32 latentPoisonID = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
        Aura* latentPoisonAura = victim->GetAura(latentPoisonID);

        if (!latentPoisonAura)
            return;

        int32 damagePct = latentPoisonAura->GetEffect(EFFECT_0)->GetAmount();
        int32 attackPower = std::max<int32>(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetCaster()->GetTotalAttackPowerValue(RANGED_ATTACK));
        float damage = int32(CalculatePct(attackPower, damagePct));
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_HUNTER_POISON_INJECTION_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
        victim->RemoveAura(latentPoisonID);
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_hunter_poison_injection::HandleProc);
    }
};

class rune_hunter_dance_with_death : public AuraScript
{
    PrepareAuraScript(rune_hunter_dance_with_death);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        int32 criticalChance = std::max<int32>(GetCaster()->GetUnitCriticalChance(BASE_ATTACK, GetCaster()), GetCaster()->GetUnitCriticalChance(RANGED_ATTACK, GetCaster()));
        uint32 random = urand(1, 100);

        return random <= criticalChance;
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_dance_with_death::CheckProc);
    }
};

class rune_hunter_might_of_the_beast : public AuraScript
{
    PrepareAuraScript(rune_hunter_might_of_the_beast);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_ASPECT_OF_THE_BEAST);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Unit* pet = player->GetPet()->ToUnit();

        if (pet)
            GetCaster()->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            GetCaster()->AddAura(procSpell, unit);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_might_of_the_beast::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_might_of_the_beast::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_dragonhawk_focus : public AuraScript
{
    PrepareAuraScript(rune_hunter_dragonhawk_focus);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_ASPECT_OF_THE_DRAGONHAWK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_dragonhawk_focus::CheckProc);
    }
};

class rune_hunter_hawk_quickness : public AuraScript
{
    PrepareAuraScript(rune_hunter_hawk_quickness);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_ASPECT_OF_THE_HAWK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_hawk_quickness::CheckProc);
    }
};

class rune_hunter_monkey_business : public AuraScript
{
    PrepareAuraScript(rune_hunter_monkey_business);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_ASPECT_OF_THE_MONKEY);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_monkey_business::CheckProc);
    }
};

class rune_hunter_ambuscade : public AuraScript
{
    PrepareAuraScript(rune_hunter_ambuscade);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_DISENGAGE, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_ambuscade::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_natural_mending : public AuraScript
{
    PrepareAuraScript(rune_hunter_natural_mending);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Aura* aura = GetAura();
        Player* target = GetTarget()->ToPlayer();

        if (!aura || !target)
            return;

        int32 spellFocus = eventInfo.GetSpellInfo()->ManaCost;

        if (spellFocus <= 0)
            return;

        int32 cooldownReduction = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        int32 focusAccumulated = GetAura()->GetEffect(EFFECT_2)->GetAmount() + spellFocus;
        int32 focusThreshold = aurEff->GetAmount();

        if (focusAccumulated >= focusThreshold)
        {
            target->ModifySpellCooldown(SPELL_HUNTER_EXHILARATION, -cooldownReduction);
            aura->GetEffect(EFFECT_2)->SetAmount(focusAccumulated - focusThreshold);
            focusAccumulated = GetAura()->GetEffect(EFFECT_2)->GetAmount();

            if (focusAccumulated >= focusThreshold)
            {
                target->ModifySpellCooldown(SPELL_HUNTER_EXHILARATION, -cooldownReduction);
                aura->GetEffect(EFFECT_2)->SetAmount(focusAccumulated - focusThreshold);
            }
        }
        else
            aura->GetEffect(EFFECT_2)->SetAmount(focusAccumulated);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_natural_mending::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_natural_mending::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_rejuvenating_wind : public AuraScript
{
    PrepareAuraScript(rune_hunter_rejuvenating_wind);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = GetCaster();

        if (!target)
            return;

        int32 maxHealth = target->GetMaxHealth();
        float heal = int32(CalculatePct(maxHealth, aurEff->GetAmount()));
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_HUNTER_REJUVENATING_WIND_HOT)->GetMaxTicks();
        int32 amount = heal / maxTicks;

        GetCaster()->CastCustomSpell(RUNE_HUNTER_REJUVENATING_WIND_HOT, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_rejuvenating_wind::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_trap_mastery : public AuraScript
{
    PrepareAuraScript(rune_hunter_trap_mastery);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetTarget()->ToPlayer())
        {
            target->ModifySpellCooldown(SPELL_HUNTER_EXPLOSIVE_TRAP, -aurEff->GetAmount());
            target->ModifySpellCooldown(SPELL_HUNTER_FREEZING_TRAP, -aurEff->GetAmount());
            target->ModifySpellCooldown(SPELL_HUNTER_FROST_TRAP, -aurEff->GetAmount());
            target->ModifySpellCooldown(SPELL_HUNTER_IMMOLATION_TRAP, -aurEff->GetAmount());
            target->ModifySpellCooldown(SPELL_HUNTER_SNAKE_TRAP, -aurEff->GetAmount());
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_trap_mastery::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_third_degree_burn : public AuraScript
{
    PrepareAuraScript(rune_hunter_third_degree_burn);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        int32 debuffSpell = GetAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->CastSpell(victim, debuffSpell, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_third_degree_burn::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_third_degree_burn::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_rest_in_peace : public AuraScript
{
    PrepareAuraScript(rune_hunter_rest_in_peace);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_FEIGN_DEATH, true);
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* target = GetCaster();

        if (!target)
            return;

        if (target->GetHealthPct() >= 100)
            return;

        if (!target->HasAura(SPELL_HUNTER_FEIGN_DEATH))
            return;

        int32 maxHealth = target->GetMaxHealth();
        int32 amount = int32(CalculatePct(maxHealth, aurEff->GetAmount()));

        target->CastCustomSpell(RUNE_HUNTER_REST_IN_PEACE_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_rest_in_peace::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_rest_in_peace::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_hunter_resilience_of_the_hunter : public AuraScript
{
    PrepareAuraScript(rune_hunter_resilience_of_the_hunter);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500316))
            return GetCaster()->GetAura(500316);

        if (GetCaster()->HasAura(500317))
            return GetCaster()->GetAura(500317);

        if (GetCaster()->HasAura(500318))
            return GetCaster()->GetAura(500318);

        if (GetCaster()->HasAura(500319))
            return GetCaster()->GetAura(500319);

        if (GetCaster()->HasAura(500320))
            return GetCaster()->GetAura(500320);

        if (GetCaster()->HasAura(500321))
            return GetCaster()->GetAura(500321);

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_resilience_of_the_hunter::HandleRemove, EFFECT_0, SPELL_AURA_FEIGN_DEATH, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_ice_skate : public SpellScript
{
    PrepareSpellScript(rune_hunter_ice_skate);

    void HandleApplyAura(SpellEffIndex effIndex)
    {
        
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(rune_hunter_ice_skate::HandleApplyAura, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
    }
};

class rune_hunter_killer_instinct : public AuraScript
{
    PrepareAuraScript(rune_hunter_killer_instinct);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
        Unit* actor = eventInfo.GetDamageInfo()->GetAttacker();
        int32 healthThreshold = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        LOG_ERROR("error", "killer instinct");
        if (!victim)
            return;
        LOG_ERROR("error", "killer instinct, Victim name = {}, Actor name = {}", victim->GetName(), actor->GetName());
       // if (victim->GetHealthPct() > healthThreshold)
       //     return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;
        LOG_ERROR("error", "killer instinct damage dealt = {}", damageDealt);
        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);
        LOG_ERROR("error", "killer instinct amount = {}", amount);
        if (!actor)
            return;

        actor->CastCustomSpell(RUNE_HUNTER_KILLER_INSTINCT_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* pet = GetCaster()->ToPlayer()->GetPet();

        if (!pet)
            return;

        GetCaster()->AddAura(GetAura()->GetId(), pet);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* pet = GetCaster()->ToPlayer()->GetPet();

        if (!pet)
            return;

        pet->RemoveAura(GetAura()->GetId());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_killer_instinct::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_killer_instinct::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectApply += AuraEffectApplyFn(rune_hunter_killer_instinct::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_killer_instinct::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_killer_instinct_aura : public SpellScript
{
    PrepareSpellScript(rune_hunter_killer_instinct_aura);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500360))
            return GetCaster()->GetAura(500360);

        if (GetCaster()->HasAura(500361))
            return GetCaster()->GetAura(500361);

        if (GetCaster()->HasAura(500362))
            return GetCaster()->GetAura(500362);

        if (GetCaster()->HasAura(500363))
            return GetCaster()->GetAura(500363);

        if (GetCaster()->HasAura(500364))
            return GetCaster()->GetAura(500364);

        if (GetCaster()->HasAura(500365))
            return GetCaster()->GetAura(500365);

        return nullptr;
    }

    void HandleBuff()
    {
        Unit* pet = GetCaster()->ToPlayer()->GetPet();

        if (!pet)
            return;

        if (!GetRuneAura())
        {
            if (pet->HasAura(500360))
                pet->RemoveAura(500360);

            if (pet->HasAura(500361))
                pet->RemoveAura(500361);

            if (pet->HasAura(500362))
                pet->RemoveAura(500362);

            if (pet->HasAura(500363))
                pet->RemoveAura(500363);

            if (pet->HasAura(500364))
                pet->RemoveAura(500364);

            if (pet->HasAura(500365))
                pet->RemoveAura(500365);

            return;
        }
            
        if (pet->HasAura(GetRuneAura()->GetId()))
            return;

        GetCaster()->AddAura(GetRuneAura()->GetId(), pet);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(rune_hunter_killer_instinct_aura::HandleBuff);
    }
};

void AddSC_hunter_perks_scripts()
{
    RegisterSpellScript(rune_hunter_exposed_weakness);
    RegisterSpellScript(rune_hunter_bullseye);
    RegisterSpellScript(rune_hunter_50cal);
    RegisterSpellScript(rune_hunter_sniper_training);
    RegisterSpellScript(rune_hunter_serpent_touch);
    RegisterSpellScript(rune_hunter_poison_injection);
    RegisterSpellScript(rune_hunter_dance_with_death);
    RegisterSpellScript(rune_hunter_might_of_the_beast);
    RegisterSpellScript(rune_hunter_dragonhawk_focus);
    RegisterSpellScript(rune_hunter_hawk_quickness);
    RegisterSpellScript(rune_hunter_monkey_business);
    RegisterSpellScript(rune_hunter_ambuscade);
    RegisterSpellScript(rune_hunter_natural_mending);
    RegisterSpellScript(rune_hunter_rejuvenating_wind);
    RegisterSpellScript(rune_hunter_trap_mastery);
    RegisterSpellScript(rune_hunter_third_degree_burn);
    RegisterSpellScript(rune_hunter_rest_in_peace);
    RegisterSpellScript(rune_hunter_resilience_of_the_hunter);
    RegisterSpellScript(rune_hunter_ice_skate);
    RegisterSpellScript(rune_hunter_killer_instinct);
    RegisterSpellScript(rune_hunter_killer_instinct_aura);


    
}
