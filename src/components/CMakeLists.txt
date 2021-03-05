set(SOURCES
        src/core/ComponentOwner.cpp
        src/core/TransformComponent.cpp
        src/core/GraphicsComponent.cpp
        src/core/KeyboardMovementComponent.cpp
        src/core/AnimationComponent.cpp
        src/core/TextComponent.cpp
        src/core/ClickableComponent.cpp
        src/core/HitboxComponent.cpp
        src/core/MouseOverComponent.cpp
        )

set(UT_SOURCES
        src/core/ComponentOwnerTest.cpp
        src/core/ComponentTest.cpp
        src/core/TransformComponentTest.cpp
        src/core/GraphicsComponentTest.cpp
        src/core/KeyboardMovementComponentTest.cpp
        src/core/AnimationComponentTest.cpp
        src/core/TextComponentTest.cpp
        src/core/ClickableComponentTest.cpp
        src/core/HitboxComponentTest.cpp
        src/core/MouseOverComponentTest.cpp
        )

add_library(components ${SOURCES})
target_link_libraries(components PUBLIC utils graphics animations input)
target_include_directories(components PUBLIC src)

add_executable(componentsUT ${UT_SOURCES})
target_link_libraries(componentsUT PUBLIC gtest_main gmock components)
add_test(componentsUT componentsUT --gtest_color=yes)
