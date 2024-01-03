/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "RunesManager.h"
#include "LuaEngine.h"
#include "Spell.h"
#include "PetDefines.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"
#include "Log.h"
#include "ScriptedGossip.h"
#include "SpellInfo.h"


// Add player scripts
class Runes_PlayerScripts: public PlayerScript
{
public:
    Runes_PlayerScripts() : PlayerScript("Runes_PlayerScripts") { }

    void OnLogin(Player* player) override
    {
        RunesManager::ApplyRunesOnLogin(player);
        RunesManager::UpdateRunicDustCountOnLogin(player);
    }

    void OnCreate(Player* player)
    {
        RunesManager::CreateDefaultCharacter(player);
    }

    void OnDelete(ObjectGuid guid, uint32 accountId)
    {
        RunesManager::RemoveSlotsOnCharacterDel(guid);
    }

    void OnPlayerResurrect(Player* player, float restore_percent, bool applySickness)
    {
        if (!player)
            return;

        RunesManager::ApplyRunesOnLogin(player);
    }


    void OnAchiComplete(Player* player, AchievementEntry const* achievement)
    {
        if (!achievement)
            return;

        if (!player)
            return;

        uint32 achievementId = achievement->ID;

        RunesManager::GiveAchievementReward(player, achievementId);
    }

    void OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid)
    {
        if (item->GetEntry() != 70008)
            return;

        RunesManager::IncreaseRunicDustAmount(player, count);
    }

    void OnQuestRewardItem(Player* player, Item* item, uint32 count)
    {
        if (item->GetEntry() != 70008)
            return;

        RunesManager::IncreaseRunicDustAmount(player, count);
    }


    void OnBeforeBuyItemFromVendor(Player* player, ObjectGuid vendorguid, uint32 vendorslot, uint32& item, uint8 count, uint8 bag, uint8 slot)
    {
        if (item != 70002)
            return;

        RunesManager::DecreaseRunicDustAmount(player, 100 * count);
    };
};

class Runes_MiscScript : public MiscScript
{
public:
    Runes_MiscScript() : MiscScript("Runes_MiscScript") { }

    void AddRunicDustToLoot(uint32 minValue, uint32 maxValue, Loot* loot)
    {
        LootStoreItem storeItem(70008, 0, 100, 0, LOOT_MODE_DEFAULT, 0, minValue, maxValue);
        loot->AddItem(storeItem);
    }

    double calculatePourcentage(uint32 skillLevel, double pourcentageInitial)
    {
        return skillLevel / 10 + pourcentageInitial;
    }

    void AddMetarial(Loot* loot, uint32 skillLevel)
    {
        double lootChanceItem1 = calculatePourcentage(skillLevel, 25.f);
        double lootChanceItem2 = calculatePourcentage(skillLevel, 5.f);
        double lootChanceItem3 = calculatePourcentage(skillLevel, 1.f);

        if (roll_chance_i(lootChanceItem1))
        {
            LootStoreItem storeItem(70010, 0, 100, 0, LOOT_MODE_DEFAULT, 0, 1, 1);
            loot->AddItem(storeItem);
        }

        if (roll_chance_i(lootChanceItem2))
        {
            LootStoreItem storeItem(70011, 0, 100, 0, LOOT_MODE_DEFAULT, 0, 1, 1);
            AddRunicDustToLoot(1, 1, loot);
        }

        if (roll_chance_i(lootChanceItem3))
        {
            LootStoreItem storeItem(70012, 0, 100, 0, LOOT_MODE_DEFAULT, 0, 1, 1);
            loot->AddItem(storeItem);
        }
    }

