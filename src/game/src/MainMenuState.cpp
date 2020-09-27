#include "MainMenuState.h"

#include "ClickableComponent.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "TextComponent.h"
#include "HitboxComponent.h"
#include "GameState.h"

namespace game
{

MainMenuState::MainMenuState(const std::shared_ptr<window::Window>& window,
                             const std::shared_ptr<input::InputManager>& inputManager,
                             const std::shared_ptr<graphics::RendererPool>& rendererPool,
                             std::stack<std::unique_ptr<State>>& statesInit)
    : State{window, inputManager, rendererPool, statesInit}
{

    background = std::make_shared<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{80, 60}, utils::Vector2f{0, 0},
        utils::getProjectPath("chimarrao-platformer") + "resources/BG/menu_background.jpg",
        graphics::VisibilityLayer::Background);

    const auto buttonColor = graphics::Color(251, 190, 102);

    const auto gameButtonPosition = utils::Vector2f{50, 15};
    const auto gameButtonSize = utils::Vector2f{18, 6};
    auto gameButton = std::make_shared<components::ComponentOwner>(gameButtonPosition);
    gameButton->addComponent<components::GraphicsComponent>(rendererPool, gameButtonSize,
                                                            gameButtonPosition, buttonColor,
                                                            graphics::VisibilityLayer::First);
    gameButton->addComponent<components::TextComponent>(rendererPool, gameButtonPosition, "Play",
                                                        utils::getProjectPath("chimarrao-platformer") +
                                                            "resources/fonts/VeraMono.ttf",
                                                        35, graphics::Color::Black, utils::Vector2f{5, 1});

    auto playGameAction = [&] {
        states.push(std::make_unique<GameState>(window, inputManager, rendererPool, states));
    };

    gameButton->addComponent<components::HitboxComponent>(gameButtonSize);
    gameButton->addComponent<components::ClickableComponent>(inputManager, playGameAction);
    buttons.push_back(gameButton);

    initialize();
}

void MainMenuState::initialize()
{
    for (auto& button : buttons)
    {
        button->loadDependentComponents();
    }

    for (auto& button : buttons)
    {
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

}