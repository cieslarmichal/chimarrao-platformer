set(SOURCES
        src/Animation.cpp
        src/AnimatorSettingsYamlReader.cpp
        src/AnimationsFromSettingsLoader.cpp
        src/DefaultAnimatorSettingsRepository.cpp
        src/SingleFileAnimator.cpp
        src/MultipleFileAnimator.cpp
        src/DefaultAnimatorFactory.cpp
        src/AnimatorFactory.cpp
        )

set(UT_SOURCES
        src/AnimationTest.cpp
        src/AnimatorSettingsYamlReaderTest.cpp
        src/AnimationTypeTest.cpp
        src/AnimationDirectionTest.cpp
        src/AnimationsFromSettingsLoaderTest.cpp
        src/DefaultAnimatorSettingsRepositoryTest.cpp
        src/SingleFileAnimatorTest.cpp
        src/MultipleFileAnimatorTest.cpp
        )

add_library(animations STATIC ${SOURCES})
target_link_libraries(animations PUBLIC utils graphics yaml-cpp)
target_include_directories(animations PUBLIC src)

add_executable(animationsUT ${UT_SOURCES} ${SOURCES})
target_link_libraries(animationsUT PUBLIC gmock_main gtest utils graphics yaml-cpp)
add_test(NAME animationsUT COMMAND animationsUT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

