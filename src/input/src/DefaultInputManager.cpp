#include "DefaultInputManager.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "InputKey.h"
#include "InputKeySfmlMapper.h"

namespace input
{
DefaultInputManager::DefaultInputManager(std::unique_ptr<Input> inputInit,
                                         std::shared_ptr<window::Window> windowInit)
    : input{std::move(inputInit)}, window{std::move(windowInit)}
{
}

const Input& DefaultInputManager::readInput()
{
    input->clearPressedKeys();

    for (const auto& keyboardKey : keyboardButtons)
    {
        if (sf::Keyboard::isKeyPressed(InputKeySfmlMapper::getKeyboardSfmlKey(keyboardKey)))
        {
            input->setKeyPressed(keyboardKey);
        }
    }

    for (const auto& mouseKey : mouseButtons)
    {
        if (sf::Mouse::isButtonPressed(InputKeySfmlMapper::getMouseSfmlKey(mouseKey)))
        {
            input->setKeyPressed(mouseKey);
        }
    }

    input->setReleasedKeys();
    input->setMousePosition(window->getMousePosition(true));

    return *input;
}
}