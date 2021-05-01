#pragma once

#include <boost/optional.hpp>
#include <string>
#include <utility>

#include "Color.h"
#include "UniqueName.h"
#include "Vector.h"
#include "VisibilityLayer.h"
#include "core/KeyAction.h"

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
          texturePath{std::move(texturePathInit)}
    {
    }

    const std::string uniqueName;
    const utils::Vector2f position;
    const utils::Vector2f size;
    const graphics::VisibilityLayer visibilityLayer;
    const std::string texturePath;
};
}