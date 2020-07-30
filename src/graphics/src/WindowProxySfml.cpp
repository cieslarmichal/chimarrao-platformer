#include "WindowProxySfml.h"

namespace graphics
{
WindowProxySFML::WindowProxySFML()
{
    window = std::make_unique<sf::RenderWindow>();
}

void WindowProxySFML::create(utils::Vector2i windowSize, std::string windowTitle)
{
    window->create(sf::VideoMode(windowSize.x, windowSize.y), windowTitle);
}

bool WindowProxySFML::isOpen() const
{
    return window->isOpen();
}

void WindowProxySFML::display()
{
    window->display();
}

void WindowProxySFML::close()
{
    window->close();
}

void WindowProxySFML::clear()
{
    window->clear();
}

bool WindowProxySFML::pollEvent(sf::Event& event)
{
    return window->pollEvent(event);
}

const sf::RenderWindow& WindowProxySFML::getRenderWindow() const
{
    return *window;
}

}