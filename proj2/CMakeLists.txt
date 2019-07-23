cmake_minimum_required(VERSION 3.14.5)

project(utah-cg-proj2)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

find_package (Threads)

file(GLOB_RECURSE HEADERS "src/*.h" "../extern/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp" "../extern/*.cpp")
include_directories("src", "../extern")

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
