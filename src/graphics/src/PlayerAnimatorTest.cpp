#include "PlayerAnimator.h"

#include "gtest/gtest.h"
#include "GraphicsIdGenerator.h"
#include "RendererPoolMock.h"

using namespace graphics;
using namespace ::testing;

namespace
{
const auto graphicsId = GraphicsIdGenerator::generateId();
}

class PlayerAnimatorTest : public Test
{
public:
    std::shared_ptr<RendererPoolMock> rendererPool = std::make_shared<StrictMock<RendererPoolMock>>();

    PlayerAnimator animator{graphicsId, rendererPool};
};

//void update(const utils::DeltaTime&) override;
//void setAnimation(AnimationType) override;
//void setAnimation(AnimationType, AnimationDirection) override;

TEST_F(PlayerAnimatorTest, xxx)
{

}

