
#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "RunesManager.h"
#include "LuaEngine.h"
#include "Spell.h"

struct Upgrade {
    uint32 itemLevel;
    uint32 runicEssenceCost;
    uint32 requiredItemId1;
    uint32 requiredItemCount1;
    uint32 requiredItemId2;
    uint32 requiredItemCount2;
};

class ItemUpgradeManager {

public:
    static void LoadCosts();
    static void LoadItemsUpgrade();
    static void UpgradeItem(Player* player, uint32 itemId);
    static void RemoveRequieredItem(Player* player, Upgrade upgrade);
    static bool HasEnoughItemToUpgrade(Player* player, Upgrade upgrade);

private:
    static std::map<uint32 /* item Level */, Upgrade> m_CostUpgrade;
    static std::map<uint32 /* item1 */, uint32 /* item1 */> m_ItemsUpgrade;


};
