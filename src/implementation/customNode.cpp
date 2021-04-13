//
// Created by simon on 13-04-2021.
//

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
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(customNode, id, latitudeAsXCoord, longtitudeAsYCoord)
        string toString() const{
            return "Id: " + to_string(id) + " XCoord: " + to_string(latitudeAsXCoord) + " YCoord: " + to_string(longtitudeAsYCoord);
        }
};
