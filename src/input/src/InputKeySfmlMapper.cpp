#include "InputKeySfmlMapper.h"

#include "exceptions/InputKeyNotFound.h"

namespace input
{

std::unordered_map<InputKey, sf::Keyboard::Key> InputKeySfmlMapper::keyboardKeys{
    {InputKey::Up, sf::Keyboard::Up},
    {InputKey::Down, sf::Keyboard::Down},
    {InputKey::Left, sf::Keyboard::Left},
    {InputKey::Right, sf::Keyboard::Right},
    {InputKey::Space, sf::Keyboard::Space},
    {InputKey::Shift, sf::Keyboard::LShift},
    {InputKey::Enter, sf::Keyboard::Enter},
    {InputKey::Escape, sf::Keyboard::Escape},
    {InputKey::Backspace, sf::Keyboard::Backspace},
    {InputKey::Number0, sf::Keyboard::Num0},
    {InputKey::Number1, sf::Keyboard::Num1},
    {InputKey::Number2, sf::Keyboard::Num2},
    {InputKey::Number3, sf::Keyboard::Num3},
    {InputKey::Number4, sf::Keyboard::Num4},
    {InputKey::Number5, sf::Keyboard::Num5},
    {InputKey::Number6, sf::Keyboard::Num6},
    {InputKey::Number7, sf::Keyboard::Num7},
    {InputKey::Number8, sf::Keyboard::Num8},
    {InputKey::Number9, sf::Keyboard::Num9},
    {InputKey::Q, sf::Keyboard::Q},
    {InputKey::W, sf::Keyboard::W},
    {InputKey::E, sf::Keyboard::E},
    {InputKey::R, sf::Keyboard::R},
    {InputKey::T, sf::Keyboard::T},
    {InputKey::Y, sf::Keyboard::Y},
    {InputKey::U, sf::Keyboard::U},
    {InputKey::I, sf::Keyboard::I},
    {InputKey::O, sf::Keyboard::O},
    {InputKey::P, sf::Keyboard::P},
    {InputKey::A, sf::Keyboard::A},
    {InputKey::S, sf::Keyboard::S},
    {InputKey::D, sf::Keyboard::D},
    {InputKey::F, sf::Keyboard::F},
    {InputKey::G, sf::Keyboard::G},
    {InputKey::H, sf::Keyboard::H},
    {InputKey::J, sf::Keyboard::J},
    {InputKey::K, sf::Keyboard::K},
    {InputKey::L, sf::Keyboard::L},
    {InputKey::Z, sf::Keyboard::Z},
    {InputKey::X, sf::Keyboard::X},
    {InputKey::C, sf::Keyboard::C},
    {InputKey::V, sf::Keyboard::V},
    {InputKey::B, sf::Keyboard::B},
    {InputKey::N, sf::Keyboard::N},
    {InputKey::M, sf::Keyboard::M}};

std::unordered_map<InputKey, sf::Mouse::Button> InputKeySfmlMapper::mouseKeys{
    {InputKey::MouseRight, sf::Mouse::Right}, {InputKey::MouseLeft, sf::Mouse::Left}};

sf::Keyboard::Key InputKeySfmlMapper::getKeyboardSfmlKey(InputKey inputKey)
{
    try
    {
        return keyboardKeys.at(inputKey);
    }
    catch (const std::out_of_range&)
    {
        throw exceptions::InputKeyNotFound{toString(inputKey) + " is not a keyboard key"};
    }
}

sf::Mouse::Button InputKeySfmlMapper::getMouseSfmlKey(InputKey inputKey)
{
    try
    {
        return mouseKeys.at(inputKey);
    }
    catch (const std::out_of_range&)
    {
        throw exceptions::InputKeyNotFound{toString(inputKey) + " is not a mouse key"};
    }
}

}