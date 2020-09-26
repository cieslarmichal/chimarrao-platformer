set(SOURCES
        src/Animation.cpp
        src/PlayerAnimator.cpp
        src/AnimatorSettingsYamlReader.cpp
        src/AnimationsFromSettingsLoader.cpp
        src/DefaultAnimatorSettingsRepository.cpp
        )

add_library(animations ${SOURCES})
target_link_libraries(animations PUBLIC utils graphics ${SFML_LIBRARIES} yaml-cpp)
target_include_directories(animations PUBLIC src)

set(UT_SOURCES
        src/AnimationTest.cpp
        src/PlayerAnimatorTest.cpp
        src/AnimatorSettingsYamlReaderTest.cpp
        src/AnimationTypeTest.cpp
        src/AnimationsFromSettingsLoaderTest.cpp
        src/DefaultAnimatorSettingsRepositoryTest.cpp
        )

add_executable(animationsUT ${UT_SOURCES})
target_link_libraries(animationsUT PUBLIC gtest_main gmock utils graphics yaml-cpp animations)

add_test(animationsUT animationsUT --gtest_color=yes)
