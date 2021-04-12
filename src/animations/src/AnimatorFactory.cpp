#include "AnimatorFactory.h"

#include "DefaultAnimatorFactory.h"

namespace animations
{

std::unique_ptr<AnimatorFactory>
AnimatorFactory::createAnimatorFactory(const std::shared_ptr<graphics::RendererPool>& rendererPool)
{
    return std::make_unique<DefaultAnimatorFactory>(rendererPool);
}
}