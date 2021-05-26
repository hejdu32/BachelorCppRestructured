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
        headers/util.h
        implementation/customNode.cpp
        implementation/customWay.cpp
        implementation/nodesAndWaysWrapper.cpp)

add_executable(BachelorCppCmake main.cpp ${SOURCES})
add_executable(Tests tests.cpp ${SOURCES})
add_executable(stressTest stressTest.cpp ${SOURCES})
add_executable(testsForAnalysis testsForAnalysis.cpp ${SOURCES})