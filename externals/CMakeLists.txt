if (WIN32)
    set(BOOST_ROOT C:\\libraries\\boost)
endif ()

find_package(Boost REQUIRED)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(googletest)
add_subdirectory(json)
add_subdirectory(yaml-cpp)
add_subdirectory(nfd)