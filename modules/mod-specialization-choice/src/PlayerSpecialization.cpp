#include "PlayerSpecialization.h"
#include "boost/bind.hpp"
#include "Config.h"
#include "ElunaIncludes.h"
#include "LuaEngine.h"
#include "boost/iterator/counting_iterator.hpp"
#include "CustomStatsManager.h"

std::map<uint64 /* guid */, uint32 /* specId */> PlayerSpecialization::m_PlayersSpecialization = {};
std::map<uint32 /* specId */, Specialization> PlayerSpecialization::m_Specializations = {};
std::map<uint32 /* specId */, std::vector<uint32>> PlayerSpecialization::m_SpecSpells = {};

void PlayerSpecialization::LoadAllSpecsPlayers()
{

    m_PlayersSpecialization = {};

    QueryResult result = CharacterDatabase.Query("SELECT guid, specId FROM characters");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint64 guid = fields[0].Get<uint64>();
        uint32 specId = fields[1].Get<uint32>();
        m_PlayersSpecialization[guid] = specId;
    } while (result->NextRow());

}

void PlayerSpecialization::LoadAllSpecsSpells()
{
    m_SpecSpells = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM specialization_spells");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 specId = fields[0].Get<uint32>();
        uint32 spellId = fields[1].Get<uint32>();
        m_SpecSpells[specId].push_back(spellId);
    } while (result->NextRow());
}

void PlayerSpecialization::InitializeSpecializations()
{
    m_Specializations[WARRIOR_ARMS] = { 1, 12294, "Arms", 1 };
    m_Specializations[WARRIOR_FURY] = { 2, 20375, "Fury", 1 };
    m_Specializations[WARRIOR_PROTECTION] = { 3, 12809, "Protection", 1 };

    m_Specializations[MAGE_ARCANE] = { 4, 42995, "Arcane", 8 };
    m_Specializations[MAGE_FIRE] = { 5, 42833, "Fire", 8 };
    m_Specializations[MAGE_FROST] = { 6, 42842, "Frost", 8 };
}

void PlayerSpecialization::ActivateSpecialization(Player* player, uint32 newSpecId)
{
    auto match = m_Specializations.find(newSpecId);

    if (match == m_Specializations.end()) {
        sEluna->OnActivateSpec(player, "This specialization doesn't exist!", false);
        return;
    }

    Specialization newSpec = m_Specializations[newSpecId];

    if (newSpec.classInfo != player->getClass()) {
        sEluna->OnActivateSpec(player, "You can't do that", false);
        return;
    }

    uint32 currentSpecId = GetCurrentSpecId(player);

    if (currentSpecId > 0)
        for (auto const& spellId : m_SpecSpells[currentSpecId])
            player->removeSpell(spellId, SPEC_MASK_ALL, false, false);


    for (auto const& spellId : m_SpecSpells[newSpecId])
        player->learnSpell(spellId, false, false, false);

    m_PlayersSpecialization[player->GetGUID().GetCounter()] = newSpecId;
    CharacterDatabase.Execute("UPDATE characters SET specId = {} WHERE guid = {}", newSpecId, player->GetGUID().GetCounter());
    sEluna->OnActivateSpec(player, "Specialization " + newSpec.name + " successfully activated!", true);
    player->UpdateMastery();
    player->SaveToDB(false, false);
}

std::vector<std::string> PlayerSpecialization::GetSpecializations(Player* player)
{
    std::vector<std::string> elements = {};
    uint32 currentSpecId = GetCurrentSpecId(player);

    std::stringstream fmt;
    fmt << currentSpecId;

    elements.push_back(fmt.str());

    for (auto const& spec : m_Specializations) {
        Specialization specialization = spec.second;
        if (spec.second.classInfo == player->getClass()) {
            std::string spellsIds = "#";
            for (auto const& spellId : m_SpecSpells[spec.second.id])
                spellsIds += std::to_string(spellId) + "#";

            std::stringstream fmt2;
            fmt2 << specialization.id << ";" << specialization.name << ";" << specialization.spellIcon << ";" << spellsIds;
            elements.push_back(fmt2.str());
        }
    }
    return elements;
}

uint32 PlayerSpecialization::GetCurrentSpecId(Player* player)
{
    uint32 specId = 0;
    auto match = m_PlayersSpecialization.find(player->GetGUID().GetCounter());

    if (match != m_PlayersSpecialization.end())
        specId = match->second;

    return specId;
}
