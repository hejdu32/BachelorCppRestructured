//
// Created by simon on 5/26/21.
//
#include <utility>

#include "headers/util.h"
void randomPointsDijkstra(){
    util::randomPointsComparrisonSingle("denmark", 1000, 1337, "dijkstra", "dijkstraDistance");
}

void randomPointsAstar(){
    util::randomPointsComparrisonSingle("denmark", 1000, 1337, "astar", "dijkstraDistance");
}
void randomPointsLandmarks(){
    util::randomPointsComparrisonSingle("denmark", 1000, 1337, "landmarks", "dijkstraDistance");
}

void writeRandomPointsToFile(const string& country, int amountOfTests, int seed, const string& algorithm, const string& landmarkSelection){
    util::randomPointsComparrisonSingleToFile(country, amountOfTests, seed, algorithm, landmarkSelection);
}

int main(){
    writeRandomPointsToFile("denmark", 1000, 1337, "landmarks", "dijkstraDistance");
    //writeRandomPointsToFile("denmark", 10000, 1337, "dijkstra", "dijkstraDistance");
    //writeRandomPointsToFile("denmark", 10000, 1337, "astar", "dijkstraDistance");

}

