cmake_minimum_required(VERSION 3.29)
project(tp3-bottin-CerberusX21)

set(CMAKE_CXX_STANDARD 14)

# Source and header files for the library
set(SOURCES
        Bottin.cpp
        ContratException.cpp
)

set(HEADERS
        Bottin.h
        ContratException.h
        FoncteurHachage.hpp
        TableHachage.hpp
)

# Create a library for the main project
add_library(tp3_bottin_lib ${SOURCES} ${HEADERS})

# Main executable
add_executable(tp3-bottin-CerberusX21 main.cpp)
target_link_libraries(tp3-bottin-CerberusX21 PRIVATE tp3_bottin_lib)

# Fetch and enable GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.12.x
)

FetchContent_MakeAvailable(googletest)
enable_testing()

# Add the test subdirectory
add_subdirectory(Test)
