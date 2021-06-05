//
// Created by simon on 12-04-2021.
//

#ifndef ADJACENCYLIST
#define ADJACENCYLIST

#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>


using namespace std;
    struct spResultStruct {
        float distanceToDest;
        vector<float> distanceVec;
        vector<int> prevNode;
        vector<bool> nodeSeen;
        long long chosenLandmark;
    };
    struct landmarksStruct {
        long long nodeID;
        vector<float> distanceVec;
        vector<float> reversedDistanceVec;
    };
    struct adjListCollection {
        vector<vector<pair<int, float>>> adjlst{};
        map<long long, int> longIdToIntID{};
        map<int, long long> intIdToLongID{};
        vector<float> xCoord;
        vector<float> yCoord;
        vector<landmarksStruct> landmarksStructs{};
        int idSoFar = 0;
    };

    class adjacencyList {
    public:
        static int insertInMaps(adjListCollection &collection, long long id) {
            //if the key doesnt already exsist
            int newId;
            if (collection.longIdToIntID.count(id) == 0) {
                collection.longIdToIntID.insert(make_pair(id, collection.idSoFar));
                collection.intIdToLongID.insert(make_pair(collection.idSoFar, id));

                newId = collection.idSoFar;
                collection.idSoFar++;
            } else {
                newId = collection.longIdToIntID.find(id)->second;
            }
            return newId;
        }


        static void addEdge(adjListCollection &collection, int source, int dest, float weight) {
            if (collection.adjlst.empty() || collection.adjlst.size() - 1 < source) {
                vector<pair<int, float>> secondVector{};
                collection.adjlst.resize(source + 1, secondVector);
            }
            if (collection.adjlst.size() > source) {
                if (collection.adjlst[source].empty()) {
                    vector<pair<int, float>> secondVector{make_pair(dest, weight)};
                    collection.adjlst[source] = secondVector;
                } else {
                    collection.adjlst[source].emplace_back(dest, weight);
                }
            }
        }

        static void printGraph(adjListCollection &collection) {
            cout << "size of adjlist " << collection.adjlst.size() << endl;
            for (int s = 0; s < collection.adjlst.size(); s++) {
                cout << "Node " << adjacencyList::getLongID(collection,s);
                for (auto &pair: collection.adjlst[s]) {
                    cout << " dest: " << adjacencyList::getLongID(collection,pair.first) << " weight: " << pair.second;
                }
                cout << endl;
            }
        }

        static void graphToFile(adjListCollection &collection) {
            cout << "making file "<< endl;
            ofstream myfile;
            myfile.open ("abcd");
            myfile  << "size of adjlist " << collection.adjlst.size() << "\n";
            for (int s = 0; s < collection.adjlst.size(); s++) {
                myfile << "Node " << adjacencyList::getLongID(collection,s);
                for (auto &pair: collection.adjlst[s]) {
                    myfile << " dest: " << adjacencyList::getLongID(collection,pair.first) << " weight: " << pair.second;
                }
                myfile << "\n";
            }
            myfile.close();
            cout << "file done"<< endl;
        }

        static vector<long long> prevNodeToShortestPath(adjListCollection &collection, vector<int> prevNode, int source, int destination) {
            int temdest = destination;
            vector<int> shortestPath;
            while (temdest != source && prevNode[temdest] != -1){
                shortestPath.push_back(temdest);
                temdest = prevNode[temdest];
            }
            shortestPath.push_back(source);
            reverse(shortestPath.begin(),shortestPath.end());
            vector<long long> spVectorLongs;
            for (auto id: shortestPath) {
                long long newid = collection.intIdToLongID.find(id)->second;
                spVectorLongs.push_back(newid);
            }
            return spVectorLongs;
        }

        static void addxCoord(adjListCollection &collection, int node, float xCoord) {
            if (node+1 > collection.xCoord.size()){
                collection.xCoord.resize(node+1);
            }
            collection.xCoord[node] = xCoord;
        }

        static void addyCoord(adjListCollection &collection, int node, float yCoord){
            if (node+1 > collection.yCoord.size()){
                collection.yCoord.resize(node+1);
            }
            collection.yCoord[node] = yCoord;
        }

        //astar heuristic algorithms
        static float euclidDistance(float srcX, float srcY, float destX, float destY){
            return sqrt(pow(srcX - destX,2.0)+pow(srcY - destY,2.0));
        }
        static float manhatDistance(float srcX, float srcY, float destX, float destY, int speed){
            return float(abs(srcX-destX)+abs(srcY-destY)* pow(speed,-1));
        }

        static float chebyDistance(float srcX, float srcY, float destX, float destY, int speed){
            return float(max(abs(srcX-destX),abs(srcY-destY))*pow(1000,-1)*pow(speed,-1));
        }
        //astar heurisctic algorithms

        static float distanceCalc(float srcX, float srcY, float destX, float destY, int speed){
            //calculates distance, and compenstates from meters to kilometers
            return sqrt(pow(srcX - destX,2.0)+pow(srcY - destY,2.0))*pow((speed*1000),-1);
        }

        static float getxCoord(adjListCollection &collection, int value){return collection.xCoord[value];}
        static float getyCoord(adjListCollection &collection, int value){return collection.yCoord[value];}

        static void setLandmarkStructs(adjListCollection &collection, landmarksStruct &landmark){
            collection.landmarksStructs.emplace_back(move(landmark));
        }

        static int getIntID(adjListCollection &collection, long long value) {
            return collection.longIdToIntID.find(value)->second;
        }

        static long long getLongID(adjListCollection &collection, int value) {
            return collection.intIdToLongID.find(value)->second;
        }
    };
#endif //ADJACENCYLIST