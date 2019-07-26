cmake_minimum_required(VERSION 3.14.5)

project(utah-cg-proj7)

find_package (Threads)

file(GLOB_RECURSE HEADERS "src/*.h" "../extern/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp" "../extern/*.cpp")
include_directories("src", "../extern")

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