    void AddBonusRunicDust(Creature* creature, Player* player, Loot* loot) {

        if (creature->getLevel() < player->getLevel())
            return;

        int valueFromEliteRareMin = sWorld->GetValue("CONFIG_DROP_ELITE_RARE_RUNIC_DUST_MIN");
        int valueFromEliteRareMax = sWorld->GetValue("CONFIG_DROP_ELITE_RARE_RUNIC_DUST_MAX");

        int valueFromEliteMin = sWorld->GetValue("CONFIG_DROP_ELITE_RUNIC_DUST_MIN");
        int valueFromEliteMax = sWorld->GetValue("CONFIG_DROP_ELITE_RUNIC_DUST_MAX");

        int dropChanceFromMonster = sWorld->GetValue("CONFIG_DROP_CHANCE_FROM_MONSTER_RUNIC_DUST");

        int valueMinFromDungeonBoss = sWorld->GetValue("CONFIG_DUNGEON_BOSS_RUNIC_DUST_MIN");
        int valueMaxFromDungeonBoss = sWorld->GetValue("CONFIG_DUNGEON_BOSS_RUNIC_DUST_MAX");

        Map* map = creature->GetMap();

        if (creature->isElite() && !map->IsDungeon())
            AddRunicDustToLoot(valueFromEliteMin, valueFromEliteMax, loot);

        if (roll_chance_i(dropChanceFromMonster) && !map->IsHeroic())
            AddRunicDustToLoot(1, 1, loot);

        if (roll_chance_i(dropChanceFromMonster) && map->IsHeroic())
            AddRunicDustToLoot(2, 2, loot);

        if (creature->GetCreatureTemplate()->rank == CREATURE_ELITE_RARE)
            AddRunicDustToLoot(valueFromEliteRareMin, valueFromEliteRareMax, loot);

        if (creature->IsDungeonBoss() && !map->IsHeroic()) 
            AddRunicDustToLoot(valueMinFromDungeonBoss, valueMaxFromDungeonBoss, loot);

        if (creature->IsDungeonBoss() && map->IsHeroic())
            AddRunicDustToLoot(valueMinFromDungeonBoss * 2, valueMaxFromDungeonBoss * 2, loot);
    }

    void OnAfterLootTemplateProcess(Loot* loot, LootTemplate const* tab, LootStore const& store, Player* lootOwner, bool personal, bool noEmptyError, uint16 lootMode, WorldObject* source)
    {
        if (!source)
            return;

        if (GameObject* go = source->ToGameObject())
        {
            uint32 lockId = go->GetGOInfo()->GetLockId();
            LockEntry const* lock = sLockStore.LookupEntry(lockId);

            for (int j = 0; j < MAX_LOCK_CASE; ++j)
            {
                switch (lock->Type[j])
                {
                    case LOCK_KEY_SKILL:
                        uint32 skillId = SkillByLockType(LockType(lock->Index[j]));

                        if (skillId == SKILL_HERBALISM || skillId == SKILL_MINING)
                        {
                            if (uint32 pureSkillValue = lootOwner->GetPureSkillValue(skillId))
                            {
                                AddMetarial(loot, pureSkillValue);
                                int valueFromMinningAndHerborsimMin = sWorld->GetValue("CONFIG_MINNING_AND_HERBORISM_RUNIC_DUST_MIN");
                                int valueFromMinningAndHerborsimMax = sWorld->GetValue("CONFIG_MINNING_AND_HERBORISM_RUNIC_DUST_MAX");

                                AddRunicDustToLoot(valueFromMinningAndHerborsimMin, valueFromMinningAndHerborsimMax, loot);
                            }
                        }
                       break;
                }
            }
        }

        if (Creature* creature = source->ToCreature())
        {
            if (creature->GetCreatureTemplate()->SkinLootId > 0)
            {
                if (uint32 pureSkillValue = lootOwner->GetPureSkillValue(SKILL_SKINNING) && creature->getLevel() >= lootOwner->getLevel())
                {
                    AddMetarial(loot, pureSkillValue);
                    int valueFromSkinningMin = sWorld->GetValue("CONFIG_SKINNING_RUNIC_DUST_MIN");
                    int valueFromSkinningMax = sWorld->GetValue("CONFIG_SKINNING_RUNIC_DUST_MAX");
                    AddRunicDustToLoot(valueFromSkinningMin, valueFromSkinningMax, loot);
                }
            }
            AddBonusRunicDust(creature, lootOwner, loot);
        }
    }
};



using namespace Acore::ChatCommands;

class Runes_CommandsScript : public CommandScript
{
public:
    Runes_CommandsScript() : CommandScript("Runes_CommandsScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "reload runes",  HandleReloadCommand, SEC_MODERATOR,     Console::No },
            { "addRune",  HandleAddRuneCommand, SEC_MODERATOR,     Console::No },
            { "getRunes",  HandleGetRunesCommand, SEC_MODERATOR,     Console::No },

        };
        return commandTable;
    }

    static bool HandleReloadCommand(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        LOG_INFO("Runes", "Reload Runes...");

        RunesManager::SetupConfig();
        RunesManager::LoadAllRunes();
        RunesManager::LoadAccountsRunes();
        RunesManager::LoadAllLoadout();
        RunesManager::LoadAllSlotRune();
        RunesManager::LoadAllProgression();
        Eluna::ReloadEluna();

        return true;
    }

    static bool HandleAddRuneCommand(ChatHandler* handler, Optional<PlayerIdentifier> playerID)
    {
        Player* player = handler->GetPlayer();
        int8 quality = NORMAL_QUALITY;
        Rune rune = RunesManager::GetRandomRune(player, quality);

        if (!rune) {
            LOG_ERROR("No Rune", "No rune found, error");
            return false;
        }
        RunesManager::AddRunePlayer(player, rune);
        return true;
    }

    static bool HandleGetRunesCommand(ChatHandler* handler, Optional<PlayerIdentifier> playerID)
    {
        return true;
    }
};

