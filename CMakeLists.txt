cmake_minimum_required(VERSION 3.5.2)
project(h5cpp)
enable_testing()
 
#Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wl,--no-as-needed -ldl")

if(${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -Og")
endif()
message(STATUS "FLAGS: ${CMAKE_CXX_FLAGS}")

#Dependencies
find_library(h5_lib hdf5)
if(NOT h5_lib)
    message(FATAL_ERROR "hdf5 library not found")
else()
    message(STATUS "hdf5 library found: ${h5_lib}")
endif()

#Include the headers
include_directories(include)
include_directories(/usr/lib/openmpi/include)
file(GLOB_RECURSE HEADERS "include/*.h")
 
#Define the sources
file(GLOB_RECURSE SOURCES "src/*.cpp")
 
#Generate the shared library from the sources
add_library(h5cpp SHARED ${SOURCES})
 
#Build executable
set (PROJECT_LINK_LIBS h5cpp ${h5_lib} ${mpi_lib})
link_directories( build )

add_executable(foo tests/main.cpp)
target_link_libraries(foo ${PROJECT_LINK_LIBS} )

#Tests
#Basic Tests
add_executable(basics tests/basics.cpp)
target_link_libraries(basics ${PROJECT_LINK_LIBS} )
add_test(Basics basics)
#Chunked Tests
add_executable(chunk tests/chunked.cpp)
target_link_libraries(chunk ${PROJECT_LINK_LIBS} )
add_test(Chunks chunk)
#String Tests
add_executable(string tests/string.cpp)
target_link_libraries(string ${PROJECT_LINK_LIBS} )
add_test(Strings string)
#Complex Output Tests
add_executable(complex tests/complex.cpp)
target_link_libraries(complex ${PROJECT_LINK_LIBS} )
add_test(Complex_Numbers complex)
#Performance Test
add_executable(performance tests/performance.cpp)
target_link_libraries(performance ${PROJECT_LINK_LIBS} )
add_test(Performance_Check performance)
#Files Test
add_executable(files tests/files.cpp)
target_link_libraries(files ${PROJECT_LINK_LIBS} )
add_test(Files files)
#Read Test
add_executable(read tests/read.cpp)
target_link_libraries(read ${PROJECT_LINK_LIBS} )
add_test(Read read)

#Eigen Test
add_executable(eigen tests/eigen.cpp)
target_link_libraries(eigen ${PROJECT_LINK_LIBS} )
add_test(Eigen eigen)

#Set the location for library installation
install(TARGETS h5cpp DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/h5cpp)
install(FILES include/h5cpp.h DESTINATION include)
