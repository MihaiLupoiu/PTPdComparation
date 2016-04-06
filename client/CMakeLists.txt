set (PROJECTNAME ClientPTPd)			# Define the name of the project

project(${PROJECTNAME})
cmake_minimum_required(VERSION 2.8)

set ( CMAKE_BUILD_TYPE Release )		# Type of build
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wextra -Wall")
set ( CMAKE_CXX_FLAGS_DEBUG "-o0 -g -ggdb3" ) 
set ( CMAKE_CXX_FLAGS_RELEASE "-o3") 
add_definitions ( -Wall )

set(source_files 
	udpClientNonblock.cpp
)

find_package(Threads REQUIRED)

add_subdirectory(gpioLib)			# GPIO library
include_directories (${gpioLib_SOURCE_DIR})

add_executable(${PROJECTNAME} ${source_files})
target_link_libraries(${PROJECTNAME} gpioLib ${CMAKE_THREAD_LIBS_INIT})