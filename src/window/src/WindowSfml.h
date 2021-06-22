#pragma once

#include <SFML/Window/Event.hpp>

#include "SFML/Graphics/RenderWindow.hpp"

#include "Vector.h"
#include "Window.h"
#include "WindowObservationHandler.h"
#include "WindowSettings.h"

namespace window
{
class WindowSfml : public Window
{
public:
    WindowSfml(const utils::Vector2u& windowSize, std::string windowTitle,
               std::unique_ptr<WindowObservationHandler>);

    bool isOpen() const override;
    void display() override;
    void update() override;
    void close() override;
    void setView(const sf::View&) override;
    bool pollEvent(sf::Event& event) const override;
    utils::Vector2f getMousePosition(bool = false) const override;
    WindowSettings getWindowSettings() const override;
    bool setDisplayMode(DisplayMode) override;
    bool setVerticalSync(bool vsyncEnabled) override;
    bool setFramerateLimit(unsigned int frameLimit) override;
    bool setResolution(const Resolution&) override;
    std::vector<Resolution> getSupportedResolutions() const override;
    std::vector<unsigned int> getSupportedFrameLimits() const override;
    void registerObserver(WindowObserver*) override;
    void removeObserver(WindowObserver*) override;

private:
    void notifyObservers() override;
    unsigned getSfmlStyleFromDisplayMode(DisplayMode) const;

    const std::string windowTitle;
    WindowSettings windowSettings;
    std::unique_ptr<WindowObservationHandler> observationHandler;
    std::unique_ptr<sf::RenderWindow> window;
};
}