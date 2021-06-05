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
#include <typeinfo>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::seconds;

class util{
public:
    static adjListCollection setUpDatastructure(const string &country, const string &type, const string& landmarkSelection) {
        adjListCollection adjCol;
        string malta = "C:/Users/a/IdeaProjects/BachelorProject/malta";
        string denmark = "C:/Users/a/IdeaProjects/BachelorProject/denmark";

        if(country== "malta"){
            cout << "parsing " << country << endl;
            auto t1 = high_resolution_clock::now();
            shortestPath::createAdjacencyList(malta, "file", adjCol);
            //vector<long long> landmarksIDs = {322591088, 259252468, 6158438720, 330038011, 5584771074, 6285925457, 4160003077, 963497183}; //hardcoded landmarks for malta
            vector <landmarksStruct> initedLandmarks = landmarks::initLandmarks(8, adjCol, landmarkSelection);
            for (auto & initedLandmark : initedLandmarks) {
                adjacencyList::setLandmarkStructs(adjCol, initedLandmark);
            }
            auto t2 = high_resolution_clock::now();
            duration<double, milli> ms_double = t2 - t1;
            cout << "time: "<< (ms_double.count()/1000) << "seconds"<<endl;
        }else if(country=="denmark"){
            cout << "parsing " << country << endl;
            auto t1 = high_resolution_clock::now();
            shortestPath::createAdjacencyList(denmark, "file", adjCol);
            vector<landmarksStruct> initedLandmarks = landmarks::initLandmarks(20, adjCol, landmarkSelection);
            for (auto & initedLandmark : initedLandmarks) {
                adjacencyList::setLandmarkStructs(adjCol, initedLandmark);
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
        for (int i : prevNode) {
            if (i != -1){
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
            strToPrint+= " lmk: "+to_string(wrongStruct.chosenLandmark);
        }
        strToPrint += " dijkstra dist: " + to_string(dijkstraStruct.distanceToDest) + " " + method + " dist: "+ to_string(wrongStruct.distanceToDest);
        cout.precision(17);
        cout<< std::fixed << strToPrint << endl;
    }

    static void randomPointsComparrisonAll(const string& country, int amountOfTests, int seed){
        adjListCollection countryCol = setUpDatastructure(country, "normal", "dijkstraDistance");
        //const float INF = std::numeric_limits<float>::infinity();
        cout<<"amount of nodes: " << countryCol.landmarksStructs[0].distanceVec.size() << endl;

        for (const landmarksStruct& lmk:countryCol.landmarksStructs) {
            //int infDistNodes=0;
            //for (float dist:lmk.distanceVec) {
            //    if (dist==INF)
            //        infDistNodes+=1;
            //}
            cout << "https://www.openstreetmap.org/node/" << lmk.nodeID << "#map=8/56.216/12.816" << endl;
            //cout<< "landmark: " << lmk.nodeID << " infNodes " << infDistNodes<< endl;
        }

        int highestNbr = countryCol.idSoFar;
        srand(seed);
        vector<int> ids(amountOfTests,0); int size = ids.size();
        for (int i = 0; i < amountOfTests; ++i) {
            ids[i] = rand() % highestNbr;
        }
        int astarFails = 0;
        int landmarksFails = 0;
        double dijkNodesConsidered=0;
        double dijkNodesInSSP =0;
        double dijkConsidDivPath = 0;

        double astarNodesConsidered=0;
        double astarNodesInSSP =0;
        double astarConsidDivPath = 0;

        double landmarksNodesConsidered=0;
        double landmarksNodesInSSP =0;
        double landmarksConsidDivPath = 0;

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
            dijkNodesInSSP += adjacencyList::prevNodeToShortestPath(countryCol,dijkstraResult.first.prevNode,from,to).size();
            //cout << "dijk: " << dijkNodesConsidered << "/" << dijkNodesInSSP << "="<<dijkConsidDivPath << endl;

            //ASTAR
            pair<spResultStruct,double> astarResult = testDistance("astar", from, to, countryCol);
            totalAstarTime+=astarResult.second;
            if (astarResult.second > worstAstarTime) worstAstarTime = astarResult.second;
            astarNodesConsidered += calcNodesConsidered(astarResult.first.prevNode);
            astarNodesInSSP += adjacencyList::prevNodeToShortestPath(countryCol,astarResult.first.prevNode,from,to).size();


            //LANDMARKS
            pair<spResultStruct,double> landmarksResult = testDistance("landmarks", from, to, countryCol);
            totalALTTime+=landmarksResult.second;
            if (landmarksResult.second > worstALTTime) worstALTTime = landmarksResult.second;
            landmarksNodesConsidered += calcNodesConsidered(landmarksResult.first.prevNode);
            landmarksNodesInSSP += adjacencyList::prevNodeToShortestPath(countryCol,landmarksResult.first.prevNode,from,to).size();


            //cout << "ALT: " << landmarksNodesConsidered << "/" << landmarksNodesInSSP << "="<<landmarksConsidDivPath << endl;

            if (dijkstraResult.first.distanceToDest != astarResult.first.distanceToDest){
                astarFails++;
                printDisagreement("astar", from, to, dijkstraResult.first, astarResult.first, countryCol);
            }
            if (dijkstraResult.first.distanceToDest != landmarksResult.first.distanceToDest){
                landmarksFails++;
                printDisagreement("landmarks", from, to, dijkstraResult.first, landmarksResult.first, countryCol);
            }


        }
        dijkConsidDivPath = dijkNodesConsidered/dijkNodesInSSP;
        dijkNodesConsidered = dijkNodesConsidered/amountOfTests;

        astarConsidDivPath = astarNodesConsidered/astarNodesInSSP;
        astarNodesConsidered = astarNodesConsidered/amountOfTests;

        landmarksConsidDivPath = landmarksNodesConsidered/landmarksNodesInSSP;
        landmarksNodesConsidered = landmarksNodesConsidered/amountOfTests;

        cout << "Finished " << amountOfTests<< " tests on " << country << endl;
        cout << "astar fails: " << astarFails << " landmark fails: " << landmarksFails << endl;
        cout << std::fixed;
        cout<< std::setprecision(3) << "avg dijk time: " << totalDijkstraTime/amountOfTests << "msec " << " nodesConsid/Path: "<< dijkConsidDivPath << " avg nodesEval: " << dijkNodesConsidered <<" worst case time: " << worstDijkstraTime << "msec"<< endl;
        cout<< std::setprecision(3) << "avg a*   time: " << totalAstarTime/amountOfTests << "msec " << " nodesConsid/Path: "<< astarConsidDivPath<< " avg nodesEval: " << astarNodesConsidered <<" worst case time: " << worstAstarTime << "msec"<< endl;
        cout<< std::setprecision(3) << "avg ALT  time: " << totalALTTime/amountOfTests << "msec " << " nodesConsid/Path: "<< landmarksConsidDivPath<< " avg nodesEval: " << landmarksNodesConsidered <<" worst case time: " << worstALTTime << "msec"<< endl;
    }

    static void amountOfLandmarksTest(const string& country, int amountOfTests, int landmarkAmount, int seed){
        adjListCollection countryCol = setUpDatastructure(country, "normal", "dijkstraDistance");
        vector<string> dataToFile;
        int highestNbr = countryCol.idSoFar;
        srand(seed);
        vector<int> ids(amountOfTests,0); int size = ids.size();
        for (int j = 0; j < amountOfTests; ++j) {
            ids[j] = rand() % highestNbr;
        }

        for (int i = landmarkAmount; i > 0; --i) {
            countryCol.landmarksStructs.resize(i);
            cout << i << " landmarks: " << endl;
            for (const landmarksStruct& lmk:countryCol.landmarksStructs) {
                cout << "https://www.openstreetmap.org/node/" << lmk.nodeID << "#map=8/56.216/12.816" << endl;
                //cout<< "landmark: " << lmk.nodeID << " infNodes " << infDistNodes<< endl;
            }


            int landmarksFails = 0;
            double dijkNodesConsidered=0;
            double dijkNodesInSSP =0;
            double dijkConsidDivPath = 0;

            double landmarksNodesConsidered=0;
            double landmarksNodesInSSP =0;
            double landmarksConsidDivPath = 0;

            double totalDijkstraTime=0;
            double worstDijkstraTime=0;

            double totalALTTime=0;
            double worstALTTime=0;
            for (int k = 0; k < size; k++) {
                int from = ids[k];
                int to;
                if (k == size - 1) {
                    to = ids[0];
                } else {
                    to = ids[k + 1];
                }
                if (k % 10 == 0 && k != 0) {
                    cout << k << " comparisons have been tested" << endl;
                }
                //DIJKSTRA
                pair<spResultStruct, double> dijkstraResult = testDistance("dijkstra", from, to, countryCol);
                totalDijkstraTime += dijkstraResult.second;
                if (dijkstraResult.second > worstDijkstraTime) worstDijkstraTime = dijkstraResult.second;
                dijkNodesConsidered += calcNodesConsidered(dijkstraResult.first.prevNode);
                dijkNodesInSSP += adjacencyList::prevNodeToShortestPath(countryCol, dijkstraResult.first.prevNode, from,
                                                                        to).size();
                //LANDMARKS
                pair<spResultStruct,double> landmarksResult = testDistance("landmarks", from, to, countryCol);
                totalALTTime+=landmarksResult.second;
                if (landmarksResult.second > worstALTTime) worstALTTime = landmarksResult.second;
                landmarksNodesConsidered += calcNodesConsidered(landmarksResult.first.prevNode);
                landmarksNodesInSSP += adjacencyList::prevNodeToShortestPath(countryCol,landmarksResult.first.prevNode,from,to).size();

                if (dijkstraResult.first.distanceToDest != landmarksResult.first.distanceToDest){
                    landmarksFails++;
                    printDisagreement("landmarks", from, to, dijkstraResult.first, landmarksResult.first, countryCol);
                }
            }
            dijkConsidDivPath = dijkNodesConsidered/dijkNodesInSSP;
            dijkNodesConsidered = dijkNodesConsidered/amountOfTests;


            landmarksConsidDivPath = landmarksNodesConsidered/landmarksNodesInSSP;
            landmarksNodesConsidered = landmarksNodesConsidered/amountOfTests;
            landmarksNodesInSSP = landmarksNodesInSSP/amountOfTests;
            //datalayout: landmarkAmount, average running time, average nodes considered,  average nodes in path, landmarks....
            string values = to_string(i)+":"+ to_string(totalDijkstraTime/amountOfTests)+":"+ to_string(landmarksNodesConsidered)+":"+to_string(landmarksNodesInSSP);
            for (const landmarksStruct& landmarker:countryCol.landmarksStructs) {
                values.append(":"+to_string(landmarker.nodeID));
            }
            std::replace(values.begin(), values.end(), '.', ',');
            dataToFile.emplace_back(values);

            cout << "Finished " << amountOfTests<< " tests on " << country << endl;
            cout << "landmark fails: " << landmarksFails << endl;
            cout << std::fixed;
            cout<< std::setprecision(3) << "avg dijk time: " << totalDijkstraTime/amountOfTests << "msec " << " nodesConsid/Path: "<< dijkConsidDivPath << " avg nodesEval: " << dijkNodesConsidered <<" worst case time: " << worstDijkstraTime << "msec"<< endl;
            cout<< std::setprecision(3) << "avg ALT  time: " << totalALTTime/amountOfTests << "msec " << " nodesConsid/Path: "<< landmarksConsidDivPath<< " avg nodesEval: " << landmarksNodesConsidered <<" worst case time: " << worstALTTime << "msec"<< endl;
        }

        reverse(dataToFile.begin(),dataToFile.end());

        ofstream myfile;
        myfile.open(country+"LandmarkAmount"+ to_string(landmarkAmount));
        for (const string& line:dataToFile) {
            cout << line << "\n";
            myfile << line << "\n";
        }
        }

    static void randomPointsComparrisonSingle(const string& country, int amountOfTests, int seed, const string& algorithm, string landmarkSelection){
        adjListCollection countryCol = setUpDatastructure(country, "normal", std::move(landmarkSelection));
        int highestNbr = countryCol.idSoFar;
        srand(seed);
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

    static void randomPointsComparrisonSingleToFile(const string& country, int amountOfTests, int seed, const string& algorithm, const string& landmarkSelection){
        adjListCollection countryCol = setUpDatastructure(country, "normal", landmarkSelection);
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
            if(i % 100 == 0) {
                cout << i << " tests written to file" << endl;
            }

            if(algorithm == "dijkstra"){
                //DIJKSTRA
                pair<spResultStruct,double> dijkstraResult = testDistance("dijkstra", from, to, countryCol);
                int numberOfNodesInShortestPath = adjacencyList::prevNodeToShortestPath(countryCol, dijkstraResult.first.prevNode, from, to).size();
                double dijkstraTime = dijkstraResult.second;
                int dijkstraNodesConsidered = calcNodesConsidered(dijkstraResult.first.prevNode);
                double dijkstraDistance = dijkstraResult.first.distanceToDest;
                string dataTobeWrittenToFile = to_string(dijkstraTime) + ":" + to_string(dijkstraNodesConsidered) + ":" + to_string(dijkstraDistance) + ":" + to_string(adjacencyList::getLongID(countryCol, from)) + ":" + to_string(adjacencyList::getLongID(countryCol, to)) + ":" + to_string(numberOfNodesInShortestPath);
                std::replace(dataTobeWrittenToFile.begin(), dataTobeWrittenToFile.end(), '.', ',');
                myfile << dataTobeWrittenToFile << "\n";
            }
            else if(algorithm == "astar") {
                //ASTAR
                pair<spResultStruct,double> astarResult = testDistance("astar", from, to, countryCol);
                int numberOfNodesInShortestPath = adjacencyList::prevNodeToShortestPath(countryCol, astarResult.first.prevNode, from, to).size();
                double astarTime = astarResult.second;
                int astarNodesConsidered = calcNodesConsidered(astarResult.first.prevNode);
                double astarDistance = astarResult.first.distanceToDest;
                string dataTobeWrittenToFile = to_string(astarTime) + ":" + to_string(astarNodesConsidered) + ":" + to_string(astarDistance)  + ":" + to_string(adjacencyList::getLongID(countryCol, from)) + ":" + to_string(adjacencyList::getLongID(countryCol, to)) + ":" + to_string(numberOfNodesInShortestPath);
                std::replace(dataTobeWrittenToFile.begin(), dataTobeWrittenToFile.end(), '.', ',');
                myfile << dataTobeWrittenToFile << "\n";
            }
            else if(algorithm == "landmarks") {
                //LANDMARKS
                pair<spResultStruct,double> landmarksResult = testDistance("landmarks", from, to, countryCol);
                landmarksStruct landmark = landmarks::choseLandmarks(from, to, countryCol);
                string landmarkFromOrTo = landmarks::calcHeuristicDistanceWithReturn(from, to, landmark);
                int numberOfNodesInShortestPath = adjacencyList::prevNodeToShortestPath(countryCol, landmarksResult.first.prevNode, from, to).size();
                double landmarksTime = landmarksResult.second;
                int landmarksNodesConsidered = calcNodesConsidered(landmarksResult.first.prevNode);
                int landmarksEdgesConsidered = countNumberOfEdgesConsidered(countryCol, landmarksResult.first);
                double landmarksDistance = landmarksResult.first.distanceToDest;
                string dataTobeWrittenToFile = to_string(landmarksTime) + ":" + to_string(landmarksNodesConsidered) + ":" + to_string(landmarksDistance) + ":" + to_string(adjacencyList::getLongID(countryCol, from)) + ":" + to_string(adjacencyList::getLongID(countryCol, to)) + ":" + to_string(landmarksResult.first.chosenLandmark) + ":" + landmarkFromOrTo + ":" + to_string(numberOfNodesInShortestPath) + ":" + to_string(landmarksEdgesConsidered);
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

    static int countNumberOfEdgesConsidered(adjListCollection &adjCol, spResultStruct &resultStruct){
        int edgesConsidered = 0;
        for(int i = 0; i < resultStruct.nodeSeen.size(); i++){
            if(resultStruct.nodeSeen[i]){
                edgesConsidered += adjCol.adjlst[i].size();
            }
        }
        return edgesConsidered;
    }


    static void specialPointsComparrisonSingleToFile(const string& country,const string& identifier, int seed, const string& algorithm, const string& landmarkSelection,const vector<long long>& testPoints){
        adjListCollection countryCol = setUpDatastructure(country, "normal", landmarkSelection);
        srand(seed);
        ofstream myfile;
        myfile.open("special "+ identifier+" "+country + "_" + algorithm);
        vector<int> ids;
        for (long long memeid: testPoints) {
            ids.emplace_back(adjacencyList::getIntID(countryCol,memeid));
        }
        int size = ids.size();

        cout << "Testing "<< size/2 << " points in " <<  country<< endl;
        //choosing from and to as i and i+1, in case of i=vector size to is i[0]
        for (int i = 0; i < size; i+=2) {
            int from = ids[i];
            int to = ids[i + 1];

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