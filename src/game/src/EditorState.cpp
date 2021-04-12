#include "EditorState.h"

#include "EditorMenuState.h"
#include "EditorStateUIConfigBuilder.h"
#include "GetProjectPath.h"
#include "core/ComponentOwner.h"
#include "core/GraphicsComponent.h"
#include "core/MouseOverComponent.h"
#include "editor/TileMap.h"
#include "ui/DefaultUIManager.h"

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
                         std::stack<std::unique_ptr<State>>& states,
                         std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, states},
      inputStatus{nullptr},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      timeAfterButtonsCanBeClicked{0.3f},
      uiManager{std::move(uiManagerInit)},
      timeBetweenTileMoves{0.005f},
      currentTileType{std::make_shared<TileType>(defaultTileType)}
{
    inputManager->registerObserver(this);
    uiManager->createUI(EditorStateUIConfigBuilder::createEditorUIConfig(this));

    currentTileId = 0;
    currentTilePath = tilesTextureVector[currentTileId];
    tileMap = std::make_unique<TileMap>(
        "", utils::Vector2i(rendererPoolSizeX / tileSizeX * 2, rendererPoolSizeY / tileSizeY));
    for (int y = 0; y < rendererPoolSizeY / tileSizeY; ++y)
    {
        for (int x = 0; x < rendererPoolSizeX / tileSizeX * 2; ++x)
        {
            layoutTileMap.emplace_back(LayoutTile{inputManager, rendererPool, utils::Vector2i{x, y},
                                                  utils::Vector2f{tileSizeX, tileSizeY}, currentTileType,
                                                  *tileMap});
        }
    }

    moveTimer.start();
}

EditorState::~EditorState()
{
    inputManager->removeObserver(this);
}

NextState EditorState::update(const utils::DeltaTime& deltaTime)
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
        for (auto& layoutTile : layoutTileMap)
        {
            layoutTile.update(deltaTime);
        }
        uiManager->update(deltaTime);
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

    states.push(std::make_unique<EditorMenuState>(
        window, inputManager, rendererPool, states,
        std::make_unique<components::ui::DefaultUIManager>(inputManager, rendererPool), *tileMap));
}

}