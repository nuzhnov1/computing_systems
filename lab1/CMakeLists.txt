cmake_minimum_required(VERSION 3.25)

project(lab1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(TBB_DIR "/opt/intel/oneapi/tbb/latest/lib/cmake/tbb")

add_executable(${PROJECT_NAME}
        src/Argument.cpp
        src/main.cpp
        src/OmpSquareMatrix.cpp
        src/PthreadSquareMatrix.cpp
        src/Random.cpp
        src/SequenceSquareMatrix.cpp
        src/SquareMatrix.cpp
        src/TbbSquareMatrix.cpp
)

find_package(OpenMP REQUIRED)
find_package(Threads REQUIRED)
find_package(TBB REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE TBB::tbb)

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

include_directories(${PROJECT_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
