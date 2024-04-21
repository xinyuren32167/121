#include "combatLogManager.h"
#include "PlayerSpecialization.h"


std::map<uint64, CombatLog> CombatLogManager::combatInfo = {};

uint64 CombatLogManager::GetMaxIdCombatLog()
{
    uint64 combatLogId = 1;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_COMBATLOG_MAXID);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        Field* fields = result->Fetch();
        combatLogId = fields[0].Get<uint64>();

        if (combatLogId == 0)
            combatLogId = 1;
    }

    return combatLogId;
}

void CombatLogManager::StartCombat(Player* player)
{
    uint64 guid = player->GetGUID().GetCounter();

    CombatLog combatLog = {
        std::chrono::system_clock::now(),
        player->GetMap()->GetEntry()->MapID,
        PlayerSpecialization::GetCurrentSpecId(player),
        0,
        0,
        false,
        {}
    };

    combatInfo.insert(std::make_pair(guid, combatLog));
}

void CombatLogManager::EndCombat(Player* player)
{
    auto it = combatInfo.find(player->GetGUID().GetCounter());

    if (it == combatInfo.end())
        return;

    CombatLog* combatLog = &it->second;

    if (!combatLog)
        return;

    combatLog->stopCombat = true;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_COMBATLOG);
    stmt->SetData(0, player->GetGUID().GetCounter());
    stmt->SetData(1, combatLog->mapId);
    stmt->SetData(2, combatLog->specId);
    stmt->SetData(3, combatLog->totalDamage);
    stmt->SetData(4, combatLog->totalHealing);

    CharacterDatabase.DirectExecute(stmt);
    uint64 maxCombatLogId = GetMaxIdCombatLog();

    if (!combatLog->spellsCast.empty())
    {
        for (auto const& spell : combatLog->spellsCast)
        {
            std::time_t now_time_t = std::chrono::system_clock::to_time_t(spell.castedWhen);
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_COMBATLOGSPELL);
            stmt->SetData(0, maxCombatLogId);
            stmt->SetData(1, spell.spellId);
            stmt->SetData(2, spell.amount);
            stmt->SetData(3, now_time_t);
            CharacterDatabase.Execute(stmt);
        }
    }

    combatInfo.erase(it);
}

void CombatLogManager::RecordLog(Unit* origin, Unit* target, uint32& amount, SpellInfo const* spellProto, bool isHeal)
{
    if (!origin->IsInCombat())
        return;

    auto it = combatInfo.find(origin->GetGUID().GetCounter());

    if (it == combatInfo.end())
        return;

    CombatLog* combatLog = &it->second;

    if (!combatLog)
        return;

    if (combatLog->stopCombat)
        return;

    if (isHeal)
        combatLog->totalHealing += amount;
    else
        combatLog->totalDamage += amount;

    if (spellProto)
    {
        CombatLogSpell log = {
            spellProto->Id,
            amount,
            std::chrono::system_clock::now(),
        };

        combatLog->spellsCast.push_back(log);
    }
}
