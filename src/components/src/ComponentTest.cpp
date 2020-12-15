#include "Component.h"

#include "gtest/gtest.h"

#include "ComponentOwner.h"

using namespace ::testing;
using namespace components;

class ComponentTest : public Test
{
public:
    const utils::Vector2f initialPosition{0.0, 11.0};
    ComponentOwner componentOwner{initialPosition};
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
