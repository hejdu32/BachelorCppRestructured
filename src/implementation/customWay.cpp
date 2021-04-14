//
// Created by simon on 13-04-2021.
//
#ifndef CUSTOMWAY
#define CUSTOMWAY

#include <vector>
#include "../headerLibs/json.hpp"
using namespace std;
using namespace nlohmann;
class customWay{
    private:
    long id;
    std::vector<long> nodeIdList;
    string tagId;
    string maxSpeed;
    public:

    [[nodiscard]] long getId() const {
        return id;
    }

    [[nodiscard]] const vector<long> &getNodeIdList() const {
        return nodeIdList;
    }

    [[nodiscard]] const string &getTagId() const {
        return tagId;
    }

    [[nodiscard]] const string &getMaxSpeed() const {
        return maxSpeed;
    }

    string toString(){
        string toPrint = "Id: " + to_string(id) + "\n";
        toPrint += "NodeIdList: ";
        for(long l: nodeIdList){
            toPrint += to_string(l) + ", ";
        }
        toPrint += "\n";
        toPrint += "TagId: " + tagId;
        return toPrint;
    }
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(customWay, id, nodeIdList, tagId)
};
#endif //CUSTOMWAY

