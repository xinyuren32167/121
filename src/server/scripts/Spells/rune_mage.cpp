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

enum MageSpells
{
    // Spell
    SPELL_MAGE_ARCANE_CHARGE = 81500,
    SPELL_MAGE_ARCANE_CHARGE_BUFF1 = 81501,
    SPELL_MAGE_ARCANE_CHARGE_BUFF2 = 81502,
    SPELL_MAGE_ARCANE_CHARGE_VISUAL = 81503,
    SPELL_MAGE_ARCANE_SURGE_DAMAGE = 81519,
    SPELL_MAGE_ARCANIC_SLASH = 81550,
    SPELL_MAGE_ARCHON = 81551,
    SPELL_MAGE_ARCHON_DAMAGE = 81552,
    SPELL_MAGE_BLACK_HOLE = 81553,
    SPELL_MAGE_BLACK_HOLE_AURA = 81554,
    SPELL_MAGE_BLACK_HOLE_TARGET_SELECT = 81559,
    SPELL_MAGE_BLAZING_BARRIER = 81526,
    SPELL_MAGE_BLAZING_BARRIER_DAMAGE = 81527,
    SPELL_MAGE_BLIZZARD = 42940,
    SPELL_MAGE_BLIZZARD_DAMAGE = 42938,
    SPELL_MAGE_BURNOUT_TRIGGER = 44450,
    SPELL_MAGE_CAUTERIZE = 81528,
    SPELL_MAGE_CAUTERIZE_AURA = 81529,
    SPELL_MAGE_CAUTERIZE_DEBUFF = 81530,
    SPELL_MAGE_COMBUSTION = 11129,
    SPELL_MAGE_COMET_STORM = 81536,
    SPELL_MAGE_COMET_STORM_DAMAGE = 81537,
    SPELL_MAGE_CONE_OF_COLD = 42931,
    SPELL_MAGE_DEATH_BLOSSOM = 81556,
    SPELL_MAGE_DISPLACEMENT_AURA = 81510,
    SPELL_MAGE_FINGERS_OF_FROST_R1 = 44543,
    SPELL_MAGE_FINGERS_OF_FROST_R2 = 44545,
    SPELL_MAGE_FINGERS_OF_FROST_AURA = 44544,
    SPELL_MAGE_FINGERS_OF_FROST_VISUAL = 74396,
    SPELL_MAGE_FIRE_BLAST = 42873,
    SPELL_MAGE_FIREBALL = 42833,
    SPELL_MAGE_FLURRY = 81533,
    SPELL_MAGE_FLURRY_DAMAGE = 81534,
    SPELL_MAGE_FLURRY_OF_SLASHES = 81547,
    SPELL_MAGE_FROSTBOLT = 81504,
    SPELL_MAGE_FROST_NOVA = 42917,
    SPELL_MAGE_FROZEN_ORB = 80013,
    SPELL_MAGE_FROZEN_ORB_DAMAGE = 80012,
    SPELL_MAGE_INVISIBILITY = 66,
    SPELL_MAGE_GREATER_INVISIBILITY = 81511,
    SPELL_MAGE_GREATER_INVISIBILITY_AURA = 81513,
    SPELL_MAGE_ICE_BARRIER = 43039,
    SPELL_MAGE_ICE_BARRIER_SLOW = 43040,
    SPELL_MAGE_ICE_BLOCK = 45438,
    SPELL_MAGE_ICE_LANCE = 42914,
    SPELL_MAGE_IMPROVED_BLIZZARD_CHILLED = 12486,
    SPELL_MAGE_LIVING_BOMB_MAIN = 55360,
    SPELL_MAGE_LIVING_BOMB_MAIN_EXPLOSION = 55362,
    SPELL_MAGE_LIVING_BOMB_SECOND = 55354,
    SPELL_MAGE_LIVING_BOMB_SECOND_EXPLOSION = 55355,
    SPELL_MAGE_MAGIC_BLOSSOM = 81561,
    SPELL_MAGE_MAGIC_STRIKE = 81546,
    SPELL_MAGE_METEOR = 81531,
    SPELL_MAGE_MIRROR_IMAGE_DAMAGE_REDUCTION = 55343,
    SPELL_MAGE_MIRROR_IMAGE_SUMMON_ID = 31216,
    SPELL_MAGE_PHOENIX_FLAMES = 80029,
    SPELL_MAGE_PRISMATIC_BARRIER = 81523,
    SPELL_MAGE_GALVANIZING_BARRIER = 81560,
    SPELL_MAGE_RAY_OF_FROST_FINGERS = 81539,
    SPELL_MAGE_RAY_OF_FROST_BUFF = 81540,
    SPELL_MAGE_SUMMON_WATER_ELEMENTAL = 31687,
    SPELL_MAGE_SUPERNOVA_DAMAGE = 81525,
    SPELL_MAGE_SUPERNOVA_KNOCKUP = 81541,
    SPELL_MAGE_UNSTABLE_ANOMALY = 81564,
    SPELL_MAGE_UNSTABLE_ANOMALY_SHIELD = 81565,
    SPELL_MAGE_UNSTABLE_ANOMALY_KNOCKBACK = 81566,
    SPELL_MAGE_UNSTABLE_ANOMALY_COOLDOWN = 81567,
    SPELL_MAGE_WAVE_OF_FORCE = 81568,
    SPELL_MAGE_WINTERS_CHILL = 81535,
    SPELL_MAGE_ARCANE_MISSILES = 42843,
    SPELL_MAGE_EVOCATION = 12051,

    // Masteries
    MASTERY_MAGE_SAVANT = 300111,
    MASTERY_MAGE_IGNITE = 300109,
    MASTERY_MAGE_IGNITE_DOT = 300110,
    MASTERY_MAGE_ICICLE = 300105,
    MASTERY_MAGE_BATTLE_KNOWLEDGE = 300114,

    // Talents
    SPELL_MAGE_TALENT_IMPROVED_FINGERS_OF_FROST_R1 = 80027,
    SPELL_MAGE_TALENT_IMPROVED_FINGERS_OF_FROST_R2 = 80028,
    SPELL_MAGE_TALENT_RULE_OF_THREES_R1 = 11237,
    SPELL_MAGE_TALENT_RULE_OF_THREES_R2 = 12463,
    SPELL_MAGE_TALENT_RULE_OF_THREES_R3 = 12464,
    SPELL_MAGE_TALENT_RULE_OF_THREES_R4 = 16769,
    SPELL_MAGE_TALENT_RULE_OF_THREES_R5 = 16770,
    SPELL_MAGE_TALENT_RULE_OF_THREES_BUFF = 16771,
    SPELL_MAGE_TALENT_PARRY_WARD_PROC = 81647,
    SPELL_MAGE_TALENT_ARCANIC_BARRIER_PROC = 81659,
    SPELL_MAGE_TALENT_INFUSED_BLADES_PROC = 81668,
    SPELL_MAGE_TALENT_IMPROVED_DEATH_BLOSSOM = 81622,

    SPELL_VISUAL_FROZEN_ORB = 72067,
    SPELL_VISUAL_ARCANE_ORB = 80015,
    SPELL_MAGE_ARCANE_ORB_DAMAGE = 80017,

    // Weapon Enchants
    SPELL_MAGE_ENCHANT_ARCANIZE_PROC = 81571,
    SPELL_MAGE_ENCHANT_CONDUIT_PROC = 81574,
    SPELL_MAGE_ENCHANT_DEFLECTION_PROC = 81577,
    SPELL_MAGE_ENCHANT_IGNIS_PROC = 81582,
    SPELL_MAGE_ENCHANT_IGNIS_PROC_STACKED = 81670,
    SPELL_MAGE_ENCHANT_SNOWBOUND_PROC = 81585,
    SPELL_MAGE_ENCHANT_ARCANIZE = 81570,
    SPELL_MAGE_ENCHANT_FORCE = 81579,
    SPELL_MAGE_ENCHANT_CONDUIT = 81573,
    SPELL_MAGE_ENCHANT_IGNIS = 81581,
    SPELL_MAGE_ENCHANT_DEFLECTION = 81576,
    SPELL_MAGE_ENCHANT_SNOWBOUND = 81584,

    // Passive
    PASSIVE_MAGE_HOT_STREAK = 44448,
    PASSIVE_MAGE_HOT_STREAK_BUFF = 48108,

