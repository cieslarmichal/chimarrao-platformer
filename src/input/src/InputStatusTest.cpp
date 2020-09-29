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

TEST_F(InputStatusTest, initialInputStatus_inputKeysAndMousePositionShouldBeSetToZero)
{
    for (const auto& key : allKeys)
    {
        ASSERT_FALSE(inputStatus.isKeyPressed(key));
    }

    ASSERT_EQ(inputStatus.getMousePosition(), zeroPosition);
}

TEST_F(InputStatusTest, initialInputKeysStatus_shouldHaveAllKeysUnpressed)
{
    for (const auto& key : allKeys)
    {
        ASSERT_FALSE(inputStatus.isKeyPressed(key));
    }
}

TEST_F(InputStatusTest, shouldSetKeyPressed)
{
    for (const auto& key : allKeys)
    {
        inputStatus.setKeyPressed(key);
        ASSERT_TRUE(inputStatus.isKeyPressed(key));
    }
}

TEST_F(InputStatusTest, shouldClearKeysStatus)
{
    for (const auto& key : allKeys)
    {
        inputStatus.setKeyPressed(key);
        ASSERT_TRUE(inputStatus.isKeyPressed(key));
    }

    inputStatus.clearStatus();

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