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
#include "Object.h"
enum Masteries
{
    // Warrior
    MASTERY_WARRIOR_PHALANX_DOMINANCE_BUFF = 199999,

    // Hunter
    MASTERY_HUNTER_FROM_THE_SHADOWS = 499996,
    MASTERY_HUNTER_FROM_THE_SHADOWS_BUFF = 499998,
    MASTERY_HUNTER_FROM_THE_SHADOWS_INVIS_BUFF = 499995,
    MASTERY_HUNTER_FROM_THE_SHADOWS_INVIS_STACK = 499999,
    SPELL_HUNTER_INVIS_ACTIVATOR = 85008,

    // Deathknight
    MASTERY_DK_UNHOLY = 600005,
    MASTERY_DK_BLOOD = 590001,
    MASTERY_DK_BLOOD_INCREASE_AP = 590002,
    SPELL_DK_BlOOD_SHIELD = 590003,

    // Priest
    MASTERY_PRIEST_GRACE = 900000,
    MASTERY_PRIEST_GRACE_APPLIER = 900001,
    MASTERY_PRIEST_GRACE_BUFF = 900002,
    MASTERY_PRIEST_ECHO_OF_LIGHT_HOT = 900006,
    MASTERY_PRIEST_ECHO_OF_LIGHT_HEAL = 900007,
    MASTERY_PRIEST_SHADOW_WEAVING = 900008,
    MASTERY_PRIEST_SHADOW_WEAVING_DAMAGE = 900009,

    // Rogue
    MASTERY_ROGUE_POTENT_ASSASSIN = 1100000,
    MASTERY_ROGUE_POTENT_ASSASSIN_BUFF = 1100002,
    MASTERY_ROGUE_MAIN_GAUCHE = 1100003,
    MASTERY_ROGUE_MAIN_GAUCHE_DAMAGE = 1100004,
    MASTERY_ROGUE_EXECUTIONER = 1100005,
    MASTERY_ROGUE_EXECUTIONER_BUFF = 1100007,

    // Shaman
    MASTERY_SHAMAN_ELEMENTAL_OVERLOAD = 1000000,
    MASTERY_SHAMAN_ELEMENTAL_OVERLOAD_BUFF = 1000001,
    MASTERY_SHAMAN_ENHANCED_ELEMENTS = 1000002,
    MASTERY_SHAMAN_ENHANCED_ELEMENTS_APPLIER = 1000003,
    MASTERY_SHAMAN_ENHANCED_ELEMENTS_BUFF = 1000004,
    MASTERY_SHAMAN_DEEP_HEALING = 1000005,
    MASTERY_SHAMAN_DEEP_HEALING_HEAL = 1000006,
    MASTERY_SHAMAN_JACK_OF_ALL_MASTER_OF_NONE = 1000007,
    MASTERY_SHAMAN_JACK_OF_ALL_MASTER_OF_NONE_BUFF = 1000010,
    MASTERY_SHAMAN_JACK_OF_ALL_MASTER_OF_NONE_DAMAGE_REDUCTION = 1000008,

    // Warlock
    MASTERY_WARLOCK_POTENT_AFFLICTIONS = 1100018,
    MASTERY_WARLOCK_CHAOTIC_ENERGIES = 1100019,
    MASTERY_WARLOCK_MASTER_DEMONOLOGY = 1100019,
    MASTERY_WARLOCK_POTENT_AFFLICTIONS_BUFF = 1100017,
    MASTERY_WARLOCK_CHAOTIC_ENERGIES_BUFF = 1100016,
    MASTERY_WARLOCK_MASTER_DEMONOLOGY_BUFF = 1100015,

    // Mage
    MASTERY_MAGE_IGNITE = 300109,
    MASTERY_MAGE_IGNITE_DOTS = 300110,
    MASTERY_MAGE_BATTLE_KNOWLEDGE = 300114,
    MASTERY_MAGE_BATTLE_KNOWLEDGE_BUFF = 300116,
};

// Mage
class spell_icicle_ice_lance : public SpellScript
{
    PrepareSpellScript(spell_icicle_ice_lance);

    Aura* GetBuffAura()
    {
        if (GetCaster()->HasAura(300106))
            return GetCaster()->GetAura(300106);

        return nullptr;
    }

    void HandleProc()
    {
        if (GetBuffAura())
        {
            GetCaster()->CastSpell(GetExplTargetUnit(), 300108, TRIGGERED_FULL_MASK);
            if (Aura* aura = GetExplTargetUnit()->GetAura(300108)) {
                if (Aura* buffAura = GetBuffAura()) {
                    aura->SetDuration(buffAura->GetStackAmount() * 200);
                    GetCaster()->RemoveAura(GetBuffAura());
                }
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_icicle_ice_lance::HandleProc);
    }
};

class spell_icicle_ice_lance_aura : public AuraScript
{
    PrepareAuraScript(spell_icicle_ice_lance_aura);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300105))
            return GetCaster()->GetAura(300105);

        return nullptr;
    }

    int GetProcPct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Player* player = GetCaster()->ToPlayer();
        if (player->IsAlive())
        {
            float pct = player->GetMastery() + GetProcPct();
            int32 amount = int32(CalculatePct(GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST), pct));

            player->CastCustomSpell(300107, SPELLVALUE_BASE_POINT0, amount, GetTarget(), true);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_icicle_ice_lance_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_icicle_frostbolt : public SpellScript
{
    PrepareSpellScript(spell_icicle_frostbolt);

    int GetStackIcicle()
    {
        return GetCaster()->GetAura(300106)->GetStackAmount();
    }

    int GetProcSpell()
    {
        return GetCaster()->GetAura(300106)->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    int GetProcPct()
    {
        return GetCaster()->GetAura(300105)->GetEffect(EFFECT_0)->GetAmount();
    }

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();
        if (player->HasAura(300106)) {
            if (GetStackIcicle() == 5) {
                float pct = player->GetMastery() + GetProcPct();
                int32 amount = int32(CalculatePct(player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST), pct));
                player->CastCustomSpell(GetProcSpell(), SPELLVALUE_BASE_POINT0, amount, GetExplTargetUnit(), true);
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_icicle_frostbolt::HandleProc);
    }
};

