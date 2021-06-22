#pragma once

#include <functional>
#include <string>

#include "Color.h"
#include "MouseOverActions.h"
#include "UniqueName.h"
#include "Vector.h"

namespace components::ui
{
struct TextFieldConfig
{
    TextFieldConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit, utils::Vector2f sizeInit,
                    graphics::Color colorInit, const std::string& textInit, unsigned int fontSizeInit,
                    const std::string& fontPathInit, utils::Vector2f textOffsetInit,
                    std::function<void(void)> clickInFieldActionInit,
                    std::function<void(std::string)> clickOutsideFieldActionInit)
        : uniqueName{uniqueNameInit},
          position{positionInit},
          size{sizeInit},
          color{colorInit},
          text{textInit},
          fontSize{fontSizeInit},
          fontPath{fontPathInit},
          textOffset{textOffsetInit},
          clickInFieldAction{std::move(clickInFieldActionInit)},
          clickOutsideFieldAction{std::move(clickOutsideFieldActionInit)},
          mouseOverActions{boost::none}
    {
    }

    TextFieldConfig(const std::string& uniqueNameInit, utils::Vector2f positionInit, utils::Vector2f sizeInit,
                    graphics::Color colorInit, const std::string& textInit, unsigned int fontSizeInit,
                    const std::string& fontPathInit, utils::Vector2f textOffsetInit,
                    std::function<void(void)> clickInFieldActionInit,
                    std::function<void(std::string)> clickOutsideFieldActionInit,
                    const MouseOverActions& mouseOverActionsInit)
        : uniqueName{uniqueNameInit},
          position{positionInit},
          size{sizeInit},
          color{colorInit},
          text{textInit},
          fontSize{fontSizeInit},
          fontPath{fontPathInit},
          textOffset{textOffsetInit},
          clickInFieldAction{std::move(clickInFieldActionInit)},
          clickOutsideFieldAction{std::move(clickOutsideFieldActionInit)},
          mouseOverActions{mouseOverActionsInit}
    {
    }

    const std::string uniqueName;
    const utils::Vector2f position;
    const utils::Vector2f size;
    const graphics::Color color;
    const std::string text;
    const unsigned int fontSize;
    const std::string fontPath;
    const utils::Vector2f textOffset;
    const std::function<void(void)> clickInFieldAction;
    const std::function<void(std::string)> clickOutsideFieldAction;
    const boost::optional<MouseOverActions> mouseOverActions;
};
}