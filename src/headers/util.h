//
// Created by a on 05-05-2021.
//

#ifndef BACHELORCPPCMAKE_UTIL_H

#define BACHELORCPPCMAKE_UTIL_H
#include "adjacencyList.h"
#include "shortestPath.h"
#include <cstdlib>
#include <chrono>


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::seconds;

class util{
public:
    static adjListCollection setUpDatastructure(string country){
        adjListCollection adjCol;
        string malta = "E:/Proj/BachelorCppRestructured/resources/malta";
        string denmark = "E:/Proj/BachelorCppRestructured/resources/denmark";

        if(country== "malta"){
            cout << "###parsing " << country << endl;
            auto t1 = high_resolution_clock::now();
            shortestPath::createAdjacencyList(malta, "file", adjCol);
            //vector<long long> landmarksIDs = {322591088, 259252468, 6158438720, 330038011, 5584771074, 6285925457, 4160003077, 963497183}; //hardcoded landmarks for malta
            vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(16, adjCol);
            adjacencyList::setLandmarkStructs(adjCol, initedLandmarks);
            auto t2 = high_resolution_clock::now();
            duration<double, milli> ms_double = t2 - t1;
            cout << " time: "<< (ms_double.count()/1000) << "seconds###"<<endl;
        }else if(country=="denmark"){
            cout << "###parsing " << country << endl;
            auto t1 = high_resolution_clock::now();
            shortestPath::createAdjacencyList(denmark, "file", adjCol);
            //vector<long long> landmarksIDs = {2753462644,5745423643,57054823,2159452194,1177521825,489401874,283198526,1818976308,5098316959,971808896,1507951792,1116342996}; //hardcoded landmarks for denmark
            vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(3, adjCol);
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

    static void testDistancePrints(const string& method, long long source, long long target, adjListCollection &adjCol){
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
        //cout << "path"<< endl;
        //shortestPath::printVec(idvec);
        //cout << endl;
    }

    static spResultStruct testDistance(const string& method, int source, int target, adjListCollection &adjCol){
        return shortestPath::chooseAlgo(spmap[method], source ,target,adjCol);
    }

    static void printDisagreement(string method, int from, int to, double dijkstraDist, double wrongDist, adjListCollection &adjCol){
        long long source = adjacencyList::getLongID(adjCol,from);
        long long dest = adjacencyList::getLongID(adjCol,to);
        cout << "Disagreement with " << method << " from: " << source << " to: " << dest << endl;
        cout << "dijkstra distance: " << dijkstraDist << " " << method << " distance: "<< wrongDist << endl;

    }

    static void randomPointsComparrison(int amountOfTests, int seed){
        adjListCollection denmark = setUpDatastructure("denmark");
        int highestNbr = denmark.idSoFar;
        srand(seed);
        vector<int> ids(amountOfTests,0); int size = ids.size();
        for (int i = 0; i < amountOfTests; ++i) {
            ids[i] = rand() % highestNbr;
        }

        int astarFails = 0;
        int landmarksFails = 0;

        spResultStruct dijkstraResult;
        spResultStruct astarResult;
        spResultStruct landmarksResult;


        cout << "Testing "<< amountOfTests << " points in denmark " << endl;
        for (int i = 0; i < size; i+=2) {
            if(i % 100 == 0 && i != 0){
                cout << i << " comparrisons have been tested" << endl;
            }
            dijkstraResult = testDistance("dijkstra", ids[i], ids[i+1],denmark);
            astarResult = testDistance("astar", ids[i], ids[i+1],denmark);
            landmarksResult = testDistance("landmarks", ids[i], ids[i+1],denmark);

            if (dijkstraResult.distanceToDest != astarResult.distanceToDest){
                astarFails++;
                printDisagreement("astar",ids[i],ids[i+1],dijkstraResult.distanceToDest,astarResult.distanceToDest,denmark);
            }
            if (dijkstraResult.distanceToDest != landmarksResult.distanceToDest){
                landmarksFails++;
                printDisagreement("landmarks",ids[i],ids[i+1],dijkstraResult.distanceToDest,landmarksResult.distanceToDest,denmark);
            }
        }
        cout << "Finished "<< amountOfTests<< " on denmark" << endl;
        cout << "astar fails: " << astarFails << " landmark fails: " << landmarksFails << endl;
    }

};
#endif //BACHELORCPPCMAKE_UTIL_H
