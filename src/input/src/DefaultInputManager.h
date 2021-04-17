#pragma once

#include <memory>

#include "InputManager.h"
#include "Window.h"

namespace input
{
class DefaultInputManager : public InputManager
{
public:
    explicit DefaultInputManager(std::unique_ptr<Input>, std::shared_ptr<window::Window>);

    const Input& readInput() override;

private:
    std::unique_ptr<Input> input;
    std::shared_ptr<window::Window> window;
};
}