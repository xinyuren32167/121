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
 * Scripts for spells with SPELLFAMILY_DEATHKNIGHT and SPELLFAMILY_GENERIC spells used by deathknight players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dk_".
 */

#include "PetDefines.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"
#include "Pet.h"

enum DeathKnightSpells
{
    SPELL_DK_DEATH_AND_DECAY_TRIGGER            = 52212,
    SPELL_DK_GLYPH_OF_SCOURGE_STRIKE            = 58642,
    SPELL_DK_WANDERING_PLAGUE_TRIGGER           = 50526,
    SPELL_DK_GLYPH_OF_THE_GHOUL                 = 58686,
    SPELL_SHADOWMOURNE_CHAOS_BANE_DAMAGE        = 71904,
    SPELL_SHADOWMOURNE_SOUL_FRAGMENT            = 71905,
    SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF          = 73422,
    SPELL_DK_ANTI_MAGIC_SHELL_TALENT            = 51052,
    SPELL_DK_BLACK_ICE_R1                       = 49140,
    SPELL_DK_BLOOD_BOIL_TRIGGERED               = 65658,
    SPELL_DK_BLOOD_GORGED                       = 50453,
    SPELL_DK_BLOOD_GORGED_HEAL                  = 50454,
    SPELL_DK_UNHOLY_PRESENCE                    = 48266,
    SPELL_DK_CORPSE_EXPLOSION_TRIGGERED         = 43999,
    SPELL_DK_CORPSE_EXPLOSION_VISUAL            = 51270,
    SPELL_DK_DEATH_COIL_DAMAGE                  = 47632,
    SPELL_DK_DEATH_COIL_HEAL                    = 47633,
    SPELL_DK_DEATH_STRIKE_HEAL                  = 45470,
    SPELL_DK_FROST_FEVER                        = 55095,
    SPELL_DK_BLOOD_PRESENCE                     = 48263,
    //SPELL_DK_BLOOD_PRESENCE_TRIGGERED           = 61261,
    SPELL_DK_GHOUL_EXPLODE                      = 47496,
    SPELL_DK_GLYPH_OF_DISEASE                   = 63334,
    SPELL_DK_GLYPH_OF_ICEBOUND_FORTITUDE        = 58625,
    SPELL_DK_IMPROVED_BLOOD_PRESENCE_R1         = 50365,
    SPELL_DK_IMPROVED_FROST_PRESENCE_R1         = 50384,
    SPELL_DK_IMPROVED_UNHOLY_PRESENCE_R1        = 50391,
    SPELL_DK_IMPROVED_BLOOD_PRESENCE_TRIGGERED  = 63611,
    SPELL_DK_IMPROVED_UNHOLY_PRESENCE_TRIGGERED = 63622,
    SPELL_DK_ITEM_SIGIL_VENGEFUL_HEART          = 64962,
    SPELL_DK_ITEM_T8_MELEE_4P_BONUS             = 64736,
    SPELL_DK_MASTER_OF_GHOULS                   = 52143,
    SPELL_DK_BLOOD_PLAGUE                       = 59879,
    SPELL_DK_RAISE_DEAD_USE_REAGENT             = 48289,
    SPELL_DK_RUNIC_POWER_ENERGIZE               = 49088,
    SPELL_DK_SCENT_OF_BLOOD                     = 50422,
    SPELL_DK_SCOURGE_STRIKE_TRIGGERED           = 70890,
    SPELL_DK_FROST_PRESENCE                     = 48265,
    //SPELL_DK_FROST_PRESENCE_TRIGGERED           = 49772,
    SPELL_DK_WILL_OF_THE_NECROPOLIS_TALENT_R1   = 49189,
    SPELL_DK_WILL_OF_THE_NECROPOLIS_AURA_R1     = 52284,

    //NEW STUFF
    SPELL_DK_DEATH_PACT                         = 48473,
    SPELL_DK_HEALING_ABSORB_DEATH_PACT          = 80301,
    SPELL_DK_GLACIAL_ADVANCE_DAMAGE             = 80305,
    SPELL_DK_GLACIAL_CHILL_STREAK_DAMAGE_BOUNCE = 80310,
    SPELL_DK_GLACIAL_CHILL_STREAK_TICK          = 80309,
    SPELL_DK_FROSTWHYRM                         = 80311,
    SPELL_DK_FROST_FEVER_RUNIC                  = 59921,
    SPELL_DK_LICHBORNE_LEECH                    = 80316,
    SPELL_DK_BLOOD_TAP                          = 45529,
    SPELL_DK_BONE_SHIELD                        = 49222,
    SPELL_DK_PLAGUE_STRIKE                      = 49921,
    SPELL_DK_OBLITERATE                         = 51425,
    SPELL_DK_FESTERING_WOUND                    = 80322,
    SPELL_DK_FESTERING_WOUND_PROC               = 80323,
    SPELL_DK_SCOURGE_STRIKE_SHADOW              = 80325,
    SPELL_DK_SUMMON_GARGOYLE_ENERGY             = 80326,
    SPELL_DK_SUMMON_GARGOYLE_DAMAGE_BUFF        = 80327,
    SPELL_DK_SUMMON_GARGOYLE_LISTENER           = 80328,
    SPELL_DK_VIRULENT_PLAGUE_PROC               = 80333,
    SPELL_DK_BREATH_OF_SINDRAGOSA               = 80314,
    SPELL_DK_DEATH_STRIKE                       = 49924,
    SPELL_DK_IMPROVED_DEATH_STRIKE              = 62905,
    SPELL_DK_SACRIFICAL_PACT_BASE               = 80335,
    SPELL_DK_SACRIFICAL_PACT_EXPLOSION          = 80336,
    SPELL_DK_RUNIC_CORRUPTION                   = 80342,
    SPELL_DK_SOUL_REAPER_PERIODIC               = 80343,
    SPELL_DK_RUNE_OF_SANGUINATION               = 80345,
    SPELL_DK_RUNE_OF_SANGUINATION_HEAL          = 80346,
    SPELL_DK_RUNE_OF_SPELLWARDING_SHIELD        = 80347,
    SPELL_DK_RUNE_APOCALYPSE_PET_AURA           = 80350,
    SPELL_DK_RUNE_APOCALYPSE_DEATH              = 80351,
    SPELL_DK_RUNE_APOCALYPSE_WAR                = 80352,
    SPELL_DK_RUNE_APOCALYPSE_FAMINE             = 80353,
    SPELL_DK_RUNE_APOCALYPSE_PESTILENCE         = 80354,
    SPELL_DK_DEATHS_CARESS                      = 80361,
    SPELL_DK_MARROWREND                         = 80362,
    SPELL_DK_GOREFIENDS_GRASP_PULL              = 80364,
    SPELL_DK_TOMBSTONE_BUFF                     = 80366,
    SPELL_DK_BONESTORM                          = 80367,
    SPELL_DK_BONESTORM_HEAL                     = 80369,
    SPELL_DK_DARK_TRANSFORMATION_DAMAGE         = 80401,
    SPELL_DK_DARK_TRANSFORMATION_POWERUP        = 80402,
    SPELL_DK_EPIDEMIC_SINGLE                    = 80376,
    SPELL_DK_EPIDEMIC_AOE                       = 80377,
    SPELL_DK_VIRULENT_PLAGUE                    = 80332,
    SPELL_DK_BLOOD_PRESENCE_ADDITIONAL          = 61261,
    SPELL_DK_FROST_PRESENCE_ADDITIONAL          = 49772,
    SPELL_DK_UNHOLY_PRESENCE_HEAL               = 50475,
    SPELL_DK_MIGHT_OF_MOGRAINE                  = 49023,
    SPELL_DK_DANCING_RUNE_WEAPON                = 49028,
    SPELL_DK_IMPROVED_BLOODWORMS                = 80391,
    SPELL_DK_IMPROVED_BLOODWORMS_DEATH          = 80390,
    SPELL_DK_IMPROVED_BLOODWORMS_HEAL           = 80394,
    SPELL_DK_IMPROVED_BLOODWORMS_OWNER          = 80395,
    SPELL_DK_IMPROVED_BLOODWORMS_R1             = 80391,
    SPELL_DK_IMPROVED_BLOODWORMS_R2             = 80392,
    SPELL_DK_IMPROVED_BLOODWORMS_R3             = 80393,
	SPELL_DK_ANNIHILATION                       = 51468,
    SPELL_DK_DEATHCHILL_TALENT                  = 80397,
    SPELL_DK_DEATHCHILL_AOE                     = 80398,
    SPELL_DK_DEATHCHILL_SINGLE                  = 80399,
    SPELL_DK_REMORSELESS_WINTER                 = 80306,
    SPELL_DK_CHAINS_OF_ICE                      = 80334,
    SPELL_DK_FROSTSCYTHE                        = 80302,
    SPELL_DK_RIME_PROC                          = 59052,
	SPELL_DK_ARMY_OF_THE_DEAD                   = 42651,
    SPELL_DK_SENSITIZATION_FROST                = 80412,
    SPELL_DK_SENSITIZATION_SHADOW               = 80413,
    SPELL_DK_SENSITIZATION_FIRE                 = 80414,
    SPELL_DK_SENSITIZATION_NATURE               = 80415,
    SPELL_DK_SENSITIZATION_HOLY                 = 80416,
    SPELL_DK_SENSITIZATION_ARCANE               = 80417,
    SPELL_DK_CONTAGIOUS_TARGET_INCREASE         = 80421,
    SPELL_DK_DEATH_AND_DECAY                    = 49938,
    SPELL_DK_DEFILE                             = 80405,
    SPELL_DK_DEATH_STRIKE_THASSARIAN            = 66953,
    SPELL_DK_FROST_STRIKE_THASSARIAN            = 66962,
    SPELL_DK_OBLITERATE_THASSARIAN              = 66974,
    SPELL_DK_PLAGUE_STRIKE_THASSARIAN           = 66992,
    SPELL_DK_FROSTSCYTHE_THASSARIAN             = 80407,
    SPELL_DK_FROST_STRIKE                       = 55268,
    MASTERY_DK_UNHOLY                           = 600005,
    NPC_CONTAGION_AREA                          = 500508,
    SPELL_DK_DEATHS_EMBRACE                     = 87001,
    SPELL_DK_DEATHS_EMBRACE_HEAL                = 87002,
    SPELL_DK_DEATHS_EMBRACE_LISTENER            = 87003,
    SPELL_DK_EBON_RENEWAL_HEAL                  = 87005,
    SPELL_DK_LEECHING_STRIKE_ENERGY             = 87011,
    SPELL_DK_NECROTIC_BLESSING_HEAL             = 87013,
    SPELL_DK_SOUL_BARRIER_HEAL                  = 87015,
    SPELL_DK_SOUL_LINK                          = 87016,
    SPELL_DK_SOUL_LINK_LISTENER                 = 87017,
    SPELL_DK_SOUL_LINK_HEAL                     = 87018,
    SPELL_DK_SOULBOLT_HEAL                      = 87021,
    SPELL_DK_SOUL_PRESENCE                      = 87000,

    //MASTERY
    SPELL_DK_LIFE_AND_DEATH                     = 590005,
    SPELL_DK_LIFE_AND_DEATH_SHIELD              = 590006,

    //TALENT
    TALENT_DK_NECROTIC_PRESENCE_PROC            = 87056,
    TALENT_DK_IMPROVED_LEECHING_STRIKE_PROC     = 87081,
    TALENT_DK_NECROTIC_VENGEANCE_HEAL_BUFF      = 87090,
    TALENT_DK_NECROTIC_VENGEANCE_DAMAGE_BUFF    = 87091,
    TALENT_DK_MERCIFUL_COMBAT                   = 87092,
    TALENT_DK_SUDDEN_SALVATION_LIFEBOLT         = 87108,
    TALENT_DK_DEATH_AND_RENEW                   = 87109,
    TAlENT_DK_DEATH_AND_RENEW_HEAL              = 87110,
    TALENT_DK_NECROTIC_PROTECTION_PROC          = 87114,

    //RUNE
    RUNE_DK_SPLINTERING_SHIELD_PROC             = 600572,
};

enum DeathKnightSpellIcons
{
    DK_ICON_ID_IMPROVED_DEATH_STRIKE            = 2751
};

enum Misc
{
    NPC_DK_GHOUL                                = 26125
};

class spell_dk_death_pact: public SpellScript
{
    PrepareSpellScript(spell_dk_death_pact);

    void HandleCast()
    {
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DK_DEATH_PACT);
        int32 hpPct = value->GetEffect(EFFECT_0).CalcValue(GetCaster());

        uint32 amount = GetCaster()->CountPctFromMaxHealth(hpPct);
        GetCaster()->CastCustomSpell(SPELL_DK_HEALING_ABSORB_DEATH_PACT, SPELLVALUE_BASE_POINT0, amount, GetCaster());
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dk_death_pact::HandleCast);
    }
};

class spell_dk_breath_of_sindragosa : public AuraScript
{
    PrepareAuraScript(spell_dk_breath_of_sindragosa);


    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        uint32 cost = GetSpellInfo()->CalcPowerCost(caster, eventInfo.GetSchoolMask());

        if (caster->GetPower(POWER_RUNIC_POWER) <= cost) {
            caster->RemoveAura(SPELL_DK_BREATH_OF_SINDRAGOSA);
            caster->ModifyPower(POWER_RUNIC_POWER, 20);
            return false;
        }

        return true;
    }

    void PeriodicTick(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        uint32 cost = GetSpellInfo()->ManaCost;
        caster->ModifyPower(POWER_RUNIC_POWER, -cost);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        caster->ModifyPower(POWER_RUNIC_POWER, 20);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_breath_of_sindragosa::CheckProc);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_breath_of_sindragosa::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        OnEffectProc += AuraEffectProcFn(spell_dk_breath_of_sindragosa::HandleProc, EFFECT_0, SPELL_AURA_MOD_PERCENT_STAT);
    }
};

class spell_dk_chill_streak : public AuraScript
{
    PrepareAuraScript(spell_dk_chill_streak);

