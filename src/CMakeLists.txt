cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        headers/adjacencyList.h
        headers/aStar.h
        implementation/aStar.cpp
        headers/dijkstra.h
        implementation/dijkstra.cpp
        headers/landmarks.h
        implementation/landmarks.cpp
        headers/fileReader.h
        headers/shortestPath.h
        implementation/customNode.cpp
        implementation/customWay.cpp
        implementation/nodesAndWaysWrapper.cpp)

add_executable(BachelorCppCmake main.cpp ${SOURCES} headers/util.h)
add_executable(Tests tests.cpp ${SOURCES})
add_executable(stressTest stressTest.cpp ${SOURCES})