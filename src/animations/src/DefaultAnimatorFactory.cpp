#include "DefaultAnimatorFactory.h"

#include "AnimatorSettingsYamlReader.h"
#include "BunnyAnimator.h"
#include "DefaultAnimatorSettingsRepository.h"
#include "PlayerAnimator.h"

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
    return std::make_unique<BunnyAnimator>(graphicsId, rendererPool, animatorSettingsRepository);
}
std::unique_ptr<Animator> DefaultAnimatorFactory::createPlayerAnimator(graphics::GraphicsId graphicsId) const
{
    return std::make_unique<PlayerAnimator>(graphicsId, rendererPool, animatorSettingsRepository);
}
}