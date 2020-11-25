#include "EditorState.h"

#include "ComponentOwner.h"
#include "EditorMenuState.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "TileMap.h"

namespace game
{

EditorState::EditorState(const std::shared_ptr<window::Window>& windowInit,
                         const std::shared_ptr<input::InputManager>& inputManagerInit,
                         const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                         std::stack<std::unique_ptr<State>>& states)
    : State{windowInit, inputManagerInit, rendererPoolInit, states},
      inputStatus{nullptr},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      timeAfterButtonsCanBeClicked{0.3f}
{
    inputManager->registerObserver(this);

    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{80, 60}, utils::Vector2f{0, 0},
        utils::getProjectPath("chimarrao-platformer") + "resources/BG/background_glacial_mountains.png",
        graphics::VisibilityLayer::Background);

    tileMap = std::make_unique<TileMap>(utils::Vector2i(16, 12), utils::Vector2f(5, 5));

    for (int y = 0; y < 12; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            auto clickableTile = std::make_unique<components::ComponentOwner>(
                utils::Vector2f{static_cast<float>(x * 5), static_cast<float>(y * 5)});
            auto graphicsComponent = clickableTile->addComponent<components::GraphicsComponent>(
                rendererPool, utils::Vector2f{5, 5},
                utils::Vector2f{static_cast<float>(x * 5), static_cast<float>(y * 5)},
                utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png",
                graphics::VisibilityLayer::Invisible);
            clickableTile->addComponent<components::HitboxComponent>(utils::Vector2f{5, 5});
            clickableTile->addComponent<components::ClickableComponent>(inputManager, [=]() {
                tileMap->setTile({x, y}, 1);
                graphicsComponent->setVisibility(graphics::VisibilityLayer::Second);
            });
            clickableTileMap.push_back(std::move(clickableTile));
        }
    }

    initialize();
}

EditorState::~EditorState()
{
    inputManager->removeObserver(this);
}

void EditorState::initialize()
{
    for (auto& tile : clickableTileMap)
    {
        tile->loadDependentComponents();
        tile->start();
        tile->getComponent<components::ClickableComponent>()->disable();
    }
}

void EditorState::update(const utils::DeltaTime& dt)
{
    if (buttonsActionsFrozen && freezeClickableButtonsTimer.getElapsedSeconds() > timeAfterButtonsCanBeClicked)
    {
        unfreezeButtons();
    }

    if (pauseTimer.getElapsedSeconds() > timeAfterStateCouldBePaused &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (not paused)
    {
        for (auto& tile : clickableTileMap)
        {
            tile->update(dt);
        }
    }
}

void EditorState::lateUpdate(const utils::DeltaTime& dt)
{
    if (not paused)
    {
        for (auto& tile : clickableTileMap)
        {
            tile->lateUpdate(dt);
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

    for (auto& tile : clickableTileMap)
    {
        tile->enable();
        tile->getComponent<components::ClickableComponent>()->disable();
    }
}

void EditorState::deactivate()
{
    active = false;
    pauseTimer.restart();
}

void EditorState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void EditorState::pause()
{
    paused = true;
    buttonsActionsFrozen = true;

    for (auto& tile : clickableTileMap)
    {
        tile->disable();
        tile->getComponent<components::GraphicsComponent>()->enable();
    }

    states.push(std::make_unique<EditorMenuState>(window, inputManager, rendererPool, states));
}

void EditorState::unfreezeButtons()
{
    std::cout<< "HEJA";
    buttonsActionsFrozen = false;
    for (auto& tile : clickableTileMap)
    {
        tile->getComponent<components::ClickableComponent>()->enable();
    }
}

}