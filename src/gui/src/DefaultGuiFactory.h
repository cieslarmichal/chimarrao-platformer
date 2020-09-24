#pragma once

#include <memory>

#include "GuiFactory.h"

namespace gui
{
class DefaultGuiFactory : public GuiFactory
{
public:
    std::unique_ptr<Window> createWindow(const utils::Vector2u& windowSize,
                                         const std::string& title) const override;
};
}
