//
// Created by a on 07-04-2021.
//

#ifndef BACHELORCPPCMAKE_ASTAR_H
#define BACHELORCPPCMAKE_ASTAR_H
#include "adjacencyList.h"
#include<iostream>
#include<vector>
#include<set>

using namespace std;
class aStar {
public:
    static spResultStruct aStarShortestPath(int source, int dest, adjListCollection &adjCol);
};


#endif //BACHELORCPPCMAKE_ASTAR_H
