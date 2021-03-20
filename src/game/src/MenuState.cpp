#include "MenuState.h"

#include "ControlsState.h"
#include "GetProjectPath.h"
#include "core/GraphicsComponent.h"
#include "core/HitBoxComponent.h"
#include "core/MouseOverComponent.h"
#include "ui/DefaultUIManager.h"
#include "MenuStateUIConfigBuilder.h"

namespace game
{
namespace
{
const auto gameButtonPosition = utils::Vector2f{50, 7};
const auto mapEditorButtonPosition = utils::Vector2f{50, 17};
const auto controlsButtonPosition = utils::Vector2f{50, 27};
const auto settingsButtonPosition = utils::Vector2f{50, 37};
const auto exitButtonPosition = utils::Vector2f{50, 47};
const std::vector<utils::Vector2f> buttonsPositions{gameButtonPosition, mapEditorButtonPosition,
                                                    controlsButtonPosition, settingsButtonPosition,
                                                    exitButtonPosition};
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto buttonSize = utils::Vector2f{23, 6};
const auto iconSize = utils::Vector2f{4, 4};
const auto iconPath = utils::getProjectPath("chimarrao-platformer") + "resources/yerba_item.png";
}

MenuState::MenuState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<input::InputManager>& inputManagerInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::stack<std::unique_ptr<State>>& statesInit, std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      currentButtonIndex{0},
      timeAfterButtonCanBeSwitched{0.1},
      buttonNames{"menuPlayButton", "menuMapEditorButton", "menuControlsButton", "menuSettingsButton",
                  "menuExitButton"},
      uiManager{std::move(uiManagerInit)}
{
    inputManager->registerObserver(this);
    uiManager->createUI(MenuStateUIConfigBuilder::createMenuUIConfig(this));

    createIcons();
    uiManager->setColor(components::ui::UIComponentType::Button, buttonNames.at(currentButtonIndex),
                        buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

MenuState::~MenuState()
{
    inputManager->removeObserver(this);
}

void MenuState::update(const utils::DeltaTime& deltaTime)
{
    if (switchButtonTimer.getElapsedSeconds() > timeAfterButtonCanBeSwitched)
    {
        handleButtonSwitching();
    }

    uiManager->update(deltaTime);
}

void MenuState::lateUpdate(const utils::DeltaTime& deltaTime)
{
}

void MenuState::render()
{
    rendererPool->renderAll();
}

std::string MenuState::getName() const
{
    return "Menu state";
}

void MenuState::activate()
{
    active = true;
    uiManager->activate();
}

void MenuState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void MenuState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void MenuState::handleButtonSwitching()
{
    if (inputStatus->isKeyPressed(input::InputKey::Up))
    {
        changeSelectedButtonUp();
    }
    else if (inputStatus->isKeyPressed(input::InputKey::Down))
    {
        changeSelectedButtonDown();
    }
    switchButtonTimer.restart();
}

void MenuState::createIcons()
{
    for (const auto& buttonPosition : buttonsPositions)
    {
        const auto iconPosition = utils::Vector2f{buttonPosition.x + buttonSize.x, buttonPosition.y + 1};
        addIcon(iconPosition);
    }
}

void MenuState::addIcon(const utils::Vector2f& position)
{
    auto icon = std::make_unique<components::core::ComponentOwner>(
        position, "icon" + std::to_string(position.x) + std::to_string(position.y));
    icon->addComponent<components::core::GraphicsComponent>(rendererPool, iconSize, position, iconPath,
                                                            graphics::VisibilityLayer::First);
    icons.push_back(std::move(icon));
}

void MenuState::changeSelectedButtonUp()
{
    unselectAllButtons();

    if (currentButtonIndex == 0)
    {
        currentButtonIndex = buttonNames.size() - 1;
    }
    else
    {
        currentButtonIndex--;
    }
    uiManager->setColor(components::ui::UIComponentType::Button, buttonNames.at(currentButtonIndex),
                        buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

void MenuState::changeSelectedButtonDown()
{
    unselectAllButtons();

    if (currentButtonIndex == buttonNames.size() - 1)
    {
        currentButtonIndex = 0;
    }
    else
    {
        currentButtonIndex++;
    }
    uiManager->setColor(components::ui::UIComponentType::Button, buttonNames.at(currentButtonIndex),
                        buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

void MenuState::changeSelectedButton(unsigned int buttonIndex)
{
    currentButtonIndex = buttonIndex;
    setIconVisible(currentButtonIndex);
}

void MenuState::unselectAllButtons()
{
    uiManager->setColor(components::ui::UIComponentType::Button, "menuPlayButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuMapEditorButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuControlsButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuSettingsButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuExitButton", buttonColor);
}

void MenuState::setIconVisible(unsigned int iconIndex)
{
    hideIcons();
    icons[iconIndex]->getComponent<components::core::GraphicsComponent>()->setVisibility(
        graphics::VisibilityLayer::First);
}

void MenuState::hideIcons()
{
    for (auto& icon : icons)
    {
        icon->getComponent<components::core::GraphicsComponent>()->setVisibility(
            graphics::VisibilityLayer::Invisible);
    }
}

}