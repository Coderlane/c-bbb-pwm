#
#
#
#

set(PROJECT_NAME "bbbpwm")

cmake_minimum_required(VERSION 2.8.11)
project(${PROJECT_NAME})

enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(UDev REQUIRED)

set(BBB_PWM_LIBRARY "bbbpwm")
set(BBB_PWM_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(BBB_PWM_SRC "${PROJECT_SOURCE_DIR}/src")
set(BBB_PWM_TEST "${PROJECT_SOURCE_DIR}/test")
set(BBB_PWM_UTIL "${PROJECT_SOURCE_DIR}/util")
set(BBB_PWM_VERSION_MAJOR "0")
set(BBB_PWM_VERSION_MINOR "1")
set(BBB_PWM_VERSION_PATCH "0")

# Uncomment to make makefiles verbose
# set(CMAKE_VERBOSE_MAKEFILE ON)

# gcc flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -O3")

# Source Code
add_subdirectory(${BBB_PWM_SRC})
SET(BBB_PWM_SOURCE_FILES ${SOURCE_FILES})

add_subdirectory(${BBB_PWM_INCLUDE_DIR})
include_directories(${BBB_PWM_INCLUDE_DIR})
set(BBB_PWM_HEADER_FILES ${HEADER_FILES})

include_directories(${udev_INCLUDE_DIR})

# LIBRARY
add_library(${BBB_PWM_LIBRARY} SHARED ${BBB_PWM_SOURCE_FILES})
target_link_libraries(${BBB_PWM_LIBRARY} ${udev_LIBRARY})

# Tests
add_subdirectory(${BBB_PWM_TEST})

# Utilities
add_subdirectory(${BBB_PWM_UTIL})

install (TARGETS ${BBB_PWM_LIBRARY} DESTINATION lib)
install (FILES ${BBB_PWM_HEADER_FILES} DESTINATION include/bbb_pwm)

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${BBB_PWM_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${BBB_PWM_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${BBB_PWM_VERSION_PATCH}")
include (CPack)
