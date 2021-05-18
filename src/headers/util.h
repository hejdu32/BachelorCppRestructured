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
        string malta = "C:/Users/svend/IdeaProjects/BachelorProjectNew/malta";
        string denmark = "C:/Users/svend/IdeaProjects/BachelorProjectNew/denmark";

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
        cout << "Disagreement " << method << " from: " << source << " to: " << dest << " landmark: " << temp << " dijkstra dist: " << dijkstraStruct.distanceToDest << " " << method << " dist: "<< wrongStruct.distanceToDest << endl;
    }

    static void randomPointsComparrison(const string& country, int amountOfTests, int seed){
        adjListCollection countryCol = setUpDatastructure(country);
        int highestNbr = countryCol.idSoFar;
        //srand(seed);
        vector<int> ids(amountOfTests,0); int size = ids.size();
        for (int i = 0; i < amountOfTests; ++i) {
            ids[i] = rand() % highestNbr;
        }
        int astarFails = 0;
        int landmarksFails = 0;

        double totalDijkstraTime=0;
        double worstDijkstraTime=0;

        double totalAstarTime=0;
        double worstAstarTime=0;

        double totalALTTime=0;
        double worstALTTime=0;

        cout << "Testing "<< amountOfTests << " points in " <<  country<< endl;
        //choosing from and to as i and i+1, in case of i=vector size to is i[0]
        for (int i = 0; i < size; i++) {
            int from = ids[i];
            int to;
            if(i == size-1){
                to = ids[0];
            }else{
                to = ids[i+1];
            }
            if(i % 10 == 0 && i != 0){
                cout << i << " comparrisons have been tested" << endl;
            }

            //DIJKSTRA
            auto timerStart = high_resolution_clock::now();
            spResultStruct dijkstraResult = testDistance("dijkstra", from, to, countryCol);

            auto timerEnd = high_resolution_clock::now();
            duration<double, milli> timeDiff = timerEnd -timerStart;
            double timeInSecs=timeDiff.count()/1000;
            totalDijkstraTime+=timeInSecs;
            if (timeInSecs > worstDijkstraTime) worstDijkstraTime = timeInSecs;

            //ASTAR
            timerStart = high_resolution_clock::now();
            spResultStruct astarResult = testDistance("astar", from, to, countryCol);

            timerEnd = high_resolution_clock::now();
            timeDiff = timerEnd-timerStart;
            timeInSecs=timeDiff.count()/1000;
            totalAstarTime+=timeInSecs;
            if (timeInSecs > worstAstarTime) worstAstarTime = timeInSecs;

            //LANDMARKS
            timerStart = high_resolution_clock::now();
            spResultStruct landmarksResult = testDistance("landmarks", from, to, countryCol);

            timerEnd = high_resolution_clock::now();
            timeDiff = timerEnd-timerStart;
            timeInSecs=timeDiff.count()/1000;
            totalALTTime+=timeInSecs;
            if (timeInSecs > worstALTTime) worstALTTime = timeInSecs;

            if (dijkstraResult.distanceToDest != astarResult.distanceToDest){
                astarFails++;
                printDisagreement("astar", from, to, dijkstraResult, astarResult, countryCol);
            }
            if (dijkstraResult.distanceToDest != landmarksResult.distanceToDest){
                landmarksFails++;
                printDisagreement("landmarks", from, to, dijkstraResult, landmarksResult, countryCol);
            }
        }
        cout << "Finished "<< amountOfTests<< " on " << country << endl;
        cout << "astar fails: " << astarFails << " landmark fails: " << landmarksFails << endl;
        cout << "avg dijk time: " << totalDijkstraTime/amountOfTests << "secs worst case: " << worstDijkstraTime << "secs"<< endl;
        cout << "avg a*   time: " << totalAstarTime/amountOfTests << "secs worst case: " << worstAstarTime << "secs"<< endl;
        cout << "avg ALT  time: " << totalALTTime/amountOfTests << "secs worst case: " << worstALTTime << "secs"<< endl;
    }

};
#endif //BACHELORCPPCMAKE_UTIL_H
