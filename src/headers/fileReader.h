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
    static int readFile(string path, adjListCollection &adjListCollection) {
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
                int src = adjacencyList::insertInMaps(adjListCollection, sourceID);

                double xCoord = stod(lineAsTokens[1]);
                adjacencyList::addxCoord(adjListCollection, src, xCoord);

                double yCoord = stod(lineAsTokens[2]);
                adjacencyList::addyCoord(adjListCollection, src, yCoord);
            } else if(lineAsTokens[0][0]==';'){
                string typeOfWay = lineAsTokens[0].substr(1,lineAsTokens[0].size()-1);
                int maxSpeed = stoi(typeOfWay);

                int loopMax = lineAsTokens.size() -1;
                for(int i=1; i < loopMax; i++){
                    long long srcID = stoll(lineAsTokens[i]);
                    long long destID = stoll(lineAsTokens[i+1]);
                    int source = adjacencyList::getIntID(adjListCollection,srcID);
                    int dest = adjacencyList::getIntID(adjListCollection,destID);
                    double srcX = adjacencyList::getxCoord(adjListCollection, source); double srcY =  adjacencyList::getyCoord(adjListCollection, source);
                    double destX =  adjacencyList::getxCoord(adjListCollection,dest); double  destY =  adjacencyList::getyCoord(adjListCollection,dest);

                    //double weight = adjacencyList::distanceCalc(srcX,srcY,destX,destY,maxSpeed);
                    double weight = adjacencyList::euclidDistance(srcX,srcY,destX,destY);
                    if(isinf(weight)){
                        cout << "inf at road with max speed: " << maxSpeed <<endl;
                        cout<< "srcID: " << srcID << " x: " << srcX << " y: " << srcY << endl;
                        cout<< "destID:" << destID << " x: " << destX << " y: " << destY << endl;
                    }

                    //needs implementation for oneway roads
                    adjacencyList::addEdge(adjListCollection,source,dest,weight);
                    adjacencyList::addEdge(adjListCollection,dest,source,weight);


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
    static int readAdjFile(string path, adjListCollection &adjListCollection) {
        string line;
        ifstream myfile(path);

        if (myfile.fail()) {
            throw runtime_error("Could not open file");
        }
        int counter = 0;
        while (getline(myfile, line)) {
            counter++;
            if (counter % 1000000 == 0) {
                cout << "counter: " << counter << endl;
            }
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> lineAsTokens(beg, end);

            int source;
            int dest;
            double xCoord;
            double yCoord;
            long long sourceID;
            long long destID;
            for (auto &value: lineAsTokens) {
                char firstChar = value[0];
                if (firstChar == '#') {
                    sourceID = stoll(value.substr(1, value.size() - 1));
                    source = adjacencyList::insertInMaps(adjListCollection, sourceID);
                } else if (firstChar == '^') {
                    xCoord = stod(value.substr(1, value.size() - 1));
                    adjacencyList::addxCoord(adjListCollection, source, xCoord);
                } else if (firstChar == ',') {
                    yCoord = stod(value.substr(1, value.size() - 1));
                    adjacencyList::addyCoord(adjListCollection, source, yCoord);
                } else if (firstChar == ';') {
                    destID = stoll(value.substr(1, value.size() - 1));
                    source = adjacencyList::getIntID(adjListCollection, destID);
                    dest = adjacencyList::getIntID(adjListCollection, destID);
                    double srcX = adjacencyList::getxCoord(adjListCollection, source);
                    double srcY = adjacencyList::getyCoord(adjListCollection, source);
                    double destX = adjacencyList::getxCoord(adjListCollection, dest);
                    double destY = adjacencyList::getyCoord(adjListCollection, dest);

                    double weight = adjacencyList::euclidDistance(srcX, srcY, destX, destY);
                    adjacencyList::addEdge(adjListCollection, source, dest, weight);
                } else if (firstChar == '%') {
                    //cout<<"we here" << endl;
                    break;
                }else if (firstChar == '!') {
                    myfile.close();
                    return 0;
                }
            }
        }
        myfile.close();
        return 0;
    }

};




#endif //FILEREADER
