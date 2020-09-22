set(SOURCES
        src/TextureLoader.cpp
        src/FontLoader.cpp
        src/TextureStorageSfml.cpp
        src/FontStorageSfml.cpp
        src/GraphicsIdGenerator.cpp
        src/RendererPoolSfml.cpp
        src/RectangleShape.cpp
        src/RenderTargetSfml.cpp
        src/GraphicsFactory.cpp
        src/DefaultGraphicsFactory.cpp
        src/Text.cpp
        )

add_library(graphics ${SOURCES})
target_link_libraries(graphics PUBLIC utils ${SFML_LIBRARIES})
target_include_directories(graphics PUBLIC src)

set(UT_SOURCES
        src/TextureLoaderTest.cpp
        src/FontLoaderTest.cpp
        src/TextureStorageSfmlTest.cpp
        src/FontStorageSfmlTest.cpp
        src/RectangleShapeTest.cpp
        src/RendererPoolSfmlTest.cpp
        src/TextTest.cpp
        )

add_executable(graphicsUT ${UT_SOURCES})
target_link_libraries(graphicsUT PUBLIC gtest_main gmock graphics)
