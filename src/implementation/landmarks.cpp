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

vector<landmarksStruct> landmarks::initLandmarks(string country, adjListCollection &adjListCollection) {
    vector<landmarksStruct> resultVector;

    if (country == "malta"){
        vector<long long> landmarksIDs = {322591088, 259252468, 6158438720, 330038011, 5584771074, 6285925457, 4160003077, 963497183}; //hardcoded landmarks for malta
        for (auto id:landmarksIDs) {
            int intID = adjacencyList::getIntID(adjListCollection,id);
            landmarksStruct landmarksStruct;
            spResultStruct distanceToEverything = dijkstra::djikstraShortestPath(intID, intID, false, adjListCollection);

            landmarksStruct.distanceVec = distanceToEverything.distanceVec;
            landmarksStruct.nodeID = id;    //is suppose to be id not intID
            landmarksStruct.prevNode = distanceToEverything.prevNode;

            resultVector.push_back(landmarksStruct);
        }
    }
    return resultVector;
}

spResultStruct landmarks::ALTShortestPath(int source, int dest, adjListCollection &adjCol) {
    landmarksStruct bestForward = choseLandmarks(source, dest, adjCol);

    //astar here
    const double INF = 999999999999;
    int sizeOfGraph = adjCol.idSoFar;
    int meanSpeed = 130;
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
    //astar ends

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

        //double aX = adjacencyList::getxCoord(adjCol, headId);  double aY = adjacencyList::getyCoord(adjCol, headId);
        //double heuristHead = calcHeuristicDistance(fdestX, fdestY, aX, aY, meanSpeed);

        //add new nodes to queue
        for(auto i: adjCol.adjlst[headId]){
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
    return resultStruct;
}

landmarksStruct landmarks::choseLandmarks(int source, int dest, adjListCollection &collection) {
    vector<landmarksStruct> landmarksVector = collection.landmarksStructs;
    landmarksStruct bestBounding;
    double bestBound = 0;

    for (auto index : landmarksVector) {
        double distToSource = index.distanceVec[source];
        double distToDest = index.distanceVec[dest];

        double distFromSource = distToSource; //placeholder values
        double distFromDest = distToDest;

        double lowerBound = max(distToDest - distToSource, distFromSource - distFromDest);
        if(bestBound == 0 || lowerBound < bestBound) {
            bestBounding = index;
            bestBound = lowerBound;
        }
    }
    return bestBounding;
}

double landmarks::calcHeuristicDistance(int start, int dest, landmarksStruct &currLandmark) {
    double distToStart = currLandmark.distanceVec[start];
    double distToDest = currLandmark.distanceVec[dest];


    double distFromStart = distToStart; //placeholder values
    double distFromDest = distToDest;

    double lowerBound = max(distToDest - distToStart, distFromStart - distFromDest);
    return lowerBound;
}

