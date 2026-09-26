//
// Created by dusan on 9/15/26.
//

#include "../include/antworld.h"
#include <cstdlib>   // for rand()


/** @brief this is where you as the applicant will make use of the above functions to develop your solution.
 * here are some existing examples of how calling these functions works to help get you started!
 */
void AntWorld::forage() {
    for (int i = 1; i < ants.size() + 1; i++){    
        Ant &ant = this->ants[i - 1];

        const int dx[] = {0, 3, 3,  3,  0, -3, -3, -3};
        const int dy[] = {3, 3, 0, -3, -3, -3,  0,  3};
        const int CHECKx[] = {0, 4, 4, 4, 0, -4, -4, -4};
        const int CHECKy[] = {4, 4, 0, -4, -4, -4, 0, 4};

        
        Coord homeCheck = {ant.homeCoord.first + CHECKx[ant.id], ant.homeCoord.second + CHECKy[ant.id]};
        
        Coord foodLoc = ant.closestFood(this->foodMap);
        Coord pheromoneLoc = ant.closestPheromone(this->pheromoneMap);

        int rowDiff = abs(foodLoc.first - ant.homeCoord.first);
        int colDiff = abs(foodLoc.second - ant.homeCoord.second);
        int chebyshevDist = std::max(rowDiff, colDiff); //calculates chebyshev distance (distance like a chessboard) rathar than Manhattan distance
        

        //ants that don't initially explore
        if (homeCheck.first >= terrainMap.size() || homeCheck.second >= terrainMap[0].size()){//if home is near an edge
            
            if (foodLoc == Coord(-1, -1)) {//if there is no food in sight
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
            else{
                ant.move(this->terrainMap, foodLoc, this->foodMap); //get food
                ant.returnHome(this->terrainMap, this->foodMap);
            }
            
        }



        //ants that go exploring
        else if (foodLoc == Coord(-1, -1)) {//if there is no food in sight
                   
            Coord destination = {ant.position.first + dx[ant.id], ant.position.second + dy[ant.id]}; //Move
            
            if (destination.first >= terrainMap.size() || destination.second >= terrainMap[0].size()){//if you reach the edge

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
            else {
                ant.move(this->terrainMap, destination, this->foodMap);
            }
        }

        
        else if(chebyshevDist > ant.foodRadius) {//if food is outside home view range
        
                ant.move(this->terrainMap, foodLoc, this->foodMap); //get food
                ant.returnHome(this->terrainMap, this->foodMap);

        }
        
        else if(ant.energy > 0) {//explore if you have the energy
                            
            Coord destination = {ant.position.first + dx[ant.id], ant.position.second + dy[ant.id]}; //Move
            
            if (destination.first >= terrainMap.size() || destination.second >= terrainMap[0].size()){//if you reach the edge
                
                destination = {ant.position.first - dx[ant.id], ant.position.second - dy[ant.id]};
                ant.move(this->terrainMap, destination, this->foodMap);    
            
            }
            else {

                ant.move(this->terrainMap, destination, this->foodMap);

            }
        }
        


        //all ants
        auto foodLocations = ant.foodScan(this->foodMap);
        if (foodLocations.size() >= 13) {

            ant.dropPheromone(this->pheromoneMap);

        }
    }
}


    // std::vector<Coord> visibleFood = this->ants[0].foodScan(this->foodMap);
    //
    // Coord desiredDestination = Coord(5, 5);
    // Coord finalPos = this->ants[0].move(this->terrainMap, desiredDestination, this->foodMap);
    // bool destCheck = (desiredDestination == finalPos);
    //
    // this->ants[0].dropPheromone(this->pheromoneMap);
    //
    // this->ants[0].erasePheromone(this->pheromoneMap);
    //
    // this->ants[0].returnHome(this->terrainMap, this->foodMap);


/** You may insert any custom functions below **/
