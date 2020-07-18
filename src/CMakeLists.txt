add_subdirectory(game)

add_executable(chimarrao main.cpp)

target_include_directories(chimarrao PRIVATE game/src)

target_link_libraries(chimarrao PRIVATE game)