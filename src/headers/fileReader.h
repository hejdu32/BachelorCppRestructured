//
// Created by abg on 22-03-2021.
//

#ifndef FILEREADER
#define FILEREADER

#include "adjacencyList.h"
#include "shortestPath.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>


using namespace std;


class fileReader {
private:
    //pretty printing method
    static void printStrVec(const vector<string> &input) {
        cout << "[";
        for (string i : input) {
            cout << i << ' ';
        }
        cout << "]";
    }

public:
    //reads the parsed file from Java containing nodes, and ways and adds these to the adjListCollection
    static int readEntireOsm(const string &pathToData, adjListCollection &adjCol) {
        string line;
        ifstream data(pathToData);
        if (!data.is_open()) {
            cout << "Error opening file" << endl;
        }
        getline(data, line);
        int nodes = stoi(line);
        getline(data, line);
        int ways = stoi(line);
        for (int i = 0; i < nodes; ++i) {
            getline(data, line);
            long long sourceID = stoll(line);
            int source = adjacencyList::insertInMaps(adjCol, sourceID);
            getline(data, line);
            adjacencyList::addxCoord(adjCol, source, stof(line));
            getline(data, line);
            adjacencyList::addyCoord(adjCol, source, stof(line));
        }
        for (int i = 0; i < ways; ++i) {
            getline(data, line);
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> lineAsTokens(beg, end);
            int maxSpeed = stoi(lineAsTokens[0]);
            int isOneway = stoi(lineAsTokens[1]);
            for (int j = 2; j < lineAsTokens.size() - 1; ++j) {
                int firstNode = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[j]));
                int secondNode = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[j + 1]));
                float srcX = adjacencyList::getxCoord(adjCol, firstNode);
                float srcY = adjacencyList::getyCoord(adjCol, firstNode);
                float destX = adjacencyList::getxCoord(adjCol, secondNode);
                float destY = adjacencyList::getyCoord(adjCol, secondNode);
                float weight = adjacencyList::distanceCalc(srcX, srcY, destX, destY,maxSpeed); //adjacencyList::distanceCalc(srcX, srcY, destX, destY,maxSpeed);
                if (isOneway) {
                    adjacencyList::addEdge(adjCol, firstNode, secondNode, weight);
                } else {
                    adjacencyList::addEdge(adjCol, firstNode, secondNode, weight);
                    adjacencyList::addEdge(adjCol, secondNode, firstNode, weight);
                }
            }
        }
        data.close();
        return 0;
    }
    static void printVec(vector<string>const &input){
        cout << "[";
        for (string i : input) {
            cout << i << ' ';
        }
        cout << "]";
    }
    //deprecated method used for reading the reduced adjlist
    static int readAdjFile(const string &pathToData, adjListCollection &adjCol) {
        //ios_base::sync_with_stdio(false);
        //cin.tie(NULL);
        string line;
        ifstream data(pathToData);
        if (!data.is_open()) {
            cout << "Error opening file" << endl;
        }
        getline(data, line);
        int nodes = stoi(line);
        getline(data, line);
        int adjlines = stoi(line);
        for (int i = 0; i < nodes; ++i) {
            getline(data, line);
            long long sourceID = stoll(line);
            int source = adjacencyList::insertInMaps(adjCol, sourceID);
            getline(data, line);
            adjacencyList::addxCoord(adjCol, source, stof(line));
            getline(data, line);
            adjacencyList::addyCoord(adjCol, source, stof(line));
        }
        cout << "we here"<< endl;
        for (int j = 0; j < adjlines; ++j) {
            if (j >107120){
                cout<< "almost at the end of file "<< j << endl;
            }
            getline(data, line);
            if (j >107120){
                cout << line <<endl;
            }
            if (line == "!"){break;}
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> lineAsTokens(beg, end);
            int src = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[0]));
            int nodesConnectedTo = stoi(lineAsTokens[1]);
            //remove the source and nodes connected to for easier looping
            lineAsTokens.erase(lineAsTokens.begin());
            lineAsTokens.erase(lineAsTokens.begin());
            for (int k = 0; k < nodesConnectedTo; ++k) {
                int idxVal = k*2;
                int dest = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[idxVal]));
                float distance = stof(lineAsTokens[idxVal + 1]);
                adjacencyList::addEdge(adjCol, src, dest, distance);
            }
        }
        cout << "Closing file" << endl;
        data.close();
        return 0;
    }
};




#endif //FILEREADER
