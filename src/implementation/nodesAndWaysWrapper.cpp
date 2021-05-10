//
// Created by simon on 13-04-2021.
//
//#ifndef WRAPPER
//#define WRAPPER
//#include <vector>
//#include "customNode.cpp"
//#include "customWay.cpp"
//#include "../headerLibs/json.hpp"
//using namespace nlohmann;
//using namespace std;
//class nodesAndWaysWrapper {
//    private:
//        int magicNumber;
//        map<long, customNode> nodes;
//        map<long, customWay> ways;
//    public:
//
//    [[nodiscard]] int getMagicNumber() const {
//        return magicNumber;
//    }
//
//    [[nodiscard]] const map<long, customNode> &getNodes() const {
//        return nodes;
//    }
//
//    [[nodiscard]] const map<long, customWay> &getWays() const {
//        return ways;
//    }
//
//    NLOHMANN_DEFINE_TYPE_INTRUSIVE(nodesAndWaysWrapper, magicNumber, nodes, ways)
//};
//#endif //WRAPPER