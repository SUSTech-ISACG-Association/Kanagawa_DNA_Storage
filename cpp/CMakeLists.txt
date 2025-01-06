cmake_minimum_required(VERSION 3.10)

# Project name
project(Kanagawa_DNA_Storage)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/GaloisField.cpp
    src/Polynomial.cpp
    src/ReedSolomon.cpp
    src/Kanagawa.cpp
)

# OpenCV package
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(TestReedSolomon ${SOURCES})

# Link OpenCV libraries
target_link_libraries(TestReedSolomon ${OpenCV_LIBS})