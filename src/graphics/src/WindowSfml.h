#pragma once

#include "SFML/Window.hpp"
#include "Window.h"
#include "Vector.h"

namespace graphics
{
class WindowSFML : public Window
{
private:
    sf::Window window;
public:
    WindowSFML(utils::Vector2i windowSize, std::string windowTitle);

    bool isOpen() const override;
    void display() override;
    void update() override;
};
}