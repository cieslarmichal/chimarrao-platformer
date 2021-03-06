#pragma once

#include <functional>
#include <string>

#include "Color.h"
#include "UniqueName.h"
#include "Vector.h"

namespace components::ui
{
struct TextFieldConfig
{
    std::unique_ptr<utils::UniqueName> uniqueName;
    utils::Vector2f position;
    utils::Vector2f size;
    graphics::Color color;
    std::string text;
    unsigned int fontSize;
    std::string fontPath;
    utils::Vector2f textOffset;
    std::function<void(void)> clickInFieldAction;
    std::function<void(std::string)> clickOutsideFieldAction;
};
}