    // Runes
    RUNE_MAGE_DIVERTED_ENERGY_HEAL = 300142,
    RUNE_MAGE_SUN_KINGS_BLESSING_LISTENER = 300696,
    RUNE_MAGE_SUN_KINGS_BLESSING_BUFF = 301035,
    RUNE_MAGE_SIPHONING_STRIKES_HEAL = 301132,
    RUNE_MAGE_OIL_FIRE_BUFF = 301202,
    RUNE_MAGE_MASTER_OF_FLAME_TARGETFINDER = 301300,
    RUNE_MAGE_MASTER_OF_FLAME_DAMAGE = 301301,
    RUNE_MAGE_FIREFALL_LISTENER = 301314,
    RUNE_MAGE_FIREFALL_BUFF = 301315,
    RUNE_MAGE_SPLINTERING_RAY_DAMAGE = 301408,
    RUNE_MAGE_GLACIAL_ASSAULT_DAMAGE = 301446,
    RUNE_MAGE_COLD_FRONT_LISTENER = 301484,
    RUNE_MAGE_COLD_FRONT_BUFF = 301485,
    RUNE_MAGE_MASTER_OF_ARCANA_DAMAGE = 301540,
    RUNE_MAGE_DUPLICATIVE_INCINERATION_PROC = 301754,
    RUNE_MAGE_ARCANE_MISSILES_SLIPSTREAM = 300924,
    RUNE_MAGE_EVOCATION_SLIPSTREAM = 300925,
};

class spell_tempest_barrier : public SpellScript
{
    PrepareSpellScript(spell_tempest_barrier);

    int ShieldPct()
    {
        auto playerAuras = GetCaster()->GetAppliedAuras();
        int shieldPct = 0;

        for (auto itr = playerAuras.begin(); itr != playerAuras.end(); ++itr)
        {
            if (Aura* aura = itr->second->GetBase())
            {
                SpellInfo const* auraInfo = aura->GetSpellInfo();

                if (auraInfo->SpellFamilyFlags[2] & 0x00002000)
                {
                    shieldPct += aura->GetEffect(EFFECT_0)->GetAmount();
                }
            }
        }

        return shieldPct;
    }

    void HandleProc()
    {
        int playerMaxHealth = GetCaster()->GetMaxHealth();
        int32 absorb = int32(CalculatePct(playerMaxHealth, ShieldPct()));
        GetCaster()->CastCustomSpell(300135, SPELLVALUE_BASE_POINT0, absorb, GetCaster(), true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_tempest_barrier::HandleProc);
    }
};

class spell_cryo_freeze : public AuraScript
{
    PrepareAuraScript(spell_cryo_freeze);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300019))
            return GetCaster()->GetAura(300019);

        if (GetCaster()->HasAura(300020))
            return GetCaster()->GetAura(300020);

        if (GetCaster()->HasAura(300021))
            return GetCaster()->GetAura(300021);

        if (GetCaster()->HasAura(300022))
            return GetCaster()->GetAura(300022);

        if (GetCaster()->HasAura(300023))
            return GetCaster()->GetAura(300023);

        if (GetCaster()->HasAura(300024))
            return GetCaster()->GetAura(300024);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetProcSpell(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_cryo_freeze::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_diverted_energy : public AuraScript
{
    PrepareAuraScript(spell_diverted_energy);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 300025; i < 300031; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        int32 shieldPct = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();
        int32 amount = CalculatePct(absorbAmount, shieldPct);

        GetCaster()->CastCustomSpell(RUNE_MAGE_DIVERTED_ENERGY_HEAL, SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
    }

    void Register() override
    {
        AfterEffectAbsorb += AuraEffectAbsorbFn(spell_diverted_energy::Absorb, EFFECT_0);
    }
};

class spell_spiritual_armor : public AuraScript
{
    PrepareAuraScript(spell_spiritual_armor);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300037))
            return GetCaster()->GetAura(300037);

        if (GetCaster()->HasAura(300038))
            return GetCaster()->GetAura(300038);

        if (GetCaster()->HasAura(300039))
            return GetCaster()->GetAura(300039);

        if (GetCaster()->HasAura(300040))
            return GetCaster()->GetAura(300040);

        if (GetCaster()->HasAura(300041))
            return GetCaster()->GetAura(300041);

        if (GetCaster()->HasAura(300042))
            return GetCaster()->GetAura(300042);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster() || !GetRuneAura())
            return;

        GetCaster()->AddAura(GetProcSpell(), GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster())
            return;

        if (GetCaster()->HasAura(300031))
            GetCaster()->RemoveAura(300031);

        if (GetCaster()->HasAura(300032))
            GetCaster()->RemoveAura(300032);

        if (GetCaster()->HasAura(300033))
            GetCaster()->RemoveAura(300033);

        if (GetCaster()->HasAura(300034))
            GetCaster()->RemoveAura(300034);

        if (GetCaster()->HasAura(300035))
            GetCaster()->RemoveAura(300035);

        if (GetCaster()->HasAura(300036))
            GetCaster()->RemoveAura(300036);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_spiritual_armor::HandleProc, EFFECT_1, SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_spiritual_armor::HandleRemove, EFFECT_1, SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_spiritual_armor_rune : public AuraScript
{
    PrepareAuraScript(spell_spiritual_armor_rune);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster())
            return;

        if (GetCaster()->HasAura(43024) || GetCaster()->HasAura(300150))
            GetCaster()->AddAura(aurEff->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        uint32 buff = aurEff->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        if (GetCaster()->HasAura(buff))
            GetCaster()->RemoveAura(buff);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_spiritual_armor_rune::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_spiritual_armor_rune::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_triune_armor : public SpellScript
{
    PrepareSpellScript(spell_triune_armor);

    bool HasPerkTriuneArmor()
    {
        return (GetCaster()->HasAura(300143) ||
            GetCaster()->HasAura(300144) ||
            GetCaster()->HasAura(300145) ||
            GetCaster()->HasAura(300146) ||
            GetCaster()->HasAura(300147) ||
            GetCaster()->HasAura(300148));
    }
        
    void HandleProc()
    {
        if (GetCaster()->HasSpell(43008) && GetCaster()->HasSpell(43024) && GetCaster()->HasSpell(43046))
            if (HasPerkTriuneArmor())
            {
                GetCaster()->RemoveAura(43024);
                GetCaster()->RemoveAura(43046);
                GetCaster()->RemoveAura(43008);

                GetCaster()->CastSpell(GetCaster(), 300149, TRIGGERED_FULL_MASK);
                GetCaster()->CastSpell(GetCaster(), 300150, TRIGGERED_FULL_MASK);
                GetCaster()->CastSpell(GetCaster(), 300151, TRIGGERED_FULL_MASK);
            }

    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_triune_armor::HandleProc);
    }
};

class spell_triune_armor_rune : public AuraScript
{
    PrepareAuraScript(spell_triune_armor_rune);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster())
            return;

        if (GetCaster()->HasAura(43024))
            GetCaster()->RemoveAura(43024);

        if (GetCaster()->HasAura(43046))
            GetCaster()->RemoveAura(43046);

        if (GetCaster()->HasAura(43008))
            GetCaster()->RemoveAura(43008);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (GetCaster()->HasAura(300149))
            GetCaster()->RemoveAura(300149);

        if (GetCaster()->HasAura(300150))
            GetCaster()->RemoveAura(300150);

        if (GetCaster()->HasAura(300151))
            GetCaster()->RemoveAura(300151);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_triune_armor_rune::HandleProc, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_triune_armor_rune::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_arcanic_gems : public SpellScript
{
    PrepareSpellScript(spell_arcanic_gems);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300043))
            return GetCaster()->GetAura(300043);

        if (GetCaster()->HasAura(300044))
            return GetCaster()->GetAura(300044);

        if (GetCaster()->HasAura(300045))
            return GetCaster()->GetAura(300045);

        if (GetCaster()->HasAura(300046))
            return GetCaster()->GetAura(300046);

        if (GetCaster()->HasAura(300047))
            return GetCaster()->GetAura(300047);

        if (GetCaster()->HasAura(300048))
            return GetCaster()->GetAura(300048);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void HandleProc()
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetProcSpell(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_arcanic_gems::HandleProc);
    }
};

class spell_unstable_magic : public AuraScript
{
    PrepareAuraScript(spell_unstable_magic);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
            return;

        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
        GetCaster()->CastCustomSpell(300062, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_IGNORE_AURA_SCALING);

    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_unstable_magic::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_unstable_magic::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_slick_ice : public AuraScript
{
    PrepareAuraScript(spell_slick_ice);

    bool HandleProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster()->HasAura(12472));
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_slick_ice::HandleProc);
    }
};

