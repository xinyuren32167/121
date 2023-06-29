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
    SPELL_WILDIMP_FIREBOLT = 83003,
};


static Unit* GetOwnerTarget(Unit* owner) {
    if (Unit* targetOwner = ObjectAccessor::GetUnit(*owner, owner->GetTarget()))
        if (owner->IsInCombat())
            return targetOwner;

    return nullptr;
}

class npc_pet_warlock_wildimp : public CreatureScript
{
public:
    npc_pet_warlock_wildimp() : CreatureScript("npc_pet_warlock_wildimp") { }

    struct npc_pet_warlock_wildimpAI : ScriptedAI
    {
        npc_pet_warlock_wildimpAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 attackTimer = 0;
        uint32 attackCount = 0;
        float angle;
        float dist; 
        float scalingFirebolt;
        bool alreadyFollowing;
        Unit* owner;

        void Reset() override
        {
            me->SetReactState(REACT_DEFENSIVE);
        }
       

        void AttackStart(Unit* who) override
        {
            ScriptedAI::AttackStart(who);
        }

        void InitializeAI() override
        {
            ScriptedAI::InitializeAI();
            owner = me->GetOwner();
            if (owner) {
                SpellInfo const* spell = sSpellMgr->GetSpellInfo(SPELL_WILDIMP_FIREBOLT);
                if (spell)
                    scalingFirebolt = spell->GetEffect(EFFECT_0).BonusMultiplier;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            attackTimer += diff;

            if (attackTimer >= 2000) {
                int32 fireSpellPower = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
                int32 damage = fireSpellPower * scalingFirebolt;
                int32 spellPower = std::max(1, damage);
                if (Unit* target = GetOwnerTarget(owner)) {
                    me->CastCustomSpell(83003, SPELLVALUE_BASE_POINT0, spellPower, target);
                }
                else {
                    me->GetMotionMaster()->Clear(false);
                    me->GetMotionMaster()->MoveFollow(owner, urand(1, 2), urand(1, 2));
                }
                attackTimer = 0;
            }
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell)
        {
            if (attackCount >= 10)
                me->DespawnOrUnsummon();
            else
                attackCount++;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_pet_warlock_wildimpAI(creature);
    }
};


class npc_pet_warlock_felboar : public CreatureScript
{
public:
    npc_pet_warlock_felboar() : CreatureScript("npc_pet_warlock_felboar") { }

    struct npc_pet_warlock_felboarAI : ScriptedAI
    {
        npc_pet_warlock_felboarAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 checkTarget = 0;
        Unit* owner;
        bool charge;

        void Reset() override
        {
            me->SetReactState(REACT_DEFENSIVE);
        }


        void AttackStart(Unit* who) override
        {
            ScriptedAI::AttackStart(who);
        }

        void InitializeAI() override
        {
            ScriptedAI::InitializeAI();
            owner = me->GetOwner();
        }

        void UpdateAI(uint32 diff) override
        {
            checkTarget += diff;

            if (checkTarget >= 1000) {
                int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
                int32 damage = shadow * 1.53;
                int32 spellPower = std::max(1, damage);
                if (Unit* enemy = GetOwnerTarget(owner)) {
                    if (!charge) {
                        me->CastCustomSpell(83005, SPELLVALUE_BASE_POINT0, shadow, enemy);
                        charge = true;
                    }
                    if (!me->GetTarget()) {
                        me->AI()->AttackStart(enemy);
                    }
                }
                checkTarget = 0;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_pet_warlock_felboarAI(creature);
    }
};

void AddSC_warlock_pet_scripts()
{
    new npc_pet_warlock_wildimp();
    new npc_pet_warlock_felboar();
}