class spell_mastery_ignite : public AuraScript
{
    PrepareAuraScript(spell_mastery_ignite);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    int GetDamagePct()
    {
        Unit* caster = GetCaster();
        return caster->GetAura(MASTERY_MAGE_IGNITE)->GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(caster);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (player->IsAlive())
            {
                float pct = GetDamagePct() + player->GetMastery();
                uint32 damage = eventInfo.GetDamageInfo()->GetDamage();

                int32 totalTicks = sSpellMgr->AssertSpellInfo(MASTERY_MAGE_IGNITE_DOTS)->GetMaxTicks();
                int32 amount = int32(CalculatePct(damage, pct) / totalTicks);

                if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(300110, 0)) {
                    int32 remainingTicks = protEff->GetRemaningTicks();
                    int32 remainingAmount = (protEff->GetAmount() * remainingTicks) / totalTicks;

                    amount += remainingAmount;
                }
                player->CastCustomSpellTrigger(MASTERY_MAGE_IGNITE_DOTS, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), TRIGGERED_IGNORE_AURA_SCALING);
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_ignite::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_ignite::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_savant : public SpellScript
{
    PrepareSpellScript(spell_mastery_savant);

    int GetManaPct()
    {
        return GetCaster()->GetAura(300111)->GetEffect(EFFECT_1)->GetAmount();
    }

    int GetDamagePct()
    {
        return GetCaster()->GetAura(300111)->GetEffect(EFFECT_2)->GetAmount();
    }

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        int32 manaPct = GetManaPct() + caster->GetMastery();
        int32 dmgPct = GetDamagePct() + caster->GetMastery();
        caster->CastCustomSpell(caster, 300112, &manaPct, &manaPct, &dmgPct, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_savant::HandleCast);
    }
};

