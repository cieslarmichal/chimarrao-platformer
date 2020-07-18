#include "WindowSfml.h"

namespace graphics {
WindowSFML::WindowSFML(utils::Vector2i windowSize, std::string windowTitle) {
    window.create(sf::VideoMode(windowSize.x, windowSize.y),windowTitle);
}

bool WindowSFML::isOpen() const
{
    return window.isOpen();
}

void WindowSFML::display() {
    window.display();
}

void WindowSFML::update() {
    //TODO: implement
}
}