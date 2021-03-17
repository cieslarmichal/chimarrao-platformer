#pragma once

#include <boost/optional.hpp>
#include <string>
#include <utility>

#include "Color.h"
#include "MouseOverActions.h"
#include "UniqueName.h"
#include "Vector.h"

namespace components::ui
{
struct ButtonConfig
{
    ButtonConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit, utils::Vector2f sizeInit,
                 graphics::Color buttonColorInit, const std::string& textInit, graphics::Color textColorInit,
                 unsigned int fontSizeInit, const std::string& fontPathInit, utils::Vector2f textOffsetInit,
                 std::function<void(void)> clickActionInit, const MouseOverActions& mouseOverActionsInit)
        : uniqueName{uniqueNameInit},
          position{positionInit},
          size{sizeInit},
          buttonColor{buttonColorInit},
          text{textInit},
          textColor{textColorInit},
          fontSize{fontSizeInit},
          fontPath{fontPathInit},
          textOffset{textOffsetInit},
          clickAction{std::move(clickActionInit)},
          mouseOverActions{mouseOverActionsInit}
    {
    }

    ButtonConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit, utils::Vector2f sizeInit,
                 graphics::Color buttonColorInit, const std::string& textInit, graphics::Color textColorInit,
                 unsigned int fontSizeInit, const std::string& fontPathInit, utils::Vector2f textOffsetInit,
                 std::function<void(void)> clickActionInit)
        : uniqueName{uniqueNameInit},
          position{positionInit},
          size{sizeInit},
          buttonColor{buttonColorInit},
          text{textInit},
          textColor{textColorInit},
          fontSize{fontSizeInit},
          fontPath{fontPathInit},
          textOffset{textOffsetInit},
          clickAction{std::move(clickActionInit)},
          mouseOverActions{boost::none}
    {
    }

    ButtonConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit, utils::Vector2f sizeInit,
                 graphics::Color buttonColorInit, const std::string& textInit, graphics::Color textColorInit,
                 unsigned int fontSizeInit, const std::string& fontPathInit, utils::Vector2f textOffsetInit)
        : uniqueName{uniqueNameInit},
          position{positionInit},
          size{sizeInit},
          buttonColor{buttonColorInit},
          text{textInit},
          textColor{textColorInit},
          fontSize{fontSizeInit},
          fontPath{fontPathInit},
          textOffset{textOffsetInit},
          clickAction{boost::none},
          mouseOverActions{boost::none}
    {
    }

    const utils::UniqueName uniqueName;
    const utils::Vector2f position;
    const utils::Vector2f size;
    const graphics::Color buttonColor;
    const std::string text;
    const graphics::Color textColor;
    const unsigned int fontSize;
    const std::string fontPath;
    const utils::Vector2f textOffset;
    const boost::optional<std::function<void(void)>> clickAction;
    const boost::optional<MouseOverActions> mouseOverActions;
};
}