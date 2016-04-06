project(gpioLib)				# Define the project
cmake_minimum_required(VERSION 2.8.9)		# minimum Cmake v. 2.8.9 is required.
set ( CMAKE_BUILD_TYPE Release )		# Type of build

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wextra -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb3")
set ( CMAKE_CXX_FLAGS_RELEASE "-o3")
add_definitions ( -Wall )

include_directories(${PROJECT_SOURCE_DIR})
#FIND_PACKAGE( wiringPi REQUIRED )
#include_directories (${WIRINGPI_INCLUDE_DIR})
FIND_PACKAGE(Threads REQUIRED)
AUX_SOURCE_DIRECTORY(. SRC_LIST)

set(C_Files					# File codes of proyects (not .h) 
	SimpleGPIO.cpp
)

include (GenerateExportHeader)			# Tell CMake to create the library
add_compiler_export_flags()		        # Export library to be compatible with windows
include_directories (${PROJECT_BINARY_DIR})
add_library(${PROJECT_NAME} STATIC ${C_Files})  # Build library by itself	
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
GENERATE_EXPORT_HEADER(${PROJECT_NAME})
