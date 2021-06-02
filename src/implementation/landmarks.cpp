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
        landmarksStruct.nodeID = id;    //is suppose to be id not intID
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

    int randomNode = rand() % highestNbr;
    const float INF = std::numeric_limits<float>::infinity();
    //cout << "https://www.openstreetmap.org/node/" << adjListCollection.intIdToLongID[randomNode] << endl;
    vector<vector<float>> markDistanceVectors;
    for (int i = 0; i < amount+1; ++i) {

        if(i!=0){cout << "https://www.openstreetmap.org/node/" << adjListCollection.intIdToLongID[randomNode] << endl;}
        landmarksStruct landmarksStruct;
        spResultStruct distanceToEverything = dijkstra::djikstraShortestPath(randomNode, randomNode, false, adjListCollection);
        spResultStruct distanceFromEverything = dijkstra::djikstraShortestPath(randomNode, randomNode, false, reversedAdjListCollection);
        landmarksStruct.distanceVec = distanceToEverything.distanceVec;
        landmarksStruct.reversedDistanceVec = distanceFromEverything.distanceVec;
        landmarksStruct.nodeID = adjListCollection.intIdToLongID[randomNode];    //is suppose to be id not intID
        //markDistanceVectors.emplace_back((landmarksStruct.distanceVec));

        //cout << "https://www.openstreetmap.org/node/" << adjListCollection.intIdToLongID[randomNode] << "#map=8/56.216/12.816"<< endl;
        if(landmarkSelection == "euclidDistance"){
            vector<float> euclidDistVector;
            euclidDistVector.resize(highestNbr,0);
            float sourceX = adjListCollection.xCoord[randomNode];
            float sourceY = adjListCollection.yCoord[randomNode];
            for (int j = 0; j < highestNbr; ++j) {
                //if(j % 1000 == 0)
                //    cout << "euclidloop: " << j << endl;
                float targetX = adjListCollection.xCoord[j];
                float targetY = adjListCollection.yCoord[j];
                float euclidDistToJ = adjacencyList::euclidDistance(sourceX, sourceY, targetX, targetY);
                euclidDistVector[j]=euclidDistToJ;
            }
            markDistanceVectors.emplace_back(euclidDistVector);
        }
        resultVector.emplace_back(landmarksStruct);


        float longestDistToClosestMark = 0.0;
        for (int j = 0; j <= highestNbr; ++j) { //loops over all nodes
            float closestMark = INF;
            //for (vector<double> distVec : markDistanceVectors){
            for (int k = 0; k < resultVector.size(); ++k) {
                float distToCandidate =0;
                float distToCandidateDijkstra;
                if(landmarkSelection == "euclidDistance"){
                    distToCandidate = markDistanceVectors[k][j];
                    distToCandidateDijkstra = resultVector[k].distanceVec[j];
                }
                else if(landmarkSelection == "dijkstraDistance"){
                    distToCandidate = resultVector[k].distanceVec[j];
                }
                else {
                    cout << "Unknown landmarkSelection provided: " << landmarkSelection << endl;
                }
                //double distToCandidate = distVec[j];
                if (distToCandidate != INF && distToCandidateDijkstra != INF && distToCandidate < closestMark)
                    closestMark = distToCandidate;
            }
            if (closestMark != INF && closestMark > longestDistToClosestMark) {
                longestDistToClosestMark = closestMark;
                randomNode = j;
            }
        }
        //if(i==0) markDistanceVectors.pop_back();
    }
    resultVector.erase(resultVector.begin());
    return resultVector;
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
    spResultStruct result = {distance[dest], distance, prevNode, landmark.nodeID};
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




