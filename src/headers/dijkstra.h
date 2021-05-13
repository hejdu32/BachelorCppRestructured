//
// Created by a on 31-03-2021.
//

#ifndef DIJKSTRA
#define DIJKSTRA
#include "adjacencyList.h"



using namespace std;
class dijkstra {
public:
    static spResultStruct djikstraShortestPath(int source, int dest,bool earlyStopping, adjListCollection &adjCol);
};


#endif //DIJKSTRA
