//
// Created by simon on 13-04-2021.
//
#ifndef CUSTOMNODE
#define CUSTOMNODE
#include <string>
#include "../headerLibs/json.hpp"

using namespace std;
using namespace nlohmann;
class customNode {
private:
    long id;
    double latitudeAsXCoord;
    double longtitudeAsYCoord;

public:
    [[nodiscard]] long getId() const {
        return id;
    }

    [[nodiscard]] double getLatitudeAsXCoord() const {
        return latitudeAsXCoord;
    }

    [[nodiscard]] double getLongtitudeAsYCoord() const {
        return longtitudeAsYCoord;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(customNode, id, latitudeAsXCoord, longtitudeAsYCoord)
    [[nodiscard]] string toString() const{
        return "Id: " + to_string(id) + " XCoord: " + to_string(latitudeAsXCoord) + " YCoord: " + to_string(longtitudeAsYCoord);
    }
};
#endif //CUSTOMNODE
