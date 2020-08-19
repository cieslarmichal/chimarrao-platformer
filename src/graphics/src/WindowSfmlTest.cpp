#include "WindowSfml.h"

#include "gtest/gtest.h"

#include "WindowProxyMock.h"

using namespace graphics;
using namespace ::testing;

namespace
{
sf::Event closeEvent{sf::Event::Closed};
}
class WindowSfmlTest : public Test
{
public:
    const std::string title = "Window Title";
    const utils::Vector2u windowSize{800, 600};
    std::unique_ptr<WindowProxyMock> windowProxyMock = std::make_unique<StrictMock<WindowProxyMock>>();
    WindowProxyMock* windowProxyPtr = windowProxyMock.get();
};

TEST_F(WindowSfmlTest, shouldCallCreateMethod)
{
    EXPECT_CALL(*windowProxyPtr, create(windowSize, title));

    const auto window = std::make_unique<WindowSfml>(windowSize, title, std::move(windowProxyMock));
}

TEST_F(WindowSfmlTest, shouldReturnValueAfterCallIsOpen)
{
    EXPECT_CALL(*windowProxyPtr, create(windowSize, title));
    const auto window = std::make_unique<WindowSfml>(windowSize, title, std::move(windowProxyMock));

    EXPECT_CALL(*windowProxyPtr, isOpen()).WillOnce(Return(true));
    EXPECT_TRUE(window->isOpen());
    EXPECT_CALL(*windowProxyPtr, isOpen()).WillOnce(Return(false));
    EXPECT_FALSE(window->isOpen());
}

TEST_F(WindowSfmlTest, shouldCallDisplay)
{
    EXPECT_CALL(*windowProxyPtr, create(windowSize, title));
    auto window = std::make_unique<WindowSfml>(windowSize, title, std::move(windowProxyMock));
    EXPECT_CALL(*windowProxyPtr, display());

    window->display();
}

TEST_F(WindowSfmlTest, shouldCloseWindowAfterCloseEvent)
{
    EXPECT_CALL(*windowProxyPtr, create(windowSize, title));
    auto window = std::make_unique<WindowSfml>(windowSize, title, std::move(windowProxyMock));
    EXPECT_CALL(*windowProxyPtr, pollEvent(_))
        .Times(2)
        .WillOnce(DoAll(SetArgReferee<0>(closeEvent), Return(true)))
        .WillOnce(Return(false));

    EXPECT_CALL(*windowProxyPtr, close());

    window->update();
}

TEST_F(WindowSfmlTest, shouldClose)
{
    EXPECT_CALL(*windowProxyPtr, create(windowSize, title));
    auto window = std::make_unique<WindowSfml>(windowSize, title, std::move(windowProxyMock));
    EXPECT_CALL(*windowProxyPtr, close());

    window->close();
}