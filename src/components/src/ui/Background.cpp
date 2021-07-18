#include "Background.h"

#include "core/BoxColliderComponent.h"
#include "core/ClickableComponent.h"
#include "core/GraphicsComponent.h"
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

    if ((not backgroundConfig->texturePath or backgroundConfig->texturePath->empty()) and
        not backgroundConfig->color)
    {
        throw exceptions::InvalidUIComponentConfig{"No background filling found"};
    }

    name = backgroundConfig->uniqueName;
    coreComponentsOwner =
        std::make_unique<components::core::ComponentOwner>(backgroundConfig->position, name);

    if (backgroundConfig->texturePath)
    {
        coreComponentsOwner->addGraphicsComponent(
            rendererPool, backgroundConfig->size, backgroundConfig->position, *backgroundConfig->texturePath,
            backgroundConfig->visibilityLayer, utils::Vector2f{0, 0}, true);
    }
    else
    {
        coreComponentsOwner->addGraphicsComponent(
            rendererPool, backgroundConfig->size, backgroundConfig->position, *backgroundConfig->color,
            backgroundConfig->visibilityLayer, utils::Vector2f{0, 0}, true);
    }

    if (not backgroundConfig->keyActions.empty())
    {
        coreComponentsOwner->addComponent<components::core::BoxColliderComponent>(backgroundConfig->size);
        coreComponentsOwner->addComponent<components::core::ClickableComponent>(backgroundConfig->keyActions);
    }

    coreComponentsOwner->loadDependentComponents();
}

void Background::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    coreComponentsOwner->update(deltaTime, input);
    coreComponentsOwner->lateUpdate(deltaTime, input);
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
    coreComponentsOwner->getMainGraphicsComponent()->setColor(color);
}

bool Background::isActive() const
{
    return coreComponentsOwner->areComponentsEnabled();
}

}