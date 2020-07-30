set(SOURCES
        src/MovableEntity.cpp
        src/NonMovableEntity.cpp
        src/InputStatus.cpp
        src/DefaultInputManager.cpp
        src/DefaultInputObservationHandler.cpp
        src/Game.cpp
        )

set(UT_SOURCES
        src/MovableEntityTest.cpp
        src/NonMovableEntityTest.cpp
        src/InputStatusTest.cpp
        src/DefaultInputManagerTest.cpp
        src/DefaultInputObservationHandlerTest.cpp
        src/GameTest.cpp
        )

add_library(game ${SOURCES})
target_include_directories(game PUBLIC ${SFML_INCLUDE_DIR} src)
target_link_libraries (game PUBLIC utils graphics)

add_executable(gameUT ${UT_SOURCES})
target_link_libraries(gameUT PUBLIC gtest_main gmock game)
