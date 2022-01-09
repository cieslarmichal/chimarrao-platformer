#include "LimitedSpaceActionComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "TimerMock.h"

#include "ComponentOwner.h"
#include "DefaultQuadtree.h"
#include "DefaultRayCast.h"
#include "ProjectPathReader.h"
#include "exceptions/DependentComponentNotFound.h"

using namespace components::core;
using namespace ::testing;

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto dummyFontSize = 4;
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
}

class LimitedSpaceActionComponentTest : public Test
{
public:
    LimitedSpaceActionComponentTest()
    {
        owner.addComponent<TextComponent>(rendererPool, position1, "press E to action", fontPath,
                                          dummyFontSize);
        owner.loadDependentComponents();
        limitedSpaceActionComponent.loadDependentComponents();
    }

    void clickAction(int& actionVariableInit)
    {
        actionVariableInit = clickValue;
    }

    bool actionPerformed(int actionVariableInit) const
    {
        return actionVariableInit == clickValue;
    }

    int actionVariable{0};
    const int clickValue{42};
    const utils::Vector2f size{4, 4};
    const utils::Vector2f position1{8, 0};
    const utils::Vector2f position2{5, 0};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner owner{position1, "LimitedSpaceActionComponentTest1", sharedContext};
    ComponentOwner player{position2, "LimitedSpaceActionComponentTest2", sharedContext};
    StrictMock<input::InputMock> input;
    const utils::DeltaTime deltaTime{1};
    LimitedSpaceActionComponent limitedSpaceActionComponent{&owner, &player,
                                                            [this] { clickAction(actionVariable); }};
};

TEST_F(LimitedSpaceActionComponentTest,
       loadDependentComponentsWithoutTextComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutText{position1, "componentOwnerWithoutTextComponent", sharedContext};
    LimitedSpaceActionComponent limitedSpaceActionComponentWithoutTextComponent{
        &componentOwnerWithoutText, &player, [this] { clickAction(actionVariable); }};

    ASSERT_THROW(limitedSpaceActionComponentWithoutTextComponent.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(LimitedSpaceActionComponentTest, distanceBetweenOwnerAndPlayerIsMoreThan6_shouldNotCallAction)
{
    player.transform->setPosition(100, 50);

    limitedSpaceActionComponent.update(deltaTime, input);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(
    LimitedSpaceActionComponentTest,
    distanceBetweenOwnerAndPlayerIsLessThan6AndKeyNotPressed_shouldAndEnableTextAndNotCallAction)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::E)).WillOnce(Return(false));

    limitedSpaceActionComponent.update(deltaTime, input);

    ASSERT_TRUE(owner.getComponent<TextComponent>()->isEnabled());
    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(
    LimitedSpaceActionComponentTest,
    distanceBetweenOwnerAndPlayerIsLessThan6AndKeyPressed_shouldrAndDisableTextAndCallAction)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::E)).WillOnce(Return(true));

    limitedSpaceActionComponent.update(deltaTime, input);

    ASSERT_FALSE(owner.getComponent<TextComponent>()->isEnabled());
    ASSERT_TRUE(actionPerformed(actionVariable));
}

TEST_F(
    LimitedSpaceActionComponentTest,
    distanceBetweenOwnerAndPlayerIsLessThan6AndThenPlayerGoesAwayWithoutPressingKey_shoulddDisableText)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::E)).WillOnce(Return(false));

    limitedSpaceActionComponent.update(deltaTime, input);
    player.transform->setPosition(100, 50);
    limitedSpaceActionComponent.update(deltaTime, input);

    ASSERT_FALSE(owner.getComponent<TextComponent>()->isEnabled());
    ASSERT_FALSE(actionPerformed(actionVariable));
}
