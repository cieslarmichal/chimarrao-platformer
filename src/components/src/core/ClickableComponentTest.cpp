#include "ClickableComponent.h"

#include "exceptions/ActionForKeyAlreadyExists.h"

#include "gtest/gtest.h"

#include "InputManagerMock.h"

#include "ComponentOwner.h"
#include "InputKey.h"

using namespace components::core;
using namespace ::testing;
using namespace input;

class ClickableComponentTest_Base : public Test
{
public:
    ClickableComponentTest_Base()
    {
        EXPECT_CALL(*inputManager, registerObserver(_));
        EXPECT_CALL(*inputManager, removeObserver(_));
    }

    std::shared_ptr<StrictMock<input::InputManagerMock>> inputManager =
        std::make_shared<StrictMock<input::InputManagerMock>>();
};

class ClickableComponentTest : public ClickableComponentTest_Base
{
public:
    ClickableComponentTest()
    {
        auto hitboxComponent = componentOwner.addComponent<HitBoxComponent>(size, offset);
        clickableComponent.loadDependentComponents();
        hitboxComponent->lateUpdate(deltaTime);
    }

    InputStatus prepareInputStatus(const utils::Vector2f& mousePosition)
    {
        InputStatus inputStatus;
        inputStatus.setMousePosition(mousePosition);
        return inputStatus;
    }

    InputStatus prepareInputStatus(InputKey inputKey, const utils::Vector2f& mousePosition)
    {
        InputStatus inputStatus;
        inputStatus.setKeyPressed(inputKey);
        inputStatus.setMousePosition(mousePosition);
        inputStatus.setReleasedKeys();
        inputStatus.clearPressedKeys();
        inputStatus.setReleasedKeys();
        return inputStatus;
    }

    InputStatus prepareInputStatus(InputKey inputKey1, InputKey inputKey2,
                                   const utils::Vector2f& mousePosition)
    {
        InputStatus inputStatus;
        inputStatus.setKeyPressed(inputKey1);
        inputStatus.setKeyPressed(inputKey2);
        inputStatus.setMousePosition(mousePosition);
        inputStatus.setReleasedKeys();
        inputStatus.clearPressedKeys();
        inputStatus.setReleasedKeys();
        return inputStatus;
    }

    void clickAction(int& actionVariable)
    {
        actionVariable = clickValue;
    }

    bool actionPerformed(int actionVariable) const
    {
        return actionVariable == clickValue;
    }

    int actionVariable{0};
    int actionVariableLeft{0};
    int actionVariableRight{0};
    const int clickValue{42};
    const utils::Vector2f size{5, 5};
    const utils::Vector2f offset{1, 1};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f positionInsideTarget{21, 21};
    const utils::Vector2f positionOutsideTarget{27, 21};
    ComponentOwner componentOwner{position1, "clickableComponentOwnerTest"};
    utils::DeltaTime deltaTime{1};
    ClickableComponent clickableComponent{&componentOwner, inputManager,
                                          [this] { clickAction(actionVariable); }};
    KeyAction keyAction1 = {input::InputKey::MouseLeft, [this] { clickAction(actionVariableLeft); }};
    KeyAction keyAction1copy = {input::InputKey::MouseLeft, [] {}};
    KeyAction keyAction2 = {input::InputKey::MouseRight, [this] { clickAction(actionVariableRight); }};
    std::vector<KeyAction> invalidKeyActionVector{keyAction1, keyAction1copy};
    std::vector<KeyAction> validKeyActionVector{keyAction1, keyAction2};
};

