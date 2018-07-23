cmake_minimum_required (VERSION 2.8.0)

project (HWInterfaceDaemon)

set(
  CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${HWInterfaceDaemon_SOURCE_DIR}/CMakeModules"
)

message("Using modules path: " ${CMAKE_MODULE_PATH})

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
endif()

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.h")

find_package(BCM2835 REQUIRED)

include_directories(
    include/
    ${BCM2835_INCLUDE_DIR}
)

add_executable(
    HWInterfaceDaemon
    ${SRC_FILES}
)

target_link_libraries(
    HWInterfaceDaemon
    -lpthread
    ${BCM2835_LIBRARY}
)
