#include "RenderTargetSfml.h"

#include <WindowMock.h>

#include "gtest/gtest.h"

using namespace ::testing;
using namespace graphics;

namespace
{
const utils::Vector2u windowSize{800, 600};
const utils::Vector2u newWindowSize{1280, 720};
const utils::Vector2u areaSize{80, 60};
const utils::Vector2f center{45,25};
}

class RendererTargetSfmlTest : public Test
{
public:
    RendererTargetSfmlTest()
        : window{std::make_shared<StrictMock<window::WindowMock>>()},
          renderTargetSfml{window, windowSize, areaSize}
    {

    }

    std::shared_ptr<StrictMock<window::WindowMock>> window;
    RenderTargetSfml renderTargetSfml;

};

TEST_F(RendererTargetSfmlTest, setView_shouldNotThrow) {
    EXPECT_CALL(*window, setView(_));

    EXPECT_NO_THROW(renderTargetSfml.setView());
}

TEST_F(RendererTargetSfmlTest, setCenter_getterShouldReturnSameValue) {
    renderTargetSfml.setCenter(center);

    EXPECT_EQ(renderTargetSfml.getCenter(), center);
}

TEST_F(RendererTargetSfmlTest, getViewSize_shouldReturnValueFromConstructor)
{
    EXPECT_CALL(*window, setView(_));
    renderTargetSfml.setView();

    auto viewSize = renderTargetSfml.getViewSize();
    EXPECT_EQ(viewSize.x, areaSize.x);
    EXPECT_EQ(viewSize.y, areaSize.y);
}

TEST_F(RendererTargetSfmlTest, getSize_shouldReturnValueFromConstructor)
{
    auto size = renderTargetSfml.getSize();

    EXPECT_EQ(size.x, windowSize.x);
    EXPECT_EQ(size.y, windowSize.y);
}

TEST_F(RendererTargetSfmlTest, setViewSize_shouldChangeWindowSize)
{
    renderTargetSfml.setViewSize(newWindowSize);

    auto size = renderTargetSfml.getSize();

    EXPECT_EQ(size.x, newWindowSize.x);
    EXPECT_EQ(size.y, newWindowSize.y);
}