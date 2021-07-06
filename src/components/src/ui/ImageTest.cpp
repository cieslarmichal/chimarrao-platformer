#include "Image.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"
#include "exceptions/UIComponentConfigNotFound.h"
#include "ProjectPathReader.h"

using namespace components::ui;
using namespace ::testing;

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto dummySize = utils::Vector2f{25.f, 5.f};
const auto dummyPosition = utils::Vector2f{25.f, 5.f};
const auto dummyFontSize = 4;
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
const auto imageName = "dummyImage";
const auto imagePath = projectPath + "resources/BG/menu_background.jpg";

std::unique_ptr<components::ui::ImageConfig> createValidConfig()
{
    return std::make_unique<components::ui::ImageConfig>(
        imageName, dummyPosition, dummySize, graphics::VisibilityLayer::First, imagePath);
}
}

class ImageTest : public Test
{
public:
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
};

TEST_F(ImageTest, createBackgroundWithoutConfig_shouldThrowUIComponentConfigNotFound)
{
    ASSERT_THROW(Image(rendererPool, nullptr), exceptions::UIComponentConfigNotFound);
}

TEST_F(ImageTest, createBackgroundWithValidConfig_shouldNoThrow)
{
    ASSERT_NO_THROW(Image(rendererPool, createValidConfig()));
}

TEST_F(ImageTest, activate_shouldActivateAfterSomeTime)
{
    auto image = Image(rendererPool, createValidConfig());

    image.activate();

    ASSERT_TRUE(image.isActive());
}

TEST_F(ImageTest, deactivate)
{
    auto image = Image(rendererPool, createValidConfig());

    image.deactivate();

    ASSERT_FALSE(image.isActive());
}

TEST_F(ImageTest, getName)
{
    auto image = Image(rendererPool, createValidConfig());

    const auto actualName = image.getName();

    ASSERT_EQ(actualName, imageName);
}
