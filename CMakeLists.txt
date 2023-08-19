cmake_minimum_required(VERSION 3.22.1)
project(HELLOEMBREE)

set(embree_DIR ${CMAKE_SOURCE_DIR}/external/embree-4.2.0/lib/cmake/embree-4.2.0)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/tinyobjloader)
#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/tinyobjloader)
#make sure to include the model directory so that obj files can be loaded   
#include_directories(${CMAKE_SOURCE_DIR}/Model)
message(${CMAKE_SOURCE_DIR}/external/tinyobjloader)
FIND_PACKAGE(embree 4 REQUIRED)
FIND_PACKAGE(glm REQUIRED)

#include all the files in the src directory
file(GLOB SOURCES "src/*.cpp")

add_executable(HELLOEMBREE ${CMAKE_SOURCE_DIR}/src/embreeTest.cpp ${SOURCES}) #external/tinyobjloader/tiny_obj_loader.cc)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external/tinyobjloader)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC tinyobjloader embree glm::glm)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} embree)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} glm::glm)
