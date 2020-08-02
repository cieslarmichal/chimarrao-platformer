#pragma once

#include "SFML/Window.hpp"
#include "WindowProxy.h"

namespace graphics
{
class WindowProxySfml : public WindowProxy
{
public:
    WindowProxySfml();

    void create(utils::Vector2u windowSize, std::string windowTitle) override;
    bool isOpen() const override;
    void display() override;
    void close() override;
    bool pollEvent(sf::Event& event) override;

private:
    sf::Window window;
};
}