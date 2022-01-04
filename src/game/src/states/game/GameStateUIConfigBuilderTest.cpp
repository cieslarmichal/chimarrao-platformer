#include "GameStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "UIConfig.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

TEST(GameStateUIConfigBuilderTest, createGameUI)
{
    const auto gameUI = GameStateUIConfigBuilder::createGameUIConfig();

    ASSERT_EQ(gameUI->backgroundConfig->uniqueName, "gameBackground");
    ASSERT_TRUE(gameUI->buttonsConfig.empty());
    ASSERT_TRUE(gameUI->labelsConfig.empty());
    ASSERT_TRUE(gameUI->checkBoxesConfig.empty());
    ASSERT_TRUE(gameUI->textFieldsConfig.empty());
    ASSERT_TRUE(gameUI->imagesConfig.empty());
}