#include "BunnyAnimator.h"

#include "AnimationsFromSettingsLoader.h"
#include "GetProjectPath.h"
#include "exceptions/AnimationTypeNotSupported.h"
#include "exceptions/InvalidAnimatorSettings.h"

namespace animations
{

BunnyAnimator::BunnyAnimator(graphics::GraphicsId graphicsIdInit,
                               std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                               const SingleFileAnimatorSettings& animatorSettings, AnimationType animationTypeInit,
                               AnimationDirection animationDirectionInit)
    : graphicsId{graphicsIdInit},
      rendererPool{std::move(rendererPoolInit)},
      currentAnimationType{animationTypeInit},
      currentAnimationDirection{animationDirectionInit},
      animatorName{"bunny"},
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

    rendererPool->setTexture(graphicsId, animations.at(currentAnimationType).getCurrentTextureRect());
}

AnimationChanged BunnyAnimator::update(const utils::DeltaTime& deltaTime)
{
    const auto textureChanged = animations.at(currentAnimationType).update(deltaTime);

    if (animationChanged(textureChanged))
    {
        const utils::Vector2f scale = (currentAnimationDirection == AnimationDirection::Left) ?
                                      utils::Vector2f(-1.0f, 1.0f) :
                                      utils::Vector2f(1.0f, 1.0f);
        rendererPool->setTexture(graphicsId, animations.at(currentAnimationType).getCurrentTextureRect(),
                                 scale);
        newAnimationTypeIsSet = false;
        newAnimationDirectionIsSet = false;
        return true;
    }
    return false;
}

void BunnyAnimator::setAnimation(AnimationType animationType)
{
    setAnimation(animationType, currentAnimationDirection);
}

void BunnyAnimator::setAnimation(AnimationType animationType, AnimationDirection animationDirection)
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

void BunnyAnimator::setAnimationDirection(AnimationDirection animationDirection)
{
    if (currentAnimationDirection != animationDirection)
    {
        currentAnimationDirection = animationDirection;
        animations.at(currentAnimationType).reset();
        newAnimationDirectionIsSet = true;
    }
}

AnimationType BunnyAnimator::getAnimationType() const
{
    return currentAnimationType;
}

AnimationDirection BunnyAnimator::getAnimationDirection() const
{
    return currentAnimationDirection;
}

void BunnyAnimator::initializeAnimations(const std::vector<SingleFileAnimationSettings>& animationsSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromSingleFileAnimationsSettings(animations, animationsSettings);
}

bool BunnyAnimator::containsAnimation(const AnimationType& animationType) const
{
    return animations.count(animationType);
}

bool BunnyAnimator::animationChanged(TextureRectChanged textureChanged) const
{
    return textureChanged || newAnimationTypeIsSet || newAnimationDirectionIsSet;
}

}