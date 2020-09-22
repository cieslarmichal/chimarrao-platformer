#include "DefaultInputManager.h"

#include "gtest/gtest.h"

#include "InputObservationHandlerMock.h"
#include "InputObserverMock.h"
#include "WindowMock.h"

using namespace ::testing;
using namespace input;

namespace
{
const InputStatus inputStatus{};
const utils::Vector2f mousePosition{3, 4};
}

class DefaultInputManagerTest : public Test
{
public:
    std::shared_ptr<StrictMock<InputObserverMock>> observer1 =
        std::make_shared<StrictMock<InputObserverMock>>();
    std::shared_ptr<StrictMock<gui::WindowMock>> window = std::make_shared<StrictMock<gui::WindowMock>>();
    std::unique_ptr<InputObservationHandlerMock> observerHandlerInit =
        std::make_unique<StrictMock<InputObservationHandlerMock>>();
    InputObservationHandlerMock* observerHandler = observerHandlerInit.get();

    DefaultInputManager inputManager{std::move(observerHandlerInit), window};
};

TEST_F(DefaultInputManagerTest,
       readInput_shouldReadMousePositionFromWindowAnddNotifyObserversAboutInputStatus)
{
    EXPECT_CALL(*window, getMousePosition()).WillOnce(Return(mousePosition));
    EXPECT_CALL(*observerHandler, notifyObservers(inputStatus));

    inputManager.readInput();
}

TEST_F(DefaultInputManagerTest, shouldRegisterObserver)
{
    EXPECT_CALL(*observerHandler, registerObserver(observer1.get()));

    inputManager.registerObserver(observer1.get());
}

TEST_F(DefaultInputManagerTest, shouldRemoveObserver)
{
    EXPECT_CALL(*observerHandler, removeObserver(observer1.get()));

    inputManager.removeObserver(observer1.get());
}
