cmake_minimum_required(VERSION 3.0)
project(RK_Projet_v1)
set(OUTPUT_NAME RK_Projet_v1)

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

file(GLOB_RECURSE HEADER_FILES *.hpp ../utils/*.hpp)
file(GLOB_RECURSE SRC_FILES *.cpp ../utils/*.cpp)
pkg_check_modules(PCKG REQUIRED IMPORTED_TARGET opencv)

set(LIBS "pkg-config opencv --libs")
set(COMP_OPTS "pkg-config opencv --cflags")

add_executable(${OUTPUT_NAME} ${SRC_FILES} ${HEADER_FILES})
target_compile_options(${OUTPUT_NAME} PRIVATE -Wall -fopenmp -std=c++14)
target_link_libraries(${OUTPUT_NAME} PUBLIC PkgConfig::PCKG)
