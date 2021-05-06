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
        resultVector.push_back(landmarksStruct);
    }

    return resultVector;
}

vector<landmarksStruct> landmarks::initLandmarks(int amount, adjListCollection &adjListCollection) {
    double minimumDistance = 3000;
    vector<landmarksStruct> resultVector;
    int highestNbr = adjListCollection.idSoFar;
    //srand(4);
    int randomNode = rand() % highestNbr;
    const double INF = std::numeric_limits<double>::infinity();
    //cout << "https://www.openstreetmap.org/node/" << adjListCollection.intIdToLongID[randomNode] << endl;

    vector<vector<double>> markDistanceVectors;
    for (int i = 0; i < amount+1; ++i) {
        if(i!=0) cout << "https://www.openstreetmap.org/node/" << adjListCollection.intIdToLongID[randomNode] << endl;
        //vector<double> euclidDistVector;
        //euclidDistVector.resize(highestNbr,0);
        //double sourceX = adjListCollection.xCoord[randomNode];
        //double sourceY = adjListCollection.yCoord[randomNode];
        //for (int j = 0; j < highestNbr; ++j) {
        //    //if(j % 1000 == 0)
        //    //    cout << "euclidloop: " << j << endl;
        //    double targetX = adjListCollection.xCoord[j];
        //    double targetY = adjListCollection.yCoord[j];
        //    double euclidDistToJ = adjacencyList::euclidDistance(sourceX, sourceY, targetX, targetY);
        //    euclidDistVector[j]=euclidDistToJ;
        //}
        landmarksStruct landmarksStruct;
        spResultStruct distanceToEverything = dijkstra::djikstraShortestPath(randomNode, randomNode, false, adjListCollection);
        landmarksStruct.distanceVec = distanceToEverything.distanceVec;
        landmarksStruct.nodeID = randomNode;    //is suppose to be id not intID
        markDistanceVectors.push_back(landmarksStruct.distanceVec);
        if(i!=0) resultVector.push_back(landmarksStruct);
        double longestDistToClosestMark = 0.0;
        for (int j = 0; j <= highestNbr; ++j) { //loops over all nodes
            //if(j % 1000 == 0)
            //    cout << "longestDistToClosestMark: " << j << endl;
            double closestMark = INF;
            //for (vector<double> distVec : markDistanceVectors){
            for (int k = 0; k < markDistanceVectors.size(); ++k) {
                double distToCandidate = markDistanceVectors[k][j];
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
        if(i==0) markDistanceVectors.pop_back();
    }

    //recast to long longs for calling initLandmarks
    //vector<long long> longIdsOfLandmarks;
    //for (int id:landmarkIDS) {
    //    long long longID = adjacencyList::getLongID(adjListCollection,id);
    //    longIdsOfLandmarks.push_back(longID);
    //}
    return resultVector;
}

adjListCollection reverseAdjListCollection(adjListCollection &adjCol){
    adjListCollection reversedAdjCol;
    reversedAdjCol.adjlst.resize(adjCol.idSoFar, vector<pair<int, double>>{});

}

spResultStruct landmarks::ALTShortestPath(int source, int dest, adjListCollection &adjCol) {
    landmarksStruct bestForward = choseLandmarks(source, dest, adjCol);
    cout << "chosen landmark: "<< bestForward.nodeID << endl;
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
        for(auto i: connectedNodes){
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
        double lowerBound = calcHeuristicDistance(source, dest,index);
        //double distToSource = index.distanceVec[source];
        //double distToDest = index.distanceVec[dest];
//
        //double distFromSource = distToSource; //placeholder values
        //double distFromDest = distToDest;
//
        //double lowerBound = max(distToSource - distToDest, distToDest - distToSource);
        if(bestBound == 0 || lowerBound > bestBound) {
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

    double lowerBound = max(distToStart - distToDest, distToDest - distToStart);
    return lowerBound;
}

