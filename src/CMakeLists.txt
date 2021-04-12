cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES main.cpp
            adjacencyList.h
            aStar.h
            aStar.cpp
            dijkstra.h
            dijkstra.cpp
            fileReader.h
            shortestPath.h)

add_executable(BachelorCppCmake ${SOURCES})