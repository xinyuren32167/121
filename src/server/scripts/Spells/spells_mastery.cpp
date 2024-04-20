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
#include "Object.h"
enum Masteries
{
    // Warrior
    MASTERY_WARRIOR_CRITICAL_BLOCK = 200005,
    MASTERY_WARRIOR_CRITICAL_BLOCK_BUFF = 200006,
    MASTERY_WARRIOR_PHALANX_DOMINANCE_BUFF = 199999,

    // Mage
    MASTERY_MAGE_ICICLE = 300105,
    MASTERY_MAGE_ICICLE_BUFF = 299999,
    MASTERY_MAGE_ICICLE_LISTENER = 300106,
    MASTERY_MAGE_ICICLE_DAMAGE = 300107,
    MASTERY_MAGE_ICICLE_PERIODIC = 300108,
    MASTERY_MAGE_IGNITE = 300109,
    MASTERY_MAGE_IGNITE_DOTS = 300110,
    MASTERY_MAGE_BATTLE_KNOWLEDGE = 300114,
    MASTERY_MAGE_BATTLE_KNOWLEDGE_BUFF = 300116,

    // Paladin
    MASTERY_PALADIN_DIVINE_TRIBUNAL_BUFF = 399997,
    MASTERY_PALADIN_DIVINE_TRIBUNAL = 399999,
    MASTERY_PALADIN_LIGHTBRINGER_HEAL = 400001,
    MASTERY_PALADIN_DIVINE_BULWARK = 400002,
    MASTERY_PALADIN_DIVINE_BULWARK_CONSEC_BUFF = 400003,
    MASTERY_PALADIN_DIVINE_BULWARK_APPLIER = 400004,
    MASTERY_PALADIN_DIVINE_BULWARK_BUFF = 400005,
    MASTERY_PALADIN_HAND_OF_LIGHT = 400006,
    MASTERY_PALADIN_HAND_OF_LIGHT_BUFF = 400007,

    // Hunter
    MASTERY_HUNTER_FROM_THE_SHADOWS = 499996,
    MASTERY_HUNTER_FROM_THE_SHADOWS_BUFF = 499998,
    MASTERY_HUNTER_FROM_THE_SHADOWS_INVIS_BUFF = 499995,
    MASTERY_HUNTER_FROM_THE_SHADOWS_INVIS_STACK = 499999,
    MASTERY_HUNTER_MASTER_OF_BEASTS_BUFF = 500000,
    MASTERY_HUNTER_MASTER_OF_BEASTS = 500003,
    MASTERY_HUNTER_SNIPER_TRAINING_BUFF = 500001,
    MASTERY_HUNTER_SNIPER_TRAINING = 500004,
    MASTERY_HUNTER_SPIRIT_BOND = 500005,
    MASTERY_HUNTER_SPIRIT_BOND_BUFF = 500008,
    MASTERY_HUNTER_SPIRIT_BOND_HEAL = 500009,
    SPELL_HUNTER_INVIS_ACTIVATOR = 85008,

    // Druid
    MASTERY_DRUID_ASTRAL_INVOCATION = 700000,
    MASTERY_DRUID_RAZOR_CLAWS = 700004,
    MASTERY_DRUID_RAZOR_CLAWS_BUFF = 700005,
    MASTERY_DRUID_NATURES_GUARDIAN = 700007,
    MASTERY_DRUID_NATURES_GUARDIAN_BUFF = 700008,
    MASTERY_DRUID_HARMONY_HEAL = 700011,

    // Deathknight
    MASTERY_DK_FROST = 600000,
    MASTERY_DK_FROST_BUFF = 600001,
    MASTERY_DK_UNHOLY_BUFF = 600004,
    MASTERY_DK_UNHOLY = 600005,
    MASTERY_DK_BLOOD = 590001,
    MASTERY_DK_BLOOD_INCREASE_AP = 590002,
    SPELL_DK_BlOOD_SHIELD = 590003,
    MASTERY_DK_LIFE_AND_DEATH = 590005,
    SPELL_DK_LIFE_AND_DEATH_SHIELD = 590006,

    // Priest
    MASTERY_PRIEST_GRACE = 900000,
    MASTERY_PRIEST_GRACE_APPLIER = 900001,
    MASTERY_PRIEST_GRACE_BUFF = 900002,
    MASTERY_PRIEST_ECHO_OF_LIGHT_HOT = 900006,
    MASTERY_PRIEST_ECHO_OF_LIGHT_HEAL = 900007,
    MASTERY_PRIEST_SHADOW_WEAVING = 900008,
    MASTERY_PRIEST_SHADOW_WEAVING_DAMAGE = 900009,
    MASTERY_PRIEST_ABSOLUTIONS_EMBRACE_BUFF = 900011,
    MASTERY_PRIEST_ABSOLUTIONS_EMBRACE_DEBUFF = 900012,

