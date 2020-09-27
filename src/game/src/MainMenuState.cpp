#include "MainMenuState.h"

#include "ClickableComponent.h"
#include "GameState.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "HitboxComponent.h"
#include "TextComponent.h"

namespace game
{

namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonSize = utils::Vector2f{23, 6};
}

MainMenuState::MainMenuState(const std::shared_ptr<window::Window>& window,
                             const std::shared_ptr<input::InputManager>& inputManager,
                             const std::shared_ptr<graphics::RendererPool>& rendererPool,
                             std::stack<std::unique_ptr<State>>& statesInit)
    : State{window, inputManager, rendererPool, statesInit}
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
    background = std::make_shared<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{80, 60}, utils::Vector2f{0, 0},
        utils::getProjectPath("chimarrao-platformer") + "resources/BG/menu_background.jpg",
        graphics::VisibilityLayer::Background);
}

void MainMenuState::createPlayGameButton()
{
    const auto gameButtonPosition = utils::Vector2f{50, 12};
    auto gameButton = std::make_shared<components::ComponentOwner>(gameButtonPosition);
    gameButton->addComponent<components::GraphicsComponent>(rendererPool, buttonSize, gameButtonPosition,
                                                            buttonColor, graphics::VisibilityLayer::First);
    gameButton->addComponent<components::TextComponent>(rendererPool, gameButtonPosition, "Play",
                                                        utils::getProjectPath("chimarrao-platformer") +
                                                            "resources/fonts/VeraMono.ttf",
                                                        35, graphics::Color::Black, utils::Vector2f{7, 1});

    auto runGame = [&] {
        states.push(std::make_unique<GameState>(window, inputManager, rendererPool, states));
    };

    gameButton->addComponent<components::HitboxComponent>(buttonSize);
    gameButton->addComponent<components::ClickableComponent>(inputManager, runGame);
    buttons.push_back(gameButton);
}

void MainMenuState::createMapEditorButton()
{
    const auto mapEditorButtonPosition = utils::Vector2f{50, 22};
    auto mapEditorButton = std::make_shared<components::ComponentOwner>(mapEditorButtonPosition);
    mapEditorButton->addComponent<components::GraphicsComponent>(rendererPool, buttonSize, mapEditorButtonPosition,
                                                            buttonColor, graphics::VisibilityLayer::First);
    mapEditorButton->addComponent<components::TextComponent>(rendererPool, mapEditorButtonPosition, "Map Editor",
                                                        utils::getProjectPath("chimarrao-platformer") +
                                                            "resources/fonts/VeraMono.ttf",
                                                        35, graphics::Color::Black, utils::Vector2f{1, 1});

    auto runMapEditor = [&] {
        // TODO: add map editor
        std::cout << "Map editor\n";
    };

    mapEditorButton->addComponent<components::HitboxComponent>(buttonSize);
    mapEditorButton->addComponent<components::ClickableComponent>(inputManager, runMapEditor);
    buttons.push_back(mapEditorButton);
}

void MainMenuState::createSettingsButton()
{
    const auto settingsButtonPosition = utils::Vector2f{50, 32};
    auto settingsButton = std::make_shared<components::ComponentOwner>(settingsButtonPosition);
    settingsButton->addComponent<components::GraphicsComponent>(rendererPool, buttonSize, settingsButtonPosition,
                                                                 buttonColor, graphics::VisibilityLayer::First);
    settingsButton->addComponent<components::TextComponent>(rendererPool, settingsButtonPosition, "Settings",
                                                             utils::getProjectPath("chimarrao-platformer") +
                                                             "resources/fonts/VeraMono.ttf",
                                                             35, graphics::Color::Black, utils::Vector2f{3, 1});

    auto runSettings = [&] {
      // TODO: add settings
      std::cout << "Settings\n";
    };

    settingsButton->addComponent<components::HitboxComponent>(buttonSize);
    settingsButton->addComponent<components::ClickableComponent>(inputManager, runSettings);
    buttons.push_back(settingsButton);
}

void MainMenuState::createExitButton()
{
    const auto exitButtonPosition = utils::Vector2f{50, 42};
    auto exitButton = std::make_shared<components::ComponentOwner>(exitButtonPosition);
    exitButton->addComponent<components::GraphicsComponent>(rendererPool, buttonSize, exitButtonPosition,
                                                                buttonColor, graphics::VisibilityLayer::First);
    exitButton->addComponent<components::TextComponent>(rendererPool, exitButtonPosition, "Exit",
                                                            utils::getProjectPath("chimarrao-platformer") +
                                                            "resources/fonts/VeraMono.ttf",
                                                            35, graphics::Color::Black, utils::Vector2f{7, 1});

    auto exit = [&] {
      // TODO: add exit
      std::cout << "Exit\n";
    };

    exitButton->addComponent<components::HitboxComponent>(buttonSize);
    exitButton->addComponent<components::ClickableComponent>(inputManager, exit);
    buttons.push_back(exitButton);
}

}