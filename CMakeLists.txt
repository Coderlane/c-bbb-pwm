#
#
#
#

set(PROJECT_NAME "BBB-PWM")

cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME})

enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(UDev REQUIRED)

set(BBB_PWM_LIBRARY "bbbpwm")
set(BBB_PWM_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(BBB_PWM_SRC "${PROJECT_SOURCE_DIR}/src")
set(BBB_PWM_TEST "${PROJECT_SOURCE_DIR}/test")

# Uncomment to make makefiles verbose
# set(CMAKE_VERBOSE_MAKEFILE ON)

# gcc flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -D_BSD_SOURCE=1")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -O3")

# Source Code
add_subdirectory(${BBB_PWM_SRC})
include_directories(${BBB_PWM_INCLUDE_DIR})
include_directories(${udev_INCLUDE_DIR})

# LIBRARY
add_library(${BBB_PWM_LIBRARY} SHARED ${SOURCE_FILES})
target_link_libraries(${BBB_PWM_LIBRARY} ${udev_LIBRARY})

# Tests
add_subdirectory(${BBB_PWM_TEST})

