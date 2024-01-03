#include "LearningSpellsManager.h"

std::map<uint32, std::vector<LearningSpell>> LearningSpellsManager::m_Spells;


enum SpellWarrior {
    SPELL_WARRIOR_VICTORY_RUSH = 34428,
    SPELL_WARRIOR_IMPEDING_VICTORY = 84515,
};


enum SpellHunter {
    SPELL_HUNTER_BEAST_LORE = 1462,
    SPELL_HUNTER_CALL_PET = 883,
    SPELL_HUNTER_CALL_STABLED_PET = 62757,
    SPELL_HUNTER_DISMISS_PET = 2641,
    SPELL_HUNTER_FEED_PET = 6991,
    SPELL_HUNTER_MEND_PET = 48990,
    SPELL_HUNTER_REVIVE_PET = 982,
    SPELL_HUNTER_TAME_PET = 1515,
    SPELL_HUNTER_CAMOUFLAGE = 80163,
    SPELL_HUNTER_ARCANE_SHOT = 49045,
    SPELL_HUNTER_MASTERY_FROM_THE_SHADOW = 499996,
};

enum WarlockSpells
{
    SPELL_WARLOCK_SUMMON_FELGUARD = 30146,
    SPELL_WARLOCK_SUMMON_FELHUNTER = 691,
    SPELL_WARLOCK_SUMMON_IMP = 688,
    SPELL_WARLOCK_SUMMON_SUCCUBUS = 712,
    SPELL_WARLOCK_SUMMON_VOIDWALKER = 697,
    SPELL_WARLOCK_SUMMON_EYE_OF_KILLROG = 126,
    SPELL_WARLOCK_MASTERY_FEL_BLOOD = 1100024,
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

    if ((spellId == SPELL_WARLOCK_SUMMON_FELGUARD
        || spellId == SPELL_HUNTER_CALL_PET
        || spellId == SPELL_HUNTER_CALL_STABLED_PET
        || spellId == SPELL_HUNTER_DISMISS_PET
        || spellId == SPELL_HUNTER_FEED_PET
        || spellId == SPELL_HUNTER_MEND_PET
        || spellId == SPELL_HUNTER_REVIVE_PET
        || spellId == SPELL_HUNTER_TAME_PET
        || spellId == SPELL_HUNTER_CAMOUFLAGE
        || spellId == SPELL_HUNTER_ARCANE_SHOT) && player->HasSpell(SPELL_HUNTER_MASTERY_FROM_THE_SHADOW))
            return false;


    if ((spellId == SPELL_HUNTER_BEAST_LORE
        || spellId == SPELL_WARLOCK_SUMMON_FELHUNTER
        || spellId == SPELL_WARLOCK_SUMMON_IMP
        || spellId == SPELL_WARLOCK_SUMMON_SUCCUBUS
        || spellId == SPELL_WARLOCK_SUMMON_VOIDWALKER
        || spellId == SPELL_WARLOCK_SUMMON_EYE_OF_KILLROG) && player->HasSpell(SPELL_WARLOCK_MASTERY_FEL_BLOOD))
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
