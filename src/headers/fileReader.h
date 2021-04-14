//
// Created by abg on 22-03-2021.
//

#ifndef FILEREADER
#define FILEREADER

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
    static void handleWays(vector<string> &lineAsTokens, adjListCollection &collection) {
        map<string, int> wayTypeToMaxSpeed ={
                {"unclassified", 1},
                {"tertiary_link", 1},
                {"primary_link", 1},
                {"tertiary", 1},
                {"living_street", 1},
                {"motorway_link", 1},
                {"trunk", 1},
                {"motorway", 1},
                {"secondary", 1},
                {"residential", 1},
                {"service", 1},
                {"track", 1},
                {"secondary_link", 1},
                {"trunk_link", 1},
                {"primary", 1},
                {"testroad", 1}
        };
        string typeOfWay = lineAsTokens[0].substr(1,lineAsTokens[0].size()-1);
        int maxSpeed = wayTypeToMaxSpeed[typeOfWay];
        long long srcID;
        long long destID;
        int source;
        int dest;
        double weight;
        int lineSize = lineAsTokens.size();
        for(int i=1; i < lineSize-1; i++){
            //cout << lineAsTokens[i] << endl;
            srcID = stoll(lineAsTokens[i]);
            destID = stoll(lineAsTokens[i+1]);
            source = adjacencyList::getIntID(collection,srcID);
            dest = adjacencyList::getIntID(collection,destID);
            double srcX = adjacencyList::getxCoord(collection, source); double srcY =  adjacencyList::getyCoord(collection, source);
            double destX =  adjacencyList::getxCoord(collection,dest); double  destY =  adjacencyList::getyCoord(collection,dest);

            weight = adjacencyList::distanceCalc(srcX,srcY,destX,destY,maxSpeed);
            //true if the road is a two way road
            if(true){
                adjacencyList::addEdge(collection,source,dest,weight);
                adjacencyList::addEdge(collection,dest,source,weight);
            } else {
            adjacencyList::addEdge(collection,source,dest,weight);
            }

        }

}
    static void printStrVec(const vector<string> &input){
        cout << "[";
        for (string i : input) {
            cout << i << ' ';
        }
        cout << "]";
    }

public:
    static int readFile(string path, adjListCollection &adjListCollection) {
        string line;
        ifstream myfile(path);

        if (myfile.fail()) {
            throw runtime_error("Could not open file");
        }

        int counter = 0;
        while (getline(myfile, line)) {
            counter++;
            if (counter % 100000 == 0){
                cout<< "counter: " << counter <<endl;
            }
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> lineAsTokens(beg, end);

            int source;
            double xCoord;
            double yCoord;
            long long sourceID;
            if (lineAsTokens[0][0]=='#'){
                sourceID = stoll(lineAsTokens[0].substr(1,lineAsTokens[0].size()-1));
                source = adjacencyList::insertInMaps(adjListCollection, sourceID);

                xCoord = stod(lineAsTokens[1]);
                adjacencyList::addxCoord(adjListCollection, source, xCoord);

                yCoord = stod(lineAsTokens[2]);
                adjacencyList::addyCoord(adjListCollection, source, yCoord);
            } else if(lineAsTokens[0][0]==';'){
                handleWays(lineAsTokens, adjListCollection);
            } else if (lineAsTokens[0][0] == '!'){
                //cout << "eof";
                myfile.close();
                return 0;
            }
        }
        myfile.close();
        return 0;
    }

};


#endif //FILEREADER
