#include "EditorState.h"

#include <utility>

#include "EditorMenuState.h"
#include "EditorStateUIConfigBuilder.h"
#include "ProjectPathReader.h"
#include "ui/DefaultUIManager.h"

namespace game
{
namespace
{
const int rendererPoolSizeX = 80;
const int rendererPoolSizeY = 60;
const int tileSizeX = 4;
const int tileSizeY = 4;
const auto tilesTextureVector =
    std::vector<std::string>{utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/brick.png",
                             utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/2.png"};

}
EditorState::EditorState(const std::shared_ptr<window::Window>& windowInit,
                         const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                         std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                         std::unique_ptr<components::ui::UIManager> uiManagerInit, TileMap& tileMapInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      timeAfterButtonsCanBeClicked{0.3f},
      uiManager{std::move(uiManagerInit)},
      tileMap{tileMapInit},
      timeBetweenTileMoves{0.005f},
      currentTileType{std::make_shared<TileType>(defaultTileType)}
{
    uiManager->createUI(EditorStateUIConfigBuilder::createEditorUIConfig(this));

    currentTileId = 0;
    currentTilePath = tilesTextureVector[currentTileId];

    for (int y = 0; y < rendererPoolSizeY / tileSizeY; ++y)
    {
        for (int x = 0; x < rendererPoolSizeX / tileSizeX * 2; ++x)
        {
            layoutTileMap.emplace_back(LayoutTile{rendererPool, utils::Vector2i{x, y},
                                                  utils::Vector2f{tileSizeX, tileSizeY}, currentTileType,
                                                  tileMap});
        }
    }

    moveTimer.start();
}

NextState EditorState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (pauseTimer.getElapsedSeconds() > timeAfterStateCouldBePaused &&
        input.isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (moveTimer.getElapsedSeconds() > timeBetweenTileMoves && input.isKeyPressed(input::InputKey::Left) &&
        layoutTileMap.front().getPosition().x < 0)
    {
        moveTimer.restart();
        for (auto& layoutTile : layoutTileMap)
        {
            layoutTile.moveTile({.3f, 0.f});
        }
    }
    else if (moveTimer.getElapsedSeconds() > timeBetweenTileMoves &&
             input.isKeyPressed(input::InputKey::Right) &&
             layoutTileMap.back().getPosition().x + tileSizeX > rendererPoolSizeX)
    {
        moveTimer.restart();
        for (auto& layoutTile : layoutTileMap)
        {
            layoutTile.moveTile({-0.3f, 0.f});
        }
    }

    if (not paused)
    {
        for (auto& layoutTile : layoutTileMap)
        {
            layoutTile.update(deltaTime, input);
        }
        uiManager->update(deltaTime, input);
    }

    return NextState::Same;
}

void EditorState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    if (not paused)
    {
        for (auto& tile : clickableTileMap)
        {
            tile->lateUpdate(deltaTime);
        }
    }
}

void EditorState::render()
{
    rendererPool->renderAll();
}

StateType EditorState::getType() const
{
    return StateType::Editor;
}

void EditorState::activate()
{
    active = true;
    paused = false;
    freezeClickableButtonsTimer.restart();
    pauseTimer.restart();

    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.activate();
    }
    uiManager->activate();
}

void EditorState::deactivate()
{
    active = false;
    pauseTimer.restart();
    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.deactivate();
    }
    uiManager->deactivate();
}

void EditorState::pause()
{
    paused = true;
    buttonsActionsFrozen = true;

    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.pause();
    }

    states.addNextState(StateType::EditorMenu);
}

}