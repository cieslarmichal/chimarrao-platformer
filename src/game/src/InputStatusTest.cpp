#include "InputStatus.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace game;

namespace
{
const auto up = InputKey::Up;
const auto down = InputKey::Down;
const auto left = InputKey::Left;
const auto right = InputKey::Right;
const auto space = InputKey::Space;
const auto shift = InputKey::Shift;
const auto enter = InputKey::Enter;
const auto escape = InputKey::Escape;
}

class InputStatusTest : public Test
{
public:
    InputStatus inputStatus;
};

TEST_F(InputStatusTest, initialInputKeysStatus_shouldReturnUnpressedKeys)
{
    ASSERT_FALSE(inputStatus.isKeyPressed(up));
    ASSERT_FALSE(inputStatus.isKeyPressed(down));
    ASSERT_FALSE(inputStatus.isKeyPressed(left));
    ASSERT_FALSE(inputStatus.isKeyPressed(right));
    ASSERT_FALSE(inputStatus.isKeyPressed(space));
    ASSERT_FALSE(inputStatus.isKeyPressed(shift));
    ASSERT_FALSE(inputStatus.isKeyPressed(enter));
    ASSERT_FALSE(inputStatus.isKeyPressed(escape));
}

TEST_F(InputStatusTest, shouldSetKeyPressed)
{
    inputStatus.setKeyPressed(up);

    ASSERT_TRUE(inputStatus.isKeyPressed(up));
}

TEST_F(InputStatusTest, shouldClearKeysStatus)
{
    inputStatus.setKeyPressed(up);
    inputStatus.setKeyPressed(down);

    inputStatus.clearStatus();

    ASSERT_FALSE(inputStatus.isKeyPressed(up));
    ASSERT_FALSE(inputStatus.isKeyPressed(down));
}