#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

#include "StringHelper.h"

namespace window
{
enum class DisplayMode
{
    Window,
    Fullscreen
};

inline std::string toString(DisplayMode displayMode)
{
    std::unordered_map<DisplayMode, std::string> displayModeToString{{DisplayMode::Window, "Window"},
                                                                     {DisplayMode::Fullscreen, "Fullscreen"}};

    return displayModeToString.at(displayMode);
}

inline DisplayMode toDisplayMode(const std::string& displayModeAsString)
{
    const auto displayModeLowerString = utils::getLowerCases(displayModeAsString);

    std::unordered_map<std::string, DisplayMode> stringToVisibilityLayer{
        {"window", DisplayMode::Window}, {"fullscreen", DisplayMode::Fullscreen}};

    return stringToVisibilityLayer.at(displayModeLowerString);
}

inline std::ostream& operator<<(std::ostream& os, DisplayMode displayMode)
{
    return os << "DisplayMode::" << toString(displayMode);
}
}