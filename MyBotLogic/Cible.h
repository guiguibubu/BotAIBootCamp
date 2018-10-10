#pragma once
#include "TileInfo.h"
#include "NPCInfo.h"

//couple NPC-Tile indiquant quel tile doit atteindre le NPC pour gagner
struct Cible {

   NPCInfo npc;
   TileInfo tileGoal;

   Cible() = default;
   Cible(TileInfo& tile, NPCInfo& npc) : tileGoal{ tile }, npc{ npc } {};
};