    void HandleEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {

        Unit* caster = GetCaster();
        Unit* target = GetTarget();

        if (!caster || caster->isDead())
            return;

        Unit* originalCaster = aurEff->GetBase()->GetUnitOwner();
        int32 currentTick = aurEff->GetBase()->GetEffect(EFFECT_0)->GetAmount();
        float range = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        int32 maxTick = aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount();

        if (currentTick == maxTick)
            return;

        Creature* creature = target->FindNearestCreatureWithoutEntry(target->GetGUID(), range, true);

        if (!creature)
            return;

        originalCaster->CastSpell(creature, SPELL_DK_GLACIAL_CHILL_STREAK_DAMAGE_BOUNCE, true, nullptr, nullptr, caster->GetGUID());
        originalCaster->CastCustomSpell(SPELL_DK_GLACIAL_CHILL_STREAK_TICK, SPELLVALUE_BASE_POINT0, currentTick + 1, creature, true, nullptr, nullptr, caster->GetGUID());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_chill_streak::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class npc_dk_spell_glacial_advance : public CreatureScript
{
public:
    npc_dk_spell_glacial_advance() : CreatureScript("npc_dk_spell_glacial_advance") { }

    struct npc_dk_spell_glacial_advanceAI : public ScriptedAI
    {
        npc_dk_spell_glacial_advanceAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 timerLastSummonSpick = 100;
        Position oldpos;
        float tick = 1;
        void Reset() override
        {
            me->CombatStop(true);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
            oldpos = me->GetPosition();
        }

        void UpdateAI(uint32 diff) override
        {
            Position pos = me->GetFirstCollisionPosition(1.f + tick, 0);

            if(!pos.IsPositionValid())
                me->DespawnOrUnsummon();

            if (tick > 100)
                me->DespawnOrUnsummon();

            if (timerLastSummonSpick >= 150) {
                timerLastSummonSpick = 0;
                Unit* owner = me->GetOwner();
                GameObject* go = owner->SummonGameObject(188537, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 0, 0, 0, 0, 0, 1000, false, GO_SUMMON_TIMED_DESPAWN);
                go->DespawnOrUnsummon(150ms);
                owner->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_DK_GLACIAL_ADVANCE_DAMAGE, true);
                tick += 5.0f;
            }
            timerLastSummonSpick += diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_dk_spell_glacial_advanceAI(creature);
    }
};

class npc_dk_spell_frostwyrm : public CreatureScript
{
public:
    npc_dk_spell_frostwyrm() : CreatureScript("npc_dk_spell_frostwyrm") { }

    struct npc_dk_spell_frostwyrmAI : public ScriptedAI
    {
        npc_dk_spell_frostwyrmAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 update = 250;

        void Reset() override
        {
            Position pos = me->GetFirstCollisionPosition(40.0f, 0);
            me->GetMotionMaster()->MovePoint(0, pos);
            me->CombatStop(true);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(uint32 diff) override
        {
            if (update >= 250) {
                if (Unit* owner = me->ToTempSummon()->GetSummonerUnit()) {
                   me->CastSpell(me, SPELL_DK_FROSTWHYRM, true, nullptr, nullptr, owner->GetGUID());
                }
                update = 0;
            }

            update += diff;
        }

        void MovementInform(uint32 /*type*/, uint32 id) override {
            if (id == 0) {
                me->DespawnOrUnsummon();
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_dk_spell_frostwyrmAI(creature);
    }
};

class spell_dk_frostwyrm : public SpellScript
{
    PrepareSpellScript(spell_dk_frostwyrm);

    void HandleSummon()
    {
        Position pos = *GetCaster();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = GetCaster()->SummonCreature(500506, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ() + 5.f, pos.GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, GetSpellInfo()->GetDuration(), properties);
        if (!summon)
            return;

        summon->SetOwnerGUID(GetCaster()->GetGUID());
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        summon->SetReactState(REACT_PASSIVE);
        summon->SetTarget();
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dk_frostwyrm::HandleSummon);
    }
};

class spell_dk_pillar_of_frost : public AuraScript
{
    PrepareAuraScript(spell_dk_pillar_of_frost);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        uint32 runeSpent = 0;

        if (SpellInfo const* spell = eventInfo.GetSpellInfo())
        {
            SpellRuneCostEntry const* src = sSpellRuneCostStore.LookupEntry(spell->RuneCostID);
            if (src)
            {
                runeSpent = src->RuneCost[RUNE_BLOOD]; // we always take the blood rune cost because "everything" cost a blood rune.
            }
        }

        int32 baseStrength = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        int32 bonusStrength = GetAura()->GetEffect(EFFECT_0)->GetAmount();

        if (runeSpent < 0)
            return;

        int32 newAmount = baseStrength + (bonusStrength * runeSpent);

        GetAura()->GetEffect(EFFECT_1)->ChangeAmount(newAmount, true, true);
    }


    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_pillar_of_frost::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_blood_strike : public SpellScript
{
    PrepareSpellScript(spell_dk_blood_strike);

    void HandleDamage(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(49930);
        uint32 diseaseBonus = value->GetEffect(EFFECT_2).CalcValue(GetCaster());

        float diseasePct = diseaseBonus / 10;

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

            uint32 count = target->GetDiseasesByCaster(GetCaster()->GetGUID());
            int32 bp = int32(count * diseasePct);
            AddPct(damage, bp);
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_blood_strike::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 50526 - Wandering Plague
class spell_dk_wandering_plague : public SpellScript
{
    PrepareSpellScript(spell_dk_wandering_plague);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        static const AuraType ccAuras[] =
        {
            SPELL_AURA_MOD_CONFUSE,
            SPELL_AURA_MOD_FEAR,
            SPELL_AURA_MOD_STUN,
            SPELL_AURA_MOD_ROOT,
            SPELL_AURA_TRANSFORM,
            SPELL_AURA_NONE
        };

        for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end();)
        {
            Unit* target = (*itr)->ToUnit();
            if (!target)
            {
                targets.erase(itr++);
                continue;
            }

            bool skip = false;
            for (uint8 index = 0; !skip && ccAuras[index] != SPELL_AURA_NONE; ++index)
            {
                Unit::AuraEffectList const& auras = target->GetAuraEffectsByType(ccAuras[index]);
                for (Unit::AuraEffectList::const_iterator i = auras.begin(); i != auras.end(); ++i)
                    if ((*i)->GetAmount())
                    {
                        skip = true;
                        break;
                    }
            }

            if (skip)
                targets.erase(itr++);
            else
                ++itr;
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_wandering_plague::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// 61999 - Raise Ally
class spell_dk_raise_ally : public SpellScript
{
    PrepareSpellScript(spell_dk_raise_ally);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Player* unitTarget = GetHitPlayer())
        {
            unitTarget->CastSpell(unitTarget, GetEffectValue(), true);
            if (Unit* ghoul = unitTarget->GetCharm())
            {
                //health, mana, armor and resistance
                PetLevelInfo const* pInfo = sObjectMgr->GetPetLevelInfo(ghoul->GetEntry(), ghoul->getLevel());
                if (pInfo)                                      // exist in DB
                {
                    ghoul->SetCreateHealth(pInfo->health);
                    ghoul->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, pInfo->health);
                    ghoul->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, float(pInfo->armor));
                    for (uint8 stat = 0; stat < MAX_STATS; ++stat)
                        ghoul->SetCreateStat(Stats(stat), float(pInfo->stats[stat]));
                }

                ghoul->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, float(ghoul->getLevel() - (ghoul->getLevel() / 4)));
                ghoul->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, float(ghoul->getLevel() + (ghoul->getLevel() / 4)));

                // Avoidance, Night of the Dead
                if (Aura* aur = ghoul->AddAura(62137, ghoul))
                    if (AuraEffect* aurEff = GetCaster()->GetAuraEffect(SPELL_AURA_ADD_FLAT_MODIFIER, SPELLFAMILY_DEATHKNIGHT, 2718, 1))
                        if (aur->GetEffect(0))
                            aur->GetEffect(0)->SetAmount(-aurEff->GetSpellInfo()->Effects[EFFECT_2].CalcValue());

                // Hit / Expertise scalling, warlock / hunter pets have this by default
                ghoul->AddAura(SPELL_HUNTER_PET_SCALING_04, ghoul);

                // DK Ghoul haste refresh
                float val = (GetCaster()->m_modAttackSpeedPct[BASE_ATTACK] - 1.0f) * 100.0f;
                ghoul->m_modAttackSpeedPct[BASE_ATTACK] = GetCaster()->m_modAttackSpeedPct[BASE_ATTACK];
                ghoul->SetFloatValue(UNIT_FIELD_BASEATTACKTIME, 2000.0f);
                ghoul->ApplyPercentModFloatValue(UNIT_FIELD_BASEATTACKTIME, val, true); // we want to reduce attack time

                // Strength + Stamina
                for (uint8 i = STAT_STRENGTH; i <= STAT_STAMINA; ++i)
                {
                    Stats stat = Stats(i);
                    if (stat != STAT_STRENGTH && stat != STAT_STAMINA)
                        continue;

                    float value = 0.0f;
                    float mod = (stat == STAT_STAMINA ? 0.3f : 0.7f);

                    // Check just if owner has Ravenous Dead since it's effect is not an aura
                    AuraEffect const* aurEff = GetCaster()->GetAuraEffect(SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, SPELLFAMILY_DEATHKNIGHT, 3010, 0);
                    if (aurEff)
                    {
                        SpellInfo const* spellInfo = aurEff->GetSpellInfo();                                                 // Then get the SpellProto and add the dummy effect value
                        AddPct(mod, spellInfo->Effects[EFFECT_1].CalcValue());                                              // Ravenous Dead edits the original scale
                    }
                    // Glyph of the Ghoul
                    aurEff = GetCaster()->GetAuraEffect(SPELL_DK_GLYPH_OF_THE_GHOUL, EFFECT_0);
                    if (aurEff)
                        mod += CalculatePct(1.0f, aurEff->GetAmount());                                                    // Glyph of the Ghoul adds a flat value to the scale mod

                    value = float(GetCaster()->GetStat(stat)) * mod;
                    value = ghoul->GetTotalStatValue(stat, value);
                    ghoul->SetStat(stat, int32(value));
                    ghoul->ApplyStatBuffMod(stat, value, true);
                }

                // Attack Power
                ghoul->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, 589 + ghoul->GetStat(STAT_STRENGTH) + ghoul->GetStat(STAT_AGILITY));
                ghoul->SetInt32Value(UNIT_FIELD_ATTACK_POWER, (int32)ghoul->GetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE) * ghoul->GetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_PCT));
                ghoul->SetInt32Value(UNIT_FIELD_ATTACK_POWER_MODS, (int32)ghoul->GetModifierValue(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE));
                ghoul->SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, ghoul->GetModifierValue(UNIT_MOD_ATTACK_POWER, TOTAL_PCT) - 1.0f);

                // Health
                ghoul->SetModifierValue(UNIT_MOD_HEALTH, TOTAL_VALUE, (ghoul->GetStat(STAT_STAMINA) - ghoul->GetCreateStat(STAT_STAMINA)) * 10.0f);

                // Power Energy
                ghoul->SetModifierValue(UnitMods(UNIT_MOD_POWER_START + static_cast<uint8>(POWER_ENERGY)), BASE_VALUE, ghoul->GetCreatePowers(POWER_ENERGY));
                ghoul->UpdateAllStats();
                ghoul->SetFullHealth();

                // Aura Immunities
                ghoul->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_MELEE_RANGED_HASTE, true, SPELL_BLOCK_TYPE_POSITIVE);
                ghoul->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MELEE_SLOW, true, SPELL_BLOCK_TYPE_POSITIVE);
                ghoul->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_STAT, true, SPELL_BLOCK_TYPE_POSITIVE);
                ghoul->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, true, SPELL_BLOCK_TYPE_POSITIVE);
                ghoul->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ATTACK_POWER, true, SPELL_BLOCK_TYPE_POSITIVE);
                ghoul->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ATTACK_POWER_PCT, true, SPELL_BLOCK_TYPE_POSITIVE);
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_raise_ally::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 46619 - Raise Ally
class spell_dk_raise_ally_trigger : public AuraScript
{
    PrepareAuraScript(spell_dk_raise_ally_trigger);

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* charm = GetUnitOwner()->GetCharm())
            if (GetSpellInfo()->Effects[EFFECT_0].MiscValue >= 0 && charm->GetEntry() == uint32(GetSpellInfo()->Effects[EFFECT_0].MiscValue))
                charm->ToCreature()->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_raise_ally_trigger::HandleEffectRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 43263 - Ghoul Taunt
class spell_dk_aotd_taunt : public SpellScript
{
    PrepareSpellScript(spell_dk_aotd_taunt);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end();)
        {
            // ignore bosses
            if (Creature* cr = (*itr)->ToCreature())
                if (cr->isWorldBoss())
                {
                    targets.erase(itr++);
                    continue;
                }

            ++itr;
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_aotd_taunt::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// 52143 - Master of Ghouls
class spell_dk_master_of_ghouls : public AuraScript
{
    PrepareAuraScript(spell_dk_master_of_ghouls);

    void HandleEffectApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (target->GetTypeId() == TYPEID_PLAYER)
            target->ToPlayer()->SetShowDKPet(true);
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (target->GetTypeId() == TYPEID_PLAYER)
            target->ToPlayer()->SetShowDKPet(false);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dk_master_of_ghouls::HandleEffectApply, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_master_of_ghouls::HandleEffectRemove, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

// 45524 - Chains of Ice
class spell_dk_chains_of_ice : public SpellScript
{
    PrepareSpellScript(spell_dk_chains_of_ice);

    void HandleAfterCast()
    {
        if (Unit* target = GetExplTargetUnit())
        {
            std::list<TargetInfo> const* targetsInfo = GetSpell()->GetUniqueTargetInfo();
            for (std::list<TargetInfo>::const_iterator ihit = targetsInfo->begin(); ihit != targetsInfo->end(); ++ihit)
                if (ihit->missCondition == SPELL_MISS_NONE && ihit->targetGUID == target->GetGUID())
                    GetCaster()->CastSpell(target, 55095 /*SPELL_FROST_FEVER*/, true);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dk_chains_of_ice::HandleAfterCast);
    }
};

class spell_dk_chains_of_ice_aura : public AuraScript
{
    PrepareAuraScript(spell_dk_chains_of_ice_aura);

    void HandlePeriodic(AuraEffect* aurEff)
    {
        // Get 0 effect aura
        if (AuraEffect* slow = GetAura()->GetEffect(0))
        {
            int32 newAmount = slow->GetAmount() + aurEff->GetAmount();
            if (newAmount > 0)
                newAmount = 0;
            slow->ChangeAmount(newAmount);
        }
    }

    void Register() override
    {
        OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_dk_chains_of_ice_aura::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 50452 - Bloodworm
class spell_dk_bloodworms : public SpellScript
{
    PrepareSpellScript(spell_dk_bloodworms);

    void HandleSummon(SpellEffIndex /*effIndex*/)
    {
        SetEffectValue(1);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_dk_bloodworms::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
    }
};

// 49206 - Summon Gargoyle
class spell_dk_summon_gargoyle : public SpellScript
{
    PrepareSpellScript(spell_dk_summon_gargoyle);

    void SetDest(SpellDestination& dest)
    {
        // Adjust effect summon position
        if (GetCaster()->IsWithinLOS(dest._position.GetPositionX(), dest._position.GetPositionY(), dest._position.GetPositionZ() + 15.0f))
            dest._position.m_positionZ += 15.0f;
    }

    void Register() override
    {
        OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_dk_summon_gargoyle::SetDest, EFFECT_0, TARGET_DEST_CASTER_FRONT_LEFT);
    }
};

// 49217 - Wandering Plague
class spell_dk_wandering_plague_aura : public AuraScript
{
    PrepareAuraScript(spell_dk_wandering_plague_aura);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo || !eventInfo.GetActionTarget() || !eventInfo.GetDamageInfo() || !eventInfo.GetActor())
            return false;

        if (!roll_chance_f(eventInfo.GetActor()->GetUnitCriticalChance(BASE_ATTACK, eventInfo.GetActionTarget())))
            return false;

        return !eventInfo.GetActor()->HasSpellCooldown(SPELL_DK_WANDERING_PLAGUE_TRIGGER);
    }

    // xinef: prevent default proc with castItem passed, which applies 30 sec cooldown to procing of the aura
    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        eventInfo.GetActor()->AddSpellCooldown(SPELL_DK_WANDERING_PLAGUE_TRIGGER, 0, 1000);
        eventInfo.GetActor()->CastCustomSpell(SPELL_DK_WANDERING_PLAGUE_TRIGGER, SPELLVALUE_BASE_POINT0, CalculatePct<int32, int32>(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()), eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_wandering_plague_aura::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_wandering_plague_aura::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 53365 - Unholy Strength
class spell_dk_rune_of_the_fallen_crusader : public SpellScript
{
    PrepareSpellScript(spell_dk_rune_of_the_fallen_crusader);

    void RecalculateDamage()
    {
        std::list<TargetInfo>* targetsInfo = GetSpell()->GetUniqueTargetInfo();
        for (std::list<TargetInfo>::iterator ihit = targetsInfo->begin(); ihit != targetsInfo->end(); ++ihit)
            if (ihit->targetGUID == GetCaster()->GetGUID())
                ihit->crit = roll_chance_f(GetCaster()->GetFloatValue(PLAYER_CRIT_PERCENTAGE));
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_dk_rune_of_the_fallen_crusader::RecalculateDamage);
    }
};

// 49222 - Bone Shield
class spell_dk_bone_shield : public AuraScript
{
    PrepareAuraScript(spell_dk_bone_shield);

    Aura* GetSplinteringAura(Unit* caster)
    {
        for (size_t i = 600566; i < 600572; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    Aura* GetRunicAura(Unit* caster)
    {
        for (size_t i = 600573; i < 600579; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    Aura* GetInsatiableAura(Unit* caster)
    {
        for (size_t i = 600579; i < 600585; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    void HandleProc(ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        ModStackAmount(-1);

        Player* caster = GetCaster()->ToPlayer();
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DK_BLOOD_TAP);
        uint32 reduction = value->GetEffect(EFFECT_1).CalcValue(GetCaster());

        caster->ModifySpellCooldown(SPELL_DK_BLOOD_TAP, reduction);

        if (AuraEffect const* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_DK_MIGHT_OF_MOGRAINE, EFFECT_1))
        {
            int32 dancingReduction = aurEff->GetAmount(); 
            caster->ModifySpellCooldown(SPELL_DK_DANCING_RUNE_WEAPON, dancingReduction);
        }

        if (Aura* splinteringAura = GetSplinteringAura(caster))
        {
            int32 amount = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), splinteringAura->GetEffect(EFFECT_0)->GetAmount());
            caster->CastCustomSpell(RUNE_DK_SPLINTERING_SHIELD_PROC, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        }

        if (Aura* runicAura = GetRunicAura(caster))
            caster->EnergizeBySpell(caster, SPELL_DK_BONE_SHIELD, runicAura->GetEffect(EFFECT_0)->GetAmount(), POWER_RUNIC_POWER);

        if (Aura* insatiableAura = GetInsatiableAura(caster))
            caster->ModifySpellCooldown(SPELL_DK_DANCING_RUNE_WEAPON, insatiableAura->GetEffect(EFFECT_0)->GetAmount());
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_dk_bone_shield::HandleProc);
    }
};

class spell_dk_bone_shield_calculation : public AuraScript
{
    PrepareAuraScript(spell_dk_bone_shield_calculation);

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* caster = GetCaster())
        {
            SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DK_BONE_SHIELD);
            int32 scaling = value->GetEffect(EFFECT_1).CalcValue(GetCaster());
            uint32 str = caster->GetStat(STAT_STRENGTH);
            amount = ApplyPct(str, scaling);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_bone_shield_calculation::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
    }
};

// 51209 - Hungering Cold
class spell_dk_hungering_cold : public AuraScript
{
    PrepareAuraScript(spell_dk_hungering_cold);

    void HandleProc(ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && (!eventInfo.GetSpellInfo() || eventInfo.GetSpellInfo()->Dispel != DISPEL_DISEASE))
            SetDuration(0);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_dk_hungering_cold::HandleProc);
    }
};

