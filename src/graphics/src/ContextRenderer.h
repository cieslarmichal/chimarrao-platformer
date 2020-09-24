#pragma once

#include <memory>

#include "RectangleShape.h"
#include "Vector.h"

namespace graphics
{
class ContextRenderer
{
public:
    virtual ~ContextRenderer() = default;

    virtual void initialize() = 0;
    virtual void clear(const Color&) = 0;
    virtual void draw(const sf::Drawable&) = 0;
    virtual void setView() = 0;
    virtual void setViewSize(const utils::Vector2u& windowsSize) = 0;
};
}