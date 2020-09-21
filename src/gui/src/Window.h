#pragma once

namespace gui
{

class Window
{
public:
    virtual ~Window() = default;

    virtual bool isOpen() const = 0;
    virtual void display() = 0;
    virtual void update() = 0;
    virtual void close() = 0;
};
}