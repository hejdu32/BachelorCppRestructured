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
    vector<int> landmarkIDS;
    landmarkIDS.resize(1,randomNode);
    //cout << "https://www.openstreetmap.org/node/" << adjListCollection.intIdToLongID[randomNode] << endl;

    vector<double> accuVec;
    accuVec.resize(highestNbr,0);
    for (int i = 0; i < amount; ++i) {
        vector<double> euclidDistVector;
        euclidDistVector.resize(highestNbr,0);
        double sourceX = adjListCollection.xCoord[randomNode];
        double sourceY = adjListCollection.yCoord[randomNode];
        for (int j = 0; j < highestNbr; ++j) {
            double targetX = adjListCollection.xCoord[j];
            double targetY = adjListCollection.yCoord[j];
            double euclidDistToJ = adjacencyList::euclidDistance(sourceX, sourceY, targetX, targetY);
            euclidDistVector[j]=euclidDistToJ;
        }
        //add the euclid distance vector to accumulator vector
        transform (accuVec.begin(), accuVec.end(), euclidDistVector.begin(), accuVec.begin(), std::plus<double>());

        double bestSoFar = 0;
        int bestSoFarIndex = 0;
        double INF = numeric_limits<double>::infinity();
        for (int j = 0; j < accuVec.size(); ++j) {
            double dist = accuVec[j];
            bool isJLandmark = find(landmarkIDS.begin(),landmarkIDS.end(),j) != landmarkIDS.end();
            bool isJFarEnoughFromOtherLandmarks = true;
            if(!isJLandmark){
                double jX = adjListCollection.xCoord[j];
                double jY = adjListCollection.yCoord[j];
                for (int lmk:landmarkIDS) {
                    double lmkX = adjListCollection.xCoord[lmk];
                    double lmkY = adjListCollection.yCoord[lmk];
                    double distance = adjacencyList::euclidDistance(jX,jY,lmkX,lmkY);
                    if (distance < minimumDistance){
                        isJFarEnoughFromOtherLandmarks = false;
                    }
                }
            }

            if(dist < INF && bestSoFar < dist && !isJLandmark && isJFarEnoughFromOtherLandmarks){
                bestSoFar = dist;
                bestSoFarIndex = j;
            }
        }
        randomNode = bestSoFarIndex;
        //cout << "https://www.openstreetmap.org/node/" << adjListCollection.intIdToLongID[randomNode] << endl;
        landmarkIDS.push_back(randomNode);
    }
    //recast to long longs for calling initLandmarks
    vector<long long> longIdsOfLandmarks;
    for (int id:landmarkIDS) {
        long long longID = adjacencyList::getLongID(adjListCollection,id);
        longIdsOfLandmarks.push_back(longID);
    }
    return initLandmarks(longIdsOfLandmarks,adjListCollection);
}

spResultStruct landmarks::ALTShortestPath(int source, int dest, adjListCollection &adjCol) {
    landmarksStruct bestForward = choseLandmarks(source, dest, adjCol);
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
        double distToSource = index.distanceVec[source];
        double distToDest = index.distanceVec[dest];

        double distFromSource = distToSource; //placeholder values
        double distFromDest = distToDest;

        double lowerBound = max(distToSource - distToDest, distToDest - distToSource);
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

