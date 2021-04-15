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
    static int readFile(string path, adjListCollection &adjCol) {
        double inf = std::numeric_limits<double>::infinity();
        string line;
        ifstream myfile(path);

        if (myfile.fail()) {
            throw runtime_error("Could not open file");
        }

        while (getline(myfile, line)) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> lineAsTokens(beg, end);

            if (lineAsTokens[0][0]=='#'){
                long long sourceID = stoll(lineAsTokens[0].substr(1,lineAsTokens[0].size()-1));
                int src = adjacencyList::insertInMaps(adjCol, sourceID);

                double xCoord = stod(lineAsTokens[1]);
                adjacencyList::addxCoord(adjCol, src, xCoord);

                double yCoord = stod(lineAsTokens[2]);
                adjacencyList::addyCoord(adjCol, src, yCoord);
            } else if(lineAsTokens[0][0]==';'){
                string typeOfWay = lineAsTokens[0].substr(1,lineAsTokens[0].size()-1);
                int maxSpeed = stoi(typeOfWay);

                int loopMax = lineAsTokens.size() -1;
                for(int i=1; i < loopMax; i++){
                    long long srcID = stoll(lineAsTokens[i]);
                    long long destID = stoll(lineAsTokens[i+1]);
                    int source = adjacencyList::getIntID(adjCol, srcID);
                    int dest = adjacencyList::getIntID(adjCol, destID);
                    double srcX = adjacencyList::getxCoord(adjCol, source); double srcY =  adjacencyList::getyCoord(adjCol, source);
                    double destX =  adjacencyList::getxCoord(adjCol, dest); double  destY =  adjacencyList::getyCoord(adjCol, dest);

                    //double weight = adjacencyList::distanceCalc(srcX,srcY,destX,destY,maxSpeed);
                    double weight = adjacencyList::euclidDistance(srcX,srcY,destX,destY);
                    if(isinf(weight)){
                        cout << "inf at road with max speed: " << maxSpeed <<endl;
                        cout<< "srcID: " << srcID << " x: " << srcX << " y: " << srcY << endl;
                        cout<< "destID:" << destID << " x: " << destX << " y: " << destY << endl;
                    }
                    //needs implementation for oneway roads
                    adjacencyList::addEdge(adjCol, source, dest, weight);
                    adjacencyList::addEdge(adjCol, dest, source, weight);
                }

            } else if (lineAsTokens[0][0] == '!'){
                //cout << "eof";
                myfile.close();
                return 0;
            }
        }
        myfile.close();
        return 0;
    }


    //OLD METHOD NOT USED
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
            for (int j = 1; j < lineAsTokens.size()-1; ++j) {
                int firstNode = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[j]));
                int secondNode = adjacencyList::getIntID(adjCol, stoll(lineAsTokens[j+1]));
                double srcX = adjacencyList::getxCoord(adjCol, firstNode);
                double srcY = adjacencyList::getyCoord(adjCol, firstNode);
                double destX = adjacencyList::getxCoord(adjCol, secondNode);
                double destY = adjacencyList::getyCoord(adjCol, secondNode);
                double weight = adjacencyList::distanceCalc(srcX, srcY, destX, destY,maxSpeed);
                adjacencyList::addEdge(adjCol, firstNode, secondNode, weight);
                adjacencyList::addEdge(adjCol, secondNode, firstNode, weight);
            }
        }
        myfile.close();
        return 0;
    }

};




#endif //FILEREADER
