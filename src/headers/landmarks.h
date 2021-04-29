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
    static vector<landmarksStruct> initLandmarks(vector<long long> nodeIDs, adjListCollection &adjListCollection);
    static vector<landmarksStruct> initLandmarks(int amount, adjListCollection &adjListCollection);
    static spResultStruct ALTShortestPath(int source, int dest, adjListCollection &adjCol);

    static landmarksStruct choseLandmarks(int source, int dest, adjListCollection &collection);
    static double calcHeuristicDistance(int start, int dest, landmarksStruct &currLandmark);
};

#endif //BACHELORCPPCMAKE_LANDMARKS_H
