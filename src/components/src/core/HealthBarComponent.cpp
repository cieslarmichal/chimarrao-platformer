#include "HealthBarComponent.h"

#include "ComponentOwner.h"
#include "GraphicsComponent.h"
#include "HealthComponent.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

HealthBarComponent::HealthBarComponent(ComponentOwner* owner,
                                       const std::shared_ptr<graphics::RendererPool>& rendererPool,
                                       const utils::Vector2f& offset)
    : Component{owner}
{
    bar =
        owner->addGraphicsComponent(rendererPool, utils::Vector2f{3.f, 0.5f}, owner->transform->getPosition(),
                                    graphics::Color::Red, graphics::VisibilityLayer::First, offset);
}

void HealthBarComponent::loadDependentComponents()
{
    health = owner->getComponent<HealthComponent>();
    if (not health)
    {
        throw exceptions::DependentComponentNotFound{"HealthBarComponent: Health component not found"};
    }
}

void HealthBarComponent::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    bar->update(deltaTime, input);
}

void HealthBarComponent::lateUpdate(utils::DeltaTime deltaTime, const input::Input& input)
{
    const auto currentBarSize = bar->getSize();
    const auto currentHealth = health->getCurrentHealth();
    const auto maximumHealth = health->getMaximumHealth();
    const auto barSizeScaledByCurrentHealth = utils::Vector2f{
        currentBarSize.x * (static_cast<float>(currentHealth) / static_cast<float>(maximumHealth)),
        currentBarSize.y};
    if (barSizeScaledByCurrentHealth != currentBarSize)
    {
        bar->setSize(barSizeScaledByCurrentHealth);
    }
    bar->lateUpdate(deltaTime, input);
}
}