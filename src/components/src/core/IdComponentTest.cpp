#include "IdComponent.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "ComponentOwner.h"

using namespace ::testing;
using namespace components::core;

class IdComponentTest : public Test
{
public:
    const utils::Vector2f position{0.0, 11.0};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position, "idComponentTest", sharedContext};
};

TEST_F(IdComponentTest, createTwoComponents_shouldHaveDifferentConsecutiveId)
{
    IdComponent idComponent1{&componentOwner};
    IdComponent idComponent2{&componentOwner};

    ASSERT_NE(idComponent1.getId(), idComponent2.getId());
    ASSERT_EQ(idComponent1.getId() + 1u, idComponent2.getId());
}