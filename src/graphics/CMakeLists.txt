set(SOURCES
        src/WindowSfml.cpp
        src/WindowProxySfml.cpp
        src/TextureLoaderSfml.cpp
        src/TextureStorageSfml.cpp
        src/GraphicsIdGenerator.cpp
        src/RendererPoolSfml.cpp
        src/RectangleShape.cpp
        )

add_library(graphics ${SOURCES})

target_link_libraries(graphics PUBLIC utils ${SFML_LIBRARIES})

target_include_directories(graphics PUBLIC src)

set(UT_SOURCES
        src/WindowSfmlTest.cpp
        src/TextureStorageSfmlTest.cpp
        src/RectangleShapeTest.cpp
        src/RendererPoolSfmlTest.cpp
        )
set(UT_SOURCES
        src/WindowSfmlTest.cpp

        )

add_executable(graphicsUT ${UT_SOURCES})

target_link_libraries(graphicsUT PUBLIC gtest_main gmock graphics)
