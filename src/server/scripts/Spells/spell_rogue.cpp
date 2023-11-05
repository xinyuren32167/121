/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with SPELLFAMILY_ROGUE and SPELLFAMILY_GENERIC spells used by rogue players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_rog_".
 */

#include "CellImpl.h"
#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellScript.h"

enum RogueSpells
{
    SPELL_ROGUE_BLADE_FLURRY_EXTRA_ATTACK       = 22482,
    SPELL_ROGUE_CHEAT_DEATH_COOLDOWN            = 31231,
    SPELL_ROGUE_CHEATING_DEATH                  = 45182,
    SPELL_ROGUE_GLYPH_OF_PREPARATION            = 56819,
    SPELL_ROGUE_KILLING_SPREE                   = 51690,
    SPELL_ROGUE_KILLING_SPREE_TELEPORT          = 57840,
    SPELL_ROGUE_KILLING_SPREE_WEAPON_DMG        = 57841,
    SPELL_ROGUE_KILLING_SPREE_DMG_BUFF          = 61851,
    SPELL_ROGUE_PREY_ON_THE_WEAK                = 58670,
    SPELL_ROGUE_SHIV_TRIGGERED                  = 5940,
    SPELL_ROGUE_TRICKS_OF_THE_TRADE_DMG_BOOST   = 57933,
    SPELL_ROGUE_TRICKS_OF_THE_TRADE_PROC        = 59628,

    //OURS
    SPELL_ROGUE_BACKSTAB                        = 82001,
    SPELL_ROGUE_BLADE_FLURRY                    = 13877,
    SPELL_ROG_BLADE_FLURRY_SELECTION            = 82012,
    SPELL_ROG_BLADE_FLURRY_DAMAGE               = 82013,
    SPELL_ROGUE_KILLING_SPREE_FLURRY_DMG        = 82014,
    SPELL_ROGUE_SHADOWSTRIKE_ACTIVATOR          = 82018,
    SPELL_ROGUE_SLICE_AND_DICE                  = 6774,
    SPELL_ROGUE_MARKED_FOR_DEATH                = 82022,
    SPELL_ROGUE_DEATHMARK                       = 82025,
    SPELL_ROGUE_DEATHMARK_PROC                  = 82026,
    SPELL_ROGUE_SERRATED_BONE_SPIKE             = 82032,
    SPELL_ROGUE_BLADE_RUSH_AOE                  = 82030,
    SPELL_ROGUE_KINGSBANE_COMBOPOINT            = 82038,
    SPELL_ROGUE_FLAGELLATION_MASTERY            = 82035,
    SPELL_ROGUE_FLAGELLATION_DAMAGE             = 82039,
    SPELL_ROGUE_SHADOW_BLADES                   = 82043,
    SPELL_ROGUE_COUNTERATTACK_PROC              = 82053,
    SPELL_ROGUE_SECRET_TECHNIQUE                = 82057,
    SPELL_ROGUE_SECRET_TECHNIQUE_AURA           = 82060,
    SPELL_ROGUE_VAMPIRIC_BURST_HEAL             = 82061,
    SPELL_ROGUE_SINISTER_CALLING_PROC           = 82063,
    SPELL_ROGUE_RIPOSTE                         = 82064,
    SPELL_ROGUE_DUELISTS_REFLEX                 = 82067,
    SPELL_ROGUE_SCIMITAR_RUSH_PROC              = 82077,
    SPELL_ROGUE_SCIMITAR_RUSH_AOE               = 82076,
    SPELL_ROGUE_SCIMITAR_RUSH_ENERGY            = 82078,
    SPELL_ROGUE_DREADBLADES_SELF_DAMAGE         = 82081,
    SPELL_ROGUE_SINISTER_STRIKE                 = 48638,
    SPELL_ROGUE_OPPORTUNITY_PROC                = 82086,
    SPELL_ROGUE_SEA_OF_STRIKES_DEBUFF           = 82088,
    SPELL_ROGUE_SEA_OF_STRIKES_PROC             = 82089,
    SPELL_ROGUE_ROLL_THE_BONES_GRAND_MELEE      = 82092,
    SPELL_ROGUE_ROLL_THE_BONES_BROADSIDE        = 82093,
    SPELL_ROGUE_ROLL_THE_BONES_RUTHLESS_PRECISION = 82094,
    SPELL_ROGUE_ROLL_THE_BONES_BURIED_TREASURE  = 82095,
    SPELL_ROGUE_ROLL_THE_BONES_SKULL_AND_CROSSBONES = 82096,
    SPELL_ROGUE_ROLL_THE_BONES_TRUE_BEARING     = 82097,
    SPELL_ROGUE_ADRENALINE_RUSH                 = 13750,
    SPELL_ROGUE_BETWEEN_THE_EYES                = 82072,
    SPELL_ROGUE_SCIMITAR_RUSH                   = 82075,
    SPELL_ROGUE_DREADBLADES                     = 82080,
    SPELL_ROGUE_CAPTAIN_STRIKE                  = 82083,
    SPELL_ROGUE_GRAPPLING_HOOK                  = 82084,
    SPELL_ROGUE_KEEP_IT_ROLLING                 = 82098,
    SPELL_ROGUE_ROLL_THE_BONES                  = 82091,
    SPELL_ROGUE_SPRINT                          = 11305,
    SPELL_ROGUE_VANISH                          = 26889,
    SPELL_ROGUE_OPPORTUNITY                     = 82085,

    TALENT_ROGUE_RELENTLESS_ATTACKS_COMBOPOINT  = 82112,
    TALENT_ROGUE_RELENTLESS_ATTACKS_STACK       = 82113,
    TALENT_ROGUE_RUTHLESS_COMBOPOINT            = 82134,
    TALENT_ROGUE_IMPROVED_ADRENALINE_RUSH       = 82165,
    TALENT_ROGUE_IMPROVED_ADRENALINE_RUSH_PROC  = 82168,
    TALENT_ROGUE_AUDACITY_PROC                  = 82183,

    //POISONS
    //LETHAL
    SPELL_ROGUE_ANESTHETIC_POISON               = 57982,
    SPELL_ROGUE_ANESTHETIC_POISON_PROC          = 57981,
    SPELL_ROGUE_WOUND_POISON                    = 57978,
    SPELL_ROGUE_WOUND_POISON_PROC               = 57975,
    SPELL_ROGUE_DEADLY_POISON                   = 57973,
    SPELL_ROGUE_DEADLY_POISON_PROC              = 57970,
    SPELL_ROGUE_INSTANT_POISON                  = 57968,
    SPELL_ROGUE_INSTANT_POISON_PROC             = 57965,
    SPELL_ROGUE_AMPLIFYING_POISON               = 82005,
    SPELL_ROGUE_AMPLIFYING_POISON_PROC          = 82006,
    SPELL_ROGUE_VAMPIRIC_POISON                 = 82007,
    SPELL_ROGUE_VAMPIRIC_POISON_PROC            = 82008,
    SPELL_ROGUE_VAMPIRIC_POISON_HEAL            = 82009,

    //NON_LETHAL
    SPELL_ROGUE_NUMBING_POISON                  = 5761,
    SPELL_ROGUE_NUMBING_POISON_PROC             = 5760,
    SPELL_ROGUE_NUMBING_POISON_CONCENTRATED     = 82010, //shiv
    SPELL_ROGUE_ATROPHIC_POISON                 = 82003,
    SPELL_ROGUE_ATROPHIC_POISON_PROC            = 82004,
    SPELL_ROGUE_ATROPHIC_POISON_CONCENTRATED    = 82011, //shiv
};

class spell_rog_savage_combat : public AuraScript
{
    PrepareAuraScript(spell_rog_savage_combat);

    void CalcPeriodic(AuraEffect const* /*effect*/, bool& isPeriodic, int32& amplitude)
    {
        isPeriodic = true;
        amplitude = 1000;
    }

