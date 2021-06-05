cmake_minimum_required(VERSION 3.17)
project(BachelorCppCmake)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "-O2")
SET(CMAKE_C_FLAGS "-O2")
#SET(CMAKE_CXX_FLAGS "-ggdb")
#SET(CMAKE_C_FLAGS "-ggdb")
SET(CMAKE_CXX_FLAGS "-fp:fast")
SET(CMAKE_C_FLAGS "-fp:fast")


add_subdirectory(src)