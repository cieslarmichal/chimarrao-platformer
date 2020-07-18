add_subdirectory(utils)
add_subdirectory(graphics)
add_subdirectory(game)

add_executable(chimarrao main.cpp)

target_link_libraries(chimarrao PUBLIC game)