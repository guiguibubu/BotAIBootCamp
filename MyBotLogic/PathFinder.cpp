#pragma once
#include "PathFinder.h"
#include <queue>
#include <string>
#include <map>
#include <algorithm>
#include "myTools.h"

Logger PathFinder::mLogger;
std::string PathFinder::logPath;

void PathFinder::configLog(std::string _logPath) {
   PathFinder::logPath = _logPath;
}

void PathFinder::rechercheGoal() {
   //initialisation du logger
#ifdef PATHFINDER_DEBUG
   PathFinder::mLogger.Init(PathFinder::logPath, "PathFinder.log");
#endif

   std::queue<unsigned int> frontier;
   frontier.push(tileStart);
   
   //tileVisited.clear();
   listeCameFrom.clear();

   //tileVisited.push_back(tileStart);
   listeCameFrom[tileStart] = tileStart;

   bool goalTrouve = false;

   while (!frontier.empty() && !goalTrouve) {
      unsigned int currentTile = frontier.front();
      frontier.pop();
      // on verifie si on a atteint notre but
      goalTrouve = currentTile == tileGoal;

      PATHFINDER_LOG(mLogger, "Visiting  " + std::to_string(currentTile),true);
      std::vector<unsigned int> voisins = getVoisins(listePosition, currentTile);
      for (unsigned int nextTile : voisins) {
         //bool tileNonVisite = find(tileVisited.begin(), tileVisited.end(), nextTile) == tileVisited.end();
         bool tileNonVisite = listeCameFrom.find(nextTile) == listeCameFrom.end();
         bool tileAutorise = std::find(listeTileInterdite.begin(), listeTileInterdite.end(), nextTile) == listeTileInterdite.end();
         if (tileNonVisite && tileAutorise) {
            frontier.push(nextTile);
            //tileVisited.push_back(nextTile);
            listeCameFrom[nextTile] = currentTile;
         }
      }
   }
}

void PathFinder::initChemin() {
   chemin.clear();
   PATHFINDER_LOG(mLogger, "Chemin retour :", true);
   unsigned int currentTile = tileGoal;
   while (currentTile != tileStart) {
      chemin.push_back(currentTile);
      PATHFINDER_LOG(mLogger, std::to_string(currentTile) + "<-", false);
      currentTile = listeCameFrom[currentTile];
   }
}