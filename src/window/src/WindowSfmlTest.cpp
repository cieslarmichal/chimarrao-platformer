#include "WindowSfml.h"

#include "gtest/gtest.h"
#include "WindowObserverMock.h"
#include "WindowObservationHandlerMock.h"

using namespace ::testing;
using namespace window;

namespace
{
const utils::Vector2u windowSize{800, 600};
const utils::Vector2u windowSizeChanged{1000, 600};
const std::string windowTitle{"window"};
const Resolution resolution{50, 100};
const DisplayMode initialDisplayMode{DisplayMode::Window};
const Resolution initialResolution{windowSize.x, windowSize.y};
const bool initialVsync{true};
const unsigned int initialFrameLimit{120};
const DisplayMode changedDisplayMode{DisplayMode::Fullscreen};
const Resolution changedResolution{windowSizeChanged.x, windowSizeChanged.y};
const bool changedVsync{false};
const unsigned int changedFrameLimit{60};
const WindowSettings initialWindowSettings{initialDisplayMode, initialResolution, initialVsync,
                                           initialFrameLimit};
const WindowSettings windowSettingsWithDisplayModeChanged{changedDisplayMode, initialResolution, initialVsync,
                                           initialFrameLimit};
const WindowSettings windowSettingsWithResolutionChanged{initialDisplayMode, changedResolution, initialVsync,
                                                          initialFrameLimit};
const WindowSettings windowSettingsWithVsyncChanged{initialDisplayMode, initialResolution, changedVsync,
                                                          initialFrameLimit};
const WindowSettings windowSettingsWithFrameLimitChanged{initialDisplayMode, initialResolution, initialVsync,
                                                         changedFrameLimit};
const std::vector<Resolution> expectedSupportedResolutions{{800, 600},  {1024, 768}, {1280, 720},
                                                           {1368, 768}, {1600, 900}, {1920, 1080}};
const std::vector<unsigned int> expectedSupportedFrameLimits{30, 60, 90, 120};
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

TEST_F(WindowSfmlTest, getWindowSettings)
{
    const auto actualWindowSettings = window.getWindowSettings();

    ASSERT_EQ(actualWindowSettings, initialWindowSettings);
}

TEST_F(WindowSfmlTest, givenSameDisplayMode_shouldNotSetDisplayMode)
{
    const auto result = window.setDisplayMode(initialDisplayMode);

    ASSERT_FALSE(result);
}

TEST_F(WindowSfmlTest, givenDifferentDisplayMode_shouldSetDisplayMode)
{
    const auto result = window.setDisplayMode(changedDisplayMode);

    const auto actualWindowSettings = window.getWindowSettings();
    ASSERT_EQ(actualWindowSettings, windowSettingsWithDisplayModeChanged);
    ASSERT_TRUE(result);
}

TEST_F(WindowSfmlTest, givenSameResolution_shouldNotSetResolution)
{
    const auto result = window.setResolution(initialResolution);

    ASSERT_FALSE(result);
}

TEST_F(WindowSfmlTest, givenDifferentResolution_shouldSetResolution)
{
    const auto result = window.setResolution(changedResolution);

    const auto actualWindowSettings = window.getWindowSettings();
    ASSERT_EQ(actualWindowSettings, windowSettingsWithResolutionChanged);
    ASSERT_TRUE(result);
}

TEST_F(WindowSfmlTest, givenSameVsync_shouldNotSetVsync)
{
    const auto result = window.setVerticalSync(initialVsync);

    ASSERT_FALSE(result);
}

TEST_F(WindowSfmlTest, givenDifferentVsync_shouldSetVsync)
{
    const auto result = window.setVerticalSync(changedVsync);

    const auto actualWindowSettings = window.getWindowSettings();
    ASSERT_EQ(actualWindowSettings, windowSettingsWithVsyncChanged);
    ASSERT_TRUE(result);
}

TEST_F(WindowSfmlTest, givenSameFrameLimit_shouldNotSetFrameLimit)
{
    const auto result = window.setFramerateLimit(initialFrameLimit);

    ASSERT_FALSE(result);
}

TEST_F(WindowSfmlTest, givenDifferentFrameLimit_shouldSetFramerateLimit)
{
    const auto result = window.setFramerateLimit(changedFrameLimit);

    const auto actualWindowSettings = window.getWindowSettings();
    ASSERT_EQ(actualWindowSettings, windowSettingsWithFrameLimitChanged);
    ASSERT_TRUE(result);
}

TEST_F(WindowSfmlTest, shouldRetrieveSupportedResolutions)
{
    const auto actualSupportedResolutions = window.getSupportedResolutions();

    ASSERT_EQ(actualSupportedResolutions, expectedSupportedResolutions);
}

TEST_F(WindowSfmlTest, shouldRetrieveSupportedFrameLimits)
{
    const auto actualSupportedFrameLimits = window.getSupportedFrameLimits();

    ASSERT_EQ(actualSupportedFrameLimits, expectedSupportedFrameLimits);
}
