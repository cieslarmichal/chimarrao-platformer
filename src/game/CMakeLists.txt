set(SOURCES
        src/GameTimer.cpp
        src/Game.cpp
        src/states/State.cpp
        src/states/game/CustomGameState.cpp
        src/states/game/StoryGameState.cpp
        src/states/menu/MenuState.cpp
        src/states/editor/EditorState.cpp
        src/states/pause/PauseState.cpp
        src/states/game/GameOverState.cpp
        src/states/game/GameVictoryState.cpp
        src/states/editor/DefaultTileMap.cpp
        src/states/editor/EditorMenuState.cpp
        src/states/settings/SettingsState.cpp
        src/states/controls/ControlsState.cpp
        src/states/editor/SaveMapState.cpp
        src/states/chooseMap/ChooseMapState.cpp
        src/states/editor/LayoutTile.cpp
        src/states/editor/TileMapSerializerJson.cpp
        src/states/controls/ControlsStateUIConfigBuilder.cpp
        src/states/editor/EditorMenuStateUIConfigBuilder.cpp
        src/states/editor/EditorStateUIConfigBuilder.cpp
        src/states/game/GameStateUIConfigBuilder.cpp
        src/states/menu/MenuStateUIConfigBuilder.cpp
        src/states/pause/PauseStateUIConfigBuilder.cpp
        src/states/game/GameOverStateUIConfigBuilder.cpp
        src/states/game/GameVictoryStateUIConfigBuilder.cpp
        src/states/editor/SaveMapStateUIConfigBuilder.cpp
        src/states/settings/SettingsStateUIConfigBuilder.cpp
        src/states/chooseMap/ChooseMapStateUIConfigBuilder.cpp
        src/states/DefaultStates.cpp
        src/states/StateFactory.cpp
        src/GameFactory.cpp
        src/states/chooseMap/FileSystemMapsReader.cpp
        src/navigators/PaginatedButtonsNavigator.cpp
        src/navigators/GridButtonsNavigator.cpp
        src/states/game/HeadsUpDisplay.cpp
        src/states/game/HeadsUpDisplayUIConfigBuilder.cpp
        src/states/game/CharacterFactory.cpp
        src/states/game/ItemFactory.cpp
        src/states/game/ObstacleFactory.cpp
        src/states/game/levels/Level1WorldBuilder.cpp
        src/states/game/CustomWorldBuilder.cpp
        src/states/game/levels/Level1Controller.cpp
        src/states/game/levels/Level1DialoguesController.cpp
        src/states/game/dialogues/DefaultDialoguesReader.cpp
        src/states/game/dialogues/DialoguesValidator.cpp
        src/states/game/dialogues/DialoguesParser.cpp
        src/states/game/dialogues/DialogueTrack.cpp
        )

set(UT_SOURCES
        src/GameTimerTest.cpp
        src/GameTest.cpp
        src/states/menu/MenuStateTest.cpp
        src/states/editor/EditorStateTest.cpp
        src/states/pause/PauseStateTest.cpp
        src/states/game/GameOverStateTest.cpp
        src/states/game/GameOverStateUIConfigBuilderTest.cpp
        src/states/game/GameVictoryStateTest.cpp
        src/states/game/GameVictoryStateUIConfigBuilderTest.cpp
        src/states/editor/EditorMenuStateTest.cpp
        src/states/settings/SettingsStateTest.cpp
        src/states/controls/ControlsStateTest.cpp
        src/states/editor/SaveMapStateTest.cpp
        src/states/chooseMap/ChooseMapStateTest.cpp
        src/states/chooseMap/FileSystemMapsReaderTest.cpp
        src/states/controls/ControlsStateUIConfigBuilderTest.cpp
        src/states/editor/EditorMenuStateUIConfigBuilderTest.cpp
        src/states/editor/EditorStateUIConfigBuilderTest.cpp
        src/states/game/GameStateUIConfigBuilderTest.cpp
        src/states/menu/MenuStateUIConfigBuilderTest.cpp
        src/states/pause/PauseStateUIConfigBuilderTest.cpp
        src/states/editor/SaveMapStateUIConfigBuilderTest.cpp
        src/states/settings/SettingsStateUIConfigBuilderTest.cpp
        src/states/editor/TileMapSerializerJsonTest.cpp
        src/states/chooseMap/ChooseMapStateUIConfigBuilderTest.cpp
        src/states/editor/LayoutTileTest.cpp
        src/states/editor/DefaultTileMapTest.cpp
        src/states/editor/TileTypeTest.cpp
        src/navigators/PaginatedButtonsNavigatorTest.cpp
        src/navigators/GridButtonsNavigatorTest.cpp
        src/states/game/HeadsUpDisplayTest.cpp
        src/states/game/HeadsUpDisplayUIConfigBuilderTest.cpp
        src/states/game/levels/Level1DialoguesControllerTest.cpp
        src/states/game/dialogues/DefaultDialoguesReaderTest.cpp
        src/states/game/dialogues/DialoguesValidatorTest.cpp
        src/states/game/dialogues/DialoguesParserTest.cpp
        src/states/game/dialogues/DialogueTrackTest.cpp
        )

add_library(game ${SOURCES})
target_include_directories(game PUBLIC
        src
        src/states
        src/states/editor
        src/states/game
        src/states/game/levels
        src/states/game/dialogues
        src/states/chooseMap
        src/states/controls
        src/states/settings
        src/navigators
        ${JSON_INCLUDE_DIR}
        ${Boost_INCLUDE_DIR}
        )
target_link_libraries(game PUBLIC utils graphics input components physics nfd audio)
target_compile_options(game PUBLIC ${FLAGS})

add_executable(gameUT ${UT_SOURCES})
target_link_libraries(gameUT PUBLIC gtest_main gmock game)
target_compile_options(gameUT PUBLIC ${FLAGS})
add_test(NAME gameUT COMMAND gameUT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

