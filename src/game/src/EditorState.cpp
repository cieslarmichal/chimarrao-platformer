#include "EditorState.h"

#include <core/MouseOverComponent.h>

#include "EditorMenuState.h"
#include "GetProjectPath.h"
#include "core/ComponentOwner.h"
#include "core/GraphicsComponent.h"
#include "editor/TileMap.h"

namespace game
{
namespace
{
const float rendererPoolSizeX = 80;
const float rendererPoolSizeY = 60;
const float tileSizeX = 4;
const float tileSizeY = 4;
const auto pathToBackground =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/background_glacial_mountains.png";
const auto pathToBrickTileTexture =
    utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png";
const auto tilesTextureVector =
    std::vector<std::string>{utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png",
                             utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/2.png"};

}

EditorState::EditorState(const std::shared_ptr<window::Window>& windowInit,
                         const std::shared_ptr<input::InputManager>& inputManagerInit,
                         const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                         std::stack<std::unique_ptr<State>>& states)
    : State{windowInit, inputManagerInit, rendererPoolInit, states},
      inputStatus{nullptr},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      timeAfterButtonsCanBeClicked{0.3f},
      timeBetweenTileMoves{0.025f},
      currentTileType{std::make_shared<TileType>(defaultTileType)}
{
    inputManager->registerObserver(this);

    currentTileId = 0;
    currentTilePath = tilesTextureVector[currentTileId];
    background = std::make_unique<components::core::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::core::GraphicsComponent>(
        rendererPool, utils::Vector2f{rendererPoolSizeX, rendererPoolSizeY}, utils::Vector2f{0, 0},
        pathToBackground, graphics::VisibilityLayer::Background);

    tileMap = std::make_unique<TileMap>(
        utils::Vector2i(rendererPoolSizeX / tileSizeX * 2, rendererPoolSizeY / tileSizeY),
        utils::Vector2f(tileSizeX, tileSizeY));
    for (int y = 0; y < rendererPoolSizeY / tileSizeY; ++y)
    {
        for (int x = 0; x < rendererPoolSizeX / tileSizeX * 2; ++x)
        {
            layoutTileMap.emplace_back(LayoutTile{inputManager, rendererPool, utils::Vector2i{x, y},
                                                  utils::Vector2f{tileSizeX, tileSizeY}, currentTileType,
                                                  *tileMap});
        }
    }

    background->loadDependentComponents();
    background->start();
    moveTimer.start();
}

EditorState::~EditorState()
{
    inputManager->removeObserver(this);
}

void EditorState::update(const utils::DeltaTime& deltaTime)
{
    if (pauseTimer.getElapsedSeconds() > timeAfterStateCouldBePaused &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (moveTimer.getElapsedSeconds() > timeBetweenTileMoves &&
        inputStatus->isKeyPressed(input::InputKey::Left) && layoutTileMap.front().getPosition().x < 0)
    {
        moveTimer.restart();
        for (auto& layoutTile : layoutTileMap)
        {
            layoutTile.moveTile({.3, 0});
        }
    }
    else if (moveTimer.getElapsedSeconds() > timeBetweenTileMoves &&
             inputStatus->isKeyPressed(input::InputKey::Right) &&
             layoutTileMap.back().getPosition().x + tileSizeX > rendererPoolSizeX)
    {
        moveTimer.restart();
        for (auto& layoutTile : layoutTileMap)
        {
            layoutTile.moveTile({-.3, 0});
        }
    }

    if (not paused)
    {
        background->update(deltaTime);
        for (auto& layoutTile : layoutTileMap)
        {
            layoutTile.update(deltaTime);
        }
    }
}

void EditorState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    if (not paused)
    {
        background->lateUpdate(deltaTime);
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
}

void EditorState::deactivate()
{
    active = false;
    pauseTimer.restart();
    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.deactivate();
    }
}

void EditorState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void EditorState::pause()
{
    paused = true;
    buttonsActionsFrozen = true;

    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.pause();
    }

    states.push(std::make_unique<EditorMenuState>(window, inputManager, rendererPool, states, *tileMap));
}

}