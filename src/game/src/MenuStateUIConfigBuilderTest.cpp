#include "MenuStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "MenuState.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> expectedButtonNames{
    "menuPlayButton", "menuMapEditorButton", "menuControlsButton", "menuSettingsButton", "menuExitButton"};
const std::vector<std::string> expectedImagesNames{"menuIcon1Image", "menuIcon2Image", "menuIcon3Image",
                                                   "menuIcon4Image", "menuIcon5Image"};
}
class MenuStateUIConfigBuilderTest : public Test
{
public:
    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::unique_ptr<components::ui::UIManagerMock> uiManagerInit{
        std::make_unique<NiceMock<components::ui::UIManagerMock>>()};
    components::ui::UIManagerMock* uiManager{uiManagerInit.get()};
    MenuState menuState{window, rendererPool, fileAccess, states, std::move(uiManagerInit)};
};

TEST_F(MenuStateUIConfigBuilderTest, createMenuUI)
{
    const auto menuUI = MenuStateUIConfigBuilder::createMenuUIConfig(&menuState);

    std::vector<std::string> actualButtonsNames;
    std::transform(menuUI->buttonsConfig.begin(), menuUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    std::vector<std::string> actualImageNames;
    std::transform(menuUI->imagesConfig.begin(), menuUI->imagesConfig.end(),
                   std::back_inserter(actualImageNames),
                   [](const auto& imageConfig) { return imageConfig->uniqueName; });

    ASSERT_EQ(menuUI->backgroundConfig->uniqueName, "menuBackground");
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(menuUI->labelsConfig.empty());
    ASSERT_TRUE(menuUI->checkBoxesConfig.empty());
    ASSERT_TRUE(menuUI->textFieldsConfig.empty());
    ASSERT_TRUE(compareVectors(actualImageNames, expectedImagesNames));
}