set(SOURCES
        src/Game.cpp
        src/Timer.cpp
        )

add_library(game ${SOURCES})

set(UT_SOURCE
        src/TimerTest.cpp)

add_executable(gameUT ${UT_SOURCE})

target_link_libraries(gameUT PUBLIC gtest_main gmock game)

