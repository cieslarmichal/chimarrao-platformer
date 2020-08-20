#pragma once

#include <memory>
#include <unordered_map>

#include "Animation.h"
#include "AnimationDirection.h"
#include "AnimationType.h"
#include "Animator.h"
#include "GraphicsId.h"
#include "RendererPool.h"

namespace graphics
{
class PlayerAnimator : public Animator
{
public:
    PlayerAnimator(graphics::GraphicsId, std::shared_ptr<graphics::RendererPool>,
                   AnimationType = AnimationType::Idle, AnimationDirection = AnimationDirection::Right);

    void update(const utils::DeltaTime&) override;
    void setAnimation(AnimationType) override;
    void setAnimation(AnimationType, AnimationDirection) override;

private:
    void initializeAnimations();

    graphics::GraphicsId graphicsId;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    std::unordered_map<AnimationType, Animation> animations;
    AnimationType currentAnimationType;
    AnimationDirection currentAnimationDirection;
};
}