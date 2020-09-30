#include "InputStatus.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace input;

namespace
{
const utils::Vector2f position{10.2, 3.0};
const utils::Vector2f zeroPosition{0.0, 0.0};
}

class InputStatusTest : public Test
{
public:
    InputStatus inputStatus;
};

TEST_F(InputStatusTest, initialInputStatus_pressedKeysAndReleasedKeysAndMousePositionShouldBeSetToZero)
{
    for (const auto& key : allKeys)
    {
        ASSERT_FALSE(inputStatus.isKeyPressed(key));
        ASSERT_FALSE(inputStatus.isKeyReleased(key));
    }

    ASSERT_EQ(inputStatus.getMousePosition(), zeroPosition);
}

TEST_F(InputStatusTest, shouldSetKeyPressed)
{
    for (const auto& key : allKeys)
    {
        inputStatus.setKeyPressed(key);
        ASSERT_TRUE(inputStatus.isKeyPressed(key));
    }
}

TEST_F(InputStatusTest, shouldClearPressedKeys)
{
    for (const auto& key : allKeys)
    {
        inputStatus.setKeyPressed(key);
    }

    inputStatus.clearPressedKeys();

    for (const auto& key : allKeys)
    {
        ASSERT_FALSE(inputStatus.isKeyPressed(key));
    }
}

TEST_F(InputStatusTest, shouldSetMousePosition)
{
    inputStatus.setMousePosition(position);

    ASSERT_EQ(inputStatus.getMousePosition(), position);
}

TEST_F(InputStatusTest, afterKeysPressed_shouldNotSetReleasedKey)
{
    inputStatus.setKeyPressed(InputKey::Up);
    inputStatus.setKeyPressed(InputKey::MouseRight);

    inputStatus.setReleasedKeys();

    ASSERT_FALSE(inputStatus.isKeyReleased(InputKey::Up));
    ASSERT_FALSE(inputStatus.isKeyReleased(InputKey::MouseRight));
}

TEST_F(InputStatusTest, afterKeysPressedAndCleared_shouldSetReleasedKey)
{
    inputStatus.setKeyPressed(InputKey::Up);
    inputStatus.setKeyPressed(InputKey::MouseLeft);
    inputStatus.setReleasedKeys();
    inputStatus.clearPressedKeys();

    inputStatus.setReleasedKeys();

    ASSERT_TRUE(inputStatus.isKeyReleased(InputKey::Up));
    ASSERT_TRUE(inputStatus.isKeyReleased(InputKey::MouseLeft));
}
