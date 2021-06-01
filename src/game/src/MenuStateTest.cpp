#include "MenuState.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const graphics::Color buttonHoverColor = graphics::Color(205, 128, 66);
const utils::DeltaTime deltaTime{1.0};
}

class MenuStateTest_Base : public Test
{
public:
    MenuStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
        EXPECT_CALL(*uiManager,
                    setColor(components::ui::UIComponentType::Button, "menuPlayButton", buttonHoverColor));
        EXPECT_CALL(*uiManager,
                    deactivateComponent(components::ui::UIComponentType::Image, "menuIcon1Image"));
        EXPECT_CALL(*uiManager,
                    deactivateComponent(components::ui::UIComponentType::Image, "menuIcon2Image"));
        EXPECT_CALL(*uiManager,
                    deactivateComponent(components::ui::UIComponentType::Image, "menuIcon3Image"));
        EXPECT_CALL(*uiManager,
                    deactivateComponent(components::ui::UIComponentType::Image, "menuIcon4Image"));
        EXPECT_CALL(*uiManager,
                    deactivateComponent(components::ui::UIComponentType::Image, "menuIcon5Image"));
        EXPECT_CALL(*uiManager, activateComponent(components::ui::UIComponentType::Image, "menuIcon1Image"));
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::unique_ptr<StrictMock<components::ui::UIManagerMock>> uiManagerInit{
        std::make_unique<StrictMock<components::ui::UIManagerMock>>()};
    StrictMock<components::ui::UIManagerMock>* uiManager{uiManagerInit.get()};
    StrictMock<input::InputMock> input;
};

class MenuStateTest : public MenuStateTest_Base
{
public:
    MenuState menuState{window, rendererPool, fileAccess, states, std::move(uiManagerInit)};
};

TEST_F(MenuStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());
    EXPECT_CALL(*uiManager,
                deactivateComponent(components::ui::UIComponentType::Image, "menuIcon1Image"));
    EXPECT_CALL(*uiManager,
                deactivateComponent(components::ui::UIComponentType::Image, "menuIcon2Image"));
    EXPECT_CALL(*uiManager,
                deactivateComponent(components::ui::UIComponentType::Image, "menuIcon3Image"));
    EXPECT_CALL(*uiManager,
                deactivateComponent(components::ui::UIComponentType::Image, "menuIcon4Image"));
    EXPECT_CALL(*uiManager,
                deactivateComponent(components::ui::UIComponentType::Image, "menuIcon5Image"));
    EXPECT_CALL(*uiManager, activateComponent(components::ui::UIComponentType::Image, "menuIcon1Image"));

    menuState.activate();
}

TEST_F(MenuStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());

    menuState.deactivate();
}

TEST_F(MenuStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(menuState.getType(), StateType::Menu);
}

TEST_F(MenuStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    menuState.render();
}

TEST_F(MenuStateTest, update_shouldUpdateUI)
{
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));
    EXPECT_CALL(input,isKeyPressed(input::InputKey::Enter)).WillOnce(Return(false));

    menuState.update(deltaTime, input);
}