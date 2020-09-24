#pragma once

#include <memory>

#include "Vector.h"
#include "Window.h"

namespace gui
{
class GuiFactory
{
public:
    virtual ~GuiFactory() = default;

    virtual std::unique_ptr<Window> createWindow(const utils::Vector2u& windowSize,
                                                 const std::string& title) const = 0;

    static std::unique_ptr<GuiFactory> createGuiFactory();
};
}