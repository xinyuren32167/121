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

enum SetSpells
{
    // Spells
    // Warrior
    SPELL_WARRIOR_ODYNS_FURY = 84507,
    SPELL_WARRIOR_LAST_STAND = 12975,
    SPELL_WARRIOR_LAST_STAND_AURA = 12976,
    SPELL_WARRIOR_SHIELD_VAULT = 84559,
    // Mage
    SPELL_WARRIOR_ICE_LANCE = 42914,
    // Paladin
    SPELL_PALADIN_AVENGERS_SHIELD = 48827,
    SPELL_PALADIN_INSPIRING_VANGUARD = 80105,
    SPELL_PALADIN_INSPIRING_VANGUARD_BUFF = 80104,


    // Sets
    SPELL_SET_GENERAL_VOLCANIC_ARMOR_DAMAGE = 9057,
    SPELL_SET_GENERAL_VESTMENTS_OF_THE_DEVOUT_SHIELD = 27779,

    // Warrior
    SPELL_SET_T1_WARRIOR_FURY_BONUS2_BUFF = 95101,
    SPELL_SET_T1_WARRIOR_FURY_BONUS2_VISUAL = 95102,
    // Mage
    SPELL_SET_T1_MAGE_FROST_BONUS2_DAMAGE = 95701,
    // Paladin
    SPELL_SET_T1_PALADIN_PROT_BONUS2_DOT = 96101,
    SPELL_SET_T1_PALADIN_PROT_BONUS2_HEAL = 96102,

};

// --------------------------------------------------------------------------- General Sets ---------------------------------------------------------------------------

// 41676 - Prisoner's Shackle
class spell_set_prisoners_shackle : public AuraScript
{
    PrepareAuraScript(spell_set_prisoners_shackle);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // Set absorbtion amount to unlimited
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 absorbPct = GetEffect(EFFECT_1)->GetAmount();
        int32 absorb = std::min<int32>(dmgInfo.GetDamage(), CalculatePct(caster->GetStat(STAT_STAMINA), absorbPct));

        absorbAmount = absorb;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_set_prisoners_shackle::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_set_prisoners_shackle::Absorb, EFFECT_0);
    }
};

// 27778 - Vestments of the Devout
class spell_set_vestments_of_the_devout : public AuraScript
{
    PrepareAuraScript(spell_set_vestments_of_the_devout);

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

        int32 ratio = aurEff->GetAmount();
        float amount = CalculatePct(caster->SpellBaseHealingBonusDone(GetSpellInfo()->GetSchoolMask()), ratio);

        // Improved PW: Shield: its weird having a SPELLMOD_ALL_EFFECTS here but its blizzards doing :)
        // Improved PW: Shield is only applied at the spell healing bonus because it was already applied to the base value in CalculateSpellDamage
        amount = caster->ApplyEffectModifiers(GetSpellInfo(), aurEff->GetEffIndex(), amount);
        amount *= caster->CalculateLevelPenalty(GetSpellInfo());

        caster->CastCustomSpell(SPELL_SET_GENERAL_VESTMENTS_OF_THE_DEVOUT_SHIELD, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_set_vestments_of_the_devout::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_set_vestments_of_the_devout::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


// 9233 - Volcanic Armor Damage
class spell_set_volcanic_armor : public AuraScript
{
    PrepareAuraScript(spell_set_volcanic_armor);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetAttacker();

        if (!target || target->isDead())
            return;

        int32 amount = CalculatePct(caster->GetStat(STAT_STAMINA), aurEff->GetAmount());

        caster->CastCustomSpell(SPELL_SET_GENERAL_VOLCANIC_ARMOR_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_set_volcanic_armor::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_set_volcanic_armor::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 79413 - Green Dragon Mail
class spell_set_green_dragon_mail : public AuraScript
{
    PrepareAuraScript(spell_set_green_dragon_mail);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetHealInfo();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_set_green_dragon_mail::CheckProc);
    }
};

// 79415 - Regalia of Undead Cleansing
class spell_set_regalia_of_undead_cleansing : public AuraScript
{
    PrepareAuraScript(spell_set_regalia_of_undead_cleansing);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return false;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return false;

        return target->GetCreatureType() == CREATURE_TYPE_UNDEAD;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_set_regalia_of_undead_cleansing::CheckProc);
    }
};

// 41736 - Staff Intellect
class spell_set_staff_intellect : public AuraScript
{
    PrepareAuraScript(spell_set_staff_intellect);

