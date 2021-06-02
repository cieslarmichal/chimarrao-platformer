set(SOURCES
        src/Game.cpp
        src/State.cpp
        src/GameState.cpp
        src/MenuState.cpp
        src/EditorState.cpp
        src/PauseState.cpp
        src/editor/DefaultTileMap.cpp
        src/EditorMenuState.cpp
        src/SettingsState.cpp
        src/ControlsState.cpp
        src/SaveMapState.cpp
        src/ChooseMapState.cpp
        src/editor/LayoutTile.cpp
        src/editor/TileMapSerializerJson.cpp
        src/ControlsStateUIConfigBuilder.cpp
        src/EditorMenuStateUIConfigBuilder.cpp
        src/EditorStateUIConfigBuilder.cpp
        src/GameStateUIConfigBuilder.cpp
        src/MenuStateUIConfigBuilder.cpp
        src/PauseStateUIConfigBuilder.cpp
        src/SaveMapStateUIConfigBuilder.cpp
        src/SettingsStateUIConfigBuilder.cpp
        src/ChooseMapStateUIConfigBuilder.cpp
        src/DefaultStates.cpp
        src/StateFactory.cpp
        src/GameFactory.cpp
        src/FileSystemMapsReader.cpp
        src/DefaultComponentOwnersManager.cpp
        src/PaginatedButtonsNavigator.cpp
        src/GridButtonsNavigator.cpp
        )


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
        src/ChooseMapStateTest.cpp
        src/FileSystemMapsReaderTest.cpp
        src/ControlsStateUIConfigBuilderTest.cpp
        src/EditorMenuStateUIConfigBuilderTest.cpp
        src/EditorStateUIConfigBuilderTest.cpp
        src/GameStateUIConfigBuilderTest.cpp
        src/MenuStateUIConfigBuilderTest.cpp
        src/PauseStateUIConfigBuilderTest.cpp
        src/SaveMapStateUIConfigBuilderTest.cpp
        src/SettingsStateUIConfigBuilderTest.cpp
        src/editor/TileMapSerializerJsonTest.cpp
        src/ChooseMapStateUIConfigBuilderTest.cpp
        src/editor/LayoutTileTest.cpp
        src/editor/TileMapTest.cpp
        src/editor/TileTypeTest.cpp
        src/DefaultComponentOwnersManagerTest.cpp
        src/PaginatedButtonsNavigatorTest.cpp
        src/GridButtonsNavigatorTest.cpp
        )

add_library(game ${SOURCES})
target_include_directories(game PUBLIC src ${JSON_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
target_link_libraries(game PUBLIC utils graphics input components physics nfd)
target_compile_options(game PUBLIC ${FLAGS})

add_executable(gameUT ${UT_SOURCES})
target_link_libraries(gameUT PUBLIC gtest_main gmock game)
target_compile_options(gameUT PUBLIC ${FLAGS})
add_test(NAME gameUT COMMAND gameUT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

