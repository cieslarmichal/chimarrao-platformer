set(SOURCES
        src/ComponentOwner.cpp
        src/TransformComponent.cpp
        src/GraphicsComponent.cpp
        src/KeyboardMovementComponent.cpp
        src/AnimationComponent.cpp
        src/TextComponent.cpp
        src/ClickableComponent.cpp
        src/HitboxComponent.cpp
        src/MouseOverComponent.cpp
        )

set(UT_SOURCES
        src/ComponentOwnerTest.cpp
        src/ComponentTest.cpp
        src/TransformComponentTest.cpp
        src/GraphicsComponentTest.cpp
        src/KeyboardMovementComponentTest.cpp
        src/AnimationComponentTest.cpp
        src/TextComponentTest.cpp
        src/ClickableComponentTest.cpp
        src/HitboxComponentTest.cpp
        src/MouseOverComponentTest.cpp
        )

add_library(components ${SOURCES})
target_link_libraries(components PUBLIC utils graphics animations input)
target_include_directories(components PUBLIC src)

add_executable(componentsUT ${UT_SOURCES})
target_link_libraries(componentsUT PUBLIC gtest_main gmock components)
add_test(componentsUT componentsUT --gtest_color=yes)
