#include "ControlsState.h"

#include "ClickableComponent.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "HitboxComponent.h"
#include "MouseOverComponent.h"
#include "StlOperators.h"
#include "TextComponent.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto controlButtonFontSize{20};
const auto controlButtonSize = utils::Vector2f{8, 3};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
const auto backgroundPath =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/menu_background.jpg";
const auto controlsTitlePosition = utils::Vector2f{32, 6};
const auto backToMenuButtonPosition = utils::Vector2f{34.5, 48};
const auto upButtonPosition = utils::Vector2f{23, 16};
const auto downButtonPosition = utils::Vector2f{23, 22};
const auto rightButtonPosition = utils::Vector2f{23, 28};
const auto leftButtonPosition = utils::Vector2f{23, 34};
const auto spaceButtonPosition = utils::Vector2f{52, 16};
const auto shiftButtonPosition = utils::Vector2f{52, 22};
const auto ctrlButtonPosition = utils::Vector2f{52, 28};
const auto eButtonPosition = utils::Vector2f{52, 34};
const unsigned int descriptionOffset = 2;
const auto upDescriptionPosition =
    utils::Vector2f{upButtonPosition.x + controlButtonSize.x + descriptionOffset, upButtonPosition.y};
const auto downDescriptionPosition =
    utils::Vector2f{downButtonPosition.x + controlButtonSize.x + descriptionOffset, downButtonPosition.y};
const auto rightDescriptionPosition =
    utils::Vector2f{rightButtonPosition.x + controlButtonSize.x + descriptionOffset, rightButtonPosition.y};
const auto leftDescriptionPosition =
    utils::Vector2f{leftButtonPosition.x + controlButtonSize.x + descriptionOffset, leftButtonPosition.y};
const auto spaceDescriptionPosition =
    utils::Vector2f{spaceButtonPosition.x + controlButtonSize.x + descriptionOffset, spaceButtonPosition.y};
const auto shiftDescriptionPosition =
    utils::Vector2f{shiftButtonPosition.x + controlButtonSize.x + descriptionOffset, shiftButtonPosition.y};
const auto ctrlDescriptionPosition =
    utils::Vector2f{ctrlButtonPosition.x + controlButtonSize.x + descriptionOffset, ctrlButtonPosition.y};
const auto eDescriptionPosition =
    utils::Vector2f{eButtonPosition.x + controlButtonSize.x + descriptionOffset, eButtonPosition.y};
}

