#include "MainMenuState.h"

#include <utility>

#include "ClickableComponent.h"
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
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
}

MainMenuState::MainMenuState(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<input::InputManager>& inputManagerInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit}
{
    createBackground();
    createPlayGameButton();
    createMapEditorButton();
    createSettingsButton();
    createExitButton();

    initialize();
}

void MainMenuState::initialize()
{
    for (auto& button : buttons)
    {
        button->loadDependentComponents();
        button->start();
    }
}

void MainMenuState::update(const utils::DeltaTime& deltaTime)
{
    for (auto& button : buttons)
    {
        button->update(deltaTime);
    }
}

void MainMenuState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    for (auto& button : buttons)
    {
        button->lateUpdate(deltaTime);
    }
}

void MainMenuState::render()
{
    rendererPool->renderAll();
}

std::string MainMenuState::getName() const
{
    return "Main menu state";
}

void MainMenuState::createBackground()
{
    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{80, 60}, utils::Vector2f{0, 0},
        utils::getProjectPath("chimarrao-platformer") + "resources/BG/menu_background.jpg",
        graphics::VisibilityLayer::Background);
}

void MainMenuState::createPlayGameButton()
{
    const auto gameButtonPosition = utils::Vector2f{50, 12};

    auto runGame = [&] {
      states.push(std::make_unique<GameState>(window, inputManager, rendererPool, states));
    };

    addButton(gameButtonPosition, "Play", utils::Vector2f{7, 1}, runGame);
}

void MainMenuState::createMapEditorButton()
{
    const auto mapEditorButtonPosition = utils::Vector2f{50, 22};

    const auto runMapEditor = [&] {
        // TODO: add map editor
        std::cout << "Map editor\n";
    };

    addButton(mapEditorButtonPosition, "Map Editor", utils::Vector2f{1, 1}, runMapEditor);
}

void MainMenuState::createSettingsButton()
{
    const auto settingsButtonPosition = utils::Vector2f{50, 32};

    const auto runSettings = [&] {
        // TODO: add settings
        std::cout << "Settings\n";
    };

    addButton(settingsButtonPosition, "Settings", utils::Vector2f{3, 1}, runSettings);
}

void MainMenuState::createExitButton()
{
    const auto exitButtonPosition = utils::Vector2f{50, 42};

    const auto exit = [&] {
        // TODO: add exit
        std::cout << "Exit\n";
    };

    addButton(exitButtonPosition, "Exit", utils::Vector2f{7, 1}, exit);
}

void MainMenuState::addButton(const utils::Vector2f& position, const std::string& text,
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