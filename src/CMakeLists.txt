cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES main.cpp
        headers/adjacencyList.h
        headers/aStar.h
        implementation/aStar.cpp
        headers/dijkstra.h
        implementation/dijkstra.cpp
        headers/fileReader.h
        headers/shortestPath.h)

add_executable(BachelorCppCmake ${SOURCES})