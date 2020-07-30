#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "WindowProxy.h"

namespace graphics
{
class WindowProxySFML : public WindowProxy
{
private:
    std::unique_ptr<sf::RenderWindow> window;

public:
    WindowProxySFML();

    void create(utils::Vector2i windowSize, std::string windowTitle) override;
    bool isOpen() const override;
    void display() override;
    void close() override;
    void clear() override;
    bool pollEvent(sf::Event& event) override;
    const sf::RenderWindow& getRenderWindow() const;
};
}