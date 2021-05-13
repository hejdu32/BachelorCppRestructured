//
// Created by simon on 12-04-2021.
//

//#include "../headers/adjacencyList.h"
#include "../headers/aStar.h"
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

int nodesConsidered(vector<bool> &nodesSeen){
    int nodes = 0;
    for (auto b:nodesSeen) {
        if(b){nodes++;}
    }
    return nodes;
}



double calcHeuristicDistance(double fdestX, double fdestY, double nodeX, double nodeY, int speed){
    return adjacencyList::euclidDistance(fdestX, fdestY, nodeX, nodeY,speed);
}


spResultStruct aStar::aStarShortestPath(int source, int dest, adjListCollection &adjCol) {
    const double INF = std::numeric_limits<double>::infinity();
    int sizeOfGraph = adjCol.idSoFar;
    int speed=130;
    //initialize distance from source to everything to infinity
    //distance from source to source to 0
    vector<double> distance(sizeOfGraph,INF);
    distance[source] = 0;
    //has the node been seen vector
    vector<bool> nodeSeen(sizeOfGraph,false);
    nodeSeen[source] = true;
    //path from source to destination
    vector<int> prevNode(sizeOfGraph,-1);
    //prevNode[source] = -1;
    //heap of nodes to evaluate
    priority_queue<pair<int,double>, vector<pair<int,double>>, comparator> minHeap;

    double fdestX = adjacencyList::getxCoord(adjCol, dest);
    double fdestY = adjacencyList::getyCoord(adjCol, dest);


    minHeap.push(make_pair(source,0.0+ calcHeuristicDistance(fdestX, fdestY, adjacencyList::getxCoord(adjCol, source), adjacencyList::getyCoord(adjCol, source),speed)));
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
            double nodeX = adjacencyList::getxCoord(adjCol, node);  double nodeY = adjacencyList::getyCoord(adjCol, node);

            double heuristIntermediate = calcHeuristicDistance(fdestX, fdestY, nodeX, nodeY, speed);
            //relaxation step, in astar we add the heuristic weight in to consideration
            double newPotentialDistance = distance[headId]+weight;
            if(!nodeSeen[node] && newPotentialDistance < distance[node]){
                //update the distance to the node and add it to the queue
                distance[node] = newPotentialDistance;
                prevNode[node] = headId; //remember the node before for finding the shortest path to destination
                //add the heuristic to the weight so we sort based on it.
                double queueVal = distance[node] + heuristIntermediate;
                if (queueVal <= 0.0){
                    cout << "negVal: " << queueVal << "\n";
                    cout << "nodeA: " << adjCol.intIdToLongID.find(headId)->second << " nodeB: " <<  adjCol.intIdToLongID.find(node)->second << endl;
                }
                minHeap.push(make_pair(node, distance[node] + heuristIntermediate));
            }
        }
        //mark head as it has been seen and cant be considered again
        nodeSeen[headId] = true;
    }
    //cout << "astar nodes considered: " << nodesConsidered(nodeSeen) << endl;
    spResultStruct result={distance[dest], distance, prevNode};

    return result;
}










