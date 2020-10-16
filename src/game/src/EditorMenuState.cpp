#include "EditorMenuState.h"

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
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/font.ttf";
}

EditorMenuState::EditorMenuState(const std::shared_ptr<window::Window>& windowInit,
                                 const std::shared_ptr<input::InputManager>& inputManagerInit,
                                 const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                                 std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditor{false},
      shouldBackToMenu{false}
{
    inputManager->registerObserver(this);

    createBackground();
    createEditorTitle();
    createBackToEditorButton();
    createNewMapButton();
    createSaveMapButton();
    createMenuButton();

    initialize();
}

EditorMenuState::~EditorMenuState()
{
    inputManager->removeObserver(this);
}

void EditorMenuState::initialize()
{
    for (auto& button : buttons)
    {
        button->loadDependentComponents();
        button->start();
    }

    timer.start();
}

void EditorMenuState::update(const utils::DeltaTime& deltaTime)
{
    if (timer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        shouldBackToEditor = true;
    }

    if (shouldBackToEditor)
    {
        backToEditor();
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

void EditorMenuState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    for (auto& button : buttons)
    {
        button->lateUpdate(deltaTime);
    }
}

void EditorMenuState::render()
{
    rendererPool->renderAll();
}

std::string EditorMenuState::getName() const
{
    return "Editor menu state";
}

void EditorMenuState::activate()
{
    active = true;
}

void EditorMenuState::deactivate()
{
    active = false;
}

void EditorMenuState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void EditorMenuState::backToEditor()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void EditorMenuState::backToMenu()
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

void EditorMenuState::createEditorTitle()
{
    const auto textPausePosition = utils::Vector2f{30, 11};
    title = std::make_unique<components::ComponentOwner>(textPausePosition);
    title->addComponent<components::TextComponent>(rendererPool, textPausePosition, "Editor Menu", fontPath,
                                                   40, graphics::Color::White, utils::Vector2f{0, 0});
}

void EditorMenuState::createBackground()
{
    const auto backgroundColor = graphics::Color{172};
    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(rendererPool, utils::Vector2f{31, 43},
                                                            utils::Vector2f{25, 8}, backgroundColor,
                                                            graphics::VisibilityLayer::Second);
}

void EditorMenuState::createBackToEditorButton()
{
    const auto backToGameButtonPosition = utils::Vector2f{28, 19};

    addButton(backToGameButtonPosition, "Back to editor", utils::Vector2f{2, 0},
              [this] { shouldBackToEditor = true; });
}

void EditorMenuState::createNewMapButton()
{
    const auto backToMenuButtonPosition = utils::Vector2f{28, 27};

    addButton(backToMenuButtonPosition, "New map", utils::Vector2f{5, 0},
              [this] { std::cout << "new map\n"; });
}

void EditorMenuState::createSaveMapButton()
{
    const auto backToMenuButtonPosition = utils::Vector2f{28, 35};

    addButton(backToMenuButtonPosition, "Save map", utils::Vector2f{5, 0},
              [this] { std::cout << "save map\n"; });
}

void EditorMenuState::createMenuButton()
{
    const auto backToMenuButtonPosition = utils::Vector2f{28, 43};

    addButton(backToMenuButtonPosition, "Back to menu", utils::Vector2f{2, 0},
              [this] { shouldBackToMenu = true; });
}

void EditorMenuState::addButton(const utils::Vector2f& position, const std::string& text,
                                const utils::Vector2f& textOffset, std::function<void(void)> clickAction)
{
    auto button = std::make_unique<components::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::GraphicsComponent>(
        rendererPool, buttonSize, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::TextComponent>(rendererPool, position, text, fontPath, 37, textColor,
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