#include "WindowSfml.h"

#include <SFML/Window/Event.hpp>

namespace gui
{
WindowSfml::WindowSfml(const utils::Vector2u& windowSize, const std::string& windowTitle)
{
    window = std::make_unique<sf::RenderWindow>();
    window->create(sf::VideoMode(windowSize.x, windowSize.y), windowTitle);
    window->setVerticalSyncEnabled(true);
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
    sf::Event event{};

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

bool WindowSfml::pollEvent(sf::Event& event)
{
    return window->pollEvent(event);
}

}