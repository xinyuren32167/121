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

enum Items {
    ITEM_RUNIC_DUST =   70008,
    ITEM_SEALED_RUNE = 70002,
};

 // Add player scripts
class RunicDust_PlayerScripts : public PlayerScript
{
public:
    RunicDust_PlayerScripts() : PlayerScript("Runes_PlayerScripts") { }

    void OnLogin(Player* player) override
    {
        RunesManager::UpdateRunicDustCountOnLogin(player);
    }

    void OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid)
    {
        if (item->GetEntry() != ITEM_RUNIC_DUST)
            return;

        RunesManager::UpdateRunicDustAmount(player, count, true);
    }

    void OnQuestRewardItem(Player* player, Item* item, uint32 count)
    {
        if (item->GetEntry() != ITEM_RUNIC_DUST)
            return;

        RunesManager::UpdateRunicDustAmount(player, count, true);
    }


    void OnBeforeBuyItemFromVendor(Player* player, ObjectGuid vendorguid, uint32 vendorslot, uint32& item, uint8 count, uint8 bag, uint8 slot)
    {
        if (item != ITEM_SEALED_RUNE || count <= 0)
            return;

        RunesManager::UpdateRunicDustAmount(player, 100 * count, false);
    };
};

class RunicDust_MiscScript : public MiscScript
{
public:
    RunicDust_MiscScript() : MiscScript("Runes_MiscScript") { }

    void AddRunicDustToLoot(uint32 minValue, uint32 maxValue, Loot* loot)
    {
        LootStoreItem storeItem(ITEM_RUNIC_DUST, 0, 100, 0, LOOT_MODE_DEFAULT, 0, minValue, maxValue);
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
            loot->AddItem(storeItem);
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


void AddSC_RunicDust_Scripts()
{
    new RunicDust_MiscScript();
    new RunicDust_PlayerScripts();
}
