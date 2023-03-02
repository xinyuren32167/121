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

    SPELL_HUNTER_KILL_SHOT = 61006,
    SPELL_HUNTER_SERPENT_STING = 49001,

    RUNE_HUNTER_BULLSEYE_BUFF = 500044,

    RUNE_HUNTER_50CAL_DAMAGE = 500052,

    RUNE_HUNTER_SNIPER_TRAINING_CHECK = 500066,

    RUNE_HUNTER_SERPENT_TOUCH_DAMAGE = 500074,

    RUNE_HUNTER_POISON_INJECTION_DAMAGE = 500094,
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
}
