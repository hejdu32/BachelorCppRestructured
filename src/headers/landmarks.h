//
// Created by svend on 23-04-2021.
//

#ifndef BACHELORCPPCMAKE_LANDMARKS_H
#define BACHELORCPPCMAKE_LANDMARKS_H
#include "adjacencyList.h"
#include<vector>

using namespace std;
class landmarks {
public:
    //choose a fixed list of landmarks
    static vector<landmarksStruct> initLandmarks(vector<long long> nodeIDs, adjListCollection &adjListCollection);

    //choose landmarks based on the furthest dijkstra distance from a random node and then all other landmarks.
    static vector<landmarksStruct>
    initLandmarks(int amount, adjListCollection &adjListCollection);

    //the ALT shortest path algorithm
    static spResultStruct ALTShortestPath(int source, int dest, adjListCollection &adjCol);

    //Chose the best bounding landmark based on source and target node
    static landmarksStruct choseLandmarks(int source, int dest, adjListCollection &collection);

    //calculates the heuristic value between two nodes and a chosen landmark
    static float calcHeuristicDistance(int start, int dest, landmarksStruct &currLandmark);

    //reverse the adjacency list
    static adjListCollection reverseAdjListCollection(adjListCollection &adjCol);

    //returns wheter the landmark was a to or from landmark
    static string calcHeuristicDistanceWithReturn(int source, int target, landmarksStruct &currLandmark);

    //choses a random intial landmark that is connected to 50% of the graph
    static int chooseRandomLandmark(adjListCollection &collection);
};
#endif //BACHELORCPPCMAKE_LANDMARKS_H
