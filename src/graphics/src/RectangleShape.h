#pragma once

#include "SFML/Graphics/RectangleShape.hpp"

#include "Color.h"
#include "GraphicsId.h"
#include "Vector.h"

namespace graphics
{
class RectangleShape : public sf::RectangleShape
{
public:
    RectangleShape(GraphicsId, const utils::Vector2f& size, const utils::Vector2f& position, const Color&);

    utils::Vector2f getPosition() const;
    utils::Vector2f getSize() const;
    GraphicsId getGraphicsId() const;

private:
    GraphicsId id;
};

inline bool operator==(const RectangleShape& lhs, const RectangleShape& rhs)
{
    return lhs.getGraphicsId() == rhs.getGraphicsId();
}
}