//
// Created by simon on 12-04-2021.
//

#ifndef SHORTESTPATH
#define SHORTESTPATH

#include "fileReader.h"
#include "dijkstra.h"
#include "aStar.h"
#include "landmarks.h"
#include <sstream>
#include <algorithm>


static map<std::string, int> spmap={{"dijkstra", 0},{"astar", 1},{"landmarks", 2}};

class shortestPath {
public:
    //wrapper method used for easy calling a shortest path problem with the desired algorithm as argument using spmap
    static spResultStruct chooseAlgo(int method, int source, int dest, adjListCollection &adjListCollection) {
        //dijkstra=0,astar=1 ect
        switch (method) {
            case 0:
                return dijkstra::djikstraShortestPath(source,dest,true,adjListCollection);
            case 1:
                return aStar::aStarShortestPath(source,dest,adjListCollection);
            case 2:
                return landmarks::ALTShortestPath(source,dest,adjListCollection);
            default:
                cout << "ERROR METHOD NOT FOUND" << endl;
                spResultStruct emptystruct;
                return emptystruct;
        }
    }
    //creates the adjacency list from either file or from java
    static int createAdjacencyList(string path, const string& method, adjListCollection &adjCol) {
        if (method == "file") {//reads a non reduced file from Java with all ways and nodes
            fileReader::readEntireOsm(std::move(path), adjCol);
        }else if (method == "reduced") {//reads a reduced adjaceny list created by Java
            fileReader::readAdjFile(path, adjCol);
        } else if (method == "java") {  //deprecated method used for reading the parsed OSM data from Java
            string line;
            bool reading = true;
            while (reading) {
                getline(cin, line);
                istringstream buf(line);
                istream_iterator<string> beg(buf), end;
                vector<string> lineAsTokens(beg, end);

                int source;
                int dest;
                float weight;
                for (auto &value: lineAsTokens) {
                    char firstChar = value[0];
                    if (firstChar == '#') {
                        long long sourceID = stoll(value.substr(1, value.size() - 1));
                        source = adjacencyList::insertInMaps(adjCol, sourceID);
                    } else if (firstChar == ';') {
                        long long destID = stoll(value.substr(1, value.size() - 1));
                        dest = adjacencyList::insertInMaps(adjCol, destID);
                    } else if (firstChar == ',') {
                        weight = stod(value.substr(1, value.size() - 1));
                        adjacencyList::addEdge(adjCol, source, dest, weight);
                    } else if (firstChar == '!') {
                        cout << "Finished making adjencency list in cpp" << endl;
                        reading = false;
                    }
                }
            }
        }
        return 0;
    }

};
#endif //SHORTESTPATH
