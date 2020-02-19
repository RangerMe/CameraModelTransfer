cmake_minimum_required(VERSION 2.8)
project(CamTransfer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -o0 -Wall -g -ggdb")

set(DIR_SRC src)
set(DIR_INCLUDE include)

INCLUDE_DIRECTORIES(${DIR_INCLUDE})
file(GLOB FILE_SRC ${DIR_SRC}/*.cpp)

add_executable(CamTransfer ${FILE_SRC})