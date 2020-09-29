#include "Game.h"

#include "DefaultInputManager.h"
#include "DefaultInputObservationHandler.h"
#include "GameState.h"
#include "EditorState.h"
#include "GraphicsFactory.h"
#include "MainMenuState.h"
#include "Vector.h"
#include "WindowFactory.h"

namespace game
{
Game::Game() : dt{0}
{
    auto graphicsFactory = graphics::GraphicsFactory::createGraphicsFactory();
    auto windowFactory = window::WindowFactory::createWindowFactory();

    auto windowSize = utils::Vector2u{800, 600};
    window = windowFactory->createWindow(windowSize, "chimarrao-platformer");

    const utils::Vector2u mapSize{80, 60};

    rendererPool = graphicsFactory->createRendererPool(window, windowSize, mapSize);
    inputManager = std::make_unique<input::DefaultInputManager>(
        std::make_unique<input::DefaultInputObservationHandler>(), window);
    timer.start();
    initStates();
}

void Game::run()
{
    while (window->isOpen())
    {
        processInput();
        update();
        lateUpdate();
        render();
    }
}

void Game::processInput()
{
    inputManager->readInput();
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
        std::cout << states.top()->getName() << std::endl;
        states.top()->update(dt);
    }
}

void Game::lateUpdate()
{
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
    states.push(std::make_unique<MainMenuState>(window, inputManager, rendererPool, states));
//    states.push(std::make_unique<EditorState>(window, inputManager, rendererPool, states));
}

}