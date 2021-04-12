#pragma once

#include "AnimatorFactory.h"
#include "AnimatorSettingsRepository.h"

namespace animations
{
class DefaultAnimatorFactory : public AnimatorFactory
{
public:
    DefaultAnimatorFactory(std::shared_ptr<graphics::RendererPool>);

    std::unique_ptr<Animator> createBunnyAnimator(graphics::GraphicsId) const override;
    std::unique_ptr<Animator> createPlayerAnimator(graphics::GraphicsId) const override;

private:
    std::shared_ptr<graphics::RendererPool> rendererPool;
    std::shared_ptr<AnimatorSettingsRepository> animatorSettingsRepository;
};
}
