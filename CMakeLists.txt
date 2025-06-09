cmake_minimum_required(VERSION 3.10)
project(PokerEquityCalculator)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Include headers
include_directories(include)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(PokerEquityCalculator ${SOURCES})
if(OpenMP_CXX_FOUND)
    target_link_libraries(PokerEquityCalculator OpenMP::OpenMP_CXX)
endif()
