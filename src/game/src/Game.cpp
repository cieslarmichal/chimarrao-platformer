#include "Game.h"

#include <WindowProxySfml.h>

#include "DefaultInputManager.h"
#include "DefaultInputObservationHandler.h"
#include "Vector.h"
#include "WindowSfml.h"

namespace game
{
Game::Game()
{
    window = std::make_unique<graphics::WindowSFML>(utils::Vector2i(800, 600), "chimarrao",
                                                    std::make_unique<graphics::WindowProxySFML>());
    inputManager = std::make_unique<DefaultInputManager>(std::make_unique<DefaultInputObservationHandler>());
    timer.start();
}

void Game::run()
{
    while (window->isOpen())
    {
        processInput();
        update(timer.getDurationFromLastUpdate());
        render();
    }
}

void Game::processInput()
{
    inputManager->readInput();
}

void Game::update(utils::DeltaTime dt)
{
    window->update();
}

void Game::render()
{
    window->display();
}
}