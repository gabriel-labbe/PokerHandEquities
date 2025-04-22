cmake_minimum_required(VERSION 3.10)

project(CardProject)

set(CMAKE_CXX_STANDARD 17)

# Grab all .cpp files in the current directory
file(GLOB SOURCES "*.cpp")

# Name the executable after the project (or anything else you like)
add_executable(CardProject ${SOURCES})
