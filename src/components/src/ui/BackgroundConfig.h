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
struct BackgroundConfig
{
    BackgroundConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit,
                     utils::Vector2f sizeInit, graphics::VisibilityLayer visibilityLayerInit,
                     const std::string& texturePathInit)
        : uniqueName{uniqueNameInit},
          position{positionInit},
          size{sizeInit},
          visibilityLayer{visibilityLayerInit},
          texturePath{texturePathInit},
          color{boost::none}
    {
    }

    BackgroundConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit,
                     utils::Vector2f sizeInit, graphics::VisibilityLayer visibilityLayerInit,
                     const std::string& texturePathInit,
                     std::vector<components::core::KeyAction> keyActionsInit)
        : uniqueName{uniqueNameInit},
          position{positionInit},
          size{sizeInit},
          visibilityLayer{visibilityLayerInit},
          texturePath{texturePathInit},
          color{boost::none},
          keyActions{std::move(keyActionsInit)}
    {
    }

    BackgroundConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit,
                     utils::Vector2f sizeInit, graphics::VisibilityLayer visibilityLayerInit,
                     graphics::Color color)
        : uniqueName{uniqueNameInit},
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
    const std::vector<components::core::KeyAction> keyActions;
};
}