//
// Created by a on 31-03-2021.
//

#include "../headers/dijkstra.h"
#include <queue>
#include <vector>

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


spResultStruct dijkstra::djikstraShortestPath(int source, int dest, bool earlyStopping, adjListCollection &adjCol) {
    const double INF = std::numeric_limits<double>::infinity();
    int sizeOfGraph = adjCol.idSoFar; //Amount of nodes in graph
    //initialize distance from source to everything to infinity, and source to 0
    vector<double> distance(sizeOfGraph,INF);
    distance[source] = 0;
    //has the node been seen vector,
    vector<bool> nodeSeen(sizeOfGraph,false);
    //The pi array containing the path from source to destination
    vector<int> prevNode(sizeOfGraph,-1);
    //heap of nodes to evaluate
    priority_queue<pair<int,double>, vector<pair<int,double>>, comparator> minHeap;

    minHeap.push(make_pair(source,0.0));
    while (!minHeap.empty()){
        //pop the top element
        pair<int,double> head = minHeap.top();
        minHeap.pop();
        int headId = head.first;

        if (earlyStopping && headId==dest){ //Early stopping check
            break;
        }

        nodeSeen[headId] = true; //mark head as considered
        auto connectedNodes = adjCol.adjlst[headId];
        for(auto i: connectedNodes){ //Check adjacent nodes
            int node = i.first;
            double weight = i.second;
            double newDistance = distance[headId]+weight;
            double oldDistnace = distance[node];
            //relaxation step
            if(!nodeSeen[node] && newDistance < oldDistnace){
                //update the distance to the node and add it to the queue
                distance[node] = newDistance;
                prevNode[node] = headId; //remember the node before for finding the shortest path to destination
                minHeap.push(make_pair(node,distance[node]));
            }
        }
    }
   spResultStruct result={distance[dest], distance, prevNode};

    return result;
}