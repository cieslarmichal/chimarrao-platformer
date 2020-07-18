#include "KeyboardStatus.h"

namespace game
{

namespace
{
const auto up = InputKey::Up;
auto down = InputKey::Down;
auto left = InputKey::Left;
auto right = InputKey::Right;
auto space = InputKey::Space;
auto shift = InputKey::Shift;
auto enter = InputKey::Enter;
auto escape = InputKey::Escape;
auto pressed = KeyState::Pressed;
auto unpressed = KeyState::Unpressed;
}

KeyboardStatus::KeyboardStatus()
{
    keyStates[up] = unpressed;
    keyStates[down] = unpressed;
    keyStates[left] = unpressed;
    keyStates[right] = unpressed;
    keyStates[space] = unpressed;
    keyStates[shift] = unpressed;
    keyStates[enter] = unpressed;
    keyStates[escape] = unpressed;
}

std::map<InputKey, KeyState> KeyboardStatus::getKeyStates() const
{
    return keyStates;
}

bool KeyboardStatus::isUpPressed() const
{
    return keyStates.at(up) == pressed;
}

bool KeyboardStatus::isDownPressed() const
{
    return isPressed(down);
}

bool KeyboardStatus::isLeftPressed() const
{
    return isPressed(left);
}

bool KeyboardStatus::isRightPressed() const
{
    return isPressed(right);
}

bool KeyboardStatus::isSpacePressed() const
{
    return isPressed(space);
}

bool KeyboardStatus::isShiftPressed() const
{
    return isPressed(shift);
}

bool KeyboardStatus::isEnterPressed() const
{
    return isPressed(enter);
}

bool KeyboardStatus::isEscapePressed() const
{
    return isPressed(escape);
}

void KeyboardStatus::setUpPressed()
{
    keyStates[up] = pressed;
}

void KeyboardStatus::setDownPressed()
{
    keyStates[down] = pressed;
}

void KeyboardStatus::setLeftPressed()
{
    keyStates[left] = pressed;
}

void KeyboardStatus::setRightPressed()
{
    keyStates[right] = pressed;
}

void KeyboardStatus::setSpacePressed()
{
    keyStates[space] = pressed;
}

void KeyboardStatus::setShiftPressed()
{
    keyStates[shift] = pressed;
}

void KeyboardStatus::setEnterPressed()
{
    keyStates[enter] = pressed;
}

void KeyboardStatus::setEscapePressed()
{
    keyStates[escape] = pressed;
}

bool KeyboardStatus::isPressed(InputKey inputKey) const
{
    return keyStates.at(inputKey) == pressed;
}


}