cmake_minimum_required(VERSION 3.25)

project(lab2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME}
        src/Argument.cpp
        src/main.cpp
        src/Random.cpp
        src/MpiSquareMatrix.cpp
)

find_package(MPI REQUIRED)

add_compile_options(
        -Werror

        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
)

target_link_libraries(${PROJECT_NAME} MPI::MPI_CXX)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