class spell_improved_brain_freeze : public AuraScript
{

    PrepareAuraScript(spell_improved_brain_freeze);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster()->HasAura(300164))
            GetCaster()->CastSpell(GetCaster(), 300170, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300165))
            GetCaster()->CastSpell(GetCaster(), 300171, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300166))
            GetCaster()->CastSpell(GetCaster(), 300172, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300167))
            GetCaster()->CastSpell(GetCaster(), 300173, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300168))
            GetCaster()->CastSpell(GetCaster(), 300174, TRIGGERED_IGNORE_CASTER_AURAS);

        if (GetCaster()->HasAura(300169))
            GetCaster()->CastSpell(GetCaster(), 300175, TRIGGERED_IGNORE_CASTER_AURAS);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_improved_brain_freeze::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_improved_brain_freeze_remove : public SpellScript
{

    PrepareSpellScript(spell_improved_brain_freeze_remove);

    void HandleProc()
    {
        if (GetCaster()->HasAura(300170))
            GetCaster()->RemoveAura(300170);

        if (GetCaster()->HasAura(300171))
            GetCaster()->RemoveAura(300171);

        if (GetCaster()->HasAura(300172))
            GetCaster()->RemoveAura(300172);

        if (GetCaster()->HasAura(300173))
            GetCaster()->RemoveAura(300173);

        if (GetCaster()->HasAura(300174))
            GetCaster()->RemoveAura(300174);

        if (GetCaster()->HasAura(300175))
            GetCaster()->RemoveAura(300175);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_improved_brain_freeze_remove::HandleProc);
    }
};

class spell_lady_vashj_s_grasp_remove : public AuraScript
{
    PrepareAuraScript(spell_lady_vashj_s_grasp_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster()->HasAura(300212))
            GetCaster()->RemoveAura(300212);

        if (GetCaster()->HasAura(300213))
            GetCaster()->RemoveAura(300213);

        if (GetCaster()->HasAura(300214))
            GetCaster()->RemoveAura(300214);

        if (GetCaster()->HasAura(300215))
            GetCaster()->RemoveAura(300215);

        if (GetCaster()->HasAura(300216))
            GetCaster()->RemoveAura(300216);

        if (GetCaster()->HasAura(300217))
            GetCaster()->RemoveAura(300217);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_lady_vashj_s_grasp_remove::OnRemove, EFFECT_0, SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_forzen_veins : public AuraScript
{
    PrepareAuraScript(spell_forzen_veins);

    bool HandleProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster()->HasAura(12472));
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (!GetCaster()->HasAura(12472))
            return;

        if (Aura* auraEff = GetCaster()->GetAura(12472))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + aurEff->GetAmount(), auraEff->GetMaxDuration() + 10000));

            auraEff->SetDuration(duration);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_forzen_veins::HandleProc);
        OnEffectProc += AuraEffectProcFn(spell_forzen_veins::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_impulsive_veins : public AuraScript
{
    PrepareAuraScript(spell_impulsive_veins);

    bool HandleProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster()->HasAura(12472));
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (GetCaster()->HasAura(12472))
        {
            if (Player* target = GetTarget()->ToPlayer())
                target->ModifySpellCooldown(12472, -aurEff->GetAmount());
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_impulsive_veins::HandleProc);
        OnEffectProc += AuraEffectProcFn(spell_impulsive_veins::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_flurry : public SpellScript
{
    PrepareSpellScript(spell_flurry);

    void HandleProc()
    {
        if (GetCaster()->HasAura(300366))
            GetCaster()->CastSpell(GetCaster(), 300372, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300367))
            GetCaster()->CastSpell(GetCaster(), 300373, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300368))
            GetCaster()->CastSpell(GetCaster(), 300374, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300369))
            GetCaster()->CastSpell(GetCaster(), 300375, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300370))
            GetCaster()->CastSpell(GetCaster(), 300376, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300371))
            GetCaster()->CastSpell(GetCaster(), 300377, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_flurry::HandleProc);
    }
};

class spell_flurry_proc : public SpellScript
{
    PrepareSpellScript(spell_flurry_proc);

    bool HasPerkFlurryProc()
    {
        return (GetCaster()->HasAura(300372) ||
            GetCaster()->HasAura(300373) ||
            GetCaster()->HasAura(300374) ||
            GetCaster()->HasAura(300375) ||
            GetCaster()->HasAura(300376) ||
            GetCaster()->HasAura(300377));
    }

    void HandleProc()
    {
        if (HasPerkFlurryProc())
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
    }

    void HandleHitProc()
    {
        if (GetCaster()->HasAura(300372))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300372);
        }

        if (GetCaster()->HasAura(300373))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300373);
        }

        if (GetCaster()->HasAura(300374))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300374);
        }

        if (GetCaster()->HasAura(300375))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300375);
        }

        if (GetCaster()->HasAura(300376))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300376);
        }

        if (GetCaster()->HasAura(300377))
        {
            GetCaster()->CastSpell(GetCaster(), 44544, TRIGGERED_FULL_MASK);
            GetCaster()->RemoveAura(300377);
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_flurry_proc::HandleProc);
        AfterHit += SpellHitFn(spell_flurry_proc::HandleHitProc);
    }
};

class spell_brain_fire : public SpellScript
{
    PrepareSpellScript(spell_brain_fire);

    void HandleProc()
    {
        if (GetCaster()->HasAura(300378))
            GetCaster()->CastSpell(GetCaster(), 300384, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300379))
            GetCaster()->CastSpell(GetCaster(), 300385, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300380))
            GetCaster()->CastSpell(GetCaster(), 300386, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300381))
            GetCaster()->CastSpell(GetCaster(), 300387, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300382))
            GetCaster()->CastSpell(GetCaster(), 300388, TRIGGERED_FULL_MASK);

        if (GetCaster()->HasAura(300383))
            GetCaster()->CastSpell(GetCaster(), 300389, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_brain_fire::HandleProc);
    }
};

class spell_ice_spike : public SpellScript
{
    PrepareSpellScript(spell_ice_spike);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 300402; i < 300408; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();

        if (!caster || caster->isDead() || !GetRuneAura(caster))
            return;

        if (!target || target->isDead())
            return;

        if (!caster->HasAura(57761))
            return;

        int32 procChance = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();
        uint32 random = urand(1, 100);

        if (random > procChance)
            return;

        GetCaster()->CastSpell(target, 300408, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_ice_spike::HandleProc);
    }
};

class spell_powerful_water_elemental : public AuraScript
{
    PrepareAuraScript(spell_powerful_water_elemental);

    Aura* GetRuneAura(Unit* player)
    {
        if (player->HasAura(300415))
            return player->GetAura(300415);

        if (player->HasAura(300416))
            return player->GetAura(300416);

        if (player->HasAura(300417))
            return player->GetAura(300417);

        if (player->HasAura(300418))
            return player->GetAura(300418);

        if (player->HasAura(300419))
            return player->GetAura(300419);

        if (player->HasAura(300420))
            return player->GetAura(300420);

        return nullptr;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* player = GetCaster()->GetCharmerOrOwner();

        if (!player || player->isDead() || !GetRuneAura(player))
            return;

        if (!GetCaster()|| GetCaster()->isDead())
            return;

        float crit = player->GetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + static_cast<uint8>(SPELL_SCHOOL_FROST));
        float haste = player->GetFloatValue(UNIT_MOD_CAST_SPEED);
        float spellPowerPct = GetRuneAura(player)->GetEffect(EFFECT_0)->GetAmount();
        float spellPowerRaw = player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST);
        float spellPowerFinal = CalculatePct(std::max<int32>(0, spellPowerRaw), spellPowerPct);

        GetCaster()->SetFloatValue(UNIT_MOD_CAST_SPEED, haste);
        GetCaster()->CastCustomSpell(300422, SPELLVALUE_BASE_POINT0, crit - 5, GetCaster(), true);
        GetCaster()->CastCustomSpell(51040, SPELLVALUE_BASE_POINT0, spellPowerFinal, GetCaster(), true);

    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_powerful_water_elemental::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class spell_cone_of_blizzard : public AuraScript
{
    PrepareAuraScript(spell_cone_of_blizzard);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(42931, -1000);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_cone_of_blizzard::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_arcane_prodigy : public AuraScript
{
    PrepareAuraScript(spell_arcane_prodigy);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(12042, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_arcane_prodigy::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_time_lord : public AuraScript
{
    PrepareAuraScript(spell_time_lord);

    bool HandleProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster()->HasAura(12042));
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_time_lord::HandleProc);
    }
};

