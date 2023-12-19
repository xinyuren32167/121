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


enum SpellsWarrior
{
    SPELL_WARR_THUNDERCLAP = 47502,
    SPELL_WARR_THUNDERCLAP_SON_OF_THUNDER = 84663,
    SPELL_WARR_THUNDER_BOLT_THUNDER_CLAP = 201157,
    SPELL_WARR_IGNORE_PAIN = 80004,
    SPELL_WARR_SHIELD_SLAM = 47488,
    SPELL_WARR_REND = 47465,
    SPELL_WARR_RAGING_BLOW = 80008,
    SPELL_WARR_SHIELD_BLOCK = 2565,
    SPELL_WARR_SHIELD_WALL = 871,
    SPELL_WARR_COLOSSAL_THRUST = 84557,
    SPELL_WARR_CRUSHING_STRIKE = 84550,
    SPELL_WARR_SECOND_CRUSHING_STRIKE = 84664,
    SPELL_WARR_HULN_FURY = 84653,
    SPELL_WARR_BATTLE_TRANCE = 84560,
    SPELL_WARR_TEST_OF_MIGHT = 200887,
    MASTERY_WARR_DEEP_WOUNDS = 200000,
    MASTERY_WARR_DEEP_WOUNDS_DAMAGE = 200001,
    MASTERY_WARR_DEEP_WOUNDS_DAMAGE_INCREASE = 200002,
    SPELL_WARR_EXECUTE = 47471,
    SPELL_WARR_CLEAVE = 47520,
    SPELL_WARR_MORTAL_STRIKE = 47486,
    SPELL_WARR_RECKLESSNESS = 1719,
    SPELL_WARR_RAVAGER = 84540,
    SPELL_WARR_ANNIHILATOR = 84543,
    SPELL_WARR_SHIELD_CHARGE = 84516,
    SPELL_WARR_REVENGE_BUFF = 57830,
    SPELL_WARR_SPEAR_SWIPE = 84561,

    RUNE_WARR_DEVASTATOR_PROC = 200868,
    RUNE_WARR_FATALITY_MARK = 201207,
    RUNE_WARR_FATALITY_DAMAGE = 201208,
    RUNE_WARR_ENRAGE_ADDITIONAL_BUFF = 201259,
    RUNE_WARR_TEST_OF_MIGHT_COUNTER = 201137,
    RUNE_WARR_TEST_OF_MIGHT_BUFF = 200887,
    RUNE_WARR_STRENGTH_OF_ARMS_PROC = 201215,
    RUNE_WARR_FERVOR_OF_BATTLE_SLAM = 201228,
    RUNE_WARR_BERSERKERS_TORMENT_PROC = 200407,
    RUNE_WARR_RELENTLESS_STACK = 200434,
    RUNE_WARR_RELENTLESS_BUFF = 200435,
    RUNE_WARR_STEEL_RESONANCE_REVENGE = 201284,
    RUNE_WARR_STEEL_RESONANCE_WHIRLWIND = 201285,
    RUNE_WARR_BLOCK_SPIKE_PROC = 200733,
    RUNE_WARR_STEEL_IMPACT_PROC = 201005,
    RUNE_WARR_GLADIATORS_AEGIS_PROC = 201018,
    RUNE_WARR_IMPENETRABLE_BARRIER_PROC = 201031,
    RUNE_WARR_GLADIATORS_TORMENT_PROC = 201051,
    RUNE_WARR_UNYIELDING_WRATH_MIGTY_THROW = 201401,
    RUNE_WARR_PLANNED_BATTLE_BUFF = 201058,
};

