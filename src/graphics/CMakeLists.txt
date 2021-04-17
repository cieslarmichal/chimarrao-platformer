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

set(UT_SOURCES
        src/TextureLoaderTest.cpp
        src/FontLoaderTest.cpp
        src/TextureStorageSfmlTest.cpp
        src/FontStorageSfmlTest.cpp
        src/RectangleShapeTest.cpp
        src/RendererPoolSfmlTest.cpp
        src/TextTest.cpp
        src/VisibilityLayerTest.cpp
        )

add_library(graphics SHARED ${SOURCES})
target_link_libraries(graphics PUBLIC utils window)
target_include_directories(graphics PUBLIC src)
target_compile_options(graphics PUBLIC ${FLAGS})

add_executable(graphicsUT ${UT_SOURCES} ${SOURCES})
target_link_libraries(graphicsUT PUBLIC gmock_main gtest utils window)
target_compile_options(graphicsUT PUBLIC ${FLAGS})

add_test(NAME graphicsUT COMMAND graphicsUT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

