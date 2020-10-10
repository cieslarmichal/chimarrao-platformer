#include "WindowSfml.h"

#include <SFML/Window/Event.hpp>

namespace window
{
WindowSfml::WindowSfml(const utils::Vector2u& windowSize, const std::string& windowTitle,
                       std::unique_ptr<WindowObservationHandler> observationHandlerInit)
    : observationHandler{std::move(observationHandlerInit)}
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
    // TODO: sf::Event in update param
    sf::Event event{};

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

void WindowSfml::setView(const sf::View& view)
{
    window->setView(view);
}

bool WindowSfml::pollEvent(sf::Event& event) const
{
    return window->pollEvent(event);
}

utils::Vector2f WindowSfml::getMousePosition() const
{
    sf::Vector2i windowCoordinates = sf::Mouse::getPosition(*window);
    sf::Vector2f worldCoordinates = window->mapPixelToCoords(windowCoordinates);
    return worldCoordinates;
}

void WindowSfml::registerObserver(WindowObserver* observer)
{
    observationHandler->registerObserver(observer);
}

void WindowSfml::removeObserver(WindowObserver* observer)
{
    observationHandler->removeObserver(observer);
}

void WindowSfml::notifyObservers()
{
    observationHandler->notifyObservers(window->getSize());
}

}