class spell_cut_the_veins : public AuraScript
{
    PrepareAuraScript(spell_cut_the_veins);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200038))
            return GetCaster()->GetAura(200038);

        if (GetCaster()->HasAura(200039))
            return GetCaster()->GetAura(200039);

        if (GetCaster()->HasAura(200040))
            return GetCaster()->GetAura(200040);

        if (GetCaster()->HasAura(200041))
            return GetCaster()->GetAura(200041);

        if (GetCaster()->HasAura(200042))
            return GetCaster()->GetAura(200042);

        if (GetCaster()->HasAura(200043))
            return GetCaster()->GetAura(200043);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints;
    }

    int GetDamagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        if (!GetCaster())
            return;

        if (GetRuneAura())
        {
            int32 totalTicks = sSpellMgr->AssertSpellInfo(GetProcSpell())->GetMaxTicks();
            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetDamagePct()) / totalTicks);
            int32 maxAmount = int32(CalculatePct(GetCaster()->GetMaxHealth(), 50));

            if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(GetProcSpell(), 0))
            {
                int32 remainingTicks = totalTicks - protEff->GetTickNumber();
                int32 remainingAmount = protEff->GetAmount() * remainingTicks;
                int32 remainingAmountPerTick = remainingAmount / totalTicks;

                amount = (std::min<int32>(amount + remainingAmountPerTick, maxAmount));
            }
            eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), GetProcSpell(), SPELL_AURA_PERIODIC_DAMAGE, amount, TRIGGERED_IGNORE_AURA_SCALING);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_cut_the_veins::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_the_art_of_war : public AuraScript
{
    PrepareAuraScript(spell_the_art_of_war);

    int GetRagePct()
    {
        return GetAura()->GetEffect(EFFECT_0)->GetAmount();
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));
            int32 rageAccumulated = GetAura()->GetEffect(EFFECT_1)->GetAmount() + spellRage;

            if (spellRage <= 0)
                return;

            if (rageAccumulated >= GetRagePct())
            {
                GetCaster()->CastSpell(GetCaster(), 200093, TRIGGERED_FULL_MASK);
                GetAura()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - GetRagePct());
            }
            else
            {
                GetAura()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_the_art_of_war::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// crash...
class spell_tide_of_blood : public AuraScript
{
    PrepareAuraScript(spell_tide_of_blood);

    void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        Aura* aura = GetAura();
        Unit* caster = GetCaster();
        Unit* target = GetTarget();

        if (!target || GetCaster()->isDead())
            return;

        if (!aura)
            return;

        Aura* auraStack = caster->GetAura(200107);

        if (!auraStack)
            return;

        uint32 stacksAmount = auraStack->GetStackAmount();
        uint32 requireStacks = aura->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;

        if (stacksAmount < requireStacks)
            return;

        int32 damage = aura->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints + 1;
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, 0);
        damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

        caster->CastCustomSpell(200106, SPELLVALUE_BASE_POINT0, damage, caster, TRIGGERED_FULL_MASK);
        caster->RemoveAura(200107);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_tide_of_blood::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_vein_cutter : public AuraScript
{
    PrepareAuraScript(spell_vein_cutter);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200108))
            return GetCaster()->GetAura(200108);

        if (GetCaster()->HasAura(200109))
            return GetCaster()->GetAura(200109);

        if (GetCaster()->HasAura(200110))
            return GetCaster()->GetAura(200110);

        if (GetCaster()->HasAura(200111))
            return GetCaster()->GetAura(200111);

        if (GetCaster()->HasAura(200112))
            return GetCaster()->GetAura(200112);

        if (GetCaster()->HasAura(200113))
            return GetCaster()->GetAura(200113);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints;
    }

    int GetDamagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (GetRuneAura())
        {
            int32 totalTicks = sSpellMgr->AssertSpellInfo(GetProcSpell())->GetMaxTicks();
            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetDamagePct()) / totalTicks);
            int32 maxAmount = int32(CalculatePct(GetCaster()->GetMaxHealth(), 50));

            if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(GetProcSpell(), 0))
            {
                int32 remainingTicks = totalTicks - protEff->GetTickNumber();
                int32 remainingAmount = protEff->GetAmount() * remainingTicks;
                int32 remainingAmountPerTick = remainingAmount / totalTicks;

                amount = (std::min<int32>(amount + remainingAmountPerTick, maxAmount));
            }
            eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), GetProcSpell(), SPELL_AURA_PERIODIC_DAMAGE, amount, TRIGGERED_IGNORE_AURA_SCALING);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_vein_cutter::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_battlelord : public AuraScript
{
    PrepareAuraScript(spell_battlelord);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* caster = GetTarget()->ToPlayer())
        {
            caster->RemoveSpellCooldown(47486, true);
            caster->RemoveSpellCooldown(47520, true);
            /*caster->ModifySpellCooldown(47486, -aurEff->GetAmount());
            caster->ModifySpellCooldown(47520, -aurEff->GetAmount());*/
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_battlelord::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_tactician : public AuraScript
{
    PrepareAuraScript(spell_tactician);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));
        float procPctPerRagePoint = aurEff->GetSpellInfo()->Effects[EFFECT_0].DamageMultiplier;

        if (spellRage <= 0)
            return;

        float procChance = spellRage * procPctPerRagePoint;
        uint32 random = urand(1, 100);

        if (random <= procChance)
            GetCaster()->ToPlayer()->ModifySpellCooldown(7384, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_tactician::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_anger_management : public AuraScript
{
    PrepareAuraScript(spell_anger_management);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));
            int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount() + spellRage;
            int32 rageThreshold = aurEff->GetAmount();

            if (spellRage <= 0)
                return;

            if (rageAccumulated >= rageThreshold)
            {
                GetCaster()->ToPlayer()->ModifySpellCooldown(46924, -aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount());
                aurEff->GetBase()->GetEffect(EFFECT_2)->SetAmount(rageAccumulated - rageThreshold);
            }
            else
                aurEff->GetBase()->GetEffect(EFFECT_2)->SetAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_anger_management::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_blood_dance : public AuraScript
{
    PrepareAuraScript(spell_blood_dance);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetProcTarget();

        if (!target || target->isDead())
            return;

        if (!target->HasAura(47465))
            return;

        AuraEffect* rendEff = target->GetAuraEffect(47465, 0);
        int32 tickDamage = rendEff->GetAmount();
        int32 remainingTicks = rendEff->GetSpellInfo()->GetMaxTicks() - rendEff->GetTickNumber();
        int32 remainingDamage = tickDamage * remainingTicks;

        int32 amount = CalculatePct(remainingDamage, aurEff->GetAmount());

        GetCaster()->CastCustomSpell(200217, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_blood_dance::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_keep_spinnin : public AuraScript
{
    PrepareAuraScript(spell_keep_spinnin);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->ModifySpellCooldown(46924, -aurEff->GetBase()->GetEffect(EFFECT_0)->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_keep_spinnin::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sweeping_rage : public AuraScript
{
    PrepareAuraScript(spell_sweeping_rage);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            if (!GetCaster()->HasAura(12328))
                return;

            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));

            if (spellRage <= 0)
                return;

            int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
            int32 rageThreshold = aurEff->GetAmount();

            if (rageAccumulated >= rageThreshold)
            {
                GetCaster()->CastSpell(GetCaster(), 200244, TRIGGERED_FULL_MASK);
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
            }
            else
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sweeping_rage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sweeping_rage_proc : public AuraScript
{
    PrepareAuraScript(spell_sweeping_rage_proc);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->HasAura(200244))
            return;

        int32 stackAmount = GetCaster()->GetAura(200244)->GetStackAmount();

        GetCaster()->RemoveAura(200244);
        GetCaster()->CastSpell(GetCaster(), 200245, TRIGGERED_FULL_MASK);
        GetCaster()->GetAura(200245)->SetStackAmount(stackAmount);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_sweeping_rage_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_collateral_damage : public AuraScript
{
    PrepareAuraScript(spell_collateral_damage);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(12328);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_collateral_damage::CheckProc);
    }
};

class spell_collateral_damage_proc : public AuraScript
{
    PrepareAuraScript(spell_collateral_damage_proc);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200246))
            return GetCaster()->GetAura(200246);

        if (GetCaster()->HasAura(200247))
            return GetCaster()->GetAura(200247);

        if (GetCaster()->HasAura(200248))
            return GetCaster()->GetAura(200248);

        if (GetCaster()->HasAura(200249))
            return GetCaster()->GetAura(200249);

        if (GetCaster()->HasAura(200250))
            return GetCaster()->GetAura(200250);

        if (GetCaster()->HasAura(200251))
            return GetCaster()->GetAura(200251);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->HasAura(200252))
            return;

        if (!GetCaster()->IsAlive())
            return;

        int32 stackAmount = GetCaster()->GetAura(200252)->GetStackAmount();
        int32 damageAmount = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->RemoveAura(200252);
        GetCaster()->CastCustomSpell(200253, SPELLVALUE_BASE_POINT0, damageAmount, GetCaster(), TRIGGERED_FULL_MASK);
        GetCaster()->GetAura(200253)->SetStackAmount(stackAmount);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_collateral_damage_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_vicious_comtempt : public AuraScript
{
    PrepareAuraScript(spell_vicious_comtempt);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(23881, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_vicious_comtempt::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_charging_for_blood : public AuraScript
{
    PrepareAuraScript(spell_charging_for_blood);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(23881, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_charging_for_blood::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_bloodcraze : public AuraScript
{
    PrepareAuraScript(spell_bloodcraze);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastSpell(eventInfo.GetActionTarget(), 23881, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_bloodcraze::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_fervor_of_battle : public SpellScript
{
    PrepareSpellScript(spell_fervor_of_battle);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200370))
            return GetCaster()->GetAura(200370);

        if (GetCaster()->HasAura(200371))
            return GetCaster()->GetAura(200371);

        if (GetCaster()->HasAura(200372))
            return GetCaster()->GetAura(200372);

        if (GetCaster()->HasAura(200373))
            return GetCaster()->GetAura(200373);

        if (GetCaster()->HasAura(200374))
            return GetCaster()->GetAura(200374);

        if (GetCaster()->HasAura(200375))
            return GetCaster()->GetAura(200375);

        return nullptr;
    }

    void FindTargets(std::list<WorldObject*>& targets)
    {
        if (!GetRuneAura())
            return;
        if (targets.size() > 0)
        {
            targets.resize(1);
            for (auto const& target : targets)
                if (Unit* unit = target->ToUnit())
                {
                    GetCaster()->CastSpell(unit, RUNE_WARR_FERVOR_OF_BATTLE_SLAM, TRIGGERED_FULL_MASK);
                    return;
                }
        }
        else
        {
            GetCaster()->CastSpell(GetExplTargetUnit(), RUNE_WARR_FERVOR_OF_BATTLE_SLAM, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fervor_of_battle::FindTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_storm_of_swords : public AuraScript
{
    PrepareAuraScript(spell_storm_of_swords);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (GetCaster()->HasAura(1719))
        {
            float remainingDuration = GetCaster()->GetAura(1719)->GetDuration();
            GetCaster()->GetAura(1719)->SetDuration(remainingDuration + 2000);
        }
        else
        {
            GetCaster()->CastSpell(GetCaster(), 1719, TRIGGERED_FULL_MASK);
            GetCaster()->GetAura(1719)->SetDuration(5000);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_storm_of_swords::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_spinning_grip : public SpellScript
{
    PrepareSpellScript(spell_spinning_grip);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200388))
            return GetCaster()->GetAura(200388);

        if (GetCaster()->HasAura(200389))
            return GetCaster()->GetAura(200389);

        if (GetCaster()->HasAura(200390))
            return GetCaster()->GetAura(200390);

        if (GetCaster()->HasAura(200391))
            return GetCaster()->GetAura(200391);

        if (GetCaster()->HasAura(200392))
            return GetCaster()->GetAura(200392);

        if (GetCaster()->HasAura(200393))
            return GetCaster()->GetAura(200393);

        return nullptr;
    }

    void FindTargets(std::list<WorldObject*>& targets)
    {
        if (!GetRuneAura())
            return;

        if (targets.size() > 0)
        {
            for (auto const& target : targets)
                if (Unit* unit = target->ToUnit())
                    unit->CastSpell(GetCaster(), 200394, TRIGGERED_FULL_MASK);
        }
    }


    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_spinning_grip::FindTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_spinning_grip_jump : public SpellScript
{
    PrepareSpellScript(spell_spinning_grip_jump);

    void HandleSpecial(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);

        float x = GetHitUnit()->GetPositionX();
        float y = GetHitUnit()->GetPositionY();
        float z = GetHitUnit()->GetPositionZ() + 0.1f;
        float speedXY, speedZ;

        if (GetSpellInfo()->Effects[effIndex].MiscValue)
            speedZ = float(GetSpellInfo()->Effects[effIndex].MiscValue) / 10;
        else if (GetSpellInfo()->Effects[effIndex].MiscValueB)
            speedZ = float(GetSpellInfo()->Effects[effIndex].MiscValueB) / 10;
        else
            speedZ = 5.0f;
        speedXY = GetCaster()->GetExactDist2d(x, y) * 10.0f / speedZ;

        GetCaster()->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ);
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_spinning_grip_jump::HandleSpecial, EFFECT_0, SPELL_EFFECT_JUMP);
    }
};

class spell_depths_of_insanity : public AuraScript
{
    PrepareAuraScript(spell_depths_of_insanity);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 spellRage =  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));

        if (spellRage <= 0)
            return;

        int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
        int32 rageThreshold = aurEff->GetAmount();

        if (rageAccumulated >= rageThreshold)
        {
            GetCaster()->ToPlayer()->ModifySpellCooldown(SPELL_WARR_RECKLESSNESS, -aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount());
            GetCaster()->ToPlayer()->ModifySpellCooldown(SPELL_WARR_RAVAGER, -aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount());
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_depths_of_insanity::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_reckless_abandon : public AuraScript
{
    PrepareAuraScript(spell_reckless_abandon);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            if (!GetCaster()->HasAura(1719))
                return;

            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));

            if (spellRage <= 0)
                return;

            int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
            int32 rageThreshold = aurEff->GetAmount();

            if (rageAccumulated >= rageThreshold)
            {
                GetCaster()->CastSpell(GetCaster(), 200414, TRIGGERED_FULL_MASK);
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
            }
            else
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_reckless_abandon::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_reckless_abandon_proc : public AuraScript
{
    PrepareAuraScript(spell_reckless_abandon_proc);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->HasAura(200414))
            return;

        if (!GetCaster()->IsAlive())
            return;

        int32 stackAmount = GetCaster()->GetAura(200414)->GetStackAmount();

        GetCaster()->RemoveAura(200414);
        GetCaster()->CastSpell(GetCaster(), 200415, TRIGGERED_FULL_MASK);
        GetCaster()->GetAura(200415)->SetStackAmount(stackAmount);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_reckless_abandon_proc::HandleProc, EFFECT_0, SPELL_AURA_MOD_RAGE_FROM_DAMAGE_DEALT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_true_rage : public AuraScript
{
    PrepareAuraScript(spell_true_rage);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (GetCaster()->GetShapeshiftForm() != FORM_BERSERKERSTANCE)
            return;

        if (GetCaster()->HasAura(200004))
        {
            float remainingDuration = GetCaster()->GetAura(200004)->GetDuration();
            GetCaster()->GetAura(200004)->SetDuration(remainingDuration + 4000);
        }
        else
        {
            GetCaster()->CastSpell(GetCaster(), 200004, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_true_rage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_relentless : public SpellScript
{
    PrepareSpellScript(rune_relentless);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200428))
            return GetCaster()->GetAura(200428);

        if (GetCaster()->HasAura(200429))
            return GetCaster()->GetAura(200429);

        if (GetCaster()->HasAura(200430))
            return GetCaster()->GetAura(200430);

        if (GetCaster()->HasAura(200431))
            return GetCaster()->GetAura(200431);

        if (GetCaster()->HasAura(200432))
            return GetCaster()->GetAura(200432);

        if (GetCaster()->HasAura(200433))
            return GetCaster()->GetAura(200433);

        return nullptr;
    }

    void HandleCast()
    {
        if (Aura* runeAura = GetRuneAura())
        {
            Unit* caster = GetCaster();
            int32 stackThreshold = runeAura->GetEffect(EFFECT_0)->GetAmount();

            caster->CastSpell(caster, RUNE_WARR_RELENTLESS_STACK, TRIGGERED_FULL_MASK);

            if (Aura* relentlessAura = caster->GetAura(RUNE_WARR_RELENTLESS_STACK))
                if (relentlessAura->GetStackAmount() >= stackThreshold)
                {
                    relentlessAura->Remove();
                    caster->CastSpell(caster, RUNE_WARR_RELENTLESS_BUFF, TRIGGERED_FULL_MASK);
                }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(rune_relentless::HandleCast);
    }
};

class rune_relentless_remove : public AuraScript
{
    PrepareAuraScript(rune_relentless_remove);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Aura* stackAura = GetCaster()->GetAura(RUNE_WARR_RELENTLESS_STACK))
            stackAura->Remove();
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_relentless_remove::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_raging_death : public AuraScript
{
    PrepareAuraScript(spell_raging_death);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));

            if (spellRage <= 0)
                return;

            int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
            int32 rageThreshold = aurEff->GetAmount();
            if (rageAccumulated >= rageThreshold)
            {
                GetCaster()->ToPlayer()->ModifySpellCooldown(12292, -aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount());
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
            }
            else
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_raging_death::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_son_of_thunder : public AuraScript
{
    PrepareAuraScript(spell_son_of_thunder);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (GetCaster()->GetShapeshiftForm() != FORM_DEFENSIVESTANCE)
            return;

        GetCaster()->CastSpell(GetCaster(), SPELL_WARR_THUNDERCLAP_SON_OF_THUNDER, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_son_of_thunder::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_best_served_cold : public AuraScript
{
    PrepareAuraScript(spell_best_served_cold);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = GetTarget();

        if (!target || GetCaster()->isDead())
            return;

        uint32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));

        Aura* shield = caster->GetAura(200496);

        if (shield)
            amount += shield->GetEffect(EFFECT_0)->GetAmount();

        caster->CastCustomSpell(200496, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_best_served_cold::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_side_step : public AuraScript
{
    PrepareAuraScript(spell_side_step);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200510))
            return GetCaster()->GetAura(200510);

        if (GetCaster()->HasAura(200511))
            return GetCaster()->GetAura(200511);

        if (GetCaster()->HasAura(200512))
            return GetCaster()->GetAura(200512);

        if (GetCaster()->HasAura(200513))
            return GetCaster()->GetAura(200513);

        if (GetCaster()->HasAura(200514))
            return GetCaster()->GetAura(200514);

        if (GetCaster()->HasAura(200515))
            return GetCaster()->GetAura(200515);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        if (!GetCaster()->IsAlive())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_side_step::HandleProc, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_shield_parry : public AuraScript
{
    PrepareAuraScript(spell_shield_parry);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200522))
            return GetCaster()->GetAura(200522);

        if (GetCaster()->HasAura(200523))
            return GetCaster()->GetAura(200523);

        if (GetCaster()->HasAura(200524))
            return GetCaster()->GetAura(200524);

        if (GetCaster()->HasAura(200525))
            return GetCaster()->GetAura(200525);

        if (GetCaster()->HasAura(200526))
            return GetCaster()->GetAura(200526);

        if (GetCaster()->HasAura(200527))
            return GetCaster()->GetAura(200527);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        if (!GetCaster()->IsAlive())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_shield_parry::HandleProc, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_magic_barrier : public AuraScript
{
    PrepareAuraScript(spell_magic_barrier);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200534))
            return GetCaster()->GetAura(200534);

        if (GetCaster()->HasAura(200535))
            return GetCaster()->GetAura(200535);

        if (GetCaster()->HasAura(200536))
            return GetCaster()->GetAura(200536);

        if (GetCaster()->HasAura(200537))
            return GetCaster()->GetAura(200537);

        if (GetCaster()->HasAura(200538))
            return GetCaster()->GetAura(200538);

        if (GetCaster()->HasAura(200539))
            return GetCaster()->GetAura(200539);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        uint32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        if (GetCaster()->HasAura(buffAura))
            GetCaster()->RemoveAura(buffAura);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_magic_barrier::HandleProc, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_magic_barrier::HandleRemove, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_enduring_defenses_shield_block : public AuraScript
{
    PrepareAuraScript(spell_enduring_defenses_shield_block);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200546))
            return GetCaster()->GetAura(200546);

        if (GetCaster()->HasAura(200547))
            return GetCaster()->GetAura(200547);

        if (GetCaster()->HasAura(200548))
            return GetCaster()->GetAura(200548);

        if (GetCaster()->HasAura(200549))
            return GetCaster()->GetAura(200549);

        if (GetCaster()->HasAura(200550))
            return GetCaster()->GetAura(200550);

        if (GetCaster()->HasAura(200551))
            return GetCaster()->GetAura(200551);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        uint32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;

        if (GetCaster()->HasAura(buffAura))
            GetCaster()->RemoveAura(buffAura);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_enduring_defenses_shield_block::HandleProc, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_enduring_defenses_shield_block::HandleRemove, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_enduring_defenses_shield_slam : public AuraScript
{
    PrepareAuraScript(spell_enduring_defenses_shield_slam);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Aura* shieldBlock = GetCaster()->GetAura(2565);

        if (!shieldBlock)
            return;

        shieldBlock->SetDuration(shieldBlock->GetDuration() + aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_enduring_defenses_shield_slam::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_barrier_lord : public AuraScript
{
    PrepareAuraScript(spell_barrier_lord);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->ModifySpellCooldown(2565, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_barrier_lord::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sword_and_board : public AuraScript
{
    PrepareAuraScript(spell_sword_and_board);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* caster = GetTarget()->ToPlayer())
            caster->RemoveSpellCooldown(47488, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sword_and_board::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_violent_outburst : public AuraScript
{
    PrepareAuraScript(rune_violent_outburst);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));
            if (spellRage <= 0)
                return;

            int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount() + spellRage;
            int32 rageThreshold = aurEff->GetAmount();

            if (rageAccumulated >= rageThreshold)
            {
                aurEff->GetBase()->GetEffect(EFFECT_2)->SetAmount(rageAccumulated - rageThreshold);
                GetCaster()->CastSpell(GetCaster(), 200612, TRIGGERED_FULL_MASK);

                if (Aura* aura = GetCaster()->GetAura(200612))
                {
                    if (aura->GetStackAmount() >= 8)
                    {
                        aura->Remove();
                        GetCaster()->CastSpell(GetCaster(), aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount(), TRIGGERED_FULL_MASK);
                    }
                }
            }
            else
                aurEff->GetBase()->GetEffect(EFFECT_2)->SetAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_violent_outburst::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_defenders_aegis : public AuraScript
{
    PrepareAuraScript(spell_defenders_aegis);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));

            if (spellRage <= 0)
                return;

            if (!GetCaster()->HasAura(871))
                return;

            int32 newDuration = GetCaster()->GetAura(871)->GetDuration() + aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount();
            int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
            int32 rageThreshold = aurEff->GetAmount();

            if (rageAccumulated >= rageThreshold)
            {
                GetCaster()->GetAura(871)->SetDuration(newDuration);
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
            }
            else
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_defenders_aegis::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_offensive_wall : public AuraScript
{
    PrepareAuraScript(spell_offensive_wall);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* caster = GetTarget()->ToPlayer())
            caster->ModifySpellCooldown(871, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_offensive_wall::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_recuperation : public AuraScript
{
    PrepareAuraScript(spell_recuperation);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200631))
            return GetCaster()->GetAura(200631);

        if (GetCaster()->HasAura(200632))
            return GetCaster()->GetAura(200632);

        if (GetCaster()->HasAura(200633))
            return GetCaster()->GetAura(200633);

        if (GetCaster()->HasAura(200634))
            return GetCaster()->GetAura(200634);

        if (GetCaster()->HasAura(200635))
            return GetCaster()->GetAura(200635);

        if (GetCaster()->HasAura(200636))
            return GetCaster()->GetAura(200636);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        if (!GetCaster()->IsAlive())
            return;

        uint32 amount = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();
        GetCaster()->CastCustomSpell(200637, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_recuperation::HandleProc, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_rageful_stand : public AuraScript
{
    PrepareAuraScript(spell_rageful_stand);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));

            if (spellRage <= 0)
                return;

            int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
            int32 rageThreshold = aurEff->GetAmount();
            if (rageAccumulated >= rageThreshold)
            {
                GetCaster()->ToPlayer()->ModifySpellCooldown(12975, -aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount());
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
            }
            else
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rageful_stand::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_unnerving_focus : public AuraScript
{
    PrepareAuraScript(spell_unnerving_focus);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200644))
            return GetCaster()->GetAura(200644);

        if (GetCaster()->HasAura(200645))
            return GetCaster()->GetAura(200645);

        if (GetCaster()->HasAura(200646))
            return GetCaster()->GetAura(200646);

        if (GetCaster()->HasAura(200647))
            return GetCaster()->GetAura(200647);

        if (GetCaster()->HasAura(200648))
            return GetCaster()->GetAura(200648);

        if (GetCaster()->HasAura(200649))
            return GetCaster()->GetAura(200649);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        uint32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        if (GetCaster()->HasAura(buffAura))
            GetCaster()->RemoveAura(buffAura);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_unnerving_focus::HandleProc, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_unnerving_focus::HandleRemove, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_preparation : public AuraScript
{
    PrepareAuraScript(spell_preparation);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* caster = GetTarget()->ToPlayer())
            caster->ModifySpellCooldown(12809, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_preparation::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_inspiring_wall : public AuraScript
{
    PrepareAuraScript(spell_inspiring_wall);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200674))
            return GetCaster()->GetAura(200674);

        if (GetCaster()->HasAura(200675))
            return GetCaster()->GetAura(200675);

        if (GetCaster()->HasAura(200676))
            return GetCaster()->GetAura(200676);

        if (GetCaster()->HasAura(200677))
            return GetCaster()->GetAura(200677);

        if (GetCaster()->HasAura(200678))
            return GetCaster()->GetAura(200678);

        if (GetCaster()->HasAura(200679))
            return GetCaster()->GetAura(200679);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        uint32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        if (GetCaster()->HasAura(buffAura))
            GetCaster()->RemoveAura(buffAura);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_inspiring_wall::HandleProc, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_inspiring_wall::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_inspiring_wall_heal : public AuraScript
{
    PrepareAuraScript(spell_inspiring_wall_heal);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && aurEff) {
            if (eventInfo.GetHitMask() != PROC_EX_CRITICAL_HIT)
                return;

            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));

            if (amount <= 0)
                return;

            GetCaster()->CastCustomSpell(200686, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActor(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_inspiring_wall_heal::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_defensive_quickness : public AuraScript
{
    PrepareAuraScript(spell_defensive_quickness);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->GetShapeshiftForm() == FORM_DEFENSIVESTANCE;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_defensive_quickness::CheckProc);
    }
};

class spell_undying_will : public AuraScript
{
    PrepareAuraScript(spell_undying_will);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->GetShapeshiftForm() == FORM_DEFENSIVESTANCE;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_undying_will::CheckProc);
    }
};

