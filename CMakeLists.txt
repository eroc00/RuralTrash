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

find_Package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})
#target_link_libraries( [executable], ${OpenCV_LIBS} )

find_Package(raspicam REQUIRED)
#target_link_libraries( [executable], ${raspicam_LIBS} )

set(MODULES Header/PinDefs.hpp
	    Header/DataStructures.hpp Header/DataStructures.cpp 
	    Header/Movement.hpp Header/Movement.cpp
	    Header/OpController.hpp Header/OpController.cpp
	    Header/Receiver.hpp Header/Receiver.cpp
	    Header/HeadwayTracker.hpp Header/HeadwayTracker.cpp
	    Header/ImageProcessor.hpp
	    Header/LateralController.hpp)

# make executable
#add_executable(vehicle_code ${MODULES} main.cpp)
#target_link_libraries(vehicle_code pigpio ${OpenCV_LIBS} ${raspicam_CV_LIBS})

#add_executable(HeadwayTrackerTest 
#		Header/HeadwayTracker.hpp Header/HeadwayTracker.cpp
#		Tests/HeadwayTrackerTest.cpp)
#target_link_libraries(HeadwayTrackerTest pigpio)

#add_executable(MotorTest
#		Header/DataStructures.hpp Header/DataStructures.cpp
#		Header/Movement.hpp Header/Movement.cpp
#		Tests/MotorTest.cpp)
#target_link_libraries(MotorTest pigpio)

#add_executable(ReceiverTest
#		Header/OpController.hpp Header/OpController.cpp
#		Header/DataStructures.hpp Header/DataStructures.cpp
#		Header/Movement.hpp Header/Movement.cpp
#		Header/Receiver.hpp Header/Receiver.cpp
#		Tests/ReceiverTest.cpp)
#target_link_libraries(ReceiverTest pigpio)


add_executable(EdgeDetectorTest Tests/EdgeDetection.cpp)
target_link_libraries(EdgeDetectorTest ${OpenCV_LIBS})

add_executable(VideoCapture Tests/VideoCapture.cpp)
target_link_libraries(VideoCapture ${raspicam_CV_LIBS} ${OpenCV_LIBS})

# build the unit tests
enable_testing()
add_executable(unit_tests ${MODULES} Tests/catch.hpp Tests/unit_tests.cpp Tests/vehicle_code_unit_tests.cpp)
target_link_libraries(unit_tests pigpio)
add_test(unit_tests unit_tests)

# register Catch tests with cmake
#include(Catch)
#catch_discover_tests(unit_tests)