class spell_arcano_shatter : public AuraScript
{
    PrepareAuraScript(spell_arcano_shatter);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300486))
            return GetCaster()->GetAura(300486);

        if (GetCaster()->HasAura(300487))
            return GetCaster()->GetAura(300487);

        if (GetCaster()->HasAura(300488))
            return GetCaster()->GetAura(300488);

        if (GetCaster()->HasAura(300489))
            return GetCaster()->GetAura(300489);

        if (GetCaster()->HasAura(300490))
            return GetCaster()->GetAura(300490);

        if (GetCaster()->HasAura(300491))
            return GetCaster()->GetAura(300491);

        return nullptr;
    }

    int GetTriggerSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void HandleEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster() || GetCaster()->isDead())
            return;

        if (GetRuneAura())
            GetCaster()->CastSpell(GetCaster(), GetTriggerSpell(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_arcano_shatter::HandleEffectRemove, EFFECT_0, SPELL_AURA_OBS_MOD_POWER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_equipoise : public AuraScript
{
    PrepareAuraScript(spell_equipoise);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 manaThreshold = aurEff->GetAmount();
        int32 damageBuff = GetEffect(EFFECT_1)->GetAmount();
        int32 regenBuff = GetEffect(EFFECT_2)->GetAmount();

        if (caster->GetPowerPct(POWER_MANA) >= manaThreshold)
        {
            caster->RemoveAura(regenBuff);

            if (!caster->HasAura(damageBuff))
                caster->AddAura(damageBuff, caster);
        }
        else
        {
            caster->RemoveAura(damageBuff);

            if (!caster->HasAura(regenBuff))
                caster->AddAura(regenBuff, caster);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_equipoise::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_touch_of_the_magi : public AuraScript
{
    PrepareAuraScript(spell_touch_of_the_magi);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return (!GetTarget()->HasAura(300528));
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        GetCaster()->CastSpell(GetCaster(), 300528, TRIGGERED_FULL_MASK);
        GetCaster()->CastSpell(GetCaster(), 300530, TRIGGERED_FULL_MASK);
        GetCaster()->CastSpell(procInfo.GetActionTarget(), 300531);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_touch_of_the_magi::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_touch_of_the_magi::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_touch_of_the_magi_debuff : public AuraScript
{
    PrepareAuraScript(spell_touch_of_the_magi_debuff);

    void TriggerDamage(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetActionTarget()->HasAura(300531))
            return;

        if (DamageInfo* damageInfo = eventInfo.GetDamageInfo())
        {
            int32 amount = eventInfo.GetDamageInfo()->GetDamage();

            if (AuraEffect* protEff = GetCaster()->GetAuraEffect(300528, 0))
            {
                protEff->SetAmount(protEff->GetAmount() + amount);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_touch_of_the_magi_debuff::TriggerDamage, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_touch_of_the_magi_explosion : public AuraScript
{
    PrepareAuraScript(spell_touch_of_the_magi_explosion);

    Aura* GetRuneAura(Unit* player)
    {
        if (player->HasAura(300522))
            return player->GetAura(300522);

        if (player->HasAura(300523))
            return player->GetAura(300523);

        if (player->HasAura(300524))
            return player->GetAura(300524);

        if (player->HasAura(300525))
            return player->GetAura(300525);

        if (player->HasAura(300526))
            return player->GetAura(300526);

        if (player->HasAura(300527))
            return player->GetAura(300527);

        return nullptr;
    }

    int DamagePct(Unit* player)
    {
        return GetAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        int damage = 0;

        if (AuraEffect* protEff = player->GetAuraEffect(300528, 0))
            damage = protEff->GetAmount();

        int damageFinal = int(CalculatePct(damage, DamagePct(player)));

        GetCaster()->CastCustomSpell(300529, SPELLVALUE_BASE_POINT0, damageFinal, GetTarget(), true);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_touch_of_the_magi_explosion::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_nether_precision : public AuraScript
{
    PrepareAuraScript(spell_nether_precision);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300538))
            return GetCaster()->GetAura(300538);

        if (GetCaster()->HasAura(300539))
            return GetCaster()->GetAura(300539);

        if (GetCaster()->HasAura(300540))
            return GetCaster()->GetAura(300540);

        if (GetCaster()->HasAura(300541))
            return GetCaster()->GetAura(300541);

        if (GetCaster()->HasAura(300542))
            return GetCaster()->GetAura(300542);

        if (GetCaster()->HasAura(300543))
            return GetCaster()->GetAura(300543);

        return nullptr;
    }

    int GetTriggeredSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetRuneAura())
            GetCaster()->AddAura(GetTriggeredSpell(), GetCaster());
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_nether_precision::OnRemove, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_resonance : public SpellScript
{
    PrepareSpellScript(spell_resonance);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300562))
            return GetCaster()->GetAura(300562);

        if (GetCaster()->HasAura(300563))
            return GetCaster()->GetAura(300563);

        if (GetCaster()->HasAura(300564))
            return GetCaster()->GetAura(300564);

        if (GetCaster()->HasAura(300565))
            return GetCaster()->GetAura(300565);

        if (GetCaster()->HasAura(300566))
            return GetCaster()->GetAura(300566);

        if (GetCaster()->HasAura(300567))
            return GetCaster()->GetAura(300567);

        return nullptr;
    }

    void HandleProc()
    {
        if (GetRuneAura())
            GetCaster()->RemoveAura(GetRuneAura());
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_resonance::HandleProc);
    }
};

class spell_light_missile : public SpellScript
{
    PrepareSpellScript(spell_light_missile);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300574))
            return GetCaster()->GetAura(300574);

        if (GetCaster()->HasAura(300575))
            return GetCaster()->GetAura(300575);

        if (GetCaster()->HasAura(300576))
            return GetCaster()->GetAura(300576);

        if (GetCaster()->HasAura(300577))
            return GetCaster()->GetAura(300577);

        if (GetCaster()->HasAura(300578))
            return GetCaster()->GetAura(300578);

        if (GetCaster()->HasAura(300579))
            return GetCaster()->GetAura(300579);

        return nullptr;
    }

    int GetProcPct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints;
    }

    void HandleProc()
    {
        if (GetRuneAura())
        {
            uint32 random = urand(0, 100);

            if (random < GetProcPct())
                GetCaster()->AddAura(44401, GetCaster());
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_light_missile::HandleProc);
    }
};

class spell_powerful_missile : public SpellScript
{
    PrepareSpellScript(spell_powerful_missile);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300580))
            return GetCaster()->GetAura(300580);

        if (GetCaster()->HasAura(300581))
            return GetCaster()->GetAura(300581);

        if (GetCaster()->HasAura(300582))
            return GetCaster()->GetAura(300582);

        if (GetCaster()->HasAura(300583))
            return GetCaster()->GetAura(300583);

        if (GetCaster()->HasAura(300584))
            return GetCaster()->GetAura(300584);

        if (GetCaster()->HasAura(300585))
            return GetCaster()->GetAura(300585);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void HandleProc()
    {
        if (GetRuneAura())
        {
            if (GetCaster()->HasAura(44401))
            {
                if (!GetCaster()->HasAura(GetProcSpell()))
                    GetCaster()->AddAura(GetProcSpell(), GetCaster());
            }
            else
            {
                if (GetCaster()->HasAura(GetProcSpell()))
                    GetCaster()->RemoveAura(GetProcSpell());
            }
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_powerful_missile::HandleProc);
    }
};

class spell_arcane_knowledge : public AuraScript
{
    PrepareAuraScript(spell_arcane_knowledge);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(12051, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_arcane_knowledge::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_arcanic_unstability : public AuraScript
{
    PrepareAuraScript(spell_arcanic_unstability);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300634))
            return GetCaster()->GetAura(300634);

        if (GetCaster()->HasAura(300635))
            return GetCaster()->GetAura(300635);

        if (GetCaster()->HasAura(300636))
            return GetCaster()->GetAura(300636);

        if (GetCaster()->HasAura(300637))
            return GetCaster()->GetAura(300637);

        if (GetCaster()->HasAura(300638))
            return GetCaster()->GetAura(300638);

        if (GetCaster()->HasAura(300639))
            return GetCaster()->GetAura(300639);

