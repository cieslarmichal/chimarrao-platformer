#include "ChooseMapState.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "MapsReaderMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "editor/TileMapMock.h"
#include "UIManagerMock.h"

#include "ProjectPathReader.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> mapPaths{"x1", "x2", "x3", "x4", "x5", "x6"};
const utils::DeltaTime deltaTime{1.0};
const std::vector<std::string> iconNames{"chooseMapIcon1Image", "chooseMapIcon2Image", "chooseMapIcon3Image",
                                         "chooseMapIcon4Image", "chooseMapIcon5Image", "chooseMapIcon6Image"};
const auto buttonHoverColor = graphics::Color(205, 128, 66);
}

class ChooseMapStateTest_Base : public Test
{
public:
    ChooseMapStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
        EXPECT_CALL(*mapsReader, readMapFilePaths()).WillOnce(Return(mapPaths));
        EXPECT_CALL(*mapsReader, readMapNames()).WillOnce(Return(mapPaths));
        expectHideAllIcons();
        EXPECT_CALL(*uiManager, activateComponent("chooseMapIcon1Image"));
        EXPECT_CALL(*uiManager, setColor("chooseMap1MapButton", buttonHoverColor));
    }

    void expectHideAllIcons()
    {
        for (const auto& iconName : iconNames)
        {
            EXPECT_CALL(*uiManager, deactivateComponent(iconName));
        }
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::shared_ptr<StrictMock<components::ui::UIManagerMock>> uiManager{
        std::make_shared<StrictMock<components::ui::UIManagerMock>>()};
    std::unique_ptr<StrictMock<MapsReaderMock>> mapsReaderInit{
        std::make_unique<StrictMock<MapsReaderMock>>()};
    StrictMock<MapsReaderMock>* mapsReader{mapsReaderInit.get()};
    StrictMock<input::InputMock> input;
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
};

class ChooseMapStateTest : public ChooseMapStateTest_Base
{
public:
    ChooseMapState chooseMapState{
        window, rendererPool, fileAccess, states, uiManager, tileMap, std::move(mapsReaderInit)};
};

TEST_F(ChooseMapStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("chooseMapIcon1Image"));

    chooseMapState.activate();
}

TEST_F(ChooseMapStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());

    chooseMapState.deactivate();
}

TEST_F(ChooseMapStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(chooseMapState.getType(), StateType::ChooseMap);
}

TEST_F(ChooseMapStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    chooseMapState.render();
}

TEST_F(ChooseMapStateTest, update_shouldUpdateUI)
{
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    chooseMapState.update(deltaTime, input);
}
