#include "headers/adjacencyList.h"
#include "headers/aStar.h"
#include "headers/shortestPath.h"
#include "headerLibs/json.hpp"
#include <iostream>
#include <cassert>

using namespace std;

void communicateWithJava() {
    enum commands{
        makeAdjacencyList,
        runDijkstra
    };
    map<string, commands> mapStringToEnum =
            {
                    {"makeAdjacencyList", makeAdjacencyList},
                    {"runDijkstra", runDijkstra}
            };
    string line;
    adjListCollection adjCol;
    while(getline(cin, line)) {
        commands switchType = mapStringToEnum[line];
        switch (switchType) {
            case makeAdjacencyList: {
                //shortestPath::createAdjacencyList("", "java", adjCol);
                nodesAndWaysWrapper wrapper = adjacencyList::deserializeFromJson("C:/proj/BachelorCppCmake/resources/malta.json");
                adjacencyList::createAdjListCollection(wrapper, adjCol);
                cout << "Finished making adjacency list" << endl;
                break;
            }
            case runDijkstra: {
                string nodeIdFrom;
                string nodeIdTo;
                cin >> nodeIdFrom;
                cin >> nodeIdTo;
                int from = adjCol.longIdToIntID[stoll(nodeIdFrom)];
                int to = adjCol.longIdToIntID[stoll(nodeIdTo)];
                spResultStruct result = shortestPath::chooseAlgo(dijkstra, from, to, adjCol);
                vector<long long> idvec = adjacencyList::spVectorToLongId(adjCol, result.prevNode);
                string listOfNodes = "NodeIds";
                for(long long nodeId: idvec) {
                    listOfNodes += " " + to_string(nodeId);
                }
                double distance = result.distanceToDest;
                cout << listOfNodes << endl;
                break;
            }
        }
    }
    cout << "OutOfLoop error" << endl;
    //BachelorCpp::createAdjList listMaker;
    //adjListCollection adjCol;
    //listMaker.createList("", "java", adjCol);
}

int main() {
    communicateWithJava();
    return 0;
}
