#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "RunesManager.h"
#include "LuaEngine.h"
#include "Spell.h"

struct CombatLogSpell
{
    uint32 spellId;
    uint32 amount = 0;
    std::chrono::system_clock::time_point castedWhen;
};

struct CombatLog
{
    std::chrono::system_clock::time_point startTime;
    uint32 mapId;
    uint32 specId;
    uint64 totalDamage = 0;
    uint64 totalHealing = 0;
    bool stopCombat = false;
    std::vector<CombatLogSpell> spellsCast;
};


class CombatLogManager {


private:

    static uint64 GetMaxIdCombatLog();

public:

    static std::map<uint64, CombatLog> combatInfo;

    static void StartCombat(Player* player);
    static void EndCombat(Player* player);
    static void RecordLog(Unit* origin, Unit* target, uint32& amount, SpellInfo const* spellProto, bool isHeal);
};
