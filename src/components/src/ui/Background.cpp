#include "Background.h"

#include "core/GraphicsComponent.h"
#include "core/HitBoxComponent.h"
#include "exceptions/InvalidUIComponentConfig.h"
#include "exceptions/UIComponentConfigNotFound.h"

namespace components::ui
{
Background::Background(const std::shared_ptr<graphics::RendererPool>& rendererPool,
                       std::unique_ptr<BackgroundConfig> backgroundConfig)
{
    if (not backgroundConfig)
    {
        throw exceptions::UIComponentConfigNotFound{"Background config not found"};
    }

    if (not backgroundConfig->texturePath && not backgroundConfig->color)
    {
        throw exceptions::InvalidUIComponentConfig{"No background filling found"};
    }

    name = backgroundConfig->uniqueName->getName();

    coreComponentsOwner = std::make_unique<components::core::ComponentOwner>(backgroundConfig->position);

    if (backgroundConfig->texturePath)
    {
        coreComponentsOwner->addComponent<components::core::GraphicsComponent>(
            rendererPool, backgroundConfig->size, backgroundConfig->position, *backgroundConfig->texturePath,
            graphics::VisibilityLayer::Background);
    }
    else
    {
        coreComponentsOwner->addComponent<components::core::GraphicsComponent>(
            rendererPool, backgroundConfig->size, backgroundConfig->position, *backgroundConfig->color,
            graphics::VisibilityLayer::Background);
    }

    coreComponentsOwner->loadDependentComponents();
}

void Background::update(utils::DeltaTime)
{
    throw 4;
}

std::string Background::getName() const
{
    return name;
}

void Background::activate()
{
    coreComponentsOwner->enable();
}

void Background::deactivate()
{
    coreComponentsOwner->disable();
}

void Background::setColor(graphics::Color color)
{
    coreComponentsOwner->getComponent<components::core::GraphicsComponent>()->setColor(color);
}
}