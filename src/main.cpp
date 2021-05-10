#include "headers/adjacencyList.h"
#include "headers/aStar.h"
#include "headers/shortestPath.h"
#include "headers/util.h"
#include "headerLibs/json.hpp"
#include <iostream>
#include <cassert>

using namespace std;

void communicateWithJava() {
    enum commands{
        makeAdjacencyList,
        runDijkstra,
        runAstar,
        runALT
    };
    map<string, commands> mapStringToEnum =
            {
                    {"makeAdjacencyList", makeAdjacencyList},
                    {"runDijkstra", runDijkstra},
                    {"runAstar", runAstar},
                    {"runALT", runALT}
            };
    string line;
    adjListCollection adjCol;
    while(getline(cin, line)) {
        commands switchType = mapStringToEnum[line];
        switch (switchType) {
            case makeAdjacencyList: {
                shortestPath::createAdjacencyList("C:/Users/svend/IdeaProjects/BachelorProjectNew/app/malta", "file", adjCol);
                vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(12, adjCol);
                for (int i = 0; i <initedLandmarks.size(); ++i) {
                    adjacencyList::setLandmarkStructs(adjCol, initedLandmarks[i]);
                }
                //nodesAndWaysWrapper wrapper = adjacencyList::deserializeFromJson("C:/proj/BachelorCppCmake/resources/malta.json");
                //adjacencyList::createAdjListCollection(wrapper, adjCol);
                cout << "Finished" << endl;
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
                vector<long long> idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode,from,to);
                string listOfNodes = "NodeIds";
                for(long long nodeId: idvec) {
                    listOfNodes += " " + to_string(nodeId);
                }
                double distance = result.distanceToDest;
                cout << listOfNodes << endl;
                break;
            }
            case runAstar: {
                string nodeIdFrom;
                string nodeIdTo;
                cin >> nodeIdFrom;
                cin >> nodeIdTo;
                int from = adjCol.longIdToIntID[stoll(nodeIdFrom)];
                int to = adjCol.longIdToIntID[stoll(nodeIdTo)];
                spResultStruct result = shortestPath::chooseAlgo(astar, from, to, adjCol);
                vector<long long> idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode,from,to);
                string listOfNodes = "NodeIds";
                for(long long nodeId: idvec) {
                    listOfNodes += " " + to_string(nodeId);
                }
                double distance = result.distanceToDest;
                cout << listOfNodes << endl;
                break;
            }
            case runALT: {
                string nodeIdFrom;
                string nodeIdTo;
                cin >> nodeIdFrom;
                cin >> nodeIdTo;
                int from = adjCol.longIdToIntID[stoll(nodeIdFrom)];
                int to = adjCol.longIdToIntID[stoll(nodeIdTo)];
                spResultStruct result = shortestPath::chooseAlgo(alt, from, to, adjCol);
                vector<long long> idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode,from,to);
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
