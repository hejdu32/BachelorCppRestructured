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
#include <iomanip>
#include <algorithm>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::seconds;

class util{
public:
    static adjListCollection setUpDatastructure(const string &country, const string &type, string landmarkSelection) {
        adjListCollection adjCol;
        string malta = "/home/simon/BachelorCppRestructured/resources/malta";
        string denmark = "/home/simon/BachelorCppRestructured/resources/denmark";

        if(country== "malta"){
            cout << "parsing " << country << endl;
            auto t1 = high_resolution_clock::now();
            shortestPath::createAdjacencyList(malta, "file", adjCol);
            //vector<long long> landmarksIDs = {322591088, 259252468, 6158438720, 330038011, 5584771074, 6285925457, 4160003077, 963497183}; //hardcoded landmarks for malta
            vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(8, adjCol, landmarkSelection);
            for (int i = 0; i <initedLandmarks.size(); ++i) {
                adjacencyList::setLandmarkStructs(adjCol, initedLandmarks[i]);
            }
            //adjacencyList::setLandmarkStructs(adjCol, initedLandmarks);
            auto t2 = high_resolution_clock::now();
            duration<double, milli> ms_double = t2 - t1;
            cout << "time: "<< (ms_double.count()/1000) << "seconds"<<endl;
        }else if(country=="denmark"){
            cout << "parsing " << country << endl;
            auto t1 = high_resolution_clock::now();
            shortestPath::createAdjacencyList(denmark, "file", adjCol);
            //vector<long long> landmarksIDs = {2753462644,5745423643,57054823,2159452194,1177521825,489401874,283198526,1818976308,5098316959,971808896,1507951792,1116342996}; //hardcoded landmarks for denmark
            vector<landmarksStruct> initedLandmarks = landmarks::initLandmarks(10, adjCol, landmarkSelection);
            for (int i = 0; i <initedLandmarks.size(); ++i) {
                adjacencyList::setLandmarkStructs(adjCol, initedLandmarks[i]);
            }
            auto t2 = high_resolution_clock::now();
            duration<double, milli> ms_double = t2 - t1;
            cout << "time: "<< (ms_double.count()/1000) << "seconds"<<endl;
        }else {
            cout << country << " not found";
        }
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
        cout << "distance: " << result.first.distanceToDest << " time to find path: "<< result.second << "msecs"<<endl;
        int nodesConsidered = calcNodesConsidered(result.first.prevNode);

        cout << "Nodes in path: "<< idvec.size() << " Nodes considered: " << nodesConsidered << endl;
        cout << "\n";

    }

