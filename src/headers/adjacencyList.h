//
// Created by simon on 12-04-2021.
//

#ifndef ADJACENCYLIST
#define ADJACENCYLIST

#include "../implementation/nodesAndWaysWrapper.cpp"
#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>


using namespace std;
    struct spResultStruct {
        double distanceToDest;
        vector<int> prevNode;
        vector<double> distanceVec;
        long long chosenLandmark;
    };
    struct landmarksStruct {
        long long nodeID;
        vector<double> distanceVec;
        vector<double> reversedDistanceVec;
    };
    struct adjListCollection {
        vector<vector<pair<int, double>>> adjlst{};
        map<long long, int> longIdToIntID{};
        map<int, long long> intIdToLongID{};
        vector<double> xCoord;
        vector<double> yCoord;
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


        static void addEdge(adjListCollection &collection, int source, int dest, double weight) {
            //cout << "sour " << source << " arrsize " << collection.adjlst.size() << "\n";
            if (collection.adjlst.empty() || collection.adjlst.size() - 1 < source) {
                vector<pair<int, double>> secondVector{};
                collection.adjlst.resize(source + 1, secondVector);
            }
            if (collection.adjlst.size() > source) {
                if (collection.adjlst[source].empty()) {
                    vector<pair<int, double>> secondVector{make_pair(dest, weight)};
                    collection.adjlst[source] = secondVector;
                    //cout << "first elem of vec is " << dest << "\n";
                } else {
                    collection.adjlst[source].emplace_back(dest, weight);
                    //cout << "new edge " << dest << "\n";
                }
            }
        }

        // Print adjacency list representaion ot graph
        //this has partly been yoinked
        static void printGraph(adjListCollection &collection) {
            cout << "size of adjlist " << collection.adjlst.size() << endl;
            for (int s = 0; s < collection.adjlst.size(); s++) {
                cout << "Node " << s << endl;
                for (auto &pair: collection.adjlst[s]) {
                    cout << "dest: " << pair.first << " weight: " << pair.second << endl;
                }
            }
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

        static void addxCoord(adjListCollection &collection, int node, double xCoord) {
            if (node+1 > collection.xCoord.size()){
                collection.xCoord.resize(node+1);
            }
            collection.xCoord[node] = xCoord;
        }

        static void addyCoord(adjListCollection &collection, int node, double yCoord){
            if (node+1 > collection.yCoord.size()){
                collection.yCoord.resize(node+1);
            }
            collection.yCoord[node] = yCoord;
        }

        //astar heuristic algorithms
        static double euclidDistance(double srcX, double srcY, double destX, double destY){
            return double(sqrt(pow(srcX-destX,2.0)+pow(srcY-destY,2.0)));
        }
        static double manhatDistance(double srcX, double srcY, double destX, double destY){
            return double(abs(srcX-destX)+abs(srcY-destY));
        }
        static double chebyDistance(double srcX, double srcY, double destX, double destY){
            return double(max(abs(srcX-destX),abs(srcY-destY)));
        }
        //astar heurisctic algorithms

        static double distanceCalc(double srcX, double srcY, double destX, double destY, int maxSpeed){
            double dist = sqrt(pow(srcX - destX,2.0)+pow(srcY - destY,2.0));
            return double(dist/maxSpeed);
        }

        static double getxCoord(adjListCollection &collection, int value){return collection.xCoord[value];}
        static double getyCoord(adjListCollection &collection, int value){return collection.yCoord[value];}

        static void setLandmarkStructs(adjListCollection &collection, landmarksStruct &landmark){
            collection.landmarksStructs.emplace_back(move(landmark));
        }

        static int getIntID(adjListCollection &collection, long long value) {
            return collection.longIdToIntID.find(value)->second;
        }

        static long long getLongID(adjListCollection &collection, int value) {
            return collection.intIdToLongID.find(value)->second;
        }
        //static nodesAndWaysWrapper deserializeFromJson(string filePath){
        //    using namespace nlohmann;
        //    json j;
        //    ifstream jsonFile;
        //    //File path with appropriate json file created by java in graphBuilder writeToFileAsJsonMethod
        //    jsonFile.open(filePath);
        //    jsonFile >> j;
        //    auto wrapperClass = j.get<nodesAndWaysWrapper>();
        //    return wrapperClass;
        //}
        //static void createAdjListCollection(nodesAndWaysWrapper wrapper, adjListCollection &adjListCollection){
        //    auto ways = wrapper.getWays();
        //    for(map<long, customWay>::iterator it = ways.begin(); it != ways.end(); it++){
        //        long previousId = 0;
        //        vector<long>::const_iterator itr;
        //        vector<long> nodeIds = it->second.getNodeIdList();
        //        auto nodes = wrapper.getNodes();
        //        for(itr = nodeIds.cbegin(); itr < nodeIds.cend(); itr++){
        //            if(*itr == nodeIds[0]){
        //                previousId = *itr;
        //            }
        //            else{
        //                customNode previousNode = nodes[previousId];
        //                double prevX = previousNode.getLatitudeAsXCoord();
        //                double prevY = previousNode.getLongtitudeAsYCoord();
//
        //                customNode currentNode = nodes[*itr];
        //                double currX = currentNode.getLatitudeAsXCoord();
        //                double currY = currentNode.getLongtitudeAsYCoord();
//
        //                double dist = euclidDistance(prevX, prevY, currX, currY);
        //                addEdge(adjListCollection, previousId, *itr, dist);
        //                addEdge(adjListCollection, *itr, previousId, dist);
//
        //                previousId = *itr;
        //            }
        //        }
        //    }
        //}
    };
#endif //ADJACENCYLIST
