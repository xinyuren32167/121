#include "PetDefines.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"


class spell_vampirism : public AuraScript
{
    PrepareAuraScript(spell_vampirism);

    uint32 healPct;
    uint32 spellId;

    bool Load() override
    {
        healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
        spellId = GetSpellInfo()->Effects[EFFECT_0].TriggerSpell;
        return true;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();
        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }
        return GetTarget()->IsAlive();
    }

    void TriggerHeal(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (DamageInfo* damageInfo = eventInfo.GetDamageInfo()) {
            int32 amount = CalculatePct(static_cast<int32>(eventInfo.GetDamageInfo()->GetDamage()), healPct);
            GetCaster()->CastCustomSpell(spellId, SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_vampirism::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_vampirism::TriggerHeal, EFFECT_1, SPELL_AURA_DUMMY);
    }
};

class spell_unstable_power : public AuraScript
{
    PrepareAuraScript(spell_unstable_power)

        uint32 health;

    bool Load() override
    {
        health = GetCaster()->GetHealth();
        return true;
    }

    void RemoveStack() {
        Unit::AuraApplicationMap const& auras = GetCaster()->GetAppliedAuras();
        for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
        {
            if (Aura* aura = itr->second->GetBase())
            {
                SpellInfo const* auraInfo = aura->GetSpellInfo();
                if (auraInfo && (
                    auraInfo->Id == 200049 ||
                    auraInfo->Id == 200051 ||
                    auraInfo->Id == 200053 ||
                    auraInfo->Id == 200055 ||
                    auraInfo->Id == 200057 ||
                    auraInfo->Id == 200059))
                {
                    aura->ModStackAmount(-1, AURA_REMOVE_BY_EXPIRE);
                    break;
                }
            }
        }
    }

    void Update(AuraEffect*  /*effect*/)
    {
        RemoveStack();
    }

    void OnDamageTaken(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        RemoveStack();
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_unstable_power::OnDamageTaken, EFFECT_1, SPELL_AURA_DUMMY);
        OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_unstable_power::Update, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};



void AddSC_generals_perks_scripts()
{

}