// -49219 - Blood-Caked Blade
class spell_dk_blood_caked_blade : public AuraScript
{
    PrepareAuraScript(spell_dk_blood_caked_blade);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActionTarget() && eventInfo.GetActionTarget()->IsAlive() && eventInfo.GetActor();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        eventInfo.GetActor()->CastSpell(eventInfo.GetActionTarget(), aurEff->GetSpellInfo()->Effects[EFFECT_0].TriggerSpell, true, nullptr, aurEff);

        // Xinef: Shadowmourne hack (blood-caked blade trigger proc disabled...)
        if (roll_chance_i(75) && eventInfo.GetActor()->FindMap() && !eventInfo.GetActor()->FindMap()->IsBattlegroundOrArena() && eventInfo.GetActor()->HasAura(71903) && !eventInfo.GetActor()->HasAura(SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF))
        {
            eventInfo.GetActor()->CastSpell(eventInfo.GetActor(), SPELL_SHADOWMOURNE_SOUL_FRAGMENT, true);

            // this can't be handled in AuraScript of SoulFragments because we need to know victim
            if (Aura* soulFragments = eventInfo.GetActor()->GetAura(SPELL_SHADOWMOURNE_SOUL_FRAGMENT))
            {
                if (soulFragments->GetStackAmount() >= 10)
                {
                    eventInfo.GetActor()->CastSpell(eventInfo.GetActor(), SPELL_SHADOWMOURNE_CHAOS_BANE_DAMAGE, true, nullptr);
                    soulFragments->Remove();
                }
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_blood_caked_blade::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_blood_caked_blade::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 49028 - Dancing Rune Weapon
class spell_dk_dancing_rune_weapon : public AuraScript
{
    PrepareAuraScript(spell_dk_dancing_rune_weapon);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetActor() || !eventInfo.GetActionTarget() || !eventInfo.GetActionTarget()->IsAlive() || eventInfo.GetActor()->GetTypeId() != TYPEID_PLAYER)
            return false;

        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo)
            return true;
         
        // Death Coil exception, Check if spell is from spellbook
        if (spellInfo->Id != SPELL_DK_DEATH_COIL_DAMAGE && !eventInfo.GetActor()->ToPlayer()->HasActiveSpell(spellInfo->Id))
            return false;

        // Can't cast raise dead/ally, death grip, dark command, death pact, death and decay, anti-magic shell
        if (spellInfo->SpellFamilyFlags.HasFlag(0x20A1220, 0x10000000, 0x0))
            return false;

        // AoE can be cast only once
        if (spellInfo->IsTargetingArea() && eventInfo.GetActor() != eventInfo.GetActionTarget())
            return false;

        // No spells with summoning
        if (spellInfo->HasEffect(SPELL_EFFECT_SUMMON))
            return false;

        // No Positive Spells
        if (spellInfo->IsPositive())
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        Unit* player = eventInfo.GetActor();
        Unit* target = eventInfo.GetActionTarget();
        Unit* dancingRuneWeapon = nullptr;
        for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
            if (int32((*itr)->GetEntry()) == GetSpellInfo()->Effects[EFFECT_0].MiscValue)
            {
                dancingRuneWeapon = *itr;
                break;
            }

        if (!dancingRuneWeapon)
            return;

        dancingRuneWeapon->SetOrientation(dancingRuneWeapon->GetAngle(target));
        if (SpellInfo const* procSpell = eventInfo.GetSpellInfo())
        {
            // xinef: ugly hack
            if (!procSpell->IsAffectingArea())
                GetUnitOwner()->SetFloatValue(UNIT_FIELD_COMBATREACH, 10.0f);
            dancingRuneWeapon->CastSpell(target, procSpell->Id, true, nullptr, aurEff, dancingRuneWeapon->GetGUID());
            GetUnitOwner()->SetFloatValue(UNIT_FIELD_COMBATREACH, 0.01f);
        }
        else if (eventInfo.GetDamageInfo())
        {
            target = player->GetMeleeHitRedirectTarget(target);
            CalcDamageInfo damageInfo;
            player->CalculateMeleeDamage(target, &damageInfo, eventInfo.GetDamageInfo()->GetAttackType());
            for (uint8 i = 0; i < MAX_ITEM_PROTO_DAMAGES; ++i)
            {
                Unit::DealDamageMods(target, damageInfo.damages[i].damage, &damageInfo.damages[i].absorb);
            }

            damageInfo.attacker = dancingRuneWeapon;
            dancingRuneWeapon->SendAttackStateUpdate(&damageInfo);
            dancingRuneWeapon->DealMeleeDamage(&damageInfo, true);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_dancing_rune_weapon::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_dancing_rune_weapon::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
    }
};

// 53160 - Dancing Rune Weapon Visual
class spell_dk_dancing_rune_weapon_visual : public AuraScript
{
    PrepareAuraScript(spell_dk_dancing_rune_weapon_visual);

    void HandleEffectApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        PreventDefaultAction();
        if (Unit* owner = GetUnitOwner()->ToTempSummon()->GetSummonerUnit())
        {
            GetUnitOwner()->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, owner->GetUInt32Value(PLAYER_VISIBLE_ITEM_16_ENTRYID));
            GetUnitOwner()->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, owner->GetUInt32Value(PLAYER_VISIBLE_ITEM_17_ENTRYID));
            GetUnitOwner()->SetFloatValue(UNIT_FIELD_COMBATREACH, 0.01f);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dk_dancing_rune_weapon_visual::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// -49004 - Scent of Blood
class spell_dk_scent_of_blood_trigger : public AuraScript
{
    PrepareAuraScript(spell_dk_scent_of_blood_trigger);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return (eventInfo.GetHitMask() & (PROC_EX_DODGE | PROC_EX_PARRY)) || (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_scent_of_blood_trigger::CheckProc);
    }
};

/* 51996 - Death Knight Pet Scaling 02
   54566 - Death Knight Pet Scaling 01
   61697 - Death Knight Pet Scaling 03 */
class spell_dk_pet_scaling : public AuraScript
{
    PrepareAuraScript(spell_dk_pet_scaling);

    void CalculateStatAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        Stats stat = Stats(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);

        // xinef: dk ghoul inherits 70% of strength and 30% of stamina
        if (GetUnitOwner()->GetEntry() != NPC_RISEN_GHOUL)
        {
            // xinef: ebon garogyle - inherit 30% of stamina
            if (GetUnitOwner()->GetEntry() == NPC_EBON_GARGOYLE && stat == STAT_STAMINA)
                if (Unit* owner = GetUnitOwner()->GetOwner())
                    amount = CalculatePct(std::max<int32>(0, owner->GetStat(stat)), 30);
            return;
        }

        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 modifier = stat == STAT_STRENGTH ? 70 : 30;

            // Check just if owner has Ravenous Dead since it's effect is not an aura
            if (AuraEffect const* rdEff = owner->GetAuraEffect(SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, SPELLFAMILY_DEATHKNIGHT, 3010, 0))
            {
                SpellInfo const* spellInfo = rdEff->GetSpellInfo();                                                 // Then get the SpellProto and add the dummy effect value
                AddPct(modifier, spellInfo->Effects[EFFECT_1].CalcValue());                                          // Ravenous Dead edits the original scale
            }

            // xinef: Glyph of the Ghoul
            if (AuraEffect const* glyphEff = owner->GetAuraEffect(SPELL_DK_GLYPH_OF_THE_GHOUL, EFFECT_0))
                modifier += glyphEff->GetAmount();

            amount = CalculatePct(std::max<int32>(0, owner->GetStat(stat)), modifier);
        }
    }

    void CalculateSPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: dk gargoyle inherits 33% of SP
        if (GetUnitOwner()->GetEntry() != NPC_EBON_GARGOYLE)
            return;

        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 modifier = 33;

            // xinef: impurity
            /*if (owner->GetDummyAuraEffect(SPELLFAMILY_DEATHKNIGHT, 1986, 0))
                modifier = 40;*/

            amount = CalculatePct(std::max<int32>(0, owner->GetTotalAttackPowerValue(BASE_ATTACK)), modifier);

            // xinef: Update appropriate player field
            if (owner->GetTypeId() == TYPEID_PLAYER)
                owner->SetUInt32Value(PLAYER_PET_SPELL_POWER, (uint32)amount);
        }
    }

    void CalculateHasteAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: scale haste with owners melee haste
        if (Unit* owner = GetUnitOwner()->GetOwner())
            if (owner->m_modAttackSpeedPct[BASE_ATTACK] < 1.0f) // inherit haste only
                amount = std::min<int32>(100, int32(((1.0f / owner->m_modAttackSpeedPct[BASE_ATTACK]) - 1.0f) * 100.0f));
    }

    void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (aurEff->GetAuraType() != SPELL_AURA_MELEE_SLOW)
            return;

        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK, true, SPELL_BLOCK_TYPE_POSITIVE);
        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_MELEE_RANGED_HASTE, true, SPELL_BLOCK_TYPE_POSITIVE);
        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MELEE_SLOW, true, SPELL_BLOCK_TYPE_POSITIVE);

        if (GetUnitOwner()->IsPet())
            return;

        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_STAT, true, SPELL_BLOCK_TYPE_POSITIVE);
        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, true, SPELL_BLOCK_TYPE_POSITIVE);
        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ATTACK_POWER, true, SPELL_BLOCK_TYPE_POSITIVE);
        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ATTACK_POWER_PCT, true, SPELL_BLOCK_TYPE_POSITIVE);
    }

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        if (!GetUnitOwner()->IsPet())
            return;

        isPeriodic = true;
        amplitude = 2 * IN_MILLISECONDS;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        PreventDefaultAction();
        if (aurEff->GetAuraType() == SPELL_AURA_MOD_STAT && (aurEff->GetMiscValue() == STAT_STAMINA || aurEff->GetMiscValue() == STAT_INTELLECT))
        {
            int32 currentAmount = aurEff->GetAmount();
            int32 newAmount = GetEffect(aurEff->GetEffIndex())->CalculateAmount(GetCaster());
            if (newAmount != currentAmount)
            {
                if (aurEff->GetMiscValue() == STAT_STAMINA)
                {
                    uint32 actStat = GetUnitOwner()->GetHealth();
                    GetEffect(aurEff->GetEffIndex())->ChangeAmount(newAmount, false);
                    GetUnitOwner()->SetHealth(std::min<uint32>(GetUnitOwner()->GetMaxHealth(), actStat));
                }
                else
                {
                    uint32 actStat = GetUnitOwner()->GetPower(POWER_MANA);
                    GetEffect(aurEff->GetEffIndex())->ChangeAmount(newAmount, false);
                    GetUnitOwner()->SetPower(POWER_MANA, std::min<uint32>(GetUnitOwner()->GetMaxPower(POWER_MANA), actStat));
                }
            }
        }
        else
            GetEffect(aurEff->GetEffIndex())->RecalculateAmount();
    }

    void Register() override
    {
        if (m_scriptSpellId == 54566)
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling::CalculateStatAmount, EFFECT_ALL, SPELL_AURA_MOD_STAT);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling::CalculateSPAmount, EFFECT_ALL, SPELL_AURA_MOD_DAMAGE_DONE);
        }

        if (m_scriptSpellId == 51996)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling::CalculateHasteAmount, EFFECT_ALL, SPELL_AURA_MELEE_SLOW);

        OnEffectApply += AuraEffectApplyFn(spell_dk_pet_scaling::HandleEffectApply, EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_dk_pet_scaling::CalcPeriodic, EFFECT_ALL, SPELL_AURA_ANY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_pet_scaling::HandlePeriodic, EFFECT_ALL, SPELL_AURA_ANY);
    }
};

// 50462 - Anti-Magic Zone (on raid member)
class spell_dk_anti_magic_shell_raid : public AuraScript
{
    PrepareAuraScript(spell_dk_anti_magic_shell_raid);

    uint32 absorbPct;

    bool Load() override
    {
        absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
        return true;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // TODO: this should absorb limited amount of damage, but no info on calculation formula
        amount = -1;
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_raid::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_raid::Absorb, EFFECT_0);
    }
};

// 48707 - Anti-Magic Shell (on self)
class spell_dk_anti_magic_shell_self : public AuraScript
{
    PrepareAuraScript(spell_dk_anti_magic_shell_self);

    uint32 absorbPct, hpPct;
    bool Load() override
    {
        absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
        hpPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
        return true;
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DK_RUNIC_POWER_ENERGIZE });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = GetCaster()->CountPctFromMaxHealth(hpPct);
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        absorbAmount = std::min(CalculatePct(dmgInfo.GetDamage(), absorbPct), GetTarget()->CountPctFromMaxHealth(hpPct));
    }

    void Trigger(AuraEffect* aurEff, DamageInfo& /*dmgInfo*/, uint32& absorbAmount)
    {
        // damage absorbed by Anti-Magic Shell energizes the DK with additional runic power.
        // This, if I'm not mistaken, shows that we get back ~20% of the absorbed damage as runic power.
        int32 bp = CalculatePct(absorbAmount, 20);
        GetTarget()->CastCustomSpell(SPELL_DK_RUNIC_POWER_ENERGIZE, SPELLVALUE_BASE_POINT0, bp, GetTarget(), true, nullptr, aurEff);
    }

    void HandleEffectApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        target->ApplySpellImmune(GetId(), IMMUNITY_ID, 33786, true); // cyclone
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        target->ApplySpellImmune(GetId(), IMMUNITY_ID, 33786, false); // cyclone
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_self::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self::Absorb, EFFECT_0);
        AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self::Trigger, EFFECT_0);

        OnEffectApply += AuraEffectApplyFn(spell_dk_anti_magic_shell_self::HandleEffectApply, EFFECT_1, SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_anti_magic_shell_self::HandleEffectRemove, EFFECT_1, SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL, AURA_EFFECT_HANDLE_REAL);
    }
};