    void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
            if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                if (!item->IsFitToSpellRequirements(GetSpellInfo()))
                    GetEffect(EFFECT_0)->ChangeAmount(0);
    }

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        isPeriodic = true;
        amplitude = 2 * IN_MILLISECONDS;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 value = GetEffect(EFFECT_1)->GetAmount();
            int32 amount = aurEff->GetAmount();

            if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
            {
                if (item->IsFitToSpellRequirements(GetSpellInfo()))
                    if (amount == value)
                        return;
                    else
                        amount = value;
            }
            else
                if (amount == 0)
                    return;
                else
                    amount = 0;

            GetEffect(EFFECT_0)->ChangeAmount(amount);
        }
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_set_staff_intellect::HandleEffectApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_set_staff_intellect::CalcPeriodic, EFFECT_0, SPELL_AURA_ANY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_set_staff_intellect::HandlePeriodic, EFFECT_0, SPELL_AURA_ANY);
    }
};

// 41736 - Shield Intellect
class spell_set_shield_intellect : public AuraScript
{
    PrepareAuraScript(spell_set_shield_intellect);

    void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
            if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                if (!item->IsFitToSpellRequirements(GetSpellInfo()))
                    GetEffect(EFFECT_0)->ChangeAmount(0);
    }

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        isPeriodic = true;
        amplitude = 2 * IN_MILLISECONDS;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (Player* player = caster->ToPlayer())
        {
            int32 value = GetEffect(EFFECT_1)->GetAmount();
            int32 amount = aurEff->GetAmount();

            if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
            {
                if (item->IsFitToSpellRequirements(GetSpellInfo()))
                    if (amount == value)
                        return;
                    else
                        amount = value;
            }
            else
                if (amount == 0)
                    return;
                else
                    amount = 0;

            GetEffect(EFFECT_0)->ChangeAmount(amount);
        }
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_set_shield_intellect::HandleEffectApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_set_shield_intellect::CalcPeriodic, EFFECT_0, SPELL_AURA_ANY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_set_shield_intellect::HandlePeriodic, EFFECT_0, SPELL_AURA_ANY);
    }
};

// --------------------------------------------------------------------------- Warrior Sets ---------------------------------------------------------------------------

// 95100 - Battlegear of Rage Fury T1 Bonus 2
class spell_set_warrior_fury_T1_B2 : public AuraScript
{
    PrepareAuraScript(spell_set_warrior_fury_T1_B2);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == SPELL_WARRIOR_ODYNS_FURY;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 stacks = aurEff->GetAmount();

        if (Aura* visualAura = caster->AddAura(SPELL_SET_T1_WARRIOR_FURY_BONUS2_VISUAL, caster))
            visualAura->SetStackAmount(stacks);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_set_warrior_fury_T1_B2::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_set_warrior_fury_T1_B2::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 95102 - Battlegear of Rage Fury T1 Bonus 2 Visual
class spell_set_warrior_fury_T1_B2_visual : public AuraScript
{
    PrepareAuraScript(spell_set_warrior_fury_T1_B2_visual);

    void ApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        caster->AddAura(SPELL_SET_T1_WARRIOR_FURY_BONUS2_BUFF, caster);
    }

    void RemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        if (caster->HasAura(SPELL_SET_T1_WARRIOR_FURY_BONUS2_BUFF))
            caster->RemoveAura(SPELL_SET_T1_WARRIOR_FURY_BONUS2_BUFF);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_set_warrior_fury_T1_B2_visual::ApplyEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_set_warrior_fury_T1_B2_visual::RemoveEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 95103 - Battlegear of Rage Fury T1 Bonus 4
class spell_set_warrior_fury_T1_B4 : public AuraScript
{
    PrepareAuraScript(spell_set_warrior_fury_T1_B4);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_WARRIOR_ODYNS_FURY, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_set_warrior_fury_T1_B4::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_set_warrior_fury_T1_B4::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 95200 - Battlegear of Safeguard Prot T1 Bonus 2
class spell_set_warrior_prot_T1_B2 : public AuraScript
{
    PrepareAuraScript(spell_set_warrior_prot_T1_B2);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = aurEff->GetAmount();

        if (caster->HasAura(SPELL_WARRIOR_LAST_STAND_AURA))
            cooldown *= 2;

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_WARRIOR_LAST_STAND, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_set_warrior_prot_T1_B2::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_set_warrior_prot_T1_B2::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 95302 - Battlegear of Tenacity Hoplite T1 Bonus 4
class spell_set_warrior_hoplite_T1_B4 : public AuraScript
{
    PrepareAuraScript(spell_set_warrior_hoplite_T1_B4);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        int32 cooldown = aurEff->GetAmount();

        if (Player* player = caster->ToPlayer())
            player->ModifySpellCooldown(SPELL_WARRIOR_SHIELD_VAULT, -cooldown);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_set_warrior_hoplite_T1_B4::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_set_warrior_hoplite_T1_B4::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// --------------------------------------------------------------------------- Mage Sets ---------------------------------------------------------------------------

// 95700 - Frostweaver Regalia Frost T1 Bonus 2
class spell_set_mage_frost_T1_B2 : public AuraScript
{
    PrepareAuraScript(spell_set_mage_frost_T1_B2);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        if (!target->HasAuraState(AURA_STATE_FROZEN, sSpellMgr->AssertSpellInfo(SPELL_WARRIOR_ICE_LANCE), caster))
            return;

