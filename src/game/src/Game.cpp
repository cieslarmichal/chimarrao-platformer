#include "Game.h"

#include "DefaultInputManager.h"
#include "DefaultInputObservationHandler.h"
#include "GraphicsFactory.h"
#include "MainGameState.h"
#include "Vector.h"

namespace game
{
Game::Game()
{
    auto graphicsFactory = graphics::GraphicsFactory::createGraphicsFactory();

    auto windowSize = utils::Vector2u{800, 600};
    window = graphicsFactory->createWindow(windowSize, "chimarrao-platformer");
    const utils::Vector2u mapSize{30, 30};

    rendererPool = graphicsFactory->createRendererPool(windowSize, mapSize);
    inputManager = std::make_unique<input::DefaultInputManager>(std::make_unique<input::DefaultInputObservationHandler>());
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
        states.top()->update(dt);
    }
    window->update();
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
    window->update();
}

void Game::render()
{
    if (not states.empty())
    {
        states.top()->render();
    }
    window->display();
}

void Game::initStates()
{
    states.push(std::make_unique<MainGameState>(window, inputManager, rendererPool));
}

}