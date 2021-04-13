#include "headers/adjacencyList.h"
#include "headers/aStar.h"
#include "headers/shortestPath.h"
#include "headerLibs/json.hpp"
#include <iostream>
#include <cassert>

using namespace std;

void communicateWithJava() {
    enum commands{
        makeAdjecencyList,
        runDijkstra
    };
    map<string, commands> mapStringToEnum =
            {
                    {"makeAdjecencyList", makeAdjecencyList},
                    {"runDijkstra", runDijkstra}
            };
    string line;
    adjListCollection adjCol;
    while(getline(cin, line)) {
        commands switchType = mapStringToEnum[line];
        switch (switchType) {
            case makeAdjecencyList: {
                shortestPath::createAdjacencyList("", "java", adjCol);
                break;
            }
            case runDijkstra: {
                string nodeIdFrom;
                string nodeIdTo;
                cin >> nodeIdFrom;
                cin >> nodeIdTo;
                int from = adjCol.longIdToIntID[stoll(nodeIdFrom)];
                int to = adjCol.longIdToIntID[stoll(nodeIdTo)];
                tuple<double,vector<int>> result = shortestPath::chooseAlgo(dijkstra, from, to, adjCol);
                vector<long long int> idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
                string listOfNodes = "NodeIds";
                for(long long int nodeId: idvec) {
                    listOfNodes += " " + to_string(nodeId);
                }
                double distance = get<0>(result);
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
