#include "Image.h"

#include "core/GraphicsComponent.h"
#include "exceptions/InvalidUIComponentConfig.h"
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

    if ((not imageConfig->texturePath or imageConfig->texturePath->empty()) and not imageConfig->color)
    {
        throw exceptions::InvalidUIComponentConfig{"No background filling found"};
    }

    name = imageConfig->uniqueName;
    coreComponentsOwner = std::make_unique<components::core::ComponentOwner>(imageConfig->position, name);

    if (imageConfig->texturePath)
    {
        coreComponentsOwner->addGraphicsComponent(rendererPool, imageConfig->size, imageConfig->position,
                                                  *imageConfig->texturePath, imageConfig->visibilityLayer,
                                                  utils::Vector2f{0, 0}, true);
    }
    else
    {
        coreComponentsOwner->addGraphicsComponent(rendererPool, imageConfig->size, imageConfig->position,
                                                  *imageConfig->color, imageConfig->visibilityLayer,
                                                  utils::Vector2f{0, 0}, true);
    }

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
    return coreComponentsOwner->areComponentsEnabled();
}

utils::Vector2f Image::getSize() const
{
    return coreComponentsOwner->getMainGraphicsComponent()->getSize();
}

void Image::setSize(const utils::Vector2f& size)
{
    return coreComponentsOwner->getMainGraphicsComponent()->setSize(size);
}

void Image::setOutline(float thickness, const sf::Color& color)
{
    coreComponentsOwner->getMainGraphicsComponent()->setOutline(thickness, color);
}
}