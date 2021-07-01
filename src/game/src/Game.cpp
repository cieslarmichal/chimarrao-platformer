#include "Game.h"

#include <chrono>
#include <thread>

namespace game
{

Game::Game(std::shared_ptr<window::Window> windowInit, std::shared_ptr<input::InputManager> inputManagerInit,
           std::unique_ptr<States> statesInit)
    : window{std::move(windowInit)}, inputManager{std::move(inputManagerInit)}, states{std::move(statesInit)}
{
    states->addNextState(StateType::Menu);
}

void Game::run()
{
    while (window->isOpen())
    {
        std::this_thread::sleep_for(std::chrono::duration<double, std::nano>(1));
        if (const auto gameState = update(); gameState == StatesStatus::Exit)
        {
            window->close();
            break;
        }
        render();
    }
}

StatesStatus Game::update()
{
    const auto deltaTime = timer.getDurationFromLastUpdate();
    const auto& input = inputManager->readInput();
    const auto statesStatus = states->updateCurrentState(deltaTime, input);
    return statesStatus;
}

void Game::render()
{
    states->render();
    window->update();
    window->display();
}

}