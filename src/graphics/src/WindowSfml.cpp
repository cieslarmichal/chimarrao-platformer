#include "WindowSfml.h"

#include <utility>

#include "WindowProxySfml.h"

namespace graphics
{
WindowSfml::WindowSfml(utils::Vector2u windowSize, std::string windowTitle,
                       std::unique_ptr<WindowProxy> windowProxy)
    : window(std::move(windowProxy))
{
    window->create(windowSize, std::move(windowTitle));
}

bool WindowSfml::isOpen() const
{
    return window->isOpen();
}

void WindowSfml::display()
{
    window->display();
}

void WindowSfml::update()
{
    sf::Event event;

    // TODO: not a place for tracking events
    while (window->pollEvent(event))
    {
        if (sf::Event::Closed == event.type)
        {
            window->close();
        }
        if (sf::Event::Resized == event.type)
        {
            // TODO: implement resize
        }
    }
}

void WindowSfml::close()
{
    window->close();
}
}