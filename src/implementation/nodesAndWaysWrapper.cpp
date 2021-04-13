//
// Created by simon on 13-04-2021.
//
#include <vector>
#include "customNode.cpp"
#include "customWay.cpp"
#include "../headerLibs/json.hpp"
using namespace nlohmann;
using namespace std;
class nodesAndWaysWrapper {
    private:
        int magicNumber;
        vector<customNode> nodes;
        vector<customWay> ways;
    public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(nodesAndWaysWrapper, magicNumber, nodes, ways)
    string toString(){
        string toPrint = "Magic Number: " + to_string(magicNumber) + "\n";
        toPrint += "Nodes: \n";
        for(customNode node: nodes) {
            toPrint += node.toString() + "\n";
        }
        for(customWay way: ways){
            toPrint += way.toString() + "\n";
        }
        return toPrint;
    }
};
