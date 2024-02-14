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
  * Ordered alphabetically using scriptname.
  * Scriptnames of files in this file should be prefixed with "npc_pet_pri_".
  */

#include "Cell.h"
#include "CellImpl.h"
#include "CombatAI.h"
#include "GridNotifiers.h"
#include "PassiveAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"

enum WarlockSpells
{
    // Spells
    SPELL_WARLOCK_DEMONIC_CORE = 83027,
    SPELL_WARLOCK_DEMONIC_CORE_BUFF = 83029,
    SPELL_GRIMOIRE_FELGUARD_INCREASE_DAMAGE = 83031,

    // Talents
    TALENT_WARLOCK_MOLTEN_HAND = 47245,
    TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R1 = 83077,
    TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R2 = 83078,
    TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R3 = 83079,
    TALENT_WARLOCK_IMPROVED_FELHUNTER_R1 = 54037,
    TALENT_WARLOCK_IMPROVED_FELHUNTER_R2 = 54038,
    TALENT_WARLOCK_IMPROVED_FELHUNTER_LISTENER = 54425,

    // Pet Spells
    SPELL_WILDIMP_FIREBOLT = 83003,
    SPELL_DREADBITE = 83212,
    SPELL_HEADBUTT = 83216,
    SPELL_VILE_SPIT = 83218,
    SPELL_DEMONFIRE = 83210,
    SPELL_SHADOW_BLAST = 83208,
    SPELL_ACIDE_BOMB = 83058,
    SPELL_FEL_FLAME_BREATH = 83220,
    SPELL_DOOM_BOLT = 83211,

    // Runes
    RUNE_WARLOCK_INQUISITORS_GAZE_AURA = 800106,
    RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER = 800716,
    RUNE_WARLOCK_IMP_GANG_BOSS_BUFF = 800857,
    RUNE_WARLOCK_STOLEN_POWER_PET_LISTENER = 800891,
    RUNE_WARLOCK_DEMONIC_SERVITUDE = 800856,
    RUNE_WARLOCK_GULDANS_AMBITION_LISTENER = 800876,
    RUNE_WARLOCK_GULDANS_AMBITION_BUFF = 800877,

    // Sets
    T1_WARLOCK_DEMONO_4PC = 98101,
    T1_WARLOCK_DEMONO_4PC_GRIMOIRE_BUFF = 98102,
};

enum PET_WARLOCKS {

    PET_DARKHOUND = 600600,
    PET_WILDIMP = 600601,
    PET_FELBOAR = 600602,
    PET_DEMONIC_TYRAN = 600603,
};

enum WarlockEvents
{
    EVENT_TRY_ATTACK_NEW_TARGET = 1,
    EVENT_WARLOCK_CAST_SPELL = 2,
};

// 17252 - Main Felguard - PET
struct npc_pet_warlock_felguard : public ScriptedAI
{
    npc_pet_warlock_felguard(Creature* creature) : ScriptedAI(creature) { }

    Aura* GetAntoranArmamentsAura(Unit* caster)
    {
        for (size_t i = 800696; i < 800702; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetImmutableHatredAura(Unit* caster)
    {
        for (size_t i = 800704; i < 800710; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        if (Aura* runeAura = GetAntoranArmamentsAura(owner))
        {
            int32 procSpell = runeAura->GetEffect(EFFECT_1)->GetAmount();
            me->AddAura(procSpell, me);
        }

        if (Aura* runeAura = GetImmutableHatredAura(owner))
            me->AddAura(RUNE_WARLOCK_IMMUTABLE_HATRED_LISTENER, me);

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }
};

// 417 - Felhunter - PET
struct npc_pet_warlock_felhunter : public ScriptedAI
{
    npc_pet_warlock_felhunter(Creature* creature) : ScriptedAI(creature) { }

    Aura* GetImprovedFelhunterAura(Unit* caster)
    {
        if (caster->HasAura(TALENT_WARLOCK_IMPROVED_FELHUNTER_R1))
            return caster->GetAura(TALENT_WARLOCK_IMPROVED_FELHUNTER_R1);

        if (caster->HasAura(TALENT_WARLOCK_IMPROVED_FELHUNTER_R2))
            return caster->GetAura(TALENT_WARLOCK_IMPROVED_FELHUNTER_R2);

        return nullptr;
    }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        // Add Improved Felhunter listener
        if (GetImprovedFelhunterAura(owner))
            me->AddAura(TALENT_WARLOCK_IMPROVED_FELHUNTER_LISTENER, me);

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }
};

// 416 - Imp - PET
struct npc_pet_warlock_imp : public ScriptedAI
{
    npc_pet_warlock_imp(Creature* creature) : ScriptedAI(creature) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }
};

// 1863 - Succubus - PET
struct npc_pet_warlock_succubus : public ScriptedAI
{
    npc_pet_warlock_succubus(Creature* creature) : ScriptedAI(creature) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }
};