ControlsState::ControlsState(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<input::InputManager>& inputManagerInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      shouldBackToMenu{false},
      inputStatus{nullptr},
      timeAfterButtonsCanBeClicked{0.3f}
{
    inputManager->registerObserver(this);

    createBackground();
    createControlsTitle();
    createBackToMenuButton();
    createControlButtons();
    createDescriptionsForControlButtons();

    initialize();
}

ControlsState::~ControlsState()
{
    inputManager->removeObserver(this);
}

void ControlsState::initialize()
{
    for (auto& button : buttons)
    {
        button->loadDependentComponents();
        button->start();
        if (auto clickableComponent = button->getComponent<components::ClickableComponent>())
        {
            clickableComponent->disable();
        }
    }
}

void ControlsState::update(const utils::DeltaTime& deltaTime)
{
    if (buttonsActionsFrozen &&
        freezeClickableButtonsTimer.getElapsedSeconds() > timeAfterButtonsCanBeClicked)
    {
        unfreezeButtons();
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

void ControlsState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    for (auto& button : buttons)
    {
        button->lateUpdate(deltaTime);
    }
}

void ControlsState::render()
{
    rendererPool->renderAll();
}

std::string ControlsState::getName() const
{
    return "Controls state";
}

void ControlsState::activate()
{
    active = true;

    for (auto& button : buttons)
    {
        button->enable();
    }
}

void ControlsState::deactivate()
{
    active = false;

    for (auto& button : buttons)
    {
        button->disable();
    }
}

void ControlsState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void ControlsState::unfreezeButtons()
{
    buttonsActionsFrozen = false;
    for (auto& button : buttons)
    {
        if (auto clickableComponent = button->getComponent<components::ClickableComponent>())
        {
            clickableComponent->enable();
        }
    }
}

void ControlsState::backToMenu()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void ControlsState::createBackground()
{
    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(rendererPool, utils::Vector2f{80, 60},
                                                            utils::Vector2f{0, 0}, backgroundPath,
                                                            graphics::VisibilityLayer::Background);
}

void ControlsState::createControlsTitle()
{
    addText(controlsTitlePosition, "Controls", 37);
}

void ControlsState::createControlButtons()
{
    addNonClickableButton(upButtonPosition, controlButtonSize, "^", controlButtonFontSize,
                          utils::Vector2f{3.5, 0.7});
    addNonClickableButton(downButtonPosition, controlButtonSize, "v", controlButtonFontSize,
                          utils::Vector2f{3.5, 0.1});
    addNonClickableButton(rightButtonPosition, controlButtonSize, "->", controlButtonFontSize,
                          utils::Vector2f{3, 0.2});
    addNonClickableButton(leftButtonPosition, controlButtonSize, "<-", controlButtonFontSize,
                          utils::Vector2f{3, 0.2});
    addNonClickableButton(spaceButtonPosition, controlButtonSize, "Space", controlButtonFontSize,
                          utils::Vector2f{1.0, 0.2});
    addNonClickableButton(shiftButtonPosition, controlButtonSize, "Shift", controlButtonFontSize,
                          utils::Vector2f{1.0, 0.2});
    addNonClickableButton(ctrlButtonPosition, controlButtonSize, "Ctrl", controlButtonFontSize,
                          utils::Vector2f{1.7, 0.2});
    addNonClickableButton(eButtonPosition, controlButtonSize, "E", controlButtonFontSize,
                          utils::Vector2f{3.5, 0.2});
}

void ControlsState::createDescriptionsForControlButtons()
{
    addText(upDescriptionPosition, "Jump", controlButtonFontSize);
    addText(downDescriptionPosition, "Squat", controlButtonFontSize);
    addText(rightDescriptionPosition, "Move right", controlButtonFontSize);
    addText(leftDescriptionPosition, "Move left", controlButtonFontSize);
    addText(spaceDescriptionPosition, "Attack", controlButtonFontSize);
    addText(shiftDescriptionPosition, "Speed up", controlButtonFontSize);
    addText(ctrlDescriptionPosition, "Not defined", controlButtonFontSize);
    addText(eDescriptionPosition, "Pick item", controlButtonFontSize);
}

void ControlsState::createBackToMenuButton()
{
    const auto backToMenuButtonSize = utils::Vector2f{13, 5};
    addButtonWithMouseOver(backToMenuButtonPosition, backToMenuButtonSize, "Back", 37, utils::Vector2f{2, 0},
                           [this] { shouldBackToMenu = true; });
}

void ControlsState::addNonClickableButton(const utils::Vector2f& position, const utils::Vector2f& size,
                                          const std::string& text, unsigned int fontSize,
                                          const utils::Vector2f& textOffset)
{
    auto button = std::make_unique<components::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::GraphicsComponent>(
        rendererPool, size, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::TextComponent>(rendererPool, position, text, fontPath, fontSize,
                                                    graphics::Color::Black, textOffset);
    buttons.push_back(std::move(button));
}

void ControlsState::addButtonWithMouseOver(const utils::Vector2f& position, const utils::Vector2f& size,
                                           const std::string& text, unsigned int fontSize,
                                           const utils::Vector2f& textOffset,
                                           std::function<void(void)> clickAction)
{
    auto button = std::make_unique<components::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::GraphicsComponent>(
        rendererPool, size, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::TextComponent>(rendererPool, position, text, fontPath, fontSize,
                                                    graphics::Color::Black, textOffset);
    button->addComponent<components::HitboxComponent>(size);
    button->addComponent<components::ClickableComponent>(inputManager, std::move(clickAction));

    const auto changeColorOnMouseOver = [=] { graphicsComponent->setColor(buttonHoverColor); };
    const auto changeColorOnMouseOut = [=] { graphicsComponent->setColor(buttonColor); };
    button->addComponent<components::MouseOverComponent>(inputManager, changeColorOnMouseOver,
                                                         changeColorOnMouseOut);
    buttons.push_back(std::move(button));
}

void ControlsState::addText(const utils::Vector2f& position, const std::string& description,
                            unsigned int fontSize)
{
    auto text = std::make_unique<components::ComponentOwner>(position);
    text->addComponent<components::TextComponent>(rendererPool, position, description, fontPath, fontSize,
                                                  graphics::Color::Black);
    texts.push_back(std::move(text));
}

}