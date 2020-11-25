#pragma once

#include "DisplayMode.h"
#include "Resolution.h"

namespace window
{
struct WindowSettings
{
    DisplayMode displayMode;
    Resolution resolution;
    bool vsync;
    unsigned int frameLimit;
};

inline bool operator==(const WindowSettings& lhs, const WindowSettings& rhs)
{
    auto tieStruct = [](const WindowSettings& windowSettings) {
        return std::tie(windowSettings.displayMode, windowSettings.resolution, windowSettings.vsync,
                        windowSettings.frameLimit);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::ostream& operator<<(std::ostream& os, const WindowSettings& windowSettings)
{
    return os << "displayMode: " << windowSettings.displayMode << " resolution: " << windowSettings.resolution
              << " vsync: " << windowSettings.vsync << " frameLimit: " << windowSettings.frameLimit;
}
}