    // Rogue
    MASTERY_ROGUE_POTENT_ASSASSIN = 1100000,
    MASTERY_ROGUE_POTENT_ASSASSIN_BUFF = 1100002,
    MASTERY_ROGUE_MAIN_GAUCHE = 1100003,
    MASTERY_ROGUE_MAIN_GAUCHE_DAMAGE = 1100004,
    MASTERY_ROGUE_EXECUTIONER = 1100005,
    MASTERY_ROGUE_EXECUTIONER_BUFF = 1100007,
    MASTERY_ROGUE_PISTOLERO = 1099999,
    MASTERY_ROGUE_PISTOLERO_BUFF = 1099997,

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
    MASTERY_WARLOCK_FEL_BLOOD = 1100024,
    MASTERY_WARLOCK_FEL_BLOOD_BUFF = 1100026,
};

// -------------------------------------------------------------------------------- Warrior --------------------------------------------------------------------------------


class spell_mastery_deep_wounds : public AuraScript
{
    PrepareAuraScript(spell_mastery_deep_wounds);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (!eventInfo.GetActor() || eventInfo.GetActor()->isDead())
                return;

            if (!eventInfo.GetProcTarget() || eventInfo.GetProcTarget()->isDead())
                return;

            int32 pct = aurEff->GetAmount() + player->GetMastery() / 4.5;
            int32 amount = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), pct);
            eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), 200001, SPELL_AURA_PERIODIC_DAMAGE, amount);
        }
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

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 200422; i < 200428; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                int32 amount = aurEff->GetAmount() + caster->GetMastery();
                int32 buff = 0;

                if (Aura* runeAura = GetRuneAura(caster))
                    AddPct(amount, runeAura->GetEffect(EFFECT_0)->GetAmount());

                if (AuraEffect* talent = caster->GetAuraEffectOfRankedSpell(12317, EFFECT_0))
                    buff = talent->GetAmount();

                caster->CastCustomSpell(caster, 200004, &amount, &buff, nullptr, true, nullptr);
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

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_WARRIOR_CRITICAL_BLOCK))
            {
                float mastery = caster->GetMastery();
                int32 blockAmount = aura->GetEffect(EFFECT_0)->GetAmount() + mastery / 3;
                int32 critBlockChance = aura->GetEffect(EFFECT_1)->GetAmount() + mastery + GetCaster()->GetFloatValue(PLAYER_CRIT_PERCENTAGE);
                int32 powerAmount = aura->GetEffect(EFFECT_2)->GetAmount() + mastery / 1.5;

                caster->CastCustomSpell(caster, MASTERY_WARRIOR_CRITICAL_BLOCK_BUFF, &blockAmount, &critBlockChance, &powerAmount, TRIGGERED_FULL_MASK);
            }
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
        if (GetCaster()->HasAura(MASTERY_WARRIOR_CRITICAL_BLOCK_BUFF))
            GetCaster()->RemoveAura(MASTERY_WARRIOR_CRITICAL_BLOCK_BUFF);
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
                    int32 damageReduction = -aurEff->GetAmount() - mastery / 2;
                    int32 critBonus = GetAura()->GetEffect(EFFECT_1)->GetAmount() + mastery;

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

// 199999 - Phalanx Dominance buff
class spell_mastery_phalanx_dominance_buff : public AuraScript
{
    PrepareAuraScript(spell_mastery_phalanx_dominance_buff);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
            ModStackAmount(-1);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_phalanx_dominance_buff::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// -------------------------------------------------------------------------------- Mage --------------------------------------------------------------------------------


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
        int32 dmgPct = GetDamagePct() + caster->GetMastery() / 1.25;
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
        if (GetCaster()->HasAura(300112))
            GetCaster()->RemoveAura(300112);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_mastery_savant_on_remove::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
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

// 299998 - Icicle buff apply
class spell_mastery_icicle_buff : public SpellScript
{
    PrepareSpellScript(spell_mastery_icicle_buff);

    void HandleCast()
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            int32 amount = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue() + player->GetMastery();
            player->RemoveAura(MASTERY_MAGE_ICICLE_BUFF);
            player->CastCustomSpell(MASTERY_MAGE_ICICLE_BUFF, SPELLVALUE_BASE_POINT0, amount, player, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_icicle_buff::HandleCast);
    }
};

// 300105 - Mastery : Icicle
class spell_mastery_icicle_buff_remove : public AuraScript
{
    PrepareAuraScript(spell_mastery_icicle_buff_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster()->HasAura(MASTERY_MAGE_ICICLE_BUFF))
            GetCaster()->RemoveAura(MASTERY_MAGE_ICICLE_BUFF);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_mastery_icicle_buff_remove::OnRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_icicle_ice_lance : public SpellScript
{
    PrepareSpellScript(spell_icicle_ice_lance);

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(MASTERY_MAGE_ICICLE_LISTENER))
            return;

