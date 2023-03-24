#include "PetDefines.h"
#include "Player.h"
#include "Pet.h"
#include "Creature.h"
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
    SPELL_HUNTER_ASPECT_OF_THE_WILD = 80130,

    SPELL_HUNTER_AIMED_SHOT = 49050,
    SPELL_HUNTER_ARCANE_SHOT = 49045,

    SPELL_HUNTER_BARBED_SHOT = 80172,
    SPELL_HUNTER_BARBED_SHOT_FRENZY = 80174,

    SPELL_HUNTER_BESTIAL_WRATH = 80133,
    SPELL_HUNTER_BESTIAL_WRATH_AURA = 80132,
    SPELL_HUNTER_BLACK_ARROW = 63672,
    SPELL_HUNTER_BUTCHERY = 80191,
    SPELL_HUNTER_BURSTING_SHOT = 80184,


    SPELL_HUNTER_EXPLOSIVE_TRAP = 80138,
    SPELL_HUNTER_FREEZING_TRAP = 80139,
    SPELL_HUNTER_FROST_TRAP = 13809,
    SPELL_HUNTER_IMMOLATION_TRAP = 49056,
    SPELL_HUNTER_IMMOLATION_TRAP_DOT = 49054,
    SPELL_HUNTER_SNAKE_TRAP = 34600,
    SPELL_HUNTER_SNAKE_TRAP_RANGED = 34601,

    SPELL_HUNTER_CALL_OF_THE_WILD = 80186,
    SPELL_HUNTER_DISENGAGE = 781,
    SPELL_HUNTER_EXHILARATION = 80161,

    SPELL_HUNTER_FEIGN_DEATH = 5384,
    SPELL_HUNTER_FLANKING_STRIKE = 80197,
    SPELL_HUNTER_FURY_OF_THE_EAGLE = 80194,

    SPELL_HUNTER_KILL_COMMAND = 80141,
    SPELL_HUNTER_KILL_SHOT = 61006,

    SPELL_HUNTER_RABID_AURA = 53401,
    SPELL_HUNTER_RAPID_FIRE = 80146,
    SPELL_HUNTER_RAPID_FIRE_DAMAGE = 80147,

    SPELL_HUNTER_SERPENT_STING = 49001,
    SPELL_HUNTER_SPEARHEAD_AURA = 80208,
    SPELL_HUNTER_STEADY_SHOT = 49052,
    SPELL_HUNTER_TRUESHOT = 80148,

    SPELL_HUNTER_WAILING_ARROW_AOE = 80149,
    SPELL_HUNTER_WAILING_ARROW_ST = 80150,
    SPELL_HUNTER_WEAK_SPOT = 80160,
    SPELL_HUNTER_WILDFIRE_BOMB = 80188,

    SPELL_HUNTER_WIND_ARROW = 80225,
    SPELL_HUNTER_WIND_ARROW_DAMAGE = 80226,

    RUNE_HUNTER_BULLSEYE_BUFF = 500044,

    RUNE_HUNTER_50CAL_DAMAGE = 500052,

    RUNE_HUNTER_SNIPER_TRAINING_CHECK = 500066,

    RUNE_HUNTER_SERPENT_TOUCH_DAMAGE = 500074,

    RUNE_HUNTER_POISON_INJECTION_DAMAGE = 500094,

    RUNE_HUNTER_REJUVENATING_WIND_HOT = 500258,

    RUNE_HUNTER_REST_IN_PEACE_HEAL = 500314,

    RUNE_HUNTER_KILLER_INSTINCT_DAMAGE = 500366,

    RUNE_HUNTER_PLAYING_WITH_MATCHES_AOE = 500352,

    RUNE_HUNTER_CLEAVE_COMMAND_DAMAGE = 500380,

    RUNE_HUNTER_RAZOR_FRAGMENTS_DAMAGE = 500442,

    RUNE_HUNTER_BEAST_CLEAVE_DAMAGE = 500480,

    RUNE_HUNTER_TRICK_SHOTS_LISTENER = 500518,

    RUNE_HUNTER_ASPECT_OF_THE_STORM_DAMAGE = 500538,

    RUNE_HUNTER_BLACK_MAMBA_DOT = 500570,
    RUNE_HUNTER_BLACK_MAMBA_DAMAGE = 500571,

    RUNE_HUNTER_STOMP_DAMAGE = 500730,

    RUNE_HUNTER_BLOODTHIRSTY_WRATH_HEAL = 500750,
    RUNE_HUNTER_BLOODTHIRSTY_WRATH_SHIELD = 500751,

    RUNE_HUNTER_THUNDERSLASH_DAMAGE = 500814,

    RUNE_HUNTER_POISON_FANGS_DOT = 500900,
    RUNE_HUNTER_POISON_FANGS_DAMAGE = 500901,

    RUNE_HUNTER_TRUE_AIMED_SHOT_BUFF = 500908,
    RUNE_HUNTER_TRUE_AIMED_SHOT_CHECK = 500909,

    RUNE_HUNTER_DOUBLE_IMPACT_DAMAGE = 500928,

    RUNE_HUNTER_LETHAL_AMMUNITION_BUFF = 500986,

    RUNE_HUNTER_QUICK_LOAD_LISTENER = 501059,

    RUNE_HUNTER_STEADFAST_FOCUS_LISTENER = 501030,

    RUNE_HUNTER_ON_THE_TRAIL_DOT = 501058,

    RUNE_HUNTER_ECHOES_OF_ONHARA_DAMAGE = 501066,

    RUNE_HUNTER_STEADY_FOCUS_LISTENER = 501174,

    RUNE_HUNTER_FOCALISED_TRUESHOT_BUFF = 501200,
    RUNE_HUNTER_FOCALISED_TRUESHOT_LISTENER = 501201,

    RUNE_HUNTER_UNERRING_VISION_BUFF = 501214,

    RUNE_HUNTER_RUTHLESS_MARAUDER_BUFF = 501410,

    RUNE_HUNTER_VOLATILE_BOMB_DAMAGE = 501520,

    RUNE_HUNTER_SHRAPNEL_BOMB_DOT = 501534,
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

        if (!victim || victim->isDead())
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

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_HUNTER_50CAL_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_IGNORE_AURA_SCALING);
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
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Aura* movementCheck = caster->GetAura(RUNE_HUNTER_SNIPER_TRAINING_CHECK);

        if (!movementCheck)
            return;

        int32 currentStacks = movementCheck->GetStackAmount();

        if (currentStacks < aurEff->GetAmount())
            return;

        int32 buffAura = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;

        if (caster->HasAura(buffAura))
            return;

        caster->CastSpell(caster, buffAura, TRIGGERED_FULL_MASK);
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

        if (!victim || victim->isDead())
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
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        Unit* victim = GetExplTargetUnit();

        if (!victim || victim->isDead())
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
        if (!eventInfo.GetDamageInfo())
            return false;

        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return false;

        int32 criticalChance = std::max<int32>(GetCaster()->GetUnitCriticalChance(BASE_ATTACK, victim), GetCaster()->GetUnitCriticalChance(RANGED_ATTACK, victim));
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

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->IsAlive())
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
        Aura* runeAura = GetAura();
        Player* target = GetTarget()->ToPlayer();

        if (!runeAura || !target || target->isDead())
            return;

        int32 spellFocus = eventInfo.GetSpellInfo()->ManaCost;

        if (spellFocus <= 0)
            return;

        int32 cooldownReduction = runeAura->GetEffect(EFFECT_1)->GetAmount();
        int32 focusAccumulated = runeAura->GetEffect(EFFECT_2)->GetAmount() + spellFocus;
        int32 focusThreshold = aurEff->GetAmount();

        if (focusAccumulated >= focusThreshold)
        {
            target->ModifySpellCooldown(SPELL_HUNTER_TRUESHOT, -cooldownReduction);
            focusAccumulated -= focusThreshold;

            if (focusAccumulated >= focusThreshold)
            {
                target->ModifySpellCooldown(SPELL_HUNTER_TRUESHOT, -cooldownReduction);
                focusAccumulated -= focusThreshold;
            }
        }

        runeAura->GetEffect(EFFECT_2)->SetAmount(focusAccumulated);
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

        if (!target || target->isDead())
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

        if (!victim || victim->isDead())
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

        if (!target || target->isDead())
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
        if (!GetRuneAura() || GetCaster()->isDead())
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

    Aura* GetRuneAura()
    {
        for (size_t i = 500328; i < 500334; i++)
        {
            if (GetOriginalCaster()->HasAura(i))
                return GetOriginalCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleApplyAura()
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        Unit* unit = GetExplTargetUnit();

        if (!unit || !unit->IsAlive())
            return;

        Position dest = unit->GetPosition();
        int32 buffAreaAura = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetOriginalCaster()->CastSpell(dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(), buffAreaAura, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(rune_hunter_ice_skate::HandleApplyAura);
    }
};

class rune_hunter_playing_with_matches : public SpellScript
{
    PrepareSpellScript(rune_hunter_playing_with_matches);

    Aura* GetRuneAura()
    {
        for (size_t i = 500346; i < 500352; i++)
        {
            if (GetOriginalCaster()->HasAura(i))
                return GetOriginalCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleApplyAura()
    {
        LOG_ERROR("error", "proc check 1");
        if (!GetRuneAura() || GetCaster()->isDead())
            return;
        LOG_ERROR("error", "proc check caster + rune");
        Unit* unit = GetExplTargetUnit();

        if (!unit || unit->isDead())
            return;
        LOG_ERROR("error", "proc check target");
        Position dest = unit->GetPosition();

        GetOriginalCaster()->CastSpell(dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(), RUNE_HUNTER_PLAYING_WITH_MATCHES_AOE, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(rune_hunter_playing_with_matches::HandleApplyAura);
    }
};

class rune_hunter_playing_with_matches_proc : public SpellScript
{
    PrepareSpellScript(rune_hunter_playing_with_matches_proc);

    void HandleApplyAura(SpellEffIndex effIndex)
    {
        Unit* unit = GetExplTargetUnit();

        if (!unit || !unit->IsAlive())
            return;

        GetOriginalCaster()->CastSpell(unit, SPELL_HUNTER_IMMOLATION_TRAP_DOT, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(rune_hunter_playing_with_matches_proc::HandleApplyAura, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class rune_hunter_killer_instinct : public SpellScript
{
    PrepareSpellScript(rune_hunter_killer_instinct);

    Aura* GetRuneAura()
    {
        Unit* owner = GetCaster()->GetOwner();

        if (owner->HasAura(500360))
            return owner->GetAura(500360);

        if (owner->HasAura(500361))
            return owner->GetAura(500361);

        if (owner->HasAura(500362))
            return owner->GetAura(500362);

        if (owner->HasAura(500363))
            return owner->GetAura(500363);

        if (owner->HasAura(500364))
            return owner->GetAura(500364);

        if (owner->HasAura(500365))
            return owner->GetAura(500365);

        return nullptr;
    }


    void HandleDummy(SpellEffIndex effIndex)
    {
        Aura* rune = GetRuneAura();

        if (!rune)
            return;

        Unit* victim = GetHitUnit();
        int32 pct = rune->GetEffect(EFFECT_0)->GetAmount();
        int32 healthThreshold = rune->GetEffect(EFFECT_1)->GetAmount();

        if (!victim->IsAlive())
            return;

        if (victim->GetHealthPct() <= healthThreshold) {
            int32 damage = GetEffectValue();
            int32 finalDamage = CalculatePct(damage, pct);
            GetCaster()->CastCustomSpell(RUNE_HUNTER_KILLER_INSTINCT_DAMAGE, SPELLVALUE_BASE_POINT0, finalDamage, victim, TRIGGERED_IGNORE_AURA_SCALING);
        }
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(rune_hunter_killer_instinct::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class rune_hunter_cleave_command_proc : public AuraScript
{
    PrepareAuraScript(rune_hunter_cleave_command_proc);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->IsAlive())
            GetCaster()->AddAura(procSpell, pet);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_cleave_command_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_cleave_command : public AuraScript
{
    PrepareAuraScript(rune_hunter_cleave_command);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        eventInfo.GetActor()->CastCustomSpell(RUNE_HUNTER_CLEAVE_COMMAND_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_IGNORE_AURA_SCALING);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_cleave_command::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_cleave_command::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_strength_of_the_pack : public AuraScript
{
    PrepareAuraScript(rune_hunter_strength_of_the_pack);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        player->AddAura(procSpell, player);

        Pet* pet = player->GetPet();

        if (pet && pet->IsAlive())
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
        OnEffectProc += AuraEffectProcFn(rune_hunter_strength_of_the_pack::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_quick_shot : public SpellScript
{
    PrepareSpellScript(rune_hunter_quick_shot);

    Aura* GetRuneAura()
    {
        if (GetOriginalCaster()->HasAura(500394))
            return GetOriginalCaster()->GetAura(500394);

        if (GetOriginalCaster()->HasAura(500395))
            return GetOriginalCaster()->GetAura(500395);

        if (GetOriginalCaster()->HasAura(500396))
            return GetOriginalCaster()->GetAura(500396);

        if (GetOriginalCaster()->HasAura(500397))
            return GetOriginalCaster()->GetAura(500397);

        if (GetOriginalCaster()->HasAura(500398))
            return GetOriginalCaster()->GetAura(500398);

        if (GetOriginalCaster()->HasAura(500399))
            return GetOriginalCaster()->GetAura(500399);

        return nullptr;
    }

    void HandleProc()
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        Unit* victim = GetExplTargetUnit();

        if (!victim || victim->isDead())
            return;

        if (Player* caster = GetCaster()->ToPlayer())
        {
            int32 coolDown = caster->GetSpellCooldownDelay(SPELL_HUNTER_ARCANE_SHOT);
            int32 arcaneShotCooldown = sSpellMgr->GetSpellInfo(SPELL_HUNTER_ARCANE_SHOT)->GetRecoveryTime();
            int32 cooldownReduction = arcaneShotCooldown - coolDown;

            caster->CastSpell(victim, SPELL_HUNTER_ARCANE_SHOT, TRIGGERED_FULL_MASK);

            caster->ModifySpellCooldown(SPELL_HUNTER_ARCANE_SHOT, -cooldownReduction);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(rune_hunter_quick_shot::HandleProc);
    }
};

class rune_hunter_cobra_senses : public AuraScript
{
    PrepareAuraScript(rune_hunter_cobra_senses);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_KILL_COMMAND, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_cobra_senses::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_deathblow : public AuraScript
{
    PrepareAuraScript(rune_hunter_deathblow);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo()->GetSpellInfo())
            return;

        int32 spellID = eventInfo.GetDamageInfo()->GetSpellInfo()->Id;
        Player* caster = GetCaster()->ToPlayer();

        if (!caster || caster->isDead())
            return;

        if (spellID != SPELL_HUNTER_RAPID_FIRE_DAMAGE && spellID != SPELL_HUNTER_AIMED_SHOT)
            return;

        uint32 random = urand(1, 100);
        int32 procChance = 0;

        if (spellID == SPELL_HUNTER_RAPID_FIRE_DAMAGE && eventInfo.GetHitMask() == PROC_EX_CRITICAL_HIT)
            procChance = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (spellID == SPELL_HUNTER_AIMED_SHOT)
            procChance = GetAura()->GetEffect(EFFECT_0)->GetAmount();

        if (random > procChance)
            return;

        caster->RemoveSpellCooldown(SPELL_HUNTER_KILL_SHOT, true);
        caster->AddAura(SPELL_HUNTER_WEAK_SPOT, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_deathblow::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_deathblow::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_razor_fragments_trick_shots : public AuraScript
{
    PrepareAuraScript(rune_hunter_razor_fragments_trick_shots);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500430))
            return GetCaster()->GetAura(500430);

        if (GetCaster()->HasAura(500431))
            return GetCaster()->GetAura(500431);

        if (GetCaster()->HasAura(500432))
            return GetCaster()->GetAura(500432);

        if (GetCaster()->HasAura(500433))
            return GetCaster()->GetAura(500433);

        if (GetCaster()->HasAura(500434))
            return GetCaster()->GetAura(500434);

        if (GetCaster()->HasAura(500435))
            return GetCaster()->GetAura(500435);

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 procAura = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->AddAura(procAura, GetCaster());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_razor_fragments_trick_shots::HandleRemove, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_razor_fragments_weak_spot : public AuraScript
{
    PrepareAuraScript(rune_hunter_razor_fragments_weak_spot);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500430))
            return GetCaster()->GetAura(500430);

        if (GetCaster()->HasAura(500431))
            return GetCaster()->GetAura(500431);

        if (GetCaster()->HasAura(500432))
            return GetCaster()->GetAura(500432);

        if (GetCaster()->HasAura(500433))
            return GetCaster()->GetAura(500433);

        if (GetCaster()->HasAura(500434))
            return GetCaster()->GetAura(500434);

        if (GetCaster()->HasAura(500435))
            return GetCaster()->GetAura(500435);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 procAura = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->AddAura(procAura, GetCaster());
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_razor_fragments_weak_spot::HandleProc, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_razor_fragments : public AuraScript
{
    PrepareAuraScript(rune_hunter_razor_fragments);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_HUNTER_RAZOR_FRAGMENTS_DAMAGE)->GetMaxTicks();
        int32 amount = damage / maxTicks;

        GetCaster()->CastCustomSpell(RUNE_HUNTER_RAZOR_FRAGMENTS_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
        GetCaster()->RemoveAura(GetAura());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_razor_fragments::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_razor_fragments::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_good_health : public AuraScript
{
    PrepareAuraScript(rune_hunter_good_health);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
        int32 procPct = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (!pet || pet->isDead())
            return;

        if (pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        float health = pet->GetMaxHealth();
        int32 amount = CalculatePct(health, procPct);

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, pet, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_good_health::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_beast_cleave_proc : public AuraScript
{
    PrepareAuraScript(rune_hunter_beast_cleave_proc);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->IsAlive())
            GetCaster()->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            GetCaster()->AddAura(procSpell, unit);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_beast_cleave_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_beast_cleave : public AuraScript
{
    PrepareAuraScript(rune_hunter_beast_cleave);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        eventInfo.GetActor()->CastCustomSpell(RUNE_HUNTER_BEAST_CLEAVE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_beast_cleave::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_beast_cleave::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_trick_shots_listener : public SpellScript
{
    PrepareSpellScript(rune_hunter_trick_shots_listener);

    void HandleApplyAura()
    {
        if (GetCaster()->HasAura(RUNE_HUNTER_TRICK_SHOTS_LISTENER))
            GetCaster()->RemoveAura(RUNE_HUNTER_TRICK_SHOTS_LISTENER);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(rune_hunter_trick_shots_listener::HandleApplyAura);
    }
};

class rune_hunter_trick_shots : public AuraScript
{
    PrepareAuraScript(rune_hunter_trick_shots);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 buffAura = aurEff->GetAmount();

        if (GetCaster()->HasAura(buffAura))
            return;

        GetCaster()->CastSpell(GetCaster(), RUNE_HUNTER_TRICK_SHOTS_LISTENER, TRIGGERED_FULL_MASK);

        Aura* listener = GetCaster()->GetAura(RUNE_HUNTER_TRICK_SHOTS_LISTENER);

        if (listener->GetStackAmount() < 3)
            return;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_trick_shots::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_trick_shots::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_aspect_of_the_storm : public AuraScript
{
    PrepareAuraScript(rune_hunter_aspect_of_the_storm);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        int32 attackPower = std::max<int32>(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetCaster()->GetTotalAttackPowerValue(RANGED_ATTACK));
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->IsAlive())
            pet->CastCustomSpell(RUNE_HUNTER_ASPECT_OF_THE_STORM_DAMAGE, SPELLVALUE_BASE_POINT0, damage, pet, TRIGGERED_FULL_MASK);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            unit->CastCustomSpell(RUNE_HUNTER_ASPECT_OF_THE_STORM_DAMAGE, SPELLVALUE_BASE_POINT0, damage, unit, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_aspect_of_the_storm::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_black_mamba_proc : public AuraScript
{
    PrepareAuraScript(rune_hunter_black_mamba_proc);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            int32 displayID = unit->GetNativeDisplayId();

            if (displayID != 6303 && displayID != 2958 && displayID != 1206 && displayID != 2957)
                continue;

            GetCaster()->AddAura(procSpell, unit);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_black_mamba_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_black_mamba : public AuraScript
{
    PrepareAuraScript(rune_hunter_black_mamba);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
        Unit* pet = eventInfo.GetActor();
        int32 attackPower = std::max<int32>(player->GetTotalAttackPowerValue(BASE_ATTACK), player->GetTotalAttackPowerValue(RANGED_ATTACK));
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        if (!victim || !player || !pet)
            return;

        if (Aura* dot = victim->GetAura(RUNE_HUNTER_BLACK_MAMBA_DOT))
        {
            if (dot->GetStackAmount() >= 5)
                player->CastCustomSpell(RUNE_HUNTER_BLACK_MAMBA_DAMAGE, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);
        }

        player->CastCustomSpell(RUNE_HUNTER_BLACK_MAMBA_DOT, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_black_mamba::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_black_mamba::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_it_had_to_be_snake : public AuraScript
{
    PrepareAuraScript(rune_hunter_it_had_to_be_snake);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        Position dest = victim->GetPosition();

        GetCaster()->CastSpell(dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(), SPELL_HUNTER_SNAKE_TRAP_RANGED, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_it_had_to_be_snake::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_mighty_snake_trap : public AuraScript
{
    PrepareAuraScript(rune_hunter_mighty_snake_trap);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            int32 displayID = unit->GetNativeDisplayId();

            if (displayID != 6303 && displayID != 2958 && displayID != 1206 && displayID != 2957)
                continue;

            GetCaster()->AddAura(procSpell, unit);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_mighty_snake_trap::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_master_handler_aura : public AuraScript
{
    PrepareAuraScript(rune_hunter_master_handler_aura);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->IsAlive() && !pet->HasAura(procSpell))
            GetCaster()->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (!unit->HasAura(procSpell))
                GetCaster()->AddAura(procSpell, unit);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (unit->HasAura(procSpell))
                unit->RemoveAura(procSpell);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_master_handler_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_master_handler_aura::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_master_handler : public AuraScript
{
    PrepareAuraScript(rune_hunter_master_handler);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = eventInfo.GetActor()->GetOwner()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_ASPECT_OF_THE_WILD, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_master_handler::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_ferocious_appetite : public AuraScript
{
    PrepareAuraScript(rune_hunter_ferocious_appetite);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetCaster()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_ASPECT_OF_THE_WILD, -aurEff->GetAmount());
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_ferocious_appetite::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_snake_bite : public AuraScript
{
    PrepareAuraScript(rune_hunter_snake_bite);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500652))
            return GetCaster()->GetAura(500652);

        if (GetCaster()->HasAura(500653))
            return GetCaster()->GetAura(500653);

        if (GetCaster()->HasAura(500654))
            return GetCaster()->GetAura(500654);

        if (GetCaster()->HasAura(500655))
            return GetCaster()->GetAura(500655);

        if (GetCaster()->HasAura(500656))
            return GetCaster()->GetAura(500656);

        if (GetCaster()->HasAura(500657))
            return GetCaster()->GetAura(500657);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(500658))
            GetCaster()->RemoveAura(500658);

        if (GetCaster()->HasAura(500659))
            GetCaster()->RemoveAura(500659);

        if (GetCaster()->HasAura(500660))
            GetCaster()->RemoveAura(500660);

        if (GetCaster()->HasAura(500661))
            GetCaster()->RemoveAura(500661);

        if (GetCaster()->HasAura(500662))
            GetCaster()->RemoveAura(500662);

        if (GetCaster()->HasAura(500663))
            GetCaster()->RemoveAura(500663);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_snake_bite::HandleProc, EFFECT_0, SPELL_AURA_MOD_CRIT_PCT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_snake_bite::HandleRemove, EFFECT_0, SPELL_AURA_MOD_CRIT_PCT, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_war_orders : public AuraScript
{
    PrepareAuraScript(rune_hunter_war_orders);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_KILL_COMMAND, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_war_orders::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_scent_of_blood : public AuraScript
{
    PrepareAuraScript(rune_hunter_scent_of_blood);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500682))
            return GetCaster()->GetAura(500682);

        if (GetCaster()->HasAura(500683))
            return GetCaster()->GetAura(500683);

        if (GetCaster()->HasAura(500684))
            return GetCaster()->GetAura(500684);

        if (GetCaster()->HasAura(500685))
            return GetCaster()->GetAura(500685);

        if (GetCaster()->HasAura(500686))
            return GetCaster()->GetAura(500686);

        if (GetCaster()->HasAura(500687))
            return GetCaster()->GetAura(500687);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_BARBED_SHOT, true);

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (GetCaster()->HasAura(500688))
            GetCaster()->RemoveAura(500688);

        if (GetCaster()->HasAura(500689))
            GetCaster()->RemoveAura(500689);

        if (GetCaster()->HasAura(500690))
            GetCaster()->RemoveAura(500690);

        if (GetCaster()->HasAura(500691))
            GetCaster()->RemoveAura(500691);

        if (GetCaster()->HasAura(500692))
            GetCaster()->RemoveAura(500692);

        if (GetCaster()->HasAura(500693))
            GetCaster()->RemoveAura(500693);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_scent_of_blood::HandleProc, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_scent_of_blood::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_deaths_dance : public AuraScript
{
    PrepareAuraScript(rune_hunter_deaths_dance);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return false;

        int32 criticalChance = std::max<int32>(GetCaster()->GetUnitCriticalChance(BASE_ATTACK, victim), GetCaster()->GetUnitCriticalChance(RANGED_ATTACK, victim));
        uint32 random = urand(1, 100);

        return random <= criticalChance;
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_deaths_dance::CheckProc);
    }
};

class rune_hunter_brutal_companion : public AuraScript
{
    PrepareAuraScript(rune_hunter_brutal_companion);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Pet* pet = GetCaster()->ToPlayer()->GetPet();

        if (!pet || pet->isDead())
            return false;

        if (!pet->HasAura(SPELL_HUNTER_BARBED_SHOT_FRENZY))
            return false;

        if (pet->GetAura(SPELL_HUNTER_BARBED_SHOT_FRENZY)->GetStackAmount() < 3)
            return false;

        if (pet->GetAura(SPELL_HUNTER_BARBED_SHOT_FRENZY)->GetStackAmount() >= 3)
            return true;

        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetActionTarget();

        if (!victim || victim->isDead())
            return;

        if (Player* caster = GetCaster()->ToPlayer())
        {
            int32 coolDown = caster->GetSpellCooldownDelay(SPELL_HUNTER_KILL_COMMAND);
            int32 arcaneShotCooldown = sSpellMgr->GetSpellInfo(SPELL_HUNTER_KILL_COMMAND)->GetRecoveryTime();
            int32 cooldownReduction = arcaneShotCooldown - coolDown;

            caster->CastSpell(victim, SPELL_HUNTER_KILL_COMMAND, TRIGGERED_FULL_MASK);

            caster->ModifySpellCooldown(SPELL_HUNTER_KILL_COMMAND, -cooldownReduction);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_brutal_companion::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_brutal_companion::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_stomp : public AuraScript
{
    PrepareAuraScript(rune_hunter_stomp);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        int32 attackPower = std::max<int32>(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetCaster()->GetTotalAttackPowerValue(RANGED_ATTACK));
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->IsAlive())
            pet->CastCustomSpell(RUNE_HUNTER_STOMP_DAMAGE, SPELLVALUE_BASE_POINT0, damage, pet, TRIGGERED_FULL_MASK);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            unit->CastCustomSpell(RUNE_HUNTER_STOMP_DAMAGE, SPELLVALUE_BASE_POINT0, damage, unit, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_stomp::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_bloodthirsty_wrath : public AuraScript
{
    PrepareAuraScript(rune_hunter_bloodthirsty_wrath);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500738))
            return GetCaster()->GetAura(500738);

        if (GetCaster()->HasAura(500739))
            return GetCaster()->GetAura(500739);

        if (GetCaster()->HasAura(500740))
            return GetCaster()->GetAura(500740);

        if (GetCaster()->HasAura(500741))
            return GetCaster()->GetAura(500741);

        if (GetCaster()->HasAura(500742))
            return GetCaster()->GetAura(500742);

        if (GetCaster()->HasAura(500743))
            return GetCaster()->GetAura(500743);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        if (GetCaster()->HasAura(500744))
            GetCaster()->RemoveAura(500744);

        if (GetCaster()->HasAura(500745))
            GetCaster()->RemoveAura(500745);

        if (GetCaster()->HasAura(500746))
            GetCaster()->RemoveAura(500746);

        if (GetCaster()->HasAura(500747))
            GetCaster()->RemoveAura(500747);

        if (GetCaster()->HasAura(500748))
            GetCaster()->RemoveAura(500748);

        if (GetCaster()->HasAura(500749))
            GetCaster()->RemoveAura(500749);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_bloodthirsty_wrath::HandleProc, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_bloodthirsty_wrath::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_bloodthirsty_wrath_heal : public AuraScript
{
    PrepareAuraScript(rune_hunter_bloodthirsty_wrath_heal);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);
        int32 maxHealth = GetCaster()->GetMaxHealth();
        int32 currentHealth = GetCaster()->GetHealth();

        if (currentHealth + amount <= maxHealth)
            GetCaster()->CastCustomSpell(RUNE_HUNTER_BLOODTHIRSTY_WRATH_HEAL, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        else
        {
            int32 newAmount = 0;

            if (currentHealth != maxHealth)
            {
                newAmount = maxHealth - currentHealth;
                GetCaster()->CastCustomSpell(RUNE_HUNTER_BLOODTHIRSTY_WRATH_HEAL, SPELLVALUE_BASE_POINT0, newAmount, GetCaster(), TRIGGERED_FULL_MASK);
            }

            int32 shieldAmount = amount - newAmount;
            int32 maxShieldThreshold = CalculatePct(maxHealth, GetAura()->GetEffect(EFFECT_1)->GetAmount());

            if (GetCaster()->HasAura(RUNE_HUNTER_BLOODTHIRSTY_WRATH_SHIELD))
                shieldAmount += GetCaster()->GetAura(RUNE_HUNTER_BLOODTHIRSTY_WRATH_SHIELD)->GetEffect(EFFECT_0)->GetAmount();

            int32 shield = std::min<int32>(shieldAmount, maxShieldThreshold);

            GetCaster()->CastCustomSpell(RUNE_HUNTER_BLOODTHIRSTY_WRATH_SHIELD, SPELLVALUE_BASE_POINT0, shield, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_bloodthirsty_wrath_heal::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_bloodthirsty_wrath_heal::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_2wolves_1man : public SpellScript
{
    PrepareSpellScript(rune_hunter_2wolves_1man);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500752))
            return GetCaster()->GetAura(500752);

        if (GetCaster()->HasAura(500753))
            return GetCaster()->GetAura(500753);

        if (GetCaster()->HasAura(500754))
            return GetCaster()->GetAura(500754);

        if (GetCaster()->HasAura(500755))
            return GetCaster()->GetAura(500755);

        if (GetCaster()->HasAura(500756))
            return GetCaster()->GetAura(500756);

        if (GetCaster()->HasAura(500757))
            return GetCaster()->GetAura(500757);

        return nullptr;
    }

    void HandleBuff()
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 procChance = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();
        int32 procSpell = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
        int32 random = urand(1, 100);

        if (random > procChance)
            return;

        Position const& pos = GetCaster()->GetPosition();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        for (size_t i = 0; i < 2; i++)
        {
            Creature* pet = GetCaster()->SummonCreature(29264, pos, TEMPSUMMON_TIMED_DESPAWN, 20000, 0, properties);

            if (pet && pet->IsAlive()) {
                GetCaster()->AddAura(34902, pet);
                GetCaster()->AddAura(34903, pet);
                GetCaster()->AddAura(34904, pet);
                pet->AI()->AttackStart(GetExplTargetUnit());
            }
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(rune_hunter_2wolves_1man::HandleBuff);
    }
};

class rune_hunter_2wolves_1man_summon : public SpellScript
{
    PrepareSpellScript(rune_hunter_2wolves_1man_summon);

    void PetBuffs(Creature* summon)
    {
        Unit* caster = GetCaster();

        summon->AddAura(34902, caster);
        summon->AddAura(34903, caster);
        summon->AddAura(34904, caster);
    }

    void HandlePet()
    {
    }

    void Register() override
    {
        OnCast += SpellCastFn(rune_hunter_2wolves_1man_summon::HandlePet);
    }
};

class rune_hunter_howl : public AuraScript
{
    PrepareAuraScript(rune_hunter_howl);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        Player* player = GetCaster()->ToPlayer();
        Pet* pet = player->GetPet();
        int32 increasedDuration = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        int32 baseDuration = aurEff->GetAmount();
        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (Aura* auraEff = player->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + increasedDuration, auraEff->GetMaxDuration() + 10000));

            auraEff->SetDuration(duration);

            if (pet && pet->IsAlive())
            {
                if (pet->HasAura(SPELL_HUNTER_BESTIAL_WRATH_AURA))
                    pet->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(duration);
                else
                {
                    player->AddAura(SPELL_HUNTER_BESTIAL_WRATH_AURA, pet);
                    pet->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(duration);
                }
            }

            if (summonedUnits.size() == 0)
                return;

            for (auto const& unit : summonedUnits)
            {
                if (unit->isDead())
                    continue;

                if (unit->HasAura(SPELL_HUNTER_BESTIAL_WRATH_AURA))
                    unit->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(duration);
                else
                {
                    player->AddAura(SPELL_HUNTER_BESTIAL_WRATH_AURA, unit);
                    unit->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(duration);
                }
            }
        }
        else
        {
            player->AddAura(SPELL_HUNTER_BESTIAL_WRATH_AURA, player);
            player->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(baseDuration);

            if (pet && pet->IsAlive())
            {
                player->AddAura(SPELL_HUNTER_BESTIAL_WRATH_AURA, pet);
                pet->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(baseDuration);
            }

            if (summonedUnits.size() == 0)
                return;

            for (auto const& unit : summonedUnits)
            {
                if (unit->isDead())
                    continue;

                player->AddAura(SPELL_HUNTER_BESTIAL_WRATH_AURA, unit);
                unit->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(baseDuration);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_howl::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_barbed_wrath : public AuraScript
{
    PrepareAuraScript(rune_hunter_barbed_wrath);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_BESTIAL_WRATH, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_barbed_wrath::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_sustained_anger : public AuraScript
{
    PrepareAuraScript(rune_hunter_sustained_anger);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_BESTIAL_WRATH_AURA);
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        Player* player = GetCaster()->ToPlayer();
        int32 procSpell = aurEff->GetAmount();

        if (!player || player->isDead())
            return;

        player->CastSpell(player, procSpell, TRIGGERED_FULL_MASK);
        Pet* pet = player->GetPet();

        if (pet && pet->IsAlive())
            pet->CastSpell(pet, procSpell, TRIGGERED_FULL_MASK);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            unit->CastSpell(unit, procSpell, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_sustained_anger::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_sustained_anger::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_killer_cobra_apply : public AuraScript
{
    PrepareAuraScript(rune_hunter_killer_cobra_apply);

    Aura* GetRuneAura()
    {

        if (GetCaster()->HasAura(500784))
            return GetCaster()->GetAura(500784);

        if (GetCaster()->HasAura(500785))
            return GetCaster()->GetAura(500785);

        if (GetCaster()->HasAura(500786))
            return GetCaster()->GetAura(500786);

        if (GetCaster()->HasAura(500787))
            return GetCaster()->GetAura(500787);

        if (GetCaster()->HasAura(500788))
            return GetCaster()->GetAura(500788);

        if (GetCaster()->HasAura(500789))
            return GetCaster()->GetAura(500789);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (GetCaster()->HasAura(500790))
            GetCaster()->RemoveAura(500790);

        if (GetCaster()->HasAura(500791))
            GetCaster()->RemoveAura(500791);

        if (GetCaster()->HasAura(500792))
            GetCaster()->RemoveAura(500792);

        if (GetCaster()->HasAura(500793))
            GetCaster()->RemoveAura(500793);

        if (GetCaster()->HasAura(500794))
            GetCaster()->RemoveAura(500794);

        if (GetCaster()->HasAura(500795))
            GetCaster()->RemoveAura(500795);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_killer_cobra_apply::HandleProc, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_killer_cobra_apply::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_killer_cobra : public AuraScript
{
    PrepareAuraScript(rune_hunter_killer_cobra);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_KILL_COMMAND, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_killer_cobra::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_thunderslash_aura : public AuraScript
{
    PrepareAuraScript(rune_hunter_thunderslash_aura);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && !pet->HasAura(procSpell))
            player->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (!unit->HasAura(procSpell))
                player->AddAura(procSpell, unit);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (unit->HasAura(procSpell))
                unit->RemoveAura(procSpell);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_thunderslash_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_thunderslash_aura::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_thunderslash : public AuraScript
{
    PrepareAuraScript(rune_hunter_thunderslash);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        if (!eventInfo.GetActor()->HasAura(SPELL_HUNTER_BESTIAL_WRATH_AURA))
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* actor = eventInfo.GetActor();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        actor->CastCustomSpell(RUNE_HUNTER_THUNDERSLASH_DAMAGE, SPELLVALUE_BASE_POINT0, amount, actor, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_thunderslash::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_thunderslash::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_wild_instincts : public AuraScript
{
    PrepareAuraScript(rune_hunter_wild_instincts);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        if (!GetCaster()->HasAura(SPELL_HUNTER_CALL_OF_THE_WILD))
            return false;

        if (eventInfo.GetSpellInfo()->ManaCost <= 0)
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_BARBED_SHOT, true);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_wild_instincts::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_wild_instincts::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_enduring_call_aura : public AuraScript
{
    PrepareAuraScript(rune_hunter_enduring_call_aura);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->IsAlive() && !pet->HasAura(procSpell))
            player->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (!unit->HasAura(procSpell))
                player->AddAura(procSpell, unit);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (unit->HasAura(procSpell))
                unit->RemoveAura(procSpell);
        }
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Pet* pet = player->GetPet();
        int32 durationIncrease = aurEff->GetAmount();

        if (Aura* auraEff = player->GetAura(SPELL_HUNTER_CALL_OF_THE_WILD))
            auraEff->SetDuration(auraEff->GetDuration() + durationIncrease);

        if (!pet || pet->isDead())
            return;

        if (Aura* auraEff = pet->GetAura(SPELL_HUNTER_RABID_AURA))
            auraEff->SetDuration(auraEff->GetDuration() + durationIncrease);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_enduring_call_aura::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_enduring_call_aura::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectProc += AuraEffectProcFn(rune_hunter_enduring_call_aura::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_enduring_call : public AuraScript
{
    PrepareAuraScript(rune_hunter_enduring_call);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* actor = eventInfo.GetActor();
        Player* player = actor->GetOwner()->ToPlayer();
        int32 durationIncrease = aurEff->GetAmount();

        if (Aura* auraEff = player->GetAura(SPELL_HUNTER_CALL_OF_THE_WILD))
            auraEff->SetDuration(auraEff->GetDuration() + durationIncrease);

        if (Aura* auraEff = actor->GetAura(SPELL_HUNTER_RABID_AURA))
            auraEff->SetDuration(auraEff->GetDuration() + durationIncrease);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_enduring_call::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_double_trouble_aura : public AuraScript
{
    PrepareAuraScript(rune_hunter_double_trouble_aura);

    void SecondaryPetsDoubleTrouble(uint32 procSpell, bool remove) {

        auto summonedUnits = GetCaster()->ToPlayer()->GetSummonedUnits();

        if (summonedUnits.size() == 0)
            return;

        for (const auto& unit : summonedUnits) {
            if (!remove && !unit->HasAura(procSpell) && unit->HasAura(80224))
                unit->AddAura(procSpell, unit);
            if (remove && unit->HasAura(procSpell) && unit->HasAura(80224))
                unit->RemoveAura(procSpell);
        }
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (!pet || pet->isDead())
            return;

        if (!pet->HasAura(procSpell))
            player->AddAura(procSpell, pet);

        SecondaryPetsDoubleTrouble(procSpell, false);
        ;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (!pet || pet->isDead())
            return;

        if (pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        SecondaryPetsDoubleTrouble(procSpell, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_double_trouble_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_double_trouble_aura::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_double_trouble : public AuraScript
{
    PrepareAuraScript(rune_hunter_double_trouble);

    void PetBuffs(Creature* summon)
    {
        Unit* caster = GetCaster();

        summon->AddAura(34902, caster);
        summon->AddAura(34903, caster);
        summon->AddAura(34904, caster);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster()->ToPlayer();
        Position const& pos = GetCaster()->GetPosition();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        int32 duration = aurEff->GetAmount();
        Unit* pet = eventInfo.GetActor();
        int32 petId = pet->ToCreature()->GetCreatureTemplate()->Entry;

        Creature* guardian = GetCaster()->SummonCreature(petId, pos, TEMPSUMMON_TIMED_DESPAWN, duration, 0, properties);
        CreatureTemplate const* petCinfo = sObjectMgr->GetCreatureTemplate(petId);
        CreatureFamilyEntry const* petFamily = sCreatureFamilyStore.LookupEntry(petCinfo->family);

        PetBuffs(guardian);

        if (guardian && guardian->IsAlive())
            guardian->AI()->AttackStart(eventInfo.GetActionTarget());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_double_trouble::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_poison_fangs_aura : public AuraScript
{
    PrepareAuraScript(rune_hunter_poison_fangs_aura);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && !pet->HasAura(procSpell))
            player->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (!unit->HasAura(procSpell))
                player->AddAura(procSpell, unit);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->IsAlive() && pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (unit->HasAura(procSpell))
                unit->RemoveAura(procSpell);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_poison_fangs_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_poison_fangs_aura::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_poison_fangs : public AuraScript
{
    PrepareAuraScript(rune_hunter_poison_fangs);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
        Unit* pet = eventInfo.GetActor();
        int32 attackPower = std::max<int32>(player->GetTotalAttackPowerValue(BASE_ATTACK), player->GetTotalAttackPowerValue(RANGED_ATTACK));
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        if (!victim || !player || !pet)
            return;

        if (Aura* dot = victim->GetAura(RUNE_HUNTER_POISON_FANGS_DOT))
        {
            if (dot->GetStackAmount() >= 5)
                player->CastCustomSpell(RUNE_HUNTER_POISON_FANGS_DAMAGE, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);
        }
        player->CastCustomSpell(RUNE_HUNTER_POISON_FANGS_DOT, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_poison_fangs::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_poison_fangs::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_true_aimed_shot : public AuraScript
{
    PrepareAuraScript(rune_hunter_true_aimed_shot);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Aura* movementCheck = GetCaster()->GetAura(RUNE_HUNTER_TRUE_AIMED_SHOT_CHECK);

        if (!movementCheck)
            return;

        if (movementCheck->GetStackAmount() < aurEff->GetAmount())
            return;

        movementCheck->SetStackAmount(aurEff->GetAmount());

        if (GetCaster()->HasAura(RUNE_HUNTER_TRUE_AIMED_SHOT_BUFF))
            return;

        GetCaster()->CastSpell(GetCaster(), RUNE_HUNTER_TRUE_AIMED_SHOT_BUFF, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_true_aimed_shot::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class rune_hunter_set_aim_shoot : public AuraScript
{
    PrepareAuraScript(rune_hunter_set_aim_shoot);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_AIMED_SHOT, -aurEff->GetAmount());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_set_aim_shoot::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_set_aim_shoot::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_double_impact : public AuraScript
{
    PrepareAuraScript(rune_hunter_double_impact);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_HUNTER_DOUBLE_IMPACT_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
        GetCaster()->RemoveAura(GetAura());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_double_impact::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_double_impact::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_hun_wind_arrow : public SpellScript
{
    PrepareSpellScript(spell_hun_wind_arrow);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500930))
            return GetCaster()->GetAura(500930);

        if (GetCaster()->HasAura(500931))
            return GetCaster()->GetAura(500931);

        if (GetCaster()->HasAura(500932))
            return GetCaster()->GetAura(500932);

        if (GetCaster()->HasAura(500933))
            return GetCaster()->GetAura(500933);

        if (GetCaster()->HasAura(500934))
            return GetCaster()->GetAura(500934);

        if (GetCaster()->HasAura(500935))
            return GetCaster()->GetAura(500935);

        return nullptr;
    }

    void HandleDamage(SpellEffIndex effIndex)
    {
        Player* caster = GetCaster()->ToPlayer();
        Unit* target = GetExplTargetUnit();
        float ap = GetCaster()->GetTotalAttackPowerValue(RANGED_ATTACK);
        int32 ratio = GetRuneAura() ? GetRuneAura()->GetEffect(EFFECT_0)->GetAmount() : sSpellMgr->AssertSpellInfo(500930)->GetEffect(EFFECT_0).CalcValue();
        int32 damage = CalculatePct(ap, ratio);

        if (target)
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, EFFECT_0);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_hun_wind_arrow::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class rune_hunter_legacy_of_the_windrunner : public AuraScript
{
    PrepareAuraScript(rune_hunter_legacy_of_the_windrunner);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        if (!eventInfo.GetSpellInfo())
            return false;

        if (eventInfo.GetSpellInfo()->Id != SPELL_HUNTER_AIMED_SHOT)
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        int32 duration = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        GetCaster()->CastSpell(victim, SPELL_HUNTER_WIND_ARROW, TRIGGERED_FULL_MASK);
        victim->GetAura(SPELL_HUNTER_WIND_ARROW)->SetDuration(duration);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_legacy_of_the_windrunner::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_legacy_of_the_windrunner::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_windrunners_guidance : public AuraScript
{
    PrepareAuraScript(rune_hunter_windrunners_guidance);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetSpellInfo())
            return false;

        if (eventInfo.GetSpellInfo()->Id != SPELL_HUNTER_WIND_ARROW_DAMAGE)
            return false;

        return true;
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        Player* player = GetCaster()->ToPlayer();
        int32 increasedDuration = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        int32 baseDuration = aurEff->GetAmount();

        if (Aura* auraEff = player->GetAura(SPELL_HUNTER_TRUESHOT))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + increasedDuration, auraEff->GetMaxDuration() + 10000));

            auraEff->SetDuration(duration);
        }
        else
        {
            player->AddAura(SPELL_HUNTER_TRUESHOT, player);
            player->GetAura(SPELL_HUNTER_TRUESHOT)->SetDuration(baseDuration);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_windrunners_guidance::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_windrunners_guidance::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_precise_shots : public AuraScript
{
    PrepareAuraScript(rune_hunter_precise_shots);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetSpellInfo())
            return false;

        if (eventInfo.GetSpellInfo()->Id != SPELL_HUNTER_AIMED_SHOT)
            return false;

        return true;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_precise_shots::CheckProc);
    }
};

class rune_hunter_lock_and_loaded : public AuraScript
{
    PrepareAuraScript(rune_hunter_lock_and_loaded);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_AIMED_SHOT, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_lock_and_loaded::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_hunter_lethal_ammunition : public SpellScript
{
    PrepareSpellScript(rune_hunter_lethal_ammunition);

    void HandleProc(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();

        if (caster->HasAura(RUNE_HUNTER_LETHAL_AMMUNITION_BUFF))
            caster->RemoveAura(RUNE_HUNTER_LETHAL_AMMUNITION_BUFF);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(rune_hunter_lethal_ammunition::HandleProc, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class rune_hunter_quick_load : public AuraScript
{
    PrepareAuraScript(rune_hunter_quick_load);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();

        if (player->HasAura(RUNE_HUNTER_QUICK_LOAD_LISTENER))
            return;

        if (player->GetHealthPct() < aurEff->GetAmount())
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();

        if (damage <= 0)
            return;

        int32 maxHealth = player->GetMaxHealth();
        int32 currentHealth = player->GetHealth() - damage;

        if (currentHealth <= 0)
            return;

        float healthPct = 100.f * currentHealth / maxHealth;

        if (healthPct > aurEff->GetAmount())
            return;

        int32 listenerDuration = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        player->RemoveSpellCooldown(SPELL_HUNTER_BURSTING_SHOT, true);
        player->AddAura(RUNE_HUNTER_QUICK_LOAD_LISTENER, player);
        player->GetAura(RUNE_HUNTER_QUICK_LOAD_LISTENER)->SetDuration(listenerDuration);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_quick_load::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_quick_load::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_steadfast_focus : public AuraScript
{
    PrepareAuraScript(rune_hunter_steadfast_focus);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* player = GetCaster();
        int32 procSpell = aurEff->GetAmount();

        if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_STEADY_SHOT)
        {
            if (player->HasAura(RUNE_HUNTER_STEADFAST_FOCUS_LISTENER))
            {
                player->CastSpell(player, procSpell, TRIGGERED_FULL_MASK);
                player->RemoveAura(RUNE_HUNTER_STEADFAST_FOCUS_LISTENER);
            }
            else
                player->AddAura(RUNE_HUNTER_STEADFAST_FOCUS_LISTENER, player);
        }
        else
            if (player->HasAura(RUNE_HUNTER_STEADFAST_FOCUS_LISTENER))
                player->RemoveAura(RUNE_HUNTER_STEADFAST_FOCUS_LISTENER);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_steadfast_focus::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_steadfast_focus::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_on_the_trail : public AuraScript
{
    PrepareAuraScript(rune_hunter_on_the_trail);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
        int32 procSpell = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
        int32 attackPower = player->GetTotalAttackPowerValue(RANGED_ATTACK);
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        if (!victim || !player)
            return;

        player->CastCustomSpell(RUNE_HUNTER_ON_THE_TRAIL_DOT, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);

        if (!player->HasAura(procSpell))
            player->AddAura(procSpell, player);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_on_the_trail::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_on_the_trail::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_on_the_trail_duration : public AuraScript
{
    PrepareAuraScript(rune_hunter_on_the_trail_duration);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetVictim();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        if (Aura* dot = victim->GetAura(RUNE_HUNTER_ON_THE_TRAIL_DOT))
        {
            int32 durationIncrease = aurEff->GetAmount();
            int32 increasedAmount = dot->GetEffect(EFFECT_0)->GetAmount();
            int32 maxIncrease = dot->GetSpellInfo()->GetMaxDuration();

            if (increasedAmount >= maxIncrease)
                return;

            int32 duration = dot->GetDuration() + durationIncrease;

            dot->SetDuration(duration);
            dot->GetEffect(EFFECT_0)->ResetTicks();
            increasedAmount += durationIncrease;
            dot->GetEffect(EFFECT_0)->SetAmount(increasedAmount);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_on_the_trail_duration::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_on_the_trail_duration::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_echoes_of_ohnara : public AuraScript
{
    PrepareAuraScript(rune_hunter_echoes_of_ohnara);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
        int32 attackPower = player->GetTotalAttackPowerValue(RANGED_ATTACK);
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        if (!victim || !player)
            return;

        player->CastCustomSpell(RUNE_HUNTER_ECHOES_OF_ONHARA_DAMAGE, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_echoes_of_ohnara::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_echoes_of_ohnara::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_surging_shots : public AuraScript
{
    PrepareAuraScript(rune_hunter_surging_shots);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_RAPID_FIRE, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_surging_shots::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_brutal_projectiles : public AuraScript
{
    PrepareAuraScript(rune_hunter_brutal_projectiles);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* player = GetCaster();

        if (player->HasAura(501132))
            player->RemoveAura(501132);

        if (player->HasAura(501133))
            player->RemoveAura(501133);

        if (player->HasAura(501134))
            player->RemoveAura(501134);

        if (player->HasAura(501135))
            player->RemoveAura(501135);

        if (player->HasAura(501136))
            player->RemoveAura(501136);

        if (player->HasAura(501137))
            player->RemoveAura(501137);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_brutal_projectiles::HandleRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_tommy_gun : public AuraScript
{
    PrepareAuraScript(rune_hunter_tommy_gun);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Unit* player = GetCaster();

        if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_RAPID_FIRE)
        {
            if (player->HasAura(procSpell))
                player->RemoveAura(procSpell);
        }
        if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_RAPID_FIRE_DAMAGE)
            player->CastSpell(player, procSpell, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_tommy_gun::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_tommy_gun::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_lethal_shots : public AuraScript
{
    PrepareAuraScript(rune_hunter_lethal_shots);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_RAPID_FIRE, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_lethal_shots::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_steady_focus : public AuraScript
{
    PrepareAuraScript(rune_hunter_steady_focus);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* player = GetCaster();
        int32 procSpell = aurEff->GetAmount();

        if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_STEADY_SHOT)
        {
            if (player->HasAura(RUNE_HUNTER_STEADY_FOCUS_LISTENER))
            {
                player->CastSpell(player, procSpell, TRIGGERED_FULL_MASK);
                player->RemoveAura(RUNE_HUNTER_STEADY_FOCUS_LISTENER);
            }
            else
                player->AddAura(RUNE_HUNTER_STEADY_FOCUS_LISTENER, player);
        }
        else
            if (player->HasAura(RUNE_HUNTER_STEADY_FOCUS_LISTENER))
                player->RemoveAura(RUNE_HUNTER_STEADY_FOCUS_LISTENER);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_steady_focus::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_steady_focus::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_focalised_trueshot : public AuraScript
{
    PrepareAuraScript(rune_hunter_focalised_trueshot);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(SPELL_HUNTER_TRUESHOT))
            return;

        int32 spellFocus = eventInfo.GetSpellInfo()->ManaCost;

        if (spellFocus <= 0)
            return;

        int32 focusAccumulated = GetAura()->GetEffect(EFFECT_1)->GetAmount() + spellFocus;
        int32 focusThreshold = aurEff->GetAmount();


        for (focusAccumulated; focusAccumulated > focusThreshold; focusAccumulated -= focusThreshold)
        {
            GetCaster()->CastSpell(GetCaster(), RUNE_HUNTER_FOCALISED_TRUESHOT_LISTENER, TRIGGERED_FULL_MASK);
        }

        GetAura()->GetEffect(EFFECT_1)->SetAmount(focusAccumulated);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_focalised_trueshot::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_focalised_trueshot::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_focalised_trueshot_remove : public AuraScript
{
    PrepareAuraScript(rune_hunter_focalised_trueshot_remove);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* player = GetCaster();
        Aura* listenerAura = player->GetAura(RUNE_HUNTER_FOCALISED_TRUESHOT_LISTENER);

        if (!listenerAura)
            return;

        int32 stackAmount = listenerAura->GetStackAmount();

        player->RemoveAura(listenerAura);
        player->CastSpell(player, RUNE_HUNTER_FOCALISED_TRUESHOT_BUFF, TRIGGERED_FULL_MASK);
        player->GetAura(RUNE_HUNTER_FOCALISED_TRUESHOT_BUFF)->SetStackAmount(stackAmount);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_focalised_trueshot_remove::HandleProc, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_unerring_vision : public AuraScript
{
    PrepareAuraScript(rune_hunter_unerring_vision);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_HUNTER_TRUESHOT))
            return;

        Aura* buffAura = caster->GetAura(RUNE_HUNTER_UNERRING_VISION_BUFF);

        if (!buffAura)
        {
            caster->CastSpell(caster, RUNE_HUNTER_UNERRING_VISION_BUFF, TRIGGERED_FULL_MASK);
            return;
        }

        int32 currentStacks = buffAura->GetStackAmount();

        if (currentStacks >= aurEff->GetAmount())
        {
            buffAura->RefreshDuration();
            return;
        }

        caster->CastSpell(caster, RUNE_HUNTER_UNERRING_VISION_BUFF, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_unerring_vision::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_hunter_calling_the_shots : public AuraScript
{
    PrepareAuraScript(rune_hunter_calling_the_shots);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Aura* runeAura = GetAura();
        Player* target = GetTarget()->ToPlayer();

        if (!runeAura || !target)
            return;

        int32 spellFocus = eventInfo.GetSpellInfo()->ManaCost;

        if (spellFocus <= 0)
            return;

        int32 cooldownReduction = runeAura->GetEffect(EFFECT_1)->GetAmount();
        int32 focusAccumulated = runeAura->GetEffect(EFFECT_2)->GetAmount() + spellFocus;
        int32 focusThreshold = aurEff->GetAmount();

        for (focusAccumulated; focusAccumulated > focusThreshold; focusAccumulated -= focusThreshold)
        {
            target->ModifySpellCooldown(SPELL_HUNTER_TRUESHOT, -cooldownReduction);
        }

        runeAura->GetEffect(EFFECT_2)->SetAmount(focusAccumulated);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_calling_the_shots::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_calling_the_shots::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_windrunners_barrage : public AuraScript
{
    PrepareAuraScript(rune_hunter_windrunners_barrage);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        if (!eventInfo.GetSpellInfo())
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        int32 duration = 0;

        if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_WAILING_ARROW_ST)
            duration += GetAura()->GetEffect(EFFECT_0)->GetAmount();

        if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_WAILING_ARROW_AOE)
            duration += GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (duration == 0)
            return;

        if (victim->HasAura(SPELL_HUNTER_WIND_ARROW))
            duration += victim->GetAura(SPELL_HUNTER_WIND_ARROW)->GetDuration();

        GetCaster()->CastSpell(victim, SPELL_HUNTER_WIND_ARROW, TRIGGERED_FULL_MASK);
        victim->GetAura(SPELL_HUNTER_WIND_ARROW)->SetDuration(duration);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_windrunners_barrage::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_windrunners_barrage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_reload : public AuraScript
{
    PrepareAuraScript(rune_hunter_reload);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_BLACK_ARROW, -aurEff->GetAmount());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_reload::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_reload::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_frequent_butcher : public AuraScript
{
    PrepareAuraScript(rune_hunter_frequent_butcher);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_BUTCHERY, -aurEff->GetAmount());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_frequent_butcher::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_frequent_butcher::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_coordinated_kill : public AuraScript
{
    PrepareAuraScript(rune_hunter_coordinated_kill);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(501338))
            return GetCaster()->GetAura(501338);

        if (GetCaster()->HasAura(501339))
            return GetCaster()->GetAura(501339);

        if (GetCaster()->HasAura(501340))
            return GetCaster()->GetAura(501340);

        if (GetCaster()->HasAura(501341))
            return GetCaster()->GetAura(501341);

        if (GetCaster()->HasAura(501342))
            return GetCaster()->GetAura(501342);

        if (GetCaster()->HasAura(501343))
            return GetCaster()->GetAura(501343);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(501344))
            GetCaster()->RemoveAura(501344);

        if (GetCaster()->HasAura(501345))
            GetCaster()->RemoveAura(501345);

        if (GetCaster()->HasAura(501346))
            GetCaster()->RemoveAura(501346);

        if (GetCaster()->HasAura(501347))
            GetCaster()->RemoveAura(501347);

        if (GetCaster()->HasAura(501348))
            GetCaster()->RemoveAura(501348);

        if (GetCaster()->HasAura(501349))
            GetCaster()->RemoveAura(501349);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_coordinated_kill::HandleProc, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_coordinated_kill::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_birds_of_prey : public AuraScript
{
    PrepareAuraScript(rune_hunter_birds_of_prey);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(501356))
            return GetCaster()->GetAura(501356);

        if (GetCaster()->HasAura(501357))
            return GetCaster()->GetAura(501357);

        if (GetCaster()->HasAura(501358))
            return GetCaster()->GetAura(501358);

        if (GetCaster()->HasAura(501359))
            return GetCaster()->GetAura(501359);

        if (GetCaster()->HasAura(501360))
            return GetCaster()->GetAura(501360);

        if (GetCaster()->HasAura(501361))
            return GetCaster()->GetAura(501361);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        for (size_t i = 501362; i < 501368; i++)
        {
            if (GetCaster()->HasAura(i))
                GetCaster()->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_birds_of_prey::HandleProc, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_birds_of_prey::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_bombardier : public AuraScript
{
    PrepareAuraScript(rune_hunter_bombardier);

    Aura* GetRuneAura()
    {
        for (size_t i = 501374; i < 501380; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_WILDFIRE_BOMB, true);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_WILDFIRE_BOMB, true);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_bombardier::HandleProc, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_bombardier::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_poisonous_claws : public AuraScript
{
    PrepareAuraScript(rune_hunter_poisonous_claws);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_FURY_OF_THE_EAGLE, -aurEff->GetAmount());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_poisonous_claws::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_poisonous_claws::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_ruthless_marauder_crit : public SpellScript
{
    PrepareSpellScript(rune_hunter_ruthless_marauder_crit);

    Aura* GetRuneAura()
    {
        for (size_t i = 501404; i < 501410; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleAura(SpellMissInfo missInfo)
    {
        if (!GetRuneAura() || GetCaster()->isDead())
            return;

        Unit* target = GetHitUnit();

        if (!target || target->isDead())
            return;

        int32 healthThreshold = GetRuneAura()->GetEffect(EFFECT_2)->GetAmount();

        if (target->GetHealthPct() > healthThreshold)
            return;

        int32 criticalChance = GetCaster()->GetUnitCriticalChance(BASE_ATTACK, target);
        int32 critPct = GetRuneAura()->GetEffect(EFFECT_1)->GetAmount();
        int32 critIncrease = CalculatePct(criticalChance, critPct);

        GetCaster()->CastCustomSpell(RUNE_HUNTER_RUTHLESS_MARAUDER_BUFF, SPELLVALUE_BASE_POINT0, critIncrease, GetCaster(), TRIGGERED_FULL_MASK);
    }
    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(rune_hunter_ruthless_marauder_crit::HandleAura);
    }
};

class rune_hunter_ruthless_marauder : public AuraScript
{
    PrepareAuraScript(rune_hunter_ruthless_marauder);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
        {
            target->ModifySpellCooldown(SPELL_HUNTER_WILDFIRE_BOMB, -aurEff->GetAmount());
            target->ModifySpellCooldown(SPELL_HUNTER_FLANKING_STRIKE, -aurEff->GetAmount());
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_ruthless_marauder::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_ruthless_marauder::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_deadly_duo_check : public AuraScript
{
    PrepareAuraScript(rune_hunter_deadly_duo_check);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_SPEARHEAD_AURA);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_deadly_duo_check::CheckProc);
    }
};

class rune_hunter_forest_fire : public AuraScript
{
    PrepareAuraScript(rune_hunter_forest_fire);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_WILDFIRE_BOMB, -aurEff->GetAmount());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_forest_fire::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_forest_fire::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_frenzy_strikes : public AuraScript
{
    PrepareAuraScript(rune_hunter_frenzy_strikes);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        Player* target = GetTarget()->ToPlayer();

        int32 bombReduction = aurEff->GetAmount();
        int32 flankingReduction = GetAura()->GetEffect(EFFECT_1)->GetAmount();


        target->ModifySpellCooldown(SPELL_HUNTER_WILDFIRE_BOMB, -bombReduction);
        target->ModifySpellCooldown(SPELL_HUNTER_FLANKING_STRIKE, -flankingReduction);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_frenzy_strikes::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_frenzy_strikes::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_volatile_bomb : public AuraScript
{
    PrepareAuraScript(rune_hunter_volatile_bomb);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || !player)
            return;

        if (Aura* serpentAura = victim->GetAura(SPELL_HUNTER_SERPENT_STING))
        {
            int32 attackPower = player->GetTotalAttackPowerValue(BASE_ATTACK);
            float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

            player->CastCustomSpell(RUNE_HUNTER_VOLATILE_BOMB_DAMAGE, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);

            serpentAura->RefreshDuration();
            serpentAura->GetEffect(EFFECT_0)->ResetTicks();
        }
        else
            player->CastSpell(victim, SPELL_HUNTER_SERPENT_STING, TRIGGERED_FULL_MASK);

    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_volatile_bomb::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_volatile_bomb::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_pheromone_bomb : public AuraScript
{
    PrepareAuraScript(rune_hunter_pheromone_bomb);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_KILL_COMMAND, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_pheromone_bomb::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_shrapnel_bomb : public AuraScript
{
    PrepareAuraScript(rune_hunter_shrapnel_bomb);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || !player)
            return;

        if (!victim->HasAura(SPELL_HUNTER_WILDFIRE_BOMB))
            return;

        int32 attackPower = player->GetTotalAttackPowerValue(BASE_ATTACK);
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        player->CastCustomSpell(RUNE_HUNTER_SHRAPNEL_BOMB_DOT, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_shrapnel_bomb::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_shrapnel_bomb::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
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
    RegisterSpellScript(rune_hunter_playing_with_matches);
    RegisterSpellScript(rune_hunter_playing_with_matches_proc);
    RegisterSpellScript(rune_hunter_killer_instinct);
    RegisterSpellScript(rune_hunter_cleave_command_proc);
    RegisterSpellScript(rune_hunter_cleave_command);
    RegisterSpellScript(rune_hunter_strength_of_the_pack);
    RegisterSpellScript(rune_hunter_quick_shot);
    RegisterSpellScript(rune_hunter_cobra_senses);
    RegisterSpellScript(rune_hunter_deathblow);
    RegisterSpellScript(rune_hunter_razor_fragments_trick_shots);
    RegisterSpellScript(rune_hunter_razor_fragments_weak_spot);
    RegisterSpellScript(rune_hunter_razor_fragments);
    RegisterSpellScript(rune_hunter_good_health);
    RegisterSpellScript(rune_hunter_beast_cleave_proc);
    RegisterSpellScript(rune_hunter_beast_cleave);
    RegisterSpellScript(rune_hunter_trick_shots_listener);
    RegisterSpellScript(rune_hunter_trick_shots);
    RegisterSpellScript(rune_hunter_aspect_of_the_storm);
    RegisterSpellScript(rune_hunter_black_mamba_proc);
    RegisterSpellScript(rune_hunter_black_mamba);
    RegisterSpellScript(rune_hunter_it_had_to_be_snake);
    RegisterSpellScript(rune_hunter_mighty_snake_trap);
    RegisterSpellScript(rune_hunter_master_handler_aura);
    RegisterSpellScript(rune_hunter_master_handler);
    RegisterSpellScript(rune_hunter_ferocious_appetite);
    RegisterSpellScript(rune_hunter_snake_bite);
    RegisterSpellScript(rune_hunter_war_orders);
    RegisterSpellScript(rune_hunter_scent_of_blood);
    RegisterSpellScript(rune_hunter_deaths_dance);
    RegisterSpellScript(rune_hunter_brutal_companion);
    RegisterSpellScript(rune_hunter_stomp);
    RegisterSpellScript(rune_hunter_bloodthirsty_wrath);
    RegisterSpellScript(rune_hunter_bloodthirsty_wrath_heal);
    RegisterSpellScript(rune_hunter_2wolves_1man);
    RegisterSpellScript(rune_hunter_2wolves_1man_summon);
    RegisterSpellScript(rune_hunter_howl);
    RegisterSpellScript(rune_hunter_barbed_wrath);
    RegisterSpellScript(rune_hunter_sustained_anger);
    RegisterSpellScript(rune_hunter_killer_cobra_apply);
    RegisterSpellScript(rune_hunter_killer_cobra);
    RegisterSpellScript(rune_hunter_thunderslash_aura);
    RegisterSpellScript(rune_hunter_thunderslash);
    RegisterSpellScript(rune_hunter_wild_instincts);
    RegisterSpellScript(rune_hunter_enduring_call_aura);
    RegisterSpellScript(rune_hunter_enduring_call);
    RegisterSpellScript(rune_hunter_double_trouble_aura);
    RegisterSpellScript(rune_hunter_double_trouble);
    RegisterSpellScript(rune_hunter_poison_fangs_aura);
    RegisterSpellScript(rune_hunter_poison_fangs);
    RegisterSpellScript(rune_hunter_true_aimed_shot);
    RegisterSpellScript(rune_hunter_set_aim_shoot);
    RegisterSpellScript(rune_hunter_double_impact);
    RegisterSpellScript(spell_hun_wind_arrow);
    RegisterSpellScript(rune_hunter_legacy_of_the_windrunner);
    RegisterSpellScript(rune_hunter_windrunners_guidance);
    RegisterSpellScript(rune_hunter_precise_shots);
    RegisterSpellScript(rune_hunter_lock_and_loaded);
    RegisterSpellScript(rune_hunter_lethal_ammunition);
    RegisterSpellScript(rune_hunter_quick_load);
    RegisterSpellScript(rune_hunter_steadfast_focus);
    RegisterSpellScript(rune_hunter_on_the_trail);
    RegisterSpellScript(rune_hunter_on_the_trail_duration);
    RegisterSpellScript(rune_hunter_echoes_of_ohnara);
    RegisterSpellScript(rune_hunter_surging_shots);
    RegisterSpellScript(rune_hunter_brutal_projectiles);
    RegisterSpellScript(rune_hunter_tommy_gun);
    RegisterSpellScript(rune_hunter_lethal_shots);
    RegisterSpellScript(rune_hunter_steady_focus);
    RegisterSpellScript(rune_hunter_focalised_trueshot);
    RegisterSpellScript(rune_hunter_focalised_trueshot_remove);
    RegisterSpellScript(rune_hunter_unerring_vision);
    RegisterSpellScript(rune_hunter_calling_the_shots);
    RegisterSpellScript(rune_hunter_windrunners_barrage);
    RegisterSpellScript(rune_hunter_reload);
    RegisterSpellScript(rune_hunter_frequent_butcher);
    RegisterSpellScript(rune_hunter_coordinated_kill);
    RegisterSpellScript(rune_hunter_birds_of_prey);
    RegisterSpellScript(rune_hunter_bombardier);
    RegisterSpellScript(rune_hunter_poisonous_claws);
    RegisterSpellScript(rune_hunter_ruthless_marauder_crit);
    RegisterSpellScript(rune_hunter_ruthless_marauder);
    RegisterSpellScript(rune_hunter_deadly_duo_check);
    RegisterSpellScript(rune_hunter_forest_fire);
    RegisterSpellScript(rune_hunter_frenzy_strikes);
    RegisterSpellScript(rune_hunter_volatile_bomb);
    RegisterSpellScript(rune_hunter_pheromone_bomb);
    RegisterSpellScript(rune_hunter_shrapnel_bomb);
}
