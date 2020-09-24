#include "PlayerAnimator.h"

#include "AnimationsFromSettingsLoader.h"
#include "GetProjectPath.h"
#include "exceptions/AnimationTypeNotSupported.h"
#include "exceptions/InvalidAnimatorSettings.h"

namespace animations
{

PlayerAnimator::PlayerAnimator(graphics::GraphicsId graphicsIdInit,
                               std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                               const AnimatorSettings& animatorSettings, AnimationType animationTypeInit,
                               AnimationDirection animationDirectionInit)
    : graphicsId{graphicsIdInit},
      rendererPool{std::move(rendererPoolInit)},
      currentAnimationType{animationTypeInit},
      currentAnimationDirection{animationDirectionInit},
      animatorName{"player"},
      newAnimationTypeIsSet{false},
      newAnimationDirectionIsSet{false}
{
    if (animatorSettings.animatorName != animatorName)
    {
        throw exceptions::InvalidAnimatorSettings{"Invalid settings for " + animatorName + ": " +
                                                  animatorSettings.animatorName};
    }

    initializeAnimations(animatorSettings.animationsSettings);

    if (not containsAnimation(currentAnimationType))
    {
        throw exceptions::AnimationTypeNotSupported{"Animation of type: " + toString(currentAnimationType) +
                                                    " is not supported in " + animatorName};
    }

    rendererPool->setTexture(graphicsId, animations.at(currentAnimationType).getCurrentTexturePath());
}

AnimationChanged PlayerAnimator::update(const utils::DeltaTime& deltaTime)
{
    const auto textureChanged = animations.at(currentAnimationType).update(deltaTime);

    if (animationChanged(textureChanged))
    {
        const utils::Vector2f scale = (currentAnimationDirection == AnimationDirection::Left) ?
                                          utils::Vector2f(-1.0f, 1.0f) :
                                          utils::Vector2f(1.0f, 1.0f);
        rendererPool->setTexture(graphicsId, animations.at(currentAnimationType).getCurrentTexturePath(),
                                 scale);
        newAnimationTypeIsSet = false;
        newAnimationDirectionIsSet = false;
        return true;
    }
    return false;
}

void PlayerAnimator::setAnimation(AnimationType animationType)
{
    setAnimation(animationType, currentAnimationDirection);
}

void PlayerAnimator::setAnimation(AnimationType animationType, AnimationDirection animationDirection)
{
    if (not containsAnimation(animationType))
    {
        throw exceptions::AnimationTypeNotSupported{"Animation of type: " + toString(animationType) +
                                                    " is not supported in " + animatorName};
    }

    if (currentAnimationType != animationType)
    {
        currentAnimationType = animationType;
        animations.at(currentAnimationType).reset();
        newAnimationTypeIsSet = true;
    }

    if (currentAnimationDirection != animationDirection)
    {
        currentAnimationDirection = animationDirection;
        animations.at(currentAnimationType).reset();
        newAnimationDirectionIsSet = true;
    }
}

void PlayerAnimator::setAnimationDirection(AnimationDirection animationDirection)
{
    if (currentAnimationDirection != animationDirection)
    {
        currentAnimationDirection = animationDirection;
        animations.at(currentAnimationType).reset();
        newAnimationDirectionIsSet = true;
    }
}

AnimationType PlayerAnimator::getAnimationType() const
{
    return currentAnimationType;
}

AnimationDirection PlayerAnimator::getAnimationDirection() const
{
    return currentAnimationDirection;
}

void PlayerAnimator::initializeAnimations(const AnimationsSettings& animationsSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromSettings(animations, animationsSettings);
}

bool PlayerAnimator::containsAnimation(const AnimationType& animationType) const
{
    return animations.count(animationType);
}

bool PlayerAnimator::animationChanged(TextureChanged textureChanged) const
{
    return textureChanged || newAnimationTypeIsSet || newAnimationDirectionIsSet;
}

}