class spell_mastery_savant_on_remove : public AuraScript
{
    PrepareAuraScript(spell_mastery_savant_on_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetCaster()->RemoveAura(300112);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_mastery_savant_on_remove::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mastery_battle_knowledge : public SpellScript
{
    PrepareSpellScript(spell_mastery_battle_knowledge);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        int32 bonus = sSpellMgr->GetSpellInfo(MASTERY_MAGE_BATTLE_KNOWLEDGE)->GetEffect(EFFECT_0).CalcValue(GetCaster()) + mastery;

        caster->CastCustomSpell(MASTERY_MAGE_BATTLE_KNOWLEDGE_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_battle_knowledge::HandleCast);
    }
};


// Warrior
class spell_mastery_deep_wounds : public AuraScript
{
    PrepareAuraScript(spell_mastery_deep_wounds);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 amount = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), aurEff->GetAmount());
        eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), 200001, SPELL_AURA_PERIODIC_DAMAGE, amount);
    }

    void HandleBuff(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                float amount = aurEff->GetAmount() + caster->GetMastery();
                caster->CastCustomSpell(200002, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_deep_wounds::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectProc += AuraEffectProcFn(spell_mastery_deep_wounds::HandleBuff, EFFECT_1, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_unshackled_fury : public AuraScript
{
    PrepareAuraScript(spell_mastery_unshackled_fury);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                int32 amount = aurEff->GetAmount() + caster->GetMastery();
                caster->CastCustomSpell(200004, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_unshackled_fury::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_critical_block : public SpellScript
{
    PrepareSpellScript(spell_mastery_critical_block);

    int GetEffectAmount(SpellEffIndex effect)
    {
        return GetCaster()->GetAura(200005)->GetEffect(effect)->GetAmount();
    }

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        int32 blockAmount = GetEffectAmount(EFFECT_0) + caster->GetMastery();
        int32 critBlockChance = GetEffectAmount(EFFECT_1) + caster->GetMastery() + GetCaster()->GetFloatValue(PLAYER_CRIT_PERCENTAGE);
        int32 powerAmount = GetEffectAmount(EFFECT_2) + caster->GetMastery();
        caster->RemoveAura(200006);
        caster->CastCustomSpell(caster, 200006, &blockAmount, &critBlockChance, &powerAmount, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_critical_block::HandleCast);
    }
};

class spell_mastery_critical_block_on_remove : public AuraScript
{
    PrepareAuraScript(spell_mastery_critical_block_on_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetCaster()->RemoveAura(190750);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_mastery_critical_block_on_remove::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mastery_phalanx_dominance : public AuraScript
{
    PrepareAuraScript(spell_mastery_phalanx_dominance);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            if (Player* caster = GetCaster()->ToPlayer())
            {
                if (caster->IsAlive())
                {
                    float mastery = caster->GetMastery();
                    int32 damageReduction = aurEff->GetBase()->GetEffect(EFFECT_0)->GetAmount() + mastery;
                    int32 critBonus = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + mastery;

                    caster->CastCustomSpell(caster, MASTERY_WARRIOR_PHALANX_DOMINANCE_BUFF, &damageReduction, &critBonus, nullptr, TRIGGERED_FULL_MASK);
                }
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_phalanx_dominance::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


//Paladin
class spell_mastery_lightbringer : public AuraScript
{
    PrepareAuraScript(spell_mastery_lightbringer);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                int32 defaultValue = aurEff->GetAmount();
                float mastery = caster->GetMastery();

                if (!procInfo.GetHealInfo())
                    return;
                if (procInfo.GetHealInfo()->GetSpellInfo()->Id == 400001)
                    return;

                int32 healAmount = procInfo.GetHealInfo()->GetHeal();

                int32 effectiveness = 0;
                int32 effectiveValue = defaultValue + mastery;

                Unit* target = procInfo.GetActionTarget();
                if (!target)
                    return;
                Position targetPos = target->GetPosition();
                float distance = GetCaster()->GetDistance(targetPos);

                if (distance >= 40)
                    effectiveness = 0;
                if (distance <= 10)
                    effectiveness = 100;
                if (distance > 10)
                    effectiveness = (40 - distance) * 3.33;

                int32 finalAmount = CalculatePct(effectiveValue, effectiveness);
                int32 finalHeal = ApplyPct(healAmount, finalAmount);

                caster->CastCustomSpell(400001, SPELLVALUE_BASE_POINT0, finalHeal, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_lightbringer::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_divine_bulwark : public SpellScript
{
    PrepareSpellScript(spell_mastery_divine_bulwark);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        if (Aura* aura = caster->GetAura(400002))
        {
            int32 attackPower = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;
            int32 blockRating = aura->GetEffect(EFFECT_2)->GetAmount() + mastery;

            caster->CastCustomSpell(GetCaster(), 400005, &attackPower, &blockRating, nullptr, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_divine_bulwark::HandleCast);
    }
};

class spell_mastery_divine_bulwark_consec : public SpellScript
{
    PrepareSpellScript(spell_mastery_divine_bulwark_consec);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(400002))
        {
            float mastery = caster->GetMastery();
            int32 damageReduction = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;

            caster->CastCustomSpell(400003, SPELLVALUE_BASE_POINT0, damageReduction, GetCaster(), true);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_divine_bulwark_consec::HandleCast);
    }
};

class spell_mastery_divine_bulwark_selection : public AuraScript
{
    PrepareAuraScript(spell_mastery_divine_bulwark_selection);

    void HandleScriptEffect(AuraEffect* aurEff)
    {
        if (Creature* creature = GetCaster()->FindNearestCreature(500502, 30))
        {
            Position player = GetCaster()->GetPosition();
            float distance = creature->GetDistance(player);

            if (const SpellInfo* info = sSpellMgr->GetSpellInfo(48819))
            {
                float radius = info->Effects[EFFECT_0].CalcRadius(GetCaster());

                if (distance > radius)
                    GetCaster()->RemoveAura(400003);
                else
                    GetCaster()->AddAura(400003, GetCaster());
            }
        }
    }

    void Register() override
    {
        OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_mastery_divine_bulwark_selection::HandleScriptEffect, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_mastery_hand_of_light : public SpellScript
{
    PrepareSpellScript(spell_mastery_hand_of_light);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        if (Aura* aura = caster->GetAura(400006))
        {
            int32 holyPower = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

            caster->CastCustomSpell(400007, SPELLVALUE_BASE_POINT0, holyPower, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_hand_of_light::HandleCast);
    }
};


//Hunter
class spell_mastery_master_of_beasts : public SpellScript
{
    PrepareSpellScript(spell_mastery_master_of_beasts);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        if (Aura* aura = caster->GetAura(500003))
        {
            int32 damageBonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

            caster->CastCustomSpell(500000, SPELLVALUE_BASE_POINT0, damageBonus, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_master_of_beasts::HandleCast);
    }
};

class spell_mastery_sniper_training : public SpellScript
{
    PrepareSpellScript(spell_mastery_sniper_training);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        if (Aura* aura = caster->GetAura(500004))
        {
            int32 rangeBonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;
            int32 damageBonus = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;

            caster->CastCustomSpell(caster, 500001, &rangeBonus, &damageBonus, nullptr, true, nullptr, nullptr);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_sniper_training::HandleCast);
    }
};

class spell_mastery_spirit_bound : public SpellScript
{
    PrepareSpellScript(spell_mastery_spirit_bound);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        if (Aura* aura = caster->GetAura(500005))
        {
            int32 amount = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

            caster->CastCustomSpell(500008, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_spirit_bound::HandleCast);
    }
};

class spell_mastery_from_the_shadows_periodic : public AuraScript
{
    PrepareAuraScript(spell_mastery_from_the_shadows_periodic);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Player* player = GetCaster()->ToPlayer();
        if (player->IsAlive())
        {
            if (Aura* invisAura = player->GetAura(SPELL_HUNTER_INVIS_ACTIVATOR))
            {
                player->AddAura(MASTERY_HUNTER_FROM_THE_SHADOWS_INVIS_STACK, player);

                if(Aura* stackAura = player->GetAura(MASTERY_HUNTER_FROM_THE_SHADOWS_INVIS_STACK))
                    if (stackAura->GetStackAmount() >= 5)
                    {
                        float mastery = player->GetMastery();
                        int32 amount = sSpellMgr->GetSpellInfo(MASTERY_HUNTER_FROM_THE_SHADOWS)->GetEffect(EFFECT_1).CalcValue(GetCaster()) + mastery;

                        player->CastCustomSpell(MASTERY_HUNTER_FROM_THE_SHADOWS_INVIS_BUFF, SPELLVALUE_BASE_POINT0, amount, player, true);
                    }
            }
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mastery_from_the_shadows_periodic::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_mastery_from_the_shadows : public SpellScript
{
    PrepareSpellScript(spell_mastery_from_the_shadows);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        int32 bonus = sSpellMgr->GetSpellInfo(MASTERY_HUNTER_FROM_THE_SHADOWS)->GetEffect(EFFECT_0).CalcValue(GetCaster()) + mastery;

        caster->CastCustomSpell(MASTERY_HUNTER_FROM_THE_SHADOWS_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_from_the_shadows::HandleCast);
    }
};

class spell_mastery_from_the_shadows_buff_remove : public AuraScript
{
    PrepareAuraScript(spell_mastery_from_the_shadows_buff_remove);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        if (Aura* stackAura = GetCaster()->GetAura(MASTERY_HUNTER_FROM_THE_SHADOWS_INVIS_STACK))
            stackAura->Remove();
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mastery_from_the_shadows_buff_remove::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
    }
};


//Druid
class spell_mastery_astral_invocation : public AuraScript
{
    PrepareAuraScript(spell_mastery_astral_invocation);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleCast(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                if (Aura* aura = caster->GetAura(700000))
                {
                    float mastery = caster->GetMastery();
                    int32 natureDamageBonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;
                    int32 arcaneDamageBonus = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;
                    Unit* target = procInfo.GetActionTarget();
                    int32 schoolMask = procInfo.GetSpellInfo()->GetSchoolMask();

                    if (schoolMask & SPELL_SCHOOL_MASK_NATURE && target->HasAura(80518))
                    {
                        int32 rawDamage = procInfo.GetDamageInfo()->GetDamage();
                        int32 pctDamage = CalculatePct(rawDamage, natureDamageBonus);
                        caster->CastCustomSpell(700002, SPELLVALUE_BASE_POINT0, std::max(1, pctDamage), target, TRIGGERED_FULL_MASK);
                    }

                    if (schoolMask & SPELL_SCHOOL_MASK_ARCANE && target->HasAura(48463))
                    {
                        int32 rawDamage = procInfo.GetDamageInfo()->GetDamage();
                        int32 pctDamage = CalculatePct(rawDamage, arcaneDamageBonus);
                        caster->CastCustomSpell(700003, SPELLVALUE_BASE_POINT0, std::max(1, pctDamage), target, TRIGGERED_FULL_MASK);
                    }
                }
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_astral_invocation::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_astral_invocation::HandleCast, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_razor_claws : public SpellScript
{
    PrepareSpellScript(spell_mastery_razor_claws);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(700004);
        int32 damageBonus = spell->GetEffect(EFFECT_0).CalcValue(caster) + mastery;

        caster->CastCustomSpell(caster, 700005, &damageBonus, &damageBonus, nullptr, true, nullptr, nullptr);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_razor_claws::HandleCast);
    }
};

class spell_mastery_razor_claws_unlearn : public AuraScript
{
    PrepareAuraScript(spell_mastery_razor_claws_unlearn);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->RemoveAura(700005);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_mastery_razor_claws_unlearn::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mastery_natures_guardian : public SpellScript
{
    PrepareSpellScript(spell_mastery_natures_guardian);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(700007);
        int32 healthPct = spell->GetEffect(EFFECT_0).CalcValue(caster) + mastery;
        int32 healthBonus = CalculatePct(caster->GetMaxHealth(), healthPct);
        int32 attackBonus = spell->GetEffect(EFFECT_2).CalcValue(caster) + mastery;

        caster->CastCustomSpell(caster, 700008, &healthBonus, &healthBonus, &attackBonus, true, nullptr, nullptr);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_natures_guardian::HandleCast);
    }
};

class spell_mastery_natures_guardian_unlearn : public AuraScript
{
    PrepareAuraScript(spell_mastery_natures_guardian_unlearn);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        GetCaster()->RemoveAura(700008);
        GetCaster()->ToPlayer()->UpdateMaxHealth();
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_mastery_natures_guardian_unlearn::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_mastery_harmony : public AuraScript
{
    PrepareAuraScript(spell_mastery_harmony);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                int32 defaultValue = aurEff->GetAmount();
                float mastery = caster->GetMastery();
                int32 healPct = defaultValue + mastery;

                if (!procInfo.GetHealInfo())
                    return;
                if (procInfo.GetHealInfo()->GetSpellInfo()->Id == 700011)
                    return;

                int32 healAmount = procInfo.GetHealInfo()->GetHeal();

                Unit* target = procInfo.GetActionTarget();
                if (!target)
                    return;

                auto targetAuras = target->GetAppliedAuras();
                std::list <Aura*> totalHots;
                for (auto itr = targetAuras.begin(); itr != targetAuras.end(); ++itr)
                {
                    if (Aura* aura = itr->second->GetBase())
                    {
                        if (GetCaster()->GetGUID() != aura->GetCasterGUID())
                            continue;

                        SpellInfo const* auraInfo = aura->GetSpellInfo();

                        if (auraInfo->SpellFamilyFlags[1] & 0x00400000 && auraInfo->SpellFamilyName == SPELLFAMILY_DRUID)
                        {
                            totalHots.push_back(aura);
                        }
                    }
                }

                int32 healMulti = healPct * totalHots.size();

                int32 finalAmount = CalculatePct(healAmount, healMulti);
                if (procInfo.GetHealInfo()->GetSpellInfo()->Id == 50464)
                    finalAmount *= 3;

                caster->CastCustomSpell(700011, SPELLVALUE_BASE_POINT0, std::max(1, finalAmount), target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_harmony::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


// DK
class spell_mastery_frozen_hearth : public SpellScript
{
    PrepareSpellScript(spell_mastery_frozen_hearth);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(600000))
        {
            float mastery = caster->GetMastery();
            int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

            caster->CastCustomSpell(600001, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_frozen_hearth::HandleCast);
    }
};

class spell_mastery_dreadblade : public SpellScript
{
    PrepareSpellScript(spell_mastery_dreadblade);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(MASTERY_DK_UNHOLY))
        {
            float mastery = caster->GetMastery();
            int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

            caster->CastCustomSpell(600004, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_dreadblade::HandleCast);
    }
};

class spell_dk_pet_scaling_damage : public AuraScript
{
    PrepareAuraScript(spell_dk_pet_scaling_damage);

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                if (Aura* mastery = caster->GetAura(MASTERY_DK_UNHOLY))
                {
                    if (AuraEffect const* aureff = GetCaster()->GetAuraEffectOfRankedSpell(49220, EFFECT_0))
                        amount += aureff->GetAmount();

                    if (mastery && caster)
                        amount += mastery->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery();
                }
            }
        }
    }

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        isPeriodic = true;
        amplitude = 1 * IN_MILLISECONDS;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        GetEffect(aurEff->GetEffIndex())->RecalculateAmount();
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling_damage::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_dk_pet_scaling_damage::CalcPeriodic, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_pet_scaling_damage::HandlePeriodic, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
    }
};

class spell_mastery_bloodshield : public AuraScript
{
    PrepareAuraScript(spell_mastery_bloodshield);


    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                float mastery = caster->GetMastery();
                int32 bonus = aurEff->GetAmount() + mastery;
                int32 damage = procInfo.GetHealInfo()->GetHeal();
                int32 shieldBonus = ApplyPct(damage, bonus);

                if (Aura* shield = caster->GetAura(SPELL_DK_BlOOD_SHIELD)) {
                    int32 currentAmount = shield->GetEffect(EFFECT_0)->GetAmount();
                    shield->GetEffect(EFFECT_0)->ChangeAmount(shieldBonus + currentAmount);
                }
                else
                    caster->CastCustomSpell(SPELL_DK_BlOOD_SHIELD, SPELLVALUE_BASE_POINT0, shieldBonus, caster, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_bloodshield::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_bloodshield::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_bloodshield_attack_power : public SpellScript
{
    PrepareSpellScript(spell_mastery_bloodshield_attack_power);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(MASTERY_DK_BLOOD))
        {
            float mastery = caster->GetMastery();
            int32 bonus = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;
            caster->CastCustomSpell(MASTERY_DK_BLOOD_INCREASE_AP, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_bloodshield_attack_power::HandleCast);
    }
};


// Priest
class spell_mastery_pri_grace : public SpellScript
{
    PrepareSpellScript(spell_mastery_pri_grace);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(MASTERY_PRIEST_GRACE))
        {
            float mastery = caster->GetMastery();
            int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

            caster->CastCustomSpell(MASTERY_PRIEST_GRACE_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_pri_grace::HandleCast);
    }
};

class spell_mastery_pri_echo_of_light : public AuraScript
{
    PrepareAuraScript(spell_mastery_pri_echo_of_light);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                Unit* target = eventInfo.GetHealInfo()->GetTarget();

                if (Aura* hot = target->GetAura(MASTERY_PRIEST_ECHO_OF_LIGHT_HOT))
                {
                    AuraEffect* hotEff = hot->GetEffect(EFFECT_0);

                    int32 remainingTicks = hot->GetDuration() / hotEff->GetAmplitude();
                    int32 remainingHeal = hotEff->GetAmount() * remainingTicks;

                    caster->CastCustomSpell(MASTERY_PRIEST_ECHO_OF_LIGHT_HEAL, SPELLVALUE_BASE_POINT0, remainingHeal, target, TRIGGERED_FULL_MASK);
                }

                int32 heal = eventInfo.GetHealInfo()->GetHeal();
                int32 basePct = aurEff->GetAmount();
                int32 pct = basePct + caster->GetMastery();
                int32 maxTicks = sSpellMgr->AssertSpellInfo(MASTERY_PRIEST_ECHO_OF_LIGHT_HOT)->GetMaxTicks();

                ApplyPct(heal, pct);
                int32 amount = heal / maxTicks;

                caster->CastCustomSpell(MASTERY_PRIEST_ECHO_OF_LIGHT_HOT, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_pri_echo_of_light::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_pri_echo_of_light::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_pri_shadow_weaving : public AuraScript
{
    PrepareAuraScript(spell_mastery_pri_shadow_weaving);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                Unit* target = eventInfo.GetDamageInfo()->GetVictim();
                int32 damage = eventInfo.GetDamageInfo()->GetDamage();
                int32 basePct = aurEff->GetAmount();

                int32 pct = basePct + caster->GetMastery();
                int32 amountPct = 0;

                // Voidform Check
                if (caster->HasAura(81044))
                    amountPct = (pct * 3);
                else
                {
                    // Vampiric Touch Check
                    if (target->HasAura(48160))
                        amountPct += pct;

                    // Shadow Word: Pain Check
                    if (target->HasAura(48125))
                        amountPct += pct;

                    // Devouring Plague Check
                    if (target->HasAura(48300))
                        amountPct += pct;
                }

                if (amountPct == 0)
                    return;

                int32 amount = CalculatePct(damage, amountPct);

                caster->CastCustomSpell(MASTERY_PRIEST_SHADOW_WEAVING_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_pri_shadow_weaving::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_pri_shadow_weaving::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


// Rogue
class spell_mastery_rog_potent_assassin : public SpellScript
{
    PrepareSpellScript(spell_mastery_rog_potent_assassin);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(MASTERY_ROGUE_POTENT_ASSASSIN))
        {
            float mastery = caster->GetMastery();
            int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

            caster->CastCustomSpell(MASTERY_ROGUE_POTENT_ASSASSIN_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_rog_potent_assassin::HandleCast);
    }
};

class spell_mastery_rog_main_gauche : public AuraScript
{
    PrepareAuraScript(spell_mastery_rog_main_gauche);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                Unit* target = eventInfo.GetDamageInfo()->GetVictim();
                int32 damage = eventInfo.GetDamageInfo()->GetDamage();
                int32 basePct = aurEff->GetAmount();
                int32 pct = basePct + caster->GetMastery();

                int32 amount = CalculatePct(damage, pct);

                caster->CastCustomSpell(MASTERY_ROGUE_MAIN_GAUCHE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_rog_main_gauche::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_rog_main_gauche::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_rog_executioner : public SpellScript
{
    PrepareSpellScript(spell_mastery_rog_executioner);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(MASTERY_ROGUE_EXECUTIONER))
        {
            float mastery = caster->GetMastery();
            int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

            caster->CastCustomSpell(MASTERY_ROGUE_EXECUTIONER_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_rog_executioner::HandleCast);
    }
};

// Shaman
class spell_mastery_sha_elemental_overload : public AuraScript
{
    PrepareAuraScript(spell_mastery_sha_elemental_overload);

    Aura* GetMountainsWillFallAura(Unit* caster)
    {
        for (size_t i = 1000638; i < 1000644; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetExtendedRageAura(Unit* caster)
    {
        for (size_t i = 1000644; i < 1000650; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActionTarget() && eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (!player || player->isDead())
                return;

            Unit* target = eventInfo.GetActionTarget();
            int32 spellID = eventInfo.GetSpellInfo()->Id;
            int32 procChance = aurEff->GetAmount() + player->GetMastery();

            // Replace Lightning Bolt by it's Overload
            if (spellID == 49238)
                spellID = 84057;

            // Check if spell is Earth Shock
            if (spellID == 49231)
            {
                // if you don't have Mountains Will Fall it can't proc.
                if (!GetMountainsWillFallAura(player))
                    procChance = 0;
            }

            if (!roll_chance_i(procChance))
            {
                // Check for Power of The Maelstrom Buff
                if (!player->HasAura(1000636) || spellID != 84057)
                    return;
                else 
                    player->GetAura(1000636)->ModCharges(-1);
            }

            // damage + maelstrom nerf
            player->AddAura(MASTERY_SHAMAN_ELEMENTAL_OVERLOAD_BUFF, player);
            player->CastSpell(target, spellID, TRIGGERED_FULL_MASK);
            player->RemoveAura(MASTERY_SHAMAN_ELEMENTAL_OVERLOAD_BUFF);

            // Extended Rage Rune
            if (GetExtendedRageAura(player))
            {
                int32 amount = GetExtendedRageAura(player)->GetEffect(EFFECT_0)->GetAmount();

                // check if Elemental Mastery is known and on cooldown
                if (player->HasSpell(16166) && player->HasSpellCooldown(16166))
                    player->ModifySpellCooldown(16166, -amount);
            }                
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_sha_elemental_overload::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_sha_elemental_overload::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 49271 - Chain Lightning / 84021 - Lava Beam
class spell_mastery_sha_chain_lightning_overload : public SpellScript
{
    PrepareSpellScript(spell_mastery_sha_chain_lightning_overload);

    Aura* GetExtendedRageAura(Unit* caster)
    {
        for (size_t i = 1000644; i < 1000650; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (!player || player->isDead())
                return;

            Unit* target = GetExplTargetUnit();

            if (!target || target->isDead())
                return;

            if (SpellInfo const* spellInfo = GetSpellInfo())
            {
                uint32 spellID = spellInfo->Id;

                if (Aura* overloadMastery = player->GetAura(MASTERY_SHAMAN_ELEMENTAL_OVERLOAD))
                {
                    int32 procChance = overloadMastery->GetEffect(EFFECT_0)->GetAmount() + player->GetMastery();

                    // Mod Stormkeeper stacks
                    if (spellID == 49271 || spellID == 84021)
                    {
                        if (Aura* stormkeeper = player->GetAura(84054))
                            stormkeeper->ModStackAmount(-1);
                    }

                    // Swap Chain lighting and Lava Beam to their overload counterpart.
                    if (spellID == 49271)
                        spellID = 84056;
                    else if (spellID == 84021)
                        spellID = 84058;

                    if (!roll_chance_i(procChance))
                        return;

                    player->AddAura(MASTERY_SHAMAN_ELEMENTAL_OVERLOAD_BUFF, player);
                    player->CastSpell(target, spellID, TRIGGERED_FULL_MASK);
                    player->RemoveAura(MASTERY_SHAMAN_ELEMENTAL_OVERLOAD_BUFF);

                    // Extended Rage Rune
                    if (GetExtendedRageAura(player))
                    {
                        int32 amount = GetExtendedRageAura(player)->GetEffect(EFFECT_0)->GetAmount();

                        // check if Elemental Mastery is known and on cooldown
                        if (player->HasSpell(16166) && player->HasSpellCooldown(16166))
                            player->ModifySpellCooldown(16166, -amount);
                    }
                }
            }
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_mastery_sha_chain_lightning_overload::HandleProc);
    }
};

// 84014 - Earthquake
class spell_mastery_sha_earthquake_overload : public SpellScript
{
    PrepareSpellScript(spell_mastery_sha_earthquake_overload);

    Aura* GetMountainsWillFallAura(Unit* caster)
    {
        for (size_t i = 1000638; i < 1000644; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetExtendedRageAura(Unit* caster)
    {
        for (size_t i = 1000644; i < 1000650; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (!player || player->isDead())
                return;

            if (!GetMountainsWillFallAura(player))
                return;

            Position pos = GetExplTargetDest()->GetPosition();

            if (SpellInfo const* spellInfo = GetSpellInfo())
            {
                uint32 spellID = spellInfo->Id;

                if (Aura* overloadMastery = player->GetAura(MASTERY_SHAMAN_ELEMENTAL_OVERLOAD))
                {
                    int32 procChance = overloadMastery->GetEffect(EFFECT_0)->GetAmount() + player->GetMastery();

                    if (!roll_chance_i(procChance))
                        return;

                    player->AddAura(MASTERY_SHAMAN_ELEMENTAL_OVERLOAD_BUFF, player);
                    player->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), spellID, TRIGGERED_FULL_MASK);
                    player->RemoveAura(MASTERY_SHAMAN_ELEMENTAL_OVERLOAD_BUFF);

                    // Extended Rage Rune
                    if (GetExtendedRageAura(player))
                    {
                        int32 amount = GetExtendedRageAura(player)->GetEffect(EFFECT_0)->GetAmount();

                        // check if Elemental Mastery is known and on cooldown
                        if (player->HasSpell(16166) && player->HasSpellCooldown(16166))
                            player->ModifySpellCooldown(16166, -amount);
                    }
                }
            }
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_mastery_sha_earthquake_overload::HandleProc);
    }
};

class spell_mastery_sha_enhanced_elements : public SpellScript
{
    PrepareSpellScript(spell_mastery_sha_enhanced_elements);

    Aura* GetStormsWrathAura(Unit* caster)
    {
        for (size_t i = 1000824; i < 1000830; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(MASTERY_SHAMAN_ENHANCED_ELEMENTS))
        {
            float mastery = caster->GetMastery();
            // proc chance increase
            int32 bonus1 = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

            // Storm's Wrath Rune proc chance increase
            if (GetStormsWrathAura(caster))
                AddPct(bonus1, GetStormsWrathAura(caster)->GetEffect(EFFECT_0)->GetAmount());

            // elemental damage increase
            int32 bonus2 = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;

            caster->CastCustomSpell(caster, MASTERY_SHAMAN_ENHANCED_ELEMENTS_BUFF, &bonus1, &bonus2, 0, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_sha_enhanced_elements::HandleCast);
    }
};

class spell_mastery_sha_deep_healing : public AuraScript
{
    PrepareAuraScript(spell_mastery_sha_deep_healing);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = eventInfo.GetHealInfo()->GetHealer();
        Unit* target = eventInfo.GetHealInfo()->GetTarget();
        int32 heal = eventInfo.GetHealInfo()->GetHeal();
        int32 maxPct = aurEff->GetAmount() + caster->ToPlayer()->GetMastery();
        int32 healthCap = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        float pct = 0;

        if (!target || target->isDead())
            return;

        int32 targetHealthPct = target->GetHealthPct();

        if (targetHealthPct < healthCap)
            pct = 100;
        else
        {
            targetHealthPct -= healthCap;
            pct = 100 - (100 * targetHealthPct / (100 - healthCap));
        }

        int32 amount = CalculatePct(maxPct, pct);
        amount = CalculatePct(heal, amount);

        caster->CastCustomSpell(MASTERY_SHAMAN_DEEP_HEALING_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_sha_deep_healing::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_sha_deep_healing::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_mastery_jack_of_all_master_of_none : public SpellScript
{
    PrepareSpellScript(spell_mastery_jack_of_all_master_of_none);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(MASTERY_SHAMAN_JACK_OF_ALL_MASTER_OF_NONE))
        {
            float mastery = caster->GetMastery();
            int32 attackPower = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;
            int32 parryRating = aura->GetEffect(EFFECT_2)->GetAmount() + mastery;

            caster->CastCustomSpell(caster, MASTERY_SHAMAN_JACK_OF_ALL_MASTER_OF_NONE_BUFF, &parryRating, &attackPower, nullptr, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_jack_of_all_master_of_none::HandleCast);
    }
};

class spell_mastery_jack_of_all_master_of_none_proc : public SpellScript
{
    PrepareSpellScript(spell_mastery_jack_of_all_master_of_none_proc);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Aura* aura = caster->GetAura(MASTERY_SHAMAN_JACK_OF_ALL_MASTER_OF_NONE))
        {
            float mastery = caster->GetMastery();
            int32 buffAmount = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;

            caster->CastCustomSpell(MASTERY_SHAMAN_JACK_OF_ALL_MASTER_OF_NONE_DAMAGE_REDUCTION, SPELLVALUE_BASE_POINT0, buffAmount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_jack_of_all_master_of_none_proc::HandleCast);
    }
};


class spell_mastery_warlock_potent_afflictions : public SpellScript
{
    PrepareSpellScript(spell_mastery_warlock_potent_afflictions);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        int32 bonus = sSpellMgr->GetSpellInfo(MASTERY_WARLOCK_POTENT_AFFLICTIONS)->GetEffect(EFFECT_0).CalcValue(GetCaster()) + mastery;

        caster->CastCustomSpell(MASTERY_WARLOCK_POTENT_AFFLICTIONS_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_warlock_potent_afflictions::HandleCast);
    }
};

class spell_mastery_warlock_chaotic_energies : public SpellScript
{
    PrepareSpellScript(spell_mastery_warlock_chaotic_energies);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        int32 bonus = sSpellMgr->GetSpellInfo(MASTERY_WARLOCK_CHAOTIC_ENERGIES)->GetEffect(EFFECT_0).CalcValue(GetCaster()) + mastery;
        caster->CastCustomSpell(MASTERY_WARLOCK_CHAOTIC_ENERGIES_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_warlock_chaotic_energies::HandleCast);
    }
};

class spell_mastery_warlock_master_demonology : public SpellScript
{
    PrepareSpellScript(spell_mastery_warlock_master_demonology);

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        float mastery = caster->GetMastery();
        int32 bonus = sSpellMgr->GetSpellInfo(MASTERY_WARLOCK_MASTER_DEMONOLOGY)->GetEffect(EFFECT_0).CalcValue(GetCaster()) + mastery;

        caster->CastCustomSpell(MASTERY_WARLOCK_MASTER_DEMONOLOGY_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_warlock_master_demonology::HandleCast);
    }
};

void AddSC_spells_mastery_scripts()
{
    RegisterSpellScript(spell_mastery_warlock_master_demonology);
    RegisterSpellScript(spell_mastery_warlock_chaotic_energies);
    RegisterSpellScript(spell_mastery_warlock_potent_afflictions);
    RegisterSpellScript(spell_icicle_ice_lance);
    RegisterSpellScript(spell_icicle_frostbolt);
    RegisterSpellScript(spell_icicle_ice_lance_aura);
    RegisterSpellScript(spell_mastery_ignite);
    RegisterSpellScript(spell_mastery_savant);
    RegisterSpellScript(spell_mastery_deep_wounds);
    RegisterSpellScript(spell_mastery_unshackled_fury);
    RegisterSpellScript(spell_mastery_critical_block);
    RegisterSpellScript(spell_mastery_savant_on_remove);
    RegisterSpellScript(spell_mastery_critical_block_on_remove);
    RegisterSpellScript(spell_mastery_lightbringer);
    RegisterSpellScript(spell_mastery_divine_bulwark);
    RegisterSpellScript(spell_mastery_divine_bulwark_consec);
    RegisterSpellScript(spell_mastery_divine_bulwark_selection);
    RegisterSpellScript(spell_mastery_hand_of_light);
    RegisterSpellScript(spell_mastery_master_of_beasts);
    RegisterSpellScript(spell_mastery_sniper_training);
    RegisterSpellScript(spell_mastery_spirit_bound);
    RegisterSpellScript(spell_mastery_astral_invocation);
    RegisterSpellScript(spell_mastery_razor_claws);
    RegisterSpellScript(spell_mastery_natures_guardian);
    RegisterSpellScript(spell_mastery_razor_claws_unlearn);
    RegisterSpellScript(spell_mastery_natures_guardian_unlearn);
    RegisterSpellScript(spell_mastery_harmony);
    RegisterSpellScript(spell_mastery_frozen_hearth);
    RegisterSpellScript(spell_dk_pet_scaling_damage);
    RegisterSpellScript(spell_mastery_dreadblade);
    RegisterSpellScript(spell_mastery_bloodshield);
    RegisterSpellScript(spell_mastery_bloodshield_attack_power);
    RegisterSpellScript(spell_mastery_pri_grace);
    RegisterSpellScript(spell_mastery_pri_echo_of_light);
    RegisterSpellScript(spell_mastery_pri_shadow_weaving);
    RegisterSpellScript(spell_mastery_rog_potent_assassin);
    RegisterSpellScript(spell_mastery_rog_main_gauche);
    RegisterSpellScript(spell_mastery_rog_executioner);
    RegisterSpellScript(spell_mastery_sha_elemental_overload);
    RegisterSpellScript(spell_mastery_sha_chain_lightning_overload);
    RegisterSpellScript(spell_mastery_sha_earthquake_overload);
    RegisterSpellScript(spell_mastery_sha_enhanced_elements);
    RegisterSpellScript(spell_mastery_sha_deep_healing);
    RegisterSpellScript(spell_mastery_jack_of_all_master_of_none);
    RegisterSpellScript(spell_mastery_jack_of_all_master_of_none_proc);
    RegisterSpellScript(spell_mastery_phalanx_dominance);
    RegisterSpellScript(spell_mastery_battle_knowledge);
    RegisterSpellScript(spell_mastery_from_the_shadows);
    RegisterSpellScript(spell_mastery_from_the_shadows_periodic);
    RegisterSpellScript(spell_mastery_from_the_shadows_buff_remove);
}
