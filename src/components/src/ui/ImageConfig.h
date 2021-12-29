#pragma once

#include <boost/optional.hpp>
#include <string>
#include <utility>

#include "Color.h"
#include "KeyAction.h"
#include "UniqueName.h"
#include "Vector.h"
#include "VisibilityLayer.h"

namespace components::ui
{
struct ImageConfig
{
    ImageConfig(std::string uniqueNameInit, utils::Vector2f positionInit, utils::Vector2f sizeInit,
                graphics::VisibilityLayer visibilityLayerInit, std::string texturePathInit)
        : uniqueName{std::move(uniqueNameInit)},
          position{positionInit},
          size{sizeInit},
          visibilityLayer{visibilityLayerInit},
          texturePath{std::move(texturePathInit)},
          color{boost::none}
    {
    }

    ImageConfig(std::string uniqueNameInit, utils::Vector2f positionInit, utils::Vector2f sizeInit,
                graphics::VisibilityLayer visibilityLayerInit, graphics::Color color)
        : uniqueName{std::move(uniqueNameInit)},
          position{positionInit},
          size{sizeInit},
          visibilityLayer{visibilityLayerInit},
          texturePath{boost::none},
          color{color}
    {
    }

    const std::string uniqueName;
    const utils::Vector2f position;
    const utils::Vector2f size;
    const graphics::VisibilityLayer visibilityLayer;
    const boost::optional<std::string> texturePath;
    const boost::optional<graphics::Color> color;
};
}