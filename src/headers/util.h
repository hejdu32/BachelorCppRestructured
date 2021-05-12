//
// Created by a on 05-05-2021.
//

#ifndef BACHELORCPPCMAKE_UTIL_H

#define BACHELORCPPCMAKE_UTIL_H
#include "adjacencyList.h"
#include "shortestPath.h"
#include <cstdlib>
#include <chrono>
#include <utility>


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::seconds;

class util{
public:
    static adjListCollection setUpDatastructure(const string& country){
        adjListCollection adjCol;
        string malta = "C:/Users/a/IdeaProjects/BachelorProject/app/malta";
        string denmark = "C:/Users/a/IdeaProjects/BachelorProject/app/denmark";

        if(country== "malta"){
            cout << "###parsing " << country << endl;
            auto t1 = high_resolution_clock::now();
            shortestPath::createAdjacencyList(malta, "file", adjCol);
            //vector<long long> landmarksIDs = {322591088, 259252468, 6158438720, 330038011, 5584771074, 6285925457, 4160003077, 963497183}; //hardcoded landmarks for malta
            vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(8, adjCol);
            for (int i = 0; i <initedLandmarks.size(); ++i) {
                adjacencyList::setLandmarkStructs(adjCol, initedLandmarks[i]);
            }
            //adjacencyList::setLandmarkStructs(adjCol, initedLandmarks);
            auto t2 = high_resolution_clock::now();
            duration<double, milli> ms_double = t2 - t1;
            cout << " time: "<< (ms_double.count()/1000) << "seconds###"<<endl;
        }else if(country=="denmark"){
            cout << "###parsing " << country << endl;
            auto t1 = high_resolution_clock::now();
            shortestPath::createAdjacencyList(denmark, "file", adjCol);
            //vector<long long> landmarksIDs = {2753462644,5745423643,57054823,2159452194,1177521825,489401874,283198526,1818976308,5098316959,971808896,1507951792,1116342996}; //hardcoded landmarks for denmark
            vector<landmarksStruct> initedLandmarks = landmarks::initLandmarks(12, adjCol);
            for (int i = 0; i <initedLandmarks.size(); ++i) {
                adjacencyList::setLandmarkStructs(adjCol, initedLandmarks[i]);
            }
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
        if(method == "landmarks"){
            cout << method <<" from: "<< source <<" to: " << target << " with landmark: " << result.chosenLandmark <<"\n" ;
        } else{
        cout << method <<" from: "<< source <<" to: " << target <<"\n" ;
        }
        cout << "distance: " << result.distanceToDest << " time to find path: "<< ms_double.count()/1000 << "secs"<<endl;
        int counterVisited = 0;
        for (int id : result.prevNode){
            if (id != -1){
                counterVisited++;
                //cout << adjacencyList::getLongID(adjCol, id) << "L," ;
            }
        }
        //cout << endl;
        cout << "Nodes in path: "<< idvec.size() << " Nodes considered: " << counterVisited << endl;
        cout << "\n";
        //cout << "path"<< endl;
        //shortestPath::printVec(idvec);
        //cout << endl;
    }

    static spResultStruct testDistance(const string& method, int source, int target, adjListCollection &adjCol){
        return shortestPath::chooseAlgo(spmap[method], source ,target,adjCol);
    }

    static void printDisagreement(string method, int from, int to, spResultStruct &dijkstraStruct, spResultStruct &wrongStruct, adjListCollection &adjCol){
        long long source = adjacencyList::getLongID(adjCol,from);
        long long dest = adjacencyList::getLongID(adjCol,to);
        long long temp = -1;
        if(method == "landmarks") {
             temp = wrongStruct.chosenLandmark;
        }
        cout << "Disagreement with " << method << " from: " << source << " to: " << dest << " landmark: " << temp << endl;
        cout << "dijkstra distance: " << dijkstraStruct.distanceToDest << " " << method << " distance: "<< wrongStruct.distanceToDest << endl;

    }

    static void randomPointsComparrison(const string& country, int amountOfTests, int seed){
        adjListCollection denmark = setUpDatastructure(country);
        int highestNbr = denmark.idSoFar;
        //srand(seed);
        vector<int> ids(amountOfTests,0); int size = ids.size();
        for (int i = 0; i < amountOfTests; ++i) {
            ids[i] = rand() % highestNbr;
        }

        int astarFails = 0;
        int landmarksFails = 0;

        spResultStruct dijkstraResult;
        spResultStruct astarResult;
        spResultStruct landmarksResult;

        cout << "Testing "<< amountOfTests << " points in" <<  country<< endl;
        for (int i = 0; i < size; i++) {
            int from = ids[i];
            int to;
            if(i == size){
                to = ids[0];
            }else{
                to = ids[i+1];
            }
            if(i % 100 == 0 && i != 0){
                cout << i << " comparrisons have been tested" << endl;
            }
            dijkstraResult = testDistance("dijkstra", from, to,denmark);
            astarResult = testDistance("astar", from, to,denmark);
            landmarksResult = testDistance("landmarks", from, to,denmark);

            if (dijkstraResult.distanceToDest != astarResult.distanceToDest){
                astarFails++;
                printDisagreement("astar",from,to,dijkstraResult, astarResult,denmark);
            }
            if (dijkstraResult.distanceToDest != landmarksResult.distanceToDest){
                landmarksFails++;
                printDisagreement("landmarks",from,to,dijkstraResult, landmarksResult,denmark);
            }
        }
        cout << "Finished "<< amountOfTests<< " on denmark" << endl;
        cout << "astar fails: " << astarFails << " landmark fails: " << landmarksFails << endl;
    }

};
#endif //BACHELORCPPCMAKE_UTIL_H
