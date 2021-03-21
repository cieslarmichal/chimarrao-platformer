set(SOURCES
        src/Game.cpp
        src/State.cpp
        src/GameState.cpp
        src/MenuState.cpp
        src/EditorState.cpp
        src/PauseState.cpp
        src/editor/TileMap.cpp
        src/EditorMenuState.cpp
        src/SettingsState.cpp
        src/ControlsState.cpp
        src/SaveMapState.cpp
        src/editor/Tile.h src/editor/LayoutTile.h src/editor/TileType.h src/editor/LayoutTile.cpp src/editor/TexturePathForTileNotFound.h src/editor/TileMapSerializerJson.h src/editor/TileMapSerializerJson.cpp)

set(UT_SOURCES
        src/GameTest.cpp
        src/GameStateTest.cpp
        src/MenuStateTest.cpp
        src/EditorStateTest.cpp
        src/PauseStateTest.cpp
        src/EditorMenuStateTest.cpp
        src/SettingsStateTest.cpp
        src/ControlsStateTest.cpp
        src/SaveMapStateTest.cpp
        )

add_library(game ${SOURCES})
target_include_directories(game PUBLIC src ${JSON_INCLUDE_DIR})
target_link_libraries(game PUBLIC utils graphics input components)

add_executable(gameUT ${UT_SOURCES})
target_link_libraries(gameUT PUBLIC gtest_main gmock game)
add_test(gameUT gameUT --gtest_color=yes)
