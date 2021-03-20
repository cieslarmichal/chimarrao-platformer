#pragma once

#include <string>

#include "Color.h"
#include "UniqueName.h"
#include "Vector.h"

namespace components::ui
{
struct LabelConfig
{
    LabelConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit, graphics::Color colorInit,
                const std::string& textInit, unsigned int fontSizeInit, const std::string& fontPathInit)
        : uniqueName{uniqueNameInit},
          position{positionInit},
          color{colorInit},
          text{textInit},
          fontSize{fontSizeInit},
          fontPath{fontPathInit}
    {
    }

    const std::string uniqueName;
    const utils::Vector2f position;
    const graphics::Color color;
    const std::string text;
    const unsigned int fontSize;
    const std::string fontPath;
};
}