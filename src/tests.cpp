//
// Created by simon on 13-04-2021.
//
#include "headers/adjacencyList.h"
#include "headers/shortestPath.h"
#include <iostream>
#include <cassert>
#include <chrono>



using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::seconds;

void testDijkstraAdjlist(){
    adjListCollection adjCol;
    shortestPath::createAdjacencyList("C:/Users/a/CLionProjects/BachelorCppRestructured/resources/adjlist","file",adjCol);

    spResultStruct result; vector<long long> idvec;
    int start = adjacencyList::getIntID(adjCol,1);
    int end = adjacencyList::getIntID(adjCol,5);

    result = shortestPath::chooseAlgo(dijkstra,start,end,adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, start, end);
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
    spResultStruct result;
    vector<long long> idvec;

    result = shortestPath::chooseAlgo(dijkstra, aNode, bNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, bNode);
    //testing a->b gives path a,e,b with distance 4
    assert(idvec[0] == aNode);
    assert(idvec[1] == eNode);
    assert(idvec[2] == bNode);
    assert(result.distanceToDest == 4);

    result = shortestPath::chooseAlgo(dijkstra, aNode, eNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, eNode);
    assert(idvec[0] == 0);
    assert(idvec[1] == 4);
    assert(result.distanceToDest == 3);

    result = shortestPath::chooseAlgo(dijkstra, aNode, cNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, cNode);
    assert(idvec[0] == 0);
    assert(idvec[1] == 4);
    assert(idvec[2] == 1);
    assert(idvec[3] == 2);
    assert(result.distanceToDest == 6);

    result = shortestPath::chooseAlgo(dijkstra, aNode, dNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, dNode);
    assert(idvec[0] == 0);
    assert(idvec[1] == 4);
    assert(idvec[2] == 3);
    assert(result.distanceToDest == 5);
    cout << "Toy example Dijkstra test passed" << endl;
}
//dep doesnt work with actual weights
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

    spResultStruct result; vector<long long> idvec;

    result = shortestPath::chooseAlgo(astar,aNode,cNode,adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, cNode);

    assert(idvec[0]==0);
    assert(idvec[1]==4);
    assert(idvec[2]==2);
    assert(result.distanceToDest == 11);
    cout << "Toy Example aStar test passed" << endl;
}
void deserializeJsonFromFile(){
    using namespace nlohmann;
    json j;
    ifstream jsonFile;
    //File path with appropriate json file created by java in graphBuilder writeToFileAsJsonMethod
    jsonFile.open("C:/proj/test.json");
    jsonFile >> j;
    auto wrapperClass = j.get<nodesAndWaysWrapper>();
    //cout << wrapperClass.toString() << endl;
}

void testDistancePrints(string method, long long source, long long target, adjListCollection &adjCol){
    int from = adjacencyList::getIntID(adjCol,source);
    int to = adjacencyList::getIntID(adjCol,target);
    auto t1 = high_resolution_clock::now();
    spResultStruct result = shortestPath::chooseAlgo(spmap[method], from ,to,adjCol);
    auto t2 = high_resolution_clock::now();
    duration<double, milli> ms_double = t2 - t1;
    vector<long long> idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, from, to);
    cout << method <<" from: "<< source <<" to: " << target <<"\n" ;
    cout << "distance: " << result.distanceToDest << " time to find path: "<< ms_double.count()/1000 << "secs"<<endl;
    cout << "nodes in path: "<< idvec.size() << endl;
    int counterVisited = 0;
    for (int id : result.prevNode){
        if (id != -1)
            counterVisited++;
    }
    cout << "Nodes considered: " << counterVisited << endl;
    cout << endl;
}

