cmake_minimum_required(VERSION 2.8.9)
project(thesaurus)

option(USE_PYTHON3 "Use python 3.x instead of python 2.x." OFF)

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

if (USE_PYTHON3)
  find_package(PythonLibs 3 REQUIRED)
  find_package(Boost 1.50.0 COMPONENTS python3 REQUIRED)
else()
  set(Python_ADDITIONAL_VERSIONS 2.7 2.6)
  find_package(PythonLibs REQUIRED)
  find_package(Boost 1.50.0 COMPONENTS python REQUIRED)
endif()

include_directories(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

python_add_module(thesaurus src/thesaurus.cc)
target_link_libraries(thesaurus ${Boost_LIBRARIES} ${Python_LIBRARIES})
