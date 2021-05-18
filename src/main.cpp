#include "headers/adjacencyList.h"
#include "headers/shortestPath.h"
#include <iostream>

using namespace std;

void sendResultToJava(const string& method, const int &from, const int &to, spResultStruct &result, adjListCollection &adjCol){
    vector<long long> idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode,from,to);
    //printing the shortest path
    string listOfNodes = "path " + method +" " + to_string(result.distanceToDest);
    for(long long nodeId: idvec) {
        listOfNodes += " " + to_string(nodeId);
    }
    cout << listOfNodes << endl;
    cout << flush;
    vector<string> nodesConsideredAsStrings;
    for (int i = 0; i < result.prevNode.size(); ++i) {
        if (result.prevNode[i] != -1){
            nodesConsideredAsStrings.emplace_back(to_string(adjacencyList::getLongID(adjCol,i)));
        }
    }

    //printing distance, amount of nodes considered and the chosen landmark if one was used
    cout << "info " + to_string(result.distanceToDest) << " "<< to_string(nodesConsideredAsStrings.size());
    if (method == "landmarks"){
        cout << " " + to_string(result.chosenLandmark);
    }
    cout << endl;
    cout << flush;

    //printing a list of all nodes considered during the ssp problem
    //cout << "size of nodescons: " << nodesConsideredAsStrings.size() << endl;
    int nodesPerLine = 10000;
    if(!nodesConsideredAsStrings.empty()){
        unsigned int timesToLoop = (nodesConsideredAsStrings.size()/nodesPerLine);
        for (int i = 0; i < timesToLoop+1; ++i) {
            int indexStart = i*nodesPerLine;
            int indexEnd = indexStart+nodesPerLine;
            if (indexEnd> nodesConsideredAsStrings.size()){
                indexEnd = (int)nodesConsideredAsStrings.size();
            }
            string listToSend =  "nodesConsidered";

            for (int j = indexStart; j < indexEnd; ++j) {
                listToSend += " " +nodesConsideredAsStrings[j];
            }
            cout<<listToSend<< endl;
            cout << flush;
        }
    }
    cout<< "nodesConsidered end" << endl;
    cout << flush;
}

void runAlgorithm(const string& method, const string &nodeIdFrom, const string &nodeIdTo, adjListCollection &adjCol) {
    int from = adjCol.longIdToIntID[stoll(nodeIdFrom)];
    int to = adjCol.longIdToIntID[stoll(nodeIdTo)];
    spResultStruct result = shortestPath::chooseAlgo(spmap[method], from, to, adjCol);

    sendResultToJava(method,from,to,result,adjCol);

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
                break;
            }
            case makeAdjacencyList: {
                shortestPath::createAdjacencyList(lineAsTokens[1], "file", adjCol);
                vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(12, adjCol);
                for (int i = 0; i <initedLandmarks.size(); ++i) {
                    adjacencyList::setLandmarkStructs(adjCol, initedLandmarks[i]);
                }
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
            default: {
                cout << "Error: did not understand command" << endl;
                cout << flush;
            }
        }
    }
    cout << "OutOfLoop error" << endl;
}



int main() {
    communicateWithJava();
    return 0;
}
