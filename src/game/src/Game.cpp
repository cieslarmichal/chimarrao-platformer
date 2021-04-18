#include "Game.h"

#include <chrono>
#include <thread>

#include "DefaultStates.h"
#include "GraphicsFactory.h"
#include "InputManagerFactory.h"
#include "MenuState.h"
#include "Vector.h"
#include "WindowFactory.h"

namespace game
{
namespace
{
const int rendererPoolSizeX = 80;
const int rendererPoolSizeY = 60;
const int tileSizeX = 4;
const int tileSizeY = 4;
}

// TODO: inject dependencies to Game constructor and create GameFactory
Game::Game() : deltaTime{0}
{
    auto graphicsFactory = graphics::GraphicsFactory::createGraphicsFactory();
    auto windowFactory = window::WindowFactory::createWindowFactory();
    auto inputManagerFactory = input::InputManagerFactory::createInputManagerFactory();

    auto windowSize = utils::Vector2u{800, 600};
    window = windowFactory->createWindow(windowSize, "chimarrao-platformer");

    const utils::Vector2u mapSize{80, 60};

    rendererPool = graphicsFactory->createRendererPool(window, windowSize, mapSize);
    inputManager = inputManagerFactory->createInputManager(window);
    tileMap = std::make_unique<TileMap>(
        "", utils::Vector2i(rendererPoolSizeX / tileSizeX * 2, rendererPoolSizeY / tileSizeY));
    states = std::make_unique<DefaultStates>(window, rendererPool, *tileMap);
    timer.start();
    states->addNextState(StateType::Menu);
}

void Game::run()
{
    while (window->isOpen())
    {
        std::this_thread::sleep_for(std::chrono::duration<double, std::nano>(1));
        update();
        render();
    }
}

void Game::update()
{
    deltaTime = timer.getDurationFromLastUpdate();

    const auto& input = inputManager->readInput();
    const auto statesStatus = states->updateCurrentState(deltaTime, input);
    if (statesStatus == StatesStatus::Exit)
    {
        window->close();
    }
}

void Game::render()
{
    states->render();
    window->update();
    window->display();
}

}