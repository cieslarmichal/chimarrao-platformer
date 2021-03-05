#include "DefaultInputManager.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "InputKey.h"
#include "InputKeySfmlMapper.h"

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

    for (const auto& keyboardKey : keyboardButtons)
    {
        if (sf::Keyboard::isKeyPressed(InputKeySfmlMapper::getKeyboardSfmlKey(keyboardKey)))
        {
            inputStatus.setKeyPressed(keyboardKey);
        }
    }

    for (const auto& mouseKey : mouseButtons)
    {
        if (sf::Mouse::isButtonPressed(InputKeySfmlMapper::getMouseSfmlKey(mouseKey)))
        {
            inputStatus.setKeyPressed(mouseKey);
        }
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