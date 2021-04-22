//
// Created by a on 31-03-2021.
//

#ifndef DIJKSTRA
#define DIJKSTRA
#include "adjacencyList.h"
#include<iostream>
#include<vector>
#include<set>
#include<tuple>



using namespace std;
class dijkstra {
public:
    static tuple<double, vector<int>,vector<double>> djikstraShortestPath(int source, int dest,bool earlyStopping, adjListCollection &adjCol);
};


#endif //DIJKSTRA
