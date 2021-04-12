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
                double weight;
                double euclidDist;
                for(auto& value: lineAsTokens){
                    char firstChar = value[0];
                    if(firstChar == '#'){
                        long long int sourceID = stoll(value.substr(1,value.size()-1));
                        source = listMutator.insertInMaps(adjListCollection, sourceID);
                        //cout << source << '\n';
                    }else if (firstChar == '*'){
                        euclidDist = stod(value.substr(1,value.size()-1));
                        listMutator.addEucildDist(adjListCollection, source, euclidDist);
                    }else if (firstChar == ';'){
                        long long int destID = stoll(value.substr(1,value.size()-1));
                        dest = listMutator.insertInMaps(adjListCollection, destID);
                        //cout << dest << '\n';
                    } else if (firstChar == ','){
                        weight = stod(value.substr(1,value.size()-1));
                        //cout << "adj pair: "<< source << "->" << dest << "," << weight << " \n";
                        listMutator.addEdge(adjListCollection,source, dest,weight);
                    }
                    //cout << "outside of statements??" << "\n";
                    //cout << value;
                }

            }
            myfile.close();
        } else cout << "Error reading file\n";
        return 0;
    }
};


#endif //FILEREADER
