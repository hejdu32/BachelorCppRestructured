//
// Created by simon on 12-04-2021.
//

#include "../headers/aStar.h"
#include "../headers/adjacencyList.h"
#include <vector>
#include <queue>
#include <tuple>

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

vector<int> createList(vector<int> prevNode, int source, int destination){
    int temdest = destination;
    vector<int> shortestPath;
    while (temdest != source && prevNode[temdest] != -1){
        shortestPath.push_back(temdest);
        temdest = prevNode[temdest];
    }
    shortestPath.push_back(source);
    reverse(shortestPath.begin(),shortestPath.end());
    return shortestPath;
}

double calcHeuristicDistance(adjListCollection &adjListCollection, double fdestX, double fdestY, int fromNode){
    double nodeX = adjacencyList::getxCoord(adjListCollection,fromNode);  double nodeY = adjacencyList::getyCoord(adjListCollection,fromNode);
    return  adjacencyList::distanceCalc(fdestX,fdestY,nodeX,nodeY);

}


tuple<double, vector<int>> aStar::aStarShortestPath(int source, int dest, adjListCollection &adjListCollection) {
    const double INF = 999999999999;
    int sizeOfGraph = adjListCollection.intIdToLongID.size();
    //initilaize distance from source to everything to infinity
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
    double fdestX = adjacencyList::getxCoord(adjListCollection,dest);
    double fdestY = adjacencyList::getyCoord(adjListCollection,dest);

    minHeap.push(make_pair(source,0.0+ calcHeuristicDistance(adjListCollection,fdestX,fdestY,source)));

    while (!minHeap.empty()){
        //pop the top element
        pair<int,double> head = minHeap.top();
        minHeap.pop();
        int headId = head.first;
        //Have we reached destination check
        if (head.first==dest){
            //we have arrived at destination and we are done
            //cout << "we have hit destination \n";
            break;
        }
        //add new nodes to queue
        for(auto i: adjListCollection.adjlst[headId]){
            int node = i.first;
            double weight = i.second;

            double heuristicWeight = calcHeuristicDistance(adjListCollection,fdestX,fdestY,node);

            //relaxation step, in astar we add the heuristic weight in to consideration
            if(!nodeSeen[node] && (distance[headId]+weight+heuristicWeight) < distance[node]){
                //update the distance to the node and add it to the queue
                distance[node] = distance[headId]+weight;
                prevNode[node] = headId; //remember the node before for finding the shortest path to destination
                //add the heuristic to the weight so we sort based on it.
                minHeap.push(make_pair(node,distance[node]+heuristicWeight));
            }
        }
        //mark head as it has been seen and cant be considered again
        nodeSeen[headId] = true;
    }
    vector<int> path = createList(prevNode,source,dest);
    return make_tuple(distance[dest],path);
}

