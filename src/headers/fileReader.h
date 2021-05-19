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
    static void printStrVec(const vector<string> &input) {
        cout << "[";
        for (string i : input) {
            cout << i << ' ';
        }
        cout << "]";
    }

public:
    static int readEntireOsm(const string &pathToData, adjListCollection &adjCol) {
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
        int ways = stoi(line);
        for (int i = 0; i < nodes; ++i) {
            getline(data, line);
            long long sourceID = stoll(line);
            int source = adjacencyList::insertInMaps(adjCol, sourceID);
            getline(data, line);
            adjacencyList::addxCoord(adjCol, source, stod(line));
            getline(data, line);
            adjacencyList::addyCoord(adjCol, source, stod(line));
        }
        for (int i = 0; i < ways; ++i) {
            getline(data, line);
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> lineAsTokens(beg, end);
            int maxSpeed = stoi(lineAsTokens[0]);
            //Specifically ferryways have a speed of 68
            if (maxSpeed == 68) {
                maxSpeed = std::numeric_limits<int>::infinity();
            }
            int isOneway = stoi(lineAsTokens[1]);
            for (int j = 2; j < lineAsTokens.size() - 1; ++j) {
                int firstNode = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[j]));
                int secondNode = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[j + 1]));
                double srcX = adjacencyList::getxCoord(adjCol, firstNode);
                double srcY = adjacencyList::getyCoord(adjCol, firstNode);
                double destX = adjacencyList::getxCoord(adjCol, secondNode);
                double destY = adjacencyList::getyCoord(adjCol, secondNode);
                double weight = adjacencyList::distanceCalc(srcX, srcY, destX, destY,
                                                            maxSpeed); //adjacencyList::distanceCalc(srcX, srcY, destX, destY,maxSpeed);
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

    static vector<string> split (string s, string delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        string token;
        vector<string> res;

        while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
            token = s.substr (pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back (token);
        }

        res.push_back (s.substr (pos_start));
        return res;
    }

    static void readAdjFile(string pathToData, adjListCollection &adjCol) {
        //ios_base::sync_with_stdio(false);
        //cin.tie(NULL);
        string line;
        ifstream data(pathToData);
        if (!data.is_open()) {
            cout << "Error opening file"<< pathToData << endl;
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
            adjacencyList::addxCoord(adjCol, source, stod(line));
            getline(data, line);
            adjacencyList::addyCoord(adjCol, source, stod(line));
        }
        for (int j = 0; j < adjlines; ++j) {
            getline(data, line);
            string delimiter = " ";
            vector<string> lineAsTokens = split(line,delimiter);
            long long id = stoll(lineAsTokens[0]);
            int src = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[0]));
            int nodesConnectedTo = stoi(lineAsTokens[1]);
            lineAsTokens.erase(lineAsTokens.begin());
            lineAsTokens.erase(lineAsTokens.begin());
            for (int i = 0; i < nodesConnectedTo; ++i) {
                int indexVal= i*2;
                long long iddest=stoll(lineAsTokens[indexVal]);
                int dest = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[indexVal]));
                double dist = stod(lineAsTokens[indexVal+1]);
                adjacencyList::addEdge(adjCol,src,dest,dist);
            }
        }
        data.close();
    }
};

#endif //FILEREADER
