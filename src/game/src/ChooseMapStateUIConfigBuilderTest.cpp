#include "ChooseMapStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "ChooseMapState.h"
#include "ProjectPathReader.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> expectedLabelNames{"chooseMapTitleLabel"};

const std::vector<std::string> expectedButtonNames{"chooseMapBackToMenuButton", "chooseMapRightButton",
                                                   "chooseMapLeftButton", "chooseMap1MapButton", "chooseMap2MapButton"};
}

class ChooseMapStateUIConfigBuilderTest_Base : public Test
{
public:
    ChooseMapStateUIConfigBuilderTest_Base()
    {
        EXPECT_CALL(*fileAccess, getAllPathsFromDirectory(mapsDirectory))
            .WillOnce(Return(std::vector<std::string>{"map1.txt", "map2.txt"}));
        EXPECT_CALL(*fileAccess, getFileNameWithoutExtension("map1.txt"))
            .WillRepeatedly(Return("map1"));
        EXPECT_CALL(*fileAccess, getFileNameWithoutExtension("map2.txt"))
            .WillRepeatedly(Return("map2"));
    }

    const std::string mapsDirectory = utils::ProjectPathReader::getProjectRootPath() + "maps";

    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::unique_ptr<components::ui::UIManagerMock> uiManagerInit{
        std::make_unique<NiceMock<components::ui::UIManagerMock>>()};
    components::ui::UIManagerMock* uiManager{uiManagerInit.get()};
};

class ChooseMapStateUIConfigBuilderTest : public ChooseMapStateUIConfigBuilderTest_Base
{
public:
    ChooseMapState chooseMapState{window, rendererPool, fileAccess, states, std::move(uiManagerInit)};
};

TEST_F(ChooseMapStateUIConfigBuilderTest, createChooseMapUI)
{
    const auto chooseMapUI = ChooseMapStateUIConfigBuilder::createChooseMapUIConfig(&chooseMapState);

    std::vector<std::string> actualLabelNames;
    std::transform(chooseMapUI->labelsConfig.begin(), chooseMapUI->labelsConfig.end(),
                   std::back_inserter(actualLabelNames),
                   [](const auto& labelConfig) { return labelConfig->uniqueName; });

    std::vector<std::string> actualButtonsNames;
    std::transform(chooseMapUI->buttonsConfig.begin(), chooseMapUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    ASSERT_EQ(chooseMapUI->backgroundConfig->uniqueName, "chooseMapBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(chooseMapUI->checkBoxesConfig.empty());
    ASSERT_TRUE(chooseMapUI->textFieldsConfig.empty());
}