#include "DefaultInputManager.h"

#include <iostream>

#include "SFML/Window/Keyboard.hpp"

namespace game
{

DefaultInputManager::DefaultInputManager(std::unique_ptr<ObserverHandler> handler) : observerHandler{std::move(handler)}
{

}

void DefaultInputManager::readInput()
{
    KeyboardStatus keyboardStatus;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        std::cout<<"x";
        keyboardStatus.setUpPressed();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        keyboardStatus.setDownPressed();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        keyboardStatus.setLeftPressed();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        keyboardStatus.setRightPressed();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        keyboardStatus.setSpacePressed();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        keyboardStatus.setShiftPressed();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        keyboardStatus.setEnterPressed();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        keyboardStatus.setEscapePressed();
    }

    notifyObservers(keyboardStatus);
}

void DefaultInputManager::registerObserver(InputObserver* observer)
{
    observerHandler->registerObserver(observer);
}

void DefaultInputManager::removeObserver(InputObserver* observer)
{
    observerHandler->removeObserver(observer);
}

void DefaultInputManager::notifyObservers(const KeyboardStatus& keyboardStatus)
{
    observerHandler->notifyObservers(keyboardStatus);
}

}