        return nullptr;
    }

    int GetProcPct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0) {

            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetProcPct()));
            GetCaster()->CastCustomSpell(300640, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_arcanic_unstability::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_arcane_bombardment : public SpellScript
{
    PrepareSpellScript(spell_arcane_bombardment);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300641))
            return GetCaster()->GetAura(300641);

        if (GetCaster()->HasAura(300642))
            return GetCaster()->GetAura(300642);

        if (GetCaster()->HasAura(300643))
            return GetCaster()->GetAura(300643);

        if (GetCaster()->HasAura(300644))
            return GetCaster()->GetAura(300644);

        if (GetCaster()->HasAura(300645))
            return GetCaster()->GetAura(300645);

        if (GetCaster()->HasAura(300646))
            return GetCaster()->GetAura(300646);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void HandleProc()
    {
        if (GetExplTargetUnit()->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT) && GetRuneAura())
            GetCaster()->AddAura(GetProcSpell(), GetCaster());
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_arcane_bombardment::HandleProc);
    }
};

class spell_pyroshatter : public AuraScript
{
    PrepareAuraScript(spell_pyroshatter);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300666))
            return GetCaster()->GetAura(300666);

        if (GetCaster()->HasAura(300667))
            return GetCaster()->GetAura(300667);

        if (GetCaster()->HasAura(300668))
            return GetCaster()->GetAura(300668);

        if (GetCaster()->HasAura(300669))
            return GetCaster()->GetAura(300669);

        if (GetCaster()->HasAura(300670))
            return GetCaster()->GetAura(300670);

        if (GetCaster()->HasAura(300671))
            return GetCaster()->GetAura(300671);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetRuneAura())
        {
            uint32 random = urand(1, 100);

            if (random <= 25)
                GetCaster()->AddAura(GetProcSpell(), GetCaster());
        }
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_pyroshatter::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_devastating_blasts : public AuraScript
{
    PrepareAuraScript(spell_devastating_blasts);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 300678; i < 300684; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (GetRuneAura(caster))
        {
            int32 procChance = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            if (roll_chance_i(procChance))
                caster->AddAura(PASSIVE_MAGE_HOT_STREAK_BUFF, caster);
        }
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_devastating_blasts::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sun_kings_blessing : public AuraScript
{
    PrepareAuraScript(spell_sun_kings_blessing);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(RUNE_MAGE_SUN_KINGS_BLESSING_LISTENER))
            caster->RemoveAura(RUNE_MAGE_SUN_KINGS_BLESSING_LISTENER);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_sun_kings_blessing::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_sun_kings_blessing_proc : public SpellScript
{
    PrepareSpellScript(spell_sun_kings_blessing_proc);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 300684; i < 300690; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        int32 maxStack = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();

        if (caster->HasAura(PASSIVE_MAGE_HOT_STREAK_BUFF))
        {
            caster->CastSpell(caster, RUNE_MAGE_SUN_KINGS_BLESSING_LISTENER, TRIGGERED_FULL_MASK);

            if (caster->GetAura(RUNE_MAGE_SUN_KINGS_BLESSING_LISTENER)->GetStackAmount() < maxStack)
                return;
            else
            {
                caster->GetAura(RUNE_MAGE_SUN_KINGS_BLESSING_LISTENER)->SetStackAmount(maxStack);
                caster->AddAura(RUNE_MAGE_SUN_KINGS_BLESSING_BUFF, caster);
                caster->RemoveAura(RUNE_MAGE_SUN_KINGS_BLESSING_LISTENER);
                return;
            }
        }

        int32 buffAura = GetRuneAura(caster)->GetEffect(EFFECT_2)->GetAmount();
        int32 combustionDuration = GetRuneAura(caster)->GetEffect(EFFECT_1)->GetAmount();

        if (caster->HasAura(RUNE_MAGE_SUN_KINGS_BLESSING_BUFF))
        {
            caster->AddAura(buffAura, caster);
            caster->AddAura(SPELL_MAGE_COMBUSTION, caster);
            caster->GetAura(SPELL_MAGE_COMBUSTION)->SetDuration(combustionDuration);

            caster->RemoveAura(RUNE_MAGE_SUN_KINGS_BLESSING_BUFF);
        }
    }

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 300690; i < 300696; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_sun_kings_blessing_proc::HandleProc);
        AfterHit += SpellHitFn(spell_sun_kings_blessing_proc::HandleAfterHit);
    }
};

class spell_desintegration : public AuraScript
{
    PrepareAuraScript(spell_desintegration);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(11129, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_desintegration::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_pre_ignited : public AuraScript
{
    PrepareAuraScript(spell_pre_ignited);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (GetCaster()->HasAura(11129))
        {
            if (AuraEffect* protEff = GetCaster()->GetAuraEffect(11129, EFFECT_0))
            {
                uint32 duration = protEff->GetBase()->GetDuration();
                if (duration < protEff->GetBase()->GetMaxDuration() + 5000)
                    protEff->GetBase()->SetDuration(protEff->GetBase()->GetDuration() + aurEff->GetAmount());
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_pre_ignited::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_hot_knowledge : public AuraScript
{
    PrepareAuraScript(spell_hot_knowledge);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300709))
            return GetCaster()->GetAura(300709);

        if (GetCaster()->HasAura(300710))
            return GetCaster()->GetAura(300710);

        if (GetCaster()->HasAura(300711))
            return GetCaster()->GetAura(300711);

        if (GetCaster()->HasAura(300712))
            return GetCaster()->GetAura(300712);

        if (GetCaster()->HasAura(300713))
            return GetCaster()->GetAura(300713);

        if (GetCaster()->HasAura(300714))
            return GetCaster()->GetAura(300714);

        return nullptr;
    }

    int GetProcPct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetRuneAura())
        {
            uint32 random = urand(0, 100);

            if (random < GetProcPct())
                GetCaster()->AddAura(GetProcSpell(), GetCaster());
        }
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_hot_knowledge::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_fiery_mind : public AuraScript
{
    PrepareAuraScript(spell_fiery_mind);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300716))
            return GetCaster()->GetAura(300716);

        if (GetCaster()->HasAura(300717))
            return GetCaster()->GetAura(300717);

        if (GetCaster()->HasAura(300718))
            return GetCaster()->GetAura(300718);

        if (GetCaster()->HasAura(300719))
            return GetCaster()->GetAura(300719);

        if (GetCaster()->HasAura(300720))
            return GetCaster()->GetAura(300720);

        if (GetCaster()->HasAura(300721))
            return GetCaster()->GetAura(300721);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetRuneAura())
        {
            GetCaster()->AddAura(GetProcSpell(), GetCaster());
        }
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_fiery_mind::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_burning_talons : public AuraScript
{
    PrepareAuraScript(spell_burning_talons);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300728))
            return GetCaster()->GetAura(300728);

        if (GetCaster()->HasAura(300729))
            return GetCaster()->GetAura(300729);

        if (GetCaster()->HasAura(300730))
            return GetCaster()->GetAura(300730);

        if (GetCaster()->HasAura(300731))
            return GetCaster()->GetAura(300731);

        if (GetCaster()->HasAura(300732))
            return GetCaster()->GetAura(300732);

        if (GetCaster()->HasAura(300733))
            return GetCaster()->GetAura(300733);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    int GetDamagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
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

            if (!eventInfo.GetActor() || eventInfo.GetActor()->isDead())
                return;

            eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), GetProcSpell(), SPELL_AURA_PERIODIC_DAMAGE, amount, TRIGGERED_IGNORE_AURA_SCALING);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_burning_talons::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_empowered_fire : public SpellScript
{
    PrepareSpellScript(spell_empowered_fire);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300735))
            return GetCaster()->GetAura(300735);

        if (GetCaster()->HasAura(300736))
            return GetCaster()->GetAura(300736);

        if (GetCaster()->HasAura(300737))
            return GetCaster()->GetAura(300737);

        if (GetCaster()->HasAura(300738))
            return GetCaster()->GetAura(300738);

        if (GetCaster()->HasAura(300739))
            return GetCaster()->GetAura(300739);

        if (GetCaster()->HasAura(300740))
            return GetCaster()->GetAura(300740);

        return nullptr;
    }

    int GetProcPct()
    {
        auto playerAuras = GetCaster()->GetAppliedAuras();
        int procPct = 0;

        for (auto itr = playerAuras.begin(); itr != playerAuras.end(); ++itr)
        {
            if (Aura* aura = itr->second->GetBase())
            {
                SpellInfo const* auraInfo = aura->GetSpellInfo();

                if (auraInfo->SpellFamilyFlags[2] & 0x00080000)
                {
                    procPct += aura->GetEffect(EFFECT_1)->GetAmount();
                }
            }
        }
        return procPct;
    }

    void HandleProc()
    {
        if (GetRuneAura())
        {
            uint32 random = urand(1, 100);

            if (random <= GetProcPct())
                GetCaster()->CastSpell(GetExplTargetUnit(), 42833, TRIGGERED_FULL_MASK);
        }
    }


    void Register() override
    {
        OnHit += SpellHitFn(spell_empowered_fire::HandleProc);
    }
};

