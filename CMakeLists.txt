cmake_minimum_required(VERSION 3.10)
project(PokerEquityCalculator)

set(CMAKE_CXX_STANDARD 17)

# Include headers
include_directories(include)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(PokerEquityCalculator ${SOURCES})
