set(SOURCES
        src/MovableEntity.cpp
        src/NonMovableEntity.cpp
        src/Game.cpp
        src/Timer.cpp
        )

set(UT_SOURCES
        src/MovableEntityTest.cpp
        src/NonMovableEntityTest.cpp
        src/TimerTest.cpp
        )

add_library(game ${SOURCES})
target_include_directories(game PUBLIC ${SFML_INCLUDE_DIR} src)
target_link_libraries (game PUBLIC utils graphics)

add_executable(gameUT ${UT_SOURCES})
target_link_libraries(gameUT PUBLIC gtest_main gmock game)
