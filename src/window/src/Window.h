#pragma once

#include "ObservableWindow.h"
#include "Vector.h"

namespace sf
{
class View;
}

namespace window
{
class Window : public ObservableWindow
{
public:
    virtual ~Window() = default;

    virtual bool isOpen() const = 0;
    virtual void display() = 0;
    virtual void update() = 0;
    virtual void close() = 0;
    virtual void setView(const sf::View&) = 0;
    virtual utils::Vector2f getMousePosition() const = 0;
};
}