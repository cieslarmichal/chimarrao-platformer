set(SOURCES
        src/Animation.cpp
        src/AnimatorSettingsYamlReader.cpp
        src/AnimationsFromSettingsLoader.cpp
        src/DefaultAnimatorSettingsRepository.cpp
        src/PlayerAnimator.cpp
        src/BunnyAnimator.cpp
        )

set(UT_SOURCES
        src/AnimationTest.cpp
        src/AnimatorSettingsYamlReaderTest.cpp
        src/AnimationTypeTest.cpp
        src/AnimationDirectionTest.cpp
        src/AnimationsFromSettingsLoaderTest.cpp
        src/DefaultAnimatorSettingsRepositoryTest.cpp
        src/PlayerAnimatorTest.cpp
        src/BunnyAnimatorTest.cpp
        )

add_library(animations ${SOURCES})
target_link_libraries(animations PUBLIC utils graphics ${SFML_LIBRARIES} yaml-cpp)
target_include_directories(animations PUBLIC src)

add_executable(animationsUT ${UT_SOURCES})
target_link_libraries(animationsUT PUBLIC gtest_main gmock utils graphics yaml-cpp animations)
add_test(animationsUT animationsUT --gtest_color=yes)
