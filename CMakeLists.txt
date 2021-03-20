cmake_minimum_required(VERSION 3.12)

project(chimarrao-platformer CXX)

add_subdirectory(externals)

set(CMAKE_CXX_STANDARD 17)
set(SFML_MINIMAL_VERSION 2.5.1)
set(FLAGS -std=c++17 -Wall -g3 -fno-omit-frame-pointer -Wextra -Werror)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})
find_package(SFML ${SFML_MINIMAL_VERSION} COMPONENTS system graphics window REQUIRED)

enable_testing()
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

add_subdirectory(src)