        Unit* target = GetExplTargetUnit();

        if (!target || target->isDead())
            return;

        caster->CastSpell(target, MASTERY_MAGE_ICICLE_PERIODIC, TRIGGERED_FULL_MASK);

        if (Aura* aura = target->GetAura(MASTERY_MAGE_ICICLE_PERIODIC))
            if (Aura* buffAura = caster->GetAura(MASTERY_MAGE_ICICLE_LISTENER))
            {
                aura->SetDuration(buffAura->GetStackAmount() * 200);
                caster->RemoveAura(buffAura);
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

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(MASTERY_MAGE_ICICLE))
            return;

        Unit* target = GetUnitOwner();

        if (!target || target->isDead())
            return;

        if (Player* player = GetCaster()->ToPlayer())
        {
            int32 procPct = caster->GetAura(MASTERY_MAGE_ICICLE)->GetEffect(EFFECT_0)->GetAmount();
            float pct = player->GetMastery() + procPct;
            int32 amount = CalculatePct(GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST), pct);

            player->CastCustomSpell(MASTERY_MAGE_ICICLE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, true);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_icicle_ice_lance_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_icicle_frostbolt_flurry : public SpellScript
{
    PrepareSpellScript(spell_icicle_frostbolt_flurry);

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(MASTERY_MAGE_ICICLE))
            return;

        Unit* target = GetExplTargetUnit();

        if (!target || target->isDead())
            return;

        if (Player* player = GetCaster()->ToPlayer())
        {
            if (Aura* icicleBuff = player->GetAura(MASTERY_MAGE_ICICLE_LISTENER))
            {
                if (icicleBuff->GetStackAmount() < 5)
                    return;

                int32 procPct = caster->GetAura(MASTERY_MAGE_ICICLE)->GetEffect(EFFECT_0)->GetAmount();
                float pct = player->GetMastery() + procPct;
                int32 amount = CalculatePct(player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST), pct);
                player->CastCustomSpell(MASTERY_MAGE_ICICLE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, true);
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_icicle_frostbolt_flurry::HandleProc);
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


// -------------------------------------------------------------------------------- Paladin --------------------------------------------------------------------------------


class spell_mastery_lightbringer : public AuraScript
{
    PrepareAuraScript(spell_mastery_lightbringer);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                if (!procInfo.GetHealInfo())
                    return;

                if (procInfo.GetHealInfo()->GetSpellInfo()->Id == 400001)
                    return;

                Unit* target = procInfo.GetActionTarget();

                if (!target)
                    return;

                Position targetPos = target->GetPosition();
                float distance = caster->GetDistance(targetPos);
                int32 effectiveness = 0;

                if (distance >= 40)
                    effectiveness = 0;
                if (distance <= 10)
                    effectiveness = 100;
                if (distance > 10)
                    effectiveness = (40 - distance) * 3.33;

                int32 heal = procInfo.GetHealInfo()->GetHeal();
                int32 maxValue = aurEff->GetAmount() + caster->GetMastery();
                int32 healPct = CalculatePct(maxValue, effectiveness);
                int32 amount = CalculatePct(heal, healPct);

