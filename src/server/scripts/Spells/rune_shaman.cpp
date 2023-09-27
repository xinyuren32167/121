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

enum ShamanSpells
{
    // Spells
    SPELL_SHAMAN_ANCESTRAL_GUIDANCE = 84010,
    SPELL_SHAMAN_ANCESTRAL_PROTECTION_TOTEM = 84079,
    SPELL_SHAMAN_ASCENDANCE_WATER = 84040,
    SPELL_SHAMAN_ASTRAL_RECALL = 556,
    SPELL_SHAMAN_ASTRAL_SHIFT = 84007,
    SPELL_SHAMAN_BLOODLUST = 2825,
    SPELL_SHAMAN_CAPACITOR_TOTEM = 84069,
    SPELL_SHAMAN_CHAIN_HEAL = 55459,
    SPELL_SHAMAN_CHAIN_LIGHTNING = 49271,
    SPELL_SHAMAN_CLOUDBURST_TOTEM = 84082,
    SPELL_SHAMAN_CRASH_LIGHTNING = 84032,
    SPELL_SHAMAN_DOOM_WINDS = 84029,
    SPELL_SHAMAN_DOWNPOUR = 84046,
    SPELL_SHAMAN_EARTH_ELEMENTAL = 2062,
    SPELL_SHAMAN_EARTH_SHOCK = 49231,
    SPELL_SHAMAN_EARTHBIND_TOTEM = 2484,
    SPELL_SHAMAN_EARTHQUAKE = 84014,
    SPELL_SHAMAN_EARTHQUAKE_DAMAGE = 84015,
    SPELL_SHAMAN_ELEMENTAL_BLAST = 84022,
    SPELL_SHAMAN_ELEMENTAL_MASTERY = 16166,
    SPELL_SHAMAN_FERAL_SPIRIT = 51533,
    SPELL_SHAMAN_FIRE_ELEMENTAL = 2894,
    SPELL_SHAMAN_FLAME_SHOCK = 49233,
    SPELL_SHAMAN_FOCUS_FOE = 84116,
    SPELL_SHAMAN_FURY_OF_THE_ELEMENTS = 84106,
    SPELL_SHAMAN_GHOST_WOLF = 2645,
    SPELL_SHAMAN_GROUNDING_TOTEM = 8177,
    SPELL_SHAMAN_GUST_OF_WIND = 84061,
    SPELL_SHAMAN_HEALING_RAIN = 84037,
    SPELL_SHAMAN_HEALING_SURGE = 84004,
    SPELL_SHAMAN_HEALING_TIDE_TOTEM = 84076,
    SPELL_SHAMAN_HEALING_WAVE = 49273,
    SPELL_SHAMAN_HEX = 51514,
    SPELL_SHAMAN_LAVA_BURST = 60043,
    SPELL_SHAMAN_LIGHTNING_BOLT = 49238,
    SPELL_SHAMAN_LIGHTNING_LASSO = 84013,
    SPELL_SHAMAN_LIGHTNING_SHIELD = 49281,
    SPELL_SHAMAN_MANA_TIDE_TOTEM = 16190,
    SPELL_SHAMAN_NATURES_SWIFTNESS = 16188,
    SPELL_SHAMAN_OVERCHARGE = 84128,
    SPELL_SHAMAN_REINCARNATION = 20608,
    SPELL_SHAMAN_RIPTIDE = 61301,
    SPELL_SHAMAN_SPIRIT_LINK_TOTEM = 84073,
    SPELL_SHAMAN_SPIRIT_WALK = 84008,
    SPELL_SHAMAN_STONECLAW_TOTEM = 58582,
    SPELL_SHAMAN_STORM_ELEMENTAL = 84084,
    SPELL_SHAMAN_STORMBRAND_TOTEM = 84244,
    SPELL_SHAMAN_STORMKEEPER = 84018,
    SPELL_SHAMAN_THUNDERSTORM = 59159,
    SPELL_SHAMAN_TIDAL_FORCE = 55198,
    SPELL_SHAMAN_WATER_SHIELD = 57960,
    SPELL_SHAMAN_WATERY_GRAVE = 84125,
    SPELL_SHAMAN_WELLSPRING = 84044,
    SPELL_SHAMAN_WIND_SHEAR = 57994,
    SPELL_SHAMAN_WINDFURY_WEAPON_MAINHAND = 25504,
    SPELL_SHAMAN_WINDFURY_WEAPON_OFFHAND = 33750,

