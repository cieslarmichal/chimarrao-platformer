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
    virtual const utils::Vector2f& getViewSize() = 0;
    virtual void setViewSize(const utils::Vector2u& windowsSize) = 0;
    virtual void synchronizeViewSize() = 0;
    virtual void setCenter(const utils::Vector2f&) = 0;
    virtual const utils::Vector2f& getCenter() const = 0;
};
}