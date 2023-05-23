#include "PlayerSpecialization.h"
#include "boost/bind.hpp"
#include "Config.h"
#include "ElunaIncludes.h"
#include "LuaEngine.h"
#include "boost/iterator/counting_iterator.hpp"

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
    m_Specializations[WARRIOR_ARMS] = { 1, 12294, "Arms", 1, 200000, 31.82,	false,	0 };
    m_Specializations[WARRIOR_FURY] = { 2, 20375, "Fury", 1, 200003, 24.00, false,	0 };
    m_Specializations[WARRIOR_PROTECTION] = { 3, 12809, "Protection", 1, 200005, 70.00,	true,	100750 };

    m_Specializations[MAGE_ARCANE] = { 4, 42995, "Arcane", 8, 300111, 29.16, true, 300113 };
    m_Specializations[MAGE_FIRE] = { 5, 42833, "Fire", 8, 300109,	46.66,	false,	0 };
    m_Specializations[MAGE_FROST] = { 6, 42842, "Frost", 8, 300105,	35.00,	false,	0 };

    m_Specializations[PALADIN_HOLY] = { 17, 48782, "Holy", 2, 400000, 23.33, false,	0 };
    m_Specializations[PALADIN_PROTECTION] = { 18, 48952, "Protection", 2, 400002, 35.00, true, 400004 };
    m_Specializations[PALADIN_RETRIBUTION] = { 19, 35395, "Retribution", 2, 400006,	21.88, true, 400008 };

    m_Specializations[HUNTER_BEAST] = { 14, 80132, "Beast Mastery", 3, 500003, 18.42, true,	500006 };
    m_Specializations[HUNTER_MARSKMANSHIP] = { 15, 80140, "Marksmanship", 3, 500004, 56.00,	true, 500007 };
    m_Specializations[HUNTER_SURVIVAL] = { 16, 19306, "Survival", 3, 500005, 21.12,	true, 500002  };

    m_Specializations[DK_BLOOD] = { 7, 55212, "Blood", 6 };
    m_Specializations[DK_FROST] = { 8, 50384, "Frost", 6 };
    m_Specializations[DK_UNHOLY] = { 9, 49772, "Unholy", 6 };

    m_Specializations[DRUID_BALANCE] = { 10, 80536, "Balance", 11, 700000, 7.00, false, 0 };
    m_Specializations[DRUID_FERAL] = { 11, 80549, "Feral", 11, 700004, 17.50, true, 700006 };
    m_Specializations[DRUID_RESTO] = { 13, 53251, "Restoration", 11 };
    m_Specializations[DRUID_GUARDIAN] = { 12, 80510, "Guardian", 11 };
}
    
void PlayerSpecialization::ActivateSpecialization(Player* player, uint32 newSpecId)
{

    if (player->GetMap()->IsDungeon()) {
        sEluna->OnActivateSpec(player, "You can't do that while in a dungeon!", false, 0);
        return;
    }

    auto match = m_Specializations.find(newSpecId);

    if (match == m_Specializations.end()) {
        sEluna->OnActivateSpec(player, "This specialization doesn't exist!", false, 0);
        return;
    }

    Specialization newSpec = m_Specializations[newSpecId];

    if (newSpec.classInfo != player->getClass()) {
        sEluna->OnActivateSpec(player, "You can't do that", false, 0);
        return;
    }

    player->CastCustomSpell(79852, SPELLVALUE_BASE_POINT0, newSpecId, player, TRIGGERED_NONE);
}


void PlayerSpecialization::UpdateMastery(Player* player, uint32 rating)
{
    uint32 currentSpecId = GetCurrentSpecId(player);

    if (currentSpecId == 0)
        return;

    Specialization spec = m_Specializations[currentSpecId];

    if (Aura* aura = player->GetAura(spec.masteryTalentId)) {
        const float value = rating / spec.modifierPerPoint;
        player->SetMastery(value);
        if (spec.update) {
            player->RemoveAura(spec.castSpellId);
            player->CastSpell(player, spec.castSpellId);
        }
    }
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