class spell_burning_touch : public SpellScript
{
    PrepareSpellScript(spell_burning_touch);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300741))
            return GetCaster()->GetAura(300741);

        if (GetCaster()->HasAura(300742))
            return GetCaster()->GetAura(300742);

        if (GetCaster()->HasAura(300743))
            return GetCaster()->GetAura(300743);

        if (GetCaster()->HasAura(300744))
            return GetCaster()->GetAura(300744);

        if (GetCaster()->HasAura(300745))
            return GetCaster()->GetAura(300745);

        if (GetCaster()->HasAura(300746))
            return GetCaster()->GetAura(300746);

        return nullptr;
    }

    int GetProcValue()
    {
        auto playerAuras = GetCaster()->GetAppliedAuras();
        int procValue = 0;

        for (auto itr = playerAuras.begin(); itr != playerAuras.end(); ++itr)
        {
            if (Aura* aura = itr->second->GetBase())
            {
                SpellInfo const* auraInfo = aura->GetSpellInfo();

                if (auraInfo->SpellFamilyFlags[2] & 0x00100000)
                {
                    procValue += aura->GetEffect(EFFECT_0)->GetAmount();
                }
            }
        }
        return procValue;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void HandleProc()
    {
        if (GetExplTargetUnit()->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT) && GetRuneAura())
            GetCaster()->CastCustomSpell(GetProcSpell(), SPELLVALUE_BASE_POINT0, GetProcValue(), GetCaster(), true);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_burning_touch::HandleProc);
    }
};

class spell_controlled_destruction : public SpellScript
{
    PrepareSpellScript(spell_controlled_destruction);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(300974))
            return GetCaster()->GetAura(300974);

        if (GetCaster()->HasAura(300975))
            return GetCaster()->GetAura(300975);

        if (GetCaster()->HasAura(300976))
            return GetCaster()->GetAura(300976);

        if (GetCaster()->HasAura(300977))
            return GetCaster()->GetAura(300977);

        if (GetCaster()->HasAura(300978))
            return GetCaster()->GetAura(300978);

        if (GetCaster()->HasAura(300979))
            return GetCaster()->GetAura(300979);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void HandleProc()
    {
        if (GetRuneAura())
        {
            if (GetExplTargetUnit()->GetHealthPct() >= 70 || GetExplTargetUnit()->GetHealthPct() <= 30)
                GetCaster()->AddAura(GetProcSpell(), GetCaster());
        }
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_controlled_destruction::HandleProc);
    }
};

class spell_incendiary_eruptions : public AuraScript
{
    PrepareAuraScript(spell_incendiary_eruptions);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (eventInfo.GetDamageInfo()->GetDamage() <= 0)
            return;

        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim || victim->isDead())
            return;

        if (victim->HasAura(SPELL_MAGE_LIVING_BOMB_MAIN))
            return;

        caster->AddAura(SPELL_MAGE_LIVING_BOMB_MAIN, victim);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_incendiary_eruptions::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_incendiary_eruptions::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_fervent_flickering : public AuraScript
{
    PrepareAuraScript(spell_fervent_flickering);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_MAGE_FIRE_BLAST, -cooldown);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_fervent_flickering::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_fervent_flickering::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_accumulative_shielding : public AuraScript
{
    PrepareAuraScript(spell_accumulative_shielding);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 barrierSpell = 0;

        if (caster->HasAura(MASTERY_MAGE_SAVANT))
            barrierSpell = SPELL_MAGE_PRISMATIC_BARRIER;

        if (caster->HasAura(MASTERY_MAGE_IGNITE))
            barrierSpell = SPELL_MAGE_BLAZING_BARRIER;

        if (caster->HasAura(MASTERY_MAGE_ICICLE))
            barrierSpell = SPELL_MAGE_ICE_BARRIER;

        if (caster->HasAura(MASTERY_MAGE_BATTLE_KNOWLEDGE))
            barrierSpell = SPELL_MAGE_GALVANIZING_BARRIER;

        if (barrierSpell == 0)
            return;

        if (!caster->HasAura(barrierSpell))
            return;

        int32 cooldown = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(barrierSpell, -cooldown);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_accumulative_shielding::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_mage_convection : public SpellScript
{
    PrepareSpellScript(rune_mage_convection);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 300796; i < 300802; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        int32 procChance = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();
        int32 cooldown = GetRuneAura(caster)->GetEffect(EFFECT_1)->GetAmount();

        if (!roll_chance_i(procChance))
            return;

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_MAGE_LIVING_BOMB_MAIN, -cooldown);
    }
    void Register() override
    {
        OnCast += SpellCastFn(rune_mage_convection::HandleProc);
    }
};

class rune_mage_siphoning_strikes : public AuraScript
{
    PrepareAuraScript(rune_mage_siphoning_strikes);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!eventInfo.GetSpellInfo())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;
        int32 procSpell;

        if (eventInfo.GetHitMask() == PROC_EX_CRITICAL_HIT)
            procSpell = GetEffect(EFFECT_1)->GetAmount();
        else
            procSpell = aurEff->GetAmount();

        if (spellID == SPELL_MAGE_MAGIC_STRIKE)
            caster->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
        /*else
        {
            int32 amount = eventInfo.GetDamageInfo()->GetDamage();

            caster->CastCustomSpell(RUNE_MAGE_SIPHONING_STRIKES_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        }*/
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_siphoning_strikes::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_siphoning_strikes::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_oil_fire : public SpellScript
{
    PrepareSpellScript(rune_mage_oil_fire);

    Aura* GetOilFireAura(Unit* caster)
    {
        for (size_t i = 301196; i < 301202; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = GetExplTargetUnit();

        if (!target || target->isDead())
            return;

        if (GetOilFireAura(caster))
        {
            int32 healthThreshold = GetOilFireAura(caster)->GetEffect(EFFECT_0)->GetAmount();

            if (target->GetHealthPct() > healthThreshold)
                caster->AddAura(RUNE_MAGE_OIL_FIRE_BUFF, caster);
        }
    }

    void HandleHit()
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(RUNE_MAGE_OIL_FIRE_BUFF))
            caster->RemoveAura(RUNE_MAGE_OIL_FIRE_BUFF);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(rune_mage_oil_fire::HandleCast);
        AfterHit += SpellHitFn(rune_mage_oil_fire::HandleHit);
    }
};

class rune_mage_conflagration : public AuraScript
{
    PrepareAuraScript(rune_mage_conflagration);

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

        int32 conflagDot = aurEff->GetAmount();
        int32 conflagDamage = GetEffect(EFFECT_2)->GetAmount();
        int32 procChance = GetEffect(EFFECT_1)->GetAmount();

        if (target->HasAura(MASTERY_MAGE_IGNITE_DOT) || target->HasAura(conflagDot))
            if (roll_chance_i(procChance))
                caster->CastSpell(target, conflagDamage, TRIGGERED_FULL_MASK);

        caster->CastSpell(target, conflagDot, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_conflagration::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_conflagration::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_fiery_rush : public AuraScript
{
    PrepareAuraScript(rune_mage_fiery_rush);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_MAGE_COMBUSTION))
            return;