    // Talents
    TALENT_SHAMAN_BLABLA = 00000,

    // Runes
    RUNE_SHAMAN_VOLCANIC_INFERNO_DOT = 1000178,
    RUNE_SHAMAN_RESURGENCE_ENERGIZE = 1000342,
    RUNE_SHAMAN_IMPROVED_EARTHLIVING_WEAPON_HEAL = 1000454,
    RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_FIRE = 1000462,
    RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_FROST = 1000463,
    RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_NATURE = 1000464,
    RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_DEBUFF = 1000465,
    RUNE_SHAMAN_FORCEFUL_WINDS_BUFF = 1000484,
    RUNE_SHAMAN_LIGHTNING_ROD_AOE = 1000528,
    RUNE_SHAMAN_LIGHTNING_ROD_DAMAGE = 1000529,
    RUNE_SHAMAN_AFTERSHOCK_ENERGIZE = 1000622,

    // Summons
    SUMMON_SHAMAN_EARTH_ELEMENTAL = 15352,
    SUMMON_SHAMAN_FIRE_ELEMENTAL = 15438,
    SUMMON_SHAMAN_STORM_ELEMENTAL = 400408,
};

class rune_sha_tidebringer : public AuraScript
{
    PrepareAuraScript(rune_sha_tidebringer);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 procSpell = GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
        int32 maxCharges = aurEff->GetAmount();

