#pragma once

#include <set>

#include "Globals.h"

struct TileInfo
{
	unsigned int tileID;
	Tile::ETileType tileType;
};

bool const operator==(TileInfo& tile1, const TileInfo& tile2);
