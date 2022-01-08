#include "SingleFileAnimator.h"

#include <utility>

#include "AnimationsFromSettingsLoader.h"
#include "exceptions/AnimationTypeNotSupported.h"
#include "exceptions/AnimatorSettingsNotFound.h"
#include "exceptions/InvalidAnimatorSettings.h"

namespace animations
{

SingleFileAnimator::SingleFileAnimator(
    graphics::GraphicsId graphicsIdInit, std::shared_ptr<graphics::RendererPool> rendererPoolInit,
    const std::shared_ptr<AnimatorSettingsRepository>& animatorSettingsRepositoryInit,
    std::string animatorNameInit, AnimationType animationTypeInit, AnimationDirection animationDirectionInit)
    : graphicsId{graphicsIdInit},
      rendererPool{std::move(rendererPoolInit)},
      currentAnimationType{animationTypeInit},
      currentAnimationDirection{animationDirectionInit},
      animatorName{std::move(animatorNameInit)},
      newAnimationTypeIsSet{false},
      newAnimationDirectionIsSet{false}
{
    auto animatorSettings = animatorSettingsRepositoryInit->getSingleFileAnimatorSettings(animatorName);

    if (not animatorSettings)
    {
        throw exceptions::AnimatorSettingsNotFound{"Animator settings for " + animatorName + " not found"};
    }

    if (animatorSettings->animatorName != animatorName)
    {
        throw exceptions::InvalidAnimatorSettings{"Invalid settings for " + animatorName + ": " +
                                                  animatorSettings->animatorName};
    }

    initializeAnimations(animatorSettings->animationsSettings);

    if (not containsAnimation(currentAnimationType))
    {
        throw exceptions::AnimationTypeNotSupported{"Animation of type: " + toString(currentAnimationType) +
                                                    " is not supported in " + animatorName};
    }

    rendererPool->setTexture(graphicsId, animations.at(currentAnimationType).getCurrentTextureRect());
}

AnimationChanged SingleFileAnimator::update(const utils::DeltaTime& deltaTime)
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

void SingleFileAnimator::setAnimation(AnimationType animationType)
{
    setAnimation(animationType, currentAnimationDirection);
}

void SingleFileAnimator::setAnimation(AnimationType animationType, AnimationDirection animationDirection)
{
    if (not containsAnimation(animationType))
    {
        throw exceptions::AnimationTypeNotSupported{"Animation of type: " + toString(animationType) +
                                                    " is not supported in " + animatorName};
    }

    auto& currentAnimation = animations.at(currentAnimationType);
    if (currentAnimation.hasAnimationFinished() || currentAnimation.areInterruptionsAllowed())
    {
        if (currentAnimationType != animationType)
        {
            currentAnimationType = animationType;
            animations.at(currentAnimationType).reset();
            newAnimationTypeIsSet = true;
        }

        setAnimationDirection(animationDirection);
    }
}

void SingleFileAnimator::setAnimationDirection(AnimationDirection animationDirection)
{
    auto& currentAnimation = animations.at(currentAnimationType);
    if (currentAnimation.hasAnimationFinished() || currentAnimation.areInterruptionsAllowed())
    {
        if (currentAnimationDirection != animationDirection)
        {
            currentAnimationDirection = animationDirection;
            newAnimationDirectionIsSet = true;
        }
    }
}

void SingleFileAnimator::forceAnimation(AnimationType animationType)
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

    setAnimationDirection(currentAnimationDirection);
}

AnimationType SingleFileAnimator::getAnimationType() const
{
    return currentAnimationType;
}

AnimationDirection SingleFileAnimator::getAnimationDirection() const
{
    return currentAnimationDirection;
}

void SingleFileAnimator::initializeAnimations(
    const std::vector<SingleFileAnimationSettings>& animationsSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromSingleFileAnimationsSettings(animations,
                                                                                 animationsSettings);
}

bool SingleFileAnimator::containsAnimation(const AnimationType& animationType) const
{
    return animations.count(animationType);
}

bool SingleFileAnimator::animationChanged(TextureRectChanged textureChanged) const
{
    return textureChanged || newAnimationTypeIsSet || newAnimationDirectionIsSet;
}

unsigned int SingleFileAnimator::getCurrentAnimationProgressInPercents()
{
    return animations.at(currentAnimationType).getProgressInPercents();
}

}