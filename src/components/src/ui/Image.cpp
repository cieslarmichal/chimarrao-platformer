#include "Image.h"

#include "core/GraphicsComponent.h"
#include "exceptions/UIComponentConfigNotFound.h"

namespace components::ui
{

Image::Image(const std::shared_ptr<graphics::RendererPool>& rendererPool,
             std::unique_ptr<ImageConfig> imageConfig)
{
    if (not imageConfig)
    {
        throw exceptions::UIComponentConfigNotFound{"Image config not found"};
    }

    name = imageConfig->uniqueName;
    coreComponentsOwner = std::make_unique<components::core::ComponentOwner>(imageConfig->position, name);
    coreComponentsOwner->addComponent<components::core::GraphicsComponent>(
        rendererPool, imageConfig->size, imageConfig->position, imageConfig->texturePath,
        imageConfig->visibilityLayer, true);

    coreComponentsOwner->loadDependentComponents();
}

void Image::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    coreComponentsOwner->update(deltaTime, input);
    coreComponentsOwner->lateUpdate(deltaTime, input);
}

std::string Image::getName() const
{
    return name;
}

void Image::activate()
{
    coreComponentsOwner->enable();
}

void Image::deactivate()
{
    coreComponentsOwner->disable();
}

bool Image::isActive() const
{
    return coreComponentsOwner->areComponentEnabled();
}
}