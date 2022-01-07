#include "DialogueActorComponent.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "ComponentOwner.h"

using namespace components::core;
using namespace ::testing;

class DialogueActorComponentTest : public Test
{
public:
    const utils::Vector2f position1{8, 0};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    components::core::ComponentOwner owner{position1, "DialogueActorComponentTest", sharedContext};
};

TEST_F(DialogueActorComponentTest, shouldReturnSameActorTypeAsInConstructor1)
{
    DialogueActorComponent actorComponent{&owner, DialogueActor::Player};

    ASSERT_EQ(actorComponent.getActorType(), DialogueActor::Player);
}

TEST_F(DialogueActorComponentTest, shouldReturnSameActorTypeAsInConstructor2)
{
    DialogueActorComponent actorComponent{&owner, DialogueActor::Rabbit};

    ASSERT_EQ(actorComponent.getActorType(), DialogueActor::Rabbit);
}
