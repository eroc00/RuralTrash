cmake_minimum_required(VERSION 3.5...3.19)
project(RuralTrash CXX)

# add cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# try to prevent accidental in-source builds, these cause lots of problems
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Remove any files created thus far and use a different directory for the build.")
endif()

# require a C++11 compiler for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND STRICT)
  message("-- Enabling strict compilation mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()


################################################################################
# Include Headers
################################################################################

#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
find_Package(pigpio REQUIRED)

set(MODULES Header/DataStructures.hpp 
	    Header/LateralController.hpp
	    Header/Movement.hpp
	    Header/OpController.hpp
	    Header/Receiver.hpp
	    Header/HeadwayTracker.hpp
	    Header/ImageProcessor.hpp)

# make executable
#add_executable(vehicle_code ${MODULES} main.cpp)
add_executable(HeadwayTrackerTest 
		Header/HeadwayTracker.hpp
		Header/HeadwayTracker.cpp
		FunctionalTests/HeadwayTrackerTest.cpp)
target_link_libraries(HeadwayTrackerTest pigpio)

# build the unit tests
#add_executable(unit_tests ${MODULES} catch.hpp unit_tests.cpp vehicle_code_unit_tests.cpp)
enable_testing()

# register Catch tests with cmake
#include(Catch)
#catch_discover_tests(unit_tests)
