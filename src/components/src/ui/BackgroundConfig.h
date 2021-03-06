#pragma once

#include <boost/optional.hpp>
#include <string>

#include "Color.h"
#include "UniqueName.h"
#include "Vector.h"

namespace components::ui
{
struct BackgroundConfig
{
    std::unique_ptr<utils::UniqueName> uniqueName;
    utils::Vector2f position;
    utils::Vector2f size;
    boost::optional<std::string> texturePath;
    boost::optional<graphics::Color> color;
};
}