// 50461 - Anti-Magic Zone
class spell_dk_anti_magic_zone : public AuraScript
{
    PrepareAuraScript(spell_dk_anti_magic_zone);

    uint32 absorbPct;

    bool Load() override
    {
        absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
        return true;
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DK_ANTI_MAGIC_SHELL_TALENT });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        SpellInfo const* talentSpell = sSpellMgr->AssertSpellInfo(SPELL_DK_ANTI_MAGIC_SHELL_TALENT);

        Unit* owner = GetCaster()->GetOwner();
        if (!owner)
        {
            return;
        }

        amount = talentSpell->Effects[EFFECT_0].CalcValue(owner);
        float scaling = talentSpell->Effects[EFFECT_1].CalcValue(owner);
        uint32 health = owner->GetMaxHealth();
        if (Player* player = owner->ToPlayer())
        {
            amount += AddPct(health, scaling);
        }
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_zone::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_zone::Absorb, EFFECT_0);
    }
};

// -48721 - Blood Boil
class spell_dk_blood_boil : public SpellScript
{
    PrepareSpellScript(spell_dk_blood_boil);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DK_BLOOD_BOIL_TRIGGERED });
    }

    bool Load() override
    {
        _executed = false;
        return GetCaster()->GetTypeId() == TYPEID_PLAYER && GetCaster()->getClass() == CLASS_DEATH_KNIGHT;
    }

    void HandleAfterHit()
    {
        if (_executed || !GetHitUnit())
            return;

        _executed = true;
        GetCaster()->CastSpell(GetCaster(), SPELL_DK_BLOOD_BOIL_TRIGGERED, true);
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_dk_blood_boil::HandleAfterHit);
    }

    bool _executed;
};

// 50453 - Bloodworms Health Leech
class spell_dk_blood_gorged : public AuraScript
{
    PrepareAuraScript(spell_dk_blood_gorged);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DK_BLOOD_GORGED_HEAL });
    }

    bool Load() override
    {
        _procTarget = nullptr;
        return true;
    }

    bool CheckProc(ProcEventInfo& /*eventInfo*/)
    {
        _procTarget = GetTarget()->GetOwner();
        return _procTarget;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return;
        }

        int32 bp = static_cast<int32>(damageInfo->GetDamage() * 1.0f);
        GetTarget()->CastCustomSpell(SPELL_DK_BLOOD_GORGED_HEAL, SPELLVALUE_BASE_POINT0, bp, _procTarget, true, nullptr, aurEff);

        if (Aura* aura = GetTarget()->GetAura(SPELL_DK_IMPROVED_BLOODWORMS_DEATH)) {
            AuraEffect* effect = aura->GetEffect(EFFECT_0);
            effect->SetAmount(effect->GetAmount() + bp);
        }

        /* /if (GetTarget()->HasAura(SPELL_DK_IMPROVED_BLOODWORMS_DEATH))
            GetTarget()->GetAura(SPELL_DK_IMPROVED_BLOODWORMS_DEATH)->ModStackAmount(bp); */
    } 

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_blood_gorged::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_blood_gorged::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }

private:
    Unit* _procTarget;
};

class CorpseExplosionCheck
{
public:
    explicit CorpseExplosionCheck(ObjectGuid casterGUID, bool allowGhoul) : _casterGUID(casterGUID), _allowGhoul(allowGhoul) { }

    bool operator()(WorldObject* obj) const
    {
        if (Unit* target = obj->ToUnit())
        {
            if ((target->isDead() || (_allowGhoul && target->GetEntry() == NPC_DK_GHOUL && target->GetOwnerGUID() == _casterGUID))
                    && !(target->GetCreatureTypeMask() & CREATURE_TYPEMASK_MECHANICAL_OR_ELEMENTAL)
                    && target->GetDisplayId() == target->GetNativeDisplayId())
                return false;
        }

        return true;
    }

private:
    ObjectGuid _casterGUID;
    bool _allowGhoul;
};

// -49158 - Corpse Explosion
class spell_dk_corpse_explosion : public SpellScript
{
    PrepareSpellScript(spell_dk_corpse_explosion);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_DK_CORPSE_EXPLOSION_TRIGGERED,
                SPELL_DK_GHOUL_EXPLODE,
                SPELL_DK_CORPSE_EXPLOSION_VISUAL,
            });
    }

    bool Load() override
    {
        _target = nullptr;
        return true;
    }

    void CheckTarget(WorldObject*& target)
    {
        if (CorpseExplosionCheck(GetCaster()->GetGUID(), true)(target))
            target = nullptr;

        _target = target;
    }

    void CheckTargets(std::list<WorldObject*>& targets)
    {
        WorldObject* target = _target;
        if (!target)
        {
            targets.remove_if(CorpseExplosionCheck(GetCaster()->GetGUID(), false));
            if (targets.empty())
            {
                FinishCast(SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW);
                return;
            }
            target = Acore::Containers::SelectRandomContainerElement(targets);
            targets.clear();
            targets.push_back(target);
        }
        else
            targets.clear();
    }

    void HandleDamage(SpellEffIndex effIndex, Unit* target)
    {
        if (effIndex == EFFECT_0)
            GetCaster()->CastCustomSpell(GetSpellInfo()->Effects[EFFECT_1].CalcValue(), SPELLVALUE_BASE_POINT0, GetEffectValue(), target, true);
        else if (effIndex == EFFECT_1)
            GetCaster()->CastCustomSpell(GetEffectValue(), SPELLVALUE_BASE_POINT0, GetSpell()->CalculateSpellDamage(EFFECT_0, nullptr), target, true);
    }

    void HandleCorpseExplosion(SpellEffIndex effIndex)
    {
        if (Unit* unitTarget = GetHitUnit())
        {
            if (unitTarget->IsAlive())  // Living ghoul as a target
            {
                unitTarget->ToCreature()->m_CreatureSpellCooldowns.clear();
                if (CharmInfo* charmInfo = unitTarget->GetCharmInfo())
                    charmInfo->GetGlobalCooldownMgr().CancelGlobalCooldown(sSpellMgr->GetSpellInfo(SPELL_DK_GHOUL_EXPLODE));

                unitTarget->StopMoving();
                unitTarget->CastSpell(unitTarget, SPELL_DK_GHOUL_EXPLODE, false);
                // Corpse Explosion (Suicide) and Set corpse look handled in SpellScript of SPELL_DK_GHOUL_EXPLODE
            }
            else                        // Some corpse
            {
                HandleDamage(effIndex, unitTarget);
                // Corpse Explosion (Suicide)
                unitTarget->CastSpell(unitTarget, SPELL_DK_CORPSE_EXPLOSION_TRIGGERED, true);
                // Set corpse look
                GetCaster()->CastSpell(unitTarget, SPELL_DK_CORPSE_EXPLOSION_VISUAL, true);
            }
        }
    }

    void Register() override
    {
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_dk_corpse_explosion::CheckTarget, EFFECT_0, TARGET_UNIT_TARGET_ANY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_corpse_explosion::CheckTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENTRY);
        OnEffectHitTarget += SpellEffectFn(spell_dk_corpse_explosion::HandleCorpseExplosion, EFFECT_0, SPELL_EFFECT_DUMMY);
        OnEffectHitTarget += SpellEffectFn(spell_dk_corpse_explosion::HandleCorpseExplosion, EFFECT_1, SPELL_EFFECT_DUMMY);
    }

private:
    WorldObject* _target;
};

// -62900, -47541, 52375, 59134 - Death Coil
class spell_dk_death_coil : public SpellScript
{
    PrepareSpellScript(spell_dk_death_coil);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (Unit* target = GetHitUnit())
        {
            if (caster->IsFriendlyTo(target))
                caster->CastSpell(target, SPELL_DK_DEATH_COIL_HEAL, TRIGGERED_FULL_MASK);
            else
                caster->CastSpell(target, SPELL_DK_DEATH_COIL_DAMAGE, TRIGGERED_FULL_MASK);
        }
    }

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        if (Unit* target = GetExplTargetUnit())
        {
            if (!caster->IsFriendlyTo(target) && !caster->isInFront(target))
                return SPELL_FAILED_UNIT_NOT_INFRONT;

            if (target->IsFriendlyTo(caster) && target->GetCreatureType() != CREATURE_TYPE_UNDEAD)
                return SPELL_FAILED_BAD_TARGETS;
        }
        else
            return SPELL_FAILED_BAD_TARGETS;

        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dk_death_coil::CheckCast);
        OnEffectHitTarget += SpellEffectFn(spell_dk_death_coil::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 52751 - Death Gate
class spell_dk_death_gate : public SpellScript
{
    PrepareSpellScript(spell_dk_death_gate);

    SpellCastResult CheckClass()
    {
        if (GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
        {
            SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
            return SPELL_FAILED_CUSTOM_ERROR;
        }

        return SPELL_CAST_OK;
    }

    void HandleScript(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        if (Unit* target = GetHitUnit())
            target->CastSpell(target, GetEffectValue(), false);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dk_death_gate::CheckClass);
        OnEffectHitTarget += SpellEffectFn(spell_dk_death_gate::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 49560, 49576 - Death Grip
class spell_dk_death_grip : public SpellScript
{
    PrepareSpellScript(spell_dk_death_grip);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();

        if (target->GetTypeId() == TYPEID_PLAYER && caster->GetExactDist(target) < 8.0f) // xinef: should be 8.0f, but we have to add target size (1.5f)
            return SPELL_FAILED_TOO_CLOSE;

        if (caster->HasUnitState(UNIT_STATE_JUMPING) || caster->HasUnitMovementFlag(MOVEMENTFLAG_FALLING))
            return SPELL_FAILED_MOVING;

        return SPELL_CAST_OK;
    }

    uint32 EntryCheck(uint32 entry)
    {
        Creature* targetCreature = GetHitCreature();

        switch (targetCreature->GetEntry())
        {
            //Alliance Faction Champions
            case 34461:
            case 34460:
            case 34469:
            case 34467:
            case 34468:
            case 34465:
            case 34471:
            case 34466:
            case 34473:
            case 34472:
            case 34470:
            case 34463:
            case 34474:
            case 34475:

            //Horde Faction Champions
            case 34458:
            case 34451:
            case 34459:
            case 34448:
            case 34449:
            case 34445:
            case 34456:
            case 34447:
            case 34441:
            case 34454:
            case 34444:
            case 34455:
            case 34450:
            case 34453:
                return entry;
                break;
        }
        return 0;
    }

    void HandleBaseDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        Unit* baseTarget = GetExplTargetUnit();
        Creature* targetCreature = GetHitCreature();

        if (caster != target)
        {
            if (targetCreature && (targetCreature->isWorldBoss() || targetCreature->IsDungeonBoss()) && targetCreature->GetEntry() != EntryCheck(targetCreature->GetEntry()))
            {
                return;
            }
            else
            {
                caster->CastSpell(target, 49560, true);
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(1766); // Rogue kick
                if (!target->IsImmunedToSpellEffect(spellInfo, EFFECT_0))
                    target->InterruptNonMeleeSpells(true);
            }
        }
        else
            baseTarget->CastSpell(caster, 49560, true);
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        float casterZ = GetCaster()->GetPositionZ(); // for Ring of Valor
        WorldLocation gripPos = *GetExplTargetDest();
        if (Unit* target = GetHitUnit())
            if (!target->HasAuraType(SPELL_AURA_DEFLECT_SPELLS) || target->HasUnitState(UNIT_STATE_STUNNED)) // Deterrence
            {
                if (target != GetCaster())
                {
                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(1766); // Rogue kick
                    if (!target->IsImmunedToSpellEffect(spellInfo, EFFECT_0))
                        target->InterruptNonMeleeSpells(false, 0, false);
                }

                if (target->GetMapId() == 618) // for Ring of Valor
                    gripPos.m_positionZ = std::max(casterZ + 0.2f, 28.5f);

                target->CastSpell(gripPos.GetPositionX(), gripPos.GetPositionY(), gripPos.GetPositionZ(), 57604, true);
            }
    }

    void Register() override
    {
        if (m_scriptSpellId == 49576) // xinef: base death grip, add pvp range restriction
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_death_grip::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_dk_death_grip::HandleBaseDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
        else
            OnEffectHitTarget += SpellEffectFn(spell_dk_death_grip::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// -49998 - Death Strike
class spell_dk_death_strike : public SpellScript
{
    PrepareSpellScript(spell_dk_death_strike);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DK_DEATH_STRIKE_HEAL });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (Unit* target = GetHitUnit())
        {
            uint32 count = target->GetDiseasesByCaster(caster->GetGUID());
            SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DK_DEATH_STRIKE);
            uint32 healPct = value->GetEffect(EFFECT_1).CalcValue(caster);
            int32 bp = int32(count * caster->CountPctFromMaxHealth(int32(healPct)));
            // Improved Death Strike
            if (AuraEffect const* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_DK_IMPROVED_DEATH_STRIKE, EFFECT_0))
                AddPct(bp, caster->CalculateSpellDamage(caster, aurEff->GetSpellInfo(), 2));

            GetCaster()->CastCustomSpell(SPELL_DK_DEATH_STRIKE_HEAL, SPELLVALUE_BASE_POINT0, bp, caster);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_death_strike::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
    }
};

// 47496 - Explode
class spell_dk_ghoul_explode : public SpellScript
{
    PrepareSpellScript(spell_dk_ghoul_explode);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DK_CORPSE_EXPLOSION_TRIGGERED });
    }

    void HandleDamage(SpellEffIndex /*effIndex*/)
    {
        int32 value = int32(GetCaster()->CountPctFromMaxHealth(GetSpellInfo()->Effects[EFFECT_2].CalcValue(GetCaster())));
        SetEffectValue(value);
    }

    void Suicide(SpellEffIndex /*effIndex*/)
    {
        if (Unit* unitTarget = GetHitUnit())
        {
            // Corpse Explosion (Suicide)
            unitTarget->CastSpell(unitTarget, SPELL_DK_CORPSE_EXPLOSION_TRIGGERED, true);
            // Set corpse look
            GetCaster()->CastSpell(unitTarget, SPELL_DK_CORPSE_EXPLOSION_VISUAL, true);
        }
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_dk_ghoul_explode::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        OnEffectHitTarget += SpellEffectFn(spell_dk_ghoul_explode::Suicide, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 48792 - Icebound Fortitude
class spell_dk_icebound_fortitude : public AuraScript
{
    PrepareAuraScript(spell_dk_icebound_fortitude);

    bool Load() override
    {
        Unit* caster = GetCaster();
        return caster && caster->GetTypeId() == TYPEID_PLAYER;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* caster = GetCaster())
        {
            int32 value = amount;
            uint32 defValue = uint32(caster->ToPlayer()->GetSkillValue(SKILL_DEFENSE) + caster->ToPlayer()->GetRatingBonusValue(CR_DEFENSE_SKILL));

            if (defValue > 400)
                value -= int32((defValue - 400) * 0.15);

            // Glyph of Icebound Fortitude
            if (AuraEffect const* aurEff = caster->GetAuraEffect(SPELL_DK_GLYPH_OF_ICEBOUND_FORTITUDE, EFFECT_0))
            {
                int32 valMax = -aurEff->GetAmount();
                if (value > valMax)
                    value = valMax;
            }
            amount = value;
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_icebound_fortitude::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
    }
};

// 50842 - Pestilence
class spell_dk_pestilence : public SpellScript
{
    PrepareSpellScript(spell_dk_pestilence);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* hitUnit = GetHitUnit();
        Unit* target = GetExplTargetUnit();
        if (!target)
            return;

        if (target != hitUnit)
        {
            if (target->GetAura(SPELL_DK_BLOOD_PLAGUE, caster->GetGUID()))
                caster->CastSpell(hitUnit, SPELL_DK_BLOOD_PLAGUE, true);
            if (target->GetAura(SPELL_DK_FROST_FEVER, caster->GetGUID()))
                caster->CastSpell(hitUnit, SPELL_DK_FROST_FEVER, true);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_pestilence::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

class RaiseDeadCheck
{
public:
    explicit RaiseDeadCheck(Player const* caster) : _caster(caster) { }

    bool operator()(WorldObject* obj) const
    {
        if (Unit* target = obj->ToUnit())
        {
            if (!target->IsAlive()
                    && _caster->isHonorOrXPTarget(target)
                    && target->GetCreatureType() == CREATURE_TYPE_HUMANOID
                    && target->GetDisplayId() == target->GetNativeDisplayId())
                return false;
        }

        return true;
    }

private:
    Player const* _caster;
};

// 46584 - Raise Dead
class spell_dk_raise_dead : public SpellScript
{
    PrepareSpellScript(spell_dk_raise_dead);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_DK_RAISE_DEAD_USE_REAGENT,
                SPELL_DK_MASTER_OF_GHOULS
            });
    }

    bool Load() override
    {
        _result = SPELL_CAST_OK;
        _corpse = false;
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    SpellCastResult CheckCast()
    {
        /// process spell target selection before cast starts
        /// targets of effect_1 are used to check cast
        GetSpell()->SelectSpellTargets();
        /// cleanup spell target map, and fill it again on normal way
        GetSpell()->CleanupTargetList();
        /// _result is set in spell target selection
        return _result;
    }

    void CheckTarget(WorldObject*& target)
    {
        // Don't add caster to target map, if we found a corpse to raise dead
        if (_corpse)
            target = nullptr;
    }

    void ConsumeReagents()
    {
        // No corpse found, take reagents
        if (!_corpse)
            GetCaster()->CastSpell(GetCaster(), SPELL_DK_RAISE_DEAD_USE_REAGENT, TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_IGNORE_POWER_AND_REAGENT_COST));
    }

    uint32 GetGhoulSpellId()
    {
        // Do we have talent Master of Ghouls?
        if (GetCaster()->HasAura(SPELL_DK_MASTER_OF_GHOULS))
            // summon as pet
            return GetSpellInfo()->Effects[EFFECT_2].CalcValue();

        // or guardian
        return GetSpellInfo()->Effects[EFFECT_1].CalcValue();
    }

    void HandleRaiseDead(SpellEffIndex /*effIndex*/)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(GetGhoulSpellId());
        SpellCastTargets targets;
        targets.SetDst(*GetHitUnit());

        GetCaster()->CastSpell(targets, spellInfo, nullptr, TRIGGERED_FULL_MASK, nullptr, nullptr, GetCaster()->GetGUID());
        GetCaster()->ToPlayer()->RemoveSpellCooldown(GetSpellInfo()->Id, true);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dk_raise_dead::CheckCast);
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_dk_raise_dead::CheckTarget, EFFECT_2, TARGET_UNIT_CASTER);
        OnCast += SpellCastFn(spell_dk_raise_dead::ConsumeReagents);
        OnEffectHitTarget += SpellEffectFn(spell_dk_raise_dead::HandleRaiseDead, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        OnEffectHitTarget += SpellEffectFn(spell_dk_raise_dead::HandleRaiseDead, EFFECT_2, SPELL_EFFECT_DUMMY);
    }