    static pair<spResultStruct, double> testDistance(const string& method, int source, int target, adjListCollection &adjCol){
        auto timerStart = high_resolution_clock::now();

        spResultStruct res = shortestPath::chooseAlgo(spmap[method], source , target, adjCol);

        auto timerEnd = high_resolution_clock::now();
        duration<double, milli> timeDiff = timerEnd -timerStart;
        double timeInSecs=timeDiff.count();
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

    static void randomPointsComparrisonAll(const string& country, int amountOfTests, int seed){
        adjListCollection countryCol = setUpDatastructure(country, "normal", "dijkstra");
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
            int from = ids[i];
            int to;
            if(i == size-1){
                to = ids[0];
            }else{
                to = ids[i+1];
            }
            if(i % 10 == 0 && i != 0){
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
        cout<< std::setprecision(3) << "avg dijk time: " << totalDijkstraTime/amountOfTests << "msec " << "avg nodesEval: " << dijkNodesConsidered <<" worst case time: " << worstDijkstraTime << "msec"<< endl;
        cout<< std::setprecision(3) << "avg a*   time: " << totalAstarTime/amountOfTests << "msec " << "avg nodesEval: " << astarNodesConsidered <<" worst case time: " << worstAstarTime << "msec"<< endl;
        cout<< std::setprecision(3) << "avg ALT  time: " << totalALTTime/amountOfTests << "msec " << "avg nodesEval: " << landmarksNodesConsidered <<" worst case time: " << worstALTTime << "msec"<< endl;
    }

    static void randomPointsComparrisonSingle(const string& country, int amountOfTests, int seed, const string& algorithm, string landmarkSelection){
        adjListCollection countryCol = setUpDatastructure(country, "normal", std::move(landmarkSelection));
        int highestNbr = countryCol.idSoFar;
        //srand(seed);
        vector<int> ids(amountOfTests,0); int size = ids.size();
        for (int i = 0; i < amountOfTests; ++i) {
            ids[i] = rand() % highestNbr;
        }
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

            if(algorithm == "dijkstra"){
                //DIJKSTRA
                pair<spResultStruct,double> dijkstraResult = testDistance("dijkstra", from, to, countryCol);
                totalDijkstraTime+=dijkstraResult.second;
                if (dijkstraResult.second > worstDijkstraTime) worstDijkstraTime = dijkstraResult.second;
                dijkNodesConsidered += calcNodesConsidered(dijkstraResult.first.prevNode);
            }
            else if(algorithm == "astar") {
                //ASTAR
                pair<spResultStruct,double> astarResult = testDistance("astar", from, to, countryCol);
                totalAstarTime+=astarResult.second;
                if (astarResult.second > worstAstarTime) worstAstarTime = astarResult.second;
                astarNodesConsidered += calcNodesConsidered(astarResult.first.prevNode);

            }
            else if(algorithm == "landmarks") {
                //LANDMARKS
                pair<spResultStruct,double> landmarksResult = testDistance("landmarks", from, to, countryCol);
                totalALTTime+=landmarksResult.second;
                if (landmarksResult.second > worstALTTime) worstALTTime = landmarksResult.second;
                landmarksNodesConsidered += calcNodesConsidered(landmarksResult.first.prevNode);

            }
            else {
                cout << "Wrong input to algorithm field" << endl;
            }
        }
        cout << std::fixed;
        if(algorithm == "dijkstra"){
            dijkNodesConsidered = dijkNodesConsidered/amountOfTests;
            cout<< std::setprecision(3) << "avg dijk time: " << totalDijkstraTime/amountOfTests << "msec " << "avg nodesEval: " << dijkNodesConsidered <<" worst case time: " << worstDijkstraTime << "msec"<< endl;
        }
        else if(algorithm == "astar") {
            astarNodesConsidered = astarNodesConsidered/amountOfTests;
            cout<< std::setprecision(3) << "avg a*   time: " << totalAstarTime/amountOfTests << "msec " << "avg nodesEval: " << astarNodesConsidered <<" worst case time: " << worstAstarTime << "msec"<< endl;
        }
        else if (algorithm == "landmarks"){
            landmarksNodesConsidered = landmarksNodesConsidered/amountOfTests;
            cout<< std::setprecision(3) << "avg ALT  time: " << totalALTTime/amountOfTests << "msec " << "avg nodesEval: " << landmarksNodesConsidered <<" worst case time: " << worstALTTime << "msec"<< endl;
        }
        }

    static void randomPointsComparrisonSingleToFile(const string& country, int amountOfTests, int seed, const string& algorithm, string landmarkSelection){
        adjListCollection countryCol = setUpDatastructure(country, "normal", std::move(landmarkSelection));
        int highestNbr = countryCol.idSoFar;
        srand(seed);
        ofstream myfile;
        myfile.open(country + "_" + algorithm);
        vector<int> ids(amountOfTests,0); int size = ids.size();
        for (int i = 0; i < amountOfTests; ++i) {
            ids[i] = rand() % highestNbr;
        }
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

            if(algorithm == "dijkstra"){
                //DIJKSTRA
                pair<spResultStruct,double> dijkstraResult = testDistance("dijkstra", from, to, countryCol);
                double dijkstraTime = dijkstraResult.second;
                int dijkstraNodesConsidered = calcNodesConsidered(dijkstraResult.first.prevNode);
                double dijkstraDistance = dijkstraResult.first.distanceToDest;
                string dataTobeWrittenToFile = to_string(dijkstraTime) + ":" + to_string(dijkstraNodesConsidered) + ":" + to_string(dijkstraDistance) + ":" + to_string(adjacencyList::getLongID(countryCol, from)) + ":" + to_string(adjacencyList::getLongID(countryCol, to));
                std::replace(dataTobeWrittenToFile.begin(), dataTobeWrittenToFile.end(), '.', ',');
                myfile << dataTobeWrittenToFile << "\n";
            }
            else if(algorithm == "astar") {
                //ASTAR
                pair<spResultStruct,double> astarResult = testDistance("astar", from, to, countryCol);
                double astarTime = astarResult.second;
                int astarNodesConsidered = calcNodesConsidered(astarResult.first.prevNode);
                double astarDistance = astarResult.first.distanceToDest;
                string dataTobeWrittenToFile = to_string(astarTime) + ":" + to_string(astarNodesConsidered) + ":" + to_string(astarDistance)  + ":" + to_string(adjacencyList::getLongID(countryCol, from)) + ":" + to_string(adjacencyList::getLongID(countryCol, to));;
                std::replace(dataTobeWrittenToFile.begin(), dataTobeWrittenToFile.end(), '.', ',');
                myfile << dataTobeWrittenToFile << "\n";
            }
            else if(algorithm == "landmarks") {
                //LANDMARKS
                pair<spResultStruct,double> landmarksResult = testDistance("landmarks", from, to, countryCol);
                double landmarksTime = landmarksResult.second;
                int landmarksNodesConsidered = calcNodesConsidered(landmarksResult.first.prevNode);
                double landmarksDistance = landmarksResult.first.distanceToDest;
                string dataTobeWrittenToFile = to_string(landmarksTime) + ":" + to_string(landmarksNodesConsidered) + ":" + to_string(landmarksDistance) + ":" + to_string(adjacencyList::getLongID(countryCol, from)) + ":" + to_string(adjacencyList::getLongID(countryCol, to)) + ":" + to_string(landmarksResult.first.chosenLandmark);
                std::replace(dataTobeWrittenToFile.begin(), dataTobeWrittenToFile.end(), '.', ',');
                myfile << dataTobeWrittenToFile << "\n";
            }
            else {
                cout << "Wrong input to algorithm field" << endl;
            }
        }
        myfile.close();
        cout << "File with country: " << country << " and algorithm: " << algorithm << endl;
    }



};
#endif //BACHELORCPPCMAKE_UTIL_H