#pragma once

#include <memory>

#include "Vector.h"
#include "Window.h"
#include "WindowApi.h"

namespace window
{
class WINDOW_API WindowFactory
{
public:
    virtual ~WindowFactory() = default;

    virtual std::unique_ptr<Window> createWindow(const utils::Vector2u& windowSize,
                                                 const std::string& title) const = 0;

    static std::unique_ptr<WindowFactory> createWindowFactory();
};
}