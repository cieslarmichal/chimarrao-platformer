#pragma once

#include <memory>
#include <unordered_map>

#include "Animation.h"
#include "AnimationDirection.h"
#include "AnimationType.h"
#include "Animator.h"
#include "AnimatorSettingsRepository.h"
#include "GraphicsId.h"
#include "RendererPool.h"
#include "SingleFileAnimatorSettings.h"

namespace animations
{
class SingleFileAnimator : public Animator
{
public:
    SingleFileAnimator(graphics::GraphicsId, std::shared_ptr<graphics::RendererPool>,
                       const std::shared_ptr<AnimatorSettingsRepository>&, std::string animatorName,
                       AnimationType, AnimationDirection = AnimationDirection::Right);

    AnimationChanged update(const utils::DeltaTime&) override;
    void setAnimation(AnimationType) override;
    void setAnimation(AnimationType, AnimationDirection) override;
    void setAnimationDirection(AnimationDirection) override;
    void forceAnimation(AnimationType) override;
    AnimationType getAnimationType() const override;
    AnimationDirection getAnimationDirection() const override;
    unsigned int getCurrentAnimationProgressInPercents() override;

private:
    void initializeAnimations(const std::vector<SingleFileAnimationSettings>&);
    bool containsAnimation(const AnimationType&) const;
    bool animationChanged(TextureRectChanged) const;

    graphics::GraphicsId graphicsId;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    std::unordered_map<AnimationType, Animation> animations;
    AnimationType currentAnimationType;
    AnimationDirection currentAnimationDirection;
    std::string animatorName;
    bool newAnimationTypeIsSet;
    bool newAnimationDirectionIsSet;
};
}