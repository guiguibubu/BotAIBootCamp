#pragma once

#include <string>
#include "Globals.h"
#include "TileInfo.h"
#include "LevelInfo.h"
#include "Cible.h"
#include "Position.h"
#include <vector>

   std::string tilePositionToString(Tile::ETilePosition tilePosition);
   std::string tileTypeToString(Tile::ETileType tileType);
   std::string currentTimeForLog();
   
   //Renvoie le nb de case a parcourir pour atteindre la tile2 en partant de la tile 1
   //renvois -1 s'il est impossible d'atteindre la tile2 a partir de la tile1
   int distanceNbTiles(LevelInfo& level, TileInfo& tile1, TileInfo& tile2);
   int distanceNbTiles(LevelInfo& level, unsigned int tile1ID, unsigned int tile2ID);
   
   //Renvoie le nb de case separant la tile2 et la tile 1 selon l'axe X ou Y (tile2 - tile1)
   int distanceNbTilesX(LevelInfo& level, TileInfo& tile1, TileInfo& tile2);
   int distanceNbTilesY(LevelInfo& level, TileInfo& tile1, TileInfo& tile2);

   //Renvoie la position en X et Y d'une tile dans le niveau
   //renvois -1 si la tile n'existe pas
   int getPositionX(LevelInfo& level, TileInfo& tile);
   int getPositionY(LevelInfo& level, TileInfo& tile);

   //Renvoie l'ID de la tile en (x,y)
   //renvois -1 si la tile n'existe pas
   int getIdTile(LevelInfo& level, unsigned int x, unsigned int y);

   //renvoie le mouvement selon X et Y a partir du mouvement cardinal
   /*
   NE (1 ,-1)
   E  (1 , 0)
   SE (0 , 1)
   SW (-1, 1)
   W  (-1, 0)
   NW (0 ,-1)
   CENTER (0,0)
   */
   int getMouvementX(Tile::ETilePosition mouvement);
   int getMouvementY(Tile::ETilePosition mouvement);

   //renvoie le mouvement cardinal selon le mouvement en X et Y
   /*
   dx >=  1 && dy <= -1 -> NE
   dx >=  1 && dy ==  0 -> E
   dx ==  0 && dy >=  1 -> SE
   dx <= -1 && dy >=  1 -> SW
   dx <= -1 && dy ==  0 -> W
   dx ==  0 && dy <= -1 -> NW
   dx ==  0 && dy ==  0 -> CENTER
   */
   Tile::ETilePosition getDirection(int dx, int dy);

   TileInfo getTileInfoFromID(LevelInfo& level, unsigned int tileID);
   ObjectInfo getObjectInfoFromID(LevelInfo& level, unsigned int objectID);

   std::vector<ObjectInfo> getObjectsOnTile(LevelInfo& level, unsigned int tileID);

   //renvoie le cote oppose a celui en argument
   //renvoie CENTER s'il n'y a pas de cote oppose
   Tile::ETilePosition getCoteOppose(Tile::ETilePosition cote);

   bool estUnePorte(LevelInfo& level, unsigned int objectID);
   bool estUnePorteFermee(LevelInfo& level, unsigned int objectID);
   bool estUneFenetre(LevelInfo& level, unsigned int objectID);
   bool estUnMur(LevelInfo& level, unsigned int objectID);

   // indique si la tile cible est deja assigne a un npc
   bool tileGoalDejaAssigne(std::vector<Cible> listeCibles, unsigned int tileID);

   //renvoie la cible concernant le npc precise
   Cible getCibleByNPC(std::vector<Cible> listeCibles, unsigned int npcID);

   //renvoie la position de la tile en argument
   Position getPositionByTile(std::vector<Position> listePosition, unsigned int tileID);

   // renvoie les ID des tiles voisines
   std::vector<unsigned int> getVoisins(std::vector<Position> listePosition, unsigned int tileID);
   // renvoie l'ID de la tile voisine dans la direction precise
   // renvoie -1 si hors du terrain de jeu
   int getVoisin(std::vector<Position> listePosition, unsigned int tileID, Tile::ETilePosition direction);

   std::vector<unsigned int> convertIntoListID(std::vector<TileInfo> listeTile);
   std::vector<unsigned int> convertIntoListID(std::vector<NPCInfo> listeNpc);
   std::vector<unsigned int> convertIntoListID(std::map<unsigned int, NPCInfo> dicoNpc);
   std::vector<unsigned int> convertIntoListID(std::map<unsigned int, TileInfo> dicoTile);
   std::vector<unsigned int> convertIntoListID(std::map<unsigned int, ObjectInfo> dicoObject);
   std::vector<unsigned int> convertIntoListID(std::map<unsigned int, unsigned int> dicoID);

   std::vector<unsigned int> getOrderedList(std::vector<unsigned int> liste);
   std::vector<unsigned int> getOrderedNpcByDistance(std::vector<unsigned int> listeNPC, std::map<unsigned int, std::vector<unsigned int>> dicoCheminRestant);
   std::vector<unsigned int> getOrderedNpcByDistance(std::map<unsigned int, NPCInfo> dicoNpc, std::map<unsigned int, std::vector<unsigned int>> dicoCheminRestant);