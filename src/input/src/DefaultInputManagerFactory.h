#pragma once

#include <memory>

#include "InputManagerFactory.h"

namespace input
{
class DefaultInputManagerFactory : public InputManagerFactory
{
public:
    std::unique_ptr<InputManager> createInputManager(std::shared_ptr<window::Window> window) const override;
};
}
