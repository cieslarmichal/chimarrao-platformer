add_subdirectory(utils)
add_subdirectory(graphics)
add_subdirectory(physics)
add_subdirectory(input)
add_subdirectory(components)
add_subdirectory(game)

add_executable(chimarrao-platformer main.cpp)

target_link_libraries(chimarrao-platformer PUBLIC game)