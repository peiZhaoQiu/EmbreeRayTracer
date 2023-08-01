cmake_minimum_required(VERSION 3.22.1)
project(HELLOEMBREE)

set(embree_DIR ${CMAKE_SOURCE_DIR}/embree-4.2.0/lib/cmake/embree-4.2.0)
FIND_PACKAGE(embree 4 REQUIRED)
FIND_PACKAGE(glm REQUIRED)
add_executable(HELLOEMBREE embreeTest.cpp)
TARGET_LINK_LIBRARIES(HELLOEMBREE embree)
TARGET_LINK_LIBRARIES(HELLOEMBREE glm::glm)
