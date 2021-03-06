#pragma once

#include <string>

#include "Color.h"
#include "UniqueName.h"
#include "Vector.h"

namespace components::ui
{
struct LabelConfig
{
    std::unique_ptr<utils::UniqueName> uniqueName;
    utils::Vector2f position;
    graphics::Color color;
    std::string text;
    unsigned int fontSize;
    std::string fontPath;
};
}