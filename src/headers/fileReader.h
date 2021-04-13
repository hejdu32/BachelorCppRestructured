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

class fileReader {
public:
    static int readFile(string path, adjListCollection &adjListCollection) {
        adjacencyList listMutator;

        string line;
        ifstream myfile(path);
        if (myfile.is_open()) {
            while (getline(myfile, line)) {
                if(line =="!"){
                    //cout << "eof";
                    myfile.close();
                    return 0;
                }
                istringstream buf(line);
                istream_iterator<string> beg(buf), end;
                vector<string> lineAsTokens(beg, end);

                int source;
                int dest;
                double xCoord;
                double yCoord;
                for(auto& value: lineAsTokens){
                    //cout << value << "\n";
                    char firstChar = value[0];
                    if(firstChar == '#'){
                        long long int sourceID = stoll(value.substr(1,value.size()-1));
                        source = listMutator.insertInMaps(adjListCollection, sourceID);
                    } else if(firstChar == '^') {
                        xCoord = stod(value.substr(1,value.size()-1));
                        listMutator.addxCoord(adjListCollection, source, xCoord);
                    } else if(firstChar == ','){
                        yCoord = stod(value.substr(1,value.size()-1));
                        listMutator.addyCoord(adjListCollection, source, yCoord);
                    } else if(firstChar == ';'){
                        string typeOfWay = value.substr(1,value.size()-1);
                        long long destID = stoll(value.substr(1,value.size()-1));
                        dest = listMutator.insertInMaps(adjListCollection,destID);
                        //do math
                        double srcX = listMutator.getxCoord(adjListCollection, source); double srcY = listMutator.getyCoord(adjListCollection, source);
                        double destX = listMutator.getxCoord(adjListCollection,dest); double  destY = listMutator.getyCoord(adjListCollection,dest);

                        double weight =listMutator.distanceCalc(srcX,srcY,destX,destY);
                        listMutator.addEdge(adjListCollection,source,dest,weight);
                    }
                }
            }
            myfile.close();
        } else cout << "Error reading file\n";
        return 0;
    }
};


#endif //FILEREADER