private:
    SpellCastResult _result;
    bool _corpse;
};

// 59754 - Rune Tap
class spell_dk_rune_tap_party : public SpellScript
{
    PrepareSpellScript(spell_dk_rune_tap_party);

    void CheckTargets(std::list<WorldObject*>& targets)
    {
        targets.remove(GetCaster());
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_rune_tap_party::CheckTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_PARTY);
    }
};

// 50421 - Scent of Blood
class spell_dk_scent_of_blood : public AuraScript
{
    PrepareAuraScript(spell_dk_scent_of_blood);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DK_SCENT_OF_BLOOD });
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(GetTarget(), SPELL_DK_SCENT_OF_BLOOD, true, nullptr, aurEff);
        GetTarget()->RemoveAuraFromStack(GetSpellInfo()->Id);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_scent_of_blood::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// -55090 - Scourge Strike
class spell_dk_scourge_strike : public SpellScript
{
    PrepareSpellScript(spell_dk_scourge_strike);
    float multiplier;
    ObjectGuid guid;

    bool Load() override
    {
        multiplier = 1.0f;
        guid.Clear();
        return true;
    }

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DK_SCOURGE_STRIKE_TRIGGERED });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (Unit* unitTarget = GetHitUnit())
        {
            uint8 mode = caster->GetAuraEffectDummy(SPELL_DK_GLYPH_OF_SCOURGE_STRIKE) ? 2 : 0;
            float disease_amt = GetEffectValue();

            // Death Knight T8 Melee 4P Bonus
            if (AuraEffect const* aurEff = caster->GetAuraEffect(SPELL_DK_ITEM_T8_MELEE_4P_BONUS, EFFECT_0))
                AddPct(disease_amt, aurEff->GetAmount());

            multiplier = disease_amt * unitTarget->GetDiseasesByCaster(caster->GetGUID(), mode) / 100.0f;
            guid = unitTarget->GetGUID();
        }
    }

    void HandleAfterHit()
    {
        Unit* caster = GetCaster();
        if (Unit* unitTarget = ObjectAccessor::GetUnit(*caster, guid))
        {
            int32 bp = GetHitDamage() * multiplier;
            caster->CastCustomSpell(unitTarget, SPELL_DK_SCOURGE_STRIKE_TRIGGERED, &bp, nullptr, nullptr, true);

            // Xinef: Shadowmourne hack (scourge strike trigger proc disabled...)
            if (roll_chance_i(75) && caster->FindMap() && !caster->FindMap()->IsBattlegroundOrArena() && caster->HasAura(71903) && !caster->HasAura(SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF))
            {
                caster->CastSpell(caster, SPELL_SHADOWMOURNE_SOUL_FRAGMENT, true);

                // this can't be handled in AuraScript of SoulFragments because we need to know victim
                if (Aura* soulFragments = caster->GetAura(SPELL_SHADOWMOURNE_SOUL_FRAGMENT))
                {
                    if (soulFragments->GetStackAmount() >= 10)
                    {
                        caster->CastSpell(caster, SPELL_SHADOWMOURNE_CHAOS_BANE_DAMAGE, true, nullptr);
                        soulFragments->Remove();
                    }
                }
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_scourge_strike::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
        AfterHit += SpellHitFn(spell_dk_scourge_strike::HandleAfterHit);
    }
};

// -49145 - Spell Deflection
class spell_dk_spell_deflection : public AuraScript
{
    PrepareAuraScript(spell_dk_spell_deflection);

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
        // You have a chance equal to your Parry chance
        float chance = GetTarget()->GetUnitParryChance();
        if (GetTarget()->IsNonMeleeSpellCast(false, false, true) || GetTarget()->HasUnitState(UNIT_STATE_CONTROLLED))
            chance = 0.0f;

        if ((dmgInfo.GetDamageType() == SPELL_DIRECT_DAMAGE) && roll_chance_f(chance))
            absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_spell_deflection::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_spell_deflection::Absorb, EFFECT_0);
    }
};

// 55233 - Vampiric Blood
class spell_dk_vampiric_blood : public AuraScript
{
    PrepareAuraScript(spell_dk_vampiric_blood);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = GetUnitOwner()->CountPctFromMaxHealth(amount);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_vampiric_blood::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_HEALTH);
    }
};

// 52284 - Will of the Necropolis
class spell_dk_will_of_the_necropolis : public AuraScript
{
    PrepareAuraScript(spell_dk_will_of_the_necropolis);

    bool Validate(SpellInfo const* spellInfo) override
    {
        SpellInfo const* firstRankSpellInfo = sSpellMgr->GetSpellInfo(SPELL_DK_WILL_OF_THE_NECROPOLIS_AURA_R1);
        if (!firstRankSpellInfo)
            return false;

        // can't use other spell than will of the necropolis due to spell_ranks dependency
        if (!spellInfo->IsRankOf(firstRankSpellInfo))
            return false;

        uint8 rank = spellInfo->GetRank();
        if (!sSpellMgr->GetSpellWithRank(SPELL_DK_WILL_OF_THE_NECROPOLIS_TALENT_R1, rank, true))
            return false;

        return true;
    }

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
        // min pct of hp is stored in effect 0 of talent spell
        uint8 rank = GetSpellInfo()->GetRank();
        SpellInfo const* talentProto = sSpellMgr->AssertSpellInfo(sSpellMgr->GetSpellWithRank(SPELL_DK_WILL_OF_THE_NECROPOLIS_TALENT_R1, rank));

        int32 remainingHp = int32(GetTarget()->GetHealth() - dmgInfo.GetDamage());
        int32 minHp = int32(GetTarget()->CountPctFromMaxHealth(talentProto->Effects[EFFECT_0].CalcValue(GetCaster())));

        // Damage that would take you below [effect0] health or taken while you are at [effect0]
        if (remainingHp < minHp)
            absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_will_of_the_necropolis::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_will_of_the_necropolis::Absorb, EFFECT_0);
    }
};

class spell_dk_frost_fever : public AuraScript
{
    PrepareAuraScript(spell_dk_frost_fever);

    void OnPeriodic(AuraEffect const* /*aurEff*/)
    {
        Unit* caster = GetCaster();

        if (!caster || !caster->IsAlive())
            return;

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DK_FROST_FEVER);
        uint32 procChance = value->GetEffect(EFFECT_1).CalcValue(caster);

