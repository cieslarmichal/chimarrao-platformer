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
    const sf::Vector2i windowCoordinates = sf::Mouse::getPosition(*window);
    const sf::Vector2f worldCoordinates = window->mapPixelToCoords(windowCoordinates);
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

bool WindowSfml::setDisplayMode(DisplayMode displayMode)
{
    if (displayMode != windowSettings.displayMode)
    {
        windowSettings.displayMode = displayMode;
        window.reset();
        window = std::make_unique<sf::RenderWindow>();

        if (displayMode == DisplayMode::Fullscreen)
        {
            const auto highestAvailableResolution =
                SupportedResolutionsRetriever::retrieveHighestResolution();
            windowSettings.resolution = highestAvailableResolution;
            window->create(sf::VideoMode(highestAvailableResolution.width, highestAvailableResolution.height),
                           windowTitle, sf::Style::None);
        }
        else
        {
            window->create(sf::VideoMode(windowSettings.resolution.width, windowSettings.resolution.height),
                           windowTitle, getSfmlStyleFromDisplayMode(windowSettings.displayMode));
        }

        //        window->setSize({window->getSize() + utils::Vector2u {1,1}});
        //        window->setSize({window->getSize() - utils::Vector2u {1,1}});
        notifyObservers();
        return true;
    }
    return false;
}

bool WindowSfml::setVerticalSync(bool vsyncEnabled)
{
    if (vsyncEnabled != windowSettings.vsync)
    {
        windowSettings.vsync = vsyncEnabled;
        window->setVerticalSyncEnabled(vsyncEnabled);
        return true;
    }
    return false;
}

bool WindowSfml::setFramerateLimit(unsigned int frameLimit)
{
    if (frameLimit != windowSettings.frameLimit)
    {
        windowSettings.frameLimit = frameLimit;
        window->setFramerateLimit(frameLimit);
        return true;
    }
    return false;
}

bool WindowSfml::setResolution(const Resolution& resolution)
{
    if (resolution != windowSettings.resolution)
    {
        windowSettings.resolution = resolution;
        window.reset();
        window = std::make_unique<sf::RenderWindow>();
        window->create(sf::VideoMode(resolution.width, resolution.height), windowTitle,
                       getSfmlStyleFromDisplayMode(windowSettings.displayMode));
        window->setSize({window->getSize() + utils::Vector2u{1, 1}});
        window->setSize({window->getSize() - utils::Vector2u{1, 1}});
        notifyObservers();
        return true;
    }
    return false;
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