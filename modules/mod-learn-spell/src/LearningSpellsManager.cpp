#include "LearningSpellsManager.h"

std::map<uint32, std::vector<LearningSpell>> LearningSpellsManager::m_Spells;


enum SpellWarrior {
    SPELL_WARRIOR_VICTORY_RUSH = 34428,
    SPELL_WARRIOR_IMPEDING_VICTORY = 84515,
};

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

bool LearningSpellsManager::Exception(Player* player, uint32 spellId)
{
    if (spellId == SPELL_WARRIOR_VICTORY_RUSH && player->HasSpell(SPELL_WARRIOR_IMPEDING_VICTORY))
        return false;


    return true;
}

void LearningSpellsManager::GiveSpellsForLevelup(Player* player)
{
    player->UpdateSkillsToMaxSkillsForLevel();

    auto it = m_Spells.find(player->getClass());

    if (it != m_Spells.end()) {
        for (auto const& learn : it->second)
            if (player->getLevel() >= learn.level && !player->HasSpell(learn.spellId) && Exception(player, learn.spellId))
                player->learnSpell(learn.spellId);
    }
}
