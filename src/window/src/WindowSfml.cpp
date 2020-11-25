#include "WindowSfml.h"

#include <SFML/Window/Event.hpp>
#include <utility>
#include "SupportedFrameLimitsRetriever.h"
#include "SupportedResolutionsRetriever.h"

namespace window
{
WindowSfml::WindowSfml(const utils::Vector2u& windowSize, std::string windowTitleInit,
                       std::unique_ptr<WindowObservationHandler> observationHandlerInit)
    : observationHandler{std::move(observationHandlerInit)}, windowTitle{std::move(windowTitleInit)}
{
    windowSettings.displayMode = DisplayMode::Window;
    windowSettings.resolution = Resolution{windowSize.x, windowSize.y};
    windowSettings.vsync = true;
    windowSettings.frameLimit = 120;

    window = std::make_unique<sf::RenderWindow>();
    window->create(sf::VideoMode(windowSettings.resolution.width, windowSettings.resolution.height),
                   windowTitle);
    window->setVerticalSyncEnabled(windowSettings.vsync);
    window->setFramerateLimit(windowSettings.frameLimit);
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

WindowSettings WindowSfml::getWindowSettings() const
{
    return windowSettings;
}

void WindowSfml::setDisplayMode(DisplayMode displayMode)
{
    windowSettings.displayMode = displayMode;
    window.reset();
    window = std::make_unique<sf::RenderWindow>();
    window->create(sf::VideoMode(windowSettings.resolution.width, windowSettings.resolution.height),
                   windowTitle, getSfmlStyleFromDisplayMode(windowSettings.displayMode));
}

void WindowSfml::setVerticalSync(bool enabled)
{
    windowSettings.vsync = enabled;
    window->setVerticalSyncEnabled(enabled);
}

void WindowSfml::setFramerateLimit(unsigned int frameLimit)
{
    windowSettings.frameLimit = frameLimit;
    window->setFramerateLimit(frameLimit);
}

void WindowSfml::setResolution(const Resolution& resolution)
{
    windowSettings.resolution = resolution;
    window.reset();
    window = std::make_unique<sf::RenderWindow>();
    window->create(sf::VideoMode(resolution.width, resolution.height), windowTitle,
                   getSfmlStyleFromDisplayMode(windowSettings.displayMode));
}

unsigned WindowSfml::getSfmlStyleFromDisplayMode(DisplayMode displayMode) const
{
    if (displayMode == DisplayMode::Window)
    {
        return sf::Style::Resize | sf::Style::Close;
    }
    else
    {
        return sf::Style::Fullscreen;
    }
}

std::vector<Resolution> WindowSfml::getSupportedResolutions() const
{
    return SupportedResolutionsRetriever::retrieveSupportedResolutions();
}

std::vector<unsigned int> WindowSfml::getSupportedFrameLimits() const
{
    return SupportedFrameLimitsRetriever::retrieveSupportedFrameLimits();
}

}