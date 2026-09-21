//
// Created by dusan on 9/15/26.
//

#include "../include/antworld.h"


/** @brief this is where you as the applicant will make use of the above functions to develop your solution.
 * here are some existing examples of how calling these functions works to help get you started!
 */
void AntWorld::forage() {
        for (auto &ant : this->ants) {
        Coord foodLoc = ant.closestFood(this->foodMap);

        if (foodLoc == Coord(-1, -1)) {//if there is no food in sight, go up and to the left
            if (ant.energy > 0) {
                
                Coord destination = {ant.position.first - 1, ant.position.second + 1};
                
                if (destination.first >= terrainMap.size() || destination.second >= terrainMap[0].size()){//if you reach the edge, go back and forth until you run out of energy
                    
                    destination = {ant.position.first + 1, ant.position.second - 1};
                    ant.move(this->terrainMap, destination, this->foodMap);    
                }
                else {
                    ant.move(this->terrainMap, destination, this->foodMap);
                }
            }

        } else {//if you see food, get it and bring it home
            ant.move(this->terrainMap, foodLoc, this->foodMap);
            ant.returnHome(this->terrainMap, this->foodMap);
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
}

/** You may insert any custom functions below **/
