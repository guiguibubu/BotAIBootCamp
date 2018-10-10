#include "TileInfo.h"

bool const operator==(TileInfo& tile1, const TileInfo& tile2) {
   return tile1.tileID == tile2.tileID;
}