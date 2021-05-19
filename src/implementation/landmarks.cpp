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

vector<landmarksStruct> landmarks::initLandmarks(vector<long long> nodeIDs, adjListCollection &adjCol) {
    vector<landmarksStruct> resultVector;
    for (auto id:nodeIDs) {
        int intID = adjacencyList::getIntID(adjCol, id);
        landmarksStruct landmarksStruct;
        spResultStruct distanceToEverything = dijkstra::djikstraShortestPath(intID, intID, false, adjCol);
        landmarksStruct.distanceVec = distanceToEverything.distanceVec;
        landmarksStruct.nodeID = id;    //is suppose to be id not intID
        resultVector.emplace_back(landmarksStruct);
    }

    return resultVector;
}

static void printVec(adjListCollection &adjCol,vector<pair<int, double>>const &input){
    cout << "[";
    for (auto i : input) {
        cout << "dest: "<< adjCol.intIdToLongID.find(i.first)->second << " dist:" << i.second << ' ';
    }
    cout << "]";
}

adjListCollection landmarks::reverseAdjListCollection(adjListCollection &adjCol){
    adjListCollection reversedAdjCol;
    reversedAdjCol.adjlst.resize(adjCol.idSoFar, vector<pair<int, double>>{});
    for(int i = 0; i < adjCol.adjlst.size(); i++){
        vector<pair<int, double>> currentEdges = adjCol.adjlst[i];
        for(int j = 0; j < currentEdges.size(); j++) {
            pair<int, double> temp = currentEdges[j];
            int newSource = temp.first;
            double newDist = temp.second;
            adjacencyList::addEdge(reversedAdjCol,newSource,i,newDist);
        }
    }
    reversedAdjCol.idSoFar = adjCol.idSoFar;
    return reversedAdjCol;
}

vector<landmarksStruct> landmarks::initLandmarks(int amount, adjListCollection &adjCol) {
    struct adjListCollection reversedAdjListCollection = reverseAdjListCollection(adjCol);
    vector<landmarksStruct> resultVector;
    int highestNbr = adjCol.idSoFar;
    int randomNode = rand() % highestNbr;

    const double INF = std::numeric_limits<double>::infinity();
    //cout << "https://www.openstreetmap.org/node/" << adjCol.intIdToLongID[randomNode] << endl;

    //vector<vector<double>> markDistanceVectors;
    for (int i = 0; i < amount+1; ++i) {
        //if(i==0){cout << "https://www.openstreetmap.org/node/" << adjCol.intIdToLongID[randomNode] << "#map=8/56.216/12.816"<< endl;}
        landmarksStruct landmarksStruct;
        spResultStruct distanceToEverything = dijkstra::djikstraShortestPath(randomNode, randomNode, false, adjCol);
        spResultStruct distanceFromEverything = dijkstra::djikstraShortestPath(randomNode, randomNode, false, reversedAdjListCollection);
        landmarksStruct.distanceVec = distanceToEverything.distanceVec;
        landmarksStruct.reversedDistanceVec = distanceFromEverything.distanceVec;
        landmarksStruct.nodeID = adjCol.intIdToLongID[randomNode];    //is suppose to be id not intID

        //markDistanceVectors.emplace_back((landmarksStruct.distanceVec));
        //cout << "https://www.openstreetmap.org/node/" << adjCol.intIdToLongID[randomNode] << "#map=8/56.216/12.816"<< endl;
        resultVector.emplace_back(landmarksStruct);
        cout << "emplacing our struct " << endl;

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
        cout<<"weve found a new landmark"<<endl;
        //if(i==0) markDistanceVectors.pop_back();
    }
    resultVector.erase(resultVector.begin());
    cout<<"size of result vector: " << resultVector.size() << endl;
    for (landmarksStruct i:resultVector) {
    cout << "lmk: " << i.nodeID << endl;
    }

    //recast to long longs for calling initLandmarks
    //vector<long long> longIdsOfLandmarks;
    //for (int id:landmarkIDS) {
    //    long long longID = adjacencyList::getLongID(adjCol,id);
    //    longIdsOfLandmarks.push_back(longID);
    //}
    return resultVector;
}


spResultStruct landmarks::ALTShortestPath(int source, int dest, adjListCollection &adjCol) {
    landmarksStruct bestForward = choseLandmarks(source, dest, adjCol);
    //cout << "chosen landmark: "<< bestForward.nodeID << endl;
    const double INF = std::numeric_limits<double>::infinity();
    int sizeOfGraph = adjCol.idSoFar;
    //initialize distance from source to everything to infinity
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

    int counter = 0;
    while (!minHeap.empty()){
        //pop the top element
        pair<int,double> head = minHeap.top();
        minHeap.pop();
        int headId = head.first;

        //counter ++;
        //if (counter%100000==0 && counter != 0){
        //    //cout << "rebuilding beep boop optimzing beep boop" << endl;
        //    bestForward = choseLandmarks(headId, dest, adjCol);
        //    priority_queue<pair<int, double>, vector<pair<int, double>>, comparator> heapRebuild;
        //    for (int i = 0; i < distance.size(); ++i) {
        //        if (distance[i] != INF && !nodeSeen[i]){
        //            double heuristIntermediate = calcHeuristicDistance(i, dest, bestForward);
        //            heapRebuild.push(make_pair(i, distance[i] + heuristIntermediate));
        //            minHeap = heapRebuild;
        //        }
        //    }
        //}

        //Have we reached destination check
        if (headId==dest){
            //we have arrived at destination and we are done
            //cout << "we have hit destination \n";
            break;
        }
        //mark head as it has been seen and cant be considered again
        nodeSeen[headId] = true;
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

    }
    //cout << "astar nodes considered: " << nodesConsidered(nodeSeen) << endl;
    spResultStruct result={distance[dest], distance, prevNode, bestForward.nodeID};

    return result;
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
    double toLandmark = distFromSourceToLandmark - distFromTargetToLandmark;

    double distFromLandmarkToSource = currLandmark.distanceVec[source];
    double distFromLandmarkToTarget = currLandmark.distanceVec[target];
    double fromLandmark = distFromLandmarkToTarget - distFromLandmarkToSource;

    bool isLowerBFromInfinite =  !isfinite(fromLandmark);
    bool isLowerBToInfinite = !isfinite(toLandmark);
    if (isLowerBFromInfinite){
        if (isLowerBToInfinite){
            //this should only happen if the we have unconnected graphs
            //cout << "both lower bounds are infinity something went wrong" << endl;
        }
        return toLandmark;
    }
    if (isLowerBToInfinite){
        return fromLandmark;
    }
    double largestLowerbound = max(toLandmark, fromLandmark);
    return largestLowerbound;
}

