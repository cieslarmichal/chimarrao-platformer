#include "WindowSfml.h"

#include <SFML/Window/Event.hpp>

namespace gui
{
WindowSfml::WindowSfml(const utils::Vector2u& windowSize, const std::string& windowTitle)
{
    window = std::make_unique<sf::RenderWindow>();
    window->create(sf::VideoMode(windowSize.x, windowSize.y), windowTitle);
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(120);
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
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }
        if (event.type == sf::Event::Resized)
        {
            notifyObservers();
        }
    }
}

void WindowSfml::close()
{
    window->close();
}

utils::Vector2f WindowSfml::getMousePosition() const
{
    sf::Vector2i windowCoordinates = sf::Mouse::getPosition(*window);
    sf::Vector2f worldCoordinates = window->mapPixelToCoords(windowCoordinates);
    return worldCoordinates;
}

bool WindowSfml::pollEvent(sf::Event& event)
{
    return window->pollEvent(event);
}

void WindowSfml::registerObserver(WindowObserver* observer)
{
    observers.push_back(observer);
}

void WindowSfml::removeObserver(WindowObserver* observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void WindowSfml::notifyObservers()
{
    for (const auto& observer : observers)
    {
        if (observer)
        {
            observer->windowSizeChanged(window->getSize());
        }
    }
}

}