#include "LearningSpellsManager.h"

std::map<uint32, std::vector<LearningSpell>> LearningSpellsManager::m_Spells;

void LearningSpellsManager::PreloadAllSpells()
{
    m_Spells = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM learning_spell_leveling");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 classId = fields[0].Get<uint32>();
        uint32 spellId = fields[1].Get<uint32>();
        uint32 level = fields[2].Get<uint32>();
        m_Spells[classId].push_back({ spellId, level });
    } while (result->NextRow());

}

void LearningSpellsManager::GiveSpellsForLevelup(Player* player)
{
    player->UpdateSkillsToMaxSkillsForLevel();

    auto it = m_Spells.find(player->getClass());

    if (it != m_Spells.end()) {
        for (auto const& learn : it->second)
            if (player->getLevel() >= learn.level && !player->HasSpell(learn.spellId))
                player->learnSpell(learn.spellId);
    }
}
