#include "Image.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "GraphicsIdGenerator.h"
#include "ProjectPathReader.h"
#include "exceptions/InvalidUIComponentConfig.h"
#include "exceptions/UIComponentConfigNotFound.h"

using namespace components::ui;
using namespace ::testing;

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto dummySize = utils::Vector2f{25.f, 5.f};
const auto changedSize = utils::Vector2f{10.f, 5.f};
const auto dummyPosition = utils::Vector2f{25.f, 5.f};
const auto dummyFontSize = 4;
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
const auto imageName = "dummyImage";
const auto imagePath = projectPath + "resources/BG/menu_background.jpg";
const auto thickness = 2.f;

std::unique_ptr<components::ui::ImageConfig> createValidConfig()
{
    return std::make_unique<components::ui::ImageConfig>(imageName, dummyPosition, dummySize,
                                                         graphics::VisibilityLayer::First, imagePath);
}

std::unique_ptr<components::ui::ImageConfig> createInvalidConfig()
{
    return std::make_unique<components::ui::ImageConfig>(imageName, dummyPosition, dummySize,
                                                         graphics::VisibilityLayer::First, "");
}
}

class ImageTest : public Test
{
public:
    void expectCreateGraphicsComponent()
    {
        EXPECT_CALL(*rendererPool,
                    acquire(dummySize, dummyPosition, imagePath, graphics::VisibilityLayer::First, _))
            .WillOnce(Return(graphicsId));
    }

    void expectReleaseGraphicsId()
    {
        EXPECT_CALL(*rendererPool, release(graphicsId));
    }

    const graphics::GraphicsId graphicsId = graphics::GraphicsIdGenerator::generateId();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
};

TEST_F(ImageTest, createImageWithoutConfig_shouldThrowUIComponentConfigNotFound)
{
    ASSERT_THROW(Image(sharedContext, nullptr), exceptions::UIComponentConfigNotFound);
}

TEST_F(ImageTest, createImageWithConfigWithoutColorNorTexture_shouldThrowInvalidUIConfig)
{
    ASSERT_THROW(Image(sharedContext, createInvalidConfig()), exceptions::InvalidUIComponentConfig);
}

TEST_F(ImageTest, activate_shouldActivateAfterSomeTime)
{
    expectCreateGraphicsComponent();
    auto image = Image{sharedContext, createValidConfig()};
    EXPECT_CALL(*rendererPool, setVisibility(_, _));

    image.activate();

    ASSERT_TRUE(image.isActive());
    expectReleaseGraphicsId();
}

TEST_F(ImageTest, deactivate)
{
    expectCreateGraphicsComponent();
    auto image = Image{sharedContext, createValidConfig()};
    EXPECT_CALL(*rendererPool, setVisibility(_, _));

    image.deactivate();

    ASSERT_FALSE(image.isActive());
    expectReleaseGraphicsId();
}

TEST_F(ImageTest, getName)
{
    expectCreateGraphicsComponent();
    auto image = Image{sharedContext, createValidConfig()};

    const auto actualName = image.getName();

    ASSERT_EQ(actualName, imageName);
    expectReleaseGraphicsId();
}

TEST_F(ImageTest, initialSize_shouldMatchWithTheOnceFromConstructor)
{
    expectCreateGraphicsComponent();
    auto image = Image{sharedContext, createValidConfig()};
    EXPECT_CALL(*rendererPool, getSize(_)).WillOnce(Return(dummySize));

    const auto actualSize = image.getSize();

    ASSERT_EQ(actualSize, dummySize);
    expectReleaseGraphicsId();
}

TEST_F(ImageTest, setSize)
{
    expectCreateGraphicsComponent();
    auto image = Image{sharedContext, createValidConfig()};
    EXPECT_CALL(*rendererPool, setSize(_, changedSize));
    EXPECT_CALL(*rendererPool, getSize(_)).WillOnce(Return(changedSize));

    image.setSize(changedSize);

    const auto actualSize = image.getSize();
    ASSERT_EQ(actualSize, changedSize);
    expectReleaseGraphicsId();
}

TEST_F(ImageTest, setOutline)
{
    expectCreateGraphicsComponent();
    auto image = Image{sharedContext, createValidConfig()};
    EXPECT_CALL(*rendererPool, setOutline(_, thickness, graphics::Color::Black));

    image.setOutline(thickness, graphics::Color::Black);

    expectReleaseGraphicsId();
}
