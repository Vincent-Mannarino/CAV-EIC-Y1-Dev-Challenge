//
// Created by dusan on 9/15/26.
//

#include "../include/antworld.h"
#include <cstdlib>   // for rand()


/** @brief this is where you as the applicant will make use of the above functions to develop your solution.
 * here are some existing examples of how calling these functions works to help get you started!
 */
void AntWorld::forage() {
    //SOLUTION 2

    const std::vector<Coord> startPos {{4, 4}, {4, 8}, {4, 12}, {8, 4}, {8, 12}, {12, 4}, {12, 8}, {12, 12}}; //only square not covered is the center square (8, 8)

    static int posCheck[] = {0, 0, 0, 0, 0, 0, 0, 0};
            

    for (int i = 1; i < ants.size() + 1; i++){ //for each ant
        Ant &ant = this->ants[i - 1];

        if (posCheck[ant.id] <= 0) { //ants innitially go to their designated exploring zone
            ant.move(this->terrainMap, startPos[ant.id], this->foodMap);
            
            if (std::max(abs(startPos[ant.id].first - ant.homeCoord.first), abs(startPos[ant.id].second - ant.homeCoord.second)) >= 7) { //calculates Chebyshev distance and dropps a pheromone if its designated search zone is far from home so it can get help from other ants if it runs out of evergy before it collects all the food.
                ant.dropPheromone(this->pheromoneMap);
            }

        }
        
        Coord foodLoc = ant.closestFood(this->foodMap);
        Coord pheromoneLoc = ant.closestPheromone(this->pheromoneMap);

        if (foodLoc == Coord(-1, -1)) {//if there is no food in sight
            posCheck[ant.id] = ++posCheck[ant.id];

            if (pheromoneLoc == Coord(-1, -1)) { //no pheromones in sight
                Coord destination = {rand() % (int)terrainMap.size(), rand() % (int)terrainMap[0].size()}; //go somewhere random. later replace this with going towards pheromones
                ant.move(this->terrainMap, destination, this->foodMap);
            }
            else {
                Coord destination = pheromoneLoc;
                ant.move(this->terrainMap, destination, this->foodMap);
                this->pheromoneMap[destination.first][destination.second] = 0; //gets rid of pheromone at this location
            } 
        }
        else { //get food and bring it home
            ant.move(this->terrainMap, foodLoc, this->foodMap); //get food
            ant.returnHome(this->terrainMap, this->foodMap);
        }
    }
}
    
    
    
    






//SOLUTION 1

//     for (int i = 1; i < ants.size() + 1; i++){    
//         Ant &ant = this->ants[i - 1];

//         const int dx[] = {0, 3, 3,  3,  0, -3, -3, -3};
//         const int dy[] = {3, 3, 0, -3, -3, -3,  0,  3};
//         const int CHECKx[] = {0, 4, 4, 4, 0, -4, -4, -4};
//         const int CHECKy[] = {4, 4, 0, -4, -4, -4, 0, 4};

        
//         Coord homeCheck = {ant.homeCoord.first + CHECKx[ant.id], ant.homeCoord.second + CHECKy[ant.id]};
        
//         Coord foodLoc = ant.closestFood(this->foodMap);
//         Coord pheromoneLoc = ant.closestPheromone(this->pheromoneMap);

//         int rowDiff = abs(foodLoc.first - ant.homeCoord.first);
//         int colDiff = abs(foodLoc.second - ant.homeCoord.second);
//         int chebyshevDist = std::max(rowDiff, colDiff); //calculates chebyshev distance (distance like a chessboard) rathar than Manhattan distance
        

//         //ants that don't initially explore
//         if (homeCheck.first >= terrainMap.size() || homeCheck.second >= terrainMap[0].size()){//if home is near an edge
            
//             if (foodLoc == Coord(-1, -1)) {//if there is no food in sight
//                 if (pheromoneLoc == Coord(-1, -1)) { //no pheromones in sight
//                     Coord destination = {rand() % (int)terrainMap.size(), rand() % (int)terrainMap[0].size()}; //go somewhere random. later replace this with going towards pheromones
//                     ant.move(this->terrainMap, destination, this->foodMap);
//                 }
//                 else {
//                     Coord destination = pheromoneLoc;
//                     ant.move(this->terrainMap, destination, this->foodMap);
//                     this->pheromoneMap[destination.first][destination.second] = 0; //gets rid of pheromone at this location
//                 }
//             }
//             else{
//                 ant.move(this->terrainMap, foodLoc, this->foodMap); //get food
//                 ant.returnHome(this->terrainMap, this->foodMap);
//             }
            
