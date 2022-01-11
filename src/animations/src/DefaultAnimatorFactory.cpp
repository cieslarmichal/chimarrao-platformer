#include "DefaultAnimatorFactory.h"

#include "AnimatorSettingsYamlReader.h"
#include "DefaultAnimatorSettingsRepository.h"
#include "MultipleFileAnimator.h"
#include "SingleFileAnimator.h"

namespace animations
{
DefaultAnimatorFactory::DefaultAnimatorFactory(std::shared_ptr<graphics::RendererPool> rendererPoolInit)
    : rendererPool{std::move(rendererPoolInit)},
      animatorSettingsRepository{
          std::make_shared<DefaultAnimatorSettingsRepository>(std::make_unique<AnimatorSettingsYamlReader>())}
{
}

std::unique_ptr<Animator> DefaultAnimatorFactory::createBunnyAnimator(graphics::GraphicsId graphicsId) const
{
    return std::make_unique<SingleFileAnimator>(graphicsId, rendererPool, animatorSettingsRepository, "bunny",
                                                AnimationType::Walk);
}

std::unique_ptr<Animator> DefaultAnimatorFactory::createPlayerAnimator(graphics::GraphicsId graphicsId) const
{
    return std::make_unique<MultipleFileAnimator>(graphicsId, rendererPool, animatorSettingsRepository,
                                                  "player", AnimationType::Idle);
}

std::unique_ptr<Animator> DefaultAnimatorFactory::createDruidAnimator(graphics::GraphicsId graphicsId) const
{
    return std::make_unique<MultipleFileAnimator>(graphicsId, rendererPool, animatorSettingsRepository,
                                                  "druid", AnimationType::Walk);
}

std::unique_ptr<Animator> DefaultAnimatorFactory::createCometAnimator(graphics::GraphicsId graphicsId) const
{
    return std::make_unique<MultipleFileAnimator>(graphicsId, rendererPool, animatorSettingsRepository,
                                                  "comet", AnimationType::Fly);
}

std::unique_ptr<Animator> DefaultAnimatorFactory::createBanditAnimator(graphics::GraphicsId graphicsId) const
{
    return std::make_unique<SingleFileAnimator>(graphicsId, rendererPool, animatorSettingsRepository,
                                                "bandit", AnimationType::Idle);
}

std::unique_ptr<Animator>
DefaultAnimatorFactory::createCampfireAnimator(graphics::GraphicsId graphicsId) const
{
    return std::make_unique<SingleFileAnimator>(graphicsId, rendererPool, animatorSettingsRepository,
                                                "campfire", AnimationType::Idle);
}
}