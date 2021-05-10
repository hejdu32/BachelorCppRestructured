//
// Created by svend on 23-04-2021.
//

#include "../headers/landmarks.h"
#include "../headers/dijkstra.h"
#include <queue>

using namespace std;

struct comparator{
    constexpr bool operator()(
            pair<int, double> const& a,
            pair<int, double> const& b)
    const noexcept
    {
        return a.second > b.second;
    }
};

vector<landmarksStruct> landmarks::initLandmarks(vector<long long> nodeIDs, adjListCollection &adjListCollection) {
    vector<landmarksStruct> resultVector;
    for (auto id:nodeIDs) {
        int intID = adjacencyList::getIntID(adjListCollection,id);
        landmarksStruct landmarksStruct;
        spResultStruct distanceToEverything = dijkstra::djikstraShortestPath(intID, intID, false, adjListCollection);
        landmarksStruct.distanceVec = distanceToEverything.distanceVec;
        landmarksStruct.nodeID = id;    //is suppose to be id not intID
        resultVector.emplace_back(landmarksStruct);
    }

    return resultVector;
}

adjListCollection landmarks::reverseAdjListCollection(adjListCollection &adjCol){
    adjListCollection reversedAdjCol;
    reversedAdjCol.adjlst.resize(adjCol.idSoFar, vector<pair<int, double>>{});
    for(int i = 0; i < adjCol.adjlst.size(); i++){
        vector<pair<int, double>> currentEdges = adjCol.adjlst[i];
        for(int j = 0; j < currentEdges.size(); j++) {
            pair<int, double> temp = currentEdges[j];
            int newSource = temp.first;
            int newDist = temp.second;
            reversedAdjCol.adjlst[newSource].push_back(make_pair(i, newDist));
        }
    }
    reversedAdjCol.idSoFar = adjCol.idSoFar;
    return reversedAdjCol;
}

vector<landmarksStruct> landmarks::initLandmarks(int amount, adjListCollection &adjListCollection) {
    struct adjListCollection reversedAdjListCollection = reverseAdjListCollection(adjListCollection);
    vector<landmarksStruct> resultVector;
    int highestNbr = adjListCollection.idSoFar;

    int randomNode = rand() % highestNbr;
    const double INF = std::numeric_limits<double>::infinity();
    //cout << "https://www.openstreetmap.org/node/" << adjListCollection.intIdToLongID[randomNode] << endl;

    //vector<vector<double>> markDistanceVectors;
    for (int i = 0; i < amount+1; ++i) {
        //if(i!=0) cout << "https://www.openstreetmap.org/node/" << adjListCollection.intIdToLongID[randomNode] << endl;
        landmarksStruct landmarksStruct;
        spResultStruct distanceToEverything = dijkstra::djikstraShortestPath(randomNode, randomNode, false, adjListCollection);
        spResultStruct distanceFromEverything = dijkstra::djikstraShortestPath(randomNode, randomNode, false, reversedAdjListCollection);
        landmarksStruct.distanceVec = distanceToEverything.distanceVec;
        landmarksStruct.reversedDistanceVec = distanceFromEverything.distanceVec;
        landmarksStruct.nodeID = adjListCollection.intIdToLongID[randomNode];    //is suppose to be id not intID
        //markDistanceVectors.emplace_back((landmarksStruct.distanceVec));

        if(i!=0) resultVector.emplace_back(landmarksStruct);

        double longestDistToClosestMark = 0.0;
        for (int j = 0; j <= highestNbr; ++j) { //loops over all nodes
            //if(j % 1000 == 0)
            //    cout << "longestDistToClosestMark: " << j << endl;
            double closestMark = INF;
            //for (vector<double> distVec : markDistanceVectors){
            for (int k = 0; k < resultVector.size(); ++k) {
                double distToCandidate = resultVector[k].distanceVec[j];
                //if(j % 1000 == 0)
                //    cout << "looping over landmarks: " << j << endl;
                //double distToCandidate = distVec[j];
                if (distToCandidate != INF && distToCandidate < closestMark)
                    closestMark = distToCandidate;
            }
            if (closestMark != INF && closestMark > longestDistToClosestMark) {
                longestDistToClosestMark = closestMark;
                randomNode = j;
            }
            //if(j % 1000 == 0)
            //    cout << "last thing of the loop: " << j << endl;
        }
        //if(i==0) markDistanceVectors.pop_back();
    }

    //recast to long longs for calling initLandmarks
    //vector<long long> longIdsOfLandmarks;
    //for (int id:landmarkIDS) {
    //    long long longID = adjacencyList::getLongID(adjListCollection,id);
    //    longIdsOfLandmarks.push_back(longID);
    //}
    return resultVector;
}


