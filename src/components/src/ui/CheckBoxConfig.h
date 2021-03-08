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
    CheckBoxConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit, utils::Vector2f sizeInit,
                 graphics::Color colorInit, bool checkedInit, unsigned int fontSizeInit,
                 const std::string& fontPathInit, utils::Vector2f textOffsetInit,
                 std::function<void(void)> clickActionInit, const MouseOverActions& mouseOverActionsInit)
        : uniqueName{uniqueNameInit},
          position{positionInit},
          size{sizeInit},
          color{colorInit},
          checked{checkedInit},
          fontSize{fontSizeInit},
          fontPath{fontPathInit},
          textOffset{textOffsetInit},
          clickAction{std::move(clickActionInit)},
          mouseOverActions{mouseOverActionsInit}
    {
    }

    CheckBoxConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit, utils::Vector2f sizeInit,
                   graphics::Color colorInit, bool checkedInit, unsigned int fontSizeInit,
                   const std::string& fontPathInit, utils::Vector2f textOffsetInit,
                   std::function<void(void)> clickActionInit)
        : uniqueName{uniqueNameInit},
          position{positionInit},
          size{sizeInit},
          color{colorInit},
          checked{checkedInit},
          fontSize{fontSizeInit},
          fontPath{fontPathInit},
          textOffset{textOffsetInit},
          clickAction{std::move(clickActionInit)},
          mouseOverActions{boost::none}
    {
    }

    const utils::UniqueName uniqueName;
    const utils::Vector2f position;
    const utils::Vector2f size;
    const graphics::Color color;
    const bool checked;
    const unsigned int fontSize;
    const std::string fontPath;
    const utils::Vector2f textOffset;
    const std::function<void(void)> clickAction;
    const boost::optional<MouseOverActions> mouseOverActions;
};
}