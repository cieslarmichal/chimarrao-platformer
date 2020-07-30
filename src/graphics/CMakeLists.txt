set(SOURCES
        src/WindowSfml.cpp
        src/WindowProxySfml.cpp
        src/TextureLoaderSfml.cpp
        )

add_library(graphics ${SOURCES})

target_link_libraries(graphics PUBLIC utils sfml-graphics sfml-window sfml-system)

target_include_directories(graphics PUBLIC src)

set(UT_SOURCE src/WindowSfmlTest.cpp)

add_executable(graphicsUT ${UT_SOURCE})

target_link_libraries(graphicsUT PUBLIC gtest_main gmock graphics)
