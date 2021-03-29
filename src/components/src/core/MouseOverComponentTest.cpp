#include "MouseOverComponent.h"

#include "gtest/gtest.h"

#include "InputManagerMock.h"

#include "ComponentOwner.h"

using namespace input;
using namespace components::core;
using namespace ::testing;

class MouseOverComponentTest_Base : public Test
{
public:
    MouseOverComponentTest_Base()
    {
        EXPECT_CALL(*inputManager, registerObserver(_));
        EXPECT_CALL(*inputManager, removeObserver(_));
    }

    std::shared_ptr<StrictMock<InputManagerMock>> inputManager =
        std::make_shared<StrictMock<InputManagerMock>>();
};

class MouseOverComponentTest : public MouseOverComponentTest_Base
{
public:
    MouseOverComponentTest()
    {
        auto hitboxComponent = componentOwner.addComponent<HitBoxComponent>(size, offset);
        mouseOverComponent.loadDependentComponents();
        hitboxComponent->lateUpdate(deltaTime);
    }

    InputStatus prepareInputStatus(const utils::Vector2f& mousePosition)
    {
        InputStatus inputStatus;
        inputStatus.setMousePosition(mousePosition);
        return inputStatus;
    }

    void mouseOverAction()
    {
        ++mouseOverVariable;
    }

    void mouseOutAction()
    {
        ++mouseOutVariable;
    }

    bool mouseWasOver() const
    {
        return mouseOverVariable > 0;
    }

    bool mouseWasOut() const
    {
        return mouseOutVariable > 0;
    }

    bool mouseWasOver(int times) const
    {
        return mouseOverVariable == times;
    }

    bool mouseWasOut(int times) const
    {
        return mouseOutVariable == times;
    }

    int mouseOverVariable{0};
    int mouseOutVariable{0};
    const utils::Vector2f size{5, 5};
    const utils::Vector2f offset{1, 1};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f positionInsideTarget1{21, 21};
    const utils::Vector2f positionInsideTarget2{23, 24};
    const utils::Vector2f positionOutsideTarget1{27, 21};
    const utils::Vector2f positionOutsideTarget2{27, 23};
    ComponentOwner componentOwner{position1, "mouseOverComponentTest"};
    utils::DeltaTime deltaTime{1};
    MouseOverComponent mouseOverComponent{&componentOwner, inputManager, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};
};

TEST_F(MouseOverComponentTest, givenMousePositionOutsideHitbox_shouldNotCallAnyAction)
{
    const auto mouseOutsideInput = prepareInputStatus(positionOutsideTarget1);

    mouseOverComponent.handleInputStatus(mouseOutsideInput);

    ASSERT_FALSE(mouseWasOut());
    ASSERT_FALSE(mouseWasOver());
}

TEST_F(MouseOverComponentTest, givenMousePositionInsideHitbox_shouldCallMouseOverAction)
{
    const auto mouseInsideInput = prepareInputStatus(positionInsideTarget1);
    mouseOverComponent.handleInputStatus(mouseInsideInput);

    mouseOverComponent.update(deltaTime);

    ASSERT_FALSE(mouseWasOut());
    ASSERT_TRUE(mouseWasOver(1));
}

TEST_F(MouseOverComponentTest,
       givenMousePositionInsideAndThenOutsideHitbox_shouldCallMouseOverActionAndMouseOverAction)
{
    const auto mouseInsideInput = prepareInputStatus(positionInsideTarget1);
    const auto mouseOutsideInput = prepareInputStatus(positionOutsideTarget1);

    mouseOverComponent.handleInputStatus(mouseInsideInput);
    mouseOverComponent.update(deltaTime);

    mouseOverComponent.handleInputStatus(mouseOutsideInput);
    mouseOverComponent.update(deltaTime);

    ASSERT_TRUE(mouseWasOut(1));
    ASSERT_TRUE(mouseWasOver(1));
}

TEST_F(MouseOverComponentTest, givenMousePositionInsideHitboxTwice_shouldCallMouseOverActionOnce)
{
    const auto mouseInsideInput1 = prepareInputStatus(positionInsideTarget1);
    const auto mouseInsideInput2 = prepareInputStatus(positionInsideTarget2);

    mouseOverComponent.handleInputStatus(mouseInsideInput1);
    mouseOverComponent.update(deltaTime);

    mouseOverComponent.handleInputStatus(mouseInsideInput2);
    mouseOverComponent.update(deltaTime);

    ASSERT_FALSE(mouseWasOut());
    ASSERT_TRUE(mouseWasOver(1));
}

TEST_F(MouseOverComponentTest, givenMousePositionInsideHitboxOutAndAgainInside_shouldCallMouseOverActionTwice)
{
    const auto mouseInsideInput1 = prepareInputStatus(positionInsideTarget1);
    const auto mouseOutsideInput = prepareInputStatus(positionOutsideTarget1);
    const auto mouseInsideInput2 = prepareInputStatus(positionInsideTarget2);

    mouseOverComponent.handleInputStatus(mouseInsideInput1);
    mouseOverComponent.update(deltaTime);

    mouseOverComponent.handleInputStatus(mouseOutsideInput);
    mouseOverComponent.update(deltaTime);

    mouseOverComponent.handleInputStatus(mouseInsideInput2);
    mouseOverComponent.update(deltaTime);

    ASSERT_TRUE(mouseWasOut(1));
    ASSERT_TRUE(mouseWasOver(2));
}

TEST_F(MouseOverComponentTest, givenMousePositionTwiceOverAndOut_shouldCallMouseOverAndOutTwice)
{
    const auto mouseInsideInput1 = prepareInputStatus(positionInsideTarget1);
    const auto mouseOutsideInput1 = prepareInputStatus(positionOutsideTarget1);
    const auto mouseInsideInput2 = prepareInputStatus(positionInsideTarget2);
    const auto mouseOutsideInput2 = prepareInputStatus(positionOutsideTarget2);

    mouseOverComponent.handleInputStatus(mouseInsideInput1);
    mouseOverComponent.update(deltaTime);

    mouseOverComponent.handleInputStatus(mouseOutsideInput1);
    mouseOverComponent.update(deltaTime);

    mouseOverComponent.handleInputStatus(mouseInsideInput2);
    mouseOverComponent.update(deltaTime);

    mouseOverComponent.handleInputStatus(mouseOutsideInput2);
    mouseOverComponent.update(deltaTime);

    ASSERT_TRUE(mouseWasOut(2));
    ASSERT_TRUE(mouseWasOver(2));
}

TEST_F(MouseOverComponentTest, componentDisabled_givenMousePositionInside_shouldNotCallAnyAction)
{
    const auto mouseInsideInput = prepareInputStatus(positionInsideTarget1);
    const auto mouseOutsideInput = prepareInputStatus(positionOutsideTarget1);
    mouseOverComponent.disable();

    mouseOverComponent.handleInputStatus(mouseInsideInput);
    mouseOverComponent.update(deltaTime);

    ASSERT_FALSE(mouseWasOut());
    ASSERT_FALSE(mouseWasOver());
}

TEST_F(MouseOverComponentTest,
       givenMousePositionInsideHitbox_thenComponentDisabled_shouldCallOverActionAndOutAction)
{
    const auto mouseInsideInput = prepareInputStatus(positionInsideTarget1);
    const auto mouseOutsideInput = prepareInputStatus(positionOutsideTarget1);
    mouseOverComponent.handleInputStatus(mouseInsideInput);
    mouseOverComponent.update(deltaTime);

    mouseOverComponent.disable();

    ASSERT_TRUE(mouseWasOut(1));
    ASSERT_TRUE(mouseWasOver(1));
}