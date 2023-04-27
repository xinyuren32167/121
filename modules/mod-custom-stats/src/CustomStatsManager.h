#pragma once
#include "Player.h"
#include "DatabaseEnv.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"

struct MasterySpell
{
    uint32 talentId;
    float modifierPerPoint;
    bool update;
    uint32 castSpellId; 
};


class CustomStatsManager
{

private:
    static std::vector<MasterySpell> m_spellsMastery;
    static std::map<Classes, uint32> m_cooldownReductionsSpellClasses;
    static std::map<Classes, uint32> m_dotsReductionsSpellClasses;
public:
    static void LoadSpellsMastery();
    static void UpdateMastery(Player* player, uint32 value);
    static void UpdateVersatility(Player* player, uint32 value);
};

