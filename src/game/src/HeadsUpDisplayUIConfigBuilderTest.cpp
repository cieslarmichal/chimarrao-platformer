#include "HeadsUpDisplayUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class HeadsUpDisplayUIConfigBuilderTest : public Test
{
public:
    HeadsUpDisplayUIConfigBuilderTest()
    {
        expectedImagesNames.insert(expectedImagesNames.begin(), slotIds.begin(), slotIds.end());
        expectedImagesNames.insert(expectedImagesNames.begin(), slotItemsIds.begin(), slotItemsIds.end());
    }

    const std::vector<std::string> expectedLabelsNames{"hudHealthPointsLabel", "hudItemsLabel"};
    std::vector<std::string> expectedImagesNames{"hudHealthPointsBar", "hudHealthPointsBarFrame"};
    const std::vector<std::string> slotIds{"slot1", "slot2", "slot3", "slot4",
                                           "slot5", "slot6", "slot7", "slot8"};
    const std::vector<std::string> slotItemsIds{"slotItem1", "slotItem2", "slotItem3",
                                                "slotItem4", "slotItem5", "slotItem6",
                                                "slotItem7", "slotItem8"};
};

TEST_F(HeadsUpDisplayUIConfigBuilderTest, createMenuUI)
{
    const auto headsUpDisplayUI = HeadsUpDisplayUIConfigBuilder::createUIConfig();

    std::vector<std::string> actualLabelsNames;
    std::transform(headsUpDisplayUI->labelsConfig.begin(), headsUpDisplayUI->labelsConfig.end(),
                   std::back_inserter(actualLabelsNames),
                   [](const auto& labelConfig) { return labelConfig->uniqueName; });

    std::vector<std::string> actualImagesNames;
    std::transform(headsUpDisplayUI->imagesConfig.begin(), headsUpDisplayUI->imagesConfig.end(),
                   std::back_inserter(actualImagesNames),
                   [](const auto& imageConfig) { return imageConfig->uniqueName; });

    ASSERT_EQ(headsUpDisplayUI->backgroundConfig, nullptr);
    ASSERT_TRUE(compareVectors(actualLabelsNames, expectedLabelsNames));
    ASSERT_TRUE(headsUpDisplayUI->buttonsConfig.empty());
    ASSERT_TRUE(headsUpDisplayUI->checkBoxesConfig.empty());
    ASSERT_TRUE(headsUpDisplayUI->textFieldsConfig.empty());
    ASSERT_TRUE(compareVectors(actualImagesNames, expectedImagesNames));
}

TEST_F(HeadsUpDisplayUIConfigBuilderTest, getHealthBarId)
{
    ASSERT_EQ(HeadsUpDisplayUIConfigBuilder::getHealthBarId(), "hudHealthPointsBar");
}

TEST_F(HeadsUpDisplayUIConfigBuilderTest, getHealthBarFrameId)
{
    ASSERT_EQ(HeadsUpDisplayUIConfigBuilder::getHealthBarFrameId(), "hudHealthPointsBarFrame");
}

TEST_F(HeadsUpDisplayUIConfigBuilderTest, getSlotIds)
{
    ASSERT_EQ(HeadsUpDisplayUIConfigBuilder::getSlotIds(), slotIds);
}

TEST_F(HeadsUpDisplayUIConfigBuilderTest, getSlotItemIds)
{
    ASSERT_EQ(HeadsUpDisplayUIConfigBuilder::getSlotItemIds(), slotItemsIds);
}