//
// Created by simon on 5/26/21.
//
#include "headers/util.h"
void randomPointsDijkstra(){
    util::randomPointsComparrisonSingle("malta", 1000, 1337, "dijkstra", "dijkstraDistance");
}

void randomPointsAstar(){
    util::randomPointsComparrisonSingle("malta", 1000, 1337, "astar", "dijkstraDistance");
}
void randomPointsLandmarks(){
    util::randomPointsComparrisonSingle("malta", 1000, 1337, "landmarks", "dijkstraDistance");
}

int main(){
    randomPointsLandmarks();
}

