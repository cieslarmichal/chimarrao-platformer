#include "DefaultInputManager.h"

#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

#include "InputKey.h"

namespace input
{
DefaultInputManager::DefaultInputManager(std::unique_ptr<InputObservationHandler> handler,
                                         std::shared_ptr<window::Window> windowInit)
    : observationHandler{std::move(handler)}, window{std::move(windowInit)}
{
}

void DefaultInputManager::readInput()
{
    inputStatus.clearPressedKeys();

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

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        inputStatus.setKeyPressed(InputKey::MouseLeft);
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        inputStatus.setKeyPressed(InputKey::MouseRight);
    }

    inputStatus.setReleasedKeys();
    inputStatus.setMousePosition(window->getMousePosition());
    notifyObservers();
}

void DefaultInputManager::registerObserver(InputObserver* observer)
{
    observationHandler->registerObserver(observer);
}

void DefaultInputManager::removeObserver(InputObserver* observer)
{
    observationHandler->removeObserver(observer);
}

void DefaultInputManager::notifyObservers()
{
    observationHandler->notifyObservers(inputStatus);
}

}