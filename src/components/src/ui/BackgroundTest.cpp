#include "Background.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "ProjectPathReader.h"
#include "exceptions/InvalidUIComponentConfig.h"
#include "exceptions/UIComponentConfigNotFound.h"

using namespace components::ui;
using namespace ::testing;

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
const auto imagePath = projectPath + "resources/BG/menu_background.jpg";
const auto backgroundName = "dummyBackground";
const auto dummyColor = graphics::Color(251, 190, 102);

std::unique_ptr<components::ui::BackgroundConfig> createValidConfig()
{
    return std::make_unique<components::ui::BackgroundConfig>(
        backgroundName, utils::Vector2f{0.f, 0.f}, utils::Vector2f{80.f, 60.f},
        graphics::VisibilityLayer::Background, imagePath);
}

std::unique_ptr<components::ui::BackgroundConfig> createInvalidConfig()
{
    return std::make_unique<components::ui::BackgroundConfig>(backgroundName, utils::Vector2f{0.f, 0.f},
                                                              utils::Vector2f{80.f, 60.f},
                                                              graphics::VisibilityLayer::Background, "");
}
}

class BackgroundTest : public Test
{
public:
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
};

TEST_F(BackgroundTest, createBackgroundWithoutConfig_shouldThrowUIComponentConfigNotFound)
{
    ASSERT_THROW(Background(sharedContext, nullptr), exceptions::UIComponentConfigNotFound);
}

TEST_F(BackgroundTest, createBackgroundWithConfigWithoutColorNorTexture_shouldThrowInvalidUIConfig)
{
    ASSERT_THROW(Background(sharedContext, createInvalidConfig()), exceptions::InvalidUIComponentConfig);
}

TEST_F(BackgroundTest, createBackgroundWithValidConfig_shouldNoThrow)
{
    ASSERT_NO_THROW(Background(sharedContext, createValidConfig()));
}

TEST_F(BackgroundTest, activate)
{
    auto background = Background(sharedContext, createValidConfig());

    background.activate();

    ASSERT_TRUE(background.isActive());
}

TEST_F(BackgroundTest, deactivate)
{
    auto background = Background(sharedContext, createValidConfig());

    background.deactivate();

    ASSERT_FALSE(background.isActive());
}

TEST_F(BackgroundTest, getName)
{
    auto background = Background(sharedContext, createValidConfig());

    const auto actualName = background.getName();

    ASSERT_EQ(actualName, backgroundName);
}

TEST_F(BackgroundTest, setColor)
{
    auto background = Background(sharedContext, createValidConfig());
    EXPECT_CALL(*rendererPool, setColor(_, dummyColor));

    background.setColor(dummyColor);
}

TEST_F(BackgroundTest, setTexture)
{
    auto background = Background(sharedContext, createValidConfig());
    EXPECT_CALL(*rendererPool, setTexture(_, _, _));

    background.setTexture(imagePath);
}