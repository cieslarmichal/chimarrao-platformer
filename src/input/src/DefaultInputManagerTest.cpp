#include "DefaultInputManager.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "WindowMock.h"

using namespace ::testing;
using namespace input;

namespace
{
const utils::Vector2f relativeMousePosition{3, 4};
const utils::Vector2f absoluteMousePosition{13, 4};
}

class DefaultInputManagerTest : public Test
{
public:
    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::unique_ptr<StrictMock<InputMock>> inputInit{std::make_unique<StrictMock<InputMock>>()};
    StrictMock<InputMock>* input{inputInit.get()};

    DefaultInputManager inputManager{std::move(inputInit), window};
};

TEST_F(DefaultInputManagerTest, readInput_shouldReadMousePositionFromWindow)
{
    EXPECT_CALL(*input, clearPressedKeys());
    EXPECT_CALL(*input, setReleasedKeys());
    EXPECT_CALL(*window, getMousePosition(true)).WillOnce(Return(relativeMousePosition));
    EXPECT_CALL(*window, getMousePosition(false)).WillOnce(Return(absoluteMousePosition));
    EXPECT_CALL(*input, setMouseRelativePosition(relativeMousePosition));
    EXPECT_CALL(*input, setMouseAbsolutePosition(absoluteMousePosition));

    inputManager.readInput();
}