cmake_minimum_required(VERSION 3.15)

project(chimarrao-platformer CXX)

add_subdirectory(externals)

set(CMAKE_CXX_STANDARD 17)
set(SFML_MINIMAL_VERSION 2.5.1)
if(MSVC)
  set(FLAGS -std:c++17 /W4 /permissive-)
else()
  set(FLAGS -std=c++17 -Wall -g3 -fno-omit-frame-pointer -Wextra -Werror)
#  set(CMAKE_CXX_FLAGS_DEBUG "/MD")
endif()
set(GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googlemock/include")
set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googletest/include")
set(JSON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/json/include")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})
find_package(SFML ${SFML_MINIMAL_VERSION} COMPONENTS system graphics window REQUIRED)

enable_testing()
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

add_subdirectory(src)


