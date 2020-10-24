#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

#include "Vector.h"
#include "Window.h"
#include "WindowObservationHandler.h"

namespace window
{
class WindowSfml : public Window
{
public:
    WindowSfml(const utils::Vector2u& windowSize, std::string  windowTitle,
               std::unique_ptr<WindowObservationHandler>);

    bool isOpen() const override;
    void display() override;
    void update() override;
    void close() override;
    void setView(const sf::View&) override;
    bool pollEvent(sf::Event& event) const override;
    utils::Vector2f getMousePosition() const override;
    void registerObserver(WindowObserver*) override;
    void removeObserver(WindowObserver*) override;
    void setVerticalSync(bool enabled) override;
    void setFramerateLimit(unsigned int frameLimit) override;
    void setVideoMode(const sf::VideoMode&) override;

private:
    void notifyObservers() override;

    const std::string windowTitle;
    bool verticalSync;
    unsigned int framerateLimit;
    std::unique_ptr<WindowObservationHandler> observationHandler;
    std::unique_ptr<sf::RenderWindow> window;
};
}