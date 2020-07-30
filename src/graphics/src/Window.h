#pragma once

namespace graphics
{

class Window
{
public:
    virtual ~Window() = default;

    virtual bool isOpen() const = 0;
    virtual void display() = 0;
    virtual void update() = 0;
    virtual void clear() = 0;
    virtual void close() = 0;
};
}