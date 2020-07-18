#pragma once

#include "SFML/Window.hpp"
#include "WindowProxy.h"

namespace graphics
{
class WindowProxySFML : public WindowProxy
{
private:
    std::unique_ptr<sf::Window> window;

public:
    ~WindowProxySFML();
    WindowProxySFML();
    void create(utils::Vector2i windowSize, std::string windowTitle) override;
    bool isOpen() const override;
    void display() override;
    void close() override;
    bool pollEvent(sf::Event& event) override;
};
}