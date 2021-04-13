//
// Created by simon on 13-04-2021.
//
#include "headers/adjacencyList.h"
#include "headers/aStar.h"
#include "headers/shortestPath.h"
#include "headerLibs/json.hpp"
#include "implementation/nodesAndWaysWrapper.cpp"
#include <iostream>
#include <cassert>
#include <chrono>

using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

void testDijkstraAdjlist(){
    adjListCollection adjCol;
    shortestPath::createAdjacencyList("C:/Users/a/CLionProjects/BachelorCppRestructured/resources/adjlist","file",adjCol);

    tuple<double,vector<int>> result; vector<long long int> idvec;
    int start = adjacencyList::getIntID(adjCol,1);
    int end = adjacencyList::getIntID(adjCol,5);

    result = shortestPath::chooseAlgo(dijkstra,start,end,adjCol);
    idvec = adjacencyList::spVectorToLongId(adjCol,get<1>(result));
    adjacencyList::printGraph(adjCol);

    assert(idvec[0]==1);
    assert(idvec[1]==2);
    assert(idvec[2]==4);
    assert(idvec[3]==5);
    //shortestPath::printVec(idvec);
    cout << "adjlist Dijkstra test passed" << endl;
}

void createToyExample(adjListCollection &collection) {
    //emulating reading from file from corresponding adjlist
    int aNode = adjacencyList::insertInMaps(collection,0);
    int bNode = adjacencyList::insertInMaps(collection,1);
    int cNode = adjacencyList::insertInMaps(collection,2);
    int dNode = adjacencyList::insertInMaps(collection,3);
    int eNode = adjacencyList::insertInMaps(collection,4);
    adjacencyList::addEdge(collection,aNode,bNode,10);
    adjacencyList::addEdge(collection,aNode,eNode,3);
    adjacencyList::addEdge(collection,bNode,cNode,2);
    adjacencyList::addEdge(collection,bNode,eNode,4);
    adjacencyList::addEdge(collection,cNode,dNode,9);
    adjacencyList::addEdge(collection,dNode,cNode,7);
    adjacencyList::addEdge(collection,eNode,bNode,1);
    adjacencyList::addEdge(collection,eNode,cNode,8);
    adjacencyList::addEdge(collection,eNode,dNode,2);
    //adjacencyList::addHeuristicDist(collection,aNode,5.0);
    //adjacencyList::addHeuristicDist(collection,bNode,9.0);
    //adjacencyList::addHeuristicDist(collection,cNode,0);
    //adjacencyList::addHeuristicDist(collection,dNode,3);
    //adjacencyList::addHeuristicDist(collection,eNode,4);
}

void testToyExampleDatastructure(){
    adjListCollection adjCol;
    //shortestPath::createAdjacencyList("C:/Users/a/CLionProjects/BachelorCppRestructured/resources/dijkstraTest", "file", adjCol);
    //testing that all the nodes in the toy graph has the smallest path
    createToyExample(adjCol);
    enum toyExampleVals {
        //translation table to values after putting the
        aNode = 0,
        bNode = 1,
        cNode = 2,
        dNode = 3,
        eNode = 4
    };
    assert(adjacencyList::getIntID(adjCol,aNode) == 0);
    assert(adjacencyList::getIntID(adjCol,bNode) == 1);
    assert(adjacencyList::getIntID(adjCol,cNode) == 2);
    assert(adjacencyList::getIntID(adjCol,dNode) == 3);
    assert(adjacencyList::getIntID(adjCol,eNode) == 4);
    //adjacencyList::printGraph(adjCol);
    cout<<"test toy example fixture passed" << endl;
}