adjListCollection setUpDatastructure(string country){
    adjListCollection adjCol;
    string malta = "C:/Users/svend/IdeaProjects/BachelorProjectNew/app/malta";
    string denmark = "C:/Users/svend/IdeaProjects/BachelorProjectNew/app/denmark";

    if(country== "malta"){
        //adjCol.adjlst.resize(106762,vector<pair<int,double>>(14));
        cout << "###parsing " << country;
        auto t1 = high_resolution_clock::now();
        shortestPath::createAdjacencyList(malta, "file", adjCol);
        vector<long long> landmarksIDs = {322591088, 259252468, 6158438720, 330038011, 5584771074, 6285925457, 4160003077, 963497183}; //hardcoded landmarks for malta
        vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(landmarksIDs, adjCol);
        adjacencyList::setLandmarkStructs(adjCol, initedLandmarks);
        auto t2 = high_resolution_clock::now();
        duration<double, milli> ms_double = t2 - t1;
        cout << " time: "<< (ms_double.count()/1000) << "seconds###"<<endl;
    }else if(country=="denmark"){
        //adjCol.xCoord.resize(3976155);
        //adjCol.yCoord.resize(3976155);
        //adjCol.adjlst.resize(2500000,vector<pair<int,double>>(15));
        cout << "###parsing " << country;
        auto t1 = high_resolution_clock::now();
        shortestPath::createAdjacencyList(denmark, "file", adjCol);
        vector<long long> landmarksIDs = {313159998, 13131369, 483022215, 349198443, 8633706138, 252151251, 369778546, 6187723341, 2252204014, 3149548115, 118416, 148762835, 1108078449, 6816079197, 2387109140, 371441979}; //hardcoded landmarks for denmark
        vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(landmarksIDs, adjCol);
        adjacencyList::setLandmarkStructs(adjCol, initedLandmarks);
        auto t2 = high_resolution_clock::now();
        duration<double, milli> ms_double = t2 - t1;
        cout << " time: "<< (ms_double.count()/1000) << "seconds###"<<endl;
    }else {
        cout << country << " not found";
    }
    //cout << "finsihed parsing " << country << endl;
    cout << "\n";
    return adjCol;
}
void runMaltaTests(){
    adjListCollection malta = setUpDatastructure("malta");
    //short distance downtown malta
    testDistancePrints("dijkstra",146885871, 1498913919,malta);
    testDistancePrints("astar",146885871, 1498913919,malta);
    testDistancePrints("landmarks",146885871, 1498913919,malta);
    //long distance arcoss malt island
    testDistancePrints("dijkstra",3593516725, 5037683804,malta);
    testDistancePrints("astar",3593516725, 5037683804,malta);
    testDistancePrints("landmarks",3593516725, 5037683804,malta);
}
void runDenmarkTests(){
    adjListCollection denmark = setUpDatastructure("denmark");
    long long itbyen = 957178678;
    long long borglum = 860574684;
    long long RandersHospital = 696724340;
    long long tivoliKobenhavn = 3207955227;

    testDistancePrints("dijkstra",itbyen, borglum,denmark);
    testDistancePrints("astar",itbyen, borglum,denmark);
    testDistancePrints("landmarks",itbyen, borglum,denmark);
    testDistancePrints("dijkstra",itbyen,RandersHospital,denmark);
    testDistancePrints("astar",itbyen,RandersHospital,denmark);
    testDistancePrints("landmarks",itbyen,RandersHospital,denmark);
    testDistancePrints("dijkstra",itbyen,tivoliKobenhavn,denmark);
    testDistancePrints("astar",itbyen,tivoliKobenhavn,denmark);
    testDistancePrints("landmarks",itbyen,tivoliKobenhavn,denmark);
}

void landmarksEmptyListTest(){
    adjListCollection malta = setUpDatastructure("malta");
    vector<long long> landmarksIDs = {322591088, 259252468, 6158438720, 330038011, 5584771074, 6285925457, 4160003077, 963497183}; //hardcoded landmarks for malta
    vector<landmarksStruct> notEmpty = landmarks::initLandmarks(landmarksIDs, malta);
    assert(notEmpty.size()==8);
    landmarksStruct firstElem = notEmpty[0];
    assert(!firstElem.distanceVec.empty());
    assert(!firstElem.prevNode.empty());
    assert(firstElem.nodeID != 0);
}

int main(){
    //TODO FIX SMALL TESTS FOR DIJKSTRA
    //testDijkstraAdjlist();
    //testToyExampleDatastructure();
    //testDijkstraToyExample();
    //landmarksEmptyListTest();
    //runMaltaTests();
    runDenmarkTests();

    //deserializeJsonFromFile();
    return 0;
}