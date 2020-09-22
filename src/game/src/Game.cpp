#include "Game.h"

#include "DefaultInputManager.h"
#include "DefaultInputObservationHandler.h"
#include "GameState.h"
#include "GraphicsFactory.h"
#include "GuiFactory.h"
#include "MainMenuState.h"
#include "Vector.h"

namespace game
{
Game::Game() : dt{0}
{
    auto graphicsFactory = graphics::GraphicsFactory::createGraphicsFactory();
    auto guiFactory = gui::GuiFactory::createGuiFactory();

    auto windowSize = utils::Vector2u{800, 600};
    window = guiFactory->createWindow(windowSize, "chimarrao-platformer");
    const utils::Vector2u mapSize{80, 40};

    rendererPool = graphicsFactory->createRendererPool(windowSize, mapSize);
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
    states.push(std::make_unique<GameState>(window, inputManager, rendererPool));
    //    states.push(std::make_unique<MainGameState>(window, inputManager, rendererPool));
}

}