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

#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "ragefire_chasm.h"
#include "TaskScheduler.h"

enum Spells
{

};

struct boss_taragaman : public BossAI
{
    boss_taragaman(Creature* creature) : BossAI(creature, DATA_TARAGAME) {  }

    void Reset() override
    {
        BossAI::Reset();
        SetCombatMovement(false);
        me->SetReactState(REACT_AGGRESSIVE);
    }

    void JustSummoned(Creature* who) override
    {

    }

    void MovementInform(uint32 type, uint32 id) override
    {

    }

    void ScheduleTasks()
    {

    }

    void DoAction(int32 action) override
    {

    }

    void JustDied(Unit* killer) override
    {
        me->GetMotionMaster()->MoveFall();
        BossAI::JustDied(killer);
    }

    void EnterEvadeMode(EvadeReason why) override
    {
        BossAI::EnterEvadeMode(why);
    }

    void EnterCombat(Unit* attacker) override
    {
        ScheduleTasks();
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType, SpellSchoolMask) override
    {

    }

    void UpdateAI(uint32 diff) override
    {

    }
};



void AddSC_boss_taragaman()
{
    RegisterRageFireChasmCreatureAI(boss_taragaman);
}
