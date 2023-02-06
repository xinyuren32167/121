#pragma once
#include "Player/Player.h"
#include "DatabaseEnv.h"
#include "Group.h"

class TimedDungeonManager {
public:
    static void HandleChangeDungeonDifficulty(Player* _player, uint8 mode);
    static void StartMythicDungeon(Player* player);
    static void EndMythicDungeon(Player* player);
};
