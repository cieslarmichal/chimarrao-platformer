#include "WindowSfml.h"

#include "gtest/gtest.h"

#include "WindowProxyMock.h"

using namespace graphics;
using namespace ::testing;

namespace
{
sf::Event closeEvent{sf::Event::Closed};
}
class WindowSFMLTest : public ::testing::Test
{
public:
    const std::string title = "Window Title";
    const utils::Vector2i windowSize = utils::Vector2i(800, 600);
    std::unique_ptr<WindowProxyMock> windowProxyMock = std::make_unique<StrictMock<WindowProxyMock>>();
    WindowProxyMock* windowProxyPtr = windowProxyMock.get();
};

TEST_F(WindowSFMLTest, shouldCallCreateMethod)
{
    EXPECT_CALL(*windowProxyPtr, create(utils::Vector2i{windowSize}, std::string{title}));

    const auto window = std::make_unique<WindowSFML>(windowSize, title, std::move(windowProxyMock));
}

TEST_F(WindowSFMLTest, shouldReturnValueAfterCallIsOpen)
{
    EXPECT_CALL(*windowProxyPtr, create(utils::Vector2i{windowSize}, std::string{title}));
    const auto window = std::make_unique<WindowSFML>(windowSize, title, std::move(windowProxyMock));

    EXPECT_CALL(*windowProxyPtr, isOpen()).WillOnce(Return(true));
    EXPECT_TRUE(window->isOpen());
    EXPECT_CALL(*windowProxyPtr, isOpen()).WillOnce(Return(false));
    EXPECT_FALSE(window->isOpen());
}

TEST_F(WindowSFMLTest, shouldCallDisplay)
{
    EXPECT_CALL(*windowProxyPtr, create(utils::Vector2i{windowSize}, std::string{title}));
    auto window = std::make_unique<WindowSFML>(windowSize, title, std::move(windowProxyMock));
    EXPECT_CALL(*windowProxyPtr, display());

    window->display();
}

TEST_F(WindowSFMLTest, shouldCloseWindowAfterCloseEvent)
{
    EXPECT_CALL(*windowProxyPtr, create(utils::Vector2i{windowSize}, std::string{title}));
    auto window = std::make_unique<WindowSFML>(windowSize, title, std::move(windowProxyMock));
    EXPECT_CALL(*windowProxyPtr, pollEvent(_))
        .Times(2)
        .WillOnce(DoAll(SetArgReferee<0>(closeEvent), Return(true)))
        .WillOnce(Return(false));

    EXPECT_CALL(*windowProxyPtr, close());

    window->update();
}

TEST_F(WindowSFMLTest, shouldClear)
{
    EXPECT_CALL(*windowProxyPtr, create(utils::Vector2i{windowSize}, std::string{title}));
    auto window = std::make_unique<WindowSFML>(windowSize, title, std::move(windowProxyMock));
    EXPECT_CALL(*windowProxyPtr, clear());

    window->clear();
}

TEST_F(WindowSFMLTest, shouldClose)
{
    EXPECT_CALL(*windowProxyPtr, create(utils::Vector2i{windowSize}, std::string{title}));
    auto window = std::make_unique<WindowSFML>(windowSize, title, std::move(windowProxyMock));
    EXPECT_CALL(*windowProxyPtr, close());

    window->close();
}