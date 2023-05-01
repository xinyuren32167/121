#include "CustomStatsManager.h"
#include <cmath>

std::map<Classes, uint32> CustomStatsManager::m_cooldownReductionsSpellClasses = {};
std::map<Classes, uint32> CustomStatsManager::m_dotsReductionsSpellClasses = {};


void CustomStatsManager::UpdateMastery(Player* player, uint32 rating)
{
    for (auto& mastery : m_spellsMastery)
        if (Aura* aura = player->GetAura(mastery.talentId)) {
            const float value = rating / mastery.modifierPerPoint;
            player->SetMastery(value);
            if (mastery.update) {
                player->RemoveAura(mastery.castSpellId);
                player->CastSpell(player, mastery.castSpellId);
            }
        }
}

void CustomStatsManager::UpdateVersatility(Player* player, uint32 rating)
{
}

