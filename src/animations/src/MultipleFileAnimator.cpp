#include "MultipleFileAnimator.h"

#include <utility>

#include "AnimationsFromSettingsLoader.h"
#include "exceptions/AnimationTypeNotSupported.h"
#include "exceptions/AnimatorSettingsNotFound.h"
#include "exceptions/InvalidAnimatorSettings.h"

namespace animations
{

MultipleFileAnimator::MultipleFileAnimator(
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
    auto animatorSettings = animatorSettingsRepositoryInit->getMultipleFileAnimatorSettings(animatorName);

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

AnimationChanged MultipleFileAnimator::update(const utils::DeltaTime& deltaTime)
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

void MultipleFileAnimator::setAnimation(AnimationType animationType)
{
    setAnimation(animationType, currentAnimationDirection);
}

void MultipleFileAnimator::setAnimation(AnimationType animationType, AnimationDirection animationDirection)
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

void MultipleFileAnimator::setAnimationDirection(AnimationDirection animationDirection)
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

void MultipleFileAnimator::forceAnimation(AnimationType animationType)
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

AnimationType MultipleFileAnimator::getAnimationType() const
{
    return currentAnimationType;
}

AnimationDirection MultipleFileAnimator::getAnimationDirection() const
{
    return currentAnimationDirection;
}

void MultipleFileAnimator::initializeAnimations(
    const std::vector<MultipleFilesAnimationSettings>& animationsSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromMultipleFilesAnimationsSettings(animations,
                                                                                    animationsSettings);
}

bool MultipleFileAnimator::containsAnimation(const AnimationType& animationType) const
{
    return animations.contains(animationType);
}

bool MultipleFileAnimator::animationChanged(TextureRectChanged textureChanged) const
{
    return textureChanged || newAnimationTypeIsSet || newAnimationDirectionIsSet;
}

unsigned int MultipleFileAnimator::getCurrentAnimationProgressInPercents()
{
    return animations.at(currentAnimationType).getProgressInPercents();
}
}