        int32 cooldown = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
        {
            player->ModifySpellCooldown(SPELL_MAGE_FIRE_BLAST, -cooldown);
            player->ModifySpellCooldown(SPELL_MAGE_PHOENIX_FLAMES, -cooldown);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_mage_fiery_rush::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_mage_hyperthermia : public AuraScript
{
    PrepareAuraScript(rune_mage_hyperthermia);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(PASSIVE_MAGE_HOT_STREAK_BUFF))
            return;

        if (caster->HasAura(SPELL_MAGE_COMBUSTION))
            return;

        int32 procChance = aurEff->GetAmount();
        int32 procSpell = GetEffect(EFFECT_1)->GetAmount();

        if (!roll_chance_i(procChance))
            return;

        caster->AddAura(procSpell, caster);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_mage_hyperthermia::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_flame_accelerant_proc : public AuraScript
{
    PrepareAuraScript(rune_mage_flame_accelerant_proc);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 301258; i < 301264; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        int32 procSpell = GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
        int32 stackAmount = GetStackAmount();
        int32 maxStack = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();

        if (GetStackAmount() < maxStack)
            return;

        caster->AddAura(procSpell, caster);
        SetStackAmount(1);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_mage_flame_accelerant_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

class rune_mage_alextraszas_fury : public AuraScript
{
    PrepareAuraScript(rune_mage_alextraszas_fury);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (AuraEffect* hotStreak = caster->GetAura(PASSIVE_MAGE_HOT_STREAK)->GetEffect(EFFECT_1))
        {
            int32 chancePct = aurEff->GetAmount();
            float critChance = caster->GetFloatValue(static_cast<uint16>(PLAYER_SPELL_CRIT_PERCENTAGE1) + SPELL_SCHOOL_FIRE);
            int32 procChance = CalculatePct(critChance, chancePct);

            if (roll_chance_i(procChance))
            {
                hotStreak->SetAmount(hotStreak->GetAmount() * 2);

                if (hotStreak->GetAmount() < 100) // not enough
                    return;

                caster->CastSpell(caster, PASSIVE_MAGE_HOT_STREAK_BUFF, TRIGGERED_FULL_MASK);
                hotStreak->SetAmount(25);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_mage_alextraszas_fury::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_phoenix_reborn : public AuraScript
{
    PrepareAuraScript(rune_mage_phoenix_reborn);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_MAGE_PHOENIX_FLAMES, -cooldown);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_phoenix_reborn::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_phoenix_reborn::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_mage_master_of_flame : public AuraScript
{
    PrepareAuraScript(rune_mage_master_of_flame);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 procSpell = eventInfo.GetSpellInfo()->Id;

        if (procSpell == SPELL_MAGE_FIRE_BLAST)
        {
            if (Aura* ignite = target->GetAura(MASTERY_MAGE_IGNITE_DOT))
            {
                int32 amount = ignite->GetEffect(EFFECT_0)->GetAmount();

                GetEffect(EFFECT_2)->SetAmount(amount);
                caster->CastSpell(target, RUNE_MAGE_MASTER_OF_FLAME_TARGETFINDER, TRIGGERED_FULL_MASK);
            }
        }

        if (procSpell == MASTERY_MAGE_IGNITE_DOT && !caster->HasAura(SPELL_MAGE_COMBUSTION))
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            int32 damagePct = GetEffect(EFFECT_1)->GetAmount();
            int32 amount = CalculatePct(damage, damagePct);

            caster->CastCustomSpell(RUNE_MAGE_MASTER_OF_FLAME_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
        }

    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procSpell = aurEff->GetAmount();

        if (caster->HasAura(procSpell))
            caster->RemoveAura(procSpell);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_master_of_flame::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_master_of_flame::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_mage_master_of_flame::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_mage_master_of_flame_targets : public SpellScript
{
    PrepareSpellScript(rune_mage_master_of_flame_targets);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 301294; i < 301300; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!GetRuneAura(caster))
            return;

        uint32 const maxTargets = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();

        if (targets.size() > maxTargets)
        {
            targets.remove_if(Acore::UnitAuraCheck(true, MASTERY_MAGE_IGNITE_DOT));

            if (targets.size() > maxTargets)
                targets.resize(maxTargets);
        }

        for (auto const& object : targets)
        {
            if (object->GetTypeId() == TYPEID_UNIT) {

                if (Unit* target = object->ToUnit()) {

                    if (target->isDead())
                        continue;

                    int32 amount = GetRuneAura(caster)->GetEffect(EFFECT_2)->GetAmount();
                    caster->CastCustomSpell(MASTERY_MAGE_IGNITE_DOT, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
                }
            }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_mage_master_of_flame_targets::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class rune_mage_firefall : public AuraScript
{
    PrepareAuraScript(rune_mage_firefall);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!eventInfo.GetSpellInfo())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 procSpell = eventInfo.GetSpellInfo()->Id;
        int32 maxStack = aurEff->GetAmount();

        if (Aura* listener = caster->GetAura(RUNE_MAGE_FIREFALL_LISTENER))
        {
            listener->ModStackAmount(1);

            if (listener->GetStackAmount() >= maxStack)
            {
                listener->SetStackAmount(maxStack);
                caster->AddAura(RUNE_MAGE_FIREFALL_BUFF, caster);
                listener->Remove();
            }
        }
        else
            caster->AddAura(RUNE_MAGE_FIREFALL_LISTENER, caster);

        if (Player* player = caster->ToPlayer())
        {
            if (procSpell == SPELL_MAGE_FIREBALL && caster->HasAura(RUNE_MAGE_FIREFALL_BUFF))
            {
                caster->CastSpell(target, SPELL_MAGE_METEOR, TRIGGERED_FULL_MASK);
                caster->RemoveAura(RUNE_MAGE_FIREFALL_BUFF);
            }
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(RUNE_MAGE_FIREFALL_LISTENER))
            caster->RemoveAura(RUNE_MAGE_FIREFALL_LISTENER);

        if (caster->HasAura(RUNE_MAGE_FIREFALL_BUFF))
            caster->RemoveAura(RUNE_MAGE_FIREFALL_BUFF);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_firefall::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_firefall::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_mage_firefall::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_mage_from_the_ashes : public AuraScript
{
    PrepareAuraScript(rune_mage_from_the_ashes);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_MAGE_PHOENIX_FLAMES, -cooldown);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_from_the_ashes::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_from_the_ashes::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_lonely_winter : public AuraScript
{
    PrepareAuraScript(rune_mage_lonely_winter);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
            player->learnSpell(aurEff->GetAmount());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
            for (size_t i = 301378; i < 301384; i++)
            {
                if (caster->HasSpell(i))
                    player->removeSpell(i, SPEC_MASK_ALL, false);
            }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_mage_lonely_winter::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_mage_lonely_winter::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_mage_splintering_ray : public AuraScript
{
    PrepareAuraScript(rune_mage_splintering_ray);

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

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 damagePct = aurEff->GetAmount();
        int32 amount = CalculatePct(damage, damagePct);

        caster->CastCustomSpell(RUNE_MAGE_SPLINTERING_RAY_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_splintering_ray::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_splintering_ray::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_glacial_assault : public AuraScript
{
    PrepareAuraScript(rune_mage_glacial_assault);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!eventInfo.GetSpellInfo())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;
        int32 procSpell = aurEff->GetAmount();

        if (spellID == SPELL_MAGE_COMET_STORM_DAMAGE)
            caster->CastSpell(target, procSpell, TRIGGERED_FULL_MASK);
        else if (spellID == SPELL_MAGE_FLURRY_DAMAGE)
        {
            int32 procChance = GetEffect(EFFECT_1)->GetAmount();

            if (roll_chance_i(procChance))
                caster->CastSpell(target, RUNE_MAGE_GLACIAL_ASSAULT_DAMAGE, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_glacial_assault::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_glacial_assault::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_cold_front : public AuraScript
{
    PrepareAuraScript(rune_mage_cold_front);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id != SPELL_MAGE_FLURRY_DAMAGE;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetProcTarget();

        if (!target || target->isDead())
            return;

        int32 procSpell = eventInfo.GetSpellInfo()->Id;
        int32 maxStack = aurEff->GetAmount();

        if (Aura* listener = caster->GetAura(RUNE_MAGE_COLD_FRONT_LISTENER))
        {
            listener->ModStackAmount(1);

            if (listener->GetStackAmount() >= maxStack)
            {
                listener->SetStackAmount(maxStack);
                caster->AddAura(RUNE_MAGE_COLD_FRONT_BUFF, caster);
                listener->Remove();
            }
        }
        else
            caster->AddAura(RUNE_MAGE_COLD_FRONT_LISTENER, caster);

        if (Player* player = caster->ToPlayer())
        {
            if (caster->HasAura(RUNE_MAGE_COLD_FRONT_BUFF))
            {
                caster->CastSpell(target, SPELL_MAGE_FROZEN_ORB, TRIGGERED_FULL_MASK);
                caster->RemoveAura(RUNE_MAGE_COLD_FRONT_BUFF);
            }
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(RUNE_MAGE_COLD_FRONT_LISTENER))
            caster->RemoveAura(RUNE_MAGE_COLD_FRONT_LISTENER);

        if (caster->HasAura(RUNE_MAGE_COLD_FRONT_BUFF))
            caster->RemoveAura(RUNE_MAGE_COLD_FRONT_BUFF);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_cold_front::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_cold_front::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_mage_cold_front::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_mage_master_of_arcana : public AuraScript
{
    PrepareAuraScript(rune_mage_master_of_arcana);

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

        if (!caster->HasAura(SPELL_MAGE_ARCHON))
            return;

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, aurEff->GetAmount());

        caster->CastCustomSpell(RUNE_MAGE_MASTER_OF_ARCANA_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_master_of_arcana::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_master_of_arcana::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_spellblade_adept : public AuraScript
{
    PrepareAuraScript(rune_mage_spellblade_adept);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;

        if (spellID != SPELL_MAGE_MAGIC_STRIKE && spellID != SPELL_MAGE_ARCANIC_SLASH)
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_MAGE_ARCHON, -cooldown);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_spellblade_adept::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_spellblade_adept::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_pulsating_power : public AuraScript
{
    PrepareAuraScript(rune_mage_pulsating_power);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_MAGE_ARCHON))
            return;

        caster->CastSpell(caster, SPELL_MAGE_WAVE_OF_FORCE, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_mage_pulsating_power::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_mage_lethal_bloom : public AuraScript
{
    PrepareAuraScript(rune_mage_lethal_bloom);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_MAGE_FLURRY_OF_SLASHES, -cooldown);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_lethal_bloom::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_lethal_bloom::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_arcane_momentum : public AuraScript
{
    PrepareAuraScript(rune_mage_arcane_momentum);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_MAGE_ARCHON, -cooldown);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_arcane_momentum::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_arcane_momentum::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_arcanic_precision : public AuraScript
{
    PrepareAuraScript(rune_mage_arcanic_precision);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_MAGE_BLACK_HOLE, -cooldown);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_arcanic_precision::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_arcanic_precision::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_more_slashes : public AuraScript
{
    PrepareAuraScript(rune_mage_more_slashes);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 cooldown = aurEff->GetAmount();
            player->ModifySpellCooldown(SPELL_MAGE_FLURRY_OF_SLASHES, -cooldown);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_more_slashes::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_more_slashes::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_duplicative_incineration : public AuraScript
{
    PrepareAuraScript(rune_mage_duplicative_incineration);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetActionTarget();

        if (eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == RUNE_MAGE_DUPLICATIVE_INCINERATION_PROC)
            return false;

        if (!target || target->isDead())
            return false;

        return (caster && caster->IsAlive());
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->ToPlayer()->HasSpellCooldown(RUNE_MAGE_DUPLICATIVE_INCINERATION_PROC))
        {
            GetCaster()->CastSpell(eventInfo.GetActionTarget(), RUNE_MAGE_DUPLICATIVE_INCINERATION_PROC, TRIGGERED_IGNORE_AURA_SCALING);
            GetCaster()->ToPlayer()->AddSpellCooldown(RUNE_MAGE_DUPLICATIVE_INCINERATION_PROC, 0, aurEff->GetAmount());
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_mage_duplicative_incineration::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_mage_duplicative_incineration::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_mage_slipstream : public AuraScript
{
    PrepareAuraScript(rune_mage_slipstream);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_MAGE_ARCANE_MISSILES, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_MAGE_EVOCATION, SPEC_MASK_ALL, false);
        target->learnSpell(RUNE_MAGE_ARCANE_MISSILES_SLIPSTREAM);
        target->learnSpell(RUNE_MAGE_EVOCATION_SLIPSTREAM);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(RUNE_MAGE_ARCANE_MISSILES_SLIPSTREAM, SPEC_MASK_ALL, false);
        target->removeSpell(RUNE_MAGE_EVOCATION_SLIPSTREAM, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_MAGE_ARCANE_MISSILES);
        target->learnSpell(SPELL_MAGE_EVOCATION);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_mage_slipstream::HandleLearn, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_mage_slipstream::HandleUnlearn, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_mage_perks_scripts()
{
    RegisterSpellScript(spell_tempest_barrier);
    RegisterSpellScript(spell_cryo_freeze);
    RegisterSpellScript(spell_diverted_energy);
    RegisterSpellScript(spell_spiritual_armor);
    RegisterSpellScript(spell_spiritual_armor_rune);
    RegisterSpellScript(spell_triune_armor);
    RegisterSpellScript(spell_triune_armor_rune);
    RegisterSpellScript(spell_arcanic_gems);
    RegisterSpellScript(spell_unstable_magic);
    RegisterSpellScript(spell_slick_ice);
    RegisterSpellScript(spell_improved_brain_freeze);
    RegisterSpellScript(spell_improved_brain_freeze_remove);
    RegisterSpellScript(spell_lady_vashj_s_grasp_remove);
    RegisterSpellScript(spell_forzen_veins);
    RegisterSpellScript(spell_impulsive_veins);
    RegisterSpellScript(spell_flurry);
    RegisterSpellScript(spell_flurry_proc);
    RegisterSpellScript(spell_brain_fire);
    RegisterSpellScript(spell_ice_spike);
    RegisterSpellScript(spell_powerful_water_elemental);
    RegisterSpellScript(spell_cone_of_blizzard);
    RegisterSpellScript(spell_arcane_prodigy);
    RegisterSpellScript(spell_time_lord);
    RegisterSpellScript(spell_arcano_shatter);
    RegisterSpellScript(spell_equipoise);
    RegisterSpellScript(spell_touch_of_the_magi);
    RegisterSpellScript(spell_touch_of_the_magi_debuff);
    RegisterSpellScript(spell_touch_of_the_magi_explosion);
    RegisterSpellScript(spell_nether_precision);
    RegisterSpellScript(spell_resonance);
    RegisterSpellScript(spell_light_missile);
    RegisterSpellScript(spell_powerful_missile);
    RegisterSpellScript(spell_arcane_knowledge);
    RegisterSpellScript(spell_arcanic_unstability);
    RegisterSpellScript(spell_arcane_bombardment);
    RegisterSpellScript(spell_pyroshatter);
    RegisterSpellScript(spell_devastating_blasts);
    RegisterSpellScript(spell_sun_kings_blessing);
    RegisterSpellScript(spell_sun_kings_blessing_proc);
    RegisterSpellScript(spell_desintegration);
    RegisterSpellScript(spell_pre_ignited);
    RegisterSpellScript(spell_hot_knowledge);
    RegisterSpellScript(spell_fiery_mind);
    RegisterSpellScript(spell_burning_talons);
    RegisterSpellScript(spell_empowered_fire);
    RegisterSpellScript(spell_burning_touch);
    RegisterSpellScript(spell_controlled_destruction);
    RegisterSpellScript(spell_incendiary_eruptions);
    RegisterSpellScript(spell_fervent_flickering);
    RegisterSpellScript(spell_accumulative_shielding);
    RegisterSpellScript(rune_mage_convection);
    RegisterSpellScript(rune_mage_siphoning_strikes);
    RegisterSpellScript(rune_mage_oil_fire);
    RegisterSpellScript(rune_mage_conflagration);
    RegisterSpellScript(rune_mage_fiery_rush);
    RegisterSpellScript(rune_mage_hyperthermia);
    RegisterSpellScript(rune_mage_flame_accelerant_proc);
    RegisterSpellScript(rune_mage_alextraszas_fury);
    RegisterSpellScript(rune_mage_phoenix_reborn);
    RegisterSpellScript(rune_mage_master_of_flame);
    RegisterSpellScript(rune_mage_master_of_flame_targets);
    RegisterSpellScript(rune_mage_firefall);
    RegisterSpellScript(rune_mage_from_the_ashes);
    RegisterSpellScript(rune_mage_lonely_winter);
    RegisterSpellScript(rune_mage_splintering_ray);
    RegisterSpellScript(rune_mage_glacial_assault);
    RegisterSpellScript(rune_mage_cold_front);
    RegisterSpellScript(rune_mage_master_of_arcana);
    RegisterSpellScript(rune_mage_spellblade_adept);
    RegisterSpellScript(rune_mage_pulsating_power);
    RegisterSpellScript(rune_mage_lethal_bloom);
    RegisterSpellScript(rune_mage_arcane_momentum);
    RegisterSpellScript(rune_mage_arcanic_precision);
    RegisterSpellScript(rune_mage_more_slashes);
    RegisterSpellScript(rune_mage_duplicative_incineration);
    RegisterSpellScript(rune_mage_slipstream);
}
