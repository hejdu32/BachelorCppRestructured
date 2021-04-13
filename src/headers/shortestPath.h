//
// Created by simon on 12-04-2021.
//

#ifndef SHORTESTPATH
#define SHORTESTPATH

#include <sstream>
#include "dijkstra.h"
#include "aStar.h"
#include "fileReader.h"

enum shortestPathMethods{
    dijkstra = 0,
    astar = 1
};

class shortestPath {

public:
    static tuple<double, vector<int>> chooseAlgo(int method, int source, int dest, adjListCollection &adjListCollection) {
        //dijkstra=0,astar=1 ect
        switch (method) {
            case 0:
                return dijkstra::djikstraShortestPath(source,dest,adjListCollection);
            case 1:
                return aStar::aStarShortestPath(source,dest,adjListCollection);
            default:
                cout << "case Unknown";
        }
    }

    static int createAdjacencyList(string path, string method, adjListCollection &adjListCollection) {
        if (method == "file") {
            fileReader::readFile(std::move(path), adjListCollection);
        } else if (method == "java") {
            string line;
            bool reading = true;
            while (reading) {
                getline(cin, line);
                istringstream buf(line);
                istream_iterator<string> beg(buf), end;
                vector<string> lineAsTokens(beg, end);

                int source;
                int dest;
                double weight;
                for (auto &value: lineAsTokens) {
                    char firstChar = value[0];
                    if (firstChar == '#') {
                        long long int sourceID = stoll(value.substr(1, value.size() - 1));
                        source = adjacencyList::insertInMaps(adjListCollection, sourceID);
                    } else if (firstChar == ';') {
                        long long int destID = stoll(value.substr(1, value.size() - 1));
                        dest = adjacencyList::insertInMaps(adjListCollection, destID);
                    } else if (firstChar == ',') {
                        weight = stod(value.substr(1, value.size() - 1));
                        adjacencyList::addEdge(adjListCollection, source, dest, weight);
                    } else if (firstChar == '!') {
                        cout << "Finished making adjencency list in cpp" << endl;
                        reading = false;
                    }
                }
            }
        }
        return 0;
    }

    static void printVec(vector<long long>const &input){
        cout << "[";
        for (long long i : input) {
            cout << i << ' ';
        }
        cout << "]";
    }
};
#endif //SHORTESTPATH
