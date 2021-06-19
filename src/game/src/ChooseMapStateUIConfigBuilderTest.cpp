#include "ChooseMapStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "MapsReaderMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "editor/TileMapMock.h"
#include "ui/UIManagerMock.h"

#include "ChooseMapState.h"
#include "ProjectPathReader.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> mapPaths{"x1", "x2", "x3", "x4", "x5", "x6"};
const std::vector<std::string> expectedLabelNames{"chooseMapTitleLabel"};
const std::vector<std::string> expectedButtonNames{
    "chooseMapBackToMenuButton", "chooseMapRightButton", "chooseMapLeftButton", "chooseMap1MapButton",
    "chooseMap2MapButton",       "chooseMap3MapButton",  "chooseMap4MapButton", "chooseMap5MapButton"};
const std::vector<std::string> expectedNonNavigationButtonNames{
    "chooseMap1MapButton", "chooseMap2MapButton", "chooseMap3MapButton",
    "chooseMap4MapButton", "chooseMap5MapButton", "chooseMapBackToMenuButton"};
const std::vector<std::string> expectedImagesNames{"chooseMapIcon1Image", "chooseMapIcon2Image",
                                                   "chooseMapIcon3Image", "chooseMapIcon4Image",
                                                   "chooseMapIcon5Image", "chooseMapIcon6Image"};
}

class ChooseMapStateUIConfigBuilderTest_Base : public Test
{
public:
    ChooseMapStateUIConfigBuilderTest_Base()
    {
        EXPECT_CALL(*mapsReader, readMapFilePaths()).WillOnce(Return(mapPaths));
        EXPECT_CALL(*mapsReader, readMapNames()).WillOnce(Return(mapPaths));
    }

    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::shared_ptr<components::ui::UIManagerMock> uiManager{
        std::make_shared<NiceMock<components::ui::UIManagerMock>>()};
    std::unique_ptr<StrictMock<MapsReaderMock>> mapsReaderInit{
        std::make_unique<StrictMock<MapsReaderMock>>()};
    StrictMock<MapsReaderMock>* mapsReader{mapsReaderInit.get()};
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
};

class ChooseMapStateUIConfigBuilderTest : public ChooseMapStateUIConfigBuilderTest_Base
{
public:
    ChooseMapState chooseMapState{
        window, rendererPool, fileAccess, states, uiManager, tileMap, std::move(mapsReaderInit)};
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

    std::vector<std::string> actualImagesNames;
    std::transform(chooseMapUI->imagesConfig.begin(), chooseMapUI->imagesConfig.end(),
                   std::back_inserter(actualImagesNames),
                   [](const auto& imageConfig) { return imageConfig->uniqueName; });

    ASSERT_EQ(chooseMapUI->backgroundConfig->uniqueName, "chooseMapBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(chooseMapUI->checkBoxesConfig.empty());
    ASSERT_TRUE(chooseMapUI->textFieldsConfig.empty());
    ASSERT_TRUE(compareVectors(actualImagesNames, expectedImagesNames));
}

TEST_F(ChooseMapStateUIConfigBuilderTest, getButtonNames)
{
    const auto actualNonNavigationButtonNames = ChooseMapStateUIConfigBuilder::getNonNavigationButtonNames();

    ASSERT_EQ(actualNonNavigationButtonNames, expectedNonNavigationButtonNames);
}

TEST_F(ChooseMapStateUIConfigBuilderTest, getIconNames)
{
    const auto actualIconNames = ChooseMapStateUIConfigBuilder::getIconNames();

    ASSERT_EQ(actualIconNames, expectedImagesNames);
}
