#include "SaveMapStateUIConfigBuilder.h"

#include <editor/TileMapSerializerJson.h>

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "SaveMapState.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> expectedLabelNames{"saveMapTitleLabel", "saveMapMapNameLabel"};
const std::vector<std::string> expectedButtonNames{"saveMapCancelButton", "saveMapSaveButton"};
const std::vector<std::string> expectedTextFieldsNames{"saveMapNameTextField"};
}
class SaveMapStateUIConfigBuilderTest : public Test
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
    TileMap tileMap{
        "", {0, 0}, std::make_unique<TileMapSerializerJson>(), std::make_shared<utils::FileAccessMock>()};
    SaveMapState saveMapState{window,
                              rendererPool,
                              fileAccess,
                              states,
                              std::move(uiManagerInit),
                              std::make_shared<TileMap>(std::move(tileMap))};
};

TEST_F(SaveMapStateUIConfigBuilderTest, createSaveMapUI)
{
    const auto saveMapUI = SaveMapStateUIConfigBuilder::createSaveMapUIConfig(&saveMapState);

    std::vector<std::string> actualLabelNames;
    std::transform(saveMapUI->labelsConfig.begin(), saveMapUI->labelsConfig.end(),
                   std::back_inserter(actualLabelNames),
                   [](const auto& labelConfig) { return labelConfig->uniqueName; });

    std::vector<std::string> actualButtonsNames;
    std::transform(saveMapUI->buttonsConfig.begin(), saveMapUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    std::vector<std::string> actualTextFieldsNames;
    std::transform(saveMapUI->textFieldsConfig.begin(), saveMapUI->textFieldsConfig.end(),
                   std::back_inserter(actualTextFieldsNames),
                   [](const auto& textFieldConfig) { return textFieldConfig->uniqueName; });

    ASSERT_EQ(saveMapUI->backgroundConfig->uniqueName, "saveMapBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(saveMapUI->checkBoxesConfig.empty());
    ASSERT_TRUE(compareVectors(actualTextFieldsNames, expectedTextFieldsNames));
    ASSERT_TRUE(saveMapUI->imagesConfig.empty());
}