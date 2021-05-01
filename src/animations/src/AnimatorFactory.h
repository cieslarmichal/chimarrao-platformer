#pragma once

#include <memory>

#include "AnimationsApi.h"
#include "Animator.h"
#include "MultipleFilesAnimatorSettings.h"
#include "RendererPool.h"
#include "SingleFileAnimatorSettings.h"
#include "Window.h"

namespace animations
{
class ANIMATIONS_API AnimatorFactory
{
public:
    virtual ~AnimatorFactory() = default;

    virtual std::unique_ptr<Animator> createBunnyAnimator(graphics::GraphicsId) const = 0;
    virtual std::unique_ptr<Animator> createPlayerAnimator(graphics::GraphicsId) const = 0;
    virtual std::unique_ptr<Animator> createDruidAnimator(graphics::GraphicsId) const = 0;

    static std::unique_ptr<AnimatorFactory>
    createAnimatorFactory(const std::shared_ptr<graphics::RendererPool>&);
};
}
