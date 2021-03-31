#include "GameState.h"

#include "AnimatorSettingsYamlReader.h"
#include "DefaultAnimatorSettingsRepository.h"
#include "GameStateUIConfigBuilder.h"
#include "GetProjectPath.h"
#include "PauseState.h"
#include "PlayerAnimator.h"
#include "core/AnimationComponent.h"
#include "core/GraphicsComponent.h"
#include "core/KeyboardMovementComponent.h"
#include "core/TextComponent.h"
#include "ui/DefaultUIManager.h"
#include "BunnyAnimator.h"

namespace game
{

GameState::GameState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<input::InputManager>& inputManagerInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::stack<std::unique_ptr<State>>& statesInit,
                     std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      uiManager{std::move(uiManagerInit)}
{
    inputManager->registerObserver(this);
    uiManager->createUI(GameStateUIConfigBuilder::createGameUIConfig(this));

    animations::DefaultAnimatorSettingsRepository settingsRepository{
        std::make_unique<animations::AnimatorSettingsYamlReader>()};

    player = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{10, 10}, "player");
    auto graphicsComponent = player->addComponent<components::core::GraphicsComponent>(
        rendererPool, utils::Vector2f{7, 7}, utils::Vector2f{10, 10}, graphics::Color::Magenta,
        graphics::VisibilityLayer::Second);
    auto graphicsId = graphicsComponent->getGraphicsId();
    player->addComponent<components::core::KeyboardMovementComponent>(inputManager);
    auto bunnyAnimatorSettings = settingsRepository.getSingleFileAnimatorSettings("bunny");
    auto bunnyAnimator =
        std::make_shared<animations::BunnyAnimator>(graphicsId, rendererPool, *bunnyAnimatorSettings);
    player->addComponent<components::core::AnimationComponent>(bunnyAnimator);

    player->loadDependentComponents();
    timer.start();
}

GameState::~GameState()
{
    inputManager->removeObserver(this);
}

void GameState::update(const utils::DeltaTime& deltaTime)
{
    if (timer.getElapsedSeconds() > timeAfterStateCouldBePaused &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (not paused)
    {
        player->update(deltaTime);
        uiManager->update(deltaTime);
    }
}

void GameState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    if (not paused)
    {
        player->lateUpdate(deltaTime);
    }
}

void GameState::render()
{
    rendererPool->renderAll();
}

std::string GameState::getName() const
{
    return "Game state";
}

void GameState::activate()
{
    active = true;
    paused = false;
    player->enable();
    timer.restart();
    uiManager->activate();
}

void GameState::deactivate()
{
    active = false;
    timer.restart();
    uiManager->deactivate();
}

void GameState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void GameState::pause()
{
    paused = true;
    player->disable();
    player->getComponent<components::core::GraphicsComponent>()->enable();

    states.push(std::make_unique<PauseState>(
        window, inputManager, rendererPool, states,
        std::make_unique<components::ui::DefaultUIManager>(inputManager, rendererPool)));
}

}
