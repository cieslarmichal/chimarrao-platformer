#include "WindowSfml.h"

#include "gtest/gtest.h"
#include "WindowObserverMock.h"
#include "WindowObservationHandlerMock.h"

using namespace ::testing;
using namespace window;

namespace
{
const utils::Vector2u windowSize{800,600};
const utils::Vector2u windowSizeChanged{1000,600};
const std::string windowTitle{"window"};
const sf::VideoMode videoMode{50, 100};
}

class WindowSfmlTest : public Test
{
public:
    std::shared_ptr<StrictMock<WindowObserverMock>> observer1 =
    std::make_shared<StrictMock<WindowObserverMock>>();
    std::unique_ptr<WindowObservationHandlerMock> observationHandlerInit =
        std::make_unique<StrictMock<WindowObservationHandlerMock>>();
    WindowObservationHandlerMock* observationHandler = observationHandlerInit.get();
    WindowSfml window{windowSize, windowTitle, std::move(observationHandlerInit)};
};

//TODO: uncomment this test
//TEST_F(WindowSfmlTest, givenWindowChangeSizeEvent_shouldNotfiyObservers)
//{
//    EXPECT_CALL(*observationHandler, notifyObservers(windowSizeChanged));
//}

TEST_F(WindowSfmlTest, shouldRegisterObserver)
{
    EXPECT_CALL(*observationHandler, registerObserver(observer1.get()));

    window.registerObserver(observer1.get());
}

TEST_F(WindowSfmlTest, shouldRemoveObserver)
{
    EXPECT_CALL(*observationHandler, removeObserver(observer1.get()));

    window.removeObserver(observer1.get());
}

TEST_F(WindowSfmlTest, setVideoMode_shouldNotThrow)
{
    ASSERT_NO_THROW(window.setVideoMode(videoMode));
}