                caster->CastCustomSpell(MASTERY_PALADIN_LIGHTBRINGER_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
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
        if (Player* caster = GetCaster()->ToPlayer())
        {
            float mastery = caster->GetMastery();
            if (Aura* aura = caster->GetAura(MASTERY_PALADIN_DIVINE_BULWARK))
            {
                int32 attackPower = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;
                int32 blockRating = aura->GetEffect(EFFECT_2)->GetAmount() + mastery;

                caster->CastCustomSpell(caster, MASTERY_PALADIN_DIVINE_BULWARK_BUFF, &attackPower, &blockRating, nullptr, TRIGGERED_FULL_MASK);
            }
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
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (Aura* aura = caster->GetAura(MASTERY_PALADIN_DIVINE_BULWARK))
            {
                float mastery = caster->GetMastery();
                int32 damageReduction = aura->GetEffect(EFFECT_1)->GetAmount() + mastery / 2.6667;

                caster->CastCustomSpell(MASTERY_PALADIN_DIVINE_BULWARK_CONSEC_BUFF, SPELLVALUE_BASE_POINT0, -damageReduction, GetCaster(), true);
            }
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
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Creature* creature = caster->FindNearestCreature(500502, 30))
        {
            if (Player* player = caster->ToPlayer())
            {
                Position playerPos = GetCaster()->GetPosition();
                float distance = creature->GetDistance(playerPos);

                if (const SpellInfo* info = sSpellMgr->GetSpellInfo(48819))
                {
                    float radius = info->Effects[EFFECT_0].CalcRadius(player);

                    if (distance > radius)
                        player->RemoveAura(MASTERY_PALADIN_DIVINE_BULWARK_CONSEC_BUFF);
                    else
                    {
                        if (Aura* aura = player->GetAura(MASTERY_PALADIN_DIVINE_BULWARK))
                        {
                            int32 damageReduction = aura->GetEffect(EFFECT_1)->GetAmount() + player->GetMastery() / 2.6667;

                            player->CastCustomSpell(MASTERY_PALADIN_DIVINE_BULWARK_CONSEC_BUFF, SPELLVALUE_BASE_POINT0, -damageReduction, player, true);
                        }
                    }
                }
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
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (Aura* aura = caster->GetAura(MASTERY_PALADIN_HAND_OF_LIGHT))
            {
                int32 holyPower = aura->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery();

                caster->CastCustomSpell(MASTERY_PALADIN_HAND_OF_LIGHT_BUFF, SPELLVALUE_BASE_POINT0, holyPower, caster, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_hand_of_light::HandleCast);
    }
};

class spell_mastery_divine_tribunal : public SpellScript
{
    PrepareSpellScript(spell_mastery_divine_tribunal);

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            float mastery = caster->GetMastery();
            int32 bonus1 = sSpellMgr->GetSpellInfo(MASTERY_PALADIN_DIVINE_TRIBUNAL)->GetEffect(EFFECT_0).CalcValue(GetCaster()) + mastery;
            int32 bonus2 = sSpellMgr->GetSpellInfo(MASTERY_PALADIN_DIVINE_TRIBUNAL)->GetEffect(EFFECT_1).CalcValue(GetCaster()) + mastery / 1.875;

            caster->CastCustomSpell(caster, MASTERY_PALADIN_DIVINE_TRIBUNAL_BUFF, &bonus1, &bonus2, &bonus1, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_divine_tribunal::HandleCast);
    }
};


// -------------------------------------------------------------------------------- Hunter --------------------------------------------------------------------------------


class spell_mastery_master_of_beasts : public SpellScript
{
    PrepareSpellScript(spell_mastery_master_of_beasts);

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (Aura* aura = caster->GetAura(MASTERY_HUNTER_MASTER_OF_BEASTS))
            {
                int32 damageBonus = aura->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery();

                caster->CastCustomSpell(MASTERY_HUNTER_MASTER_OF_BEASTS_BUFF, SPELLVALUE_BASE_POINT0, damageBonus, caster, TRIGGERED_FULL_MASK);
            }
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
        if (Player* caster = GetCaster()->ToPlayer())
        {
            float mastery = caster->GetMastery();
            if (Aura* aura = caster->GetAura(MASTERY_HUNTER_SNIPER_TRAINING))
            {
                int32 rangeBonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery / 2.2;
                int32 damageBonus = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;

                caster->CastCustomSpell(caster, MASTERY_HUNTER_SNIPER_TRAINING_BUFF, &rangeBonus, &damageBonus, nullptr, true, nullptr, nullptr);
            }
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
        if (Player* caster = GetCaster()->ToPlayer())
        {
            float mastery = caster->GetMastery();
            if (Aura* aura = caster->GetAura(MASTERY_HUNTER_SPIRIT_BOND))
            {
                int32 amount = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

                caster->CastCustomSpell(MASTERY_HUNTER_SPIRIT_BOND_BUFF, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_spirit_bound::HandleCast);
    }
};

// 500005 - Spirit Bound
class spell_mastery_spirit_bound_heal : public AuraScript
{
    PrepareAuraScript(spell_mastery_spirit_bound_heal);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (player->isDead())
                return;

            Pet* pet = player->GetPet();

            if (!pet)
                return;

            int32 amount = aurEff->GetAmount() + player->GetMastery() / 13;

            player->CastCustomSpell(MASTERY_HUNTER_SPIRIT_BOND_HEAL, SPELLVALUE_BASE_POINT0, amount, player, TRIGGERED_FULL_MASK);

            if (pet->IsAlive())
                player->CastCustomSpell(MASTERY_HUNTER_SPIRIT_BOND_HEAL, SPELLVALUE_BASE_POINT0, amount, pet, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mastery_spirit_bound_heal::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_mastery_from_the_shadows_periodic : public AuraScript
{
    PrepareAuraScript(spell_mastery_from_the_shadows_periodic);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (player->isDead())
                return;

            if (Aura* invisAura = player->GetAura(SPELL_HUNTER_INVIS_ACTIVATOR))
            {
                player->AddAura(MASTERY_HUNTER_FROM_THE_SHADOWS_INVIS_STACK, player);

                if (Aura* stackAura = player->GetAura(MASTERY_HUNTER_FROM_THE_SHADOWS_INVIS_STACK))
                    if (stackAura->GetStackAmount() >= 5)
                    {
                        int32 amount = sSpellMgr->GetSpellInfo(MASTERY_HUNTER_FROM_THE_SHADOWS)->GetEffect(EFFECT_1).CalcValue(GetCaster()) + player->GetMastery();

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
        if (Player* caster = GetCaster()->ToPlayer())
        {
            int32 bonus = sSpellMgr->GetSpellInfo(MASTERY_HUNTER_FROM_THE_SHADOWS)->GetEffect(EFFECT_0).CalcValue(GetCaster()) + caster->GetMastery() / 1.66667;

            caster->CastCustomSpell(MASTERY_HUNTER_FROM_THE_SHADOWS_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
        }
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
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mastery_from_the_shadows_buff_remove::HandlePeriodic, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
    }
};


// -------------------------------------------------------------------------------- DeathKnight --------------------------------------------------------------------------------


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
            if (caster->isDead())
                return;

            int32 bonus = aurEff->GetAmount() + caster->GetMastery();
            int32 damage = procInfo.GetHealInfo()->GetHeal();
            int32 shieldBonus = ApplyPct(damage, bonus);

            if (Aura* shield = caster->GetAura(SPELL_DK_BlOOD_SHIELD)) {
                int32 currentAmount = shield->GetEffect(EFFECT_0)->GetAmount();
                int32 amount = std::min<int32>(shieldBonus + currentAmount, caster->CountPctFromMaxHealth(20));
                shield->GetEffect(EFFECT_0)->ChangeAmount(amount);
            }
            else
            {
                int32 amount = std::min<int32>(shieldBonus, caster->CountPctFromMaxHealth(20));
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
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_DK_BLOOD))
            {
                int32 bonus = aura->GetEffect(EFFECT_1)->GetAmount() + caster->GetMastery() / 2;
                caster->CastCustomSpell(MASTERY_DK_BLOOD_INCREASE_AP, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
            }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_bloodshield_attack_power::HandleCast);
    }
};

class spell_mastery_frozen_hearth : public SpellScript
{
    PrepareSpellScript(spell_mastery_frozen_hearth);

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_DK_FROST))
            {
                float mastery = caster->GetMastery();
                int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

                caster->CastCustomSpell(MASTERY_DK_FROST_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
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
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_DK_UNHOLY))
            {
                float mastery = caster->GetMastery();
                int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

                caster->CastCustomSpell(MASTERY_DK_UNHOLY_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
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
            if (caster->isDead())
                return;

            if (Aura* mastery = caster->GetAura(MASTERY_DK_UNHOLY))
            {
                if (AuraEffect const* aureff = GetCaster()->GetAuraEffectOfRankedSpell(49220, EFFECT_0))
                    amount += aureff->GetAmount();

                if (mastery && caster)
                    amount += mastery->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery();
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

class spell_mastery_life_and_death_shield : public AuraScript
{
    PrepareAuraScript(spell_mastery_life_and_death_shield);


    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 bonus = aurEff->GetAmount() + player->GetMastery();
            int32 damage = procInfo.GetDamageInfo()->GetDamage();
            int32 shieldBonus = ApplyPct(damage, bonus);

            if (Aura* shield = player->GetAura(SPELL_DK_LIFE_AND_DEATH_SHIELD))
            {
                int32 currentAmount = shield->GetEffect(EFFECT_0)->GetAmount();
                int32 amount = std::min<int32>(shieldBonus + currentAmount, player->CountPctFromMaxHealth(30));

                shield->GetEffect(EFFECT_0)->ChangeAmount(amount);
                shield->RefreshDuration();
            }
            else
                player->CastCustomSpell(SPELL_DK_LIFE_AND_DEATH_SHIELD, SPELLVALUE_BASE_POINT0, shieldBonus, player, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_life_and_death_shield::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_life_and_death_shield::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


// -------------------------------------------------------------------------------- Druid --------------------------------------------------------------------------------


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

                    if (!target || target->isDead())
                        return;

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
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* mastery = caster->GetAura(MASTERY_DRUID_RAZOR_CLAWS))
            {
                int32 damageBonus = mastery->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery();

                caster->CastCustomSpell(caster, MASTERY_DRUID_RAZOR_CLAWS_BUFF, &damageBonus, &damageBonus, nullptr, true, nullptr, nullptr);
            }
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
        if (GetCaster()->HasAura(MASTERY_DRUID_RAZOR_CLAWS_BUFF))
            GetCaster()->RemoveAura(MASTERY_DRUID_RAZOR_CLAWS_BUFF);
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
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* mastery = caster->GetAura(MASTERY_DRUID_NATURES_GUARDIAN))
            {
                int32 healthPct = mastery->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery() / 2;
                int32 attackBonus = mastery->GetEffect(EFFECT_2)->GetAmount() + caster->GetMastery();

                caster->CastCustomSpell(caster, MASTERY_DRUID_NATURES_GUARDIAN_BUFF, &healthPct, &healthPct, &attackBonus, true, nullptr, nullptr);
            }
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
        GetCaster()->RemoveAura(MASTERY_DRUID_NATURES_GUARDIAN_BUFF);
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
                int32 healPct = defaultValue + caster->GetMastery();

                if (!procInfo.GetHealInfo())
                    return;

                if (procInfo.GetHealInfo()->GetSpellInfo()->Id == MASTERY_DRUID_HARMONY_HEAL)
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

                caster->CastCustomSpell(MASTERY_DRUID_HARMONY_HEAL, SPELLVALUE_BASE_POINT0, std::max(1, finalAmount), target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_mastery_harmony::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


// -------------------------------------------------------------------------------- Warlock --------------------------------------------------------------------------------


// WARLOCK
class spell_mastery_warlock_potent_afflictions : public SpellScript
{
    PrepareSpellScript(spell_mastery_warlock_potent_afflictions);

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_WARLOCK_POTENT_AFFLICTIONS))
            {
                int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery();

                caster->CastCustomSpell(MASTERY_WARLOCK_POTENT_AFFLICTIONS_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
            }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_warlock_potent_afflictions::HandleCast);
    }
};

class spell_mastery_warlock_master_demonology : public SpellScript
{
    PrepareSpellScript(spell_mastery_warlock_master_demonology);

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_WARLOCK_MASTER_DEMONOLOGY))
            {
                int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery();

                caster->CastCustomSpell(MASTERY_WARLOCK_MASTER_DEMONOLOGY_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
            }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_warlock_master_demonology::HandleCast);
    }
};

// 1100019 - Chaotic Energies
class spell_mastery_warlock_chaotic_energies : public AuraScript
{
    PrepareAuraScript(spell_mastery_warlock_chaotic_energies);

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        isPeriodic = true;
        amplitude = 1 * IN_MILLISECONDS;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 damageIncrease = 8 + player->GetMastery();
            int32 damageReduction = 2 + player->GetMastery() / 4;

            damageIncrease = damageIncrease + urand(1, damageIncrease);
            damageReduction = damageReduction + urand(1, damageReduction);

            GetEffect(EFFECT_0)->ChangeAmount(damageIncrease);
            GetEffect(EFFECT_1)->ChangeAmount(-damageReduction);
        }
    }

