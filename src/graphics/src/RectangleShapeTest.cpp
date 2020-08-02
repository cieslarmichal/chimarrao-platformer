#include "RectangleShape.h"

#include "gtest/gtest.h"

#include "GraphicsIdGenerator.h"

using namespace graphics;
using namespace ::testing;

namespace
{
    const utils::Vector2f position{3.0f, 2.0f};
    const utils::Vector2f size{200, 100};
    const auto color{Color::Magenta};
    auto shapeId = GraphicsIdGenerator::generateId();
}

class RectangleShapeTest : public Test
{
public:
    RectangleShape rectangleShape{shapeId, size, position, color};
};

TEST_F(RectangleShapeTest, getRendererId)
{
    EXPECT_EQ(shapeId, rectangleShape.getGraphicsId());
}

TEST_F(RectangleShapeTest, getPosition)
{
    EXPECT_EQ(position, rectangleShape.getPosition());
}

TEST_F(RectangleShapeTest, getSize)
{
    EXPECT_EQ(size, rectangleShape.getSize());
}