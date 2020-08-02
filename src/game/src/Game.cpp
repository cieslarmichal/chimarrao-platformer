#include "Game.h"

#include <WindowProxySfml.h>

#include "DefaultInputManager.h"
#include "DefaultInputObservationHandler.h"
#include "Vector.h"
#include "WindowSfml.h"
#include "MainGameState.h"

namespace game
{
Game::Game()
{
    window = std::make_shared<graphics::WindowSFML>(utils::Vector2i(800, 600), "chimarrao",
                                                    std::make_unique<graphics::WindowProxySFML>());
    inputManager = std::make_unique<DefaultInputManager>(std::make_unique<DefaultInputObservationHandler>());
    timer.start();
    initStates();
}

void Game::run()
{
    while (window->isOpen())
    {
        processInput();
        dt = timer.getDurationFromLastUpdate();
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
//        states.top()->render(window.get());
    }
    window->display();
}

void Game::initStates()
{
    states.push(std::make_unique<MainGameState>(window, *inputManager));
}

}