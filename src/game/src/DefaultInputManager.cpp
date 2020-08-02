#include "DefaultInputManager.h"

#include <iostream>

#include "InputKey.h"
#include "SFML/Window/Keyboard.hpp"

namespace game
{

DefaultInputManager::DefaultInputManager(std::unique_ptr<InputObservationHandler> handler)
    : observerHandler{std::move(handler)}
{
}

void DefaultInputManager::readInput()
{
    inputKeysStatus.clearStatus();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        inputKeysStatus.setKeyPressed(InputKey::Up);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        inputKeysStatus.setKeyPressed(InputKey::Down);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        inputKeysStatus.setKeyPressed(InputKey::Left);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        inputKeysStatus.setKeyPressed(InputKey::Right);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        inputKeysStatus.setKeyPressed(InputKey::Space);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        inputKeysStatus.setKeyPressed(InputKey::Shift);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        inputKeysStatus.setKeyPressed(InputKey::Enter);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        inputKeysStatus.setKeyPressed(InputKey::Escape);
    }

    notifyObservers(inputKeysStatus);
}

void DefaultInputManager::registerObserver(InputObserver* observer)
{
    observerHandler->registerObserver(observer);
}

void DefaultInputManager::removeObserver(InputObserver* observer)
{
    observerHandler->removeObserver(observer);
}

void DefaultInputManager::notifyObservers(const InputStatus& keyboardStatus)
{
    observerHandler->notifyObservers(keyboardStatus);
}

}