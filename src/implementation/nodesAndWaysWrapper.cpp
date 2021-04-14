//
// Created by simon on 13-04-2021.
//
#ifndef WRAPPER
#define WRAPPER
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

    [[nodiscard]] int getMagicNumber() const {
        return magicNumber;
    }

    [[nodiscard]] const vector<customNode> &getNodes() const {
        return nodes;
    }

    [[nodiscard]] const vector<customWay> &getWays() const {
        return ways;
    }

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
#endif //WRAPPER