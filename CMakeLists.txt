cmake_minimum_required(VERSION 3.17)
project(BachelorCppCmake)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "-O3")
SET(CMAKE_C_FLAGS "-O3")

#SET(CMAKE_CXX_FLAGS "-Ot")
#SET(CMAKE_C_FLAGS  "-Ot")

#SET(CMAKE_CXX_FLAGS "-L")
#SET(CMAKE_C_FLAGS  "-L")

#SET(CMAKE_CXX_FLAGS "-funroll-loops")
#-DNDEBUG -DUNPACK

add_subdirectory(src)