#include "MenuState.h"

#include "ClickableComponent.h"
#include "EditorState.h"
#include "GameState.h"
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
const auto iconSize = utils::Vector2f{4, 4};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
const auto iconPath = utils::getProjectPath("chimarrao-platformer") + "resources/yerba_item.png";
}

MenuState::MenuState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<input::InputManager>& inputManagerInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      currentButtonIndex{0},
      timeAfterButtonCanBeSwitched{0.2}
{
    inputManager->registerObserver(this);

    createBackground();
    createPlayGameButton();
    createMapEditorButton();
    createSettingsButton();
    createExitButton();
    createIcons();

    initialize();
}

MenuState::~MenuState()
{
    inputManager->removeObserver(this);
}

void MenuState::initialize()
{
    for (auto& button : buttons)
    {
        button->loadDependentComponents();
        button->start();
    }

    buttons[currentButtonIndex]->getComponent<components::GraphicsComponent>()->setColor(buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

void MenuState::update(const utils::DeltaTime& deltaTime)
{
    if (timer.getElapsedSeconds() > timeAfterButtonCanBeSwitched)
    {
        if (inputStatus->isKeyPressed(input::InputKey::Up))
        {
            changeSelectedButtonUp();
            timer.restart();
        }
        else if (inputStatus->isKeyPressed(input::InputKey::Down))
        {
            changeSelectedButtonDown();
            timer.restart();
        }
    }

    for (auto& button : buttons)
    {
        button->update(deltaTime);
    }
}

void MenuState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    for (auto& button : buttons)
    {
        button->lateUpdate(deltaTime);
    }
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

    for (auto& button : buttons)
    {
        button->enable();
    }
}

void MenuState::deactivate()
{
    active = false;

    for (auto& button : buttons)
    {
        button->disable();
    }
}

void MenuState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void MenuState::createBackground()
{
    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{80, 60}, utils::Vector2f{0, 0},
        utils::getProjectPath("chimarrao-platformer") + "resources/BG/menu_background.jpg",
        graphics::VisibilityLayer::Background);
}

void MenuState::createPlayGameButton()
{
    const auto gameButtonPosition = utils::Vector2f{50, 12};

    const auto runGame = [&] {
        states.top()->deactivate();
      states.push(std::make_unique<GameState>(window, inputManager, rendererPool, states));
    };

    addButton(gameButtonPosition, "Play", utils::Vector2f{7, 1}, runGame);
}

void MenuState::createMapEditorButton()
{
    const auto mapEditorButtonPosition = utils::Vector2f{50, 22};

    const auto runMapEditor = [&] {
      states.top()->deactivate();
      states.push(std::make_unique<EditorState>(window, inputManager, rendererPool, states));
    };

    addButton(mapEditorButtonPosition, "Map Editor", utils::Vector2f{1, 1}, runMapEditor);
}

void MenuState::createSettingsButton()
{
    const auto settingsButtonPosition = utils::Vector2f{50, 32};

    const auto runSettings = [&] {
        // TODO: add settings
        std::cout << "Settings\n";
    };

    addButton(settingsButtonPosition, "Settings", utils::Vector2f{3, 1}, runSettings);
}

void MenuState::createExitButton()
{
    const auto exitButtonPosition = utils::Vector2f{50, 42};

    const auto exit = [&] {
        if (not states.empty())
        {
            states.pop();
        }
    };

    addButton(exitButtonPosition, "Exit", utils::Vector2f{7, 1}, exit);
}

void MenuState::addButton(const utils::Vector2f& position, const std::string& text,
                              const utils::Vector2f& textOffset, std::function<void(void)> clickAction)
{
    auto button = std::make_unique<components::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::GraphicsComponent>(
        rendererPool, buttonSize, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::TextComponent>(rendererPool, position, text, fontPath, 35,
                                                    graphics::Color::Black, textOffset);
    button->addComponent<components::HitboxComponent>(buttonSize);
    button->addComponent<components::ClickableComponent>(inputManager, std::move(clickAction));

    const auto buttonIndex = buttons.size();

    const auto changeColorOnMouseOver = [=] {
        unselectAllButtons();
        changeSelectedButton(buttonIndex);
        setIconVisible(buttonIndex);
        graphicsComponent->setColor(buttonHoverColor);
    };
    const auto changeColorOnMouseOut = [=] {
        graphicsComponent->setColor(buttonColor);
        hideIcons();
    };
    button->addComponent<components::MouseOverComponent>(inputManager, changeColorOnMouseOver,
                                                         changeColorOnMouseOut);
    buttons.push_back(std::move(button));
}

void MenuState::createIcons()
{
    const auto gameIconPosition = utils::Vector2f{73, 13};
    const auto mapEditorIconPosition = utils::Vector2f{73, 23};
    const auto settingsIconPosition = utils::Vector2f{73, 33};
    const auto exitIconPosition = utils::Vector2f{73, 43};
    const std::vector<utils::Vector2f> iconPositions{gameIconPosition, mapEditorIconPosition,
                                                     settingsIconPosition, exitIconPosition};

    for (const auto& iconPosition : iconPositions)
    {
        addIcon(iconPosition);
    }
}

void MenuState::addIcon(const utils::Vector2f& position)
{
    auto icon = std::make_unique<components::ComponentOwner>(position);
    icon->addComponent<components::GraphicsComponent>(rendererPool, iconSize, position, iconPath,
                                                      graphics::VisibilityLayer::First);
    icons.push_back(std::move(icon));
}

void MenuState::changeSelectedButtonUp()
{
    unselectAllButtons();

    if (currentButtonIndex == 0)
    {
        currentButtonIndex = buttons.size() - 1;
    }
    else
    {
        currentButtonIndex--;
    }
    buttons[currentButtonIndex]->getComponent<components::GraphicsComponent>()->setColor(buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

void MenuState::changeSelectedButtonDown()
{
    unselectAllButtons();

    if (currentButtonIndex == buttons.size() - 1)
    {
        currentButtonIndex = 0;
    }
    else
    {
        currentButtonIndex++;
    }
    buttons[currentButtonIndex]->getComponent<components::GraphicsComponent>()->setColor(buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

void MenuState::changeSelectedButton(unsigned int buttonIndex)
{
    currentButtonIndex = buttonIndex;
    setIconVisible(currentButtonIndex);
}

void MenuState::unselectAllButtons()
{
    for (auto& button : buttons)
    {
        button->getComponent<components::GraphicsComponent>()->setColor(buttonColor);
    }
}

void MenuState::hideIcons()
{
    for (auto& icon : icons)
    {
        icon->getComponent<components::GraphicsComponent>()->setVisibility(
            graphics::VisibilityLayer::Invisible);
    }
}

void MenuState::setIconVisible(unsigned int iconIndex)
{
    hideIcons();
    icons[currentButtonIndex]->getComponent<components::GraphicsComponent>()->setVisibility(
        graphics::VisibilityLayer::First);
}

}