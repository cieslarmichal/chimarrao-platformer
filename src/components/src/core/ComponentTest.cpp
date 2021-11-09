#include "Component.h"

#include "gtest/gtest.h"

#include "ComponentOwner.h"
#include "RendererPoolMock.h"

using namespace ::testing;
using namespace components::core;

class ComponentTest : public Test
{
public:
    const utils::Vector2f initialPosition{0.0, 11.0};
    const std::string ownerName{"componentTest"};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{initialPosition, ownerName, sharedContext};
    Component component{&componentOwner};
};

TEST_F(ComponentTest, initialComponent_shouldBeEnabled)
{
    ASSERT_TRUE(component.isEnabled());
}

TEST_F(ComponentTest, disableComponent)
{
    component.disable();

    ASSERT_FALSE(component.isEnabled());
}

TEST_F(ComponentTest, enableComponent)
{
    component.disable();

    component.enable();

    ASSERT_TRUE(component.isEnabled());
}

TEST_F(ComponentTest, shouldReturnOwnerName)
{
    const auto actualOwnerName = component.getOwnerName();

    ASSERT_EQ(actualOwnerName, ownerName);
}

TEST_F(ComponentTest, newlyCreatedComponent_shouldNotBeSetToRemoval)
{
    ASSERT_FALSE(component.shouldBeRemoved());
}

TEST_F(ComponentTest, givenOwnerSetToRemoval_componentShouldBeSetToRemoveAsWell)
{
    componentOwner.remove();

    ASSERT_TRUE(component.shouldBeRemoved());
}
