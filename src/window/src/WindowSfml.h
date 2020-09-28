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
    WindowSfml(const utils::Vector2u& windowSize, const std::string& windowTitle,
               std::unique_ptr<WindowObservationHandler>);

    bool isOpen() const override;
    void display() override;
    void update() override;
    void close() override;
    void setView(const sf::View&) override;
    utils::Vector2f getMousePosition() const override;
    bool pollEvent(sf::Event& event);
    void registerObserver(WindowObserver*) override;
    void removeObserver(WindowObserver*) override;

private:
    void notifyObservers() override;

    std::unique_ptr<WindowObservationHandler> observationHandler;
    std::unique_ptr<sf::RenderWindow> window;
};
}