    void Update(AuraEffect*  /*auraEffect*/)
    {
        Unit::AuraApplicationMap const& auras = GetUnitOwner()->GetAppliedAuras();
        for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            if (itr->second->GetBase()->GetCasterGUID() == this->GetCasterGUID() && itr->second->GetBase()->GetSpellInfo()->Dispel == DISPEL_POISON)
                return;

        SetDuration(0);
    }

    void Register() override
    {
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_rog_savage_combat::CalcPeriodic, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
        OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_rog_savage_combat::Update, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
    }
};

class spell_rog_combat_potency : public AuraScript
{
    PrepareAuraScript(spell_rog_combat_potency);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetTypeMask() & PROC_FLAG_DONE_MELEE_AUTO_ATTACK;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_rog_combat_potency::CheckProc);
    }
};

// 13877, 33735, (check 51211, 65956) - Blade Flurry
class spell_rog_blade_flurry : public AuraScript
{
    PrepareAuraScript(spell_rog_blade_flurry);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ROGUE_BLADE_FLURRY_EXTRA_ATTACK });
    }

    bool Load() override
    {
        _procTargetGUID.Clear();
        return true;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetActor())
            return false;

        Unit* _procTarget = eventInfo.GetActor()->SelectNearbyNoTotemTarget(eventInfo.GetProcTarget());
        if (_procTarget)
            _procTargetGUID = _procTarget->GetGUID();
        return _procTarget;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        // Xinef: no _procTarget but checkproc passed??
        // Unit::CalculateAOEDamageReduction (this=0x0, damage=4118, schoolMask=1, caster=0x7ffdad089000)
        Unit* procTarget = ObjectAccessor::GetUnit(*GetTarget(), _procTargetGUID);
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();
        if (procTarget && damageInfo)
        {
            int32 damage = damageInfo->GetUnmitigatedDamage();

            CustomSpellValues values;
            values.AddSpellMod(SPELLVALUE_BASE_POINT0, damage);
            values.AddSpellMod(SPELLVALUE_FORCED_CRIT_RESULT, int32(eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT));
            GetTarget()->CastCustomSpell(SPELL_ROGUE_BLADE_FLURRY_EXTRA_ATTACK, values, procTarget, TRIGGERED_FULL_MASK, nullptr, aurEff);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_rog_blade_flurry::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_rog_blade_flurry::HandleProc, EFFECT_0, SPELL_AURA_MOD_MELEE_HASTE);
    }

private:
    ObjectGuid _procTargetGUID;
};

// -31228 - Cheat Death
class spell_rog_cheat_death : public AuraScript
{
    PrepareAuraScript(spell_rog_cheat_death);

    uint32 absorbChance;

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ROGUE_CHEAT_DEATH_COOLDOWN, SPELL_ROGUE_CHEATING_DEATH });
    }

    bool Load() override
    {
        absorbChance = GetSpellInfo()->Effects[EFFECT_0].CalcValue();
        return GetUnitOwner()->ToPlayer();
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Player* target = GetTarget()->ToPlayer();
        if (dmgInfo.GetDamage() < target->GetHealth() || target->HasSpellCooldown(SPELL_ROGUE_CHEAT_DEATH_COOLDOWN) || !roll_chance_i(absorbChance))
            return;

        target->CastSpell(target, SPELL_ROGUE_CHEAT_DEATH_COOLDOWN, true);
        target->CastSpell(target, SPELL_ROGUE_CHEATING_DEATH, true);
        target->AddSpellCooldown(SPELL_ROGUE_CHEAT_DEATH_COOLDOWN, 0, MINUTE * IN_MILLISECONDS);

        uint32 health10 = target->CountPctFromMaxHealth(10);

        // hp > 10% - absorb hp till 10%
        if (target->GetHealth() > health10)
            absorbAmount = dmgInfo.GetDamage() - target->GetHealth() + health10;
        // hp lower than 10% - absorb everything
        else
            absorbAmount = dmgInfo.GetDamage();
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_cheat_death::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_rog_cheat_death::Absorb, EFFECT_0);
    }
};

// -2818 - Deadly Poison
class spell_rog_deadly_poison : public SpellScript
{
    PrepareSpellScript(spell_rog_deadly_poison);

    bool Load() override
    {
        _stackAmount = 0;
        // at this point CastItem must already be initialized
        return GetCaster()->GetTypeId() == TYPEID_PLAYER && GetCastItem();
    }

    void HandleBeforeHit(SpellMissInfo missInfo)
    {
        if (missInfo != SPELL_MISS_NONE)
        {
            return;
        }

        if (Unit* target = GetHitUnit())
            // Deadly Poison
            if (AuraEffect const* aurEff = target->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_ROGUE, 0x10000, 0x80000, 0, GetCaster()->GetGUID()))
                _stackAmount = aurEff->GetBase()->GetStackAmount();
    }

    void HandleAfterHit()
    {
        if (_stackAmount < 5)
            return;

        Player* player = GetCaster()->ToPlayer();

        if (Unit* target = GetHitUnit())
        {
            Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

            if (item == GetCastItem())
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

            if (!item)
                return;

            // item combat enchantments
            for (uint8 slot = 0; slot < MAX_ENCHANTMENT_SLOT; ++slot)
            {
                SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(item->GetEnchantmentId(EnchantmentSlot(slot)));
                if (!enchant)
                    continue;

                for (uint8 s = 0; s < 3; ++s)
                {
                    if (enchant->type[s] != ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
                        continue;

                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(enchant->spellid[s]);
                    if (!spellInfo)
                    {
                        LOG_ERROR("misc", "Player::CastItemCombatSpell Enchant {}, player (Name: {}, {}) cast unknown spell {}",
                            enchant->ID, player->GetName(), player->GetGUID().ToString(), enchant->spellid[s]);
                        continue;
                    }

                    // Proc only rogue poisons
                    if (spellInfo->SpellFamilyName != SPELLFAMILY_ROGUE || spellInfo->Dispel != DISPEL_POISON)
                        continue;

                    // Do not reproc deadly
                    if (spellInfo->Id == SPELL_ROGUE_DEADLY_POISON_PROC)
                        continue;

                    if (spellInfo->IsPositive())
                        player->CastSpell(player, enchant->spellid[s], true, item);
                    else
                        player->CastSpell(target, enchant->spellid[s], true, item);
                }
            }
        }
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_rog_deadly_poison::HandleBeforeHit);
        AfterHit += SpellHitFn(spell_rog_deadly_poison::HandleAfterHit);
    }

    uint8 _stackAmount;
};

// 51690 - Killing Spree
#define KillingSpreeScriptName "spell_rog_killing_spree"
class spell_rog_killing_spree : public SpellScriptLoader
{
public:
    spell_rog_killing_spree() : SpellScriptLoader(KillingSpreeScriptName) { }

