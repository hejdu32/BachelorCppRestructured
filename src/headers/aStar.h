//
// Created by a on 07-04-2021.
//

#ifndef BACHELORCPPCMAKE_ASTAR_H
#define BACHELORCPPCMAKE_ASTAR_H
#include "adjacencyList.h"
#include<iostream>
#include<vector>
#include<set>
#include<tuple>

using namespace std;
class aStar {
public:
    static tuple<double, vector<int>> aStarShortestPath(int source, int dest, adjListCollection &adjListCollection);
};


#endif //BACHELORCPPCMAKE_ASTAR_H
