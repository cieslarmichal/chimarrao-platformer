#pragma once

#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "StringHelper.h"
#include "exceptions/InputKeyNotFound.h"

namespace input
{
enum class InputKey
{
    Up,
    Down,
    Left,
    Right,
    Space,
    Shift,
    Enter,
    Escape,
    MouseLeft,
    MouseRight,
};

const std::vector<InputKey> allKeys{
    InputKey::Up,    InputKey::Down,  InputKey::Left,   InputKey::Right,     InputKey::Space,
    InputKey::Shift, InputKey::Enter, InputKey::Escape, InputKey::MouseLeft, InputKey::MouseRight};

inline std::string toString(InputKey inputKey)
{
    std::unordered_map<InputKey, std::string> inputKeyToString{{InputKey::Up, "Up"},
                                                               {InputKey::Down, "Down"},
                                                               {InputKey::Left, "Left"},
                                                               {InputKey::Right, "Right"},
                                                               {InputKey::Space, "Space"},
                                                               {InputKey::Shift, "Shift"},
                                                               {InputKey::Enter, "Enter"},
                                                               {InputKey::Escape, "Escape"},
                                                               {InputKey::MouseLeft, "MouseLeft"},
                                                               {InputKey::MouseRight, "MouseRight"}};

    try
    {
        return inputKeyToString.at(inputKey);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::InputKeyNotFound{e.what()};
    }
}

inline InputKey toInputKey(const std::string& inputKeyAsString)
{
    const auto inputKeyLowerString = utils::getLowerCases(inputKeyAsString);

    std::unordered_map<std::string, InputKey> stringToInputKey{{"up", InputKey::Up},
                                                               {"down", InputKey::Down},
                                                               {"left", InputKey::Left},
                                                               {"right", InputKey::Right},
                                                               {"space", InputKey::Space},
                                                               {"shift", InputKey::Shift},
                                                               {"enter", InputKey::Enter},
                                                               {"escape", InputKey::Escape},
                                                               {"mouseleft", InputKey::MouseLeft},
                                                               {"mouseright", InputKey::MouseRight}};

    try
    {
        return stringToInputKey.at(inputKeyLowerString);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::InputKeyNotFound{e.what()};
    }
}

inline std::ostream& operator<<(std::ostream& os, InputKey inputKey)
{
    return os << "InputKey::" << toString(inputKey);
}
}