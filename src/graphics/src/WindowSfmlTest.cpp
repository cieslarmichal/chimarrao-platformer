#include "WindowSfml.h"

#include "gtest/gtest.h"

#include "WindowProxyMock.h"

using namespace graphics;
using namespace ::testing;
class WindowSFMLTest : public ::testing::Test
{
public:
    const std::string title = "Window Title";
    const utils::Vector2i windowSize = utils::Vector2i(800, 600);
    std::unique_ptr<WindowProxyMock> windowProxyMock = std::make_unique<NiceMock<WindowProxyMock>>();
    WindowProxyMock* windowProxyPtr = windowProxyMock.get();
};

TEST_F(WindowSFMLTest, shouldCallCreateMethod)
{
    EXPECT_CALL(*windowProxyPtr, create(utils::Vector2i{windowSize}, std::string{title}));
    auto window = std::make_unique<WindowSFML>(windowSize, title, std::move(windowProxyMock));
}

TEST_F(WindowSFMLTest, shouldReturnValueAfterCallIsOpen)
{
    auto window = std::make_unique<WindowSFML>(windowSize, title, std::move(windowProxyMock));
    EXPECT_CALL(*windowProxyPtr, isOpen()).WillOnce(Return(true));
    EXPECT_TRUE(window->isOpen());

    EXPECT_CALL(*windowProxyPtr, isOpen()).WillOnce(Return(false));
    EXPECT_FALSE(window->isOpen());
}

TEST_F(WindowSFMLTest, shouldCallDisplay)
{
    auto window = std::make_unique<WindowSFML>(windowSize, title, std::move(windowProxyMock));
    EXPECT_CALL(*windowProxyPtr, display());
    window->display();
}

TEST_F(WindowSFMLTest, shouldCloseWindowAfterCloseEvent) {
    sf::Event close_event;
    close_event.type = sf::Event::Closed;
    auto window = std::make_unique<WindowSFML>(windowSize, title, std::move(windowProxyMock));
    EXPECT_CALL(*windowProxyPtr, pollEvent(_))
        .Times(2)
        .WillOnce(DoAll(SetArgReferee<0>(close_event), Return(true)))
        .WillOnce(Return(false));
    EXPECT_CALL(*windowProxyPtr, close());
    window -> update();
}
