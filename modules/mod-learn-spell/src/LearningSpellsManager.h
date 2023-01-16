
#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "RunesManager.h"
#include "LuaEngine.h"
#include "Spell.h"

struct LearningSpell {
    uint32 spellId;
    uint32 level;
};

class LearningSpellsManager {

private:
    static std::map<uint32 /* classId */, std::vector<LearningSpell>> m_Spells;

public:
    static void PreloadAllSpells();
    static void GiveSpellsForLevelup(Player* player);
};
