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

        Unit* GetOwnerTarget(ObjectGuid target) {
            if (Unit* targetOwner = ObjectAccessor::GetUnit(*owner, target))
                return targetOwner;

            return nullptr;
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
                if (Unit* target = GetOwnerTarget(owner->GetTarget())) {
                    me->CastCustomSpell(83003, SPELLVALUE_BASE_POINT0, spellPower, target);
                }
                else {
                    if (!alreadyFollowing) {
                        me->GetMotionMaster()->Clear(false);
                        me->GetMotionMaster()->MoveFollow(owner, urand(1, 2), urand(1, 2));
                    }
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

void AddSC_warlock_pet_scripts()
{
    new npc_pet_warlock_wildimp();
}
