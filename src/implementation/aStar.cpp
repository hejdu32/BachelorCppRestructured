//
// Created by simon on 12-04-2021.
//

//#include "../headers/adjacencyList.h"
#include "../headers/aStar.h"
#include <queue>

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


float calcHeuristicDistance(float fdestX, float fdestY, float nodeX, float nodeY, int speed){
    return adjacencyList::distanceCalc(fdestX, fdestY, nodeX, nodeY,speed);
}

spResultStruct aStar::aStarShortestPath(int source, int dest, adjListCollection &adjCol) {
    const float INF = std::numeric_limits<float>::infinity();
    int speed=130;
    int sizeOfGraph = adjCol.idSoFar; //Amount of nodes in graph
    //initialize distance from source to everything to infinity, and source to 0
    vector<float> distance(sizeOfGraph,INF);
    distance[source] = 0;
    //has the node been seen vector
    vector<bool> nodeSeen(sizeOfGraph,false);
    //The pi array containing the path from source to destination
    vector<int> prevNode(sizeOfGraph,-1);
    //heap of nodes to evaluate
    priority_queue<pair<int,float>, vector<pair<int,float>>, comparator> minHeap;
    //save source coordiantes for later use, fdest denotes final destination
    float fdestX = adjacencyList::getxCoord(adjCol, dest);
    float fdestY = adjacencyList::getyCoord(adjCol, dest);

    minHeap.push(make_pair(source,0.0+ calcHeuristicDistance(fdestX, fdestY, adjacencyList::getxCoord(adjCol, source), adjacencyList::getyCoord(adjCol, source),speed)));
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
        for(auto i: connectedNodes){ //Check adjacent nodes
            int node = i.first;
            float weight = i.second;
            float nodeX = adjacencyList::getxCoord(adjCol, node);  float nodeY = adjacencyList::getyCoord(adjCol, node);
            float heuristIntermediate = calcHeuristicDistance(fdestX, fdestY, nodeX, nodeY, speed);
            float newPotentialDistance = distance[headId]+weight;
            //relaxation step, in astar we add the heuristic weight in to consideration
            if(!nodeSeen[node] && newPotentialDistance < distance[node]){
                //update the distance to the node and add it to the queue
                distance[node] = newPotentialDistance;
                prevNode[node] = headId; //Update the previous node in the path
                //add the heuristic to the weight so we sort based on it.
                minHeap.push(make_pair(node, distance[node] + heuristIntermediate));
            }
        }

    }
    spResultStruct result = {distance[dest], distance, prevNode};
    return result;
}










