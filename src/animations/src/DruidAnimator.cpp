#include "DruidAnimator.h"

#include "AnimationsFromSettingsLoader.h"
#include "exceptions/AnimationTypeNotSupported.h"
#include "exceptions/AnimatorSettingsNotFound.h"
#include "exceptions/InvalidAnimatorSettings.h"

namespace animations
{

DruidAnimator::DruidAnimator(graphics::GraphicsId graphicsIdInit, std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                             const std::shared_ptr<AnimatorSettingsRepository>& animatorSettingsRepositoryInit, AnimationType animationTypeInit,
                             AnimationDirection animationDirectionInit)
    : graphicsId{graphicsIdInit},
      rendererPool{std::move(rendererPoolInit)},
      currentAnimationType{animationTypeInit},
      currentAnimationDirection{animationDirectionInit},
      animatorName{"druid"},
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

AnimationChanged DruidAnimator::update(const utils::DeltaTime& deltaTime)
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

void DruidAnimator::setAnimation(AnimationType animationType)
{
    setAnimation(animationType, currentAnimationDirection);
}

void DruidAnimator::setAnimation(AnimationType animationType, AnimationDirection animationDirection)
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

void DruidAnimator::setAnimationDirection(AnimationDirection animationDirection)
{
    if (currentAnimationDirection != animationDirection)
    {
        currentAnimationDirection = animationDirection;
        animations.at(currentAnimationType).reset();
        newAnimationDirectionIsSet = true;
    }
}

AnimationType DruidAnimator::getAnimationType() const
{
    return currentAnimationType;
}

AnimationDirection DruidAnimator::getAnimationDirection() const
{
    return currentAnimationDirection;
}

void DruidAnimator::initializeAnimations(const std::vector<MultipleFilesAnimationSettings>& animationsSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromMultipleFilesAnimationsSettings(animations,
                                                                                    animationsSettings);
}

bool DruidAnimator::containsAnimation(const AnimationType& animationType) const
{
    return animations.count(animationType);
}

bool DruidAnimator::animationChanged(TextureRectChanged textureChanged) const
{
    return textureChanged || newAnimationTypeIsSet || newAnimationDirectionIsSet;
}
}