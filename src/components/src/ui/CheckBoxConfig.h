#pragma once

#include <boost/optional.hpp>
#include <functional>
#include <string>

#include "Color.h"
#include "MouseOverActions.h"
#include "UniqueName.h"
#include "Vector.h"

namespace components::ui
{
struct CheckBoxConfig
{
    std::unique_ptr<utils::UniqueName> uniqueName;
    utils::Vector2f position;
    utils::Vector2f size;
    graphics::Color color;
    bool checked;
    unsigned int fontSize;
    std::string fontPath;
    utils::Vector2f textOffset;
    std::function<void(void)> clickAction;
    boost::optional<MouseOverActions> mouseOverActions;
};
}