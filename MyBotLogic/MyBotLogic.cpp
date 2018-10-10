#include "MyBotLogic.h"

#include "TurnInfo.h"
#include "NPCInfo.h"

#include <ctime>
#include <chrono>
#include <iomanip> // put_time
#include <sstream>

#include "Windows.h"

#include <algorithm>
#include "myTools.h"
#include "PathFinder.h"

MyBotLogic::MyBotLogic()
{
   //Write Code Here
}

/*virtual*/ MyBotLogic::~MyBotLogic()
{
   //Write Code Here
}

/*virtual*/ void MyBotLogic::Configure(int argc, char *argv[], const std::string& _logpath)
{
#ifdef BOT_LOGIC_DEBUG
   mLogger.Init(_logpath, "MyBotLogic.log");
   PathFinder::configLog(_logpath);
#endif

   BOT_LOGIC_LOG(mLogger, "Configure", true);


   //Write Code Here
}

/*virtual*/ void MyBotLogic::Start()
{
   //Write Code Here

   BOT_LOGIC_LOG(mLogger, "Bot Started", true);
}

/*virtual*/ void MyBotLogic::Init(LevelInfo& _levelInfo)
{
   //Write Code Here
   BOT_LOGIC_LOG(mLogger, "Bot Initiated", true);

   // on recence les tiles a atteindre et interdites
   listeTileGoals.clear();
   listeTileInterdite.clear();
   for (auto tile : _levelInfo.tiles) {
      // on memorise toutes les positions visibles dans le niveau
      listePosition.push_back(Position::Position(tile.second.tileID, getPositionX(_levelInfo, tile.second), getPositionY(_levelInfo, tile.second)));
      if (tile.second.tileType == Tile::ETileType::TileAttribute_Goal) {
         listeTileGoals.push_back(tile.second);
      }
      else if (tile.second.tileType == Tile::ETileType::TileAttribute_Forbidden) {
         listeTileInterdite.push_back(tile.second);
      }
   }

   std::vector<TileInfo> listeTileGoalNonAlloue = listeTileGoals;
   // on associe chaque npc a une tile goal
   for (auto npc : _levelInfo.npcs) {
      TileInfo tileGoal;
      int minDistance = _levelInfo.colCount*_levelInfo.rowCount;
      for (TileInfo tile : listeTileGoalNonAlloue) {
         int minDistanceOld = minDistance;
         minDistance = (std::min)(minDistance, distanceNbTiles(_levelInfo, npc.second.tileID, tile.tileID));
         if (minDistance != minDistanceOld && !tileGoalDejaAssigne(listeCibles, tile.tileID)) {
            tileGoal = tile;
         }
      }

      std::vector<TileInfo>::iterator it = std::find(listeTileGoalNonAlloue.begin(), listeTileGoalNonAlloue.end(), tileGoal);
      listeTileGoalNonAlloue.erase(it, it + 1);
      listeCibles.push_back(Cible::Cible(tileGoal, npc.second));

      //initialise le dico des actions
      dicoAction[npc.second.npcID] = Action::EActionType::ActionType_Move;
   }
   // on liste tous les chemin des differents NPC
   for (Cible cible : listeCibles) {
      PathFinder pathfinder = PathFinder::PathFinder(cible.tileGoal.tileID, cible.npc.tileID, listePosition, convertIntoListID(listeTileInterdite));
      dicoChemin[cible.npc.npcID] = pathfinder.chemin;
   }

   // liste des npc allant de du npc le plus proche a celui le plus eloigne de sa cible
   listeOrderedNpcByDistance = getOrderedNpcByDistance(_levelInfo.npcs, dicoChemin);
   std::reverse(listeOrderedNpcByDistance.begin(), listeOrderedNpcByDistance.end());

#ifdef BOT_LOGIC_DEBUG
   BOT_LOGIC_LOG(mLogger, "Liste position : ", true);
   for (Position position : listePosition) {
      BOT_LOGIC_LOG(mLogger, "Tile " + std::to_string(position.tileID) + " (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")", true);
   }
   BOT_LOGIC_LOG(mLogger, "Tile goals : ", true);
   for (TileInfo tile : listeTileGoals) {
      BOT_LOGIC_LOG(mLogger, std::to_string(tile.tileID) + " ", false);
   }
   BOT_LOGIC_LOG(mLogger, "", true);
   BOT_LOGIC_LOG(mLogger, "Tile interdite : ", false);
   for (TileInfo tile : listeTileInterdite) {
      BOT_LOGIC_LOG(mLogger, std::to_string(tile.tileID) + " ", false);
   }
   BOT_LOGIC_LOG(mLogger, "", true);
   for (Cible cible : listeCibles) {
      BOT_LOGIC_LOG(mLogger, std::to_string(cible.npc.npcID) + " doit aller vers " + std::to_string(cible.tileGoal.tileID), true);
   }
   std::stringstream ss;
   ss << "NPC Ordered :";
   for (unsigned int npcID : listeOrderedNpcByDistance) {
      ss << npcID;
      ss << " ";
   }
   ss << std::endl;

   BOT_LOGIC_LOG(mLogger, ss.str(), true);
#endif



}

