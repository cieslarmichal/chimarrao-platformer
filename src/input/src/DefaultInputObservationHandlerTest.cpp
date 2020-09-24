#include "DefaultInputObservationHandler.h"

#include "gtest/gtest.h"

#include "InputObserverMock.h"

using namespace ::testing;
using namespace input;

namespace
{
const InputStatus inputStatus;
}

class DefaultInputObservationHandlerTest : public Test
{
public:
    std::shared_ptr<InputObserverMock> observer1 = std::make_shared<StrictMock<InputObserverMock>>();
    std::shared_ptr<InputObserverMock> observer2 = std::make_shared<StrictMock<InputObserverMock>>();
    DefaultInputObservationHandler inputObservationHandler;
};

TEST_F(DefaultInputObservationHandlerTest, givenRegisteredObserver_shouldNotifyObserverAboutChanges)
{
    inputObservationHandler.registerObserver(observer1.get());
    EXPECT_CALL(*observer1, handleInputStatus(inputStatus));

    inputObservationHandler.notifyObservers(inputStatus);
}

TEST_F(DefaultInputObservationHandlerTest, givenRegisteredObservers_shouldNotifyObserversAboutChanges)
{
    inputObservationHandler.registerObserver(observer1.get());
    inputObservationHandler.registerObserver(observer2.get());
    EXPECT_CALL(*observer1, handleInputStatus(inputStatus));
    EXPECT_CALL(*observer2, handleInputStatus(inputStatus));

    inputObservationHandler.notifyObservers(inputStatus);
}

TEST_F(DefaultInputObservationHandlerTest, givenRemovedObserver_shouldNotNotifyObserverAboutChanges)
{
    inputObservationHandler.registerObserver(observer1.get());
    inputObservationHandler.removeObserver(observer1.get());

    inputObservationHandler.notifyObservers(inputStatus);
}

TEST_F(DefaultInputObservationHandlerTest, givenRemovedObservers_shouldNotNotifyObserversAboutChanges)
{
    inputObservationHandler.registerObserver(observer1.get());
    inputObservationHandler.registerObserver(observer2.get());
    inputObservationHandler.removeObserver(observer1.get());
    inputObservationHandler.removeObserver(observer2.get());

    inputObservationHandler.notifyObservers(inputStatus);
}

TEST_F(DefaultInputObservationHandlerTest,
       givenRemovedOneObserverFromTwo_shouldNotNotifyOneObserverAboutChanges)
{
    inputObservationHandler.registerObserver(observer1.get());
    inputObservationHandler.registerObserver(observer2.get());
    inputObservationHandler.removeObserver(observer2.get());
    EXPECT_CALL(*observer1, handleInputStatus(inputStatus));

    inputObservationHandler.notifyObservers(inputStatus);
}