    class spell_rog_killing_spree_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_killing_spree_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_ROGUE_KILLING_SPREE });
        }

        SpellCastResult CheckCast()
        {
            // Kologarn area, Killing Spree should not work
            if (GetCaster()->GetMapId() == 603 /*Ulduar*/ && GetCaster()->GetDistance2d(1766.936f, -24.748f) < 50.0f)
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
            return SPELL_CAST_OK;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (targets.empty() || GetCaster()->GetVehicleBase() || GetCaster()->HasUnitState(UNIT_STATE_ROOT))
                FinishCast(SPELL_FAILED_OUT_OF_RANGE);
            else
            {
                // Added attribute not breaking stealth, removes auras here
                GetCaster()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_CAST);
                GetCaster()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_SPELL_ATTACK);
            }
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Aura* aura = GetCaster()->GetAura(SPELL_ROGUE_KILLING_SPREE))
            {
                if (spell_rog_killing_spree_AuraScript* script = dynamic_cast<spell_rog_killing_spree_AuraScript*>(aura->GetScriptByName(KillingSpreeScriptName)))
                    script->AddTarget(GetHitUnit());
            }
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_rog_killing_spree_SpellScript::CheckCast);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rog_killing_spree_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_rog_killing_spree_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_rog_killing_spree_SpellScript();
    }

    class spell_rog_killing_spree_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_killing_spree_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
                {
                    SPELL_ROGUE_KILLING_SPREE_TELEPORT,
                    SPELL_ROGUE_KILLING_SPREE_WEAPON_DMG,
                    SPELL_ROGUE_KILLING_SPREE_DMG_BUFF
                });
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->CastSpell(GetTarget(), SPELL_ROGUE_KILLING_SPREE_DMG_BUFF, true);
        }

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            while (!_targets.empty())
            {
                ObjectGuid guid = Acore::Containers::SelectRandomContainerElement(_targets);
                if (Unit* target = ObjectAccessor::GetUnit(*GetTarget(), guid))
                {
                    // xinef: target may be no longer valid
                    if (!GetTarget()->IsValidAttackTarget(target) || target->HasStealthAura() || target->HasInvisibilityAura())
                    {
                        _targets.remove(guid);
                        continue;
                    }

                    GetTarget()->CastSpell(target, SPELL_ROGUE_KILLING_SPREE_TELEPORT, true);

                    // xinef: ensure fast coordinates switch, dont wait for client to send opcode
                    WorldLocation const& dest = GetTarget()->ToPlayer()->GetTeleportDest();
                    GetTarget()->ToPlayer()->UpdatePosition(dest, true);

                    GetTarget()->CastSpell(target, SPELL_ROGUE_KILLING_SPREE_WEAPON_DMG, TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_DONT_REPORT_CAST_ERROR));
                    if (GetTarget()->HasAura(SPELL_ROGUE_BLADE_FLURRY))
                        GetTarget()->CastSpell(target, SPELL_ROGUE_KILLING_SPREE_FLURRY_DMG, TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_DONT_REPORT_CAST_ERROR));
                    break;
                }
                else
                    _targets.remove(guid);
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_ROGUE_KILLING_SPREE_DMG_BUFF);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_rog_killing_spree_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_killing_spree_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            AfterEffectRemove += AuraEffectRemoveFn(spell_rog_killing_spree_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }

    public:
        void AddTarget(Unit* target)
        {
            _targets.push_back(target->GetGUID());
        }

    private:
        GuidList _targets;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_rog_killing_spree_AuraScript();
    }
};

// -31130 - Nerves of Steel
class spell_rog_nerves_of_steel : public AuraScript
{
    PrepareAuraScript(spell_rog_nerves_of_steel);

    uint32 absorbPct;

    bool Load() override
    {
        absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
        return true;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        // reduces all damage taken while stun or fear
        if (GetTarget()->GetUnitFlags() & (UNIT_FLAG_FLEEING) || (GetTarget()->GetUnitFlags() & (UNIT_FLAG_STUNNED) && GetTarget()->HasAuraWithMechanic(1 << MECHANIC_STUN)))
            absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_nerves_of_steel::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_rog_nerves_of_steel::Absorb, EFFECT_0);
    }
};

// 14185 - Preparation
class spell_rog_preparation : public SpellScript
{
    PrepareSpellScript(spell_rog_preparation);

    bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ROGUE_GLYPH_OF_PREPARATION });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Player* caster = GetCaster()->ToPlayer();
        //immediately finishes the cooldown on certain Rogue abilities

        bool hasGlyph = caster->HasAura(SPELL_ROGUE_GLYPH_OF_PREPARATION);
        PlayerSpellMap const& spellMap = caster->GetSpellMap();
        for (PlayerSpellMap::const_iterator itr = spellMap.begin(); itr != spellMap.end(); ++itr)
        {
            SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(itr->first);
            if (spellInfo->SpellFamilyName == SPELLFAMILY_ROGUE)
            {
                if (spellInfo->SpellFamilyFlags[1] & SPELLFAMILYFLAG1_ROGUE_COLDB_SHADOWSTEP ||      // Cold Blood, Shadowstep
                        spellInfo->SpellFamilyFlags[0] & SPELLFAMILYFLAG_ROGUE_VAN_EVAS_SPRINT)           // Vanish, Evasion, Sprint
                {
                    SpellCooldowns::iterator citr = caster->GetSpellCooldownMap().find(spellInfo->Id);
                    if (citr != caster->GetSpellCooldownMap().end() && citr->second.needSendToClient)
                        caster->RemoveSpellCooldown(spellInfo->Id, true);
                    else
                        caster->RemoveSpellCooldown(spellInfo->Id, false);
                }
                else if (hasGlyph)
                {
                    if (spellInfo->SpellFamilyFlags[1] & SPELLFAMILYFLAG1_ROGUE_DISMANTLE ||         // Dismantle
                            spellInfo->SpellFamilyFlags[0] & SPELLFAMILYFLAG_ROGUE_KICK ||               // Kick
                            (spellInfo->SpellFamilyFlags[0] & SPELLFAMILYFLAG_ROGUE_BLADE_FLURRY &&     // Blade Flurry
                                spellInfo->SpellFamilyFlags[1] & SPELLFAMILYFLAG1_ROGUE_BLADE_FLURRY))
                    {
                        SpellCooldowns::iterator citr = caster->GetSpellCooldownMap().find(spellInfo->Id);
                        if (citr != caster->GetSpellCooldownMap().end() && citr->second.needSendToClient)
                            caster->RemoveSpellCooldown(spellInfo->Id, true);
                        else
                            caster->RemoveSpellCooldown(spellInfo->Id, false);
                    }
                }
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_preparation::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// -51685 - Prey on the Weak
class spell_rog_prey_on_the_weak : public AuraScript
{
    PrepareAuraScript(spell_rog_prey_on_the_weak);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ROGUE_PREY_ON_THE_WEAK });
    }

    void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
    {
        Unit* target = GetTarget();
        Unit* victim = target->GetVictim();
        if (!victim && target->GetTypeId() == TYPEID_PLAYER)
            victim = target->ToPlayer()->GetSelectedUnit();

        if (victim && (target->GetHealthPct() > victim->GetHealthPct()))
        {
            if (!target->HasAura(SPELL_ROGUE_PREY_ON_THE_WEAK))
            {
                int32 bp = GetSpellInfo()->Effects[EFFECT_0].CalcValue();
                target->CastCustomSpell(target, SPELL_ROGUE_PREY_ON_THE_WEAK, &bp, 0, 0, true);
            }
        }
        else
            target->RemoveAurasDueToSpell(SPELL_ROGUE_PREY_ON_THE_WEAK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_prey_on_the_weak::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// -1943 - Rupture
class spell_rog_rupture : public AuraScript
{
    PrepareAuraScript(spell_rog_rupture);

    bool Load() override
    {
        Unit* caster = GetCaster();
        return caster && caster->GetTypeId() == TYPEID_PLAYER;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        if (Unit* caster = GetCaster())
        {
            canBeRecalculated = false;

            float const attackpowerPerCombo[6] =
            {
                0.0f,
                0.015f,         // 1 point:  ${($m1 + $b1*1 + 0.015 * $AP) * 4} damage over 8 secs
                0.024f,         // 2 points: ${($m1 + $b1*2 + 0.024 * $AP) * 5} damage over 10 secs
                0.03f,          // 3 points: ${($m1 + $b1*3 + 0.03 * $AP) * 6} damage over 12 secs
                0.03428571f,    // 4 points: ${($m1 + $b1*4 + 0.03428571 * $AP) * 7} damage over 14 secs
                0.0375f         // 5 points: ${($m1 + $b1*5 + 0.0375 * $AP) * 8} damage over 16 secs
            };

            uint8 cp = caster->ToPlayer()->GetComboPoints();
            if (cp > 5)
                cp = 5;

            amount += int32(caster->GetTotalAttackPowerValue(BASE_ATTACK) * attackpowerPerCombo[cp]);

            if (AuraEffect const* gamblersLuck = GetCaster()->GetAuraEffectOfRankedSpell(82179, EFFECT_0))
                if (GetCaster()->HasAura(82092) || GetCaster()->HasAura(82093) || GetCaster()->HasAura(82094) || GetCaster()->HasAura(82095) || GetCaster()->HasAura(82096) || GetCaster()->HasAura(82097))
                    AddPct(amount, gamblersLuck->GetAmount());
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_rupture::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// 5938 - Shiv
class spell_rog_shiv : public SpellScript
{
    PrepareSpellScript(spell_rog_shiv);

    bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ROGUE_SHIV_TRIGGERED });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (Unit* unitTarget = GetHitUnit())
            caster->CastSpell(unitTarget, SPELL_ROGUE_SHIV_TRIGGERED, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_shiv::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 57934 - Tricks of the Trade
class spell_rog_tricks_of_the_trade : public AuraScript
{
    PrepareAuraScript(spell_rog_tricks_of_the_trade);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ROGUE_TRICKS_OF_THE_TRADE_DMG_BOOST, SPELL_ROGUE_TRICKS_OF_THE_TRADE_PROC });
    }

    bool Load() override
    {
        _redirectTarget = nullptr;
        return true;
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEFAULT)
            GetTarget()->ResetRedirectThreat();
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        _redirectTarget = GetTarget()->GetRedirectThreatTarget();
        return _redirectTarget;
    }

    void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();

        Unit* target = GetTarget();
        target->CastSpell(_redirectTarget, SPELL_ROGUE_TRICKS_OF_THE_TRADE_DMG_BOOST, true);
        target->CastSpell(target, SPELL_ROGUE_TRICKS_OF_THE_TRADE_PROC, true);
        Remove(AURA_REMOVE_BY_DEFAULT); // maybe handle by proc charges
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_rog_tricks_of_the_trade::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        DoCheckProc += AuraCheckProcFn(spell_rog_tricks_of_the_trade::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_rog_tricks_of_the_trade::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
    }

private:
    Unit* _redirectTarget;
};

// 59628 - Tricks of the Trade (Proc)
class spell_rog_tricks_of_the_trade_proc : public AuraScript
{
    PrepareAuraScript(spell_rog_tricks_of_the_trade_proc);

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->ResetRedirectThreat();
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_rog_tricks_of_the_trade_proc::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_rog_backstab : public SpellScript
{
    PrepareSpellScript(spell_rog_backstab);

    void HandleDamage(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        if (Unit* target = GetHitUnit())
        {
            if (!target->HasInArc(M_PI, caster))
            {
                int32 damage = GetEffectValue();
                SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_ROGUE_BACKSTAB);
                uint32 damageBonus = value->GetEffect(EFFECT_1).CalcValue(caster);

                damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
                damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

                AddPct(damage, damageBonus);

                SetHitDamage(damage);
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_backstab::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_rog_deadly_throw : public SpellScript
{
    PrepareSpellScript(spell_rog_deadly_throw);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damageRatio = GetCaster()->GetComboPoints() * GetEffectValue();
        int32 damage = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        if (AuraEffect const* gamblersLuck = GetCaster()->GetAuraEffectOfRankedSpell(82179, EFFECT_0))
            if (GetCaster()->HasAura(82092) || GetCaster()->HasAura(82093) || GetCaster()->HasAura(82094) || GetCaster()->HasAura(82095) || GetCaster()->HasAura(82096) || GetCaster()->HasAura(82097))
                AddPct(damage, gamblersLuck->GetAmount());

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_deadly_throw::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_rog_eviscerate : public SpellScript
{
    PrepareSpellScript(spell_rog_eviscerate);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damageRatio = GetCaster()->GetComboPoints() * GetEffectValue();
        int32 damage = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        if (AuraEffect const* gamblersLuck = GetCaster()->GetAuraEffectOfRankedSpell(82179, EFFECT_0))
            if (GetCaster()->HasAura(82092) || GetCaster()->HasAura(82093) || GetCaster()->HasAura(82094) || GetCaster()->HasAura(82095) || GetCaster()->HasAura(82096) || GetCaster()->HasAura(82097))
                AddPct(damage, gamblersLuck->GetAmount());

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_eviscerate::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_rog_vampiric_poison : public SpellScript
{
    PrepareSpellScript(spell_rog_vampiric_poison);

    uint8 _stackAmount;

    void HandleBeforeHit(SpellMissInfo missInfo)
    {
        if (missInfo != SPELL_MISS_NONE)
        {
            return;
        }

        if (Unit* target = GetHitUnit())
            if (auto* aurEff = target->GetAura(SPELL_ROGUE_VAMPIRIC_POISON_PROC, GetCaster()->GetGUID()))
                _stackAmount = aurEff->GetStackAmount();
    }

    void HandleAfterHit()
    {
        if (_stackAmount < 5)
            return;

        Unit* caster = GetCaster();

        caster->CastSpell(caster, SPELL_ROGUE_VAMPIRIC_POISON_HEAL, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_rog_vampiric_poison::HandleBeforeHit);
        AfterHit += SpellHitFn(spell_rog_vampiric_poison::HandleAfterHit);
    }
};

class spell_rog_envenom : public SpellScript
{
    PrepareSpellScript(spell_rog_envenom);

    void HandleHit(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        int32 damageRatio = caster->GetComboPoints() * GetEffectValue();
        int32 damage = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);

        if (Unit* target = GetHitUnit())
        {
            if (auto* aurEff = target->GetAura(SPELL_ROGUE_AMPLIFYING_POISON_PROC, caster->GetGUID()))
            {
                int32 stackAmount = aurEff->GetStackAmount();
                if (stackAmount >= 10)
                {
                    SpellInfo const* spell = sSpellMgr->AssertSpellInfo(SPELL_ROGUE_AMPLIFYING_POISON);
                    uint32 damagePct = spell->GetEffect(EFFECT_0).CalcValue(caster);

                    aurEff->SetStackAmount(stackAmount - 10);
                    damage += CalculatePct(damage, damagePct);
                }
            }
            damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_envenom::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_rog_shiv_poison : public SpellScript
{
    PrepareSpellScript(spell_rog_shiv_poison);

    void HandleHit(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
        {
            Player* player = GetCaster()->ToPlayer();

            if (uint32 enchantID = player->GetItemEnchant(EQUIPMENT_SLOT_OFFHAND, SPELLFAMILY_ROGUE, DISPEL_POISON)) {
                if (enchantID == SPELL_ROGUE_NUMBING_POISON_PROC)
                    player->CastSpell(target, SPELL_ROGUE_NUMBING_POISON_CONCENTRATED, TRIGGERED_FULL_MASK);
                if (enchantID == SPELL_ROGUE_ATROPHIC_POISON_PROC)
                    player->CastSpell(target, SPELL_ROGUE_ATROPHIC_POISON_CONCENTRATED, TRIGGERED_FULL_MASK);
            }
        } 
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_shiv_poison::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_rog_blade_flurry_new : public AuraScript
{
    PrepareAuraScript(spell_rog_blade_flurry_new);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage && GetCaster()->IsAlive())
            {
                int32 damagePct = aurEff->GetAmount();
                int32 damageAmount = CalculatePct(damage, damagePct);
                GetCaster()->CastCustomSpell(SPELL_ROG_BLADE_FLURRY_SELECTION, SPELLVALUE_BASE_POINT0, damageAmount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_blade_flurry_new::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_blade_flurry_triggered : public SpellScript
{
    PrepareSpellScript(spell_rog_blade_flurry_triggered);

    void HandleHit(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
        {
            Spell* spell = GetSpell();
            int32 damage = GetEffectValue();
            Unit* caster = GetCaster();
            SpellInfo const* flurry = sSpellMgr->AssertSpellInfo(SPELL_ROGUE_BLADE_FLURRY);

            std::list<WorldObject*> targets;
            spell->SearchAreaTargets(targets, 5.0f, target, caster, TARGET_OBJECT_TYPE_UNIT, TARGET_CHECK_ENEMY, nullptr);
            targets.remove(target);

            if (targets.size() > 0)
            {
                for (auto const& enemy : targets)
                    if (Unit* creatureTarget = enemy->ToUnit())
                    {
                        caster->CastCustomSpell(SPELL_ROG_BLADE_FLURRY_DAMAGE, SPELLVALUE_BASE_POINT0, damage, creatureTarget, TRIGGERED_FULL_MASK);
                    }
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_blade_flurry_triggered::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_rog_shadowstrike_activator : public AuraScript
{
    PrepareAuraScript(spell_rog_shadowstrike_activator);

    void OnApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetCaster()->CastSpell(GetCaster(), SPELL_ROGUE_SHADOWSTRIKE_ACTIVATOR, TRIGGERED_FULL_MASK);
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Aura* aura = GetCaster()->GetAura(SPELL_ROGUE_SHADOWSTRIKE_ACTIVATOR))
            aura->SetDuration(500);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_rog_shadowstrike_activator::OnApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_rog_shadowstrike_activator::OnRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_rog_shadowstrike : public SpellScript
{
    PrepareSpellScript(spell_rog_shadowstrike);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        if (caster->HasAura(SPELL_ROGUE_SHADOWSTRIKE_ACTIVATOR))
        {
            Position pos = GetExplTargetUnit()->GetPosition();
            caster->NearTeleportTo(pos);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_rog_shadowstrike::HandleCast);
    }
};

class spell_rog_premeditation : public AuraScript
{
    PrepareAuraScript(spell_rog_premeditation);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_ROGUE_SHADOWSTRIKE_ACTIVATOR))
            {
                Unit* target = eventInfo.GetActionTarget();
                int32 duration = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();

                if (Aura* aura = caster->GetAura(SPELL_ROGUE_SLICE_AND_DICE))
                {
                    caster->AddComboPoints(target, aurEff->GetAmount());

                    if (aura->GetDuration() < duration)
                        aura->SetDuration(duration);
                }
                else
                    caster->CastCustomSpell(SPELL_ROGUE_SLICE_AND_DICE, SPELLVALUE_AURA_DURATION, duration, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_premeditation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_marked_for_death : public AuraScript
{
    PrepareAuraScript(spell_rog_marked_for_death);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
           caster->RemoveSpellCooldown(SPELL_ROGUE_MARKED_FOR_DEATH, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_marked_for_death::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_poisoned_knife : public SpellScript
{
    PrepareSpellScript(spell_rog_poisoned_knife);

    void HandleHit(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
        {
            Player* player = GetCaster()->ToPlayer();

            if (uint32 mainHand = player->GetItemEnchant(EQUIPMENT_SLOT_MAINHAND, SPELLFAMILY_ROGUE, DISPEL_POISON))
                player->CastSpell(target, mainHand, TRIGGERED_FULL_MASK);

            if(uint32 offHand = player->GetItemEnchant(EQUIPMENT_SLOT_OFFHAND, SPELLFAMILY_ROGUE, DISPEL_POISON))
                player->CastSpell(target, offHand, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_poisoned_knife::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_rog_deathmark : public AuraScript
{
    PrepareAuraScript(spell_rog_deathmark);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Unit* target = eventInfo.GetActionTarget())
        {
            if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && target->HasAura(SPELL_ROGUE_DEATHMARK))
            {
                Unit* caster = GetCaster();
                uint32 damage = eventInfo.GetDamageInfo()->GetDamage();
                if (damage && caster->IsAlive())
                {
                    int32 damagePct = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
                    uint32 damageAmount = CalculatePct(damage, damagePct);

                    caster->CastCustomSpell(SPELL_ROGUE_DEATHMARK_PROC, SPELLVALUE_BASE_POINT0, damageAmount, target, TRIGGERED_FULL_MASK);
                }
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_deathmark::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
    }
};

class spell_rog_crimson_tempest : public SpellScript
{
    PrepareSpellScript(spell_rog_crimson_tempest);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damageRatio = GetCaster()->GetComboPoints() * GetEffectValue();
        int32 damage = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        SetHitDamage(damage);
    }
    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_crimson_tempest::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_rog_blade_rush : public SpellScript
{
    PrepareSpellScript(spell_rog_blade_rush);

    void HandleHit()
    {
        if (Unit* target = GetHitUnit())
            GetCaster()->CastSpell(target, SPELL_ROGUE_BLADE_RUSH_AOE);
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_rog_blade_rush::HandleHit);
    }
};

class spell_rog_crimson_tempest_dot : public AuraScript
{
    PrepareAuraScript(spell_rog_crimson_tempest_dot);

    bool Load() override
    {
        Unit* caster = GetCaster();
        return caster && caster->GetTypeId() == TYPEID_PLAYER;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        if (Unit* caster = GetCaster())
        {
            canBeRecalculated = false;

            uint32 ap = caster->GetTotalAttackPowerValue(BASE_ATTACK);
            uint32 damage = CalculatePct(ap, 14);

            amount = damage;
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_crimson_tempest_dot::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

class spell_rog_serrated_bone_spike : public SpellScript
{
    PrepareSpellScript(spell_rog_serrated_bone_spike);

    void HandleHit(SpellMissInfo missInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_ROGUE_SERRATED_BONE_SPIKE);
        int32 points = 1;
        int32 additionalPoints = value->GetEffect(EFFECT_0).CalcValue(caster);

        if (Aura* aura = caster->GetAura(SPELL_ROGUE_SERRATED_BONE_SPIKE))
        {
            uint32 stackAmount = aura->GetStackAmount();
            points += stackAmount * additionalPoints;
        }
        caster->AddComboPoints(target, points);
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_rog_serrated_bone_spike::HandleHit);
    }
};

class spell_rog_serrated_bone_spike_death : public AuraScript
{
    PrepareAuraScript(spell_rog_serrated_bone_spike_death);


    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_ROGUE_SERRATED_BONE_SPIKE, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_serrated_bone_spike_death::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_kingsbane : public AuraScript
{
    PrepareAuraScript(spell_rog_kingsbane);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActor()->GetGUID() == GetCaster()->GetGUID();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        AuraEffect* effect = GetEffect(EFFECT_1);
        uint32 damage = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), 26);
        uint32 damageIncrease = aurEff->GetAmount();

        damage += CalculatePct(damage, damageIncrease);
        effect->ChangeAmount(damage);
        effect->ResetTicks();
        effect->CalculatePeriodic(caster);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_rog_kingsbane::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_rog_kingsbane::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_kingsbane_combopoint : public SpellScript
{
    PrepareSpellScript(spell_rog_kingsbane_combopoint);

    void HandleHit(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
        {
            GetCaster()->CastSpell(target, SPELL_ROGUE_KINGSBANE_COMBOPOINT, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_kingsbane_combopoint::HandleHit, EFFECT_2, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_rog_flagellation : public AuraScript
{
    PrepareAuraScript(spell_rog_flagellation);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            uint32 comboPoints = player->GetComboPoints();
            int32 attackPowerPct = aurEff->GetAmount();
            int32 damage = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), comboPoints * attackPowerPct);
            if (Unit* target = ObjectAccessor::GetUnit(*player, player->GetTarget()))
            {
                player->CastCustomSpell(SPELL_ROGUE_FLAGELLATION_MASTERY, SPELLVALUE_AURA_STACK, comboPoints, target, TRIGGERED_FULL_MASK);
                player->CastCustomSpell(SPELL_ROGUE_FLAGELLATION_DAMAGE, SPELLVALUE_BASE_POINT0, damage, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Aura* flagellationMastery = GetCaster()->GetAura(SPELL_ROGUE_FLAGELLATION_MASTERY);
        if (flagellationMastery)
        {
            int32 duration = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
            flagellationMastery->SetDuration(duration);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_flagellation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_rog_flagellation::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};
 
class spell_rog_shadow_blade : public AuraScript
{
    PrepareAuraScript(spell_rog_shadow_blade);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && GetCaster()->GetTarget();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 attackPowerPct = aurEff->GetAmount();
        uint32 damage = eventInfo.GetDamageInfo()->GetDamage();
        damage = CalculatePct(damage, attackPowerPct);

        if (Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetTarget()))
            GetCaster()->CastCustomSpell(SPELL_ROGUE_SHADOW_BLADES, SPELLVALUE_BASE_POINT0, damage, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_rog_shadow_blade::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_rog_shadow_blade::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
    }
};

class spell_rog_black_powder : public SpellScript
{
    PrepareSpellScript(spell_rog_black_powder);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damageRatio = GetCaster()->GetComboPoints() * GetEffectValue();
        int32 damage = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        SetHitDamage(damage);
    }
    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_black_powder::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 82060 - Secret Technique
#define spellRogSecretTechnique "spell_rog_secret_technique"
class spell_rog_secret_technique : public SpellScriptLoader
{
public:
    spell_rog_secret_technique() : SpellScriptLoader(spellRogSecretTechnique) { }

    class spell_rog_secret_technique_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_secret_technique_SpellScript);

        void HandleHit(SpellEffIndex effIndex)
        {
            int32 damageRatio = GetCaster()->GetComboPoints() * GetEffectValue();
            int32 damage = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio / 100);

            if (Unit* target = GetHitUnit())
            {
                damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
                damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
            }

            SetHitDamage(damage);
        }
        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_rog_secret_technique_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_rog_secret_technique_SpellScript();
    }
};

class spell_rog_secret_technique_AuraScript : public AuraScript
{
    PrepareAuraScript(spell_rog_secret_technique_AuraScript);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetSpellInfo()->Id == SPELL_ROGUE_SECRET_TECHNIQUE)
            return;

        uint32 duration = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        player->ModifySpellCooldown(SPELL_ROGUE_SECRET_TECHNIQUE, -duration * player->GetComboPoints());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_secret_technique_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_vampiric_burst : public SpellScript
{
    PrepareSpellScript(spell_rog_vampiric_burst);

    SpellCastResult CheckCast()
    {
        Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetTarget());

        if (!target)
            return SPELL_FAILED_NOTHING_TO_STEAL;

        if (Aura* aura = target->GetAura(SPELL_ROGUE_VAMPIRIC_POISON_PROC))
            return SPELL_CAST_OK;

        return SPELL_FAILED_NOTHING_TO_STEAL;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();
        Unit* target = ObjectAccessor::GetUnit(*caster, caster->GetTarget());

        if (!target)
            return;

        int32 ratio = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(caster);

        Aura* aura = target->GetAura(SPELL_ROGUE_VAMPIRIC_POISON_PROC);

        if (!aura)
            return;

        AuraEffect* effect = aura->GetEffect(EFFECT_0);
        uint32 remaningTicks = effect->GetRemaningTicks();
        uint32 remaningDamage = effect->GetAmount() / remaningTicks;
        int32 heal = CalculatePct(remaningDamage, ratio);
        caster->CastCustomSpell(SPELL_ROGUE_VAMPIRIC_BURST_HEAL, SPELLVALUE_BASE_POINT0, heal, caster, TRIGGERED_FULL_MASK);
        aura->Remove();
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_rog_vampiric_burst::CheckCast);
        OnCast += SpellCastFn(spell_rog_vampiric_burst::HandleCast);
    }
};

class spell_rog_counterattack : public AuraScript
{
    PrepareAuraScript(spell_rog_counterattack);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        uint32 parryChance = caster->GetUnitParryChance();
        caster->CastCustomSpell(SPELL_ROGUE_COUNTERATTACK_PROC, SPELLVALUE_BASE_POINT0, parryChance, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_counterattack::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_sinister_strike : public SpellScript
{
    PrepareSpellScript(spell_rog_sinister_strike);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damageRatio = GetEffectValue();
        int32 damage = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        if (AuraEffect const* gamblersLuck = GetCaster()->GetAuraEffectOfRankedSpell(82179, EFFECT_0))
            if (GetCaster()->HasAura(82092) || GetCaster()->HasAura(82093) || GetCaster()->HasAura(82094) || GetCaster()->HasAura(82095) || GetCaster()->HasAura(82096) || GetCaster()->HasAura(82097))
                AddPct(damage, gamblersLuck->GetAmount());

        SetHitDamage(damage);
    }
   
    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_sinister_strike::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_rog_secret_technique_teacher : public AuraScript
{
    PrepareAuraScript(spell_rog_secret_technique_teacher);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->learnSpell(SPELL_ROGUE_SECRET_TECHNIQUE);
        target->CastSpell(target, SPELL_ROGUE_SECRET_TECHNIQUE_AURA, TRIGGERED_FULL_MASK);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_ROGUE_SECRET_TECHNIQUE, SPEC_MASK_ALL, false);
        target->RemoveAura(SPELL_ROGUE_SECRET_TECHNIQUE_AURA);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_rog_secret_technique_teacher::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_rog_secret_technique_teacher::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_rog_sinister_calling : public AuraScript
{
    PrepareAuraScript(spell_rog_sinister_calling);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() >0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        Unit* caster = GetCaster();

        if (!target || caster->isDead())
            return;

        uint32 damageAmount = aurEff->GetAmount();
        uint32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), damageAmount);

        caster->CastCustomSpell(SPELL_ROGUE_SINISTER_CALLING_PROC, SPELLVALUE_BASE_POINT0, damage, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_rog_sinister_calling::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_rog_sinister_calling::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_forced_counter : public AuraScript
{
    PrepareAuraScript(spell_rog_forced_counter);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        uint32 chance = CalculatePct(caster->GetUnitParryChance(),aurEff->GetAmount());

        if (!roll_chance_i(chance))
        {
            return;
        }
        caster->RemoveSpellCooldown(SPELL_ROGUE_RIPOSTE, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_forced_counter::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_amplifying_poison_replacer : public AuraScript
{
    PrepareAuraScript(spell_rog_amplifying_poison_replacer);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_ROGUE_INSTANT_POISON, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_ROGUE_AMPLIFYING_POISON);

        Item* itemMain = target->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
        if (uint32 mainEnchantID = target->GetItemEnchant(EQUIPMENT_SLOT_MAINHAND, SPELLFAMILY_ROGUE, DISPEL_POISON))
            if (mainEnchantID == SPELL_ROGUE_INSTANT_POISON_PROC)
                itemMain->ClearEnchantment(TEMP_ENCHANTMENT_SLOT);

        Item* itemOff = target->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
        if (uint32 offEnchantID = target->GetItemEnchant(EQUIPMENT_SLOT_OFFHAND, SPELLFAMILY_ROGUE, DISPEL_POISON))
            if (offEnchantID == SPELL_ROGUE_INSTANT_POISON_PROC)
                itemOff->ClearEnchantment(TEMP_ENCHANTMENT_SLOT);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        
        target->removeSpell(SPELL_ROGUE_AMPLIFYING_POISON, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_ROGUE_INSTANT_POISON);

        Item* itemMain = target->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
        if (uint32 mainEnchantID = target->GetItemEnchant(EQUIPMENT_SLOT_MAINHAND, SPELLFAMILY_ROGUE, DISPEL_POISON))
            if (mainEnchantID == SPELL_ROGUE_AMPLIFYING_POISON_PROC)
                itemMain->ClearEnchantment(TEMP_ENCHANTMENT_SLOT);

        Item* itemOff = target->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
        if(uint32 offEnchantID = target->GetItemEnchant(EQUIPMENT_SLOT_OFFHAND, SPELLFAMILY_ROGUE, DISPEL_POISON))
            if (offEnchantID == SPELL_ROGUE_AMPLIFYING_POISON_PROC)
                itemOff->ClearEnchantment(TEMP_ENCHANTMENT_SLOT);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_rog_amplifying_poison_replacer::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_rog_amplifying_poison_replacer::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_rog_duelists_reflex : public AuraScript
{
    PrepareAuraScript(spell_rog_duelists_reflex);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_ROGUE_RIPOSTE, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_duelists_reflex::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_master_duelist : public AuraScript
{
    PrepareAuraScript(spell_rog_master_duelist);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->learnSpell(SPELL_ROGUE_VAMPIRIC_POISON);
        target->learnSpell(SPELL_ROGUE_DUELISTS_REFLEX);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_ROGUE_VAMPIRIC_POISON, SPEC_MASK_ALL, false);
        target->removeSpell(SPELL_ROGUE_DUELISTS_REFLEX, SPEC_MASK_ALL, false);

        Item* itemMain = target->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
        if (uint32 mainEnchantID = target->GetItemEnchant(EQUIPMENT_SLOT_MAINHAND, SPELLFAMILY_ROGUE, DISPEL_POISON))
            if (mainEnchantID == SPELL_ROGUE_VAMPIRIC_POISON_PROC)
                itemMain->ClearEnchantment(TEMP_ENCHANTMENT_SLOT);

        Item* itemOff = target->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
        if (uint32 offEnchantID = target->GetItemEnchant(EQUIPMENT_SLOT_OFFHAND, SPELLFAMILY_ROGUE, DISPEL_POISON))
            if (offEnchantID == SPELL_ROGUE_VAMPIRIC_POISON_PROC)
                itemOff->ClearEnchantment(TEMP_ENCHANTMENT_SLOT);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_rog_master_duelist::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_rog_master_duelist::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_rog_cut_to_the_chase: public AuraScript
{
    PrepareAuraScript(spell_rog_cut_to_the_chase);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (Aura* aura = caster->GetAura(SPELL_ROGUE_SLICE_AND_DICE))
        {
            uint32 comboPoints = caster->GetComboPoints();
            uint32 duration = aurEff->GetAmount() * comboPoints;

            aura->SetDuration(aura->GetDuration() + duration);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_cut_to_the_chase::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_between_the_eyes : public SpellScript
{
    PrepareSpellScript(spell_rog_between_the_eyes);

    void HandleHit(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        int32 damageRatio = caster->GetComboPoints() * GetEffectValue();
        int32 damage = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);

        if (Unit* target = GetHitUnit())
        {
            damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        if (AuraEffect const* gamblersLuck = GetCaster()->GetAuraEffectOfRankedSpell(82179, EFFECT_0))
            if (GetCaster()->HasAura(82092) || GetCaster()->HasAura(82093) || GetCaster()->HasAura(82094) || GetCaster()->HasAura(82095) || GetCaster()->HasAura(82096) || GetCaster()->HasAura(82097))
                AddPct(damage, gamblersLuck->GetAmount());

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_between_the_eyes::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_rog_scimitar_rush : public SpellScript
{
    PrepareSpellScript(spell_rog_scimitar_rush);

    void HandleHit()
    {
        Unit* caster = GetCaster();

        if (Unit* target = GetHitUnit())
        {
            caster->CastSpell(target, SPELL_ROGUE_SCIMITAR_RUSH_AOE, TRIGGERED_FULL_MASK);
            caster->CastSpell(target, SPELL_ROGUE_SCIMITAR_RUSH_PROC, TRIGGERED_FULL_MASK);
            caster->CastSpell(caster, SPELL_ROGUE_SCIMITAR_RUSH_ENERGY, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_rog_scimitar_rush::HandleHit);
    }
};

class spell_rog_scimitar_rush_target_select : public SpellScript
{
    PrepareSpellScript(spell_rog_scimitar_rush_target_select);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetTarget());
        targets.remove(target);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rog_scimitar_rush_target_select::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class spell_rog_dispatch : public SpellScript
{
    PrepareSpellScript(spell_rog_dispatch);

    void HandleHit(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        int32 damageRatio = caster->GetComboPoints() * GetEffectValue();
        int32 damage = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), damageRatio);

        if (Unit* target = GetHitUnit())
        {
            damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rog_dispatch::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_rog_dreadblades : public AuraScript
{
    PrepareAuraScript(spell_rog_dreadblades);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        uint32 health = caster->CountPctFromCurHealth(aurEff->GetAmount());
        caster->CastCustomSpell(SPELL_ROGUE_DREADBLADES_SELF_DAMAGE, SPELLVALUE_BASE_POINT0, health, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_dreadblades::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_opportunity : public AuraScript
{
    PrepareAuraScript(spell_rog_opportunity);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(eventInfo.GetActionTarget(), SPELL_ROGUE_SINISTER_STRIKE, TRIGGERED_FULL_MASK);
        caster->CastSpell(caster, SPELL_ROGUE_OPPORTUNITY_PROC, TRIGGERED_FULL_MASK);

        if (roll_chance_i(aurEff->GetAmount()))
            caster->CastSpell(eventInfo.GetActionTarget(), SPELL_ROGUE_SINISTER_STRIKE, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_opportunity::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rogue_sea_of_strikes : public AuraScript
{
    PrepareAuraScript(spell_rogue_sea_of_strikes);

    std::list <Unit*> FindTargets()
    {
        auto const& threatlist = GetCaster()->getAttackers();
        std::list <Unit*> targetAvailable;

        for (auto const& target : threatlist)
        {
            if (target)
                if (target->HasAura(SPELL_ROGUE_SEA_OF_STRIKES_DEBUFF))
                    if (target->GetAura(SPELL_ROGUE_SEA_OF_STRIKES_DEBUFF)->GetCasterGUID() == GetCaster()->GetGUID())
                    {
                        Unit* dummy = target->ToUnit();
                        if (dummy)
                            targetAvailable.push_back(dummy);
                    }
        }return targetAvailable;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage() || damageInfo->GetDamage() == 0)
        {
            return false;
        }

        return GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        int32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());

        for (auto const& target : FindTargets())
        {
            caster->CastCustomSpell(SPELL_ROGUE_SEA_OF_STRIKES_PROC, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            target->RemoveAura(SPELL_ROGUE_SEA_OF_STRIKES_DEBUFF);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_rogue_sea_of_strikes::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_rogue_sea_of_strikes::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_roll_the_bones : public SpellScript
{
    PrepareSpellScript(spell_rog_roll_the_bones);

    void HandleCast()
    {
        uint32 rand = urand(1, 6);
        Unit* caster = GetCaster();

        if (rand == 1)
            caster->CastSpell(caster, SPELL_ROGUE_ROLL_THE_BONES_GRAND_MELEE, TRIGGERED_FULL_MASK);
        else if (rand == 2)
            caster->CastSpell(caster, SPELL_ROGUE_ROLL_THE_BONES_BROADSIDE, TRIGGERED_FULL_MASK);
        else if (rand == 3)
            caster->CastSpell(caster, SPELL_ROGUE_ROLL_THE_BONES_RUTHLESS_PRECISION, TRIGGERED_FULL_MASK);
        else if (rand == 4)
            caster->CastSpell(caster, SPELL_ROGUE_ROLL_THE_BONES_BURIED_TREASURE, TRIGGERED_FULL_MASK);
        else if (rand == 5)
            caster->CastSpell(caster, SPELL_ROGUE_ROLL_THE_BONES_SKULL_AND_CROSSBONES, TRIGGERED_FULL_MASK);
        else if (rand == 6)
            caster->CastSpell(caster, SPELL_ROGUE_ROLL_THE_BONES_TRUE_BEARING, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_rog_roll_the_bones::HandleCast);
    }
};

class spell_rog_roll_the_bones_skull_and_crossbones : public AuraScript
{
    PrepareAuraScript(spell_rog_roll_the_bones_skull_and_crossbones);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(eventInfo.GetActionTarget(), SPELL_ROGUE_SINISTER_STRIKE, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_roll_the_bones_skull_and_crossbones::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_roll_the_bones_true_bearing : public AuraScript
{
    PrepareAuraScript(spell_rog_roll_the_bones_true_bearing);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        uint32 reduction = aurEff->GetAmount() * GetCaster()->GetComboPoints();

        caster->ModifySpellCooldown(SPELL_ROGUE_MARKED_FOR_DEATH, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_BLADE_FLURRY, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_ADRENALINE_RUSH, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_BETWEEN_THE_EYES, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_SCIMITAR_RUSH, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_DREADBLADES, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_CAPTAIN_STRIKE, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_GRAPPLING_HOOK, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_KEEP_IT_ROLLING, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_ROLL_THE_BONES, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_SPRINT, reduction);
        caster->ModifySpellCooldown(SPELL_ROGUE_VANISH, reduction);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_roll_the_bones_true_bearing::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_keep_it_rolling : public SpellScript
{
    PrepareSpellScript(spell_rog_keep_it_rolling);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        uint32 increase = sSpellMgr->AssertSpellInfo(SPELL_ROGUE_KEEP_IT_ROLLING)->GetEffect(EFFECT_0).CalcValue(caster);

        if (Aura* grand = caster->GetAura(SPELL_ROGUE_ROLL_THE_BONES_GRAND_MELEE))
            grand->SetDuration(grand->GetDuration() + increase);
        if (Aura* bones = caster->GetAura(SPELL_ROGUE_ROLL_THE_BONES_BROADSIDE))
            bones->SetDuration(bones->GetDuration() + increase);
        if (Aura* precision = caster->GetAura(SPELL_ROGUE_ROLL_THE_BONES_RUTHLESS_PRECISION))
            precision->SetDuration(precision->GetDuration() + increase);
        if (Aura* treasure = caster->GetAura(SPELL_ROGUE_ROLL_THE_BONES_BURIED_TREASURE))
            treasure->SetDuration(treasure->GetDuration() + increase);
        if (Aura* skull = caster->GetAura(SPELL_ROGUE_ROLL_THE_BONES_SKULL_AND_CROSSBONES))
            skull->SetDuration(skull->GetDuration() + increase);
        if (Aura* bearing = caster->GetAura(SPELL_ROGUE_ROLL_THE_BONES_TRUE_BEARING))
            bearing->SetDuration(bearing->GetDuration() + increase);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_rog_keep_it_rolling::HandleCast);
    }
};

class spell_rog_relentless_attacks : public AuraScript
{
    PrepareAuraScript(spell_rog_relentless_attacks);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        uint32 limit = aurEff->GetAmount();

        caster->CastSpell(caster, TALENT_ROGUE_RELENTLESS_ATTACKS_STACK, TRIGGERED_FULL_MASK);

        if (Aura* stack = caster->GetAura(TALENT_ROGUE_RELENTLESS_ATTACKS_STACK))
            if (stack->GetStackAmount() >= limit)
            {
                caster->CastSpell(caster, TALENT_ROGUE_RELENTLESS_ATTACKS_COMBOPOINT, TRIGGERED_FULL_MASK);
                stack->Remove();
            }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_relentless_attacks::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_ruthless : public AuraScript
{
    PrepareAuraScript(spell_rog_ruthless);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        uint32 chance = aurEff->GetAmount() * caster->GetComboPoints();

        if (roll_chance_i(chance))
            caster->CastSpell(caster, TALENT_ROGUE_RUTHLESS_COMBOPOINT, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_ruthless::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_rog_improved_adrenaline_rush : public AuraScript
{
    PrepareAuraScript(spell_rog_improved_adrenaline_rush);

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (AuraEffect* aurEff = GetCaster()->GetAuraEffectOfRankedSpell(TALENT_ROGUE_IMPROVED_ADRENALINE_RUSH, EFFECT_0))
        {
            if (roll_chance_i(aurEff->GetAmount()))
                GetCaster()->CastSpell(GetCaster(), TALENT_ROGUE_IMPROVED_ADRENALINE_RUSH_PROC, TRIGGERED_FULL_MASK);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (AuraEffect* aurEff = GetCaster()->GetAuraEffectOfRankedSpell(TALENT_ROGUE_IMPROVED_ADRENALINE_RUSH, EFFECT_0))
        {
            if (roll_chance_i(aurEff->GetAmount()))
                GetCaster()->CastSpell(GetCaster(), TALENT_ROGUE_IMPROVED_ADRENALINE_RUSH_PROC, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_rog_improved_adrenaline_rush::HandleApply, EFFECT_0, SPELL_AURA_MOD_POWER_REGEN_PERCENT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_rog_improved_adrenaline_rush::HandleRemove, EFFECT_0, SPELL_AURA_MOD_POWER_REGEN_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_rog_audacity : public AuraScript
{
    PrepareAuraScript(spell_rog_audacity);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Aura* aura = GetCaster()->GetAura(SPELL_ROGUE_OPPORTUNITY))
        {
            Unit* caster = GetCaster();
            uint32 chance = aura->GetSpellInfo()->ProcChance;

            if (roll_chance_i(chance))
                caster->CastSpell(caster, TALENT_ROGUE_AUDACITY_PROC, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rog_audacity::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

void AddSC_rogue_spell_scripts()
{
    RegisterSpellScript(spell_rog_savage_combat);
    RegisterSpellScript(spell_rog_combat_potency);
    RegisterSpellScript(spell_rog_blade_flurry);
    RegisterSpellScript(spell_rog_cheat_death);
    RegisterSpellScript(spell_rog_deadly_poison);
    new spell_rog_killing_spree();
    RegisterSpellScript(spell_rog_nerves_of_steel);
    RegisterSpellScript(spell_rog_preparation);
    RegisterSpellScript(spell_rog_prey_on_the_weak);
    //RegisterSpellScript(spell_rog_rupture);
    RegisterSpellScript(spell_rog_shiv);
    RegisterSpellScript(spell_rog_tricks_of_the_trade);
    RegisterSpellScript(spell_rog_tricks_of_the_trade_proc);
    RegisterSpellScript(spell_rog_backstab);
    RegisterSpellScript(spell_rog_deadly_throw);
    RegisterSpellScript(spell_rog_eviscerate);
    RegisterSpellScript(spell_rog_vampiric_poison);
    RegisterSpellScript(spell_rog_envenom);
    RegisterSpellScript(spell_rog_shiv_poison);
    RegisterSpellScript(spell_rog_blade_flurry_new);
    RegisterSpellScript(spell_rog_blade_flurry_triggered);
    RegisterSpellScript(spell_rog_shadowstrike_activator);
    RegisterSpellScript(spell_rog_shadowstrike);
    RegisterSpellScript(spell_rog_premeditation);
    RegisterSpellScript(spell_rog_marked_for_death);
    RegisterSpellScript(spell_rog_poisoned_knife);
    RegisterSpellScript(spell_rog_deathmark);
    RegisterSpellScript(spell_rog_crimson_tempest);
    RegisterSpellScript(spell_rog_crimson_tempest_dot);
    RegisterSpellScript(spell_rog_serrated_bone_spike);
    RegisterSpellScript(spell_rog_blade_rush);
    RegisterSpellScript(spell_rog_serrated_bone_spike_death);
    RegisterSpellScript(spell_rog_kingsbane);
    RegisterSpellScript(spell_rog_kingsbane_combopoint);
    RegisterSpellScript(spell_rog_flagellation);
    RegisterSpellScript(spell_rog_black_powder);
    RegisterSpellScript(spell_rog_shadow_blade);
    RegisterSpellScript(spell_rog_vampiric_burst);
    RegisterSpellScript(spell_rog_counterattack);
    RegisterSpellScript(spell_rog_sinister_strike);
    RegisterSpellScript(spell_rog_secret_technique_AuraScript);
    new spell_rog_secret_technique();
    RegisterSpellScript(spell_rog_secret_technique_teacher);
    RegisterSpellScript(spell_rog_sinister_calling);
    RegisterSpellScript(spell_rog_forced_counter);
    RegisterSpellScript(spell_rog_amplifying_poison_replacer);
    RegisterSpellScript(spell_rog_duelists_reflex);
    RegisterSpellScript(spell_rog_master_duelist);
    RegisterSpellScript(spell_rog_cut_to_the_chase);
    RegisterSpellScript(spell_rog_between_the_eyes);
    RegisterSpellScript(spell_rog_scimitar_rush);
    RegisterSpellScript(spell_rog_scimitar_rush_target_select);
    RegisterSpellScript(spell_rog_dispatch);
    RegisterSpellScript(spell_rog_dreadblades);
    RegisterSpellScript(spell_rog_opportunity);
    RegisterSpellScript(spell_rogue_sea_of_strikes);
    RegisterSpellScript(spell_rog_roll_the_bones);
    RegisterSpellScript(spell_rog_roll_the_bones_skull_and_crossbones);
    RegisterSpellScript(spell_rog_roll_the_bones_true_bearing);
    RegisterSpellScript(spell_rog_keep_it_rolling);
    RegisterSpellScript(spell_rog_relentless_attacks);
    RegisterSpellScript(spell_rog_ruthless);
    RegisterSpellScript(spell_rog_improved_adrenaline_rush);
    RegisterSpellScript(spell_rog_audacity);
}
