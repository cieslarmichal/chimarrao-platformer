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
    Backspace,
    MouseLeft,
    MouseRight,
    Number0,
    Number1,
    Number2,
    Number3,
    Number4,
    Number5,
    Number6,
    Number7,
    Number8,
    Number9,
    Q,
    W,
    E,
    R,
    T,
    Y,
    U,
    I,
    O,
    P,
    A,
    S,
    D,
    F,
    G,
    H,
    J,
    K,
    L,
    Z,
    X,
    C,
    V,
    B,
    N,
    M,
    Tab,
};

const std::vector<InputKey> allKeys{
    InputKey::Up,         InputKey::Down,    InputKey::Left,    InputKey::Right,     InputKey::Space,
    InputKey::Shift,      InputKey::Enter,   InputKey::Escape,  InputKey::Backspace, InputKey::MouseLeft,
    InputKey::MouseRight, InputKey::Number0, InputKey::Number1, InputKey::Number2,   InputKey::Number3,
    InputKey::Number4,    InputKey::Number5, InputKey::Number6, InputKey::Number7,   InputKey::Number8,
    InputKey::Number9,    InputKey::Q,       InputKey::W,       InputKey::E,         InputKey::R,
    InputKey::T,          InputKey::Y,       InputKey::U,       InputKey::I,         InputKey::O,
    InputKey::P,          InputKey::A,       InputKey::S,       InputKey::D,         InputKey::F,
    InputKey::G,          InputKey::H,       InputKey::J,       InputKey::K,         InputKey::L,
    InputKey::Z,          InputKey::X,       InputKey::C,       InputKey::V,         InputKey::B,
    InputKey::N,          InputKey::M,       InputKey::Tab};

const std::vector<InputKey> keyboardButtons{
    InputKey::Up,      InputKey::Down,    InputKey::Left,    InputKey::Right,     InputKey::Space,
    InputKey::Shift,   InputKey::Enter,   InputKey::Escape,  InputKey::Backspace, InputKey::Number0,
    InputKey::Number1, InputKey::Number2, InputKey::Number3, InputKey::Number4,   InputKey::Number5,
    InputKey::Number6, InputKey::Number7, InputKey::Number8, InputKey::Number9,   InputKey::Q,
    InputKey::W,       InputKey::E,       InputKey::R,       InputKey::T,         InputKey::Y,
    InputKey::U,       InputKey::I,       InputKey::O,       InputKey::P,         InputKey::A,
    InputKey::S,       InputKey::D,       InputKey::F,       InputKey::G,         InputKey::H,
    InputKey::J,       InputKey::K,       InputKey::L,       InputKey::Z,         InputKey::X,
    InputKey::C,       InputKey::V,       InputKey::B,       InputKey::N,         InputKey::M,
    InputKey::Tab};

const std::vector<InputKey> alphaNumericalButtons{
    InputKey::Number0, InputKey::Number1, InputKey::Number2, InputKey::Number3, InputKey::Number4,
    InputKey::Number5, InputKey::Number6, InputKey::Number7, InputKey::Number8, InputKey::Number9,
    InputKey::Q,       InputKey::W,       InputKey::E,       InputKey::R,       InputKey::T,
    InputKey::Y,       InputKey::U,       InputKey::I,       InputKey::O,       InputKey::P,
    InputKey::A,       InputKey::S,       InputKey::D,       InputKey::F,       InputKey::G,
    InputKey::H,       InputKey::J,       InputKey::K,       InputKey::L,       InputKey::Z,
    InputKey::X,       InputKey::C,       InputKey::V,       InputKey::B,       InputKey::N,
    InputKey::M};

const std::vector<InputKey> mouseButtons{InputKey::MouseLeft, InputKey::MouseRight};

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
                                                               {InputKey::Backspace, "Backspace"},
                                                               {InputKey::MouseLeft, "MouseLeft"},
                                                               {InputKey::MouseRight, "MouseRight"},
                                                               {InputKey::Number0, "0"},
                                                               {InputKey::Number1, "1"},
                                                               {InputKey::Number2, "2"},
                                                               {InputKey::Number3, "3"},
                                                               {InputKey::Number4, "4"},
                                                               {InputKey::Number5, "5"},
                                                               {InputKey::Number6, "6"},
                                                               {InputKey::Number7, "7"},
                                                               {InputKey::Number8, "8"},
                                                               {InputKey::Number9, "9"},
                                                               {InputKey::Q, "Q"},
                                                               {InputKey::W, "W"},
                                                               {InputKey::E, "E"},
                                                               {InputKey::R, "R"},
                                                               {InputKey::T, "T"},
                                                               {InputKey::Y, "Y"},
                                                               {InputKey::U, "U"},
                                                               {InputKey::I, "I"},
                                                               {InputKey::O, "O"},
                                                               {InputKey::P, "P"},
                                                               {InputKey::A, "A"},
                                                               {InputKey::S, "S"},
                                                               {InputKey::D, "D"},
                                                               {InputKey::F, "F"},
                                                               {InputKey::G, "G"},
                                                               {InputKey::H, "H"},
                                                               {InputKey::J, "J"},
                                                               {InputKey::K, "K"},
                                                               {InputKey::L, "L"},
                                                               {InputKey::Z, "Z"},
                                                               {InputKey::X, "X"},
                                                               {InputKey::C, "C"},
                                                               {InputKey::V, "V"},
                                                               {InputKey::B, "B"},
                                                               {InputKey::N, "N"},
                                                               {InputKey::M, "M"},
                                                               {InputKey::Tab, "Tab"}};

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
    const auto inputKeyLowerString = utils::StringHelper::getLowerCases(inputKeyAsString);

    std::unordered_map<std::string, InputKey> stringToInputKey{
        {"up", InputKey::Up},
        {"down", InputKey::Down},
        {"left", InputKey::Left},
        {"right", InputKey::Right},
        {"space", InputKey::Space},
        {"shift", InputKey::Shift},
        {"enter", InputKey::Enter},
        {"escape", InputKey::Escape},
        {"backspace", InputKey::Backspace},
        {"mouseleft", InputKey::MouseLeft},
        {"mouseright", InputKey::MouseRight},
        {"0", InputKey::Number0},
        {"1", InputKey::Number1},
        {"2", InputKey::Number2},
        {"3", InputKey::Number3},
        {"4", InputKey::Number4},
        {"5", InputKey::Number5},
        {"6", InputKey::Number6},
        {"7", InputKey::Number7},
        {"8", InputKey::Number8},
        {"9", InputKey::Number9},
        {"q", InputKey::Q},
        {"w", InputKey::W},
        {"e", InputKey::E},
        {"r", InputKey::R},
        {"t", InputKey::T},
        {"y", InputKey::Y},
        {"u", InputKey::U},
        {"i", InputKey::I},
        {"o", InputKey::O},
        {"p", InputKey::P},
        {"a", InputKey::A},
        {"s", InputKey::S},
        {"d", InputKey::D},
        {"f", InputKey::F},
        {"g", InputKey::G},
        {"h", InputKey::H},
        {"j", InputKey::J},
        {"k", InputKey::K},
        {"l", InputKey::L},
        {"z", InputKey::Z},
        {"x", InputKey::X},
        {"c", InputKey::C},
        {"v", InputKey::V},
        {"b", InputKey::B},
        {"n", InputKey::N},
        {"m", InputKey::M},
        {"tab", InputKey::Tab},
    };

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