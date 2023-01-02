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
        uint32 talentId = fields[0].Get<uint32>();
        float ratingPerPointPct = fields[1].Get<float>();
        bool update = fields[2].Get<bool>();
        uint32 auraId = fields[3].Get<uint32>();
        m_spellsMastery.push_back({ talentId, ratingPerPointPct, update, auraId });
    } while (result->NextRow());
}

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
    player->RemoveAura(VERSATILITY_SPELL);
    int32 damageIncreasesPct = round(rating / 40);
    int32 healingAndAbsbordIncreasePct = round(rating / 40);
    int32 damageReductionPct = round((rating / 40) * 0.5);
    player->CastCustomSpell(player->ToUnit(), VERSATILITY_SPELL, &damageIncreasesPct, &healingAndAbsbordIncreasePct, &damageReductionPct, true, nullptr, nullptr, player->GetGUID());
}