void testDijkstraToyExample() {
    adjListCollection adjCol;
    //shortestPath::createAdjacencyList("C:/Users/a/CLionProjects/BachelorCppRestructured/resources/dijkstraTest", "file", adjCol);
    //testing that all the nodes in the toy graph has the smallest path
    createToyExample(adjCol);
    enum toyExampleVals {
        //translation table to values after putting the
        aNode = 0,
        bNode = 1,
        cNode = 2,
        dNode = 3,
        eNode = 4
    };
    tuple<double, vector<int>> result;
    vector<long long> idvec;

    result = shortestPath::chooseAlgo(dijkstra, aNode, bNode, adjCol);
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    //testing a->b gives path a,e,b with distance 4
    assert(idvec[0] == aNode);
    assert(idvec[1] == eNode);
    assert(idvec[2] == bNode);
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
    createToyExample(adjCol);
    //testing that all the nodes in the toy graph has the smallest path
    enum toyExampleVals{
        //translation table to values after putting the
        aNode = 0,
        bNode = 1,
        cNode = 2,
        dNode = 3,
        eNode = 4
    };
    //test that the euclidDist has been written correct
    //assert(adjCol.heuristicDistance.find(aNode)->second == 5.0);
    //assert(adjCol.heuristicDistance.find(bNode)->second == 9.0);
    //assert(adjCol.heuristicDistance.find(cNode)->second == 0.0);
    //assert(adjCol.heuristicDistance.find(dNode)->second == 3.0);
    //assert(adjCol.heuristicDistance.find(eNode)->second == 4.0);

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
    string maltaNew = "C:/Users/a/IdeaProjects/BachelorProject/app/malta";
    shortestPath::createAdjacencyList(maltaNew, "file", adjCol);

    //short area in malta
    int from = adjacencyList::getIntID(adjCol,146885871);
    int to = adjacencyList::getIntID(adjCol,1498913919);

    tuple<double,vector<int>> result; vector<long long int> idvec;
    auto t1 = high_resolution_clock::now();
    result = shortestPath::chooseAlgo(dijkstra, from ,to,adjCol);
    auto t2 = high_resolution_clock::now();
    duration<double, milli> ms_double = t2 - t1;
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    cout << "dijkstra from: 146885871, to: 1498913919 (short distance malta) \n" ;
    cout << "distance: " << get<0>(result) << "\n";
    cout << "time to find path: "<< ms_double.count() << "ms \n";
    cout << "path: ";
    shortestPath::printVec(idvec);
    cout << "\n";
}
void testAStarMaltaSmall(){
    adjListCollection adjCol;
    string maltaNew = "C:/Users/a/IdeaProjects/BachelorProject/app/malta";
    shortestPath::createAdjacencyList(maltaNew, "file", adjCol);

    //short area in malta
    int from = adjacencyList::getIntID(adjCol,146885871);
    int to = adjacencyList::getIntID(adjCol,1498913919);

    tuple<double,vector<int>> result; vector<long long int> idvec;
    auto t1 = high_resolution_clock::now();
    result = shortestPath::chooseAlgo(astar, from ,to,adjCol);
    auto t2 = high_resolution_clock::now();
    duration<double, milli> ms_double = t2 - t1;
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    cout << "astar from: 146885871, to: 1498913919 (short distance malta) \n" ;
    cout << "distance: " << get<0>(result) << "\n";
    cout << "time to find path: "<< ms_double.count() << "ms \n";
    cout << "path: ";
    shortestPath::printVec(idvec);
    cout << "\n";
}

void testDijkstraMaltaLarge(){
    adjListCollection adjCol;
    string maltaNew = "C:/Users/a/IdeaProjects/BachelorProject/app/malta";
    shortestPath::createAdjacencyList(maltaNew, "file", adjCol);

    //long area in malta
    int maltaNorth = adjacencyList::getIntID(adjCol,3593516725);
    int maltaSouth = adjacencyList::getIntID(adjCol,5037683804);

    tuple<double,vector<int>> result; vector<long long int> idvec;
    auto t1 = high_resolution_clock::now();
    result = shortestPath::chooseAlgo(dijkstra, maltaNorth, maltaSouth,adjCol);
    auto t2 = high_resolution_clock::now();
    duration<double, milli> ms_double = t2 - t1;
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    cout << "dijkstra from: 3593516725, to: 5037683804 (long distance malta) \n" ;
    cout << "distance: " << get<0>(result) << "\n";
    cout << "time to find path: "<< ms_double.count() << "ms \n";
    //cout << "path: ";
    //shortestPath::printVec(idvec);
    //cout << "\n";
}

void testAStarMaltaLarge(){
    adjListCollection adjCol;
    string maltaNew = "C:/Users/a/IdeaProjects/BachelorProject/app/malta";
    shortestPath::createAdjacencyList(maltaNew, "file", adjCol);

    //long area in malta
    int maltaNorth = adjacencyList::getIntID(adjCol,3593516725);
    int maltaSouth = adjacencyList::getIntID(adjCol,5037683804);

    tuple<double,vector<int>> result; vector<long long int> idvec;
    auto t1 = high_resolution_clock::now();
    result = shortestPath::chooseAlgo(astar, maltaNorth, maltaSouth,adjCol);
    auto t2 = high_resolution_clock::now();
    duration<double, milli> ms_double = t2 - t1;
    idvec = adjacencyList::spVectorToLongId(adjCol, get<1>(result));
    cout << "astar from: 3593516725, to: 5037683804 (long distance malta) \n" ;
    cout << "distance: " << get<0>(result) << "\n";
    cout << "time to find path: "<< ms_double.count() << "ms \n";
    //cout << "path: ";
    //shortestPath::printVec(idvec);
    //cout << "\n";
}


void deserializeJsonFromFile(){
    using namespace nlohmann;
    json j;
    ifstream jsonFile;
    //File path with appropriate json file created by java in graphBuilder writeToFileAsJsonMethod
    jsonFile.open("C:/proj/test.json");
    jsonFile >> j;
    auto wrapperClass = j.get<nodesAndWaysWrapper>();
    cout << wrapperClass.toString() << endl;
    }



int main(){

    //_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    //_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    //_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_WNDW );
    //_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );

    //testDijkstraAdjlist();
    //testToyExampleDatastructure();
    //testDijkstraToyExample();
    //testAStarToyExample();
    testDijkstraMaltaLarge();
    testAStarMaltaLarge();
    testDijkstraMaltaSmall();
    testAStarMaltaSmall();

    //deserializeJsonFromFile();
    return 0;
}