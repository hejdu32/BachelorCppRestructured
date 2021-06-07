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
    //the struct returned by a shortest path algorithm
    struct spResultStruct {
        float distanceToDest;       //the distance from source to target
        vector<float> distanceVec;  //the vector of all distances calcualted during runtime
        vector<int> prevNode;       //the pi vector containing the predecessor of any evaluated node
        vector<bool> nodeSeen;      //the boolean vector describing every node that has been seen
        long long chosenLandmark;   // if a landmark was used during runtime, this is the chosen landmark
    };
    //the struct of a calculated landmark
    struct landmarksStruct {
        long long nodeID;                   //the node id of the landmark
        vector<float> distanceVec;          //the distance vector from the landmark to everynode
        vector<float> reversedDistanceVec;  //the distance vector from every node to landmark
    };
    //the adjacency list collection contains the adjacenylist, the maps of every node, the corresponding long id, their coordinates and the calcualted landmarks
    struct adjListCollection {
        vector<vector<pair<int, float>>> adjlst{};  //the adjacency list
        map<long long, int> longIdToIntID{};        //map from long long ids to internal int ids
        map<int, long long> intIdToLongID{};        //map from internal int ids to long long ids
        vector<float> xCoord;                       //x-coordinates for every node
        vector<float> yCoord;                       //y-coordinates for every node
        vector<landmarksStruct> landmarksStructs{}; //collection of all landmark calcualted during setup
        int idSoFar = 0;                            //counter used for generating unique int ids for every node, this is also an indicator of the amount of nodes stored in the graph
    };

    class adjacencyList {
    public:
        //inserts node into the maps of the adjListCollection
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
        //adds and edge from a source to a target node with a specified weight to the adjacencylist
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
        //test method used to print the read adjacency list to file
        static void graphToFile(adjListCollection &collection) {
            cout << "making file "<< endl;
            ofstream myfile;
            myfile.open ("testgraph");
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
        //given a target and source node reads though the prevnode vector and calculates the nodes in the shortest path from source to target
        //these are then returned as long long ids
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
        //adds a x-coordinate for a node
        static void addxCoord(adjListCollection &collection, int node, float xCoord) {
            if (node+1 > collection.xCoord.size()){
                collection.xCoord.resize(node+1);
            }
            collection.xCoord[node] = xCoord;
        }
        //adds a y-coordinate for a node
        static void addyCoord(adjListCollection &collection, int node, float yCoord){
            if (node+1 > collection.yCoord.size()){
                collection.yCoord.resize(node+1);
            }
            collection.yCoord[node] = yCoord;
        }

        //distance calculation for two nodes given their coordiantes and a speed between them
        //notice the euclid distnace calculated is divided by 1000 to convert to kilometers from meters
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