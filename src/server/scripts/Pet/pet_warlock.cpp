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

};


class npc_pet_warlock_wildimp : public CreatureScript
{
public:
    npc_pet_warlock_wildimp() : CreatureScript("npc_pet_warlock_wildimp") { }

    struct npc_pet_warlock_wildimpAI : public CombatAI
    {
        npc_pet_warlock_wildimpAI(Creature* creature) : CombatAI(creature) { }

        uint32 attackTimer;

        void InitializeAI() override
        {
            CombatAI::InitializeAI();

            Unit* owner = me->GetOwner();

            if (!owner)
                return;
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