TEST_F(ClickableComponentTest, givenMousePositionOutsideHitboxAndLeftMouseKeyNotClicked_shouldNotCallAction)
{
    const auto inputStatus = prepareInputStatus(positionOutsideTarget);
    clickableComponent.handleInputStatus(inputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest, givenMousePositionOutsideHitboxAndLeftMouseKeyClicked_shouldNotCallAction)
{
    const auto mouseLeftKeyInputStatus = prepareInputStatus(InputKey::MouseLeft, positionOutsideTarget);
    clickableComponent.handleInputStatus(mouseLeftKeyInputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest, givenMousePositionInsideHitboxAndLeftMouseKeyNotClicked_shouldNotCallAction)
{
    const auto inputStatus = prepareInputStatus(positionInsideTarget);
    clickableComponent.handleInputStatus(inputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest, givenMousePositionInsideHitboxRightMouseKeyClicked_shouldNotCallAction)
{
    const auto mouseRightKeyInputStatus = prepareInputStatus(InputKey::MouseRight, positionInsideTarget);
    clickableComponent.handleInputStatus(mouseRightKeyInputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest, givenMousePositionInsideHitboxLeftMouseKeyClicked_shouldCallAction)
{
    const auto mouseLeftInputStatus = prepareInputStatus(InputKey::MouseLeft, positionInsideTarget);
    clickableComponent.handleInputStatus(mouseLeftInputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_TRUE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest,
       componentDisabled_givenMousePositionInsideHitboxLeftMouseKeyClicked_shouldNotCallAction)
{
    const auto mouseLeftInputStatus = prepareInputStatus(InputKey::MouseLeft, positionInsideTarget);
    clickableComponent.handleInputStatus(mouseLeftInputStatus);
    clickableComponent.disable();

    clickableComponent.update(deltaTime);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(ClickableComponentTest,
       multipleKeyActionClicableComponentConstructor_givenMoreThanOneActionForOneKey_shouldThrowException)
{
    EXPECT_CALL(*inputManager, registerObserver(_));
    ASSERT_THROW(ClickableComponent(&componentOwner, inputManager, invalidKeyActionVector),
                 components::core::exceptions::ActionForKeyAlreadyExists);
}

TEST_F(ClickableComponentTest,
       givenMousePositionInsideHitboxRightAndLeftMouseKeyClicked_shouldCallBothActionAtOneUpdate)
{
    std::shared_ptr<StrictMock<input::InputManagerMock>> inputManager =
        std::make_shared<StrictMock<input::InputManagerMock>>();
    EXPECT_CALL(*inputManager, registerObserver(_));
    EXPECT_CALL(*inputManager, removeObserver(_));
    ComponentOwner localComponentOwner{position1, "clickableComponentOwner1"};
    auto hitboxComponent = localComponentOwner.addComponent<HitBoxComponent>(size, offset);
    hitboxComponent->lateUpdate(deltaTime);
    auto clickableComponent = ClickableComponent(&localComponentOwner, inputManager, validKeyActionVector);
    clickableComponent.loadDependentComponents();
    const auto mouseInputStatus =
        prepareInputStatus(InputKey::MouseLeft, InputKey::MouseRight, positionInsideTarget);
    clickableComponent.handleInputStatus(mouseInputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_TRUE(actionPerformed(actionVariableRight));
    ASSERT_TRUE(actionPerformed(actionVariableLeft));
}

TEST_F(ClickableComponentTest,
       givenMousePositionInsideHitboxRightMouseKeyClicked_shouldCallOnlyRightClickAction)
{
    std::shared_ptr<StrictMock<input::InputManagerMock>> inputManager =
        std::make_shared<StrictMock<input::InputManagerMock>>();
    EXPECT_CALL(*inputManager, registerObserver(_));
    EXPECT_CALL(*inputManager, removeObserver(_));
    ComponentOwner localComponentOwner{position1, "clickableComponentOwner2"};
    auto hitboxComponent = localComponentOwner.addComponent<HitBoxComponent>(size, offset);
    hitboxComponent->lateUpdate(deltaTime);
    auto clickableComponent = ClickableComponent(&localComponentOwner, inputManager, validKeyActionVector);
    clickableComponent.loadDependentComponents();
    const auto mouseInputStatus = prepareInputStatus(InputKey::MouseRight, positionInsideTarget);
    clickableComponent.handleInputStatus(mouseInputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_TRUE(actionPerformed(actionVariableRight));
    ASSERT_FALSE(actionPerformed(actionVariableLeft));
}