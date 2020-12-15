#include "PauseState.h"

#include "ClickableComponent.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "HitboxComponent.h"
#include "MouseOverComponent.h"
#include "TextComponent.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
const auto textColor = graphics::Color(200, 200, 200);
const auto buttonSize = utils::Vector2f{25, 5};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
}

PauseState::PauseState(const std::shared_ptr<window::Window>& windowInit,
                       const std::shared_ptr<input::InputManager>& inputManagerInit,
                       const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                       std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToGame{false},
      shouldBackToMenu{false},
      timeAfterButtonsCanBeClicked{0.3f}
{
    inputManager->registerObserver(this);

    createBackground();
    createPauseTitle();
    createBackToGameButton();
    createMenuButton();

    initialize();
}

PauseState::~PauseState()
{
    inputManager->removeObserver(this);
}

void PauseState::initialize()
{
    for (auto& button : buttons)
    {
        button->loadDependentComponents();
        button->start();
        button->getComponent<components::ClickableComponent>()->disable();
    }

    timer.start();
}

void PauseState::update(const utils::DeltaTime& deltaTime)
{
    if (buttonsActionsFrozen &&
        freezeClickableButtonsTimer.getElapsedSeconds() > timeAfterButtonsCanBeClicked)
    {
        unfreezeButtons();
    }

    if (timer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        shouldBackToGame = true;
    }

    if (shouldBackToGame)
    {
        backToGame();
        return;
    }

    if (shouldBackToMenu)
    {
        backToMenu();
        return;
    }

    for (auto& button : buttons)
    {
        button->update(deltaTime);
    }
}

void PauseState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    for (auto& button : buttons)
    {
        button->lateUpdate(deltaTime);
    }
}

void PauseState::render()
{
    rendererPool->renderAll();
}

std::string PauseState::getName() const
{
    return "Pause state";
}

void PauseState::activate()
{
    active = true;
}

void PauseState::deactivate()
{
    active = false;
}

void PauseState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void PauseState::unfreezeButtons()
{
    buttonsActionsFrozen = false;
    for (auto& button : buttons)
    {
        button->getComponent<components::ClickableComponent>()->enable();
    }
}

void PauseState::backToGame()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void PauseState::backToMenu()
{
    while (not states.empty() && states.top()->getName() != "Menu state")
    {
        states.pop();
    }

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void PauseState::createPauseTitle()
{
    const auto textPausePosition = utils::Vector2f{35, 13};
    title = std::make_unique<components::ComponentOwner>(textPausePosition);
    title->addComponent<components::TextComponent>(rendererPool, textPausePosition, "Pause", fontPath, 40,
                                                   graphics::Color::White, utils::Vector2f{0, 0});
}

void PauseState::createBackground()
{
    const auto backgroundColor = graphics::Color{172};
    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(rendererPool, utils::Vector2f{31, 32},
                                                            utils::Vector2f{25, 10}, backgroundColor,
                                                            graphics::VisibilityLayer::Background);
}

void PauseState::createBackToGameButton()
{
    const auto backToGameButtonPosition = utils::Vector2f{28, 21};

    addButton(backToGameButtonPosition, "Back to game", utils::Vector2f{2, 0.5},
              [this] { shouldBackToGame = true; });
}

void PauseState::createMenuButton()
{
    const auto backToMenuButtonPosition = utils::Vector2f{28, 30};

    addButton(backToMenuButtonPosition, "Back to menu", utils::Vector2f{2, 0.5},
              [this] { shouldBackToMenu = true; });
}

void PauseState::addButton(const utils::Vector2f& position, const std::string& text,
                           const utils::Vector2f& textOffset, std::function<void(void)> clickAction)
{
    auto button = std::make_unique<components::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::GraphicsComponent>(
        rendererPool, buttonSize, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::TextComponent>(rendererPool, position, text, fontPath, 30, textColor,
                                                    textOffset);
    button->addComponent<components::HitboxComponent>(buttonSize);
    button->addComponent<components::ClickableComponent>(inputManager, std::move(clickAction));

    const auto changeColorOnMouseOver = [=] { graphicsComponent->setColor(buttonHoverColor); };
    const auto changeColorOnMouseOut = [=] { graphicsComponent->setColor(buttonColor); };
    button->addComponent<components::MouseOverComponent>(inputManager, changeColorOnMouseOver,
                                                         changeColorOnMouseOut);
    buttons.push_back(std::move(button));
}

}