// 1860 - Voidwalker - PET
struct npc_pet_warlock_voidwalker : public ScriptedAI
{
    npc_pet_warlock_voidwalker(Creature* creature) : ScriptedAI(creature) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }
};

struct npc_pet_warlock_wildimp : public ScriptedAI
{
    npc_pet_warlock_wildimp(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    Aura* GetBloodboundImpsAura(Unit* caster)
    {
        for (size_t i = 800788; i < 800794; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetImpGangBossAura(Unit* caster)
    {
        for (size_t i = 800800; i < 800806; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetTheExpendablesAura(Unit* caster)
    {
        for (size_t i = 800806; i < 800812; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetStolenPowerAura(Unit* caster)
    {
        for (size_t i = 800818; i < 800824; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        if (Player* player = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            AuraEffect* aura = player->GetAuraEffectOfRankedSpell(TALENT_WARLOCK_MOLTEN_HAND, EFFECT_0);
            if (aura) {
                uint32 rank = aura->GetAmount();
                if (rank == 1)
                    me->CastSpell(me, TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R1);
                else if (rank == 2)
                    me->CastSpell(me, TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R2);
                else
                    me->CastSpell(me, TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R3);
            }

            // Transform Wild Imp into Gang Boss
            if (Aura* runeAura = GetImpGangBossAura(player))
            {
                int32 procChance = runeAura->GetEffect(EFFECT_0)->GetAmount();

                if (roll_chance_i(procChance))
                    me->AddAura(RUNE_WARLOCK_IMP_GANG_BOSS_BUFF, me);
            }

            // Add Stolen Power rune buff
            if (Aura* runeAura = GetStolenPowerAura(player))
                me->AddAura(RUNE_WARLOCK_STOLEN_POWER_PET_LISTENER, me);

            // Add a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(player))
            {
                int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount();

                if (Aura* demonicServitude = player->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(stackIncrease);
                else
                    player->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, player, TRIGGERED_FULL_MASK);
            }
        }
    }

    void EnterCombat(Unit*) override
    {
        attackCount = 0;
        _events.Reset();
        _events.ScheduleEvent(1, 500);
    }

    bool IsDemonicTyrantSummoned()
    {
        bool isSummoned = false;
        if (Unit* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself()) {
            for (Unit::ControlSet::const_iterator itr = owner->m_Controlled.begin(); itr != owner->m_Controlled.end(); ++itr)
                if ((*itr)->IsAlive() && (*itr)->GetEntry() == PET_DEMONIC_TYRAN)
                {
                    isSummoned = true;
                    break;
                }
        }

        return isSummoned;
    }

    void Reset() override
    {
        if (me->IsAlive())
        {
            float angle = me->ToTempSummon()->GetAngle();
            float dist = me->ToTempSummon()->GetDist();

            if (Unit* unit = me->GetCharmerOrOwnerPlayerOrPlayerItself()) {
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveFollow(unit, dist, angle, MOTION_SLOT_ACTIVE);
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* target = owner->GetSelectedUnit())
                    {
                        if (me->CanCreatureAttack(target))
                        {
                            me->CastSpell(target, SPELL_WILDIMP_FIREBOLT);
                        }
                    }
                }
                _events.ScheduleEvent(1, 2000);
                break;
            default:
                break;
            }
        }
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            int32 procChance = 10.f;

            if (Aura* demonicCoreBuff = owner->GetAura(SPELL_WARLOCK_DEMONIC_CORE))
            {
                procChance += demonicCoreBuff->GetEffect(EFFECT_0)->GetAmount();

                if (Aura* runeAura = GetBloodboundImpsAura(owner))
                {
                    int32 procIncrease = runeAura->GetEffect(EFFECT_0)->GetAmount();
                    procChance += procIncrease;
                }
            }

            if (roll_chance_i(procChance))
            {
                if (Aura* aura = owner->GetAura(SPELL_WARLOCK_DEMONIC_CORE_BUFF))
                    aura->SetCharges(aura->GetCharges() + 1);
                else
                    owner->CastCustomSpell(SPELL_WARLOCK_DEMONIC_CORE_BUFF, SPELLVALUE_AURA_CHARGE, 1, owner, true);
            }

            // Imp Gang Boss summon another Imp
            if (me->HasAura(RUNE_WARLOCK_IMP_GANG_BOSS_BUFF))
            {
                TempSummon* summon = owner->SummonCreatureGuardian(me->GetEntry(), owner, owner, 30000, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
                if (summon)
                    summon->SetPositionReset(PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
            }

            // The Expendables rune - When despawned buff your other pet's damage
            if (Aura* runeAura = GetTheExpendablesAura(owner))
            {
                int32 procSpell = runeAura->GetEffect(EFFECT_0)->GetAmount();
                auto summonedUnits = owner->m_Controlled;

                for (auto const& unit : summonedUnits)
                {
                    if (!unit || unit->isDead())
                        continue;

                    owner->CastSpell(unit, procSpell, TRIGGERED_FULL_MASK);
                }
            }

            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount();

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }

private:
    EventMap _events;
    bool _initAttack;
    uint32 attackCount;
};

struct npc_pet_warlock_bomber : public ScriptedAI
{
    npc_pet_warlock_bomber(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        me->SetCanFly(true);
        me->SetDisableGravity(true);

        float tz = me->GetMapHeight(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), true, MAX_FALL_DISTANCE);
        me->GetMotionMaster()->MoveCharge(me->GetPositionX(), me->GetPositionY(), tz, 7.0f, 1);


        if (Player* player = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Add a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(player))
            {
                int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount();

                if (Aura* demonicServitude = player->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(stackIncrease);
                else
                    player->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, player, TRIGGERED_FULL_MASK);
            }
        }
    }

    void EnterCombat(Unit*) override
    {
        attackCount = 0;
        _events.Reset();
        _events.ScheduleEvent(1, 1000);
    }

    void Reset() override
    {
        if (me->IsAlive())
        {
            float angle = me->ToTempSummon()->GetAngle();
            float dist = me->ToTempSummon()->GetDist();

            if (Unit* unit = me->GetCharmerOrOwnerPlayerOrPlayerItself()) {
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveFollow(unit, dist, angle, MOTION_SLOT_ACTIVE);
            }
        }
    }


    void AttackBomb()
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            if (Unit* target = owner->GetSelectedUnit())
            {
                if (me->CanCreatureAttack(target))
                {
                    me->CastSpell(target, SPELL_ACIDE_BOMB);
                    AttackStart(target);
                }
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                AttackBomb();
                _events.ScheduleEvent(1, 1000);
                break;
            default:
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount();

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }

private:
    EventMap _events;
    bool _initAttack;
    uint32 attackCount;
};

struct npc_pet_warlock_darkglare : public ScriptedAI
{
    npc_pet_warlock_darkglare(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void EnterCombat(Unit*) override
    {
        _events.Reset();
        _events.ScheduleEvent(1, 0);
    }

    void Reset() override
    {
        if (me->IsAlive())
        {
            float angle = me->ToTempSummon()->GetAngle();
            float dist = me->ToTempSummon()->GetDist();

            if (Unit* unit = me->GetCharmerOrOwnerPlayerOrPlayerItself()) {
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveFollow(unit, dist, angle, MOTION_SLOT_ACTIVE);
            }
        }
    }


    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (Unit* target = owner->GetSelectedUnit())
                {
                    if (me->CanCreatureAttack(target))
                    {
                        AttackStart(target);
                        _initAttack = false;
                    }
                }
            }
        }

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    me->CastSpell(me->GetVictim(), SPELL_SHADOW_BLAST);
                }
                _events.ScheduleEvent(1, 2000);
                break;
            default:
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }

private:
    EventMap _events;
    bool _initAttack;
};



class spell_warl_darkglare_spell : public SpellScriptLoader
{
public:
    spell_warl_darkglare_spell() : SpellScriptLoader("spell_warl_darkglare_spell") { }

    class spell_warl_darkglareSpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_darkglareSpellScript);

        uint8 GetCountDamageOvertime()
        {
            Unit* victim = GetCaster()->GetVictim();
            auto auras = victim->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);

            auras.remove_if([&](AuraEffect const* effect) -> bool
                {
                    return effect->GetCasterGUID() == GetCaster()->GetGUID();
                });

            return auras.size();
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            int32 damage = GetEffectValue();

            int32 damageIncreasePerEffect = 25.f;
            float pct = GetCountDamageOvertime() * damageIncreasePerEffect;
            damage = AddPct(damage, pct);

            SetEffectValue(damage);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_warl_darkglareSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_darkglareSpellScript();
    }
};

struct npc_pet_warlock_vilefiend : public ScriptedAI
{
    npc_pet_warlock_vilefiend(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);
        _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 2000);
        hasCharged = false;

        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount();

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void AttackTarget(Unit* target)
    {
        if (hasCharged == false)
        {
            me->CastSpell(target, SPELL_HEADBUTT);
            hasCharged = true;
            AttackStart(target);
        }
        else
        {
            me->JumpTo(target, 9.375f);
            AttackStart(target);
        }
    }

