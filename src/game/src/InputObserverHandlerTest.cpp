#include "InputObserverHandler.h"

#include "gtest/gtest.h"

#include "InputObserverMock.h"

using namespace ::testing;
using namespace game;

namespace
{
const KeyboardStatus keyboardStatus;
}

class InputObserverHandlerTest : public Test
{
public:
    std::shared_ptr<InputObserverMock> observer1 =
        std::make_shared<StrictMock<InputObserverMock>>();
    std::shared_ptr<InputObserverMock> observer2 =
        std::make_shared<StrictMock<InputObserverMock>>();
    InputObserverHandler inputObserverHandler;
};

TEST_F(InputObserverHandlerTest, givenRegisteredObserver_shouldNotifyObserverAboutChanges)
{
    inputObserverHandler.registerObserver(observer1.get());
    EXPECT_CALL(*observer1, update(keyboardStatus));

    inputObserverHandler.notifyObservers(keyboardStatus);
}

TEST_F(InputObserverHandlerTest, givenRegisteredObservers_shouldNotifyObserversAboutChanges)
{
    inputObserverHandler.registerObserver(observer1.get());
    inputObserverHandler.registerObserver(observer2.get());
    EXPECT_CALL(*observer1, update(keyboardStatus));
    EXPECT_CALL(*observer2, update(keyboardStatus));

    inputObserverHandler.notifyObservers(keyboardStatus);
}

TEST_F(InputObserverHandlerTest, givenRemovedObserver_shouldNotNotifyObserverAboutChanges)
{
    inputObserverHandler.registerObserver(observer1.get());
    inputObserverHandler.removeObserver(observer1.get());

    inputObserverHandler.notifyObservers(keyboardStatus);
}

TEST_F(InputObserverHandlerTest, givenRemovedObservers_shouldNotNotifyObserversAboutChanges)
{
    inputObserverHandler.registerObserver(observer1.get());
    inputObserverHandler.registerObserver(observer2.get());
    inputObserverHandler.removeObserver(observer1.get());
    inputObserverHandler.removeObserver(observer2.get());

    inputObserverHandler.notifyObservers(keyboardStatus);
}

TEST_F(InputObserverHandlerTest, givenRemovedOneObserverFromTwo_shouldNotNotifyOneObserverAboutChanges)
{
    inputObserverHandler.registerObserver(observer1.get());
    inputObserverHandler.registerObserver(observer2.get());
    inputObserverHandler.removeObserver(observer2.get());
    EXPECT_CALL(*observer1, update(keyboardStatus));

    inputObserverHandler.notifyObservers(keyboardStatus);
}