//         }


//         //ants that go exploring
//         else if (foodLoc == Coord(-1, -1)) {//if there is no food in sight
                   
//             Coord destination = {ant.position.first + dx[ant.id], ant.position.second + dy[ant.id]}; //Move
            
//             if (destination.first >= terrainMap.size() || destination.second >= terrainMap[0].size()){//if you reach the edge

//                 if (pheromoneLoc == Coord(-1, -1)) { //no pheromones in sight
//                     Coord destination = {rand() % (int)terrainMap.size(), rand() % (int)terrainMap[0].size()}; //go somewhere random. later replace this with going towards pheromones
//                     ant.move(this->terrainMap, destination, this->foodMap);
//                 }
//                 else {
//                     Coord destination = pheromoneLoc;
//                     ant.move(this->terrainMap, destination, this->foodMap);
//                     this->pheromoneMap[destination.first][destination.second] = 0; //gets rid of pheromone at this location
//                 } 
//             }
//             else {
//                 ant.move(this->terrainMap, destination, this->foodMap);
//             }
//         }

        
//         else if(chebyshevDist > ant.foodRadius) {//if food is outside home view range
        
//                 ant.move(this->terrainMap, foodLoc, this->foodMap); //get food
//                 ant.returnHome(this->terrainMap, this->foodMap);

//         }
        
//         else if(ant.energy > 0) {//explore if you have the energy
                            
//             Coord destination = {ant.position.first + dx[ant.id], ant.position.second + dy[ant.id]}; //Move
            
//             if (destination.first >= terrainMap.size() || destination.second >= terrainMap[0].size()){//if you reach the edge
                
//                 destination = {ant.position.first - dx[ant.id], ant.position.second - dy[ant.id]};
//                 ant.move(this->terrainMap, destination, this->foodMap);    
            
//             }
//             else {

//                 ant.move(this->terrainMap, destination, this->foodMap);

//             }
//         }
        


//         //all ants
//         auto foodLocations = ant.foodScan(this->foodMap);
//         if (foodLocations.size() >= 13) {

//             ant.dropPheromone(this->pheromoneMap);

//         }
//     }
// }






/** You may insert any custom functions below **/

//runs foodScan and returns the closest food location to the ant's current position
Coord Ant::closestFood(MapTemplate &foodMap) {
    std::vector<Coord> foodLocations = this->foodScan(foodMap);
    
    if (foodLocations.empty()) {
        return {-1, -1};   // no food visible
    }

    Coord best = foodLocations[0];
    int bestDistance = abs(foodLocations[0].first - this->position.first) + abs(foodLocations[0].second - this->position.second);

    for (int i = 1; i < foodLocations.size(); ++i) {
        if (abs(foodLocations[i].first - this->position.first) + abs(foodLocations[i].second - this->position.second) < bestDistance) {
            best = foodLocations[i];
            bestDistance = abs(foodLocations[i].first - this->position.first) + abs(foodLocations[i].second - this->position.second);
        }
    }
    return best;
}

//runs pheromoneScan and returns the closest pheromone location to the ant's current position
Coord Ant::closestPheromone(MapTemplate &pheromoneMap) {
    std::vector<Coord> pheromoneLocations = this->pheromoneScan(pheromoneMap);
    
    if (pheromoneLocations.empty()) {
        return {-1, -1};   // no pheromones visible
    }

    Coord best = pheromoneLocations[0];
    int bestDistance = abs(pheromoneLocations[0].first - this->position.first) + abs(pheromoneLocations[0].second - this->position.second);

    for (int i = 1; i < pheromoneLocations.size(); ++i) {
        if (abs(pheromoneLocations[i].first - this->position.first) + abs(pheromoneLocations[i].second - this->position.second) < bestDistance) {
            best = pheromoneLocations[i];
            bestDistance = abs(pheromoneLocations[i].first - this->position.first) + abs(pheromoneLocations[i].second - this->position.second);
        }
    }
    return best;
}