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

enum DruidSpells
{
    SPELL_DRUID_NAME = 000,
    

    RUNE_DRUID_NAME_TYPE = 000,

};

class rune_druid_lycara_fleeting_glimpse : public AuraScript
{
    PrepareAuraScript(rune_druid_lycara_fleeting_glimpse);

    void HandleProc(AuraEffect const* aurEff)
    {
        Player* caster = GetCaster()->ToPlayer();

        if (!caster || caster->isDead())
            return;

        caster->SetPower(POWER_ENERGY, caster->GetPower(POWER_ENERGY) + 1);
    }

    void Register()
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_druid_lycara_fleeting_glimpse::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};



void AddSC_druid_rune_scripts()
{
    RegisterSpellScript(rune_druid_lycara_fleeting_glimpse);
    


}
