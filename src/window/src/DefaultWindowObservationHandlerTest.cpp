#include "DefaultWindowObservationHandler.h"

#include "gtest/gtest.h"

#include "WindowObserverMock.h"

using namespace ::testing;
using namespace window;

namespace
{
const utils::Vector2u windowSize{1000, 600};
}

class DefaultInputObservationHandlerTest : public Test
{
public:
    std::shared_ptr<WindowObserverMock> observer1 = std::make_shared<StrictMock<WindowObserverMock>>();
    std::shared_ptr<WindowObserverMock> observer2 = std::make_shared<StrictMock<WindowObserverMock>>();
    DefaultWindowObservationHandler inputObservationHandler;
};

TEST_F(DefaultInputObservationHandlerTest, givenRegisteredObserver_shouldNotifyObserverAboutChanges)
{
    inputObservationHandler.registerObserver(observer1.get());
    EXPECT_CALL(*observer1, handleWindowSizeChange(windowSize));

    inputObservationHandler.notifyObservers(windowSize);
}

TEST_F(DefaultInputObservationHandlerTest, givenRegisteredObservers_shouldNotifyObserversAboutChanges)
{
    inputObservationHandler.registerObserver(observer1.get());
    inputObservationHandler.registerObserver(observer2.get());
    EXPECT_CALL(*observer1, handleWindowSizeChange(windowSize));
    EXPECT_CALL(*observer2, handleWindowSizeChange(windowSize));

    inputObservationHandler.notifyObservers(windowSize);
}

TEST_F(DefaultInputObservationHandlerTest, givenRemovedObserver_shouldNotNotifyObserverAboutChanges)
{
    inputObservationHandler.registerObserver(observer1.get());
    inputObservationHandler.removeObserver(observer1.get());

    inputObservationHandler.notifyObservers(windowSize);
}

TEST_F(DefaultInputObservationHandlerTest, givenRemovedObservers_shouldNotNotifyObserversAboutChanges)
{
    inputObservationHandler.registerObserver(observer1.get());
    inputObservationHandler.registerObserver(observer2.get());
    inputObservationHandler.removeObserver(observer1.get());
    inputObservationHandler.removeObserver(observer2.get());

    inputObservationHandler.notifyObservers(windowSize);
}

TEST_F(DefaultInputObservationHandlerTest,
       givenRemovedOneObserverFromTwo_shouldNotNotifyOneObserverAboutChanges)
{
    inputObservationHandler.registerObserver(observer1.get());
    inputObservationHandler.registerObserver(observer2.get());
    inputObservationHandler.removeObserver(observer2.get());
    EXPECT_CALL(*observer1, handleWindowSizeChange(windowSize));

    inputObservationHandler.notifyObservers(windowSize);
}