cmake_minimum_required(VERSION 3.13)
project(RK_Projet)
set(OUTPUT_NAME RK_Projet)

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

file(GLOB_RECURSE HEADER_FILES src/*.hpp)
file(GLOB_RECURSE SRC_FILES src/*.cpp)
pkg_check_modules(PCKG REQUIRED IMPORTED_TARGET opencv)

set(LIBS "pkg-config opencv --libs")
set(COMP_OPTS "pkg-config opencv --cflags")

add_executable(${OUTPUT_NAME} ${SRC_FILES} RK_Sabre.cpp ${HEADER_FILES})
target_compile_options(${OUTPUT_NAME} PRIVATE -Wall -fopenmp)
target_link_libraries(${OUTPUT_NAME} PUBLIC PkgConfig::PCKG)
