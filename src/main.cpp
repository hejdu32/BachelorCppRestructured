#include <iostream>
#include "headers/adjacencyList.h"
#include "headers/aStar.h"
#include "headers/shortestPath.h"
#include <cassert>
#include "headerLibs/json.hpp"

using namespace std;

void testDijkstraAdjlist(){
    adjListCollection adjCol;
    shortestPath::createAdjacencyList("C:/Users/a/CLionProjects/BachelorCppRestructured/resources/adjlist","file",adjCol);

    tuple<double,vector<int>> result; vector<long long int> idvec;
    int start = adjacencyList::getIntID(adjCol,1);
    int end = adjacencyList::getIntID(adjCol,5);

    result = shortestPath::chooseAlgo(dijkstra,start,end,adjCol);
    idvec = adjacencyList::spVectorToLongId(adjCol,get<1>(result));
    assert(idvec[0]==1);
    assert(idvec[1]==2);
    assert(idvec[2]==4);
    assert(idvec[3]==5);
    //shortestPath::printVec(idvec);
    cout << "adjlist Dijkstra test passed" << endl;
}

void testDijkstraToyExample() {
    adjListCollection adjCol;
    shortestPath::createAdjacencyList("C:/Users/a/CLionProjects/BachelorCppRestructured/resources/dijkstraTest", "file", adjCol);
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

void testAStarToyExample(){
    adjListCollection adjCol;
    shortestPath::createAdjacencyList("C:/Users/a/CLionProjects/BachelorCpp/app/src/resources/aStarTest","file",adjCol);
    //testing that all the nodes in the toy graph has the smallest path
    enum toyExampleVals{
        //translation table to values after putting the
        aNode = 0,
        bNode = 1,
        cNode = 3,
        dNode = 4,
        eNode = 2
    };
    //test that the euclidDist has been written correct
    assert(adjCol.heuristicDistance.find(aNode)->second == 5.0);
    assert(adjCol.heuristicDistance.find(bNode)->second == 9.0);
    assert(adjCol.heuristicDistance.find(cNode)->second == 0.0);
    assert(adjCol.heuristicDistance.find(dNode)->second == 3.0);
    assert(adjCol.heuristicDistance.find(eNode)->second == 4.0);

    tuple<double,vector<int>> result; vector<long long int> idvec;

    result = shortestPath::chooseAlgo(astar,aNode,cNode,adjCol);
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));

    assert(idvec[0]==0);
    assert(idvec[1]==4);
    assert(idvec[2]==2);
    assert(get<0>(result) == 11);

    //shortestPath.printVec(idvec);
    cout << "Toy Example aStar test passed" << endl;
}

void testDijkstraMaltaSmall(){
    adjListCollection adjCol;
    shortestPath::createAdjacencyList("C:/Users/a/CLionProjects/BachelorCppRestructured/resources/malta", "file", adjCol);

    //short area in malta
    int from = adjacencyList::getIntID(adjCol,146885871);
    int to = adjacencyList::getIntID(adjCol,1498913919);

    tuple<double,vector<int>> result; vector<long long int> idvec;

    result = shortestPath::chooseAlgo(dijkstra, from ,to,adjCol);
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    cout << "testing short distance in malta \n" ;
    cout << "from node: 146885871, to node: 1498913919 \n";
    cout << "distance: " << get<0>(result) << "\n";
    cout << "path: ";
    shortestPath::printVec(idvec);
    cout << "\n";
}

void testDijkstraMaltaLarge(){
    adjListCollection adjCol;
    shortestPath::createAdjacencyList("C:/Users/a/CLionProjects/BachelorCppRestructured/resources/malta", "file", adjCol);

    //long area in malta
    int maltaNorth = adjacencyList::getIntID(adjCol,3593516725);
    int maltaSouth = adjacencyList::getIntID(adjCol,5037683804);

    tuple<double,vector<int>> result; vector<long long int> idvec;
    result = shortestPath::chooseAlgo(dijkstra,maltaNorth,maltaSouth,adjCol);
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    cout << "testing long distance in malta \n" ;
    cout << "from node: 3593516725, to node: 5037683804 \n";
    cout << "distance: " << get<0>(result) << "\n";
    cout << "path: ";
    shortestPath::printVec(idvec);
    cout << "\n";
}




int main() {
    //testDijkstraToyExample();
    //testDijkstraMaltaSmall();
    //testDijkstraMaltaLarge();
    //testDijkstraAdjlist();
    //testAStarToyExample();
    return 0;
}
