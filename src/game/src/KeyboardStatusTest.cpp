#include "KeyboardStatus.h"
#include "gtest/gtest.h"

using namespace ::testing;
using namespace game;

class KeyboardStatusTest : public Test
{
public:
    KeyboardStatus keyboardStatus{};
};

TEST_F(KeyboardStatusTest, initialKeyboardStatus_shouldReturnUnpressedKeys)
{
    ASSERT_FALSE(keyboardStatus.isUpPressed());
    ASSERT_FALSE(keyboardStatus.isDownPressed());
    ASSERT_FALSE(keyboardStatus.isLeftPressed());
    ASSERT_FALSE(keyboardStatus.isRightPressed());
    ASSERT_FALSE(keyboardStatus.isSpacePressed());
    ASSERT_FALSE(keyboardStatus.isEnterPressed());
    ASSERT_FALSE(keyboardStatus.isShiftPressed());
    ASSERT_FALSE(keyboardStatus.isEscapePressed());
}

TEST_F(KeyboardStatusTest, shouldSetUpKey)
{
    keyboardStatus.setUpPressed();

    ASSERT_TRUE(keyboardStatus.isUpPressed());
}

TEST_F(KeyboardStatusTest, shouldSetDownKey)
{
    keyboardStatus.setDownPressed();

    ASSERT_TRUE(keyboardStatus.isDownPressed());
}

TEST_F(KeyboardStatusTest, shouldSetRightKey)
{
    keyboardStatus.setRightPressed();

    ASSERT_TRUE(keyboardStatus.isRightPressed());
}

TEST_F(KeyboardStatusTest, shouldSetLeftKey)
{
    keyboardStatus.setLeftPressed();

    ASSERT_TRUE(keyboardStatus.isLeftPressed());
}

TEST_F(KeyboardStatusTest, shouldShiftDownKey)
{
    keyboardStatus.setShiftPressed();

    ASSERT_TRUE(keyboardStatus.isShiftPressed());
}

TEST_F(KeyboardStatusTest, shouldSetEnterKey)
{
    keyboardStatus.setEnterPressed();

    ASSERT_TRUE(keyboardStatus.isEnterPressed());
}

TEST_F(KeyboardStatusTest, shouldSetEscapeKey)
{
    keyboardStatus.setEscapePressed();

    ASSERT_TRUE(keyboardStatus.isEscapePressed());
}

TEST_F(KeyboardStatusTest, shouldSetKey)
{
    keyboardStatus.setSpacePressed();

    ASSERT_TRUE(keyboardStatus.isSpacePressed());
}