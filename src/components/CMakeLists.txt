set(SOURCES
        src/core/ComponentOwner.cpp
        src/core/Component.cpp
        src/core/TransformComponent.cpp
        src/core/GraphicsComponent.cpp
        src/core/movement/KeyboardAnimatedMovementComponent.cpp
        src/core/movement/KeyboardHorizontalMovementComponent.cpp
        src/core/AnimationComponent.cpp
        src/core/TextComponent.cpp
        src/core/ClickableComponent.cpp
        src/core/MouseOverComponent.cpp
        src/core/BoxColliderComponent.cpp
        src/core/IdComponent.cpp
        src/core/movement/VelocityComponent.cpp
        src/ui/Background.cpp
        src/ui/Button.cpp
        src/ui/CheckBox.cpp
        src/ui/Label.cpp
        src/ui/TextField.cpp
        src/ui/Image.cpp
        src/ui/DefaultUIManager.cpp
        src/ui/UIComponentFactory.cpp
        src/core/CameraComponent.cpp
        src/core/movement/FriendFollowerComponent.cpp
        src/core/movement/EnemyFollowerComponent.cpp
        src/core/movement/MovementComponent.cpp
        src/core/health/HealthComponent.cpp
        src/core/health/HealthBarComponent.cpp
        src/core/DirectionComponent.cpp
        src/core/attack/KeyboardAttackComponent.cpp
        src/core/movement/IdleNpcMovementComponent.cpp
        src/core/movement/FreeFallMovementComponent.cpp
        src/core/items/CollectableItemComponent.cpp
        src/core/items/ItemCollectorComponent.cpp
        src/core/items/ItemHealEffect.cpp
        src/core/attack/MeleeAttack.cpp
        src/core/attack/ArtificialIntelligenceAttackComponent.cpp
        src/core/attack/DefaultFriendlyFireValidator.cpp
        )

set(UT_SOURCES
        src/core/ComponentOwnerTest.cpp
        src/core/ComponentTest.cpp
        src/core/TransformComponentTest.cpp
        src/core/GraphicsComponentTest.cpp
        src/core/movement/KeyboardAnimatedMovementComponentTest.cpp
        src/core/movement/KeyboardHorizontalMovementComponentTest.cpp
        src/core/AnimationComponentTest.cpp
        src/core/TextComponentTest.cpp
        src/core/ClickableComponentTest.cpp
        src/core/MouseOverComponentTest.cpp
        src/core/BoxColliderComponentTest.cpp
        src/core/IdComponentTest.cpp
        src/core/movement/VelocityComponentTest.cpp
        src/core/CameraComponentTest.cpp
        src/core/movement/FriendFollowerComponentTest.cpp
        src/core/movement/EnemyFollowerComponentTest.cpp
        src/core/movement/MovementComponentTest.cpp
        src/core/health/HealthComponentTest.cpp
        src/core/health/HealthBarComponentTest.cpp
        src/ui/DefaultUIManagerTest.cpp
        src/ui/BackgroundTest.cpp
        src/ui/ButtonTest.cpp
        src/ui/CheckBoxTest.cpp
        src/ui/ImageTest.cpp
        src/ui/LabelTest.cpp
        src/ui/TextFieldTest.cpp
        src/core/DirectionComponentTest.cpp
        src/core/attack/KeyboardAttackComponentTest.cpp
        src/core/movement/IdleNpcMovementComponentTest.cpp
        src/core/movement/FreeFallMovementComponentTest.cpp
        src/core/items/CollectableItemComponentTest.cpp
        src/core/items/ItemCollectorComponentTest.cpp
        src/core/items/ItemHealEffectTest.cpp
        src/core/attack/MeleeAttackTest.cpp
        src/core/attack/ArtificialIntelligenceAttackComponentTest.cpp
        src/core/attack/DefaultFriendlyFireValidatorTest.cpp
        )

add_library(components ${SOURCES})
target_link_libraries(components PUBLIC utils graphics animations input physics)
target_include_directories(components PUBLIC src/core src/ui src/core/attack src/core/health src/core/items src/core/movement)
target_compile_options(components PUBLIC ${FLAGS})

add_executable(componentsUT ${UT_SOURCES})
target_link_libraries(componentsUT PUBLIC gmock_main gtest components physics)
target_compile_options(componentsUT PUBLIC ${FLAGS})
add_test(NAME componentsUT COMMAND componentsUT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

