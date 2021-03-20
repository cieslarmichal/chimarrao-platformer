#include "GameState.h"

#include "AnimatorSettingsYamlReader.h"
#include "DefaultAnimatorSettingsRepository.h"
#include "GetProjectPath.h"
#include "PauseState.h"
#include "PlayerAnimator.h"
#include "core/AnimationComponent.h"
#include "core/GraphicsComponent.h"
#include "core/KeyboardMovementComponent.h"
#include "core/TextComponent.h"
#include "ui/DefaultUIManager.h"

namespace game
{
namespace
{
const std::string pathToBackground =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/background_glacial_mountains.png";
}

GameState::GameState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<input::InputManager>& inputManagerInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      uiManager{std::make_unique<components::ui::DefaultUIManager>(inputManagerInit, rendererPoolInit,
                                                                   createSettingsUIConfig())}
{
    inputManager->registerObserver(this);

    animations::DefaultAnimatorSettingsRepository settingsRepository{
        std::make_unique<animations::AnimatorSettingsYamlReader>()};

    player = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{10, 10}, "player");
    auto graphicsComponent = player->addComponent<components::core::GraphicsComponent>(
        rendererPool, utils::Vector2f{7, 7}, utils::Vector2f{10, 10}, graphics::Color::Red,
        graphics::VisibilityLayer::Second);
    auto graphicsId = graphicsComponent->getGraphicsId();
    player->addComponent<components::core::KeyboardMovementComponent>(inputManager);
    auto playerAnimatorSettings = settingsRepository.getAnimatorSettings("player");
    auto playerAnimator =
        std::make_shared<animations::PlayerAnimator>(graphicsId, rendererPool, *playerAnimatorSettings);
    player->addComponent<components::core::AnimationComponent>(playerAnimator);
    player->addComponent<components::core::TextComponent>(
        rendererPool, utils::Vector2f{10, 10}, "hello",
        utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf", 13,
        graphics::Color::Black, utils::Vector2f{1.5, -1.5});

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
    player->getComponent<components::core::TextComponent>()->enable();

    states.push(std::make_unique<PauseState>(window, inputManager, rendererPool, states));
}

std::unique_ptr<components::ui::UIConfig> GameState::createSettingsUIConfig()
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> checkBoxesConfig;
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;
    std::vector<std::unique_ptr<components::ui::TextFieldConfig>> textFieldsConfig;

    auto backgroundConfig = std::make_unique<components::ui::BackgroundConfig>(
        "gameBackground", utils::Vector2f{0, 0}, utils::Vector2f{80, 60},
        graphics::VisibilityLayer::Background, pathToBackground);

    return std::make_unique<components::ui::UIConfig>(std::move(backgroundConfig), std::move(buttonsConfig),
                                                      std::move(checkBoxesConfig), std::move(labelsConfig),
                                                      std::move(textFieldsConfig));
}

}
