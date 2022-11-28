#include "CustomStatsManager.h"
#include <cmath>

std::vector<MasterySpell> CustomStatsManager::m_spellsMastery = {};

void CustomStatsManager::LoadSpellsMastery()
{
    QueryResult result = WorldDatabase.Query("SELECT * FROM spell_mastery");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 spellId = fields[0].Get<uint32>();
        float ratingPerPointPct = fields[1].Get<float>();
        m_spellsMastery.push_back({ spellId, ratingPerPointPct });
    } while (result->NextRow());
}

void CustomStatsManager::UpdateMastery(Player* player, uint32 rating)
{
    for (auto& mastery : m_spellsMastery)
        if (Aura* aura = player->GetAura(mastery.spellId)) {
            const float value = rating / mastery.modifierPerPoint;
            player->SetMastery(value);
        }
}

void CustomStatsManager::UpdateVersatility(Player* player, uint32 rating)
{
    player->RemoveAura(VERSATILITY_SPELL);
    int32 damageIncreasesPct = round(rating / 40);
    int32 healingAndAbsbordIncreasePct = round(rating / 40);
    int32 damageReductionPct = round((rating / 40) * 0.5);
    player->CastCustomSpell(player->ToUnit(), VERSATILITY_SPELL, &damageIncreasesPct, &healingAndAbsbordIncreasePct, &damageReductionPct, true, nullptr, nullptr, player->GetGUID());
}

