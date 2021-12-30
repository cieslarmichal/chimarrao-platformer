#include "EditorState.h"

#include <utility>

#include "CameraComponent.h"
#include "DefaultUIManager.h"
#include "EditorMenuState.h"
#include "EditorStateUIConfigBuilder.h"
#include "KeyboardHorizontalMovementComponent.h"
#include "TimerFactory.h"

namespace game
{
namespace
{
const int rendererPoolSizeX = 80;
const int rendererPoolSizeY = 60;
const int tileSizeX = 4;
const int tileSizeY = 4;
const utils::Vector2f cameraInitialPosition{rendererPoolSizeX / 2.f, rendererPoolSizeY / 2.f};
}

EditorState::EditorState(const std::shared_ptr<window::Window>& windowInit,
                         const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                         std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                         std::shared_ptr<components::ui::UIManager> uiManagerInit,
                         std::shared_ptr<TileMap> tileMapInit, std::unique_ptr<utils::Timer> moveTimer,
                         const std::shared_ptr<components::core::SharedContext>& sharedContext)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      moveTimer{std::move(moveTimer)},
      timeAfterStateCouldBePaused{0.5f},
      timeBetweenTileMoves{0.005f},
      currentTileType{std::make_shared<TileType>(defaultTileType)},
      tileMap{std::move(tileMapInit)},
      uiManager{std::move(uiManagerInit)},
      sharedContext{sharedContext}
{
    uiManager->createUI(EditorStateUIConfigBuilder::createEditorUIConfig(this));
    tileMap->setTileMapInfo(TileMapInfo{"", tileMap->getSize(), {}});
    setTileMap();

    camera =
        std::make_shared<components::core::ComponentOwner>(cameraInitialPosition, "camera", sharedContext);
    camera->addComponent<components::core::KeyboardHorizontalMovementComponent>();
    camera->addComponent<components::core::CameraComponent>(
        sharedContext->rendererPool, utils::FloatRect{0, 0, static_cast<float>(tileMap->getSize().x) * 4.f,
                                                      static_cast<float>(tileMap->getSize().y) * 4.f});
    camera->loadDependentComponents();

    pauseTimer = utils::TimerFactory::createTimer();
}

NextState EditorState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (pauseTimer->getElapsedSeconds() > timeAfterStateCouldBePaused &&
        input.isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (not paused)
    {
        for (auto& layoutTile : layoutTileMap)
        {
            layoutTile.update(deltaTime, input);
        }

        uiManager->update(deltaTime, input);
        camera->update(deltaTime, input);
    }

    return NextState::Same;
}

void EditorState::lateUpdate(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (not paused)
    {
        for (auto& tile : clickableTileMap)
        {
            tile->lateUpdate(deltaTime, input);
        }

        camera->lateUpdate(deltaTime, input);
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
    pauseTimer->restart();

    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.activate();
    }
    uiManager->activate();
    camera->enable();

    setTileMap();
}

void EditorState::deactivate()
{
    active = false;
    pauseTimer->restart();
    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.deactivate();
    }
    uiManager->deactivate();
    camera->disable();
}

void EditorState::pause()
{
    paused = true;

    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.pause();
    }

    camera->disable();

    states.addNextState(StateType::EditorMenu);
}

void EditorState::setTileMap()
{
    layoutTileMap.clear();

    for (int y = 0; y < tileMap->getSize().y; ++y)
    {
        for (int x = 0; x < tileMap->getSize().x; ++x)
        {
            layoutTileMap.emplace_back(LayoutTile{sharedContext, utils::Vector2i{x, y},
                                                  utils::Vector2f{tileSizeX, tileSizeY}, *currentTileType,
                                                  *tileMap});
        }
    }
}

}