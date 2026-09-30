//
// Created by dusan on 9/15/26.
//

#include "../include/antworld.h"
#include <cstdlib>   // for rand()


/** @brief this is where you as the applicant will make use of the above functions to develop your solution.
 * here are some existing examples of how calling these functions works to help get you started!
 */
void AntWorld::forage() {
//SOLUTION 1

const int dx[] = {0, 3, 3,  3,  0, -3, -3, -3};
const int dy[] = {3, 3, 0, -3, -3, -3,  0,  3};
const int CHECKx[] = {0, 4, 4, 4, 0, -4, -4, -4};
const int CHECKy[] = {4, 4, 0, -4, -4, -4, 0, 4};


    for (int i = 1; i < ants.size() + 1; i++){    
        Ant &ant = this->ants[i - 1];
                
        Coord foodLoc = ant.closestFood(this->foodMap, this->terrainMap);
        Coord pheromoneLoc = ant.closestPheromone(this->pheromoneMap, this->terrainMap);

        //calculates chebyshev distance (distance like a chessboard) rathar than Manhattan distance
        //used later in code so that exploring ants ignore food near home and leave it to non-exploring ants
        int rowDiff = abs(foodLoc.first - ant.homeCoord.first);
        int colDiff = abs(foodLoc.second - ant.homeCoord.second);
        int chebyshevDist = std::max(rowDiff, colDiff); 
        
        //is used to check if home is near an edge, and therfore if an ant should try exploring in that direction
        Coord homeCheck = {ant.homeCoord.first + CHECKx[ant.id], ant.homeCoord.second + CHECKy[ant.id]};


        //ants that don't initially explore:
        //if home is near an edge (ant is a non-exploring ant)
        if (homeCheck.first >= terrainMap.size() || homeCheck.second >= terrainMap[0].size()){
            //if there is no food in sight
            if (foodLoc == Coord(-1, -1)) {
                ant.exploreOrFollowPheromone(pheromoneLoc, this->terrainMap, this->pheromoneMap, this->foodMap);
            }
            //if there is food in sight
            else{
                //get food
                ant.move(this->terrainMap, foodLoc, this->foodMap); 
                ant.returnHome(this->terrainMap, this->foodMap);
            }
            
        }

        //ants that go exploring:
        //if there is no food in sight
        else if (foodLoc == Coord(-1, -1)) {
            Coord destination = {ant.position.first + dx[ant.id], ant.position.second + dy[ant.id]};
            //if you reach the edge of the map
            if (destination.first >= terrainMap.size() || destination.second >= terrainMap[0].size()){
                ant.exploreOrFollowPheromone(pheromoneLoc, this->terrainMap, this->pheromoneMap, this->foodMap);
            }
            //if there is no food in sight but you have not reached the edge of the map
            else {
                //move in designated direction
                ant.move(this->terrainMap, destination, this->foodMap);
            }
        }
        //if there is food in sight and it is outside viewing range of home
        else if(chebyshevDist > ant.foodRadius) {
            //get food
            ant.move(this->terrainMap, foodLoc, this->foodMap);
            ant.returnHome(this->terrainMap, this->foodMap);
        }
        //there is food in sight but it is within viewing range of home (leave it for the non-exploring ants)
        else {
            Coord destination = {ant.position.first + dx[ant.id], ant.position.second + dy[ant.id]};
            //if you reach the edge of the map
            if (destination.first >= terrainMap.size() || destination.second >= terrainMap[0].size()){
                ant.exploreOrFollowPheromone(pheromoneLoc, this->terrainMap, this->pheromoneMap, this->foodMap);
            }
            //if there is no food in sight but you have not reached the edge of the map
            else {
                //move in designated direction
                ant.move(this->terrainMap, destination, this->foodMap);
            }
        }

        //all ants:
        //if at least 4 food is in view, drop pheromones to aleart other ants to come help
        auto foodLocations = ant.foodScan(this->foodMap); 
        if (foodLocations.size() >= 4) {
            ant.dropPheromone(this->pheromoneMap);
        }
    }
}






/** You may insert any custom functions below **/

//runs foodScan and returns the closest food location to the ant's current position
Coord Ant::closestFood(MapTemplate &foodMap, MapTemplate &terrainMap) {
    std::vector<Coord> foodLocations = this->foodScan(foodMap);
    
    if (foodLocations.empty()) {
        return {-1, -1};   // no food visible
    }

    Coord best = foodLocations[0];
    std::vector<Coord> pathFood = shortestPath(terrainMap, this->position, foodLocations[0]); 
    std::vector<Coord> pathHome = shortestPath(terrainMap, foodLocations[0], this->homeCoord); 
    int bestEnergy = calculatePathCost(terrainMap, pathFood) + calculatePathCost(terrainMap, pathHome); //calculates energy cost to go to the food and back home

    for (int i = 1; i < foodLocations.size(); ++i) {
        pathFood = shortestPath(terrainMap, this->position, foodLocations[i]); 
        pathHome = shortestPath(terrainMap, foodLocations[i], this->homeCoord);
        int pathEnergy = calculatePathCost(terrainMap, pathFood) + calculatePathCost(terrainMap, pathHome); //calculates energy cost to go to the food and back home
        if (pathEnergy < bestEnergy) {
            best = foodLocations[i];
            bestEnergy = pathEnergy;
        }
    }
    return best;
}


//runs pheromoneScan and returns the closest pheromone location to the ant's current position
Coord Ant::closestPheromone(MapTemplate &pheromoneMap, MapTemplate &terrainMap) {
    std::vector<Coord> pheromoneLocations = this->pheromoneScan(pheromoneMap);
    
    if (pheromoneLocations.empty()) {
        return {-1, -1};   // no pheromones visible
    }

    Coord best = pheromoneLocations[0];
    std::vector<Coord> path = shortestPath(terrainMap, this->position, pheromoneLocations[0]); 
    int bestEnergy = calculatePathCost(terrainMap, path);

    for (int i = 1; i < pheromoneLocations.size(); ++i) {
        std::vector<Coord> path = shortestPath(terrainMap, this->position, pheromoneLocations[i]); 
        int pathEnergy = calculatePathCost(terrainMap, path);
        if (pathEnergy < bestEnergy) {
            best = pheromoneLocations[i];
            bestEnergy = pathEnergy;
        }
    }
    return best;
}

// Random destination, or go to a pheromone if one's visible
void Ant::exploreOrFollowPheromone(Coord pheromoneLoc, MapTemplate &terrainMap, MapTemplate &pheromoneMap, MapTemplate foodMap) {
    //if there are no pheromones in sight
    if (pheromoneLoc == Coord(-1, -1)) { 
        //go somewhere random
        Coord destination = {rand() % (int)terrainMap.size(), rand() % (int)terrainMap[0].size()}; 
        this->move(terrainMap, destination, foodMap);
    }
    // if there is no food in sight, you reach the edge of the map, but you see a pheromone
    else {
        //goes to the pheromone
        Coord destination = pheromoneLoc;
        this->move(terrainMap, destination, foodMap);
        //gets rid of pheromone at this location
        pheromoneMap[destination.first][destination.second] = 0; 
    }
}