/*virtual*/ void MyBotLogic::OnGameStarted()
{
   //Write Code Here
   BOT_LOGIC_LOG(mLogger, "Bot on game started", true);


}

/*virtual*/ void MyBotLogic::FillActionList(TurnInfo& _turnInfo, std::vector<Action*>& _actionList)
{
   //Write Code Here.
   dicoTileCibleTour.clear();

#ifdef BOT_LOGIC_DEBUG
   std::stringstream ss;
   ss << currentTimeForLog() << " : " << "Tour num " << _turnInfo.turnNb << std::endl;
   BOT_LOGIC_LOG(mLogger, ss.str(), true);
#endif

   for (unsigned int npcID : listeOrderedNpcByDistance) {
      NPCInfo npc = _turnInfo.npcs.at(npcID);
      Action::EActionType currentAction = dicoAction.at(npcID);

#ifdef BOT_LOGIC_DEBUG
      std::stringstream ss;
      ss << "NPC num : " << npcID << std::endl;
      BOT_LOGIC_LOG(mLogger, ss.str(), true);
#endif

      switch (currentAction) {
      case Action::EActionType::ActionType_None:
         break;
      case Action::EActionType::ActionType_Move:
         moveToNextTile(_actionList, npc, dicoTileCibleTour);
         break;
      case Action::EActionType::ActionType_Interact:
         break;
      default:
         break;
      }

   }
}

/*virtual*/ void MyBotLogic::Exit()
{
   //Write Code Here
}


void MyBotLogic::moveToNextTile(std::vector<Action*>& _actionList, NPCInfo& npc, std::map<unsigned int, unsigned int>& dicoTileCibleTour) {
   unsigned int tileCible;
   //on recupere sa cible
   Cible cibleNPC = getCibleByNPC(listeCibles, npc.npcID);
   // s'il n'est pas deja a son goal
   if (npc.tileID != cibleNPC.tileGoal.tileID) {
      //on recupere la position de la tile a atteindre
      tileCible = (dicoChemin.at(npc.npcID).empty()) ? npc.tileID : dicoChemin.at(npc.npcID).back();
#ifdef BOT_LOGIC_DEBUG
      std::stringstream ss;
      ss << "NPC veut aller vers : " << tileCible << std::endl;

      BOT_LOGIC_LOG(mLogger, ss.str(), true);
#endif
      // on verifie que le mouvement est autorise
      if (npcAllowedToMoveTo(npc.npcID, tileCible, cibleNPC.tileGoal.tileID, dicoChemin, convertIntoListID(dicoTileCibleTour))) {

         dicoTileCibleTour[npc.npcID] = tileCible;
         if (!(dicoChemin.at(npc.npcID).empty())) {
            (&dicoChemin[npc.npcID])->pop_back();
         }

         // on donne l'ordre du mouvement

         //on recupere la position actuelle du NPC
         Position positionNPC = getPositionByTile(listePosition, npc.tileID);

         Position positionTile = getPositionByTile(listePosition, tileCible);
         //on en deduit la direction a prendre
         Tile::ETilePosition direction = getDirection(positionTile.x - positionNPC.x, positionTile.y - positionNPC.y);
         //on indique au npc la direction a prendre
         _actionList.push_back(new Move(npc.npcID, direction));
#ifdef BOT_LOGIC_DEBUG
         std::stringstream ss;
         ss << currentTimeForLog() << " : " << npc.npcID << " move to ";
         ss << tilePositionToString(direction);
         ss << std::endl;

         BOT_LOGIC_LOG(mLogger, ss.str(), true);
#endif
      }
   }
}

