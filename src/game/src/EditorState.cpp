#include "EditorState.h"

#include "EditorMenuState.h"
#include "EditorStateUIConfigBuilder.h"
#include "GetProjectPath.h"
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
    std::vector<std::string>{utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png",
                             utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/2.png"};

}
EditorState::EditorState(const std::shared_ptr<window::Window>& windowInit,
                         const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                         std::stack<std::unique_ptr<State>>& states,
                         std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, states},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      timeAfterButtonsCanBeClicked{0.3f},
      uiManager{std::move(uiManagerInit)},
      timeBetweenTileMoves{0.005f},
      currentTileType{std::make_shared<TileType>(defaultTileType)}
{
    uiManager->createUI(EditorStateUIConfigBuilder::createEditorUIConfig(this));

    currentTileId = 0;
    currentTilePath = tilesTextureVector[currentTileId];
    tileMap = std::make_unique<TileMap>(
        "", utils::Vector2i(rendererPoolSizeX / tileSizeX * 2, rendererPoolSizeY / tileSizeY));
    for (int y = 0; y < rendererPoolSizeY / tileSizeY; ++y)
    {
        for (int x = 0; x < rendererPoolSizeX / tileSizeX * 2; ++x)
        {
            layoutTileMap.emplace_back(LayoutTile{rendererPool, utils::Vector2i{x, y},
                                                  utils::Vector2f{tileSizeX, tileSizeY}, currentTileType,
                                                  *tileMap});
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

std::string EditorState::getName() const
{
    return "Editor state";
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

    states.push(std::make_unique<EditorMenuState>(
        window, rendererPool, states, std::make_unique<components::ui::DefaultUIManager>(rendererPool),
        *tileMap));
}

}