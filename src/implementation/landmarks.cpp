//
// Created by svend on 23-04-2021.
//

#include "../headers/landmarks.h"
#include "../headers/dijkstra.h"
#include <queue>
#include <utility>

using namespace std;


struct comparator{
    constexpr bool operator()(
            pair<int, float> const& a,
            pair<int, float> const& b)
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
        landmarksStruct.nodeID = id;
        resultVector.emplace_back(landmarksStruct);
    }

    return resultVector;
}

adjListCollection landmarks::reverseAdjListCollection(adjListCollection &adjCol){
    adjListCollection reversedAdjCol;
    reversedAdjCol.adjlst.resize(adjCol.idSoFar, vector<pair<int, float>>{});
    for(int i = 0; i < adjCol.adjlst.size(); i++){
        vector<pair<int, float>> currentEdges = adjCol.adjlst[i];
        for(int j = 0; j < currentEdges.size(); j++) {
            pair<int, float> temp = currentEdges[j];
            int newSource = temp.first;
            float newDist = temp.second;
            reversedAdjCol.adjlst[newSource].emplace_back(make_pair(i, newDist));
        }
    }
    reversedAdjCol.idSoFar = adjCol.idSoFar;
    return reversedAdjCol;
}

vector<landmarksStruct> landmarks::initLandmarks(int amount, adjListCollection &adjListCollection, const string& landmarkSelection) {
    struct adjListCollection reversedAdjListCollection = reverseAdjListCollection(adjListCollection);
    vector<landmarksStruct> resultVector;
    int highestNbr = adjListCollection.idSoFar;

    int randomNode = chooseRandomLandmark(adjListCollection);
    const float INF = std::numeric_limits<float>::infinity();
    vector<vector<float>> lmkDistanceVectors;



    for (int i = 0; i < amount+1; ++i) {

        landmarksStruct landmarksStruct;
        spResultStruct distanceToEverything = dijkstra::djikstraShortestPath(randomNode, randomNode, false, adjListCollection);
        spResultStruct distanceFromEverything = dijkstra::djikstraShortestPath(randomNode, randomNode, false, reversedAdjListCollection);
        landmarksStruct.distanceVec = distanceToEverything.distanceVec;
        landmarksStruct.reversedDistanceVec = distanceFromEverything.distanceVec;
        landmarksStruct.nodeID = adjListCollection.intIdToLongID[randomNode];    //is suppose to be id not intID

        resultVector.emplace_back(landmarksStruct);


        float longestDistToClosestMark = 0.0;
        for (int j = 0; j <= highestNbr; ++j) { //loops over all nodes
            float closestMark = INF;
            for (int k = 0; k < resultVector.size(); ++k) {
                float distToCandidate =0;

                distToCandidate = resultVector[k].distanceVec[j];

                if (distToCandidate != INF  && distToCandidate < closestMark)
                    closestMark = distToCandidate;
            }
            if (closestMark != INF && closestMark > longestDistToClosestMark) {
                longestDistToClosestMark = closestMark;
                randomNode = j;
            }
        }

    }
    resultVector.erase(resultVector.begin());
    return resultVector;
}

priority_queue<pair<int, float>, vector<pair<int, float>>, comparator> replaceHeap(landmarksStruct &landmark,vector<float> &distance, vector<bool> &nodeSeen, int dest, priority_queue<pair<int, float>, vector<pair<int, float>>, comparator> &minHeap){
    priority_queue<pair<int, float>, vector<pair<int, float>>, comparator> newHeap;
    while (!minHeap.empty()){
        pair<int,float> head = minHeap.top();
        minHeap.pop();
        int nodeID = head.first;
        if (!nodeSeen[nodeID]){
        float newHeuristIntermediate = landmarks::calcHeuristicDistance(nodeID, dest, landmark);
        float distanceToNode = distance[nodeID];
        newHeap.push(make_pair(nodeID,distanceToNode+newHeuristIntermediate));
        }
    }
    return newHeap;
}


