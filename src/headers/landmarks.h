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

    //choose landmarks based on the furthest distance from a random node and then all other landmarks.
    static vector<landmarksStruct>
    initLandmarks(int amount, adjListCollection &adjListCollection, const string& landmarkSelection);

    static spResultStruct ALTShortestPath(int source, int dest, adjListCollection &adjCol);

    static landmarksStruct choseLandmarks(int source, int dest, adjListCollection &collection);

    static float calcHeuristicDistance(int start, int dest, landmarksStruct &currLandmark);

    static adjListCollection reverseAdjListCollection(adjListCollection &adjCol);

    static string calcHeuristicDistanceWithReturn(int source, int target, landmarksStruct &currLandmark);
};
#endif //BACHELORCPPCMAKE_LANDMARKS_H