    void Reset() override
    {
        if (me->IsAlive())
        {
            float angle = me->ToTempSummon()->GetAngle();
            float dist = me->ToTempSummon()->GetDist();

            if (Unit* unit = me->GetCharmerOrOwnerPlayerOrPlayerItself()) {
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveFollow(unit, dist, angle, MOTION_SLOT_ACTIVE);
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (Unit* target = owner->GetSelectedUnit())
                {
                    if (me->CanCreatureAttack(target) && owner->IsInCombat())
                    {
                        AttackTarget(target);
                        _initAttack = false;
                    }
                }
            }
        }

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_TRY_ATTACK_NEW_TARGET:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* newTarget = owner->GetSelectedUnit())
                    {
                        if (Unit* victim = me->GetVictim()) {
                            if (victim->GetGUID() != newTarget->GetGUID() && owner->IsInCombatWith(victim))
                            {
                                if (me->CanCreatureAttack(newTarget))
                                    AttackTarget(newTarget);
                            }
                        }
                    }
                }
                _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);
                break;
            case EVENT_WARLOCK_CAST_SPELL:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* target = owner->GetSelectedUnit())
                    {
                        if (me->CanCreatureAttack(target))
                        {
                            me->CastSpell(target, SPELL_VILE_SPIT);
                        }
                    }
                }
                _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 4500);
                break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount();

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }

private:
    EventMap _events;
    bool _initAttack;
    Player* owner;
    int32 shadow;
    bool hasCharged;
};

struct npc_pet_warlock_felguard_grimoire : public ScriptedAI
{
    npc_pet_warlock_felguard_grimoire(Creature* creature) : ScriptedAI(creature), _initAttack(true), _didCharge(false) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        _events.Reset();
        me->CastSpell(me, 83031);
        _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);

        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        if (owner->HasAura(T1_WARLOCK_DEMONO_4PC))
            owner->AddAura(T1_WARLOCK_DEMONO_4PC_GRIMOIRE_BUFF, me);

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void AttackTarget(Unit* target)
    {
        if (!_didCharge)
        {
            me->CastSpell(target, 25821);
            _didCharge = true;
        }
        AttackStart(target);
    }

    void Reset() override
    {
        if (me->IsAlive())
        {
            float angle = me->ToTempSummon()->GetAngle();
            float dist = me->ToTempSummon()->GetDist();

            if (Unit* unit = me->GetCharmerOrOwnerPlayerOrPlayerItself()) {
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveFollow(unit, dist, angle, MOTION_SLOT_ACTIVE);
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (Unit* target = owner->GetSelectedUnit())
                {
                    if (me->CanCreatureAttack(target) && owner->IsInCombat())
                    {
                        AttackTarget(target);
                        _initAttack = false;
                    }
                }
            }
        }

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_TRY_ATTACK_NEW_TARGET:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* newTarget = owner->GetSelectedUnit())
                    {
                        if (Unit* victim = me->GetVictim()) {
                            if (victim->GetGUID() != newTarget->GetGUID() && owner->IsInCombatWith(victim))
                            {
                                if (me->CanCreatureAttack(newTarget))
                                    AttackTarget(newTarget);
                            }
                        }
                    }
                }
                _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);
                break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }

private:
    EventMap _events;
    bool _initAttack;
    bool _didCharge;
    Player* owner;
};


struct npc_pet_warlock_demonic_tyrant : public ScriptedAI
{
    npc_pet_warlock_demonic_tyrant(Creature* creature) : ScriptedAI(creature) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        _events.Reset();
        _events.ScheduleEvent(1, 2000);
        owner = me->GetCharmerOrOwnerPlayerOrPlayerItself();

        if (!owner)
            return;
    }

    void Reset() override
    {
        if (me->IsAlive())
        {
            float angle = me->ToTempSummon()->GetAngle();
            float dist = me->ToTempSummon()->GetDist();

            if (Unit* unit = me->GetCharmerOrOwnerPlayerOrPlayerItself()) {
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveFollow(unit, dist, angle, MOTION_SLOT_ACTIVE);
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* newTarget = owner->GetSelectedUnit())
                    {
                        if (Unit* victim = me->GetVictim()) {
                            if (victim->GetGUID() != newTarget->GetGUID() && owner->IsInCombatWith(victim))
                            {
                                if (me->CanCreatureAttack(newTarget))
                                    me->CastSpell(newTarget, SPELL_DEMONFIRE);
                            }
                        }
                    }
                }
                _events.ScheduleEvent(1, 2000);
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    Player* owner;
    bool alreadyFollowing;
    int32 shadow;
};


struct npc_pet_warlock_dreadstalker : public ScriptedAI
{
    npc_pet_warlock_dreadstalker(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);
        _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 2000);

        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount();

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void Reset() override
    {
        if (me->IsAlive())
        {
            float angle = me->ToTempSummon()->GetAngle();
            float dist = me->ToTempSummon()->GetDist();

            if (Unit* unit = me->GetCharmerOrOwnerPlayerOrPlayerItself()) {
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveFollow(unit, dist, angle, MOTION_SLOT_ACTIVE);
            }
        }
    }

    void AttackTarget(Unit* target)
    {
        me->JumpTo(target, 9.375f);
        AttackStart(target);
    }

    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (Unit* target = owner->GetSelectedUnit())
                {
                    if (me->CanCreatureAttack(target) && owner->IsInCombat())
                    {
                        AttackTarget(target);
                        _initAttack = false;
                    }
                }
            }
        }

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_TRY_ATTACK_NEW_TARGET:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* newTarget = owner->GetSelectedUnit())
                    {
                        if (Unit* victim = me->GetVictim()) {
                            if (victim->GetGUID() != newTarget->GetGUID() && owner->IsInCombatWith(victim))
                            {
                                if (me->CanCreatureAttack(newTarget))
                                    AttackTarget(newTarget);
                            }
                        }
                    }
                }
                _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);
                break;
            case EVENT_WARLOCK_CAST_SPELL:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* target = owner->GetSelectedUnit())
                    {
                        if (me->CanCreatureAttack(target))
                        {
                            me->CastSpell(target, SPELL_DREADBITE);
                        }
                    }
                }
                _events.ScheduleEvent(1, 4000);
                break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            if (Aura* aura = owner->GetAura(SPELL_WARLOCK_DEMONIC_CORE_BUFF))
                aura->SetCharges(aura->GetCharges() + 1);
            else
                owner->CastCustomSpell(SPELL_WARLOCK_DEMONIC_CORE_BUFF, SPELLVALUE_AURA_CHARGE, 1, owner, true);

            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount();

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }

private:
    EventMap _events;
    bool _initAttack;
    Player* owner;
    int32 shadow;
};

// 600616 - Doomguard - PET
struct npc_pet_warlock_doomguard : public ScriptedAI
{
    npc_pet_warlock_doomguard(Creature* creature) : ScriptedAI(creature) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void AttackTarget(Unit* target)
    {
        AttackStart(target);
    }