bool MyBotLogic::npcAllowedToMoveTo(unsigned int npcID, unsigned int tileID, unsigned int tileGoal, std::map<unsigned int, std::vector<unsigned int>>& dicoChemin, std::vector<unsigned int>& listeTileCibleTour) {
   bool moveInterdis = true;
   bool nextTileLastTile = tileID == tileGoal;
   bool tileSurChemin = tileBloqueCHemin(npcID, tileID, dicoChemin);
   bool tileDejaDemande = tileDejaDemandee(tileID, listeTileCibleTour);
   moveInterdis = tileDejaDemande || tileSurChemin && nextTileLastTile;

#ifdef BOT_LOGIC_DEBUG
   std::stringstream ss;
   ss << "NPC veut aller vers sa goal : " << nextTileLastTile << std::endl;
   ss << "La tile cible est le chemin d'un autre : " << tileSurChemin << std::endl;
   ss << "La tile deja demandee : " << tileDejaDemande << std::endl;
   ss << "mouvement interdis : " << moveInterdis << std::endl;
   BOT_LOGIC_LOG(mLogger, ss.str(), true);
#endif

   return !moveInterdis;
}

bool MyBotLogic::tileBloqueCHemin(unsigned int npcID, unsigned int tileID, std::map<unsigned int, std::vector<unsigned int>>& dicoChemin) {
   bool tileBloquante = false;
#ifdef BOT_LOGIC_DEBUG
   std::stringstream ss;
   ss << "*******************BLOQUE CHEMIN*************************" << std::endl;
   BOT_LOGIC_LOG(mLogger, ss.str(), true);
#endif
   for (auto chemin : dicoChemin) {
#ifdef BOT_LOGIC_DEBUG
      ss.clear();
      ss << "idNpcChemin : " << chemin.first << " idNpcMove : " << npcID << std::endl;
      BOT_LOGIC_LOG(mLogger, ss.str(), true);
#endif
      if (chemin.first != npcID) {
#ifdef BOT_LOGIC_DEBUG
         ss.clear();
         ss << "N'est pas le chemin du npc etudier" << std::endl;
         BOT_LOGIC_LOG(mLogger, ss.str(), true);
#endif
         size_t nbTileRestante = chemin.second.size();
         int i = 0;
         while (!tileBloquante && i < nbTileRestante) {
            tileBloquante = (chemin.second.at(i) == tileID);
            i++;
         }
      }
   }
#ifdef BOT_LOGIC_DEBUG
   ss.clear();
   ss << "********************************************************" << std::endl;
   BOT_LOGIC_LOG(mLogger, ss.str(), true);
#endif
   return tileBloquante;

}
bool MyBotLogic::tileDejaDemandee(unsigned int tileID, std::vector<unsigned int> listeTileCibleTour) {
   return std::find(listeTileCibleTour.begin(), listeTileCibleTour.end(), tileID) != listeTileCibleTour.end();
}