    void Register() override
    {
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_mastery_warlock_chaotic_energies::CalcPeriodic, EFFECT_2, SPELL_AURA_ANY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_mastery_warlock_chaotic_energies::HandlePeriodic, EFFECT_2, SPELL_AURA_ANY);
    }
};

class spell_mastery_warlock_fel_blood : public SpellScript
{
    PrepareSpellScript(spell_mastery_warlock_fel_blood);

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_WARLOCK_FEL_BLOOD))
            {
                float mastery = caster->GetMastery();
                int32 armorRating = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;
                int32 attackRating = aura->GetEffect(EFFECT_1)->GetAmount() + mastery / 3;

                caster->CastCustomSpell(caster, MASTERY_WARLOCK_FEL_BLOOD_BUFF, &armorRating, &attackRating, nullptr, TRIGGERED_FULL_MASK);
            }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_warlock_fel_blood::HandleCast);
    }
};


// -------------------------------------------------------------------------------- Priest --------------------------------------------------------------------------------


class spell_mastery_pri_grace : public SpellScript
{
    PrepareSpellScript(spell_mastery_pri_grace);

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_PRIEST_GRACE))
            {
                int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery();

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

                if (!target || target->isDead())
                    return;

                if (Aura* hot = target->GetAura(MASTERY_PRIEST_ECHO_OF_LIGHT_HOT))
                {
                    AuraEffect* hotEff = hot->GetEffect(EFFECT_0);
                    int32 remainingHeal = hotEff->GetAmount() * hotEff->GetRemaningTicks();

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

                if (!target || target->isDead())
                    return;

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

class spell_mastery_pri_absolutions_embrace : public AuraScript
{
    PrepareAuraScript(spell_mastery_pri_absolutions_embrace);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActionTarget() != GetCaster();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                Unit* target = eventInfo.GetActionTarget();

                if (!target || target->isDead())
                    return;

                if (target->IsFriendlyTo(caster))
                {
                    int32 buffAmount = aurEff->GetAmount() + caster->GetMastery();
                    buffAmount *= 18;
                    caster->CastCustomSpell(MASTERY_PRIEST_ABSOLUTIONS_EMBRACE_BUFF, SPELLVALUE_BASE_POINT0, buffAmount, target, TRIGGERED_FULL_MASK);
                }
                else
                {
                    int32 debuffAmount = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + caster->GetMastery() / 2;
                    caster->CastCustomSpell(MASTERY_PRIEST_ABSOLUTIONS_EMBRACE_DEBUFF, SPELLVALUE_BASE_POINT0, debuffAmount, target, TRIGGERED_FULL_MASK);
                }
            }
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_mastery_pri_absolutions_embrace::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_mastery_pri_absolutions_embrace::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


// -------------------------------------------------------------------------------- Shaman --------------------------------------------------------------------------------


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

            if (!target || target->isDead())
                return;

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
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_SHAMAN_ENHANCED_ELEMENTS))
            {
                float mastery = caster->GetMastery();
                // proc chance increase
                int32 bonus1 = aura->GetEffect(EFFECT_0)->GetAmount() + mastery / 8;

                // Storm's Wrath Rune proc chance increase
                if (Aura* runeAura = GetStormsWrathAura(caster))
                    bonus1 += runeAura->GetEffect(EFFECT_0)->GetAmount();

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

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 heal = eventInfo.GetHealInfo()->GetHeal();
            int32 maxPct = aurEff->GetAmount() + player->GetMastery();
            float effectiveness = 0;

            effectiveness = 100 - target->GetHealthPct();

            int32 amount = CalculatePct(maxPct, effectiveness);
            amount = CalculatePct(heal, amount);

            caster->CastCustomSpell(MASTERY_SHAMAN_DEEP_HEALING_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
        }
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
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_SHAMAN_JACK_OF_ALL_MASTER_OF_NONE))
            {
                float mastery = caster->GetMastery();
                int32 parryRating = aura->GetEffect(EFFECT_2)->GetAmount() + mastery / 2;
                int32 attackPower = aura->GetEffect(EFFECT_0)->GetAmount() + mastery;

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
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_SHAMAN_JACK_OF_ALL_MASTER_OF_NONE))
            {
                int32 buffAmount = aura->GetEffect(EFFECT_1)->GetAmount() + caster->GetMastery() / 2.666667;

                caster->CastCustomSpell(MASTERY_SHAMAN_JACK_OF_ALL_MASTER_OF_NONE_DAMAGE_REDUCTION, SPELLVALUE_BASE_POINT0, buffAmount, caster, TRIGGERED_FULL_MASK);
            }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_jack_of_all_master_of_none_proc::HandleCast);
    }
};


