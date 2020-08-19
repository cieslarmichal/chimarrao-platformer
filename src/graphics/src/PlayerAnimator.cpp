#include "PlayerAnimator.h"

#include "GetProjectPath.h"

namespace graphics
{
namespace
{
const std::string playerResourcesPath = utils::getProjectPath("chimarrao") + "/resources/Player/";
const std::string idleResourcesPath = playerResourcesPath + "Idle/";
const std::string walkResourcesPath = playerResourcesPath + "Walk/";
std::vector<TexturePath> idleTexturePaths{
    idleResourcesPath + "idle-with-weapon-1.png", idleResourcesPath + "idle-with-weapon-2.png",
    idleResourcesPath + "idle-with-weapon-3.png", idleResourcesPath + "idle-with-weapon-4.png",
    idleResourcesPath + "idle-with-weapon-5.png", idleResourcesPath + "idle-with-weapon-6.png"};
std::vector<TexturePath> walkTexturePaths{
    walkResourcesPath + "walk-with-weapon-1.png", walkResourcesPath + "walk-with-weapon-2.png",
    walkResourcesPath + "walk-with-weapon-3.png", walkResourcesPath + "walk-with-weapon-4.png",
    walkResourcesPath + "walk-with-weapon-5.png", walkResourcesPath + "walk-with-weapon-6.png",
    walkResourcesPath + "walk-with-weapon-7.png", walkResourcesPath + "walk-with-weapon-8.png",
    walkResourcesPath + "walk-with-weapon-9.png", walkResourcesPath + "walk-with-weapon-10.png",
    walkResourcesPath + "walk-with-weapon-11.png"};
}

PlayerAnimator::PlayerAnimator(graphics::GraphicsId graphicsIdInit,
                               std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                               AnimationType animationTypeInit, AnimationDirection animationDirectionInit)
    : graphicsId{graphicsIdInit},
      rendererPool{std::move(rendererPoolInit)},
      currentAnimationType{animationTypeInit},
      currentAnimationDirection{animationDirectionInit}
{
    animations.reserve(3);
    animations.insert({AnimationType::Idle, Animation{idleTexturePaths, 0.3f}});
    animations.insert({AnimationType::Walk, Animation{walkTexturePaths, 0.1f}});
}

void PlayerAnimator::update(const utils::DeltaTime& deltaTime)
{
    // TODO: check if animationType exists
    animations.at(currentAnimationType).update(deltaTime);
    // TODO: check if texture is changed from animation
    // TODO: get rid of ternary operation
    rendererPool->setTexture(graphicsId, animations.at(currentAnimationType).getCurrentTexturePath(),
                             {(currentAnimationDirection == AnimationDirection::Left ? -1.5f : 1.5f), 1.5f});
}

void PlayerAnimator::setAnimation(AnimationType animationType)
{
    setAnimation(animationType, currentAnimationDirection);
}

void PlayerAnimator::setAnimation(AnimationType animationType, AnimationDirection animationDirection)
{
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
}