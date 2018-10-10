#pragma once
#include <vector>
#include <map>
#include "Position.h"
#include "Logger.h"
#include <utility>
#include "TileInfo.h"

#ifdef _DEBUG
#define PATHFINDER_DEBUG
#endif

#ifdef PATHFINDER_DEBUG
#define PATHFINDER_LOG(logger, text, autoEndLine) logger.Log(text, autoEndLine)
#else
#define PATHFINDER_LOG(logger, text, autoEndLine) 0
#endif

struct PathFinder {

   static std::string logPath;
   unsigned int tileGoal;
   unsigned int tileStart;
   std::vector<Position> listePosition;
   std::vector<unsigned int> listeTileInterdite;
   // chemin contenant les tuiles a suivre pour rejoindre l'objectif
   // [goal, goal -1 , ....., start+1]
   std::vector<unsigned int> chemin;

private:
   //std::vector<unsigned int> tileVisited;
   std::map<unsigned int, unsigned int> listeCameFrom;
   void rechercheGoal();
   void initChemin();
   
public:
   PathFinder() = default;
   PathFinder(unsigned int tileGoal, unsigned int tileStart, std::vector<Position> listePosition, std::vector<unsigned int> listeTileInterdite) : tileGoal { tileGoal }, tileStart{ tileStart }, listePosition{ listePosition }, listeTileInterdite{ listeTileInterdite }  {
      rechercheGoal();
      initChemin();
   };

   static void configLog(std::string _logPath);

private:
   static Logger mLogger;
};

