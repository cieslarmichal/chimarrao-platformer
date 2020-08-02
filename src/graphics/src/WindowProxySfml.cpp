#include "WindowProxySfml.h"

namespace graphics
{
WindowProxySfml::WindowProxySfml()
{
    window = std::make_unique<sf::RenderWindow>();
}

void WindowProxySfml::create(utils::Vector2u windowSize, std::string windowTitle)
{
    window->create(sf::VideoMode(windowSize.x, windowSize.y), windowTitle);
}

bool WindowProxySfml::isOpen() const
{
    return window->isOpen();
}

void WindowProxySfml::display()
{
    window->display();
}

void WindowProxySfml::close()
{
    window->close();
}

void WindowProxySfml::clear()
{
    window->clear();
}

bool WindowProxySfml::pollEvent(sf::Event& event)
{
    return window->pollEvent(event);
}

}