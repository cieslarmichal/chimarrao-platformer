#include "Game.h"

#include <chrono>
#include <thread>

#include "GraphicsFactory.h"
#include "InputManagerFactory.h"
#include "MenuState.h"
#include "Vector.h"
#include "WindowFactory.h"
#include "ui/DefaultUIManager.h"

namespace game
{
Game::Game() : dt{0}
{
    auto graphicsFactory = graphics::GraphicsFactory::createGraphicsFactory();
    auto windowFactory = window::WindowFactory::createWindowFactory();
    auto inputManagerFactory = input::InputManagerFactory::createInputManagerFactory();

    auto windowSize = utils::Vector2u{800, 600};
    window = windowFactory->createWindow(windowSize, "chimarrao-platformer");

    const utils::Vector2u mapSize{80, 60};

    rendererPool = graphicsFactory->createRendererPool(window, windowSize, mapSize);
    inputManager = inputManagerFactory->createInputManager(window);
    timer.start();
    initStates();
}

void Game::run()
{
    while (window->isOpen())
    {
        std::this_thread::sleep_for(std::chrono::duration<double, std::nano>(1));
        update();
        lateUpdate();
        render();
    }
}

void Game::update()
{
    dt = timer.getDurationFromLastUpdate();

    if (states.empty())
    {
        window->close();
    }
    else
    {
        const auto& input = inputManager->readInput();
        auto nextState = states.top()->update(dt, input);

        if (nextState == NextState::Previous)
        {
            backToThePreviousState();
        }
        else if (nextState == NextState::Menu)
        {
            backToTheMenuState();
        }
    }
}

void Game::lateUpdate()
{
    // TODO: problem with two different dt within update and lateUpdate
    dt = timer.getDurationFromLastUpdate();

    if (states.empty())
    {
        window->close();
    }
    else
    {
        states.top()->lateUpdate(dt);
    }
}

void Game::render()
{
    if (not states.empty())
    {
        states.top()->render();
    }

    window->update();
    window->display();
}

void Game::initStates()
{
    states.push(std::make_unique<MenuState>(
        window, rendererPool, states, std::make_unique<components::ui::DefaultUIManager>(rendererPool)));
}

void Game::backToThePreviousState()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void Game::backToTheMenuState()
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

}