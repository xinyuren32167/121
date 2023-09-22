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
    //Spells
    SPELL_SHAMAN_ANCESTRAL_GUIDANCE = 84010,
    SPELL_SHAMAN_ANCESTRAL_PROTECTION_TOTEM = 84079,
    SPELL_SHAMAN_ASCENDANCE_WATER = 84040,
    SPELL_SHAMAN_ASTRAL_RECALL = 556,
    SPELL_SHAMAN_ASTRAL_SHIFT = 84007,
    SPELL_SHAMAN_BLOODLUST = 2825,
    SPELL_SHAMAN_CAPACITOR_TOTEM = 84069,
    SPELL_SHAMAN_CHAIN_LIGHTNING = 49271,
    SPELL_SHAMAN_CLOUDBURST_TOTEM = 84082,
    SPELL_SHAMAN_CRASH_LIGHTNING = 84032,
    SPELL_SHAMAN_DOOM_WINDS = 84029,
    SPELL_SHAMAN_DOWNPOUR = 84046,
    SPELL_SHAMAN_EARTH_ELEMENTAL = 2062,
    SPELL_SHAMAN_EARTHBIND_TOTEM = 2484,
    SPELL_SHAMAN_ELEMENTAL_BLAST = 84022,
    SPELL_SHAMAN_ELEMENTAL_MASTERY = 16166,
    SPELL_SHAMAN_FERAL_SPIRIT = 51533,
    SPELL_SHAMAN_FIRE_ELEMENTAL = 2894,
    SPELL_SHAMAN_FLAME_SHOCK = 49233,
    SPELL_SHAMAN_FOCUS_FOE = 84116,
    SPELL_SHAMAN_FURY_OF_THE_ELEMENTS = 84106,
    SPELL_SHAMAN_GROUNDING_TOTEM = 8177,
    SPELL_SHAMAN_GUST_OF_WIND = 84061,
    SPELL_SHAMAN_HEALING_RAIN = 84037,
    SPELL_SHAMAN_HEALING_SURGE = 84004,
    SPELL_SHAMAN_HEALING_TIDE_TOTEM = 84076,
    SPELL_SHAMAN_HEALING_WAVE = 49273,
    SPELL_SHAMAN_HEX = 51514,
    SPELL_SHAMAN_LAVA_BURST = 60043,
    SPELL_SHAMAN_LIGHTNING_LASSO = 84013,
    SPELL_SHAMAN_LIGHTNING_SHIELD = 49281,
    SPELL_SHAMAN_MANA_TIDE_TOTEM = 16190,
    SPELL_SHAMAN_NATURES_SWIFTNESS = 16188,
    SPELL_SHAMAN_OVERCHARGE = 84128,
    SPELL_SHAMAN_REINCARNATION = 20608,
    SPELL_SHAMAN_SPIRIT_LINK_TOTEM = 84073,
    SPELL_SHAMAN_SPIRIT_WALK = 84008,
    SPELL_SHAMAN_STONECLAW_TOTEM = 58582,
    SPELL_SHAMAN_STORM_ELEMENTAL = 84084,
    SPELL_SHAMAN_STORMBRAND_TOTEM = 84244,
    SPELL_SHAMAN_STORMKEEPER = 84018,
    SPELL_SHAMAN_THUNDERSTORM = 59159,
    SPELL_SHAMAN_TIDAL_FORCE = 55198,
    SPELL_SHAMAN_WATERY_GRAVE = 84125,
    SPELL_SHAMAN_WELLSPRING = 84044,
    SPELL_SHAMAN_WIND_SHEAR = 57994,

    //Talents
    TALENT_SHAMAN_BLABLA = 00000,

    //Runes
    RUNE_SHAMAN_VOLCANIC_INFERNO_DOT = 1000178,
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



}
