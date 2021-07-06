set(SOURCES
        src/core/ComponentOwner.cpp
        src/core/Component.cpp
        src/core/TransformComponent.cpp
        src/core/GraphicsComponent.cpp
        src/core/KeyboardMovementComponent.cpp
        src/core/AnimationComponent.cpp
        src/core/TextComponent.cpp
        src/core/ClickableComponent.cpp
        src/core/MouseOverComponent.cpp
        src/core/BoxColliderComponent.cpp
        src/core/IdComponent.cpp
        src/core/VelocityComponent.cpp
        src/ui/Background.cpp
        src/ui/Button.cpp
        src/ui/CheckBox.cpp
        src/ui/Label.cpp
        src/ui/TextField.cpp
        src/ui/Image.cpp
        src/ui/DefaultUIManager.cpp
        src/ui/UIComponentFactory.cpp
        src/core/CameraComponent.cpp
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
        src/core/MouseOverComponentTest.cpp
        src/core/BoxColliderComponentTest.cpp
        src/core/IdComponentTest.cpp
        src/core/VelocityComponentTest.cpp
        src/ui/DefaultUIManagerTest.cpp
        src/ui/BackgroundTest.cpp
        src/ui/ButtonTest.cpp
        src/ui/CheckBoxTest.cpp
        src/ui/ImageTest.cpp
        src/ui/LabelTest.cpp
        src/ui/TextFieldTest.cpp
        )

add_library(components ${SOURCES})
target_link_libraries(components PUBLIC utils graphics animations input)
target_include_directories(components PUBLIC src)
target_compile_options(components PUBLIC ${FLAGS})

add_executable(componentsUT ${UT_SOURCES})
target_link_libraries(componentsUT PUBLIC gmock_main gtest components)
target_compile_options(componentsUT PUBLIC ${FLAGS})
add_test(NAME componentsUT COMMAND componentsUT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

