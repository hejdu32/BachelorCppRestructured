//
// Created by abg on 22-03-2021.
//

#ifndef FILEREADER
#define FILEREADER

#include "fileReader.h"
#include "adjacencyList.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>


using namespace std;


class fileReader{
private:
    static void printStrVec(const vector<string> &input){
        cout << "[";
        for (string i : input) {
            cout << i << ' ';
        }
        cout << "]";
    }

public:
    static int readAdjFile(string path, adjListCollection &adjCol) {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        string line;
        ifstream myfile(path);

        getline(myfile,line);
        int nodes= stoi(line);
        getline(myfile,line);
        int ways = stoi(line);
        for (int i = 0; i < nodes; ++i) {
            getline(myfile, line);
            long long sourceID = stoll(line);
            int source = adjacencyList::insertInMaps(adjCol, sourceID);
            getline(myfile, line);
            adjacencyList::addxCoord(adjCol, source, stod(line));
            getline(myfile, line);
            adjacencyList::addyCoord(adjCol, source, stod(line));
        }
        for (int i = 0; i < ways; ++i) {
            getline(myfile,line);
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> lineAsTokens(beg, end);
            int maxSpeed = stoi(lineAsTokens[0]);
            int isOneway = stoi(lineAsTokens[1]);
            for (int j = 2; j < lineAsTokens.size()-1; ++j) {
                int firstNode = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[j]));
                int secondNode = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[j+1]));
                double srcX = adjacencyList::getxCoord(adjCol, firstNode);
                double srcY = adjacencyList::getyCoord(adjCol, firstNode);
                double destX = adjacencyList::getxCoord(adjCol, secondNode);
                double destY = adjacencyList::getyCoord(adjCol, secondNode);
                double weight = adjacencyList::distanceCalc(srcX, srcY, destX, destY,maxSpeed);
                if(isOneway) {
                    adjacencyList::addEdge(adjCol, firstNode, secondNode, weight);
                }
                else {
                    adjacencyList::addEdge(adjCol, firstNode, secondNode, weight);
                    adjacencyList::addEdge(adjCol, secondNode, firstNode, weight);
                }
            }
        }
        myfile.close();
        return 0;
    }

};




#endif //FILEREADER
