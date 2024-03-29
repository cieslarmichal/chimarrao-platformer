cmake_minimum_required(VERSION 3.12)

project(chimarrao-platformer CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(externals)

set(CMAKE_CXX_STANDARD 20)
set(SFML_MINIMAL_VERSION 2.5.1)
if(MSVC)
  set(FLAGS /std:c++latest /W4 /permissive- /MP)
else()
  add_compile_options(-std=c++2a -Wall -Wextra -Werror -fPIC)
  set(FLAGS -std=c++2a -Wall -Wextra -Werror -fPIC)
endif()
set(GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googlemock/include")
set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googletest/include")
set(JSON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/json/include")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})
find_package(SFML ${SFML_MINIMAL_VERSION} COMPONENTS system graphics window audio REQUIRED)

enable_testing()
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

add_subdirectory(src)


