#pragma once

#include <SFML/Window/Event.hpp>
#include <Vector.h>
#include <string>

namespace graphics
{
class WindowProxy
{
public:
    virtual ~WindowProxy() = default;
    virtual void create(utils::Vector2u windowSize, std::string windowTitle) = 0;

    virtual bool isOpen() const = 0;
    virtual void display() = 0;
    virtual void close() = 0;
    virtual void clear() = 0;
    virtual bool pollEvent(sf::Event& event) = 0;
};
}