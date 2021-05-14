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
        string malta = "C:/Users/a/IdeaProjects/BachelorProject/malta";
        string denmark = "C:/Users/a/IdeaProjects/BachelorProject/denmark";

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

    static int calcNodesConsidered(vector<int> &prevNode){
        int counterVisited = 0;
        for (int i = 0; i < prevNode.size(); ++i) {
            if (prevNode[i] != -1){
                counterVisited++;
            }
        }
        return counterVisited;
    }
    static void testDistancePrints(const string& method, long long source, long long target, adjListCollection &adjCol){
        int from = adjacencyList::getIntID(adjCol,source);
        int to = adjacencyList::getIntID(adjCol,target);

        pair<spResultStruct, double> result = testDistance(method, from ,to,adjCol);

        vector<long long> idvec = adjacencyList::prevNodeToShortestPath(adjCol, result.first.prevNode, from, to);
        if(method == "landmarks"){
            cout << method <<" from: "<< source <<" to: " << target << " with landmark: " << result.first.chosenLandmark <<"\n" ;
        } else{
        cout << method <<" from: "<< source <<" to: " << target <<"\n" ;
        }
        cout << "distance: " << result.first.distanceToDest << " time to find path: "<< result.second << "secs"<<endl;
        int nodesConsidered = calcNodesConsidered(result.first.prevNode);
        //cout << endl;
        cout << "Nodes in path: "<< idvec.size() << " Nodes considered: " << nodesConsidered << endl;
        cout << "\n";
        //cout << "path"<< endl;
        //shortestPath::printVec(idvec);
        //cout << endl;
    }

    static pair<spResultStruct, double> testDistance(const string& method, int source, int target, adjListCollection &adjCol){
        auto timerStart = high_resolution_clock::now();

        spResultStruct res = shortestPath::chooseAlgo(spmap[method], source , target, adjCol);

        auto timerEnd = high_resolution_clock::now();
        duration<double, milli> timeDiff = timerEnd -timerStart;
        double timeInSecs=timeDiff.count()/1000;
        return make_pair(res,timeInSecs);
    }

    static void printDisagreement(const string& method, int from, int to, spResultStruct &dijkstraStruct, spResultStruct &wrongStruct, adjListCollection &adjCol){
        long long source = adjacencyList::getLongID(adjCol,from);
        long long dest = adjacencyList::getLongID(adjCol,to);
        string strToPrint = "Disagreement " + method + " from: " + to_string(source) + " to: " + to_string(dest);
        if(method == "landmarks") {
             strToPrint+= " landmark: "+to_string(wrongStruct.chosenLandmark);
        }
        strToPrint += " dijkstra dist: " + to_string(dijkstraStruct.distanceToDest) + " " + method + " dist: "+ to_string(wrongStruct.distanceToDest);
        cout << strToPrint << endl;
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
        long long dijkNodesConsidered=0;
        long long astarNodesConsidered=0;
        long long landmarksNodesConsidered=0;

        double totalDijkstraTime=0;
        double worstDijkstraTime=0;

        double totalAstarTime=0;
        double worstAstarTime=0;

        double totalALTTime=0;
        double worstALTTime=0;

        cout << "Testing "<< amountOfTests << " points in " <<  country<< endl;
        //choosing from and to as i and i+1, in case of i=vector size to is i[0]
        for (int i = 0; i < size; i++) {
            cout<< "test nmbr: " << i << endl;
            int from = ids[i];
            int to;
            if(i == size-1){
                to = ids[0];
            }else{
                to = ids[i+1];
            }
            if(i % 100 == 0 && i != 0){
                cout << i << " comparisons have been tested" << endl;
            }
            //DIJKSTRA
            pair<spResultStruct,double> dijkstraResult = testDistance("dijkstra", from, to, countryCol);
            totalDijkstraTime+=dijkstraResult.second;
            if (dijkstraResult.second > worstDijkstraTime) worstDijkstraTime = dijkstraResult.second;
            dijkNodesConsidered += calcNodesConsidered(dijkstraResult.first.prevNode);
            //ASTAR
            pair<spResultStruct,double> astarResult = testDistance("astar", from, to, countryCol);
            totalAstarTime+=astarResult.second;
            if (astarResult.second > worstAstarTime) worstAstarTime = astarResult.second;
            astarNodesConsidered += calcNodesConsidered(astarResult.first.prevNode);

            //LANDMARKS
            pair<spResultStruct,double> landmarksResult = testDistance("landmarks", from, to, countryCol);
            totalALTTime+=landmarksResult.second;
            if (landmarksResult.second > worstALTTime) worstALTTime = landmarksResult.second;
            landmarksNodesConsidered += calcNodesConsidered(landmarksResult.first.prevNode);


            if (dijkstraResult.first.distanceToDest != astarResult.first.distanceToDest){
                astarFails++;
                printDisagreement("astar", from, to, dijkstraResult.first, astarResult.first, countryCol);
            }
            if (dijkstraResult.first.distanceToDest != landmarksResult.first.distanceToDest){
                landmarksFails++;
                printDisagreement("landmarks", from, to, dijkstraResult.first, landmarksResult.first, countryCol);
            }
        }

        dijkNodesConsidered = dijkNodesConsidered/amountOfTests;
        astarNodesConsidered = astarNodesConsidered/amountOfTests;
        landmarksNodesConsidered = landmarksNodesConsidered/amountOfTests;

        cout << "Finished " << amountOfTests<< " tests on " << country << endl;
        cout << "astar fails: " << astarFails << " landmark fails: " << landmarksFails << endl;
        cout << std::fixed;
        cout<< std::setprecision(10) << "avg dijk time: " << totalDijkstraTime/amountOfTests << "secs " << "avg nodesEval: " << dijkNodesConsidered <<" worst case time: " << worstDijkstraTime << "secs"<< endl;
        cout<< std::setprecision(10) << "avg a*   time: " << totalAstarTime/amountOfTests << "secs " << "avg nodesEval: " << astarNodesConsidered <<" worst case time: " << worstAstarTime << "secs"<< endl;
        cout<< std::setprecision(10) << "avg ALT  time: " << totalALTTime/amountOfTests << "secs " << "avg nodesEval: " << landmarksNodesConsidered <<" worst case time: " << worstALTTime << "secs"<< endl;
    }

};
#endif //BACHELORCPPCMAKE_UTIL_H