class spell_activate_rune : public SpellScript
{
    PrepareSpellScript(spell_activate_rune);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        SpellValue const* value = GetSpellValue();
        uint32 runeId = value->EffectBasePoints[EFFECT_0];
        Rune rune = RunesManager::GetRuneBySpellId(runeId);

        if (rune.spellId > 0)
        {
            RunesManager::SpellConversion(rune.spellId, player, true);
            RunesManager::AddRuneToSlot(player, rune);
            sEluna->OnActivateRune(player, "Rune successfully activated!", 0);
        }

    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_activate_rune::HandleProc);
    }
};

class Runes_WorldScript : public WorldScript
{
public:
    Runes_WorldScript() : WorldScript("Runes_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        LOG_INFO("Runes", "Initialize Runes...");
        RunesManager::SetupConfig();
        RunesManager::LoadAllRunes();
        RunesManager::LoadAccountsRunes();
        RunesManager::LoadAllLoadout();
        RunesManager::LoadAllSlotRune();
        RunesManager::LoadAllProgression();
        RunesManager::LoadSpellsConversion();
        RunesManager::LoadCharacterDraws();
        RunesManager::LoadRewardsAchievement();
    }
};


class spell_generate_random_rune : public SpellScript
{
    PrepareSpellScript(spell_generate_random_rune);

    void HandleProc()
    {

        Player* player = GetCaster()->ToPlayer();
        int8 quality ;

        if (m_scriptSpellId == 79500) quality = NORMAL_QUALITY;
        else if (m_scriptSpellId == 79501) quality = UNCOMMON_QUALITY;
        else if (m_scriptSpellId == 79502) quality = RARE_QUALITY;
        else if (m_scriptSpellId == 79503) quality = EPIC_QUALITY;
        else quality = LEGENDARY_QUALITY;

        Rune rune = RunesManager::GetRandomRune(player, quality);

        if (!rune) {
            LOG_ERROR("No Rune", "No rune found, error");
            return;
        }

        RunesManager::AddRunePlayer(player, rune);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_generate_random_rune::HandleProc);
    }
};

class spell_upgrade_rune : public SpellScript
{
    PrepareSpellScript(spell_upgrade_rune);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();
        SpellValue const* value = GetSpellValue();
        uint32 runeId = value->EffectBasePoints[EFFECT_0];

        Rune rune = RunesManager::GetRuneBySpellId(runeId);

        if (!rune)
            return;

        RunesManager::RemoveNecessaryItemsForUpgrade(player, rune);
        RunesManager::AddRunePlayer(player, rune);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_upgrade_rune::HandleProc);
    }
};

class go_rune_upgrader : public GameObjectScript
{
public:
    go_rune_upgrader() : GameObjectScript("go_rune_upgrader") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        sEluna->OpenForgeRune(player);
        return true;
    }
};

class npc_buying_rune : public CreatureScript
{
public:
    npc_buying_rune() : CreatureScript("npc_buying_rune") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);
        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF:
        {
            RunesManager::ApplyBuyingRuneWithGold(player);
            uint32 cost = RunesManager::CalculateGoldCostToBuyRune(player);
            player->ModifyMoney(-cost);
            player->AddItem(70002, 1);
            CloseGossipMenuFor(player);
        }
            break;
        case GOSSIP_ACTION_TRADE:
            player->GetSession()->SendListInventory(creature->GetGUID());
            break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        uint32 cost = RunesManager::CalculateGoldCostToBuyRune(player);
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Buy a Sealed Common Rune with Gold", 50, GOSSIP_ACTION_INFO_DEF, "Are you sure want to buy a Sealed Common Rune?", cost, false);
        AddGossipItemFor(player, GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        SendGossipMenuFor(player, 900900, creature->GetGUID());
        return true;
    }
};

// Add all scripts in one
void AddSC_runesScripts()
{
    new Runes_PlayerScripts();
    new Runes_WorldScript();
    new Runes_CommandsScript();
    new go_rune_upgrader();
    new Runes_MiscScript();
    new npc_buying_rune();
    RegisterSpellScript(spell_activate_rune);
    RegisterSpellScript(spell_generate_random_rune);
    RegisterSpellScript(spell_upgrade_rune);
}