// -------------------------------------------------------------------------------- Rogue --------------------------------------------------------------------------------


class spell_mastery_rog_potent_assassin : public SpellScript
{
    PrepareSpellScript(spell_mastery_rog_potent_assassin);

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_ROGUE_POTENT_ASSASSIN))
            {
                int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery();

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
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            if (caster->IsAlive())
            {
                Unit* target = eventInfo.GetDamageInfo()->GetVictim();

                if (!target || target->isDead())
                    return;

                int32 power = caster->GetTotalAttackPowerValue(OFF_ATTACK);
                int32 pct = aurEff->GetAmount() + caster->GetMastery();

                if (Item* item = caster->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                {
                    float   DamageMin = item->GetTemplate()->Damage[0].DamageMin;
                    float   DamageMax = item->GetTemplate()->Damage[0].DamageMax;
                    int32 weaponDamage = (DamageMin + DamageMax) / 2;
                    LOG_ERROR("error", "DamageMin = {}, DamageMax = {}, weaponDamage = {}", DamageMin, DamageMax, weaponDamage);
                    int32 amount = power + weaponDamage;
                    LOG_ERROR("error", "power = {}, weaponDamage = {}, amount = {}", power, weaponDamage, amount);
                    ApplyPct(amount, pct);
                    LOG_ERROR("error", "pct = {}, amount = {}", pct, amount);
                    caster->CastCustomSpell(MASTERY_ROGUE_MAIN_GAUCHE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
                }
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
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_ROGUE_EXECUTIONER))
            {
                int32 bonus = aura->GetEffect(EFFECT_0)->GetAmount() + caster->GetMastery();

                caster->CastCustomSpell(MASTERY_ROGUE_EXECUTIONER_BUFF, SPELLVALUE_BASE_POINT0, bonus, caster, TRIGGERED_FULL_MASK);
            }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_rog_executioner::HandleCast);
    }
};