spResultStruct landmarks::ALTShortestPath(int source, int dest, adjListCollection &adjCol) {
    //Choose which landmark is best for this problem
    landmarksStruct landmark = choseLandmarks(source, dest, adjCol);
    const float INF = std::numeric_limits<float>::infinity();
    int sizeOfGraph = adjCol.idSoFar; //Amount of nodes in graph
    //initialize distance from source to everything to infinity, and source to 0
    vector<float> distance(sizeOfGraph, INF);
    distance[source] = 0;
    //has the node been seen vector
    vector<bool> nodeSeen(sizeOfGraph, false);
    //The pi array containing the path from source to destination
    vector<int> prevNode(sizeOfGraph, -1);
    //heap of nodes to evaluate
    priority_queue<pair<int, float>, vector<pair<int, float>>, comparator> minHeap;

    minHeap.push(make_pair(source, 0.0));
    //int counter = 0;
    //cout << "old lmk: " << landmark.nodeID;
    while (!minHeap.empty()){
        //pop the top element
        pair<int,float> head = minHeap.top();
        minHeap.pop();
        int headId = head.first;

        if (headId==dest){ //Early stopping check
            break;
        }
        nodeSeen[headId] = true; //mark head as considered
        auto connectedNodes = adjCol.adjlst[headId];
        for(auto const &i: connectedNodes){ //Check adjacent nodes
            int node = i.first;
            float weight = i.second;
            float heuristIntermediate = calcHeuristicDistance(node, dest, landmark);
            //relaxation step, in ALT we add the heuristic weight in to consideration
            if(!nodeSeen[node] && (distance[headId] + weight) < distance[node]){
                distance[node] = distance[headId]+weight;
                prevNode[node] = headId; //Update the previous node in the path
                //add the heuristic to the weight so we sort based on it.
                minHeap.push(make_pair(node, distance[node] + heuristIntermediate));
            }
        }

    }
    spResultStruct result = {distance[dest], distance, prevNode, nodeSeen,landmark.nodeID};
    //cout << endl;
    return result;
}

landmarksStruct landmarks::choseLandmarks(int source, int dest, adjListCollection &collection) {
    int bestBounding;
    float bestBound = 0;
    //Iterate over all landmarks and choose the one with the best bound
    for(int i = 0; i < collection.landmarksStructs.size(); i++) {
        float lowerBound = calcHeuristicDistance(source, dest,collection.landmarksStructs[i]);
        if(bestBound == 0 || lowerBound > bestBound) {
            bestBounding = i;
            bestBound = lowerBound;
        }
    }
    return collection.landmarksStructs[bestBounding];
}



float landmarks::calcHeuristicDistance(int source, int target, landmarksStruct &currLandmark) {
    //From source/Target to landmark
    float distFromSourceToLandmark = currLandmark.reversedDistanceVec[source];
    float distFromTargetToLandmark = currLandmark.reversedDistanceVec[target];
    float toLandmark = distFromSourceToLandmark - distFromTargetToLandmark;
    //From landmark to source/Target
    float distFromLandmarkToSource = currLandmark.distanceVec[source];
    float distFromLandmarkToTarget = currLandmark.distanceVec[target];
    float fromLandmark = distFromLandmarkToTarget - distFromLandmarkToSource;

    //handle the case where either source or target is not in the same graph as the landmark
    bool isLowerBFromInfinite = !isfinite(fromLandmark);
    bool isLowerBToInfinite = !isfinite(toLandmark);
    if (isLowerBFromInfinite) {
        if (isLowerBToInfinite) {
            //this should only happen if the we have unconnected graphs
        }
        return toLandmark;
    }
    if (isLowerBToInfinite) {
        return fromLandmark;
    }
    float largestLowerbound = max(toLandmark, fromLandmark);
    return largestLowerbound;
}
string landmarks::calcHeuristicDistanceWithReturn(int source, int target, landmarksStruct &currLandmark) {
        //From source/Target to landmark
        float distFromSourceToLandmark = currLandmark.reversedDistanceVec[source];
        float distFromTargetToLandmark = currLandmark.reversedDistanceVec[target];
        float toLandmark = distFromSourceToLandmark - distFromTargetToLandmark;
        //From landmark to source/Target
        float distFromLandmarkToSource = currLandmark.distanceVec[source];
        float distFromLandmarkToTarget = currLandmark.distanceVec[target];
        float fromLandmark = distFromLandmarkToTarget - distFromLandmarkToSource;

        //handle the case where either source or target is not in the same graph as the landmark
        bool isLowerBFromInfinite =  !isfinite(fromLandmark);
        bool isLowerBToInfinite = !isfinite(toLandmark);
        if (isLowerBFromInfinite){
            if (isLowerBToInfinite){
                //this should only happen if the we have unconnected graphs
            }
            return "toLandmark";
        }
        if (isLowerBToInfinite){
            return "fromLandmark";
        }
        float largestLowerbound = max(toLandmark, fromLandmark);
        if(toLandmark > fromLandmark)
        {
            return "toLandmark";
        }
        else {
            return "fromLandmark";
        }
}

int landmarks::chooseRandomLandmark(adjListCollection &collection) {
    int randomNode = rand() % collection.idSoFar;
    //check if the chosen random initial landmark is connected to more than 50% of the graph
    while(true){
        spResultStruct intialLmk = dijkstra::djikstraShortestPath(randomNode, randomNode, false, collection);
        int nodesConnectedToLmk=0;
        for (int predNode:intialLmk.prevNode) {
            if (predNode!=-1){
                nodesConnectedToLmk++;
            }
        }
        if (nodesConnectedToLmk > collection.idSoFar/2 ){
            break;
        }else{
            cout<< "random landmark " << randomNode << " is bad \n";
            randomNode = rand() % collection.idSoFar;
            cout << "rechosing landmark as " << randomNode <<endl;
        }
    }
    return  randomNode;
}