class spell_rumbling_earth : public SpellScript
{
    PrepareSpellScript(spell_rumbling_earth);

    uint32 auraShockwaveStack = 80009;

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200714))
            return GetCaster()->GetAura(200714);

        if (GetCaster()->HasAura(200715))
            return GetCaster()->GetAura(200715);

        if (GetCaster()->HasAura(200716))
            return GetCaster()->GetAura(200716);

        if (GetCaster()->HasAura(200717))
            return GetCaster()->GetAura(200717);

        if (GetCaster()->HasAura(200718))
            return GetCaster()->GetAura(200718);

        if (GetCaster()->HasAura(200719))
            return GetCaster()->GetAura(200719);

        return nullptr;
    }

    void HandleOnHit()
    {
        if (!GetRuneAura())
            return;

        if (GetHitUnit() && GetHitUnit()->GetTypeId() == TYPEID_UNIT && GetCaster()) {
            if (Aura* aura = GetCaster()->GetAura(auraShockwaveStack))
            {
                if (aura->GetStackAmount() >= 2)
                {
                    int32 cooldown = aura->GetDuration();
                    GetCaster()->ToPlayer()->ModifySpellCooldown(46968, -GetRuneAura()->GetEffect(EFFECT_2)->GetAmount());
                    GetCaster()->RemoveAura(auraShockwaveStack);
                    return;
                }
                aura->ModStackAmount(1);
            }
            else {
                GetCaster()->AddAura(auraShockwaveStack, GetCaster());
            }
        }
    }

    void HandleOnCast()
    {
        if (Aura* aura = GetCaster()->GetAura(auraShockwaveStack))
            GetCaster()->RemoveAura(auraShockwaveStack);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_rumbling_earth::HandleOnHit);
        OnCast += SpellCastFn(spell_rumbling_earth::HandleOnCast);
    }
};

