#pragma once

#include <memory>

#include "InputApi.h"
#include "InputManager.h"
#include "Window.h"

namespace input
{
class INPUT_API InputManagerFactory
{
public:
    virtual ~InputManagerFactory() = default;

    virtual std::unique_ptr<InputManager>
    createInputManager(std::shared_ptr<window::Window> window) const = 0;

    static std::unique_ptr<InputManagerFactory> createInputManagerFactory();
};
}
