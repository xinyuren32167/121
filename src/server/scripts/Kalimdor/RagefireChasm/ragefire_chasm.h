/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEF_RUINS_OF_AHNQIRAJ_H
#define DEF_RUINS_OF_AHNQIRAJ_H

#include "CreatureAIImpl.h"

#define RageFireChasmScriptName "instance_rage_fire_chasm"

enum DataTypes
{
    DATA_BAZZALAN = 0,
    DATA_JERGOSH = 1,
    DATA_OGGLEFLINT = 2,
    DATA_TARAGAME = 3,
};

enum Creatures
{
  
};

enum GameObjects
{
};

template <class AI, class T>
inline AI* GetRageFireChasmAI(T* obj)
{
    return GetInstanceAI<AI>(obj, RageFireChasmScriptName);
}

#define RegisterRageFireChasmCreatureAI(ai_name) RegisterCreatureAIWithFactory(ai_name, GetRageFireChasmAI)

#endif
