#include "WindowProxySfml.h"

namespace graphics
{
WindowProxySFML::WindowProxySFML()
{
    window = std::make_unique<sf::Window>();
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
bool WindowProxySFML::pollEvent(sf::Event& event)
{
    return window->pollEvent(event);
}
WindowProxySFML::~WindowProxySFML() {
}

}