spResultStruct landmarks::ALTShortestPath(int source, int dest, adjListCollection &adjCol) {
    landmarksStruct bestForward = choseLandmarks(source, dest, adjCol);
    //cout << "chosen landmark: "<< bestForward.nodeID << endl;
    const double INF = std::numeric_limits<double>::infinity();
    int sizeOfGraph = adjCol.idSoFar;
    //initilaize distance from source to everything to infinity
    //distance from source to source to 0
    vector<double> distance(sizeOfGraph, INF);
    distance[source] = 0;
    //has the node been seen vector
    vector<bool> nodeSeen(sizeOfGraph, false);
    nodeSeen[source] = true;
    //path from source to destination
    vector<int> prevNode(sizeOfGraph, -1);
    //prevNode[source] = -1;
    //heap of nodes to evaluate
    priority_queue<pair<int, double>, vector<pair<int, double>>, comparator> minHeap;

    minHeap.push(make_pair(source, 0.0));

    while (!minHeap.empty()){
        //pop the top element
        pair<int,double> head = minHeap.top();
        minHeap.pop();
        int headId = head.first;
        //Have we reached destination check
        if (headId==dest){
            //we have arrived at destination and we are done
            //cout << "we have hit destination \n";
            break;
        }
        //add new nodes to queue
        auto connectedNodes = adjCol.adjlst[headId];
        for(auto const &i: connectedNodes){
            int node = i.first;
            double weight = i.second;
            double heuristIntermediate = calcHeuristicDistance(node, dest, bestForward);
            //relaxation step, in astar we add the heuristic weight in to consideration
            if(!nodeSeen[node] && (distance[headId] + weight + heuristIntermediate) < distance[node] + heuristIntermediate){
                //update the distance to the node and add it to the queue
                distance[node] = distance[headId]+weight;
                prevNode[node] = headId; //remember the node before for finding the shortest path to destination
                //add the heuristic to the weight so we sort based on it.

                minHeap.push(make_pair(node, distance[node] + heuristIntermediate));//
            }
        }
        //mark head as it has been seen and cant be considered again
        nodeSeen[headId] = true;
    }
    //cout << "astar nodes considered: " << nodesConsidered(nodeSeen) << endl;
    spResultStruct resultStruct;
    resultStruct.distanceToDest = distance[dest];
    resultStruct.distanceVec = distance;
    resultStruct.prevNode = prevNode;
    resultStruct.chosenLandmark = bestForward.nodeID;
    return resultStruct;
}

landmarksStruct landmarks::choseLandmarks(int source, int dest, adjListCollection &collection) {
    int bestBounding;
    double bestBound = 0;

    for(int i = 0; i < collection.landmarksStructs.size(); i++) {
        double lowerBound = calcHeuristicDistance(source, dest,collection.landmarksStructs[i]);
        //cout << "landmark: " << collection.landmarksStructs[i].nodeID << " lowerBound " << lowerBound << endl;
        if(bestBound == 0 || lowerBound > bestBound) {
            bestBounding = i;
            bestBound = lowerBound;
        }
    }
    return collection.landmarksStructs[bestBounding];
}

double landmarks::calcHeuristicDistance(int source, int target, landmarksStruct &currLandmark) {
    double distFromSourceToLandmark = currLandmark.reversedDistanceVec[source];
    double distFromTargetToLandmark = currLandmark.reversedDistanceVec[target];
    double lowerBoundToLandmark =  distFromSourceToLandmark - distFromTargetToLandmark;


    double distFromLandmarkToSource = currLandmark.distanceVec[source];
    double distFromLandmarkToTarget = currLandmark.distanceVec[target];
    double lowerBoundFromLandmark = distFromLandmarkToTarget - distFromLandmarkToSource;

    double largestLowerbound = max(lowerBoundToLandmark, lowerBoundFromLandmark);
    return largestLowerbound;
}

