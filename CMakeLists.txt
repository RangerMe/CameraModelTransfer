# Set minimum cmake version
cmake_minimum_required(VERSION 2.8)
# Set Project name
project(CamTransfer)

# Set log detail level
# LOG_LEVEL = 0: print nothing
#           = 1: print result only
#           = 2: print everything
add_definitions(-DLOG_LEVEL=2)

# Set Debug mode
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -o0 -Wall -g -ggdb")

# Set directorys to include files and source files
set(DIR_SRC src)
set(DIR_INCLUDE include)

INCLUDE_DIRECTORIES(${DIR_INCLUDE})
file(GLOB FILE_SRC ${DIR_SRC}/*.cpp)

# add executable
add_executable(CamTransfer ${FILE_SRC})