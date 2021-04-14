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


using namespace std;

    struct adjListCollection {
        std::vector<std::vector<std::pair<int, double>>> adjlst{};
        std::map<long long, int> longIdToIntID{};
        std::map<int, long long> intIdToLongID{};
        //std::map<int, double> heuristicDistance;
        std::vector<double> xCoord;
        std::vector<double> yCoord;
        int idSoFar = 0;
    };

    class adjacencyList {
    public:
        static int insertInMaps(adjListCollection &collection, long long int id) {
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
                    cout << " dest: " << pair.first << " weight: " << pair.second << endl;
                }
            }
        }

        static vector<long long int> spVectorToLongId(adjListCollection &collection, vector<int> spList) {
            vector<long long int> vectorWLongs;
            for (auto id: spList) {
                long long int newid = collection.intIdToLongID.find(id)->second;
                vectorWLongs.push_back(newid);
            }
            return vectorWLongs;
        }

        //static void addHeuristicDist(adjListCollection &collection, int source, double HeuristicDist) {
        //    collection.heuristicDistance.insert(make_pair(source, HeuristicDist));
        //}
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

        static double distanceCalc(double srcX, double srcY, double destX, double destY){
            return double(sqrt(pow(srcX-destX,2.0)+pow(srcY-destY,2.0)));
        }
        static double distanceCalc(double srcX, double srcY, double destX, double destY, int maxSpeed){
            double dist = sqrt(pow(srcX - destX,2.0)+pow(srcY - destY,2.0));
            return double(dist/maxSpeed);
        }

        static double getxCoord(adjListCollection &collection, int value){return collection.xCoord[value];}
        static double getyCoord(adjListCollection &collection, int value){return collection.yCoord[value];}

        static int getIntID(adjListCollection &collection, long long int value) {
            return collection.longIdToIntID.find(value)->second;
        }

        static long long int getLongID(adjListCollection &collection, int value) {
            return collection.intIdToLongID.find(value)->second;
        }
        static nodesAndWaysWrapper deserializeFromJson(string filePath){
            using namespace nlohmann;
            json j;
            ifstream jsonFile;
            //File path with appropriate json file created by java in graphBuilder writeToFileAsJsonMethod
            jsonFile.open(filePath);
            jsonFile >> j;
            auto wrapperClass = j.get<nodesAndWaysWrapper>();
            return wrapperClass;
        }
        static void createAdjListCollection(nodesAndWaysWrapper wrapper, adjListCollection &adjListCollection){
            auto ways = wrapper.getWays();
            for(map<long, customWay>::iterator it = ways.begin(); it != ways.end(); it++){
                long previousId = 0;
                vector<long>::const_iterator itr;
                vector<long> nodeIds = it->second.getNodeIdList();
                auto nodes = wrapper.getNodes();
                for(itr = nodeIds.cbegin(); itr < nodeIds.cend(); itr++){
                    if(*itr == nodeIds[0]){
                        previousId = *itr;
                    }
                    else{
                        customNode previousNode = nodes[previousId];
                        double prevX = previousNode.getLatitudeAsXCoord();
                        double prevY = previousNode.getLongtitudeAsYCoord();

                        customNode currentNode = nodes[*itr];
                        double currX = currentNode.getLatitudeAsXCoord();
                        double currY = currentNode.getLongtitudeAsYCoord();

                        double dist = distanceCalc(prevX, prevY, currX, currY);
                        addEdge(adjListCollection, previousId, *itr, dist);
                        addEdge(adjListCollection, *itr, previousId, dist);

                        previousId = *itr;
                    }
                }
            }
        }
    };
#endif //ADJACENCYLIST