class spell_blade_turning : public AuraScript
{
    PrepareAuraScript(spell_blade_turning);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetBlock()) {
            float blockValue = eventInfo.GetDamageInfo()->GetBlock();

            if (blockValue <= 0)
                return;

            uint32 amount = int32(CalculatePct(blockValue, aurEff->GetAmount()));

            if (GetCaster()->HasAura(200726))
                amount += GetCaster()->GetAura(200726)->GetEffect(EFFECT_0)->GetAmount();

            GetCaster()->CastCustomSpell(200726, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_blade_turning::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_block_spike : public AuraScript
{
    PrepareAuraScript(spell_block_spike);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetBlock())
        {
            uint32 damageAmount = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount());

            GetCaster()->CastCustomSpell(RUNE_WARR_BLOCK_SPIKE_PROC, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActor(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_block_spike::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_healing_block : public AuraScript
{
    PrepareAuraScript(spell_healing_block);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetBlock()) {

            float blockValue = eventInfo.GetDamageInfo()->GetBlock();

            if (blockValue <= 0)
                return;

            uint32 amount = int32(CalculatePct(blockValue, aurEff->GetAmount()));

            GetCaster()->CastCustomSpell(200740, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_healing_block::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// Crash ..
class spell_hurricane : public AuraScript
{
    PrepareAuraScript(spell_hurricane);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200796))
            return GetCaster()->GetAura(200796);

        if (GetCaster()->HasAura(200797))
            return GetCaster()->GetAura(200797);

        if (GetCaster()->HasAura(200798))
            return GetCaster()->GetAura(200798);

        if (GetCaster()->HasAura(200799))
            return GetCaster()->GetAura(200799);

        if (GetCaster()->HasAura(200800))
            return GetCaster()->GetAura(200800);

        if (GetCaster()->HasAura(200801))
            return GetCaster()->GetAura(200801);

        return nullptr;
    }

    int GetProcAura()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster())
            return;

        if (!GetRuneAura())
            return;

        GetCaster()->AddAura(GetProcAura(), GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster())
            return;

        if (!GetRuneAura())
            return;

        if (!GetCaster()->HasAura(GetProcAura()))
            return;

        GetCaster()->RemoveAura(GetProcAura());
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_hurricane::HandleProc, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY_MASK, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_hurricane::HandleRemove, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY_MASK, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_improved_execute : public SpellScript
{
    PrepareSpellScript(rune_improved_execute);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200838))
            return GetCaster()->GetAura(200838);

        if (GetCaster()->HasAura(200839))
            return GetCaster()->GetAura(200839);

        if (GetCaster()->HasAura(200840))
            return GetCaster()->GetAura(200840);

        if (GetCaster()->HasAura(200841))
            return GetCaster()->GetAura(200841);

        if (GetCaster()->HasAura(200842))
            return GetCaster()->GetAura(200842);

        if (GetCaster()->HasAura(200843))
            return GetCaster()->GetAura(200843);

        return nullptr;
    }

    void OnAfterCast()
    {
        Unit* caster = GetCaster();
        uint32 currentRage = caster->GetPower(POWER_RAGE);
        uint32 leftRage = rageBeforeCast - currentRage;

        if (Unit* target = GetExplTargetUnit()) {
            if (target->IsAlive())
            {
                if (Aura* aura = GetRuneAura()) {
                    int32 pct = aura->GetEffect(EFFECT_0)->GetAmount();
                    uint32 newRage = CalculatePct(leftRage, pct);
                    caster->ModifyPower(POWER_RAGE, newRage);
                }
            }
        }
    }

    void OnBeforeCast()
    {
        Unit* caster = GetCaster();
        rageBeforeCast = caster->GetPower(POWER_RAGE);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(rune_improved_execute::OnAfterCast);
        BeforeCast += SpellCastFn(rune_improved_execute::OnBeforeCast);
    }

private:
    uint32 rageBeforeCast;

};

