#include "DefaultInputManager.h"

#include <utility>

#include "SFML/Window/Keyboard.hpp"

#include "InputKey.h"

namespace input
{
DefaultInputManager::DefaultInputManager(std::unique_ptr<InputObservationHandler> handler, std::shared_ptr<gui::Window> windowInit)
    : observerHandler{std::move(handler)}, window{std::move(windowInit)}
{
}

void DefaultInputManager::readInput()
{
    inputStatus.clearStatus();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        inputStatus.setKeyPressed(InputKey::Up);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        inputStatus.setKeyPressed(InputKey::Down);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        inputStatus.setKeyPressed(InputKey::Left);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        inputStatus.setKeyPressed(InputKey::Right);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        inputStatus.setKeyPressed(InputKey::Space);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        inputStatus.setKeyPressed(InputKey::Shift);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        inputStatus.setKeyPressed(InputKey::Enter);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        inputStatus.setKeyPressed(InputKey::Escape);
    }

    inputStatus.setMousePosition(window->getMousePosition());

    notifyObservers();
}

void DefaultInputManager::registerObserver(InputObserver* observer)
{
    observerHandler->registerObserver(observer);
}

void DefaultInputManager::removeObserver(InputObserver* observer)
{
    observerHandler->removeObserver(observer);
}

void DefaultInputManager::notifyObservers()
{
    observerHandler->notifyObservers(inputStatus);
}

}