set(SOURCES
        src/Game.cpp
        src/State.cpp
        src/GameState.cpp
        src/MenuState.cpp
        src/EditorState.cpp
        src/PauseState.cpp
        src/TileMap.cpp
        src/EditorMenuState.cpp
        )

set(UT_SOURCES
        src/GameTest.cpp
        src/GameStateTest.cpp
        src/MenuStateTest.cpp
        src/EditorStateTest.cpp
        src/PauseStateTest.cpp
        src/EditorMenuStateTest.cpp
        )

add_library(game ${SOURCES})
target_include_directories(game PUBLIC src)
target_link_libraries(game PUBLIC utils graphics input components)

add_executable(gameUT ${UT_SOURCES})
target_link_libraries(gameUT PUBLIC gtest_main gmock game)
add_test(gameUT gameUT --gtest_color=yes)