        if (roll_chance_f(procChance) && caster->GetTypeId() == TYPEID_PLAYER)
        {
            caster->CastSpell(caster, SPELL_DK_FROST_FEVER_RUNIC, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_frost_fever::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

class spell_dk_lichborne_leech : public AuraScript
{
    PrepareAuraScript(spell_dk_lichborne_leech);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage)
            {
                int32 healPct = aurEff->GetAmount();
                int32 healAmount = CalculatePct(damage, healPct);
                GetCaster()->CastCustomSpell(SPELL_DK_LICHBORNE_LEECH, SPELLVALUE_BASE_POINT0, healAmount, GetCaster(), TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_lichborne_leech::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_raise_dead_new : public SpellScript
{
    PrepareSpellScript(spell_dk_raise_dead_new);

    uint32 GetGhoulSpellId()
    {
        // Do we have talent Master of Ghouls?
        if (GetCaster()->HasAura(SPELL_DK_MASTER_OF_GHOULS))
            // summon as pet
            return GetSpellInfo()->Effects[EFFECT_2].CalcValue();

        // or guardian
        return GetSpellInfo()->Effects[EFFECT_1].CalcValue();
    }

    void HandleRaiseDead()
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, GetGhoulSpellId(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dk_raise_dead_new::HandleRaiseDead);
    }
};

class spell_dk_obliterate : public SpellScript
{
    PrepareSpellScript(spell_dk_obliterate);

    void HandleDamage(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();

        bool consumeDiseases = true;

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DK_OBLITERATE);
        uint32 diseaseBonus = value->GetEffect(EFFECT_1).CalcValue(GetCaster());

        float diseasePct = diseaseBonus / 10;

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

            uint32 count = target->GetDiseasesByCaster(GetCaster()->GetGUID());
            int32 bp = int32(count * diseasePct);
            AddPct(damage, bp);

            if (AuraEffect const* aurEff = GetCaster()->GetAuraEffectOfRankedSpell(SPELL_DK_ANNIHILATION, EFFECT_0))
                if (roll_chance_i(aurEff->GetAmount()))
                    consumeDiseases = false;

            if (consumeDiseases == true)
            {
                target->RemoveAura(SPELL_DK_FROST_FEVER);
                target->RemoveAura(SPELL_DK_BLOOD_PLAGUE);
                target->RemoveAura(SPELL_DK_FESTERING_WOUND);
            }
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_obliterate::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_dk_festering_wound : public AuraScript
{
    PrepareAuraScript(spell_dk_festering_wound);

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetCaster()->CastSpell(GetTarget(), SPELL_DK_FESTERING_WOUND_PROC, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_festering_wound::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_scourge_strike_new : public SpellScript
{
    PrepareSpellScript(spell_dk_scourge_strike_new);

    void HandleHit(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        ObjectGuid guid = caster->GetGUID();
        if (Aura* targetAura = target->GetAura(SPELL_DK_FESTERING_WOUND, guid))
        {
            targetAura->ModStackAmount(-1);

            caster->CastSpell(target, SPELL_DK_FESTERING_WOUND_PROC, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_scourge_strike_new::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_dk_summon_gargoyle_energy : public SpellScript
{
    PrepareSpellScript(spell_dk_summon_gargoyle_energy);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, SPELL_DK_SUMMON_GARGOYLE_ENERGY, TRIGGERED_FULL_MASK);
        caster->CastSpell(caster, SPELL_DK_SUMMON_GARGOYLE_LISTENER, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dk_summon_gargoyle_energy::HandleCast);
    }
};

class spell_dk_summon_gargoyle_power : public AuraScript
{
    PrepareAuraScript(spell_dk_summon_gargoyle_power);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* target = GetTarget()->ToPlayer();

        if (!target || target->isDead())
            return;

        int32 spellRunic = (eventInfo.GetSpellInfo()->ManaCost) / 10;

        if (spellRunic <= 0)
            return;

        auto summonedUnits = target->m_Controlled;

        for (auto const& unit : summonedUnits)
        {
            if (!unit->IsInWorld())
                continue;

            if (unit->isDead())
                continue;

            if (Aura* targetAura = unit->GetAura(SPELL_DK_SUMMON_GARGOYLE_DAMAGE_BUFF))
            {
                int32 baseValue = GetAura()->GetEffect(EFFECT_0)->GetAmount();
                targetAura->GetEffect(EFFECT_0)->ChangeAmount(baseValue + spellRunic);
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_summon_gargoyle_power::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_summon_gargoyle_power::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_virulent_plague : public AuraScript
{

    PrepareAuraScript(spell_dk_virulent_plague);

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetCaster()->CastSpell(GetTarget(), SPELL_DK_VIRULENT_PLAGUE_PROC, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_virulent_plague::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_sacrifical_pact : public SpellScript
{
    PrepareSpellScript(spell_dk_sacrifical_pact);

    SpellCastResult CheckCast()
    {
        // Check if we have valid targets, otherwise skip spell casting here
        if (Player* player = GetCaster()->ToPlayer())
            for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                if (Unit* undeadPet = (*itr))
                    if (undeadPet->IsAlive() &&
                        undeadPet->GetOwnerGUID() == player->GetGUID() &&
                        undeadPet->GetEntry() == NPC_DK_GHOUL &&
                        undeadPet->IsWithinDist(player, 100.0f, false))
                        return SPELL_CAST_OK;

        return SPELL_FAILED_NO_PET;
    }

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        auto summonedUnits = caster->m_Controlled;

        for (auto const& unit : summonedUnits)
        {
            if (unit->GetEntry() == NPC_DK_GHOUL)
            {
                caster->CastSpell(unit, SPELL_DK_SACRIFICAL_PACT_EXPLOSION, TRIGGERED_FULL_MASK);
                break;
            }
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dk_sacrifical_pact::CheckCast);
        OnCast += SpellCastFn(spell_dk_sacrifical_pact::HandleCast);
    }
};

class spell_dk_dark_transformation : public SpellScript
{
    PrepareSpellScript(spell_dk_dark_transformation);

    SpellCastResult CheckCast()
    {
        // Check if we have valid targets, otherwise skip spell casting here
        Player* caster = GetCaster()->ToPlayer();
        Pet* pet = caster->GetPet();

        if (!pet)
            return SPELL_FAILED_NO_PET;

        if (pet && pet->GetEntry() == NPC_DK_GHOUL)
            return SPELL_CAST_OK;

        return SPELL_FAILED_NO_PET;
    }

    void HandleCast()
    {
        Player* caster = GetCaster()->ToPlayer();
        Pet* pet = caster->GetPet();

        if (!pet)
            return;

        if (!GetCaster()->GetOwner())
            return;

        if (pet->GetEntry() == NPC_DK_GHOUL)
        {
            caster->CastSpell(pet, SPELL_DK_DARK_TRANSFORMATION_DAMAGE, TRIGGERED_FULL_MASK, nullptr, nullptr, GetCaster()->GetOwner()->GetGUID());
            caster->AddAura(SPELL_DK_DARK_TRANSFORMATION_POWERUP, pet);
            pet->unlearnSpell(47468, false, true, false);
            pet->learnSpell(80403, false);
            pet->SetDisplayId(1693);
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dk_dark_transformation::CheckCast);
        OnCast += SpellCastFn(spell_dk_dark_transformation::HandleCast);
    }
};

class spell_dk_dark_transformation_expire : public AuraScript {

    PrepareAuraScript(spell_dk_dark_transformation_expire);
  
    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        target->DeMorph();
        target->ToPet()->unlearnSpell(80403, false, true, false);
        target->ToPet()->learnSpell(47468, false);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_dark_transformation_expire::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_asphyxiate : public AuraScript
{
    PrepareAuraScript(spell_dk_asphyxiate);

    void HandleEffectApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        target->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STRANGULATE);
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        target->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dk_asphyxiate::HandleEffectApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_asphyxiate::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_soul_reaper : public AuraScript
{
    PrepareAuraScript(spell_dk_soul_reaper);

    Aura* GetRuneAura(Unit* caster)
    {
        for (size_t i = 600510; i < 600516; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }
        return nullptr;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        Unit* target = GetTarget();
        if (target->GetHealthPct() <= 35 && caster->IsAlive())
            GetCaster()->CastSpell(target, SPELL_DK_SOUL_REAPER_PERIODIC, TRIGGERED_FULL_MASK);
    };

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, SPELL_DK_RUNIC_CORRUPTION, TRIGGERED_FULL_MASK);

        if (Aura* runeAura = GetRuneAura(caster))
            caster->CastSpell(caster, runeAura->GetEffect(EFFECT_0)->GetAmount(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_soul_reaper::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectProc += AuraEffectProcFn(spell_dk_soul_reaper::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_rune_of_sanguination : public SpellScript
{
    PrepareSpellScript(spell_dk_rune_of_sanguination);

    void HandleDamage(SpellEffIndex effIndex)
    {
        if (GetCaster()->HasAura(SPELL_DK_RUNE_OF_SANGUINATION))
        {
            int32 damage = GetEffectValue();

            SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DK_RUNE_OF_SANGUINATION);
            uint32 diseaseBonus = value->GetEffect(EFFECT_0).CalcValue(GetCaster());

            if (Unit* target = GetHitUnit())
            {
                int32 increase = 100 - (GetExplTargetUnit()->GetHealthPct());

                damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
                damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

                AddPct(damage, increase);
            }

            SetHitDamage(damage);
        };
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_rune_of_sanguination::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_dk_rune_of_sanguination_heal : public AuraScript
{
    PrepareAuraScript(spell_dk_rune_of_sanguination_heal);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && GetCaster()->GetHealthPct() <= 35)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage)
            {
                int32 healPct = aurEff->GetAmount();
                int32 healAmount = CalculatePct(damage, healPct) / 8;
                GetCaster()->CastCustomSpell(SPELL_DK_RUNE_OF_SANGUINATION_HEAL, SPELLVALUE_BASE_POINT0, healAmount, GetCaster(), TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_rune_of_sanguination_heal::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_rune_of_spellwarding : public AuraScript
{
    PrepareAuraScript(spell_dk_rune_of_spellwarding);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 shieldPct = aurEff->GetAmount();
        int32 shieldAmount = CalculatePct(GetCaster()->GetMaxHealth(), shieldPct);
        GetCaster()->CastCustomSpell(SPELL_DK_RUNE_OF_SPELLWARDING_SHIELD, SPELLVALUE_BASE_POINT0, shieldAmount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_rune_of_spellwarding::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_rune_of_apocalypse : public AuraScript
{
    PrepareAuraScript(spell_dk_rune_of_apocalypse);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* owner = GetCaster();
        Unit* pet = GetAura()->GetOwner()->ToUnit();
        Unit* target = eventInfo.GetActionTarget();

        if (!owner->IsAlive())
            return;

        std::vector<uint32> procs =
        {
                SPELL_DK_RUNE_APOCALYPSE_DEATH,
                SPELL_DK_RUNE_APOCALYPSE_WAR,
                SPELL_DK_RUNE_APOCALYPSE_FAMINE,
                SPELL_DK_RUNE_APOCALYPSE_PESTILENCE
        };

        uint32 random = urand(0, procs.size() - 1);
        uint32 spellIdToProc = procs[random];
        if (spellIdToProc == SPELL_DK_RUNE_APOCALYPSE_PESTILENCE)
        {
            int32 ap = owner->GetTotalAttackPowerValue(BASE_ATTACK);
            int32 damageAmount = CalculatePct(ap, 5);
            pet->CastCustomSpell(SPELL_DK_RUNE_APOCALYPSE_PESTILENCE, SPELLVALUE_BASE_POINT0, damageAmount, target, TRIGGERED_FULL_MASK);
        }
        else
        {
            owner->CastSpell(target, spellIdToProc, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_rune_of_apocalypse::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_remove_impairing_auras : public SpellScript
{
    PrepareSpellScript(spell_dk_remove_impairing_auras);

    void HandleScriptEffect(SpellEffIndex /* effIndex */)
    {
        GetHitUnit()->RemoveMovementImpairingAuras(true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_remove_impairing_auras::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    }
};

class spell_dk_deaths_caress : public SpellScript
{
    PrepareSpellScript(spell_dk_deaths_caress);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DK_DEATHS_CARESS);
        int32 stackAmount = value->GetEffect(EFFECT_2).CalcValue(caster);

        if (Aura* aura = caster->GetAura(SPELL_DK_BONE_SHIELD))
            aura->ModStackAmount(stackAmount);
        else
        {
            caster->CastSpell(caster, SPELL_DK_BONE_SHIELD, TRIGGERED_FULL_MASK);

            Aura* newAura = caster->GetAura(SPELL_DK_BONE_SHIELD);
            newAura->ModStackAmount(stackAmount - 1);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dk_deaths_caress::HandleCast);
    }
};

class spell_dk_marrowrend : public SpellScript
{
    PrepareSpellScript(spell_dk_marrowrend);

    void HandleCast()
    {
        Unit* caster = GetCaster();

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DK_MARROWREND);
        int32 stackAmount = value->GetEffect(EFFECT_1).CalcValue(caster);

        if (Aura* aura = caster->GetAura(SPELL_DK_BONE_SHIELD))
            aura->ModStackAmount(stackAmount);
        else
        {
            caster->CastSpell(caster, SPELL_DK_BONE_SHIELD, TRIGGERED_FULL_MASK);

            Aura* newAura = caster->GetAura(SPELL_DK_BONE_SHIELD);
            newAura->ModStackAmount(stackAmount - 1);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dk_marrowrend::HandleCast);
    }
};

class spell_dk_gorefiends_grasp : public SpellScript
{
    PrepareSpellScript(spell_dk_gorefiends_grasp);

    void FindTargets(std::list<WorldObject*>& targets)
    {
        Unit* initialTarget = GetExplTargetUnit();
        Position targetPos = initialTarget->GetPosition();
        if (targets.size() > 0)
        {
            for (auto const& target : targets)
                if (Creature* creatureTarget = target->ToCreature())
                {
                    if (CreatureTemplate const* cinfo = sObjectMgr->GetCreatureTemplate(creatureTarget->GetEntry()))
                        if (cinfo->MechanicImmuneMask & 32)
                            return;

                    if (!creatureTarget->isWorldBoss() || !creatureTarget->IsDungeonBoss())
                    {
                        float distance = target->GetDistance(initialTarget);
                        if (distance <= 15)
                            if (Unit* unit = target->ToUnit())
                                unit->CastSpell(initialTarget, SPELL_DK_GOREFIENDS_GRASP_PULL, TRIGGERED_FULL_MASK);
                    }
                }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_gorefiends_grasp::FindTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_dk_tombstone : public SpellScript
{
    PrepareSpellScript(spell_dk_tombstone);

    SpellCastResult CheckCast()
    {
        if (!GetCaster()->HasAura(SPELL_DK_BONE_SHIELD))
            return SPELL_FAILED_SPELL_UNAVAILABLE;

        return SPELL_CAST_OK;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();
        int32 maxHealth = caster->GetMaxHealth();

        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_DK_TOMBSTONE_BUFF);
        int32 removalAmount = value->GetEffect(EFFECT_2).CalcValue(caster);
        int32 energyAmount = value->GetEffect(EFFECT_0).CalcValue(caster);
        int32 absorbPct = value->GetEffect(EFFECT_1).CalcValue(caster);

        Aura* aura = caster->GetAura(SPELL_DK_BONE_SHIELD);
        int32 stackAmount = aura->GetStackAmount();

        if (stackAmount <= removalAmount)
        {
            energyAmount *= stackAmount;
            absorbPct *= stackAmount;
            int32 absorbAmount = CalculatePct(maxHealth, absorbPct);
            caster->CastCustomSpell(caster, SPELL_DK_TOMBSTONE_BUFF, &energyAmount, &absorbAmount, nullptr, TRIGGERED_FULL_MASK);
            caster->RemoveAura(SPELL_DK_BONE_SHIELD);
        }
        else
        {
            energyAmount *= removalAmount;
            absorbPct *= removalAmount;
            int32 absorbAmount = CalculatePct(maxHealth, absorbPct);
            caster->CastCustomSpell(caster, SPELL_DK_TOMBSTONE_BUFF, &energyAmount, &absorbAmount, nullptr, TRIGGERED_FULL_MASK);
            aura->SetStackAmount(stackAmount - removalAmount);
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dk_tombstone::CheckCast);
        OnCast += SpellCastFn(spell_dk_tombstone::HandleCast);
    }
};

class spell_dk_bonestorm : public SpellScript
{
    PrepareSpellScript(spell_dk_bonestorm);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        SpellInfo const* spell = sSpellMgr->AssertSpellInfo(SPELL_DK_BONESTORM);
        uint32 healPct = spell->GetEffect(EFFECT_1).CalcValue(caster);
        int32 healAmount = targets.size() * healPct;
        if (targets.size() > 5)
            healAmount = 15;

        caster->CastCustomSpell(SPELL_DK_BONESTORM_HEAL, SPELLVALUE_BASE_POINT0, healAmount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_bonestorm::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_dk_bonestorm_duration : public AuraScript
{
    PrepareAuraScript(spell_dk_bonestorm_duration);

    void HandleEffectApply(AuraEffect const*  aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        int32 duration = caster->GetPower(POWER_RUNIC_POWER) / aurEff->GetBase()->GetEffect(EFFECT_0)->GetAmount();
        caster->SetPower(POWER_RUNIC_POWER, 0);

        aurEff->GetBase()->SetDuration(duration * 100);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dk_bonestorm_duration::HandleEffectApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_festering_strike : public SpellScript
{
    PrepareSpellScript(spell_dk_festering_strike);

    void HandleCast()
    {
        Unit* target = GetExplTargetUnit();

        uint32 randomAmount = urand(1,2);

        if (Aura* aura = target->GetAura(SPELL_DK_FESTERING_WOUND))
        {
            aura->ModStackAmount(randomAmount);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dk_festering_strike::HandleCast);
    }
};

class spell_dk_epidemic : public SpellScript
{
    PrepareSpellScript(spell_dk_epidemic);

    SpellCastResult CheckCast()
    {
        if (Unit* target = GetExplTargetUnit())
        {
            if (target->HasAura(SPELL_DK_VIRULENT_PLAGUE))
                return SPELL_CAST_OK;
        }
        else
            return SPELL_FAILED_BAD_TARGETS;
    }

    void FindTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        if (targets.size() > 0)
        {
            for (auto const& target : targets)
                if (Unit* unit = target->ToUnit())
                    if (unit->HasAura(SPELL_DK_VIRULENT_PLAGUE) && !unit->isDead())
                    {
                        unit->RemoveAura(SPELL_DK_VIRULENT_PLAGUE);
                        caster->CastSpell(unit, SPELL_DK_EPIDEMIC_SINGLE, TRIGGERED_FULL_MASK);
                        caster->CastSpell(unit, SPELL_DK_EPIDEMIC_AOE, TRIGGERED_FULL_MASK);
                    }
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_dk_epidemic::CheckCast);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_epidemic::FindTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class spell_dk_vile_contagion : public SpellScript
{
    PrepareSpellScript(spell_dk_vile_contagion);

    void CountTargets(std::list<WorldObject*>& targetList)
    {
        Unit* caster = GetCaster();

        uint32 _targetCount;

        int32 targetCount = GetSpellInfo()->Effects[EFFECT_0].CalcValue(caster);
        Acore::Containers::RandomResize(targetList, targetCount);
        _targetCount = targetList.size();

        Unit* initialTarget = GetExplTargetUnit();
        
        if (_targetCount > 0)
        {
            for (auto const& target : targetList)
                if (Unit* unit = target->ToUnit())
                    if (Aura* aura = initialTarget->GetAura(SPELL_DK_FESTERING_WOUND))
                    {
                        if (unit->isDead())
                            continue;
                        int32 stackAmount = aura->GetStackAmount();

                        caster->CastSpell(unit, SPELL_DK_FESTERING_WOUND, TRIGGERED_FULL_MASK);
                        unit->GetAura(SPELL_DK_FESTERING_WOUND)->SetStackAmount(stackAmount);
                    }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_vile_contagion::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class spell_dk_unholy_assault : public SpellScript
{
    PrepareSpellScript(spell_dk_unholy_assault);

    void HandleCast()
    {
        Unit* target = GetExplTargetUnit();

        uint32 amount = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());

        if (Aura* aura = target->GetAura(SPELL_DK_FESTERING_WOUND, GetCaster()->GetGUID()))
            aura->ModStackAmount(amount);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dk_unholy_assault::HandleCast);
    }
};


class spell_dk_bloody_strikes : public AuraScript
{
    PrepareAuraScript(spell_dk_bloody_strikes);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetActionTarget() && eventInfo.GetActionTarget()->HasAura(SPELL_DK_BLOOD_PLAGUE))
            return true;

        return false;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_bloody_strikes::CheckProc);
    }
};

class spell_dk_blood_presence : public AuraScript
{
    PrepareAuraScript(spell_dk_blood_presence);

    void HandleEffectApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, SPELL_DK_BLOOD_PRESENCE_ADDITIONAL, TRIGGERED_FULL_MASK);
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        caster->RemoveAura(SPELL_DK_BLOOD_PRESENCE_ADDITIONAL);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dk_blood_presence::HandleEffectApply, EFFECT_0, SPELL_AURA_MOD_BASE_RESISTANCE_PCT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_blood_presence::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_BASE_RESISTANCE_PCT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_frost_presence : public AuraScript
{
    PrepareAuraScript(spell_dk_frost_presence);

    void HandleEffectApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, SPELL_DK_FROST_PRESENCE_ADDITIONAL, TRIGGERED_FULL_MASK);
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        caster->RemoveAura(SPELL_DK_FROST_PRESENCE_ADDITIONAL);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dk_frost_presence::HandleEffectApply, EFFECT_0, SPELL_AURA_MOD_MELEE_HASTE , AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_frost_presence::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_MELEE_HASTE, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_unholy_presence_heal : public AuraScript
{
    PrepareAuraScript(spell_dk_unholy_presence_heal);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage)
            {
                int32 healPct = aurEff->GetAmount();
                int32 healAmount = CalculatePct(damage, healPct);
                GetCaster()->CastCustomSpell(SPELL_DK_UNHOLY_PRESENCE_HEAL, SPELLVALUE_BASE_POINT0, healAmount, GetCaster(), TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_unholy_presence_heal::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_might_of_mograine : public SpellScript
{
    PrepareSpellScript(spell_dk_might_of_mograine);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        if (AuraEffect const* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_DK_MIGHT_OF_MOGRAINE, EFFECT_0))
        {
            int32 charges = aurEff->GetAmount();

            if (Aura* aura = caster->GetAura(SPELL_DK_BONE_SHIELD))
                aura->ModStackAmount(charges);
            else
            {
                caster->CastSpell(caster, SPELL_DK_BONE_SHIELD, TRIGGERED_FULL_MASK);

                Aura* newAura = caster->GetAura(SPELL_DK_BONE_SHIELD);
                newAura->ModStackAmount(charges - 1);
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dk_might_of_mograine::HandleCast);
    }
};

class spell_dk_apocalyspe : public SpellScript
{
    PrepareSpellScript(spell_dk_apocalyspe);

    void HandleCast()
    {
        Unit* target = GetExplTargetUnit();

        if (Aura* targetAura = target->GetAura(SPELL_DK_FESTERING_WOUND))
        {
            int32 stackAmount = targetAura->GetStackAmount();
            int32 maxFesteringWound = GetSpellInfo()->GetEffect(EFFECT_2).CalcValue(GetCaster());
            int32 maxStackAmount = std::min(stackAmount, maxFesteringWound);
            targetAura->ModStackAmount(-maxStackAmount);

            for (size_t i = 0; i < stackAmount; i++) {

                GetCaster()->CastCustomSpell(SPELL_DK_ARMY_OF_THE_DEAD, SPELLVALUE_AURA_DURATION, 20000, GetCaster(), TRIGGERED_FULL_MASK);
                GetCaster()->CastSpell(target, SPELL_DK_FESTERING_WOUND_PROC, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dk_apocalyspe::HandleCast);
    }
};

class spell_dk_improved_bloodworms_health_low : public AuraScript
{
    PrepareAuraScript(spell_dk_improved_bloodworms_health_low);
    
    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Player* victim = GetTarget()->ToPlayer();
        int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
        uint32 allowedHealth = victim->CountPctFromMaxHealth(20);
        if (remainingHealth < int32(allowedHealth))
        {
            std::vector<Unit*> summonedUnits = victim->GetSummonedUnits();
            for (auto const& unit : summonedUnits)
            {
                if (!unit || !unit->IsAlive())
                    continue;

                if (Aura* aura = unit->GetAura(SPELL_DK_IMPROVED_BLOODWORMS_DEATH))
                {
                    int32 healAmount = aura->GetEffect(EFFECT_0)->GetAmount();

                    if (victim->HasAura(SPELL_DK_IMPROVED_BLOODWORMS_R1))
                        healAmount = CalculatePct(healAmount, 15);
                    else if (victim->HasAura(SPELL_DK_IMPROVED_BLOODWORMS_R2))
                        healAmount = CalculatePct(healAmount, 30);
                    else if (victim->HasAura(SPELL_DK_IMPROVED_BLOODWORMS_R3))
                        healAmount = CalculatePct(healAmount, 45);

                    unit->CastCustomSpell(SPELL_DK_IMPROVED_BLOODWORMS_HEAL, SPELLVALUE_BASE_POINT0, healAmount, victim, TRIGGERED_FULL_MASK);
                    unit->Kill(unit, unit);
                }
            }
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_improved_bloodworms_health_low::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_improved_bloodworms_health_low::Absorb, EFFECT_0);
    }
};

class spell_dk_improved_bloodworms_death : public AuraScript
{
    PrepareAuraScript(spell_dk_improved_bloodworms_death);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* owner = GetCaster()->GetOwner();

        if (!owner)
            return;

        Unit* bloodworm = GetCaster();
        int32 healAmount = 0;

        int32 damage = aurEff->GetAmount();

        if (owner->HasAura(SPELL_DK_IMPROVED_BLOODWORMS_R1))
            healAmount = CalculatePct(damage, 15);
        else if (owner->HasAura(SPELL_DK_IMPROVED_BLOODWORMS_R2))
            healAmount = CalculatePct(damage, 30);
        else if (owner->HasAura(SPELL_DK_IMPROVED_BLOODWORMS_R3))
            healAmount = CalculatePct(damage, 45);

        bloodworm->CastCustomSpell(SPELL_DK_IMPROVED_BLOODWORMS_HEAL, SPELLVALUE_BASE_POINT0, healAmount, owner, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_improved_bloodworms_death::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_contagions_periodic_tick : public AuraScript
{
    PrepareAuraScript(spell_dk_contagions_periodic_tick);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        Position position = caster->GetPosition();
        Creature* creature = caster->FindNearestCreature(NPC_CONTAGION_AREA, 10.f, true);
        float radius = GetSpellInfo()->Effects[EFFECT_0].CalcRadius(caster);
        if (creature) {
            Position pos = creature->GetPosition();
            float distance = caster->GetDistance(pos);
            if (distance <= radius)
                caster->AddAura(SPELL_DK_CONTAGIOUS_TARGET_INCREASE, caster);
            else
                caster->RemoveAura(SPELL_DK_CONTAGIOUS_TARGET_INCREASE);
        }
        else {
            caster->RemoveAura(SPELL_DK_CONTAGIOUS_TARGET_INCREASE);
        }

    }


    void HandleEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;
    }

    void Register() override
    {
        // OnEffectRemove += AuraEffectRemoveFn(spell_dk_contagions_periodic_tick::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_MASTERY_PCT, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_contagions_periodic_tick::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class spell_dk_contagious_summon : public SpellScript
{
    PrepareSpellScript(spell_dk_contagious_summon);

    void HandleSummon(SpellEffIndex effIndex)
    {
        WorldLocation const* dest = GetHitDest();
        Position pos = dest->GetPosition();

        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        Creature* summon = GetCaster()->SummonCreature(500508, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ() + 5.f, pos.GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, GetSpellInfo()->GetDuration(), properties);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_dk_contagious_summon::HandleSummon, EFFECT_2, SPELL_EFFECT_DUMMY);
    }
};

class spell_dk_deathchill : public SpellScript
{
    PrepareSpellScript(spell_dk_deathchill);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        if (caster->HasAura(SPELL_DK_DEATHCHILL_TALENT))
        {
            int32 spellID = GetSpellInfo()->Id;

            if (spellID == SPELL_DK_REMORSELESS_WINTER)
                caster->CastSpell(caster, SPELL_DK_DEATHCHILL_AOE, TRIGGERED_FULL_MASK);
            else
                caster->CastSpell(GetExplTargetUnit(), SPELL_DK_DEATHCHILL_SINGLE, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dk_deathchill::HandleCast);
    }
};

class spell_dk_rime : public AuraScript
{
    PrepareAuraScript(spell_dk_rime);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        int32 damageAmount = aurEff->GetAmount();

        int32 procSpell = eventInfo.GetProcSpell()->GetSpellInfo()->Id;

        if (procSpell == SPELL_DK_FROSTSCYTHE)
        {
            if (roll_chance_i(22))
            {
                caster->CastCustomSpell(SPELL_DK_RIME_PROC, SPELLVALUE_BASE_POINT0, damageAmount, caster, TRIGGERED_FULL_MASK);
            }
        }
        else
        {
            if (roll_chance_i(45))
            {
                caster->CastCustomSpell(SPELL_DK_RIME_PROC, SPELLVALUE_BASE_POINT0, damageAmount, caster, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_rime::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_sensitization : public AuraScript
{
    PrepareAuraScript(spell_dk_sensitization);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            Unit* target = eventInfo.GetActionTarget();
            Unit* caster = GetCaster();
            int32 amount = aurEff->GetAmount();
            SpellSchoolMask type = eventInfo.GetSpellInfo()->GetSchoolMask();

            if (type == 16) //FROST
                caster->CastCustomSpell(SPELL_DK_SENSITIZATION_FROST, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            else if (type == 32) //SHADOW
                caster->CastCustomSpell(SPELL_DK_SENSITIZATION_SHADOW, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            else if (type == 4) //FIRE
                caster->CastCustomSpell(SPELL_DK_SENSITIZATION_FIRE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            else if (type == 8) //NATURE
                caster->CastCustomSpell(SPELL_DK_SENSITIZATION_NATURE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            else if (type == 2) //HOLY
                caster->CastCustomSpell(SPELL_DK_SENSITIZATION_HOLY, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            else
                caster->CastCustomSpell(SPELL_DK_SENSITIZATION_ARCANE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_sensitization::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_contagion_replacer : public AuraScript
{
    PrepareAuraScript(spell_dk_contagion_replacer);

    void HandleLearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_DK_DEATH_AND_DECAY, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_DK_DEFILE);
    }

    void HandleUnlearn(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Player* target = GetCaster()->ToPlayer();

        target->removeSpell(SPELL_DK_DEFILE, SPEC_MASK_ALL, false);
        target->learnSpell(SPELL_DK_DEATH_AND_DECAY);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dk_contagion_replacer::HandleLearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_contagion_replacer::HandleUnlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_infected_claws : public AuraScript
{
    PrepareAuraScript(spell_dk_infected_claws);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* owner = GetCaster();
        Unit* pet = GetAura()->GetOwner()->ToUnit();
        Unit* target = eventInfo.GetActionTarget();

        if (!owner->IsAlive())
            return;

        owner->CastSpell(target, SPELL_DK_FESTERING_WOUND, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_infected_claws::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_thassarian : public AuraScript
{
    PrepareAuraScript(spell_dk_thassarian);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = eventInfo.GetActionTarget();
        int32 spellId = eventInfo.GetSpellInfo()->Id;

        if (spellId == SPELL_DK_DEATH_STRIKE)
        {
            caster->CastSpell(target, SPELL_DK_DEATH_STRIKE_THASSARIAN, TRIGGERED_FULL_MASK);
        }
        else if (spellId == SPELL_DK_FROST_STRIKE)
        {
            caster->CastSpell(target, SPELL_DK_FROST_STRIKE_THASSARIAN, TRIGGERED_FULL_MASK);
        }
        else if (spellId == SPELL_DK_OBLITERATE)
        {
            caster->CastSpell(target, SPELL_DK_OBLITERATE_THASSARIAN, TRIGGERED_FULL_MASK);
        }
        else if (spellId == SPELL_DK_PLAGUE_STRIKE)
        {
            caster->CastSpell(target, SPELL_DK_PLAGUE_STRIKE_THASSARIAN, TRIGGERED_FULL_MASK);
        }
        else if (spellId == SPELL_DK_FROSTSCYTHE)
        {
            caster->CastSpell(target, SPELL_DK_FROSTSCYTHE_THASSARIAN, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dk_thassarian::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_deaths_embrace : public AuraScript
{
    PrepareAuraScript(spell_dk_deaths_embrace);

    std::list <Unit*> FindTargets(int32 spellId)
    {
        Player* caster = GetCaster()->ToPlayer();
        std::list <Unit*> targetAvailable;
        auto const& allyList = caster->GetGroup()->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* player = ObjectAccessor::FindPlayer(target.guid);
            if (player)
                if (player->HasAura(spellId))
                    if (player->GetAura(spellId)->GetCasterGUID() == GetCaster()->GetGUID())
                    {
                        Unit* dummy = player->ToUnit();
                        if (dummy)
                            targetAvailable.push_back(dummy);
                    }
        }
        return targetAvailable;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player->GetGroup() || !player || !player->IsAlive())
            return false;
        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();

            int32 heal = CalculatePct(damage, aurEff->GetAmount());

            for (auto const& targetheal : FindTargets(SPELL_DK_DEATHS_EMBRACE))
            {
                GetCaster()->CastCustomSpell(SPELL_DK_DEATHS_EMBRACE_HEAL, SPELLVALUE_BASE_POINT0, heal, targetheal, true, nullptr);
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_deaths_embrace::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_deaths_embrace::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_deaths_embrace_listener : public SpellScript
{
    PrepareSpellScript(spell_dk_deaths_embrace_listener);

    void HandleProc()
    {
        GetCaster()->CastSpell(GetCaster(), SPELL_DK_DEATHS_EMBRACE_LISTENER, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnCast += SpellCastFn(spell_dk_deaths_embrace_listener::HandleProc);
    }
};

class spell_dk_ebon_renewal : public SpellScript
{
    PrepareSpellScript(spell_dk_ebon_renewal);

    void HandleHit(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        SpellInfo const* spell = GetSpellInfo();
        int32 heal = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), GetEffectValue());
        int32 hotDuration = sSpellMgr->AssertSpellInfo(SPELL_DK_EBON_RENEWAL_HEAL)->GetDuration();
        int32 hotRate = sSpellMgr->AssertSpellInfo(SPELL_DK_EBON_RENEWAL_HEAL)->GetEffect(EFFECT_1).Amplitude;
        int32 runicPower = caster->GetPower(POWER_RUNIC_POWER);
        int32 consumption = spell->GetEffect(EFFECT_1).CalcValue(caster);
        int32 hotPct = spell->GetEffect(EFFECT_2).CalcValue(caster);

        if (runicPower >= 0)
        {
            int32 bonusPercent = std::min<int32>(runicPower, consumption) + 200;
            heal = (bonusPercent / 10) * heal;

            caster->ModifyPower(POWER_RUNIC_POWER, -(bonusPercent - 200));
        }

        if (Unit* target = GetHitUnit())
        {
            heal = caster->SpellHealingBonusDone(target, GetSpellInfo(), uint32(heal), HEAL, effIndex);
            heal = target->SpellHealingBonusTaken(caster, GetSpellInfo(), uint32(heal), HEAL);

            if (Aura* shield = caster->GetAura(SPELL_DK_LIFE_AND_DEATH_SHIELD))
            {
                if (Aura* aura = caster->GetAura(SPELL_DK_LIFE_AND_DEATH))
                {
                    int32 mastery = caster->ToPlayer()->GetMastery();
                    int32 consumePct = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;
                    int32 shieldAmount = shield->GetEffect(EFFECT_0)->GetAmount();
                    int32 consumeAmount = caster->CountPctFromMaxHealth(consumePct);

                    int32 amount = std::min(consumeAmount, shieldAmount);

                    heal += CalculatePct(heal, amount);

                    shield->GetEffect(EFFECT_0)->ChangeAmount(-amount);

                    if (shield->GetEffect(EFFECT_0)->GetAmount() <= 0)
                        shield->Remove();
                }
            }

            if (AuraEffect const* mercifulCombat = caster->GetAuraEffectOfRankedSpell(TALENT_DK_MERCIFUL_COMBAT, EFFECT_0))
            {
                if (target->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT))
                {
                    int32 bonus = mercifulCombat->GetAmount();
                    heal += CalculatePct(heal, bonus);
                }
            }

            int32 healPerTick = heal / (hotDuration / hotRate);
            ApplyPct(healPerTick, hotPct);

            healPerTick = std::max<int32>(1, healPerTick);

            caster->CastCustomSpell(target, SPELL_DK_EBON_RENEWAL_HEAL, &heal, &healPerTick, nullptr, true, nullptr);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_ebon_renewal::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_dk_face_of_death : public AuraScript
{
    PrepareAuraScript(spell_dk_face_of_death);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        caster->SetDisplayId(32759);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        GetCaster()->DeMorph();
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dk_face_of_death::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_face_of_death::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_leeching_strike_energy : public SpellScript
{
    PrepareSpellScript(spell_dk_leeching_strike_energy);

    void HandleCast()
    {
        GetCaster()->CastSpell(GetCaster(), SPELL_DK_LEECHING_STRIKE_ENERGY, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dk_leeching_strike_energy::HandleCast);
    }
};

class spell_dk_lifedrain_bolt : public SpellScript
{
    PrepareSpellScript(spell_dk_lifedrain_bolt);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Acore::RaidCheck(GetCaster(), false));

        uint32 const maxTargets = GetSpellInfo()->GetEffect(EFFECT_2).CalcValue(GetCaster());

        if (targets.size() > maxTargets)
        {
            targets.sort(Acore::HealthPctOrderPred());
            targets.resize(maxTargets);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_lifedrain_bolt::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

class spell_dk_necrotic_blessing : public SpellScript
{
    PrepareSpellScript(spell_dk_necrotic_blessing);

    void HandleHit(SpellEffIndex effIndex)
    {
        if (GetHitUnit()->HasAuraState(AURA_STATE_HEALTHLESS_20_PERCENT))
            GetCaster()->CastSpell(GetCaster(), SPELL_DK_NECROTIC_BLESSING_HEAL, TRIGGERED_FULL_MASK);

        if (Unit* target = GetHitUnit())
        {
            Unit* caster = GetCaster();
            int32 heal = GetEffectValue();

            heal = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), heal);

            heal = caster->SpellHealingBonusDone(target, GetSpellInfo(), uint32(heal), SPELL_DIRECT_DAMAGE, effIndex);
            heal = target->SpellHealingBonusTaken(caster, GetSpellInfo(), uint32(heal), SPELL_DIRECT_DAMAGE);

            if (Aura* shield = caster->GetAura(SPELL_DK_LIFE_AND_DEATH_SHIELD))
            {
                if (Aura* aura = caster->GetAura(SPELL_DK_LIFE_AND_DEATH))
                {
                    int32 mastery = caster->ToPlayer()->GetMastery();
                    int32 consumePct = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;
                    int32 shieldAmount = shield->GetEffect(EFFECT_0)->GetAmount();
                    int32 consumeAmount = caster->CountPctFromMaxHealth(consumePct);

                    int32 amount = std::min(consumeAmount, shieldAmount);

                    heal += CalculatePct(heal, amount);

                    shield->GetEffect(EFFECT_0)->ChangeAmount(-amount);

                    if (shield->GetEffect(EFFECT_0)->GetAmount() <= 0)
                        shield->Remove();
                }
            }

            SetHitHeal(heal);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_necrotic_blessing::HandleHit, EFFECT_0, SPELL_EFFECT_HEAL);
    }
};

class spell_dk_soul_barrier : public AuraScript
{
    PrepareAuraScript(spell_dk_soul_barrier);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster() && GetCaster()->IsAlive())
        {
            Unit* target = GetAura()->GetOwner()->ToUnit();
            target->CastSpell(target, SPELL_DK_SOUL_BARRIER_HEAL, TRIGGERED_FULL_MASK, nullptr, nullptr, GetCaster()->GetGUID());

        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_dk_soul_barrier::HandleRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_dk_soul_link : public AuraScript
{
    PrepareAuraScript(spell_dk_soul_link);

    std::list <Unit*> FindTargets(int32 spellId)
    {
        Player* caster = GetCaster()->ToPlayer();
        std::list <Unit*> targetAvailable;
        auto const& allyList = caster->GetGroup()->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* player = ObjectAccessor::FindPlayer(target.guid);
            if (player)
                if (player->HasAura(spellId))
                    if (player->GetAura(spellId)->GetCasterGUID() == GetCaster()->GetGUID())
                    {
                        Unit* dummy = player->ToUnit();
                        if (dummy)
                            targetAvailable.push_back(dummy);
                    }
        }
        return targetAvailable;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player->GetGroup() || !player || !player->IsAlive())
            return false;
        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();

            int32 heal = CalculatePct(damage, aurEff->GetAmount());

            for (auto const& targetheal : FindTargets(SPELL_DK_SOUL_LINK))
            {
                GetCaster()->CastCustomSpell(SPELL_DK_SOUL_LINK_HEAL, SPELLVALUE_BASE_POINT0, heal, targetheal, true, nullptr);
            }
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_soul_link::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_soul_link::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_soul_link_listener : public SpellScript
{
    PrepareSpellScript(spell_dk_soul_link_listener);

    void HandleProc()
    {
        GetCaster()->CastSpell(GetCaster(), SPELL_DK_SOUL_LINK_LISTENER, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnCast += SpellCastFn(spell_dk_soul_link_listener::HandleProc);
    }
};

class spell_dk_soulbolt : public AuraScript
{
    PrepareAuraScript(spell_dk_soulbolt);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player || !player->IsAlive())
            return false;
        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            uint32 count = eventInfo.GetActionTarget()->GetDiseasesByCaster(GetCaster()->GetGUID());
            int32 amount = aurEff->GetAmount() * count;
            int32 heal = CalculatePct(damage, amount);
            
            GetCaster()->CastCustomSpell(SPELL_DK_SOULBOLT_HEAL, SPELLVALUE_BASE_POINT0, heal, GetCaster(), true, nullptr);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_soulbolt::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_soulbolt::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_vitality_burst : public SpellScript
{
    PrepareSpellScript(spell_dk_vitality_burst);

    void HandleHeal(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
        {
            Unit* caster = GetCaster();
            int32 heal = GetEffectValue();

            heal = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), heal);

            heal = caster->SpellHealingBonusDone(target, GetSpellInfo(), uint32(heal), SPELL_DIRECT_DAMAGE, effIndex);
            heal = target->SpellHealingBonusTaken(caster, GetSpellInfo(), uint32(heal), SPELL_DIRECT_DAMAGE);

            if (Aura* shield = caster->GetAura(SPELL_DK_LIFE_AND_DEATH_SHIELD))
            {
                if (Aura* aura = caster->GetAura(SPELL_DK_LIFE_AND_DEATH))
                {
                    int32 mastery = caster->ToPlayer()->GetMastery();
                    int32 consumePct = aura->GetEffect(EFFECT_1)->GetAmount() + mastery;
                    int32 shieldAmount = shield->GetEffect(EFFECT_0)->GetAmount();
                    int32 consumeAmount = caster->CountPctFromMaxHealth(consumePct);

                    int32 amount = std::min(consumeAmount, shieldAmount);

                    heal += CalculatePct(heal, amount);

                    shield->GetEffect(EFFECT_0)->ChangeAmount(-amount);

                    if (shield->GetEffect(EFFECT_0)->GetAmount() <= 0)
                        shield->Remove();
                }
            }

            if (AuraEffect const* mercifulCombat = caster->GetAuraEffectOfRankedSpell(TALENT_DK_MERCIFUL_COMBAT, EFFECT_0))
            {
                if (target->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT))
                {
                    int32 bonus = mercifulCombat->GetAmount();
                    heal += CalculatePct(heal, bonus);
                }
            }

            SetHitHeal(heal);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dk_vitality_burst::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
    }
};

class spell_dk_necrotic_presence : public AuraScript
{
    PrepareAuraScript(spell_dk_necrotic_presence);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
         if (eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == TALENT_DK_NECROTIC_PRESENCE_PROC)
            return false;

        return (player && player->IsAlive());
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            int32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
            int32 heal = CalculatePct(damage, aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount());

            GetCaster()->CastCustomSpell(eventInfo.GetActionTarget(), TALENT_DK_NECROTIC_PRESENCE_PROC, &damage, &heal, nullptr, true, nullptr);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_necrotic_presence::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_necrotic_presence::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_improved_leeching_strike : public AuraScript
{
    PrepareAuraScript(spell_dk_improved_leeching_strike);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player->HasAura(SPELL_DK_SOUL_PRESENCE))
            return false;

        return (player && player->IsAlive());
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            GetCaster()->CastSpell(eventInfo.GetActionTarget(), TALENT_DK_IMPROVED_LEECHING_STRIKE_PROC, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_improved_leeching_strike::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_improved_leeching_strike::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_necrotic_vengeance : public AuraScript
{
    PrepareAuraScript(spell_dk_necrotic_vengeance);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster() && GetCaster()->IsAlive());
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 amount = aurEff->GetAmount();
        if (eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0)
            GetCaster()->CastCustomSpell(TALENT_DK_NECROTIC_VENGEANCE_DAMAGE_BUFF, SPELLVALUE_BASE_POINT0, amount, GetCaster(), true, nullptr);
        else if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
            GetCaster()->CastCustomSpell(TALENT_DK_NECROTIC_VENGEANCE_HEAL_BUFF, SPELLVALUE_BASE_POINT0, amount, GetCaster(), true, nullptr);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_necrotic_vengeance::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_necrotic_vengeance::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_sudden_salvation : public AuraScript
{
    PrepareAuraScript(spell_dk_sudden_salvation);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return (GetCaster() && GetCaster()->IsAlive());
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0)
        {
            GetCaster()->CastSpell(eventInfo.GetActionTarget(), TALENT_DK_SUDDEN_SALVATION_LIFEBOLT, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_sudden_salvation::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_sudden_salvation::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dk_death_and_renew : public SpellScript
{
    PrepareSpellScript(spell_dk_death_and_renew);

    void HandleProc()
    {
        if (GetCaster()->HasAura(TALENT_DK_DEATH_AND_RENEW))
        {
            GetCaster()->CastSpell(GetExplTargetDest()->GetPositionX(), GetExplTargetDest()->GetPositionY(), GetExplTargetDest()->GetPositionZ(), TAlENT_DK_DEATH_AND_RENEW_HEAL, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnCast += SpellCastFn(spell_dk_death_and_renew::HandleProc);
    }
};

class spell_dk_necrotic_protection : public AuraScript
{
    PrepareAuraScript(spell_dk_necrotic_protection);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0)
        {
            if ((eventInfo.GetActionTarget()->GetMaxHealth() - eventInfo.GetActionTarget()->GetHealth()) > eventInfo.GetHealInfo()->GetHeal())
                return false;

            return (GetCaster() && GetCaster()->IsAlive());
        }
        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastCustomSpell(TALENT_DK_NECROTIC_PROTECTION_PROC, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), eventInfo.GetProcTarget(), true, nullptr);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dk_necrotic_protection::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dk_necrotic_protection::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

void AddSC_deathknight_spell_scripts()
{
    RegisterSpellScript(spell_dk_wandering_plague);
    //RegisterSpellScript(spell_dk_raise_ally);
    //RegisterSpellScript(spell_dk_raise_ally_trigger);
    RegisterSpellScript(spell_dk_aotd_taunt);
    RegisterSpellScript(spell_dk_master_of_ghouls);
    RegisterSpellAndAuraScriptPair(spell_dk_chains_of_ice, spell_dk_chains_of_ice_aura);
    RegisterSpellScript(spell_dk_bloodworms);
    RegisterSpellScript(spell_dk_summon_gargoyle);
    RegisterSpellScript(spell_dk_wandering_plague_aura);
    RegisterSpellScript(spell_dk_rune_of_the_fallen_crusader);
    RegisterSpellScript(spell_dk_bone_shield);
    RegisterSpellScript(spell_dk_hungering_cold);
    //RegisterSpellScript(spell_dk_blood_caked_blade);
    RegisterSpellScript(spell_dk_dancing_rune_weapon);
    RegisterSpellScript(spell_dk_dancing_rune_weapon_visual);
    RegisterSpellScript(spell_dk_scent_of_blood_trigger);
    RegisterSpellScript(spell_dk_pet_scaling);
    RegisterSpellScript(spell_dk_anti_magic_shell_raid);
    RegisterSpellScript(spell_dk_anti_magic_shell_self);
    RegisterSpellScript(spell_dk_anti_magic_zone);
    RegisterSpellScript(spell_dk_blood_boil);
    RegisterSpellScript(spell_dk_blood_gorged);
    RegisterSpellScript(spell_dk_corpse_explosion);
    RegisterSpellScript(spell_dk_death_coil);
    RegisterSpellScript(spell_dk_death_gate);
    RegisterSpellScript(spell_dk_death_grip);
    RegisterSpellScript(spell_dk_death_strike);
    RegisterSpellScript(spell_dk_ghoul_explode);
    RegisterSpellScript(spell_dk_icebound_fortitude);
    RegisterSpellScript(spell_dk_pestilence);
    //RegisterSpellScript(spell_dk_raise_dead);
    RegisterSpellScript(spell_dk_rune_tap_party);
    RegisterSpellScript(spell_dk_scent_of_blood);
    //RegisterSpellScript(spell_dk_scourge_strike);
    RegisterSpellScript(spell_dk_spell_deflection);
    RegisterSpellScript(spell_dk_vampiric_blood);
    RegisterSpellScript(spell_dk_will_of_the_necropolis);
    RegisterSpellScript(spell_dk_blood_strike);
    RegisterSpellScript(spell_dk_bone_shield_calculation);
    RegisterSpellScript(spell_dk_death_pact);
    RegisterSpellScript(spell_dk_pillar_of_frost);
    RegisterSpellScript(spell_dk_chill_streak);
    RegisterSpellScript(spell_dk_frostwyrm);
    RegisterSpellScript(spell_dk_frost_fever);
    RegisterSpellScript(spell_dk_lichborne_leech);
    RegisterSpellScript(spell_dk_raise_dead_new);
    RegisterSpellScript(spell_dk_obliterate);
    RegisterSpellScript(spell_dk_festering_wound);
    RegisterSpellScript(spell_dk_scourge_strike_new);
    RegisterSpellScript(spell_dk_summon_gargoyle_energy);
    RegisterSpellScript(spell_dk_summon_gargoyle_power);
    RegisterSpellScript(spell_dk_virulent_plague);
    RegisterSpellScript(spell_dk_breath_of_sindragosa);
    RegisterSpellScript(spell_dk_sacrifical_pact);
    RegisterSpellScript(spell_dk_asphyxiate);
    RegisterSpellScript(spell_dk_soul_reaper);
    RegisterSpellScript(spell_dk_rune_of_sanguination);
    RegisterSpellScript(spell_dk_rune_of_sanguination_heal);
    RegisterSpellScript(spell_dk_rune_of_spellwarding);
    RegisterSpellScript(spell_dk_rune_of_apocalypse);
    RegisterSpellScript(spell_dk_remove_impairing_auras);
    RegisterSpellScript(spell_dk_deaths_caress);
    RegisterSpellScript(spell_dk_marrowrend);
    RegisterSpellScript(spell_dk_gorefiends_grasp);
    RegisterSpellScript(spell_dk_tombstone);
    RegisterSpellScript(spell_dk_bonestorm);
    RegisterSpellScript(spell_dk_bonestorm_duration);
    RegisterSpellScript(spell_dk_festering_strike);
    RegisterSpellScript(spell_dk_epidemic);
    RegisterSpellScript(spell_dk_vile_contagion);
    RegisterSpellScript(spell_dk_unholy_assault);
    RegisterSpellScript(spell_dk_bloody_strikes);
    RegisterSpellScript(spell_dk_blood_presence);
    RegisterSpellScript(spell_dk_frost_presence);
    RegisterSpellScript(spell_dk_unholy_presence_heal);
    RegisterSpellScript(spell_dk_dark_transformation);
    RegisterSpellScript(spell_dk_dark_transformation_expire);
    RegisterSpellScript(spell_dk_might_of_mograine);
    RegisterSpellScript(spell_dk_improved_bloodworms_health_low);
    RegisterSpellScript(spell_dk_improved_bloodworms_death);
	RegisterSpellScript(spell_dk_deathchill);
    RegisterSpellScript(spell_dk_rime);
	RegisterSpellScript(spell_dk_apocalyspe);
    RegisterSpellScript(spell_dk_sensitization);
    RegisterSpellScript(spell_dk_apocalyspe);
    RegisterSpellScript(spell_dk_contagions_periodic_tick);
    RegisterSpellScript(spell_dk_contagious_summon);
    RegisterSpellScript(spell_dk_contagion_replacer);
    RegisterSpellScript(spell_dk_infected_claws);
    RegisterSpellScript(spell_dk_thassarian);
    RegisterSpellScript(spell_dk_deaths_embrace);
    RegisterSpellScript(spell_dk_deaths_embrace_listener);
    RegisterSpellScript(spell_dk_ebon_renewal);
    RegisterSpellScript(spell_dk_face_of_death);
    RegisterSpellScript(spell_dk_leeching_strike_energy);
    RegisterSpellScript(spell_dk_lifedrain_bolt);
    RegisterSpellScript(spell_dk_necrotic_blessing);
    RegisterSpellScript(spell_dk_soul_barrier);
    RegisterSpellScript(spell_dk_soul_link);
    RegisterSpellScript(spell_dk_soul_link_listener);
    RegisterSpellScript(spell_dk_soulbolt);
    RegisterSpellScript(spell_dk_vitality_burst);
    RegisterSpellScript(spell_dk_necrotic_presence);
    RegisterSpellScript(spell_dk_improved_leeching_strike);
    RegisterSpellScript(spell_dk_necrotic_vengeance);
    RegisterSpellScript(spell_dk_sudden_salvation);
    RegisterSpellScript(spell_dk_death_and_renew);
    RegisterSpellScript(spell_dk_necrotic_protection);
    new npc_dk_spell_glacial_advance();
    new npc_dk_spell_frostwyrm();
}
