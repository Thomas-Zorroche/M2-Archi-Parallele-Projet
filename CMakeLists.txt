cmake_minimum_required(VERSION 3.13)
project(RK_Projet)
set(OUTPUT_NAME RK_Projet)

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE HEADER_FILES src/*.hpp)
file(GLOB_RECURSE SRC_FILES src/*.cpp)
pkg_check_modules(PCKG REQUIRED IMPORTED_TARGET opencv)

set(LIBS "pkg-config opencv --libs")
set(COMP_OPTS "pkg-config opencv --cflags")

add_executable(${OUTPUT_NAME} ${SRC_FILES} RK_Sabre.cpp ${HEADER_FILES})
target_compile_options(${OUTPUT_NAME} PRIVATE -Wall)
target_link_libraries(${OUTPUT_NAME} PUBLIC PkgConfig::PCKG)
