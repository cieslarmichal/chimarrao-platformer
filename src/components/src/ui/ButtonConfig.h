#pragma once

#include <boost/optional.hpp>
#include <string>

#include "Color.h"
#include "MouseOverActions.h"
#include "UniqueName.h"
#include "Vector.h"

namespace components::ui
{
struct ButtonConfig
{
    std::unique_ptr<utils::UniqueName> uniqueName;
    utils::Vector2f position;
    utils::Vector2f size;
    graphics::Color color;
    std::string text;
    unsigned int fontSize;
    std::string fontPath;
    utils::Vector2f textOffset;
    std::function<void(void)> clickAction;
    boost::optional<MouseOverActions> mouseOverActions;
};
}