        int32 amount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
        caster->CastCustomSpell(SPELL_SET_T1_MAGE_FROST_BONUS2_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_set_mage_frost_T1_B2::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_set_mage_frost_T1_B2::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_set_mage_frost_T1_B2_target : public SpellScript
{
    PrepareSpellScript(spell_set_mage_frost_T1_B2_target);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetTarget());

        if (!target || target->isDead())
            return;

        targets.remove(target);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_set_mage_frost_T1_B2_target::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// --------------------------------------------------------------------------- Paladin Sets ---------------------------------------------------------------------------

// 96100 - Holy Defender Armor Prot T1 Bonus 2
class spell_set_paladin_prot_T1_B2 : public AuraScript
{
    PrepareAuraScript(spell_set_paladin_prot_T1_B2);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        if (!eventInfo.GetSpellInfo())
            return false;

        if (eventInfo.GetDamageInfo()->GetDamageType() == DOT)
            return eventInfo.GetSpellInfo()->Id == SPELL_SET_T1_PALADIN_PROT_BONUS2_DOT;

        if (eventInfo.GetDamageInfo()->GetDamageType() == SPELL_DIRECT_DAMAGE)
            return eventInfo.GetSpellInfo()->Id == SPELL_PALADIN_AVENGERS_SHIELD;

        return false;
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

        if (damage <= 0)
            return;

        int32 spellID = eventInfo.GetSpellInfo()->Id;

        if (spellID == SPELL_PALADIN_AVENGERS_SHIELD)
        {
            int32 amount = CalculatePct(damage, aurEff->GetAmount());
            amount /= 5;
            caster->CastCustomSpell(SPELL_SET_T1_PALADIN_PROT_BONUS2_DOT, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
        }

        if (spellID == SPELL_SET_T1_PALADIN_PROT_BONUS2_DOT)
        {
            int32 amount = CalculatePct(damage, GetEffect(EFFECT_1)->GetAmount());
            caster->CastCustomSpell(SPELL_SET_T1_PALADIN_PROT_BONUS2_HEAL, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_set_paladin_prot_T1_B2::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_set_paladin_prot_T1_B2::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 96103 - Holy Defender Armor Prot T1 Bonus 4
class spell_set_paladin_prot_T1_B4 : public AuraScript
{
    PrepareAuraScript(spell_set_paladin_prot_T1_B4);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() >= 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster || caster->isDead())
            return;

        Unit* target = eventInfo.GetDamageInfo()->GetVictim();

        if (!target || target->isDead())
            return;

        int32 procChance = GetEffect(EFFECT_1)->GetAmount();

        if (roll_chance_i(procChance))
            if (Player* player = caster->ToPlayer())
            {
                player->RemoveSpellCooldown(SPELL_PALADIN_AVENGERS_SHIELD, true);

                if (player->HasAura(SPELL_PALADIN_INSPIRING_VANGUARD))
                    player->CastSpell(player, SPELL_PALADIN_INSPIRING_VANGUARD_BUFF, TRIGGERED_FULL_MASK);
            }
   
        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        int32 amount = CalculatePct(damage, aurEff->GetAmount());
        amount /= 5;

        caster->CastCustomSpell(SPELL_SET_T1_PALADIN_PROT_BONUS2_DOT, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_set_paladin_prot_T1_B4::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_set_paladin_prot_T1_B4::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};



void AddSC_item_set_bonus_scripts()
{
    RegisterSpellScript(spell_set_prisoners_shackle);
    RegisterSpellScript(spell_set_vestments_of_the_devout);
    RegisterSpellScript(spell_set_volcanic_armor);
    RegisterSpellScript(spell_set_green_dragon_mail);
    RegisterSpellScript(spell_set_regalia_of_undead_cleansing);
    RegisterSpellScript(spell_set_staff_intellect);
    RegisterSpellScript(spell_set_shield_intellect);

    RegisterSpellScript(spell_set_warrior_fury_T1_B2);
    RegisterSpellScript(spell_set_warrior_fury_T1_B2_visual);
    RegisterSpellScript(spell_set_warrior_fury_T1_B4);
    RegisterSpellScript(spell_set_warrior_prot_T1_B2);
    RegisterSpellScript(spell_set_warrior_hoplite_T1_B4);

    RegisterSpellScript(spell_set_mage_frost_T1_B2);
    RegisterSpellScript(spell_set_mage_frost_T1_B2_target);

    RegisterSpellScript(spell_set_paladin_prot_T1_B2);
    RegisterSpellScript(spell_set_paladin_prot_T1_B4);


    

}