class rune_devastator : public AuraScript
{
    PrepareAuraScript(rune_devastator);


    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        int32 damage = int32(CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount()));

        caster->CastCustomSpell(RUNE_WARR_DEVASTATOR_PROC, SPELLVALUE_BASE_POINT0, damage, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
        caster->ToPlayer()->RemoveSpellCooldown(SPELL_WARR_SHIELD_SLAM, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_devastator::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_battle_scarred_veteran : public AuraScript
{
    PrepareAuraScript(rune_battle_scarred_veteran);

public:
    rune_battle_scarred_veteran() { }
private:

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* victim = GetTarget();
        int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
        uint32 allowedHealth = victim->CountPctFromMaxHealth(30.f);

        uint32 battleScaredProcTriggerSpell = aurEff->GetAmount();

        if ((remainingHealth < int32(allowedHealth)) && !victim->ToPlayer()->HasSpellCooldown(battleScaredProcTriggerSpell))
        {
            victim->CastSpell(victim, battleScaredProcTriggerSpell);
            victim->ToPlayer()->AddSpellCooldown(battleScaredProcTriggerSpell, 0, 180000);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(rune_battle_scarred_veteran::CalculateAmount, EFFECT_2, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(rune_battle_scarred_veteran::Absorb, EFFECT_2);
    }
};

class rune_bonegrinder : public AuraScript
{
    PrepareAuraScript(rune_bonegrinder);

    Aura* GetRuneAura()
    {
        for (size_t i = 202913; i < 202919; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->IsAlive())
            return;

        if (Aura* aura = GetRuneAura())
            GetCaster()->CastSpell(GetCaster(), aura->GetEffect(EFFECT_0)->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_bonegrinder::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hack_and_slash : public AuraScript
{
    PrepareAuraScript(rune_hack_and_slash);

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_WARR_RAGING_BLOW, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hack_and_slash::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_merciless : public AuraScript
{
    PrepareAuraScript(rune_merciless);

    Aura* GetRuneAura()
    {
        for (size_t i = 201253; i < 201259; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->IsAlive())
            return;

        if (Aura* runeAura = GetRuneAura())
            GetCaster()->CastSpell(GetCaster(), runeAura->GetEffect(EFFECT_1)->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_merciless::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_heavy_blocks : public AuraScript
{
    PrepareAuraScript(rune_heavy_blocks);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(SPELL_WARR_SHIELD_BLOCK))
        {
            caster->EnergizeBySpell(caster, SPELL_WARR_SHIELD_SLAM, aurEff->GetAmount(), POWER_RAGE);
            aura->SetDuration(aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + aura->GetDuration());
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_heavy_blocks::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_impenetrable_shield : public AuraScript
{
    PrepareAuraScript(rune_impenetrable_shield);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        caster->EnergizeBySpell(caster, SPELL_WARR_SHIELD_SLAM, aurEff->GetAmount(), POWER_RAGE);
        caster->ModifySpellCooldown(SPELL_WARR_SHIELD_WALL, aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_impenetrable_shield::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_bolster : public SpellScript
{
    PrepareSpellScript(rune_bolster);

    Aura* GetRuneAura()
    {
        for (size_t i = 200981; i < 200987; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleOnCast()
    {
        if (Aura* runeAura = GetRuneAura())
            GetCaster()->AddAura(SPELL_WARR_SHIELD_BLOCK, GetCaster());
    }


    void Register() override
    {
        OnCast += SpellCastFn(rune_bolster::HandleOnCast);
    }
};

class rune_fierce_strikes : public AuraScript
{
    PrepareAuraScript(rune_fierce_strikes);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->ModifySpellCooldown(SPELL_WARR_COLOSSAL_THRUST, aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_fierce_strikes::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_steel_impact : public AuraScript
{
    PrepareAuraScript(rune_steel_impact);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return eventInfo.GetActionTarget()->IsAlive();
    }

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 damagePct = aurEff->GetAmount();
        int32 damageAmount = CalculatePct(static_cast<int32>(eventInfo.GetDamageInfo()->GetDamage()), damagePct);

        GetCaster()->CastCustomSpell(RUNE_WARR_STEEL_IMPACT_PROC, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_steel_impact::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_steel_impact::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_rage_of_huln : public AuraScript
{
    PrepareAuraScript(rune_rage_of_huln);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            Player* caster = GetCaster()->ToPlayer();

            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));
            if (spellRage <= 0)
                return;

            int32 rageAccumulated = GetAura()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
            int32 rageThreshold = aurEff->GetAmount(); 

            if (rageAccumulated >= rageThreshold)
            {
                caster->ModifySpellCooldown(SPELL_WARR_HULN_FURY, -aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount());
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
            }
            else
                aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_rage_of_huln::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_gladiators_aegis : public AuraScript
{
    PrepareAuraScript(rune_gladiators_aegis);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 absorbPct = aurEff->GetAmount();
        uint32 absorbAmount = CalculatePct(GetCaster()->GetMaxHealth(), absorbPct);

        GetCaster()->CastCustomSpell(RUNE_WARR_GLADIATORS_AEGIS_PROC, SPELLVALUE_BASE_POINT0, absorbAmount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_gladiators_aegis::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_furious_onslaught: public AuraScript
{
    PrepareAuraScript(rune_furious_onslaught);

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_WARR_COLOSSAL_THRUST, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_furious_onslaught::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_impenetrable_barrier : public AuraScript
{
    PrepareAuraScript(rune_impenetrable_barrier);


    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 absorbPct = aurEff->GetAmount();
        uint32 absorbAmount = CalculatePct(GetCaster()->GetMaxHealth(), absorbPct);

        GetCaster()->CastCustomSpell(RUNE_WARR_IMPENETRABLE_BARRIER_PROC, SPELLVALUE_BASE_POINT0, absorbAmount, GetCaster(), TRIGGERED_FULL_MASK);

    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_impenetrable_barrier::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_depths_of_rage : public AuraScript
{
    PrepareAuraScript(rune_depths_of_rage);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            Player* caster = GetCaster()->ToPlayer();

            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));
            if (spellRage <= 0)
                return;

            int32 rageAccumulated = GetAura()->GetEffect(EFFECT_2)->GetAmount() + spellRage;
            int32 rageThreshold = aurEff->GetAmount();

            if (rageAccumulated >= rageThreshold)
            {
                caster->ModifySpellCooldown(SPELL_WARR_BATTLE_TRANCE , -aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount());
                aurEff->GetBase()->GetEffect(EFFECT_2)->SetAmount(rageAccumulated - rageThreshold);
            }
            else
                aurEff->GetBase()->GetEffect(EFFECT_2)->SetAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_depths_of_rage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_reprisal : public AuraScript
{
    PrepareAuraScript(rune_reprisal);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        int32 duration = aurEff->GetAmount();
        if (Aura* aura = caster->GetAura(SPELL_WARR_SHIELD_BLOCK))
        {
            float remainingDuration = aura->GetDuration();
            aura->SetDuration(remainingDuration + duration);
        }
        else
        {
            caster->AddAura(SPELL_WARR_SHIELD_BLOCK, caster);
            caster->GetAura(SPELL_WARR_SHIELD_BLOCK)->SetDuration(duration);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_reprisal::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_thunder_bolt : public AuraScript
{
    PrepareAuraScript(rune_thunder_bolt);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        if (!target)
            return false;

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return target->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastSpell(eventInfo.GetActionTarget(), SPELL_WARR_THUNDER_BOLT_THUNDER_CLAP, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_thunder_bolt::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_thunder_bolt::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_improved_heroic_throw : public AuraScript
{
    PrepareAuraScript(rune_improved_heroic_throw);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        if (!target)
            return false;

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return target->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (Aura* aura = caster->GetAura(MASTERY_WARR_DEEP_WOUNDS))
        {
            int32 damageAmount = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), aura->GetEffect(EFFECT_0)->GetAmount());
            eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), MASTERY_WARR_DEEP_WOUNDS_DAMAGE, SPELL_AURA_PERIODIC_DAMAGE, damageAmount);

            float vulnAmount = aura->GetEffect(EFFECT_1)->GetAmount() + caster->ToPlayer()->GetMastery();
            caster->CastCustomSpell(MASTERY_WARR_DEEP_WOUNDS_DAMAGE_INCREASE, SPELLVALUE_BASE_POINT0, vulnAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_improved_heroic_throw::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_improved_heroic_throw::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_blood_and_thunder : public AuraScript
{
    PrepareAuraScript(rune_blood_and_thunder);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        if (!target)
            return false;

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return target->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->AddAura(SPELL_WARR_REND, eventInfo.GetActionTarget());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_blood_and_thunder::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_blood_and_thunder::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_brutal_vitality : public AuraScript
{
    PrepareAuraScript(rune_brutal_vitality);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return GetCaster()->HasAura(SPELL_WARR_IGNORE_PAIN);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (Aura* aura = caster->GetAura(SPELL_WARR_IGNORE_PAIN))
        {
            int32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
            int32 currentAbsorb = aura->GetEffect(EFFECT_0)->GetAmount();
            aura->GetEffect(EFFECT_0)->ChangeAmount(amount + currentAbsorb);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_brutal_vitality::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_brutal_vitality::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_fatality : public AuraScript
{
    PrepareAuraScript(rune_fatality);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        if (!target)
            return false;

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return target->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        int32 targetHealth = target->GetHealthPct();

        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
            if (targetHealth > 30 && (spellInfo->Id == SPELL_WARR_MORTAL_STRIKE || spellInfo->Id == SPELL_WARR_CLEAVE))
                GetCaster()->CastSpell(target, RUNE_WARR_FATALITY_MARK, TRIGGERED_FULL_MASK);
            else if (targetHealth <= 30 && spellInfo->Id == SPELL_WARR_EXECUTE && target->HasAura(RUNE_WARR_FATALITY_MARK))
            {
                target->RemoveAura(RUNE_WARR_FATALITY_MARK);
                GetCaster()->CastSpell(target, RUNE_WARR_FATALITY_DAMAGE, TRIGGERED_FULL_MASK);
            }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_fatality::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_fatality::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_test_of_might : public AuraScript
{
    PrepareAuraScript(rune_test_of_might);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(RUNE_WARR_TEST_OF_MIGHT_COUNTER))
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));
            if (spellRage <= 0)
                return;

            if (Aura* mightCounter = GetCaster()->GetAura(RUNE_WARR_TEST_OF_MIGHT_COUNTER))
            {
                int32 rageAccumulated = mightCounter->GetEffect(EFFECT_0)->GetAmount() + spellRage;
                mightCounter->GetEffect(EFFECT_0)->ChangeAmount(rageAccumulated);
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_test_of_might::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_test_of_might::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_test_of_might_expire : public AuraScript
{
    PrepareAuraScript(rune_test_of_might_expire);

    Aura* GetRuneAura()
    {
        for (size_t i = 200881; i < 200887; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Aura* runeAura = GetRuneAura())
        {
            GetCaster()->AddAura(RUNE_WARR_TEST_OF_MIGHT_COUNTER, GetCaster());
        }
    }

    /*void HandleOverlap(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->IsAlive())
            return;

        if (Aura* runeAura = GetRuneAura())
        {
            if (Aura* mightCounter = GetCaster()->GetAura(RUNE_WARR_TEST_OF_MIGHT_COUNTER))
            {
                int32 threadshold = runeAura->GetEffect(EFFECT_0)->GetAmount();
                int32 rageAccumulated = mightCounter->GetEffect(EFFECT_0)->GetAmount();
                int32 calculatedStack = static_cast<int32>(rageAccumulated / threadshold);
                GetCaster()->CastCustomSpell(RUNE_WARR_TEST_OF_MIGHT_BUFF, SPELLVALUE_BASE_POINT0, calculatedStack, GetCaster(), TRIGGERED_FULL_MASK);
                mightCounter->GetEffect(EFFECT_0)->SetAmount(0);
            }
        }
    }*/

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster()->IsAlive())
            return;

        if (Aura* runeAura = GetRuneAura())
        {
            if (Aura* mightCounter = GetCaster()->GetAura(RUNE_WARR_TEST_OF_MIGHT_COUNTER))
            {
                int32 threadshold = runeAura->GetEffect(EFFECT_0)->GetAmount();
                int32 rageAccumulated = mightCounter->GetEffect(EFFECT_0)->GetAmount();
                int32 calculatedStack = static_cast<int32>(rageAccumulated / threadshold);
                GetCaster()->CastCustomSpell(RUNE_WARR_TEST_OF_MIGHT_BUFF, SPELLVALUE_BASE_POINT0, calculatedStack, GetCaster(), TRIGGERED_FULL_MASK);
                mightCounter->Remove();
            }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_test_of_might_expire::HandleApply, EFFECT_1, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        //OnEffectApply += AuraEffectApplyFn(rune_test_of_might_expire::HandleOverlap, EFFECT_1, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAPPLY);
        OnEffectRemove += AuraEffectRemoveFn(rune_test_of_might_expire::HandleRemove, EFFECT_1, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_in_for_the_kill : public AuraScript
{
    PrepareAuraScript(rune_in_for_the_kill);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        if (!target)
            return false;

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return target->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        int32 normalProc = aurEff->GetAmount();
        int32 improvedProc = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        if (eventInfo.GetActionTarget()->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT))
            caster->CastSpell(caster, improvedProc);
        else
            caster->CastSpell(caster, normalProc);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_in_for_the_kill::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_in_for_the_kill::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_strength_of_arms : public AuraScript
{
    PrepareAuraScript(rune_strength_of_arms);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        if (!target)
            return false;

        return target->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastCustomSpell(RUNE_WARR_STRENGTH_OF_ARMS_PROC, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_strength_of_arms::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_strength_of_arms::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_bloodmark : public AuraScript
{
    PrepareAuraScript(rune_bloodmark);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        if (!target)
            return false;

        return target->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastCustomSpell(RUNE_WARR_STRENGTH_OF_ARMS_PROC, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_bloodmark::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_bloodmark::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_cold_steel_hot_blood : public AuraScript
{
    PrepareAuraScript(rune_cold_steel_hot_blood);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        if (!target)
            return false;

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return target->IsAlive();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_cold_steel_hot_blood::CheckProc);
    }
};

class rune_berserkers_torment : public AuraScript
{
    PrepareAuraScript(rune_berserkers_torment);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
            return false;

        return GetCaster()->HasAura(SPELL_WARR_RECKLESSNESS);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 damageAmount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
        GetCaster()->CastCustomSpell(RUNE_WARR_BERSERKERS_TORMENT_PROC, SPELLVALUE_BASE_POINT0, damageAmount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_berserkers_torment::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_berserkers_torment::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_slaughtering_strikes : public AuraScript
{
    PrepareAuraScript(rune_slaughtering_strikes);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
            if (spellInfo->Id == SPELL_WARR_RAGING_BLOW)
                GetCaster()->CastSpell(GetCaster(), aurEff->GetAmount(), TRIGGERED_FULL_MASK);
            else if (spellInfo->Id == SPELL_WARR_ANNIHILATOR)
                GetCaster()->CastSpell(GetCaster(), GetAura()->GetEffect(EFFECT_1)->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_slaughtering_strikes::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_slaughtering_strikes::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_warpaint_and_cruelty : public AuraScript
{
    PrepareAuraScript(rune_warpaint_and_cruelty);

    Aura* GetWarpaintAura()
    {
        for (size_t i = 201253; i < 201259; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetCrueltyAura()
    {
        for (size_t i = 201260; i < 201266; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        int32 damageReduction = 0;
        int32 ragingBlowBuff = 0;
        int32 annihillatorBuff = 0;

        if (Aura* warpaintAura = GetWarpaintAura())
            damageReduction = warpaintAura->GetEffect(0)->GetAmount();

        if (Aura* crueltyAura = GetCrueltyAura())
        {
            ragingBlowBuff = crueltyAura->GetEffect(0)->GetAmount();
            annihillatorBuff = crueltyAura->GetEffect(1)->GetAmount();
        }

        GetCaster()->CastCustomSpell(GetCaster(), RUNE_WARR_ENRAGE_ADDITIONAL_BUFF, &damageReduction, &ragingBlowBuff, &annihillatorBuff, true, nullptr);
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Aura* buffAura = GetCaster()->GetAura(RUNE_WARR_ENRAGE_ADDITIONAL_BUFF))
            buffAura->Remove();
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_warpaint_and_cruelty::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_warpaint_and_cruelty::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_steel_resonance_revenge : public SpellScript
{
    PrepareSpellScript(rune_steel_resonance_revenge);

    Aura* GetRuneAura()
    {
        for (size_t i = 200963; i < 200969; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (Aura* runeAura = GetRuneAura())
        {
            if (targets.size() < 3)
                return;

            Unit* caster = GetCaster();
            int32 damagePct = runeAura->GetEffect(EFFECT_0)->GetAmount();
            int32 damageAmount = CalculatePct(CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), 20), damagePct);

            caster->CastCustomSpell(RUNE_WARR_STEEL_RESONANCE_REVENGE, SPELLVALUE_BASE_POINT0, damageAmount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_steel_resonance_revenge::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);  
    }
};

class rune_steel_resonance_whirlwind : public SpellScript
{
    PrepareSpellScript(rune_steel_resonance_whirlwind);

    Aura* GetRuneAura()
    {
        for (size_t i = 200963; i < 200969; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (Aura* runeAura = GetRuneAura())
        {
            if (targets.size() < 3)
                return;

            Unit* caster = GetCaster();
            int32 damagePct = runeAura->GetEffect(EFFECT_0)->GetAmount();
            int32 damageAmount = CalculatePct(CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), 20), damagePct);

            caster->CastCustomSpell(RUNE_WARR_STEEL_RESONANCE_WHIRLWIND, SPELLVALUE_BASE_POINT0, damageAmount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_steel_resonance_whirlwind::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class rune_champions_bulwark : public AuraScript
{
    PrepareAuraScript(rune_champions_bulwark);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        caster->EnergizeBySpell(caster, SPELL_WARR_SHIELD_CHARGE, aurEff->GetAmount(), POWER_RAGE);
        caster->AddAura(SPELL_WARR_SHIELD_BLOCK, caster);
        caster->AddAura(SPELL_WARR_REVENGE_BUFF, caster);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_champions_bulwark::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_critical_precision : public AuraScript
{
    PrepareAuraScript(rune_critical_precision);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
            if (spellInfo->Id == SPELL_WARR_CRUSHING_STRIKE)
                GetCaster()->EnergizeBySpell(GetCaster(), SPELL_WARR_CRUSHING_STRIKE, aurEff->GetAmount(), POWER_RAGE);
            else if (spellInfo->Id == SPELL_WARR_COLOSSAL_THRUST)
                GetCaster()->EnergizeBySpell(GetCaster(), SPELL_WARR_COLOSSAL_THRUST, aurEff->GetAmount(), POWER_RAGE);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_critical_precision::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_critical_precision::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_masterful_swipe : public SpellScript
{
    PrepareSpellScript(rune_masterful_swipe);

    Aura* GetRuneAura()
    {
        for (size_t i = 201032; i < 201038; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (Aura* runeAura = GetRuneAura())
        {
            if (targets.size() < 3)
                return;

            Unit* caster = GetCaster();
            int32 damagePct = runeAura->GetEffect(EFFECT_0)->GetAmount();
            int32 damageAmount = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), damagePct);

            caster->CastCustomSpell(RUNE_WARR_STEEL_RESONANCE_WHIRLWIND, SPELLVALUE_BASE_POINT0, damageAmount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_masterful_swipe::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
    }
};

class rune_warriors_wrath : public AuraScript
{
    PrepareAuraScript(rune_warriors_wrath);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->EnergizeBySpell(GetCaster(), SPELL_WARR_SPEAR_SWIPE, aurEff->GetAmount(), POWER_RAGE);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_warriors_wrath::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_warriors_wrath::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_gladiators_torment : public AuraScript
{
    PrepareAuraScript(rune_gladiators_torment);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
            return false;

        return GetCaster()->IsAlive() && GetCaster()->HasAura(SPELL_WARR_BATTLE_TRANCE);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        int32 damageAmount = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_WARR_GLADIATORS_TORMENT_PROC, SPELLVALUE_BASE_POINT0, damageAmount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_gladiators_torment::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_gladiators_torment::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_unyielding_wrath : public AuraScript
{
    PrepareAuraScript(rune_unyielding_wrath);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        if (!target || target->isDead())
            return false;

        return GetCaster()->IsAlive() && GetCaster()->HasAura(SPELL_WARR_BATTLE_TRANCE);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastSpell(eventInfo.GetActionTarget(), RUNE_WARR_UNYIELDING_WRATH_MIGTY_THROW, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_unyielding_wrath::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_unyielding_wrath::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_planned_battle : public AuraScript
{
    PrepareAuraScript(rune_planned_battle);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(SPELL_WARR_BATTLE_TRANCE))
            return false;

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            int32 spellRage = spellInfo->CalcPowerCost(GetCaster(), SpellSchoolMask(spellInfo->SchoolMask));
            if (spellRage <= 0)
                return;

            int32 rageAccumulated = GetAura()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
            GetAura()->GetEffect(EFFECT_1)->ChangeAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_planned_battle::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_planned_battle::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_planned_battle_expire : public AuraScript
{
    PrepareAuraScript(rune_planned_battle_expire);

    Aura* GetRuneAura()
    {
        for (size_t i = 201052; i < 201058; i++)
        {
            if (GetCaster()->HasAura(i))
                return GetCaster()->GetAura(i);
        }

        return nullptr;
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster()->IsAlive())
            return;

        if (Aura* runeAura = GetRuneAura())
        {
            int32 threshshold = runeAura->GetEffect(EFFECT_0)->GetAmount();
            int32 rageAccumulated = runeAura->GetEffect(EFFECT_1)->GetAmount();
            runeAura->GetEffect(EFFECT_1)->SetAmount(0);
            int32 calculatedStack = static_cast<int32>(rageAccumulated / threshshold);
            GetCaster()->CastCustomSpell(RUNE_WARR_PLANNED_BATTLE_BUFF, SPELLVALUE_BASE_POINT0, calculatedStack, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_planned_battle_expire::HandleRemove, EFFECT_0, SPELL_AURA_MOD_RAGE_FROM_DAMAGE_DEALT, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_warrior_perks_scripts()
{
    RegisterSpellScript(rune_improved_execute);
    RegisterSpellScript(rune_brutal_vitality);
    RegisterSpellScript(rune_devastator);
    RegisterSpellScript(rune_battle_scarred_veteran);
    RegisterSpellScript(rune_bonegrinder);
    RegisterSpellScript(rune_merciless);
    RegisterSpellScript(rune_hack_and_slash);
    RegisterSpellScript(rune_heavy_blocks);
    RegisterSpellScript(rune_impenetrable_shield);
    RegisterSpellScript(rune_bolster);
    RegisterSpellScript(rune_fierce_strikes);
    RegisterSpellScript(rune_steel_impact);
    RegisterSpellScript(rune_rage_of_huln);
    RegisterSpellScript(rune_gladiators_aegis);
    RegisterSpellScript(rune_furious_onslaught);
    RegisterSpellScript(rune_impenetrable_barrier);
    RegisterSpellScript(rune_depths_of_rage);
    RegisterSpellScript(rune_gladiators_torment);
    RegisterSpellScript(spell_cut_the_veins);
    RegisterSpellScript(spell_the_art_of_war);
    RegisterSpellScript(spell_tide_of_blood);
    RegisterSpellScript(spell_vein_cutter);
    RegisterSpellScript(spell_battlelord);
    RegisterSpellScript(spell_tactician);
    RegisterSpellScript(spell_anger_management);
    RegisterSpellScript(spell_blood_dance);
    RegisterSpellScript(spell_keep_spinnin);
    RegisterSpellScript(spell_sweeping_rage);
    RegisterSpellScript(spell_sweeping_rage_proc);
    RegisterSpellScript(spell_collateral_damage);
    RegisterSpellScript(spell_collateral_damage_proc);
    RegisterSpellScript(spell_vicious_comtempt);
    RegisterSpellScript(spell_charging_for_blood);
    RegisterSpellScript(spell_bloodcraze);
    RegisterSpellScript(spell_fervor_of_battle);
    RegisterSpellScript(spell_storm_of_swords);
    RegisterSpellScript(spell_spinning_grip);
    RegisterSpellScript(spell_spinning_grip_jump);
    RegisterSpellScript(spell_depths_of_insanity);
    RegisterSpellScript(spell_reckless_abandon);
    RegisterSpellScript(spell_reckless_abandon_proc);
    RegisterSpellScript(spell_true_rage);
    RegisterSpellScript(rune_relentless);
    RegisterSpellScript(rune_relentless_remove);
    RegisterSpellScript(spell_raging_death);
    RegisterSpellScript(spell_son_of_thunder);
    RegisterSpellScript(spell_best_served_cold);
    RegisterSpellScript(spell_side_step);
    RegisterSpellScript(spell_shield_parry);
    RegisterSpellScript(spell_magic_barrier);
    RegisterSpellScript(spell_enduring_defenses_shield_block);
    RegisterSpellScript(spell_enduring_defenses_shield_slam);
    RegisterSpellScript(spell_barrier_lord);
    RegisterSpellScript(spell_sword_and_board);
    RegisterSpellScript(rune_violent_outburst);
    RegisterSpellScript(spell_defenders_aegis);
    RegisterSpellScript(spell_offensive_wall);
    RegisterSpellScript(spell_recuperation);
    RegisterSpellScript(spell_rageful_stand);
    RegisterSpellScript(spell_unnerving_focus);
    RegisterSpellScript(spell_preparation);
    RegisterSpellScript(spell_inspiring_wall);
    RegisterSpellScript(spell_inspiring_wall_heal);
    RegisterSpellScript(spell_defensive_quickness);
    RegisterSpellScript(spell_undying_will);
    RegisterSpellScript(spell_rumbling_earth);
    RegisterSpellScript(spell_blade_turning);
    RegisterSpellScript(spell_block_spike);
    RegisterSpellScript(spell_healing_block);
    RegisterSpellScript(spell_hurricane);
    RegisterSpellScript(rune_reprisal);
    RegisterSpellScript(rune_thunder_bolt);
    RegisterSpellScript(rune_improved_heroic_throw);
    RegisterSpellScript(rune_blood_and_thunder);
    RegisterSpellScript(rune_brutal_vitality);
    RegisterSpellScript(rune_fatality);
    RegisterSpellScript(rune_test_of_might);
    RegisterSpellScript(rune_test_of_might_expire);
    RegisterSpellScript(rune_in_for_the_kill);
    RegisterSpellScript(rune_strength_of_arms);
    RegisterSpellScript(rune_bloodmark);
    RegisterSpellScript(rune_cold_steel_hot_blood);
    RegisterSpellScript(rune_berserkers_torment);
    RegisterSpellScript(rune_slaughtering_strikes);
    RegisterSpellScript(rune_warpaint_and_cruelty);
    RegisterSpellScript(rune_steel_resonance_revenge);
    RegisterSpellScript(rune_steel_resonance_whirlwind);
    RegisterSpellScript(rune_champions_bulwark);
    RegisterSpellScript(rune_critical_precision);
    RegisterSpellScript(rune_masterful_swipe);
    RegisterSpellScript(rune_warriors_wrath);
    RegisterSpellScript(rune_unyielding_wrath);
    RegisterSpellScript(rune_planned_battle);
    RegisterSpellScript(rune_planned_battle_expire);
}
