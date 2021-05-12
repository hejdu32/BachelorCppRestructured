//
// Created by simon on 13-04-2021.
//
#include "headers/adjacencyList.h"
#include "headers/shortestPath.h"
#include "headers/util.h"
//#include <cstdlib>
#include <iostream>
#include <cassert>
//#include <chrono>
#include <vector>
#include <string>

using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::seconds;

void testAdjlistSimpleDijkstra(){
    adjListCollection adjCol;
    shortestPath::createAdjacencyList("/home/a/CLionProjects/BachelorCppRestructured/src/resources/adjlist","file",adjCol);

    spResultStruct result; vector<long long> idvec;
    int start = adjacencyList::getIntID(adjCol,1);
    int end = adjacencyList::getIntID(adjCol,5);

    result = shortestPath::chooseAlgo(spmap["dijkstra"],start,end,adjCol);
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

    //adjacencyList::printGraph(adjCol);
    //adjListCollection revCol = landmarks::reverseAdjListCollection(adjCol);
    //adjacencyList::printGraph(revCol);
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
    result = shortestPath::chooseAlgo(spmap["dijkstra"], aNode, bNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, bNode);
    //testing a->b gives path a,c,b with distance 4
    assert(idvec[0] == aNode);
    assert(idvec[1] == cNode);
    assert(idvec[2] == bNode);
    assert(result.distanceToDest == 4);

    result = shortestPath::chooseAlgo(spmap["dijkstra"], aNode, cNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, cNode);
    //testing a->c gives path a,c with distance 3
    assert(idvec[0] == aNode);
    assert(idvec[1] == cNode);
    assert(result.distanceToDest == 3);

    result = shortestPath::chooseAlgo(spmap["dijkstra"], aNode, dNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, dNode);
    //testing a->d gives path a,c,b,d with distance 6
    assert(idvec[0] == aNode);
    assert(idvec[1] == cNode);
    assert(idvec[2] == bNode);
    assert(idvec[3] == dNode);
    assert(result.distanceToDest == 6);

    result = shortestPath::chooseAlgo(spmap["dijkstra"], aNode, eNode, adjCol);
    idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.prevNode, aNode, eNode);
    //testing a->e gives path a,c,e with distance 5
    assert(idvec[0] == aNode);
    assert(idvec[1] == cNode);
    assert(idvec[2] == eNode);
    assert(result.distanceToDest == 5);
    cout << "Toy example Dijkstra test passed" << endl;
}

