#include "MouseOverComponent.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "exceptions/DependentComponentNotFound.h"

using namespace input;
using namespace components::core;
using namespace ::testing;

class MouseOverComponentTest : public Test
{
public:
    void prepareMouseOverComponent(MouseOverComponent& mouseOverComponent)
    {
        auto boxCollider =
            componentOwner.addComponent<BoxColliderComponent>(size, CollisionLayer::Default, offset);
        mouseOverComponent.loadDependentComponents();
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
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position1, "mouseOverComponentTest", sharedContext};
    utils::DeltaTime deltaTime{1};
    std::unique_ptr<StrictMock<input::InputMock>> inputInit{std::make_unique<StrictMock<input::InputMock>>()};
    StrictMock<input::InputMock>* input{inputInit.get()};
};

TEST_F(MouseOverComponentTest,
       loadDependentComponentsWithoutBoxColliderComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutBoxCollider{position1, "componentOwnerWithoutBoxCollider",
                                                    sharedContext};
    MouseOverComponent mouseOverComponentWithoutBoxCollider{
        &componentOwnerWithoutBoxCollider, [this] { mouseOverAction(); }, [this] { mouseOutAction(); }};
    ASSERT_THROW(mouseOverComponentWithoutBoxCollider.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(MouseOverComponentTest, givenMousePositionOutsideHitbox_shouldNotCallAnyAction)
{
    MouseOverComponent mouseOverComponent{&componentOwner, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};
    prepareMouseOverComponent(mouseOverComponent);
    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionOutsideTarget1));

    mouseOverComponent.update(deltaTime, *input);

    ASSERT_FALSE(mouseWasOut());
    ASSERT_FALSE(mouseWasOver());
}

TEST_F(MouseOverComponentTest, givenMousePositionInsideHitbox_shouldCallMouseOverAction)
{
    MouseOverComponent mouseOverComponent{&componentOwner, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};
    prepareMouseOverComponent(mouseOverComponent);
    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionInsideTarget1));

    mouseOverComponent.update(deltaTime, *input);

    ASSERT_FALSE(mouseWasOut());
    ASSERT_TRUE(mouseWasOver(1));
}

TEST_F(MouseOverComponentTest,
       givenMousePositionInsideAndThenOutsideHitbox_shouldCallMouseOverActionAndMouseOverAction)
{
    MouseOverComponent mouseOverComponent{&componentOwner, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};
    prepareMouseOverComponent(mouseOverComponent);

    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionInsideTarget1));
    mouseOverComponent.update(deltaTime, *input);

    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionOutsideTarget1));
    mouseOverComponent.update(deltaTime, *input);

    ASSERT_TRUE(mouseWasOut(1));
    ASSERT_TRUE(mouseWasOver(1));
}

TEST_F(MouseOverComponentTest, givenMousePositionInsideHitboxTwice_shouldCallMouseOverActionOnce)
{
    MouseOverComponent mouseOverComponent{&componentOwner, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};
    prepareMouseOverComponent(mouseOverComponent);

    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionInsideTarget1));
    mouseOverComponent.update(deltaTime, *input);

    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionInsideTarget2));
    mouseOverComponent.update(deltaTime, *input);

    ASSERT_FALSE(mouseWasOut());
    ASSERT_TRUE(mouseWasOver(1));
}

TEST_F(MouseOverComponentTest, givenMousePositionInsideHitboxOutAndAgainInside_shouldCallMouseOverActionTwice)
{
    MouseOverComponent mouseOverComponent{&componentOwner, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};
    prepareMouseOverComponent(mouseOverComponent);

    EXPECT_CALL(*input, getMousePosition())
        .WillOnce(Return(positionInsideTarget1))
        .WillOnce(Return(positionOutsideTarget1))
        .WillOnce(Return(positionInsideTarget2));
    mouseOverComponent.update(deltaTime, *input);
    mouseOverComponent.update(deltaTime, *input);
    mouseOverComponent.update(deltaTime, *input);

    ASSERT_TRUE(mouseWasOut(1));
    ASSERT_TRUE(mouseWasOver(2));
}

TEST_F(MouseOverComponentTest, givenMousePositionTwiceOverAndOut_shouldCallMouseOverAndOutTwice)
{
    MouseOverComponent mouseOverComponent{&componentOwner, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};
    prepareMouseOverComponent(mouseOverComponent);

    EXPECT_CALL(*input, getMousePosition())
        .WillOnce(Return(positionInsideTarget1))
        .WillOnce(Return(positionOutsideTarget1))
        .WillOnce(Return(positionInsideTarget2))
        .WillOnce(Return(positionOutsideTarget2));

    mouseOverComponent.update(deltaTime, *input);
    mouseOverComponent.update(deltaTime, *input);
    mouseOverComponent.update(deltaTime, *input);
    mouseOverComponent.update(deltaTime, *input);

    ASSERT_TRUE(mouseWasOut(2));
    ASSERT_TRUE(mouseWasOver(2));
}

TEST_F(MouseOverComponentTest, componentDisabled_givenMousePositionInside_shouldNotCallAnyAction)
{
    MouseOverComponent mouseOverComponent{&componentOwner, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};
    prepareMouseOverComponent(mouseOverComponent);
    mouseOverComponent.disable();

    mouseOverComponent.update(deltaTime, *input);

    ASSERT_FALSE(mouseWasOut());
    ASSERT_FALSE(mouseWasOver());
}

TEST_F(MouseOverComponentTest,
       givenMousePositionInsideHitbox_thenComponentDisabled_shouldCallOverActionAndOutAction)
{
    MouseOverComponent mouseOverComponent{&componentOwner, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};
    prepareMouseOverComponent(mouseOverComponent);

    EXPECT_CALL(*input, getMousePosition()).WillOnce(Return(positionInsideTarget1));
    mouseOverComponent.update(deltaTime, *input);

    mouseOverComponent.disable();

    ASSERT_TRUE(mouseWasOut(1));
    ASSERT_TRUE(mouseWasOver(1));
}

TEST_F(MouseOverComponentTest, shouldInvokeMouseOverAction)
{
    MouseOverComponent mouseOverComponent{&componentOwner, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};

    mouseOverComponent.invokeMouseOverAction();

    ASSERT_TRUE(mouseWasOver(1));
}

TEST_F(MouseOverComponentTest, shouldInvokeMouseOutAction)
{
    MouseOverComponent mouseOverComponent{&componentOwner, [this] { mouseOverAction(); },
                                          [this] { mouseOutAction(); }};

    mouseOverComponent.invokeMouseOutAction();

    ASSERT_TRUE(mouseWasOut(1));
}