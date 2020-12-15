#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <unordered_map>

#include "InputKey.h"

namespace input
{
class InputKeySfmlMapper
{
public:
    static sf::Keyboard::Key getKeyboardSfmlKey(InputKey);
    static sf::Mouse::Button getMouseSfmlKey(InputKey);

private:
    static std::unordered_map<InputKey, sf::Keyboard::Key> keyboardKeys;
    static std::unordered_map<InputKey, sf::Mouse::Button> mouseKeys;
};
}