#include "ClickableComponent.h"

#include "gtest/gtest.h"

#include "InputMock.h"

#include "ComponentOwner.h"
#include "exceptions/ActionForKeyAlreadyExists.h"

using namespace components::core;
using namespace ::testing;
using namespace input;

class ClickableComponentTest : public Test
{
public:
    ClickableComponentTest()
    {
        auto boxCollider = owner.addComponent<BoxColliderComponent>(size, offset);
        clickableComponent.loadDependentComponents();
        boxCollider->lateUpdate(deltaTime);
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
    int actionVariableLeft{0};
    int actionVariableRight{0};
    int actionVariableRight2{0};
    const int clickValue{42};
    const utils::Vector2f size{5, 5};
    const utils::Vector2f offset{1, 1};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f positionInsideTarget{21, 21};
    const utils::Vector2f positionOutsideTarget{27, 21};
    ComponentOwner owner{position1, "clickableComponentOwnerTest"};
    utils::DeltaTime deltaTime{1};
    std::unique_ptr<StrictMock<input::InputMock>> inputInit{std::make_unique<StrictMock<input::InputMock>>()};
    StrictMock<input::InputMock>* input{inputInit.get()};
    ClickableComponent clickableComponent{&owner, [this] { clickAction(actionVariable); }};
    KeyAction keyAction1 = {InputKey::MouseLeft, [this] { clickAction(actionVariableLeft); }};
    KeyAction keyAction1copy = {InputKey::MouseLeft, [] {}};
    KeyAction keyAction2 = {InputKey::MouseRight, [this] { clickAction(actionVariableRight); }};
    KeyAction keyAction3 = {InputKey::MouseRight, [this] { clickAction(actionVariableRight2); }};
    std::vector<KeyAction> invalidKeyActionVector{keyAction1, keyAction1copy};
    std::vector<KeyAction> validKeyActionVector{keyAction1, keyAction2};
    std::vector<KeyAction> validKeyActionVectorAfterChange{keyAction3};
};

TEST_F(ClickableComponentTest, givenMousePositionOutsideHitboxAndLeftMouseKeyNotClicked_shouldNotCallAction)
{
    EXPECT_CALL(*input, isKeyReleased(InputKey::MouseLeft)).WillOnce(Return(false));

    clickableComponent.update(deltaTime, *input);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest, givenMousePositionOutsideHitboxAndLeftMouseKeyClicked_shouldNotCallAction)
{
    EXPECT_CALL(*input, isKeyReleased(InputKey::MouseLeft)).WillOnce(Return(true));
    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionOutsideTarget));

    clickableComponent.update(deltaTime, *input);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest, givenMousePositionInsideHitboxAndLeftMouseKeyNotClicked_shouldNotCallAction)
{
    EXPECT_CALL(*input, isKeyReleased(InputKey::MouseLeft)).WillOnce(Return(false));

    clickableComponent.update(deltaTime, *input);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest, givenMousePositionInsideHitboxLeftMouseKeyClicked_shouldCallAction)
{
    EXPECT_CALL(*input, isKeyReleased(InputKey::MouseLeft)).WillOnce(Return(true));
    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionInsideTarget));

    clickableComponent.update(deltaTime, *input);

    ASSERT_TRUE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest,
       componentDisabled_givenMousePositionInsideHitboxLeftMouseKeyClicked_shouldNotCallAction)
{
    clickableComponent.disable();

    clickableComponent.update(deltaTime, *input);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest,
       multipleKeyActionClicableComponentConstructor_givenMoreThanOneActionForOneKey_shouldThrowException)
{
    ASSERT_THROW(ClickableComponent(&owner, invalidKeyActionVector),
                 components::core::exceptions::ActionForKeyAlreadyExists);
}

TEST_F(ClickableComponentTest,
       givenMousePositionInsideHitboxRightAndLeftMouseKeyClicked_shouldCallBothActionAtOneUpdate)
{
    ComponentOwner localComponentOwner{position1, "clickableComponentOwner1"};
    auto boxCollider = localComponentOwner.addComponent<BoxColliderComponent>(size, offset);
    auto localClickableComponent = ClickableComponent(&localComponentOwner, validKeyActionVector);
    localClickableComponent.loadDependentComponents();
    EXPECT_CALL(*input, isKeyReleased(InputKey::MouseRight)).WillOnce(Return(true));
    EXPECT_CALL(*input, isKeyReleased(InputKey::MouseLeft)).WillOnce(Return(true));
    EXPECT_CALL(*input, getMousePosition()).WillRepeatedly(Return(positionInsideTarget));

    localClickableComponent.update(deltaTime, *input);

    ASSERT_TRUE(actionPerformed(actionVariableRight));
    ASSERT_TRUE(actionPerformed(actionVariableLeft));
}

TEST_F(ClickableComponentTest,
       givenMousePositionInsideHitboxRightMouseKeyClicked_shouldCallOnlyRightClickAction)
{
    ComponentOwner localComponentOwner{position1, "clickableComponentOwner2"};
    auto boxCollider = localComponentOwner.addComponent<BoxColliderComponent>(size, offset);
    auto localClickableComponent = ClickableComponent(&localComponentOwner, validKeyActionVector);
    localClickableComponent.loadDependentComponents();
    EXPECT_CALL(*input, isKeyReleased(InputKey::MouseLeft)).WillOnce(Return(false));
    EXPECT_CALL(*input, isKeyReleased(InputKey::MouseRight)).WillOnce(Return(true));
    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionInsideTarget));

    localClickableComponent.update(deltaTime, *input);

    ASSERT_TRUE(actionPerformed(actionVariableRight));
    ASSERT_FALSE(actionPerformed(actionVariableLeft));
}

TEST_F(ClickableComponentTest,
       givenTwoKeyActions_thenChangeIntoOtherKeyAction_shouldBeAbleToOnlyPerformOtherOne)
{
    ComponentOwner localComponentOwner{position1, "clickableComponentOwner3"};
    auto boxCollider = localComponentOwner.addComponent<BoxColliderComponent>(size, offset);
    auto localClickableComponent = ClickableComponent(&localComponentOwner, validKeyActionVector);
    localClickableComponent.loadDependentComponents();

    localClickableComponent.setKeyActions(validKeyActionVectorAfterChange);

    EXPECT_CALL(*input, isKeyReleased(InputKey::MouseRight)).WillOnce(Return(true));
    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionInsideTarget));

    localClickableComponent.update(deltaTime, *input);

    ASSERT_FALSE(actionPerformed(actionVariableLeft));
    ASSERT_FALSE(actionPerformed(actionVariableRight));
    ASSERT_TRUE(actionPerformed(actionVariableRight2));
}
