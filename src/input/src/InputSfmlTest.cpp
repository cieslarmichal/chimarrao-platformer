#include "InputSfml.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace input;

namespace
{
const utils::Vector2f position{10.2f, 3.0f};
const utils::Vector2f zeroPosition{0.0f, 0.0f};
}

class InputSfmlTest : public Test
{
public:
    InputSfml input;
};

TEST_F(InputSfmlTest, initialInput_pressedKeysAndReleasedKeysAndMousePositionShouldBeSetToZero)
{
    for (const auto& key : allKeys)
    {
        ASSERT_FALSE(input.isKeyPressed(key));
        ASSERT_FALSE(input.isKeyReleased(key));
    }

    ASSERT_EQ(input.getMouseRelativePosition(), zeroPosition);
}

TEST_F(InputSfmlTest, shouldSetKeyPressed)
{
    for (const auto& key : allKeys)
    {
        input.setKeyPressed(key);
        ASSERT_TRUE(input.isKeyPressed(key));
    }
}

TEST_F(InputSfmlTest, shouldClearPressedKeys)
{
    for (const auto& key : allKeys)
    {
        input.setKeyPressed(key);
    }

    input.clearPressedKeys();

    for (const auto& key : allKeys)
    {
        ASSERT_FALSE(input.isKeyPressed(key));
    }
}

TEST_F(InputSfmlTest, shouldSetRelativeMousePosition)
{
    input.setMouseRelativePosition(position);

    ASSERT_EQ(input.getMouseRelativePosition(), position);
}

TEST_F(InputSfmlTest, shouldSetAbsoluteMousePosition)
{
    input.setMouseAbsolutePosition(position);

    ASSERT_EQ(input.getMouseAbsolutePosition(), position);
}

TEST_F(InputSfmlTest, afterKeysPressed_shouldNotSetReleasedKey)
{
    input.setKeyPressed(InputKey::Up);
    input.setKeyPressed(InputKey::MouseRight);

    input.setReleasedKeys();

    ASSERT_FALSE(input.isKeyReleased(InputKey::Up));
    ASSERT_FALSE(input.isKeyReleased(InputKey::MouseRight));
}

TEST_F(InputSfmlTest, afterKeysPressedAndCleared_shouldSetReleasedKey)
{
    input.setKeyPressed(InputKey::Up);
    input.setKeyPressed(InputKey::MouseLeft);
    input.setReleasedKeys();
    input.clearPressedKeys();

    input.setReleasedKeys();

    ASSERT_TRUE(input.isKeyReleased(InputKey::Up));
    ASSERT_TRUE(input.isKeyReleased(InputKey::MouseLeft));
}
