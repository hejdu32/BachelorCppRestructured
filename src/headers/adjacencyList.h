//
// Created by simon on 12-04-2021.
//

#ifndef ADJANCENCYLIST
#define ADJANCENCYLIST

#include <vector>
#include <map>
#include <utility>
#include <iostream>

using namespace std;

    struct adjListCollection {
        int idSoFar = 0;
        std::map<long long int, int> longIdToIntID{};
        std::map<int, long long int> intIdToLongID{};
        std::vector<std::vector<std::pair<int, double>>> adjlst{};
        std::map<int, double> euclidDistance;
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
                    collection.adjlst[source].push_back(make_pair(dest, weight));
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

        static void addEucildDist(adjListCollection &collection, int source, double euclidDist) {
            collection.euclidDistance.insert(make_pair(source, euclidDist));
        }

        static int getIntID(adjListCollection &collection, long long int value) {
            return collection.longIdToIntID.find(value)->second;
        }

        static long long int getLongID(adjListCollection &collection, int value) {
            return collection.intIdToLongID.find(value)->second;
        }
    };
#endif //ADJANCENCYLIST
