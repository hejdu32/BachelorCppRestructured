#include "headers/adjacencyList.h"
//#include "headers/aStar.h"
#include "headers/shortestPath.h"
//#include "headers/util.h"
//#include "headerLibs/json.hpp"
#include <iostream>
//#include <cassert>

using namespace std;

void runAlgorithm(const string& method, const string &nodeIdFrom, const string &nodeIdTo, adjListCollection &adjCol) {
    int from = adjCol.longIdToIntID[stoll(nodeIdFrom)];
    int to = adjCol.longIdToIntID[stoll(nodeIdTo)];
    spResultStruct result = shortestPath::chooseAlgo(spmap[method], from, to, adjCol);
    vector<long long> idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode,from,to);
    string listOfNodes = "path " + method +" " + to_string(result.distanceToDest);
    for(long long nodeId: idvec) {
        listOfNodes += " " + to_string(nodeId);
    }
    cout << listOfNodes << endl;
    cout << flush;
}


void communicateWithJava() {
    enum commands{
        ERROR,
        makeAdjacencyList,
        runDijkstra,
        runAstar,
        runALT,
    };
    map<string, commands> mapStringToEnum =
            {
                    {"error", ERROR},
                    {"makeAdjacencyList", makeAdjacencyList},
                    {"runDijkstra", runDijkstra},
                    {"runAstar", runAstar},
                    {"runALT", runALT}
            };
    string line;
    adjListCollection adjCol;
    while(getline(cin, line)) {
        istringstream buf(line);
        istream_iterator<string> beg(buf), end;
        vector<string> lineAsTokens(beg, end);
        commands switchType = mapStringToEnum[lineAsTokens[0]]; // lineAsTokens[0]
        switch (switchType) {
            case ERROR: {
                cout << "ERROR HAPPENED" << "\n";
                cout << flush;
            }
            case makeAdjacencyList: {
                shortestPath::createAdjacencyList("/home/a/IdeaProjects/BachelorProject/app/malta", "file", adjCol);
                vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(8, adjCol);
                for (int i = 0; i <initedLandmarks.size(); ++i) {
                    adjacencyList::setLandmarkStructs(adjCol, initedLandmarks[i]);
                }
                //nodesAndWaysWrapper wrapper = adjacencyList::deserializeFromJson("C:/proj/BachelorCppCmake/resources/malta.json");
                //adjacencyList::createAdjListCollection(wrapper, adjCol);
                cout << "Finished" << endl;
                cout << flush;
                break;
            }
            case runDijkstra: {
                string nodeIdFrom = lineAsTokens[1];
                string nodeIdTo= lineAsTokens[2];
                runAlgorithm("dijkstra", nodeIdFrom, nodeIdTo, adjCol);
                break;
            }
            case runAstar: {
                string nodeIdFrom = lineAsTokens[1];
                string nodeIdTo= lineAsTokens[2];
                runAlgorithm("astar", nodeIdFrom, nodeIdTo, adjCol);
                break;
            }
            case runALT: {
                string nodeIdFrom = lineAsTokens[1];
                string nodeIdTo= lineAsTokens[2];
                runAlgorithm("landmarks", nodeIdFrom, nodeIdTo, adjCol);
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
