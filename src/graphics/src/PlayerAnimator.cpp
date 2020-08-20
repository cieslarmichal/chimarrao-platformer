#include "PlayerAnimator.h"

#include "GetProjectPath.h"
#include "IncrementalFilePathsCreator.h"

namespace graphics
{
namespace
{
const std::string playerResourcesPath = utils::getProjectPath("chimarrao") + "/resources/Player/";
const std::string idleResourcesPath = playerResourcesPath + "Idle/";
const std::string walkResourcesPath = playerResourcesPath + "Walk/";
const TexturePath idleTexturePath = idleResourcesPath + "idle-with-weapon-1.png";
const TexturePath walkTexturePath = walkResourcesPath + "walk-with-weapon-1.png";
const int numberOfIdleTextures = 6;
const int numberOfWalkTextures = 11;
}

PlayerAnimator::PlayerAnimator(graphics::GraphicsId graphicsIdInit,
                               std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                               AnimationType animationTypeInit, AnimationDirection animationDirectionInit)
    : graphicsId{graphicsIdInit},
      rendererPool{std::move(rendererPoolInit)},
      currentAnimationType{animationTypeInit},
      currentAnimationDirection{animationDirectionInit}
{
    initializeAnimations();
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

void PlayerAnimator::initializeAnimations()
{
    animations.reserve(3);

    const auto idleTexturesPaths =
        utils::IncrementalFilePathsCreator::createFilePaths(idleTexturePath, numberOfIdleTextures);
    const auto walkTexturesPaths =
        utils::IncrementalFilePathsCreator::createFilePaths(walkTexturePath, numberOfWalkTextures);

    animations.insert({AnimationType::Idle, Animation{idleTexturesPaths, 0.3f}});
    animations.insert({AnimationType::Walk, Animation{walkTexturesPaths, 0.1f}});
}

}