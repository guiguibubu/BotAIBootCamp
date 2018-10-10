#include "AutorisationAction.h"
#include "myTools.h"

bool mouvementVoisinAutorise(LevelInfo& level, NPCInfo& npc, Tile::ETilePosition mouvement) {
   TileInfo NPCTile = getTileInfoFromID(level, npc.tileID);
   TileInfo tileCible = getTileInfoFromID(level, getIdTile(level, getPositionX(level, NPCTile) + getMouvementX(mouvement), getPositionY(level, NPCTile) + getMouvementY(mouvement)));
   bool estTileInterdite = tileCible.tileType == Tile::ETileType::TileAttribute_Forbidden;
   bool aUnMurBloquant = (estTileInterdite) ? true : murBloquant(level, tileCible, mouvement);
   bool aPorteFermeBloquant = (estTileInterdite) ? true : porteFermeBloquant(level, tileCible, mouvement);
   return !(estTileInterdite || aUnMurBloquant || aPorteFermeBloquant);
}

bool murBloquant(LevelInfo& level, TileInfo& tile, Tile::ETilePosition mouvement) {
   std::vector<ObjectInfo> listeObjectsOnTile = getObjectsOnTile(level, tile.tileID);
   bool murBloquant = false;
   int i = 0;
   size_t nbObjects = listeObjectsOnTile.size();
   while (!murBloquant && i < nbObjects) {
      ObjectInfo objet = listeObjectsOnTile[i];
      murBloquant = (estUnMur(level, objet.objectID)) && (mouvement == getCoteOppose(objet.position));
      i++;
   }
   return murBloquant;
}

bool porteFermeBloquant(LevelInfo& level, TileInfo& tile, Tile::ETilePosition mouvement) {
   std::vector<ObjectInfo> listeObjectsOnTile = getObjectsOnTile(level, tile.tileID);
   bool porteFerme = false;
   int i = 0;
   size_t nbObjects = listeObjectsOnTile.size();
   while (!porteFerme && i < nbObjects) {
      ObjectInfo objet = listeObjectsOnTile[i];
      porteFerme = (estUnePorteFermee(level, objet.objectID)) && (mouvement == getCoteOppose(objet.position));
      i++;
   }
   return porteFerme;
}