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
    SPELL_CHARGE_FELBOAR    = 83005,
    SPELL_DEMONBOLT         = 83008
};

enum PET_WARLOCKS {

    PET_DARKHOUND = 600600,
    PET_WILDIMP = 600601,
    PET_FELBOAR = 600602,
    PET_DEMONIC_TYRAN = 600603,
};


static Unit* GetOwnerTarget(Unit* owner) {
    if (Unit* targetOwner = ObjectAccessor::GetUnit(*owner, owner->GetTarget()))
        if (owner->IsInCombat())
            return targetOwner;

    return nullptr;
}

static bool hasSummonedDemonicTyrant(Unit* owner)
{
    bool isSummoned = false;

    for (Unit::ControlSet::const_iterator itr = owner->m_Controlled.begin(); itr != owner->m_Controlled.end(); ++itr)
        if ((*itr)->IsAlive() && (*itr)->GetEntry() == PET_DEMONIC_TYRAN)
        {
            isSummoned = true;
            break;
        }
    return isSummoned;
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
                int32 damage = owner->CalculateSpellDamageWithRatio(SPELL_SCHOOL_MASK_FIRE, scalingFirebolt);
                if (Unit* target = GetOwnerTarget(owner)) {
                    me->CastCustomSpell(83003, SPELLVALUE_BASE_POINT0, damage, target);
                }
                else {
                    if (!me->IsInCombat()) {
                        me->GetMotionMaster()->Clear(false);
                        me->GetMotionMaster()->MoveFollow(owner, urand(1, 2), urand(1, 2));
                    }
                }
                attackTimer = 0;
            }
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell)
        {
            if (hasSummonedDemonicTyrant(owner))
                return;

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


struct npc_pet_warlock_felboar : public ScriptedAI
{
    npc_pet_warlock_felboar(Creature* creature) : ScriptedAI(creature), _initAttack(true) { }

    void InitializeAI() override
    {
        owner = me->GetCharmerOrOwnerPlayerOrPlayerItself();
    }


    void UpdateAI(uint32 diff) override
    {
        if (_initAttack)
        {
            if (owner->IsInCombat()) {
                if (Unit* target = owner->GetSelectedUnit())
                    if (me->CanCreatureAttack(target)) {
                        int32 damage = owner->CalculateSpellDamageWithRatio(SPELL_SCHOOL_MASK_SHADOW, 1.53);
                        me->CastCustomSpell(SPELL_CHARGE_FELBOAR, SPELLVALUE_BASE_POINT0, damage, target);
                        AttackStart(target);
                        _initAttack = false;
                    }
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    bool _initAttack;
    Player* owner;
    int32 shadow;
};


struct npc_pet_warlock_demonic_tyrant : public ScriptedAI
{
    npc_pet_warlock_demonic_tyrant(Creature* creature) : ScriptedAI(creature) { }

    void InitializeAI() override
    {
        _events.Reset();
        _events.ScheduleEvent(1, 2000);
        owner = me->GetCharmerOrOwnerPlayerOrPlayerItself();
    }

    void UpdateAI(uint32 diff) override
    {
        if (!owner->IsInCombat())
            return;

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                if (Unit* target = owner->GetSelectedUnit()) {
                    int32 spellDamage = owner->CalculateSpellDamageWithRatio(SPELL_SCHOOL_MASK_SHADOW, 0.53);
                    me->CastCustomSpell(SPELL_DEMONBOLT, SPELLVALUE_BASE_POINT0, spellDamage, target);
                    _events.ScheduleEvent(1, 2000);
                }
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    Player* owner;
    int32 shadow;
};


void AddSC_warlock_pet_scripts()
{
    new npc_pet_warlock_wildimp();
    RegisterCreatureAI(npc_pet_warlock_felboar);
    RegisterCreatureAI(npc_pet_warlock_demonic_tyrant);
}