void runMaltaTests(){
    adjListCollection malta = util::setUpDatastructure("malta");
    //short distance downtown malta

    util::testDistancePrints("dijkstra",146885871, 1498913919,malta);
    util::testDistancePrints("astar",146885871, 1498913919,malta);
    util::testDistancePrints("landmarks",146885871, 1498913919,malta);
    //long distance arcoss malt island
    util::testDistancePrints("dijkstra",3593516725, 5037683804,malta);
    util::testDistancePrints("astar",3593516725, 5037683804,malta);
    util::testDistancePrints("landmarks",3593516725, 5037683804,malta);
}
void runDenmarkTests(){
    adjListCollection denmark = util::setUpDatastructure("denmark");
    long long itbyen = 957178678;
    long long borglum = 860574684;
    long long RandersHospital = 696724340;
    long long tivoliKobenhavn = 3207955227;
    long long skagen = 1818942364; //just south of skagen
    long long westernJutland = 2400200478; //western jutland
    long long swJutland = 7725330619; //south western jutland
    long long southernJutland = 2461634106; // southern jutland
    long long langeland = 5543870050; //langeland
    long long lolland = 1471395254; // lolland
    long long northernZealand = 1305304467; //nord sjælland
    //århus to århus
    util::testDistancePrints("dijkstra",itbyen, borglum,denmark);
    util::testDistancePrints("astar",itbyen, borglum,denmark);
    util::testDistancePrints("landmarks",itbyen, borglum,denmark);
    //århus to randers
    util::testDistancePrints("dijkstra",itbyen,RandersHospital,denmark);
    util::testDistancePrints("astar",itbyen,RandersHospital,denmark);
    util::testDistancePrints("landmarks",itbyen,RandersHospital,denmark);
    //århus to københavn
    util::testDistancePrints("dijkstra",itbyen,tivoliKobenhavn,denmark);
    util::testDistancePrints("astar",itbyen,tivoliKobenhavn,denmark);
    util::testDistancePrints("landmarks",itbyen,tivoliKobenhavn,denmark);
    //vest jylland til lolland
    util::testDistancePrints("dijkstra", westernJutland, lolland, denmark);
    util::testDistancePrints("astar", westernJutland, lolland, denmark);
    util::testDistancePrints("landmarks", westernJutland, lolland, denmark);
    //skagen til langeland
    util::testDistancePrints("dijkstra", skagen, langeland, denmark);
    util::testDistancePrints("astar", skagen, langeland, denmark);
    util::testDistancePrints("landmarks", skagen, langeland, denmark);
    //ho klit to skagen
    util::testDistancePrints("dijkstra", swJutland, skagen, denmark);
    util::testDistancePrints("astar", swJutland, skagen, denmark);
    util::testDistancePrints("landmarks", swJutland, skagen, denmark);
    //gammel ren to toftum bjerge
    util::testDistancePrints("dijkstra", southernJutland, westernJutland, denmark);
    util::testDistancePrints("astar", southernJutland, westernJutland, denmark);
    util::testDistancePrints("landmarks", southernJutland, westernJutland, denmark);
    //skagen to northern zealand
    util::testDistancePrints("dijkstra", skagen, northernZealand, denmark);
    util::testDistancePrints("astar", skagen, northernZealand, denmark);
    util::testDistancePrints("landmarks", skagen, northernZealand, denmark);

}

void landmarksEmptyListTest(){
    adjListCollection malta = util::setUpDatastructure("malta");
    //vector<long long> landmarksIDs = {322591088, 259252468, 6158438720, 330038011, 5584771074, 6285925457, 4160003077, 963497183}; //hardcoded landmarks for malta
    vector<landmarksStruct> notEmpty = landmarks::initLandmarks(8, malta);
    assert(notEmpty.size()==8);
    landmarksStruct firstElem = notEmpty[0];
    assert(!firstElem.distanceVec.empty());
    assert(firstElem.nodeID != 0);
}
void formatPrinting(){

    //cout<< "27/5 = " << 27/5 << endl;
    //cout << "27.8632/5.17531 = " << 27.8632/6.17531<< endl;
    //cout << "27.8632*(5.17531)^-1 = " << 27.8632* pow(6.17531,-1) << endl;
    vector<int> resVec = {-1,1,-1,2,3,4,-1,5,-1,-1,-1,6,7,8};
    vector<string> nodesConsideredAsStrings;
    for (int i = 0; i < resVec.size(); ++i) {
        if (resVec[i] != -1){
            nodesConsideredAsStrings.emplace_back(to_string(resVec[i]));
        }
    }
    int nodesPerLine = 3;
    int timesToLoop = (nodesConsideredAsStrings.size()/nodesPerLine);
    for (int i = 0; i < timesToLoop+1; ++i) {
        int indexStart = i*nodesPerLine;
        int indexEnd = indexStart+nodesPerLine;
        if (indexEnd> nodesConsideredAsStrings.size()){
            indexEnd = nodesConsideredAsStrings.size();
        }
        string listToSend =  "nodesConsidered";

        for (int j = indexStart; j < indexEnd; ++j) {
            listToSend += " " +nodesConsideredAsStrings[j];
        }
        cout<<listToSend<< endl;
    }
    cout<< "nodesConsidered end" << endl;
}

int main(){
    cout << "Testing" << endl;
    //formatPrinting();
    //testAdjlistSimpleDijkstra();
    //testToyExampleDatastructure();
    //testDijkstraToyExample();
    //landmarksEmptyListTest();
    runMaltaTests();
    runDenmarkTests();

    return 0;
}