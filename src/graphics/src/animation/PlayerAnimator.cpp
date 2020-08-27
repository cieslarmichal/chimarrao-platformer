#include "PlayerAnimator.h"

#include "AnimationsFromSettingsLoader.h"
#include "GetProjectPath.h"
#include "exceptions/AnimationTypeNotSupported.h"
#include "exceptions/InvalidAnimatorConfigFile.h"

namespace graphics::animation
{

PlayerAnimator::PlayerAnimator(graphics::GraphicsId graphicsIdInit,
                               std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                               const AnimatorSettings& animatorSettings, AnimationType animationTypeInit,
                               AnimationDirection animationDirectionInit)
    : graphicsId{graphicsIdInit},
      rendererPool{std::move(rendererPoolInit)},
      currentAnimationType{animationTypeInit},
      currentAnimationDirection{animationDirectionInit},
      animatorName{"player"}
{
    if (animatorSettings.animatorName != animatorName)
    {
        throw exceptions::InvalidAnimatorConfigFile{"Invalid settings for " + animatorName + ": " +
                                                    animatorSettings.animatorName};
    }

    initializeAnimations(animatorSettings.animationsSettings);

    if (not containsAnimation(currentAnimationType))
    {
        throw exceptions::AnimationTypeNotSupported{"Animation of type: " + toString(currentAnimationType) +
                                                    " is not supported in " + animatorName};
    }
}

void PlayerAnimator::update(const utils::DeltaTime& deltaTime)
{
    animations.at(currentAnimationType).update(deltaTime);
    // TODO: check if texture is changed from animation
    // TODO: get rid of ternary operation
    rendererPool->setTexture(graphicsId, animations.at(currentAnimationType).getCurrentTexturePath(),
                             {(currentAnimationDirection == AnimationDirection::Left ? -1.5f : 1.5f), 1.5f});
}

void PlayerAnimator::setAnimation(AnimationType animationType)
{
    if (not containsAnimation(animationType))
    {
        throw exceptions::AnimationTypeNotSupported{"Animation of type: " + toString(currentAnimationType) +
                                                    " is not supported in " + animatorName};
    }

    setAnimation(animationType, currentAnimationDirection);
}

void PlayerAnimator::setAnimation(AnimationType animationType, AnimationDirection animationDirection)
{
    if (not containsAnimation(animationType))
    {
        throw exceptions::AnimationTypeNotSupported{"Animation of type: " + toString(currentAnimationType) +
                                                    " is not supported in " + animatorName};
    }

    if (currentAnimationType != animationType)
    {
        currentAnimationType = animationType;
        animations.at(currentAnimationType).reset();
    }

    if (currentAnimationDirection != animationDirection)
    {
        currentAnimationDirection = animationDirection;
        animations.at(currentAnimationType).reset();
    }
}

void PlayerAnimator::initializeAnimations(const AnimationsSettings& animationsSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromSettings(animations, animationsSettings);
}

bool PlayerAnimator::containsAnimation(const AnimationType&) const
{
    return animations.count(currentAnimationType);
}

}