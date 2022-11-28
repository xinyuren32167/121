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
        uint32 ratingPerPointPct = fields[1].Get<uint32>();
        m_spellsMastery.push_back({ spellId, ratingPerPointPct });
    } while (result->NextRow());
}

void CustomStatsManager::UpdateMastery(Player* player, uint32 rating)
{
    for (auto& mastery : m_spellsMastery)
        if (Aura* aura = player->GetAura(mastery.spellId)) {
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i) {
                int32 base = sSpellMgr->AssertSpellInfo(mastery.spellId)->Effects[i].BasePoints - 1;
                if (AuraEffect* aurEff = aura->GetEffect(i))
                {
                    const uint32 pct = round(rating / mastery.modifierPerPoint);
                    aurEff->SetAmount(base + pct);
                }
            }
            break;
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

