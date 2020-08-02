#include "Game.h"

#include <iostream>

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
    window = graphicsFactory->createWindow(windowSize, "chimarrao");
    const utils::Vector2u mapSize{30, 30};

    rendererPool = graphicsFactory->createRendererPool(windowSize, mapSize);

    inputManager = std::make_unique<DefaultInputManager>(std::make_unique<DefaultInputObservationHandler>());
    timer.start();
    initStates();
}

void Game::run()
{
    while (window->isOpen())
    {
        processInput();
        update();
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
    states.push(std::make_unique<MainGameState>(window, *inputManager, rendererPool));
}

}