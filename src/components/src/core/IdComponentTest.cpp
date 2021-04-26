#include "IdComponent.h"

#include "gtest/gtest.h"

#include "ComponentOwner.h"

using namespace ::testing;
using namespace components::core;

class IdComponentTest : public Test
{
public:
    const utils::Vector2f position{0.0, 11.0};
    ComponentOwner componentOwner{position, "idComponentTest"};
};

TEST_F(IdComponentTest, createTwoComponents_shouldHaveDifferentConsecutiveId)
{
    IdComponent idComponent1{&componentOwner};
    IdComponent idComponent2{&componentOwner};

    ASSERT_NE(idComponent1.getId(), idComponent2.getId());
    ASSERT_EQ(idComponent1.getId() + 1u, idComponent2.getId());
}