cmake_minimum_required(VERSION 3.5.2)
project(h5cpp)
set(CMAKE_BUILD_TYPE Release)
enable_testing()
 
#Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -g -Og")
endif()

#Dependencies
find_library(h5_lib hdf5)
if(NOT h5_lib)
    message(FATAL_ERROR "hdf5 library not found")
else()
    message(STATUS "hdf5 library found")
endif()

#Include the headers
include_directories(include)
 
#Define the sources
file(GLOB_RECURSE SOURCES "src/*.cpp")
 
#Generate the shared library from the sources
add_library(h5cpp SHARED ${SOURCES})
 
#Build executable
set (PROJECT_LINK_LIBS h5cpp ${h5_lib})
link_directories( build )

add_executable(foo tests/main.cpp)
target_link_libraries(foo ${PROJECT_LINK_LIBS} )

#Tests
#Simple Test
#add_executable(simple tests/simple.cpp)
#target_link_libraries(simple ${PROJECT_LINK_LIBS} )
#add_test(SimpleRun simple)

#Set the location for library installation
#install(TARGETS h5cpp DESTINATION lib)
#install(FILES ${MAIN_HEADERS} DESTINATION include/h5cpp)
#install(FILES ${MONITORS_HEADERS} DESTINATION include/h5cpp/monitors)
#install(FILES ${OBJECTS_HEADERS} DESTINATION include/h5cpp/objects)
#install(FILES ${SOURCES_HEADERS} DESTINATION include/h5cpp/sources)
#install(FILES include/h5cpp.h DESTINATION include)
