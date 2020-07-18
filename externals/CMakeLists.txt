find_package(Boost REQUIRED)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(googletest)
add_subdirectory(sfml)
add_subdirectory(json)
