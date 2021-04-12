#include <iostream>
#include "adjacencyList.h"
#include "aStar.h"
#include "shortestPath.h"
#include <cassert>
#include "headerLibs/json.hpp"

using namespace std;
void testDijkstraToyExample() {
    adjListCollection adjCol;
    shortestPath::createAdjacencyList("C:/proj/BachelorCppCmake/resources/dijkstraTest", "file", adjCol);
    //testing that all the nodes in the toy graph has the smallest path
    enum toyExampleVals {
        //translation table to values after putting the
        aNode = 0,
        bNode = 1,
        cNode = 3,
        dNode = 4,
        eNode = 2
    };
    tuple<double, vector<int>> result;
    vector<long long int> idvec;

    result = shortestPath::chooseAlgo(dijkstra, aNode, bNode, adjCol);
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    //testing a->b gives path a,e,b with distance 4
    assert(idvec[0] == 0);
    assert(idvec[1] == 4);
    assert(idvec[2] == 1);
    assert(get<0>(result) == 4);

    result = shortestPath::chooseAlgo(dijkstra, aNode, eNode, adjCol);
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    assert(idvec[0] == 0);
    assert(idvec[1] == 4);
    assert(get<0>(result) == 3);

    result = shortestPath::chooseAlgo(dijkstra, aNode, cNode, adjCol);
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    assert(idvec[0] == 0);
    assert(idvec[1] == 4);
    assert(idvec[2] == 1);
    assert(idvec[3] == 2);
    assert(get<0>(result) == 6);

    result = shortestPath::chooseAlgo(dijkstra, aNode, dNode, adjCol);
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    assert(idvec[0] == 0);
    assert(idvec[1] == 4);
    assert(idvec[2] == 3);
    assert(get<0>(result) == 5);
    cout << "Toy example Dijkstra test passed" << endl;
}
int main() {
    //testDijkstraToyExample();
    nlohmann::json xd;
    xd["Name"] = "Svend";
    xd["Age"] = "23";
    xd["Gender"] = "Unknown";
    cout << xd;
    return 0;
}
