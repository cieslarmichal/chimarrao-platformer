#include "MenuStateUIConfigBuilder.h"

#include <FileAccessMock.h>

#include "gtest/gtest.h"

#include "InputManagerMock.h"
#include "RendererPoolMock.h"
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
}
class MenuStateUIConfigBuilderTest : public Test
{
public:
    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<input::InputManagerMock>> inputManager =
        std::make_shared<NiceMock<input::InputManagerMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<NiceMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<NiceMock<utils::FileAccessMock>>();
    std::stack<std::unique_ptr<State>> states;
    std::unique_ptr<components::ui::UIManagerMock> uiManagerInit{
        std::make_unique<NiceMock<components::ui::UIManagerMock>>()};
    components::ui::UIManagerMock* uiManager{uiManagerInit.get()};
    MenuState menuState{window, inputManager, rendererPool, fileAccess, states, std::move(uiManagerInit)};
};

TEST_F(MenuStateUIConfigBuilderTest, createMenuUI)
{
    const auto menuUI = MenuStateUIConfigBuilder::createMenuUIConfig(&menuState);

    std::vector<std::string> actualButtonsNames;
    std::transform(menuUI->buttonsConfig.begin(), menuUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    ASSERT_EQ(menuUI->backgroundConfig->uniqueName, "menuBackground");
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(menuUI->labelsConfig.empty());
    ASSERT_TRUE(menuUI->checkBoxesConfig.empty());
    ASSERT_TRUE(menuUI->textFieldsConfig.empty());
}