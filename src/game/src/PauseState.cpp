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
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto buttonSize = utils::Vector2f{23, 6};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
}

PauseState::PauseState(const std::shared_ptr<window::Window>& windowInit,
                       const std::shared_ptr<input::InputManager>& inputManagerInit,
                       const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                       std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      backToGameCallback{[&] { states.pop(); }}
{
    inputManager->registerObserver(this);

    createBackground();
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
    }
}

void PauseState::update(const utils::DeltaTime& deltaTime)
{
    if (inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        backToGameCallback();
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

void PauseState::createBackground()
{
    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(rendererPool, utils::Vector2f{80, 60},
                                                            utils::Vector2f{0, 0}, graphics::Color::Black,
                                                            graphics::VisibilityLayer::Background);
}

void PauseState::createBackToGameButton()
{
    const auto backToGameButtonPosition = utils::Vector2f{30, 12};

    addButton(backToGameButtonPosition, "Back to game", utils::Vector2f{3, 1}, backToGameCallback);
}

void PauseState::createMenuButton()
{
    const auto backToMenuButtonPosition = utils::Vector2f{30, 22};

    const auto backToMenu = [&] {
        while (states.top()->getName() != "Menu state" && not states.empty())
        {
            states.pop();
        }
    };

    addButton(backToMenuButtonPosition, "Back to menu", utils::Vector2f{3, 1}, backToMenu);
}

void PauseState::addButton(const utils::Vector2f& position, const std::string& text,
                           const utils::Vector2f& textOffset, std::function<void(void)> clickAction)
{
    auto button = std::make_unique<components::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::GraphicsComponent>(
        rendererPool, buttonSize, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::TextComponent>(rendererPool, position, text, fontPath, 35,
                                                    graphics::Color::Black, textOffset);
    button->addComponent<components::HitboxComponent>(buttonSize);
    button->addComponent<components::ClickableComponent>(inputManager, std::move(clickAction));

    const auto changeColorOnMouseOver = [=] { graphicsComponent->setColor(buttonHoverColor); };
    const auto changeColorOnMouseOut = [=] { graphicsComponent->setColor(buttonColor); };
    button->addComponent<components::MouseOverComponent>(inputManager, changeColorOnMouseOver,
                                                         changeColorOnMouseOut);
    buttons.push_back(std::move(button));
}

}