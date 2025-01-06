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
)

find_package(OpenCV REQUIRED)
find_package(OpenSSL REQUIRED)

# Add the encoder executable
add_executable(encoder ${SOURCES} src/Encoder.cpp)
target_link_libraries(encoder ${OpenCV_LIBS} OpenSSL::SSL OpenSSL::Crypto)

# Add the decoder executable
add_executable(decoder ${SOURCES} src/Decoder.cpp)
target_link_libraries(decoder ${OpenCV_LIBS} OpenSSL::SSL OpenSSL::Crypto)