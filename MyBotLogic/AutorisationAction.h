#pragma once
#include "NPCInfo.h"
#include "LevelInfo.h"

// indique si le mouvement vers la case voisine est possible (mur, case autorise)
bool mouvementVoisinAutorise(LevelInfo& level, NPCInfo& npc, Tile::ETilePosition mouvement);
// indique s'il y a un mur bloquant ou une porte fermee bloquante si on arrive sur la tile avec ce mouvement
bool murBloquant(LevelInfo& level, TileInfo& tile, Tile::ETilePosition mouvement);
bool porteFermeBloquant(LevelInfo& level, TileInfo& tile, Tile::ETilePosition mouvement);