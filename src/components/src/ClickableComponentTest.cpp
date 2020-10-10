#include "ClickableComponent.h"

#include "gtest/gtest.h"

#include "InputManagerMock.h"

#include "ComponentOwner.h"

using namespace components;
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
        auto hitboxComponent = componentOwner.addComponent<HitboxComponent>(size, offset);
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
        return inputStatus;
    }

    void clickAction()
    {
        actionVariable = clickValue;
    }

    bool actionPerformed() const
    {
        return actionVariable == clickValue;
    }

    int actionVariable{0};
    const int clickValue{42};
    const utils::Vector2f size{5, 5};
    const utils::Vector2f offset{1, 1};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f positionInsideTarget{21, 21};
    const utils::Vector2f positionOutsideTarget{27, 21};
    ComponentOwner componentOwner{position1};
    utils::DeltaTime deltaTime{1};
    ClickableComponent clickableComponent{&componentOwner, inputManager, [this] { clickAction(); }};
};

TEST_F(ClickableComponentTest, givenMousePositionOutsideHitboxAndLeftMouseKeyNotClicked_shouldNotCallAction)
{
    const auto inputStatus = prepareInputStatus(positionOutsideTarget);
    clickableComponent.handleInputStatus(inputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_FALSE(actionPerformed());
}

TEST_F(ClickableComponentTest, givenMousePositionOutsideHitboxAndLeftMouseKeyClicked_shouldNotCallAction)
{
    const auto mouseLeftKeyInputStatus = prepareInputStatus(InputKey::MouseLeft, positionOutsideTarget);
    clickableComponent.handleInputStatus(mouseLeftKeyInputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_FALSE(actionPerformed());
}

TEST_F(ClickableComponentTest, givenMousePositionInsideHitboxAndLeftMouseKeyNotClicked_shouldNotCallAction)
{
    const auto inputStatus = prepareInputStatus(positionInsideTarget);
    clickableComponent.handleInputStatus(inputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_FALSE(actionPerformed());
}

TEST_F(ClickableComponentTest, givenMousePositionInsideHitboxRightMouseKeyClicked_shouldNotCallAction)
{
    const auto mouseRightKeyInputStatus = prepareInputStatus(InputKey::MouseRight, positionInsideTarget);
    clickableComponent.handleInputStatus(mouseRightKeyInputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_FALSE(actionPerformed());
}


TEST_F(ClickableComponentTest, givenMousePositionInsideHitboxLeftMouseKeyClicked_shouldCallAction)
{
    const auto mouseLeftInputStatus = prepareInputStatus(InputKey::MouseLeft, positionInsideTarget);
    clickableComponent.handleInputStatus(mouseLeftInputStatus);

    clickableComponent.update(deltaTime);

    ASSERT_TRUE(actionPerformed());
}

TEST_F(ClickableComponentTest, componentDisabled_givenMousePositionInsideHitboxLeftMouseKeyClicked_shouldNotCallAction)
{
    const auto mouseLeftInputStatus = prepareInputStatus(InputKey::MouseLeft, positionInsideTarget);
    clickableComponent.handleInputStatus(mouseLeftInputStatus);
    clickableComponent.disable();

    clickableComponent.update(deltaTime);

    ASSERT_FALSE(actionPerformed());
}