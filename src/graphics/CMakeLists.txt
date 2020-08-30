set(SOURCES
        src/WindowSfml.cpp
        src/WindowProxySfml.cpp
        src/TextureLoaderSfml.cpp
        src/TextureStorageSfml.cpp
        src/GraphicsIdGenerator.cpp
        src/RendererPoolSfml.cpp
        src/RectangleShape.cpp
        src/RenderTargetSfml.cpp
        src/GraphicsFactory.cpp
        src/DefaultGraphicsFactory.cpp
        src/animation/Animation.cpp
        src/animation/PlayerAnimator.cpp
        src/animation/AnimatorSettingsYamlReader.cpp
        src/animation/AnimationsFromSettingsLoader.cpp
        )

add_library(graphics ${SOURCES})
target_link_libraries(graphics PUBLIC utils ${SFML_LIBRARIES} yaml-cpp)
target_include_directories(graphics PUBLIC src)

set(UT_SOURCES
        src/WindowSfmlTest.cpp
        src/TextureLoaderSfmlTest.cpp
        src/TextureStorageSfmlTest.cpp
        src/RectangleShapeTest.cpp
        src/RendererPoolSfmlTest.cpp
        src/RenderTargetSfmlTest.cpp
        src/animation/AnimationTest.cpp
        src/animation/PlayerAnimatorTest.cpp
        src/animation/AnimatorSettingsYamlReaderTest.cpp
        src/animation/AnimationTypeTest.cpp
        src/animation/AnimationsFromSettingsLoaderTest.cpp
        )

add_executable(graphicsUT ${UT_SOURCES})
target_link_libraries(graphicsUT PUBLIC gtest_main gmock graphics yaml-cpp)
