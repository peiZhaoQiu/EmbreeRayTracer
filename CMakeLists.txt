cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_C_COMPILER /opt/intel/oneapi/compiler/latest/linux/bin/icx)
set(CMAKE_CXX_COMPILER /opt/intel/oneapi/compiler/latest/linux/bin/icpx)
project(HELLOEMBREE)

option(ENABLE_DEBUG "Enable Valgrind-based tests" OFF)
if(ENABLE_DEBUG)
   message(STATUS "DEBUG FLAG IS ON")
#    set(CMAKE_EXE_LINKER_FLAGS "-g -O0")
   add_compile_options(-g -O0)
endif()

if(ENABLE_SYCL)
   message(STATUS "SYCL FLAG IS ON")
   add_compile_options(-fsycl)
endif()



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
if(ENABLE_SYCL)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external/tinyobjloader /opt/intel/oneapi/compiler/latest/linux/include)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC tinyobjloader embree glm::glm sycl)
else()
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external/tinyobjloader)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC tinyobjloader embree glm::glm)
endif()
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} embree)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} glm::glm)
