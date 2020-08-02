#pragma once

#include "SFML/Window.hpp"
#include "Vector.h"
#include "Window.h"
#include "WindowProxy.h"

namespace graphics
{
class WindowSfml : public Window
{
public:
    WindowSfml(utils::Vector2u windowSize, std::string windowTitle, std::unique_ptr<WindowProxy> window);

    bool isOpen() const override;
    void display() override;
    void update() override;
    void clear() override;
    void close() override;

private:
    std::unique_ptr<WindowProxy> window;
};
}