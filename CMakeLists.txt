cmake_minimum_required(VERSION 2.8.9)
project(thesaurus)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Debug', use -DCMAKE_BUILD_TYPE=... to change this.")
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_definitions(-std=c++11 -pedantic)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-inline -O0")

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER MATCHES "clang")
  add_definitions(-Weverything
    -Weffc++
    -Wno-padded
    -Wno-weak-vtables
    -Wno-exit-time-destructors
    -Wno-global-constructors
    -Wno-switch-enum
    -Wno-missing-prototypes
    -Wno-missing-noreturn
    -Wno-unused-parameter
    -Wno-c++98-compat
    -fno-limit-debug-info)
endif()

find_package(Boost 1.50.0 COMPONENTS REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(test test.cc thesaurus.cc)