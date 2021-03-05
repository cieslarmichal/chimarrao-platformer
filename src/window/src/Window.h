#pragma once

#include "DisplayMode.h"
#include "ObservableWindow.h"
#include "Resolution.h"
#include "Vector.h"
#include "WindowSettings.h"

namespace sf
{
class View;
class Event;
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
    virtual bool pollEvent(sf::Event& event) const = 0;
    virtual utils::Vector2f getMousePosition() const = 0;
    virtual WindowSettings getWindowSettings() const = 0;
    virtual bool setDisplayMode(DisplayMode) = 0;
    virtual bool setVerticalSync(bool vsyncEnabled) = 0;
    virtual bool setFramerateLimit(unsigned int frameLimit) = 0;
    virtual bool setResolution(const Resolution&) = 0;
    virtual std::vector<Resolution> getSupportedResolutions() const = 0;
    virtual std::vector<unsigned int> getSupportedFrameLimits() const = 0;
};
}