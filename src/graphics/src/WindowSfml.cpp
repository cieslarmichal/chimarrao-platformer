#include "WindowSfml.h"

#include "WindowProxySfml.h"

namespace graphics {
WindowSFML::WindowSFML(utils::Vector2i windowSize, std::string windowTitle, std::unique_ptr<WindowProxy> windowProxy)
    : window(std::move(windowProxy))
{
    window->create(windowSize,windowTitle);
}

bool WindowSFML::isOpen() const
{
    return window->isOpen();
}

void WindowSFML::display() {
    window->display();
}

void WindowSFML::update() {
    sf::Event event;

    while(window->pollEvent(event))
    {
        if(sf::Event::Closed == event.type)
        {
            window->close();
        }
        if(sf::Event::Resized == event.type)
        {
            //TODO: implement resize
        }
    }
}
}