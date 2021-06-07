//
// Created by a on 31-03-2021.
//

#ifndef DIJKSTRA
#define DIJKSTRA
#include "adjacencyList.h"



using namespace std;
class dijkstra {
public:
    //the Dijkstra shortest path algorithm, with flags for early stopping
    static spResultStruct djikstraShortestPath(int source, int dest,bool earlyStopping, adjListCollection &adjCol);
};


#endif //DIJKSTRA
