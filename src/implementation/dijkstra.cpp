//
// Created by a on 31-03-2021.
//

#include "../headers/dijkstra.h"
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

//this comparator ensures that the top most element of a queue has the lowest weight
struct comparator{
    constexpr bool operator()(
            pair<int, double> const& a,
            pair<int, double> const& b)
    const noexcept
    {
        return a.second > b.second;
    }
};

void toyExampleConverter(vector<int> const &prevPath, int source, int dest){
    if (dest < 0 ||dest == source){
        if(source == 0){cout << "a ";}
        else{cout <<  source << " ";}
        return;
    }
    toyExampleConverter(prevPath, source, prevPath[dest]);
    if (dest ==1){cout << "b ";
    }else if(dest == 2){cout << "e ";}
    else if(dest == 4){cout << "d ";}
    else if(dest == 3){cout << "c ";}
    else{cout << "??????";}
    //cout << dest << " ";
}

void printRoute(vector<int> const &prevPath, int source, int dest){
    if (dest < 0 ||dest == source){
       cout <<  source << " ";
        return;
    }
    printRoute(prevPath, source, prevPath[dest]);
    cout << dest << " ";
}


vector<int> createSPList(vector<int> prevNode, int source, int destination){
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

int nodesConsidered2(vector<bool> nodesSeen){
    int nodes = 0;
    for (auto b:nodesSeen) {
        if(b){nodes++;}
    }
    return nodes;
}

spResultStruct dijkstra::djikstraShortestPath(int source, int dest, bool earlyStopping, adjListCollection &adjCol) {
    const double INF = 999999999999;
    int sizeOfGraph = adjCol.idSoFar;
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

    minHeap.push(make_pair(source,0.0));
    while (!minHeap.empty()){
        //pop the top element
        pair<int,double> head = minHeap.top();
        minHeap.pop();
        int headId = head.first;

        //Have we reached destination check
        if (earlyStopping && headId==dest){
            //we have arrived at destination and we are done
            //cout << "we have hit destination \n";
            break;
        }

        //add new nodes to queue
        for(auto i: adjCol.adjlst[headId]){
            int node = i.first;
            double weight = i.second;

            //relaxation step
            if(!nodeSeen[node] && (distance[headId]+weight) < distance[node]){
                //update the distance to the node and add it to the queue
                distance[node] = distance[headId]+weight;
                prevNode[node] = headId; //remember the node before for finding the shortest path to destination
                minHeap.push(make_pair(node,distance[node]));
            }
        }
        //mark head as it has been seen and cant be considered again
        nodeSeen[headId] = true;
    }
    //vector<int> path = createSPList(prevNode,source,dest);
    spResultStruct resultStruct;
    resultStruct.distanceToDest = distance[dest];
    resultStruct.distanceVec = distance;
    resultStruct.prevNode = prevNode;

    return resultStruct;
}