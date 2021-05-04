//
// Created by simon on 13-04-2021.
//
#include "headers/adjacencyList.h"
#include "headers/shortestPath.h"
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <chrono>



using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::seconds;

void testAdjlistSimpleDijkstra(){
    adjListCollection adjCol;
    shortestPath::createAdjacencyList("C:/Users/a/CLionProjects/BachelorCppRestructured/resources/adjlist","file",adjCol);

    spResultStruct result; vector<long long> idvec;
    int start = adjacencyList::getIntID(adjCol,1);
    int end = adjacencyList::getIntID(adjCol,5);

    result = shortestPath::chooseAlgo(dijkstra,start,end,adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, start, end);
    //adjacencyList::printGraph(adjCol);

    assert(idvec[0]==1);
    assert(idvec[1]==2);
    assert(idvec[2]==4);
    assert(idvec[3]==5);
    //shortestPath::printVec(idvec);
    cout << "Adjlist Simple Dijkstra test passed" << endl;
}

void createToyExample(adjListCollection &collection) {
    //emulating reading from file from corresponding adjlist
    int aNode = adjacencyList::insertInMaps(collection,0);
    int bNode = adjacencyList::insertInMaps(collection,1);
    int cNode = adjacencyList::insertInMaps(collection,2);
    int dNode = adjacencyList::insertInMaps(collection,3);
    int eNode = adjacencyList::insertInMaps(collection,4);
    adjacencyList::addEdge(collection,aNode,bNode,6);
    adjacencyList::addEdge(collection,aNode,cNode,3);
    adjacencyList::addEdge(collection,bNode,cNode,4);
    adjacencyList::addEdge(collection,bNode,dNode,2);
    adjacencyList::addEdge(collection,cNode,bNode,1);
    adjacencyList::addEdge(collection,cNode,dNode,6);
    adjacencyList::addEdge(collection,cNode,eNode,2);
    adjacencyList::addEdge(collection,dNode,eNode,4);
    adjacencyList::addEdge(collection,eNode,dNode,2);
}

void testToyExampleDatastructure(){
    adjListCollection adjCol;
    //shortestPath::createAdjacencyList("/home/a/CLionProjects/BachelorCppRestructured/src/resources/dijkstraTest", "file", adjCol);
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
    //adjacencyList::printGraph(adjCol);
    assert(adjacencyList::getIntID(adjCol,aNode) == 0);
    assert(adjacencyList::getIntID(adjCol,bNode) == 1);
    assert(adjacencyList::getIntID(adjCol,cNode) == 2);
    assert(adjacencyList::getIntID(adjCol,dNode) == 3);
    assert(adjacencyList::getIntID(adjCol,eNode) == 4);
    assert(adjCol.adjlst[2].size()==3);
    assert(adjCol.adjlst[3].size()==1);
    cout<<"test toy example fixture passed" << endl;
}

void testDijkstraToyExample() {
    adjListCollection adjCol;
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
    //testing a->b gives path a,c,b with distance 4
    assert(idvec[0] == aNode);
    assert(idvec[1] == cNode);
    assert(idvec[2] == bNode);
    assert(result.distanceToDest == 4);

    result = shortestPath::chooseAlgo(dijkstra, aNode, cNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, cNode);
    //testing a->c gives path a,c with distance 3
    assert(idvec[0] == aNode);
    assert(idvec[1] == cNode);
    assert(result.distanceToDest == 3);

    result = shortestPath::chooseAlgo(dijkstra, aNode, dNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, dNode);
    //testing a->d gives path a,c,b,d with distance 6
    assert(idvec[0] == aNode);
    assert(idvec[1] == cNode);
    assert(idvec[2] == bNode);
    assert(idvec[3] == dNode);
    assert(result.distanceToDest == 6);

    result = shortestPath::chooseAlgo(dijkstra, aNode, eNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, eNode);
    //testing a->e gives path a,c,e with distance 5
    assert(idvec[0] == aNode);
    assert(idvec[1] == cNode);
    assert(idvec[2] == eNode);
    assert(result.distanceToDest == 5);
    cout << "Toy example Dijkstra test passed" << endl;
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

void testDistance(string method, int source, int target, adjListCollection &adjCol){
    //int from = adjacencyList::getIntID(adjCol,source);
    //int to = adjacencyList::getIntID(adjCol,target);
    spResultStruct result = shortestPath::chooseAlgo(spmap[method], source ,target,adjCol);
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
        //vector<long long> landmarksIDs = {322591088, 259252468, 6158438720, 330038011, 5584771074, 6285925457, 4160003077, 963497183}; //hardcoded landmarks for malta
        vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(12, adjCol);
        adjacencyList::setLandmarkStructs(adjCol, initedLandmarks);
        //cout << "gothere" << endl;
        auto t2 = high_resolution_clock::now();
        duration<double, milli> ms_double = t2 - t1;
        cout << " time: "<< (ms_double.count()/1000) << "seconds###"<<endl;
    }else if(country=="denmark"){
        //adjCol.xCoord.resize(3976155);
        //adjCol.yCoord.resize(3976155);
        //adjCol.adjlst.resize(3976155,vector<pair<int,double>>(14));

        cout << "###parsing " << country;
        auto t1 = high_resolution_clock::now();
        shortestPath::createAdjacencyList(denmark, "file", adjCol);
        //vector<long long> landmarksIDs = {313159998, 13131369, 483022215, 349198443, 252151251, 369778546, 6187723341, 2252204014, 3149548115, 118416, 1108078449, 6816079197, 2387109140, 371441979}; //hardcoded landmarks for denmark
        vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(12, adjCol);
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
    assert(firstElem.nodeID != 0);
}

void denmark50RandomPoints(string method){
    adjListCollection denmark = setUpDatastructure("denmark");
    int highestNbr = denmark.idSoFar;
    int amountOfTests = 50;
    srand(100);
    vector<int> ids(amountOfTests,0);
    for (int i = 0; i < amountOfTests; ++i) {
        ids[i] = rand() % highestNbr;
    }

    for (int i = 0; i < ids.size(); i+=2) {
        testDistance(method, i, i+1,denmark);
    }
    cout << "denmark 50 tests with " << method << " passed "<< endl;
}

int main(){
    //testAdjlistSimpleDijkstra();
    //testToyExampleDatastructure();
    //testDijkstraToyExample();
    //landmarksEmptyListTest();
    //runMaltaTests();
    runDenmarkTests();
    denmark50RandomPoints("dijkstra");
    denmark50RandomPoints("astar");
    denmark50RandomPoints("landmarks");
    return 0;
}