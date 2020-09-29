set(SOURCES
        src/Game.cpp
        src/State.cpp
        src/GameState.cpp
        src/MainMenuState.cpp
        src/EditorState.cpp src/TileMap.cpp)

set(UT_SOURCES
        src/GameTest.cpp
        src/GameStateTest.cpp
        src/MainMenuStateTest.cpp
        )

add_library(game ${SOURCES})
target_include_directories(game PUBLIC src)
target_link_libraries(game PUBLIC utils graphics input components)

add_executable(gameUT ${UT_SOURCES})
target_link_libraries(gameUT PUBLIC gtest_main gmock game)

add_test(gameUT gameUT --gtest_color=yes)
