set(SOURCES
        src/InputStatus.cpp
        src/DefaultInputManager.cpp
        src/DefaultInputObservationHandler.cpp
        )

set(UT_SOURCES
        src/InputKeyTest.cpp
        src/InputStatusTest.cpp
        src/DefaultInputManagerTest.cpp
        src/DefaultInputObservationHandlerTest.cpp
        )

add_library(input ${SOURCES})
target_link_libraries(input PUBLIC utils window)
target_include_directories(input PUBLIC src)

add_executable(inputUT ${UT_SOURCES})
target_link_libraries(inputUT PUBLIC gtest_main gmock input)
add_test(inputUT inputUT --gtest_color=yes)