    void Reset() override
    {
        if (me->IsAlive())
        {
            float angle = me->ToTempSummon()->GetAngle();
            float dist = me->ToTempSummon()->GetDist();

            if (Unit* unit = me->GetCharmerOrOwnerPlayerOrPlayerItself()) {
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveFollow(unit, dist, angle, MOTION_SLOT_ACTIVE);
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (Unit* target = owner->GetSelectedUnit())
                {
                    if (me->CanCreatureAttack(target) && owner->IsInCombat())
                    {
                        AttackTarget(target);
                        _initAttack = false;
                    }
                }
            }
        }

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_TRY_ATTACK_NEW_TARGET:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* newTarget = owner->GetSelectedUnit())
                    {
                        if (Unit* victim = me->GetVictim()) {
                            if (victim->GetGUID() != newTarget->GetGUID() && owner->IsInCombatWith(victim))
                            {
                                if (me->CanCreatureAttack(newTarget))
                                    AttackTarget(newTarget);
                            }
                        }
                    }
                }
                _events.ScheduleEvent(EVENT_TRY_ATTACK_NEW_TARGET, 1500);
                break;
            case EVENT_WARLOCK_CAST_SPELL:
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (Unit* target = owner->GetSelectedUnit())
                    {
                        if (me->CanCreatureAttack(target))
                        {
                            me->CastSpell(target, SPELL_DOOM_BOLT);
                        }
                    }
                }
                _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 6000);
                break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }

private:
    EventMap _events;
    bool _initAttack;
    Player* owner;
};

// 89 - Infernal - PET
struct npc_pet_warlock_infernal : public ScriptedAI
{
    npc_pet_warlock_infernal(Creature* creature) : ScriptedAI(creature) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetPrimordialInfernalAura(Unit* caster)
    {
        for (size_t i = 801136; i < 801142; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    Aura* GetInfernalBrandAura(Unit* caster)
    {
        for (size_t i = 801148; i < 801154; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }

        // Add a Primordial Infernal buff
        if (Aura* runeAura = GetPrimordialInfernalAura(owner))
        {
            int32 procSpell = runeAura->GetEffect(EFFECT_2)->GetAmount();
            owner->CastSpell(me, procSpell, TRIGGERED_FULL_MASK);
        }

        // Add a Infernal Brand listener on the infernal
        if (Aura* runeAura = GetInfernalBrandAura(owner))
        {
            int32 procSpell = runeAura->GetEffect(EFFECT_0)->GetAmount();
            owner->CastSpell(me, procSpell, TRIGGERED_FULL_MASK);
        }
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }
};

struct npc_pet_warlock_inquisitors_eye : public ScriptedAI
{
    npc_pet_warlock_inquisitors_eye(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount();

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void EnterCombat(Unit*) override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 0);
    }

    void UpdateAI(uint32 diff) override
    {
        Unit* mainTarget = ObjectAccessor::GetUnit(*me, me->GetTarget());

        if (_initAttack)
        {
            if (!me->IsInCombat())
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                    if (Unit* target = mainTarget)
                        if (me->CanCreatureAttack(target))
                            AttackStart(target);

            _initAttack = false;
        }

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        bool isCasting = false;

        if (_events.ExecuteEvent() == EVENT_WARLOCK_CAST_SPELL)
        {
            if (mainTarget && mainTarget->IsAlive())
            {
                if (isCasting)
                    _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 1000);
                else
                {
                    me->CastSpell(mainTarget, RUNE_WARLOCK_INQUISITORS_GAZE_AURA, TRIGGERED_FULL_MASK);
                    isCasting = true;
                    _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 1000);
                }
            }
            else
            {
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                    if (Unit* target = owner->GetSelectedUnit())
                    {
                        me->CastSpell(target, RUNE_WARLOCK_INQUISITORS_GAZE_AURA, TRIGGERED_FULL_MASK);
                        isCasting = true;
                        mainTarget = target;
                    }

                _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 1000);
            }
        }

        DoMeleeAttackIfReady();
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }

private:
    EventMap _events;
    bool _initAttack;
};

