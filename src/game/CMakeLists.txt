set(SOURCES
        src/Game.cpp
        src/MainGameState.cpp
        src/GameState.cpp
        )

set(UT_SOURCES
        src/GameTest.cpp
        src/MainGameStateTest.cpp
        )

add_library(game ${SOURCES})
target_include_directories(game PUBLIC src)
target_link_libraries(game PUBLIC utils graphics input components)

add_executable(gameUT ${UT_SOURCES})
target_link_libraries(gameUT PUBLIC gtest_main gmock game)
