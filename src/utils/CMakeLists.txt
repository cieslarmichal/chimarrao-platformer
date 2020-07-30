set(SOURCES
        src/BitMask.cpp
        src/Timer.cpp
        )

set(UT_SOURCES
        src/BitMaskTest.cpp
        src/TimerTest.cpp
        )

add_library(utils ${SOURCES})
add_executable(utilsUT ${UT_SOURCES})

target_include_directories(utils PUBLIC src)

target_link_libraries(utils PUBLIC sfml-graphics sfml-window sfml-system)
target_link_libraries(utilsUT PUBLIC gtest_main gmock utils)
