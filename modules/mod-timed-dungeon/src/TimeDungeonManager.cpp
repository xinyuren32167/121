#include "TimeDungeonManager.h"

void TimedDungeonManager::HandleChangeDungeonDifficulty(Player* _player, uint8 mode)
{
    _player->SetDungeonDifficulty(Difficulty(mode));
    _player->SendDungeonDifficulty(false, DUNGEON_DIFFICULTY_EPIC);

    Player::ResetInstances(_player->GetGUID(), INSTANCE_RESET_CHANGE_DIFFICULTY, false);
}
