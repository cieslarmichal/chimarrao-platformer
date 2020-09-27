#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

#include "Vector.h"
#include "Window.h"

namespace window
{
class WindowSfml : public Window
{
public:
    WindowSfml(const utils::Vector2u& windowSize, const std::string& windowTitle);

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

    std::vector<WindowObserver*> observers;
    std::unique_ptr<sf::RenderWindow> window;
};
}