class spell_mastery_rog_pistolero : public SpellScript
{
    PrepareSpellScript(spell_mastery_rog_pistolero);

    void HandleCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (Aura* aura = caster->GetAura(MASTERY_ROGUE_PISTOLERO))
            {
                float mastery = caster->GetMastery();
                int32 opportunityBonus = aura->GetEffect(EFFECT_0)->GetAmount() + mastery / 1.2;
                int32 damageBonus = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;

                caster->CastCustomSpell(caster, MASTERY_ROGUE_PISTOLERO_BUFF, &opportunityBonus, &damageBonus, nullptr, true, nullptr, nullptr);
            }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_mastery_rog_pistolero::HandleCast);
    }
};






void AddSC_spells_mastery_scripts()
{
    // Warrior
    RegisterSpellScript(spell_mastery_deep_wounds);
    RegisterSpellScript(spell_mastery_unshackled_fury);
    RegisterSpellScript(spell_mastery_critical_block);
    RegisterSpellScript(spell_mastery_critical_block_on_remove);
    RegisterSpellScript(spell_mastery_phalanx_dominance);
    RegisterSpellScript(spell_mastery_phalanx_dominance_buff);

    // Mage
    RegisterSpellScript(spell_mastery_savant);
    RegisterSpellScript(spell_mastery_savant_on_remove);
    RegisterSpellScript(spell_mastery_ignite);
    RegisterSpellScript(spell_mastery_icicle_buff);
    RegisterSpellScript(spell_mastery_icicle_buff_remove);
    RegisterSpellScript(spell_icicle_ice_lance);
    RegisterSpellScript(spell_icicle_frostbolt_flurry);
    RegisterSpellScript(spell_icicle_ice_lance_aura);
    RegisterSpellScript(spell_mastery_battle_knowledge);

    // Paladin
    RegisterSpellScript(spell_mastery_lightbringer);
    RegisterSpellScript(spell_mastery_divine_bulwark);
    RegisterSpellScript(spell_mastery_divine_bulwark_consec);
    RegisterSpellScript(spell_mastery_divine_bulwark_selection);
    RegisterSpellScript(spell_mastery_hand_of_light);
    RegisterSpellScript(spell_mastery_divine_tribunal);

    // Hunter
    RegisterSpellScript(spell_mastery_master_of_beasts);
    RegisterSpellScript(spell_mastery_sniper_training);
    RegisterSpellScript(spell_mastery_spirit_bound);
    RegisterSpellScript(spell_mastery_spirit_bound_heal);
    RegisterSpellScript(spell_mastery_from_the_shadows);
    RegisterSpellScript(spell_mastery_from_the_shadows_periodic);
    RegisterSpellScript(spell_mastery_from_the_shadows_buff_remove);

    // DeathKnight
    RegisterSpellScript(spell_mastery_bloodshield);
    RegisterSpellScript(spell_mastery_bloodshield_attack_power);
    RegisterSpellScript(spell_mastery_frozen_hearth);
    RegisterSpellScript(spell_mastery_dreadblade);
    RegisterSpellScript(spell_dk_pet_scaling_damage);
    RegisterSpellScript(spell_mastery_life_and_death_shield);

    // Druid
    RegisterSpellScript(spell_mastery_astral_invocation);
    RegisterSpellScript(spell_mastery_razor_claws);
    RegisterSpellScript(spell_mastery_razor_claws_unlearn);
    RegisterSpellScript(spell_mastery_natures_guardian);
    RegisterSpellScript(spell_mastery_natures_guardian_unlearn);
    RegisterSpellScript(spell_mastery_harmony);

    // Warlock
    RegisterSpellScript(spell_mastery_warlock_potent_afflictions);
    RegisterSpellScript(spell_mastery_warlock_master_demonology);
    RegisterSpellScript(spell_mastery_warlock_chaotic_energies);
    RegisterSpellScript(spell_mastery_warlock_fel_blood);

    // Priest
    RegisterSpellScript(spell_mastery_pri_grace);
    RegisterSpellScript(spell_mastery_pri_echo_of_light);
    RegisterSpellScript(spell_mastery_pri_shadow_weaving);
    RegisterSpellScript(spell_mastery_pri_absolutions_embrace);

    // Shaman
    RegisterSpellScript(spell_mastery_sha_elemental_overload);
    RegisterSpellScript(spell_mastery_sha_chain_lightning_overload);
    RegisterSpellScript(spell_mastery_sha_earthquake_overload);
    RegisterSpellScript(spell_mastery_sha_enhanced_elements);
    RegisterSpellScript(spell_mastery_sha_deep_healing);
    RegisterSpellScript(spell_mastery_jack_of_all_master_of_none);
    RegisterSpellScript(spell_mastery_jack_of_all_master_of_none_proc);

    // Rogue
    RegisterSpellScript(spell_mastery_rog_potent_assassin);
    RegisterSpellScript(spell_mastery_rog_main_gauche);
    RegisterSpellScript(spell_mastery_rog_executioner);
    RegisterSpellScript(spell_mastery_rog_pistolero);

}