// 800001 - Pit Lord
struct npc_pet_warlock_pit_lord : public ScriptedAI
{
    npc_pet_warlock_pit_lord(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    Aura* GetReignofTyrannyAura(Unit* caster)
    {
        for (size_t i = 800844; i < 800850; i++)
        {
            if (caster->HasAura(i))
                return caster->GetAura(i);
        }

        return nullptr;
    }

    void InitializeAI() override
    {
        Unit* owner = me->GetOwner();

        if (!owner)
            return;

        if (Aura* guldanListener = owner->GetAura(RUNE_WARLOCK_GULDANS_AMBITION_LISTENER))
        {
            int32 stacks = guldanListener->GetStackAmount();
            me->CastCustomSpell(RUNE_WARLOCK_GULDANS_AMBITION_BUFF, SPELLVALUE_AURA_STACK, stacks, me, TRIGGERED_FULL_MASK);
            guldanListener->Remove();
        }


        // Add a Stack of Demonic Servitude from Reign of Tyranny
        if (Aura* runeAura = GetReignofTyrannyAura(owner))
        {
            int32 stackIncrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

            if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                demonicServitude->ModStackAmount(stackIncrease);
            else
                owner->CastCustomSpell(RUNE_WARLOCK_DEMONIC_SERVITUDE, SPELLVALUE_AURA_STACK, stackIncrease, owner, TRIGGERED_FULL_MASK);
        }
    }

    void EnterCombat(Unit*) override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 0);
    }

    void UpdateAI(uint32 diff) override
    {
        Unit* mainTarget = ObjectAccessor::GetUnit(*me, me->GetTarget());

        if (_initAttack)
        {
            if (!me->IsInCombat())
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                    if (Unit* target = mainTarget)
                        if (me->CanCreatureAttack(target))
                            AttackStart(target);

            _initAttack = false;
        }

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (_events.ExecuteEvent() == EVENT_WARLOCK_CAST_SPELL)
        {
            if (mainTarget && mainTarget->IsAlive())
            {
                if (me->HasSpellCooldown(SPELL_FEL_FLAME_BREATH))
                    _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 1000);
                else
                {
                    me->CastSpell(mainTarget, SPELL_FEL_FLAME_BREATH);
                    _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 4000);
                    DoMeleeAttackIfReady();
                }
            }
            else
            {
                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                    if (Unit* target = owner->GetSelectedUnit())
                        mainTarget = target;

                _events.ScheduleEvent(EVENT_WARLOCK_CAST_SPELL, 500);
            }
        }

        DoMeleeAttackIfReady();
    }

    void JustDespawned() override
    {
        if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            // Remove a Stack of Demonic Servitude from Reign of Tyranny
            if (Aura* runeAura = GetReignofTyrannyAura(owner))
            {
                int32 stackDecrease = runeAura->GetEffect(EFFECT_1)->GetAmount() * 3;

                if (Aura* demonicServitude = owner->GetAura(RUNE_WARLOCK_DEMONIC_SERVITUDE))
                    demonicServitude->ModStackAmount(-stackDecrease);
            }
        }
    }

private:
    EventMap _events;
    bool _initAttack;
};



void AddSC_warlock_pet_scripts()
{
    RegisterCreatureAI(npc_pet_warlock_felguard);
    RegisterCreatureAI(npc_pet_warlock_felhunter);
    RegisterCreatureAI(npc_pet_warlock_imp);
    RegisterCreatureAI(npc_pet_warlock_succubus);
    RegisterCreatureAI(npc_pet_warlock_voidwalker);
    RegisterCreatureAI(npc_pet_warlock_wildimp);
    RegisterCreatureAI(npc_pet_warlock_bomber);
    RegisterCreatureAI(npc_pet_warlock_felguard_grimoire);
    RegisterCreatureAI(npc_pet_warlock_darkglare);
    RegisterCreatureAI(npc_pet_warlock_dreadstalker);
    RegisterCreatureAI(npc_pet_warlock_vilefiend);
    RegisterCreatureAI(npc_pet_warlock_demonic_tyrant);
    RegisterCreatureAI(npc_pet_warlock_doomguard);
    RegisterCreatureAI(npc_pet_warlock_infernal);
    RegisterCreatureAI(npc_pet_warlock_inquisitors_eye);
    RegisterCreatureAI(npc_pet_warlock_pit_lord);



    new spell_warl_darkglare_spell();
}
