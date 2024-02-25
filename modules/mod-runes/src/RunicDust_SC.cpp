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

        const bool isNormalDungeonBoss = creature->IsDungeonBoss() && !map->IsHeroic();
        const bool isHeroicDungeonBoss = creature->IsDungeonBoss() && map->IsHeroic();
        const bool isMythicDungeonBoss = creature->IsDungeonBoss() && map->IsMythic();
        const bool isNormalMobs = !creature->isElite() && roll_chance_i(dropChanceFromMonster);
        const bool isCreatureEliteRare = creature->GetCreatureTemplate()->rank == CREATURE_ELITE_RARE;


        if (isNormalDungeonBoss) {
            AddRunicDustToLoot(valueMinFromDungeonBoss, valueMaxFromDungeonBoss, loot);
        }

        if (isHeroicDungeonBoss) {
            AddRunicDustToLoot(valueMinFromDungeonBoss * 2, valueMaxFromDungeonBoss * 2, loot);
        }

        if (isMythicDungeonBoss) {
            AddRunicDustToLoot(valueMinFromDungeonBoss * 3, valueMaxFromDungeonBoss * 3, loot);
        }

        if (isNormalMobs) {
            AddRunicDustToLoot(1, 1, loot);
        }

        if (isCreatureEliteRare) {
            AddRunicDustToLoot(valueFromEliteRareMin, valueFromEliteRareMax, loot);
        }
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
}
