#include "headers/adjacencyList.h"
#include "headers/shortestPath.h"
#include <iostream>



using namespace std;

void sendResultToJava(const string& method, const int &from, const int &to, spResultStruct &result, adjListCollection &adjCol){
    vector<long long> idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode,from,to);

    vector<long long> nodesConsidered;
    for (int i = 0; i < result.prevNode.size(); ++i) {
        if (result.prevNode[i] != -1){
            nodesConsidered.emplace_back(adjacencyList::getLongID(adjCol,i));
        }
    }

    ofstream resultFile;
    resultFile.open ("result");
    if (!resultFile.is_open()){
        cout << "error opening file\n";
        cout << flush;
    }

    string firstLine ="info "+ method + " " + to_string(result.distanceToDest) + " "+ to_string(nodesConsidered.size());
    if (method == "landmarks"){
        firstLine = firstLine + " " + to_string(result.chosenLandmark);
    }
    resultFile << firstLine << "\n";

    string path = "path";
    for (long long id:idvec) {
        path += " " + to_string(id);
    }
    resultFile << path << "\n";

    string nodesCons = "nodesConsid";
    for (long long idNodeConsid: nodesConsidered) {
        nodesCons += " " + to_string(idNodeConsid);
    }
    resultFile << nodesCons << "\n";
    resultFile.close();
    cout<< "resInFile\n";
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
        Error,
        makeAdjacencyList,
        runDijkstra,
        runAstar,
        runALT,
    };
    map<string, commands> mapStringToEnum =
            {
                    {"error", Error},
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
            case Error: {
                cout << "Could not understand input: "<< line << endl;
                cout << flush;
                break;
            }
            case makeAdjacencyList: {
                cout << "Parsing file"<<endl;
                cout << flush;
                shortestPath::createAdjacencyList(lineAsTokens[1], "file", adjCol);
                vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(12, adjCol, std::string());
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
            default:{
                cout << "could not understand input: " << line << flush;
                break;
            }
        }
    }
    cout << "OutOfLoop error" << endl;
}



int main() {
    communicateWithJava();
    return 0;
}
