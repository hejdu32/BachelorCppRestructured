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

    //vector<long long> landmarksIDs = {322591088, 259252468, 6158438720, 330038011, 5584771074, 6285925457, 4160003077, 963497183}; //hardcoded landmarks for malta
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
    vector<landmarksStruct> resultVector;
    int highestNbr = adjListCollection.idSoFar;
    srand(100);
    int randomNode = rand() % highestNbr;
    map<int,bool> landmarkIDS;

    //cout << "Random node " << adjListCollection.intIdToLongID[randomNode] << endl;
    vector<double> accuVec;
    accuVec.resize(highestNbr,0);
    for (int i = 0; i < amount; ++i) {
        landmarksStruct landmarksStruct;
        spResultStruct distanceToEverything = dijkstra::djikstraShortestPath(randomNode, randomNode, false, adjListCollection);
        landmarksStruct.distanceVec = distanceToEverything.distanceVec;
        landmarksStruct.nodeID = adjListCollection.intIdToLongID[randomNode];    //is suppose to be id not intID
        resultVector.push_back(landmarksStruct);


        //cout << "Accuvec size " << accuVec.size() << endl;

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

        transform (accuVec.begin(), accuVec.end(), euclidDistVector.begin(), accuVec.begin(), std::plus<double>());

        int counter = 0;
        double bestSoFar = 0;
        int bestSoFarIndex = 0;
        double INF = numeric_limits<double>::infinity();

        for (int j = 0; j < accuVec.size(); ++j) {
            if(accuVec[j] == numeric_limits<double>::infinity()){
                counter ++;
            }
            double dist = accuVec[j];
            if(dist < INF && bestSoFar < dist && landmarkIDS.find(j)==landmarkIDS.end()){
                //cout << "J: " << j << " dist: "<< dist << " bestSoFar: "<< bestSoFar << endl;
                bestSoFar = dist;
                bestSoFarIndex = j;
            }
        }
        //cout << "inf counter: " << counter << endl;

        randomNode = bestSoFarIndex;
        //cout << "intID for randomNode: " << randomNode << endl;
        //cout << "Chosen node: " << adjListCollection.intIdToLongID[randomNode] << " with dist: " << accuVec[randomNode] << endl;

        cout << "https://www.openstreetmap.org/node/" << adjListCollection.intIdToLongID[randomNode] << endl;
        landmarkIDS.insert(make_pair(randomNode, true));
    }

    return resultVector;
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

