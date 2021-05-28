//
// Created by simon on 5/26/21.
//
#include <utility>

#include "headers/util.h"
void randomPointsDijkstra(){
    util::randomPointsComparrisonSingle("malta", 1000, 1337, "dijkstra", "dijkstraDistance");
}

void randomPointsAstar(){
    util::randomPointsComparrisonSingle("malta", 1000, 1337, "astar", "dijkstraDistance");
}
void randomPointsLandmarks(){
    util::randomPointsComparrisonSingle("malta", 1000, 1337, "landmarks", "euclidDistance");
}

void writeRandomPointsToFile(const string& country, int amountOfTests, int seed, const string& algorithm, string landmarkSelection){
    util::randomPointsComparrisonSingleToFile(country, amountOfTests, seed, algorithm, std::move(landmarkSelection));
}

int main(){
    writeRandomPointsToFile("malta", 1000, 1337, "dijkstra", "dijkstraDistance");
    writeRandomPointsToFile("malta", 1000, 1337, "astar", "dijkstraDistance");
    writeRandomPointsToFile("malta", 1000, 1337, "landmarks", "dijkstraDistance");
}