        if (Aura* proc = caster->GetAura(procSpell))
        {
            int32 currentCharges = proc->GetCharges();
            proc->Remove();
            caster->AddAura(procSpell, caster);
        }
        else
        {
            caster->AddAura(procSpell, caster);
            caster->GetAura(procSpell)->ModCharges(-1);
        }
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_sha_tidebringer::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_sha_lava_surge : public AuraScript
{
    PrepareAuraScript(rune_sha_lava_surge);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
            player->RemoveSpellCooldown(SPELL_SHAMAN_LAVA_BURST, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_sha_lava_surge::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_sha_fiery_demise : public AuraScript
{
    PrepareAuraScript(rune_sha_fiery_demise);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            player->ModifySpellCooldown(SPELL_SHAMAN_FIRE_ELEMENTAL, -aurEff->GetAmount());
            player->ModifySpellCooldown(SPELL_SHAMAN_STORM_ELEMENTAL, -aurEff->GetAmount());
        }

    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_sha_fiery_demise::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_volcanic_inferno : public AuraScript
{
    PrepareAuraScript(rune_sha_volcanic_inferno);

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

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();

        if (damage == 0)
            return;

        float amount = CalculatePct(damage, aurEff->GetAmount());
        int32 totalTick = sSpellMgr->AssertSpellInfo(RUNE_SHAMAN_VOLCANIC_INFERNO_DOT)->GetMaxTicks();
        amount /= totalTick;

        caster->CastCustomSpell(RUNE_SHAMAN_VOLCANIC_INFERNO_DOT, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_volcanic_inferno::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_volcanic_inferno::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_magma_burst : public AuraScript
{
    PrepareAuraScript(rune_sha_magma_burst);

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

        if (Aura* flameShock = target->GetAura(SPELL_SHAMAN_FLAME_SHOCK))
        {
            int32 newDuration = std::min<int32>(flameShock->GetMaxDuration(), flameShock->GetDuration() + aurEff->GetAmount());
            flameShock->SetDuration(newDuration);
            flameShock->GetEffect(EFFECT_1)->ResetPeriodic();
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_magma_burst::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_magma_burst::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_flash_of_lightning : public AuraScript
{
    PrepareAuraScript(rune_sha_flash_of_lightning);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldownReduction = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
        {
            std::vector<uint32> natureSpells =
            {
                SPELL_SHAMAN_ASTRAL_RECALL, SPELL_SHAMAN_BLOODLUST,SPELL_SHAMAN_CHAIN_LIGHTNING, SPELL_SHAMAN_EARTH_ELEMENTAL, SPELL_SHAMAN_EARTHBIND_TOTEM,
                SPELL_SHAMAN_GROUNDING_TOTEM, SPELL_SHAMAN_HEX, SPELL_SHAMAN_REINCARNATION, SPELL_SHAMAN_STONECLAW_TOTEM, SPELL_SHAMAN_WIND_SHEAR,
                SPELL_SHAMAN_ELEMENTAL_MASTERY, SPELL_SHAMAN_THUNDERSTORM, SPELL_SHAMAN_FERAL_SPIRIT, SPELL_SHAMAN_MANA_TIDE_TOTEM, SPELL_SHAMAN_NATURES_SWIFTNESS,
                SPELL_SHAMAN_TIDAL_FORCE, SPELL_SHAMAN_ANCESTRAL_GUIDANCE, SPELL_SHAMAN_ASTRAL_SHIFT, SPELL_SHAMAN_CAPACITOR_TOTEM, SPELL_SHAMAN_GUST_OF_WIND,
                SPELL_SHAMAN_LIGHTNING_LASSO, SPELL_SHAMAN_SPIRIT_WALK, SPELL_SHAMAN_ELEMENTAL_BLAST, SPELL_SHAMAN_STORM_ELEMENTAL, SPELL_SHAMAN_STORMKEEPER,
                SPELL_SHAMAN_CRASH_LIGHTNING, SPELL_SHAMAN_DOOM_WINDS, SPELL_SHAMAN_ASCENDANCE_WATER, SPELL_SHAMAN_ANCESTRAL_PROTECTION_TOTEM, SPELL_SHAMAN_CLOUDBURST_TOTEM,
                SPELL_SHAMAN_DOWNPOUR, SPELL_SHAMAN_HEALING_RAIN, SPELL_SHAMAN_HEALING_TIDE_TOTEM, SPELL_SHAMAN_SPIRIT_LINK_TOTEM, SPELL_SHAMAN_WELLSPRING,
                SPELL_SHAMAN_FOCUS_FOE, SPELL_SHAMAN_FURY_OF_THE_ELEMENTS, SPELL_SHAMAN_OVERCHARGE, SPELL_SHAMAN_STORMBRAND_TOTEM, SPELL_SHAMAN_WATERY_GRAVE
            };

            for (uint32 spellId : natureSpells)
            {
                if (!player->HasSpell(spellId))
                    continue;

                player->ModifySpellCooldown(spellId, -cooldownReduction);
            }
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_sha_flash_of_lightning::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_pulsating_lightning : public AuraScript
{
    PrepareAuraScript(rune_sha_pulsating_lightning);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_SHAMAN_LIGHTNING_SHIELD))
            return;

        if (!caster->IsInCombat())
            return;

        caster->CastSpell(caster, aurEff->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_sha_pulsating_lightning::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_sha_flowing_streams : public AuraScript
{
    PrepareAuraScript(rune_sha_flowing_streams);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        caster->CastSpell(target, SPELL_SHAMAN_HEALING_SURGE, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_flowing_streams::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_flowing_streams::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_undulation : public AuraScript
{
    PrepareAuraScript(rune_sha_undulation);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetProcSpell();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (healingWaveQte == 3)
            healingWaveQte = 0;

        if (healingSurgeQte == 3)
            healingSurgeQte = 0;

        int32 procSpell = eventInfo.GetProcSpell()->GetSpellInfo()->Id;

        if (procSpell == SPELL_SHAMAN_HEALING_WAVE)
        {
            healingWaveQte++;

            if (healingWaveQte == 2)
                caster->AddAura(GetEffect(EFFECT_1)->GetAmount(), caster);
        }

        if (procSpell == SPELL_SHAMAN_HEALING_SURGE)
        {
            healingSurgeQte++;

            if (healingSurgeQte == 2)
                caster->AddAura(GetEffect(EFFECT_2)->GetAmount(), caster);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_undulation::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_undulation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
private:
    int32 healingWaveQte = 0;
    int32 healingSurgeQte = 0;
};

class rune_sha_resurgence : public AuraScript
{
    PrepareAuraScript(rune_sha_resurgence);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetProcSpell();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 manaPct = 0;
        int32 procSpell = eventInfo.GetProcSpell()->GetSpellInfo()->Id;

        if (procSpell == SPELL_SHAMAN_HEALING_WAVE)
            manaPct = aurEff->GetAmount();

        if (procSpell == SPELL_SHAMAN_HEALING_SURGE || procSpell == SPELL_SHAMAN_RIPTIDE)
            manaPct = GetEffect(EFFECT_1)->GetAmount();

        if (procSpell == SPELL_SHAMAN_CHAIN_HEAL)
            manaPct = GetEffect(EFFECT_2)->GetAmount();

        if (manaPct == 0)
            return;

        int32 maxMana = caster->GetMaxPower(POWER_MANA);
        float amount = CalculatePct(maxMana, manaPct);
        amount /= 100;

        caster->CastCustomSpell(RUNE_SHAMAN_RESURGENCE_ENERGIZE, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_resurgence::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_resurgence::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_high_tide : public AuraScript
{
    PrepareAuraScript(rune_sha_high_tide);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (eventInfo.GetSpellInfo()->PowerType != POWER_MANA)
            return;

        int32 manaThreshold = aurEff->GetAmount();
        int32 procSpell = GetEffect(EFFECT_1)->GetAmount();
        manaSpent += eventInfo.GetSpellInfo()->CalcPowerCost(caster, eventInfo.GetSchoolMask());

        if (manaSpent < manaThreshold)
            return;

        manaSpent -= manaThreshold;
        caster->AddAura(procSpell, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_high_tide::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_high_tide::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
private:
    int32 manaSpent = 0;
};

class rune_sha_spirit_wolf : public AuraScript
{
    PrepareAuraScript(rune_sha_spirit_wolf);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_SHAMAN_GHOST_WOLF))
            return;

        caster->CastSpell(caster, aurEff->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_sha_spirit_wolf::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_sha_rejuvenating_wolf : public AuraScript
{
    PrepareAuraScript(rune_sha_rejuvenating_wolf);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_SHAMAN_GHOST_WOLF))
            return;

        caster->CastSpell(caster, aurEff->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_sha_rejuvenating_wolf::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_sha_regenerating_wolf : public AuraScript
{
    PrepareAuraScript(rune_sha_regenerating_wolf);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_SHAMAN_GHOST_WOLF))
            return;

        caster->CastSpell(caster, aurEff->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_sha_regenerating_wolf::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_sha_brimming_with_life : public AuraScript
{
    PrepareAuraScript(rune_sha_brimming_with_life);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->GetHealthPct() < 100)
            return;

        if (Player* player = caster->ToPlayer())
        {
            if (!player->HasSpellCooldown(SPELL_SHAMAN_REINCARNATION))
                return;

            player->ModifySpellCooldown(SPELL_SHAMAN_REINCARNATION, -aurEff->GetAmount());
        }
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_sha_brimming_with_life::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_sha_improved_earthliving_weapon : public AuraScript
{
    PrepareAuraScript(rune_sha_improved_earthliving_weapon);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetHealInfo()->GetTarget();

        if (!target || target->isDead())
            return;

        int32 heal = eventInfo.GetHealInfo()->GetHeal();
        int32 healPct = aurEff->GetAmount();
        int32 healthThreshold = GetEffect(EFFECT_2)->GetAmount();

        if (target->GetHealthPct() > healthThreshold)
            return;

        int32 amount = CalculatePct(heal, healPct);

        caster->CastCustomSpell(RUNE_SHAMAN_IMPROVED_EARTHLIVING_WEAPON_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_improved_earthliving_weapon::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_improved_earthliving_weapon::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_elemental_equilibrium : public AuraScript
{
    PrepareAuraScript(rune_sha_elemental_equilibrium);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (eventInfo.GetDamageInfo()->GetSchoolMask() == SPELL_SCHOOL_MASK_FIRE)
            caster->AddAura(RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_FIRE, caster);

        if (eventInfo.GetDamageInfo()->GetSchoolMask() == SPELL_SCHOOL_MASK_FROST)
            caster->AddAura(RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_FROST, caster);

        if (eventInfo.GetDamageInfo()->GetSchoolMask() == SPELL_SCHOOL_MASK_NATURE)
            caster->AddAura(RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_NATURE, caster);

        if (!caster->HasAura(RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_FIRE) ||
            !caster->HasAura(RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_FROST) ||
            !caster->HasAura(RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_NATURE))
            return;

        if (caster->HasAura(RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_DEBUFF))
            return;

        caster->AddAura(aurEff->GetAmount(), caster);
        caster->AddAura(RUNE_SHAMAN_ELEMENTAL_EQUILIBRIUM_DEBUFF, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_elemental_equilibrium::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_elemental_equilibrium::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_forceful_winds : public AuraScript
{
    PrepareAuraScript(rune_sha_forceful_winds);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(RUNE_SHAMAN_FORCEFUL_WINDS_BUFF))
            caster->AddAura(RUNE_SHAMAN_FORCEFUL_WINDS_BUFF, caster);

        caster->CastSpell(caster, aurEff->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_forceful_winds::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_forceful_winds::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_forceful_winds_remove : public AuraScript
{
    PrepareAuraScript(rune_sha_forceful_winds_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1000478; i < 1000484; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_sha_forceful_winds_remove::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_sha_primordial_bond : public AuraScript
{
    PrepareAuraScript(rune_sha_primordial_bond);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        hasElemental = false;

        for (Unit::ControlSet::iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
        {
            if ((*itr)->GetEntry() == SUMMON_SHAMAN_FIRE_ELEMENTAL ||
                (*itr)->GetEntry() == SUMMON_SHAMAN_EARTH_ELEMENTAL ||
                (*itr)->GetEntry() == SUMMON_SHAMAN_STORM_ELEMENTAL &&
                (*itr)->IsAlive())
                hasElemental = true;
        }

        int32 buffAura = aurEff->GetAmount();

        if (hasElemental == false)
        {
            if (caster->HasAura(buffAura))
                caster->RemoveAura(buffAura);
        }
        else
        {
            if (!caster->HasAura(buffAura))
                caster->AddAura(buffAura, caster);
        }

    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1000498; i < 1000504; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_sha_primordial_bond::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_sha_primordial_bond::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
private:
    bool hasElemental = false;
};

class rune_sha_pulsating_water : public AuraScript
{
    PrepareAuraScript(rune_sha_pulsating_water);

    void HandleProc(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (!caster->HasAura(SPELL_SHAMAN_WATER_SHIELD))
            return;

        caster->CastSpell(caster, aurEff->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_sha_pulsating_water::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_sha_lightning_rod : public AuraScript
{
    PrepareAuraScript(rune_sha_lightning_rod);

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

        int32 damage = eventInfo.GetDamageInfo()->GetDamage();

        if (damage == 0)
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;
        int32 LightningRod = aurEff->GetAmount();

        if (spellID == SPELL_SHAMAN_EARTH_SHOCK || spellID == SPELL_SHAMAN_EARTHQUAKE_DAMAGE || spellID == SPELL_SHAMAN_ELEMENTAL_BLAST)
            caster->CastSpell(target, LightningRod, TRIGGERED_FULL_MASK);
        else if (spellID == SPELL_SHAMAN_LIGHTNING_BOLT || spellID == SPELL_SHAMAN_CHAIN_LIGHTNING)
        {
            GetEffect(EFFECT_1)->SetAmount(eventInfo.GetDamageInfo()->GetDamage());
            caster->CastSpell(caster, RUNE_SHAMAN_LIGHTNING_ROD_AOE,TRIGGERED_FULL_MASK);
        }

    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_lightning_rod::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_lightning_rod::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_lightning_rod_aoe : public SpellScript
{
    PrepareSpellScript(rune_sha_lightning_rod_aoe);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 1000516; i < 1000522; i++)
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

        int32 lightningRod = GetRuneAura(caster)->GetEffect(EFFECT_0)->GetAmount();
        int32 damage = GetRuneAura(caster)->GetEffect(EFFECT_1)->GetAmount();

        for (auto const& object : targets)
        {
            Unit* target = object->ToUnit();

            if (target->isDead() || !target->HasAura(lightningRod))
                continue;

            int32 damagePct = target->GetAura(lightningRod)->GetEffect(EFFECT_0)->GetAmount();
            int32 amount = CalculatePct(damage, damagePct);
            LOG_ERROR("error", "damagePct = {}, amount = {}", damagePct, amount);
            caster->CastCustomSpell(RUNE_SHAMAN_LIGHTNING_ROD_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(rune_sha_lightning_rod_aoe::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class rune_sha_echoes_of_the_great_sundering : public AuraScript
{
    PrepareAuraScript(rune_sha_echoes_of_the_great_sundering);

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
        int32 earthShockBuff = aurEff->GetAmount();
        int32 elementalBlastBuff = GetEffect(EFFECT_1)->GetAmount();

        if (spellID == SPELL_SHAMAN_EARTH_SHOCK)
            caster->AddAura(earthShockBuff, caster);
        else if (spellID == SPELL_SHAMAN_ELEMENTAL_BLAST)
            caster->AddAura(elementalBlastBuff, caster);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_echoes_of_the_great_sundering::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_echoes_of_the_great_sundering::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_sha_electrified_shocks_remove : public AuraScript
{
    PrepareAuraScript(rune_sha_electrified_shocks_remove);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        for (size_t i = 1000586; i < 1000592; i++)
        {
            if (caster->HasAura(i))
                caster->RemoveAura(i);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_sha_electrified_shocks_remove::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_sha_aftershock : public AuraScript
{
    PrepareAuraScript(rune_sha_aftershock);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 amount = eventInfo.GetSpellInfo()->CalcPowerCost(caster, eventInfo.GetSpellInfo()->GetSchoolMask());

        caster->CastCustomSpell(RUNE_SHAMAN_AFTERSHOCK_ENERGIZE, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_sha_aftershock::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_sha_aftershock::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};



void AddSC_shaman_perks_scripts()
{
    RegisterSpellScript(rune_sha_tidebringer);
    RegisterSpellScript(rune_sha_lava_surge);
    RegisterSpellScript(rune_sha_fiery_demise);
    RegisterSpellScript(rune_sha_volcanic_inferno);
    RegisterSpellScript(rune_sha_magma_burst);
    RegisterSpellScript(rune_sha_flash_of_lightning);
    RegisterSpellScript(rune_sha_pulsating_lightning);
    RegisterSpellScript(rune_sha_flowing_streams);
    RegisterSpellScript(rune_sha_undulation);
    RegisterSpellScript(rune_sha_resurgence);
    RegisterSpellScript(rune_sha_high_tide);
    RegisterSpellScript(rune_sha_spirit_wolf);
    RegisterSpellScript(rune_sha_rejuvenating_wolf);
    RegisterSpellScript(rune_sha_regenerating_wolf);
    RegisterSpellScript(rune_sha_improved_earthliving_weapon);
    RegisterSpellScript(rune_sha_elemental_equilibrium);
    RegisterSpellScript(rune_sha_forceful_winds);
    RegisterSpellScript(rune_sha_forceful_winds_remove);
    RegisterSpellScript(rune_sha_primordial_bond);
    RegisterSpellScript(rune_sha_pulsating_water);
    RegisterSpellScript(rune_sha_lightning_rod);
    RegisterSpellScript(rune_sha_lightning_rod_aoe);
    RegisterSpellScript(rune_sha_echoes_of_the_great_sundering);
    RegisterSpellScript(rune_sha_electrified_shocks_remove);
    RegisterSpellScript(rune_sha_aftershock);

    
    
}
