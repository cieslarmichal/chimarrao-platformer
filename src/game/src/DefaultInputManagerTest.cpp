#include "DefaultInputManager.h"

#include "gtest/gtest.h"
#include "ObserverHandlerMock.h"
#include "InputObserverMock.h"

using namespace ::testing;
using namespace game;

namespace
{
const KeyboardStatus keyboardStatus;
}

class DefaultInputManagerTest : public Test
{
public:
    std::shared_ptr<InputObserverMock> observer1 =
        std::make_shared<StrictMock<InputObserverMock>>();
    std::unique_ptr<ObserverHandlerMock> observerHandlerInit =
        std::make_unique<StrictMock<ObserverHandlerMock>>();
    ObserverHandlerMock* observerHandler = observerHandlerInit.get();

    DefaultInputManager inputManager{std::move(observerHandlerInit)};
};

TEST_F(DefaultInputManagerTest, readInput_shouldNotifyObserversAboutKeyboardStatus)
{
    EXPECT_CALL(*observerHandler, notifyObservers(keyboardStatus));

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

