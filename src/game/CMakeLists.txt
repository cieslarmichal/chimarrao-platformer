set(SOURCES
        src/MovableEntity.cpp
        src/NonMovableEntity.cpp
        src/Game.cpp
        src/Timer.cpp
        src/KeyboardStatus.cpp
        src/DefaultInputManager.cpp
        src/InputObserverHandler.cpp
        )

set(UT_SOURCES
        src/MovableEntityTest.cpp
        src/NonMovableEntityTest.cpp
        src/TimerTest.cpp
        src/KeyboardStatusTest.cpp
        src/DefaultInputManagerTest.cpp
        src/InputObserverHandlerTest.cpp
        )

add_library(game ${SOURCES})
target_include_directories(game PUBLIC ${SFML_INCLUDE_DIR})
target_link_libraries (game PUBLIC sfml-graphics sfml-window sfml-system)

add_executable(gameUT ${UT_SOURCES})
target_link_libraries(gameUT PUBLIC gtest_main gmock game)
