set(SOURCES
        src/MovableEntity.cpp
        src/NonMovableEntity.cpp
        src/Timer.cpp
        src/InputStatus.cpp
        src/DefaultInputManager.cpp
        src/DefaultInputObservationHandler.cpp
        src/Game.cpp
        src/MainGameState.cpp
        src/GameState.cpp
        src/Player.cpp
        )

set(UT_SOURCES
        src/MovableEntityTest.cpp
        src/NonMovableEntityTest.cpp
        src/TimerTest.cpp
        src/InputStatusTest.cpp
        src/DefaultInputManagerTest.cpp
        src/DefaultInputObservationHandlerTest.cpp
        src/GameTest.cpp
        src/MainGameStateTest.cpp
        )

add_library(game ${SOURCES})
target_include_directories(game PUBLIC src)
target_link_libraries (game PUBLIC utils graphics physics)

add_executable(gameUT ${UT_SOURCES})
target_link_libraries(gameUT PUBLIC gtest_main gmock game)
