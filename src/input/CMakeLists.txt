set(SOURCES
        src/InputStatus.cpp
        src/DefaultInputManager.cpp
        src/DefaultInputObservationHandler.cpp
        src/InputKeySfmlMapper.cpp
        src/InputManagerFactory.cpp
        src/DefaultInputManagerFactory.cpp
        )

set(UT_SOURCES
        src/InputKeyTest.cpp
        src/InputStatusTest.cpp
        src/DefaultInputManagerTest.cpp
        src/DefaultInputObservationHandlerTest.cpp
        src/InputKeySfmlMapperTest.cpp
        )

add_library(input SHARED ${SOURCES})
target_link_libraries(input PUBLIC window utils)
target_include_directories(input PUBLIC src)

add_executable(inputUT ${UT_SOURCES} ${SOURCES})
target_link_libraries(inputUT PUBLIC gmock_main gtest window utils)
add_test(inputUT inputUT --gtest_color=yes)
