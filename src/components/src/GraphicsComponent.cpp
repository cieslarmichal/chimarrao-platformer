#include "GraphicsComponent.h"
#include "ComponentOwner.h"
#include <iostream>

namespace components
{

GraphicsComponent::GraphicsComponent(ComponentOwner* owner,
                                     std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                                     const utils::Vector2f& size, const utils::Vector2f& position)
    : Component{owner}, rendererPool{std::move(rendererPoolInit)}
{
    id = rendererPool->acquire(size, position, graphics::Color::Red);
}

void GraphicsComponent::lateUpdate(utils::DeltaTime deltaTime)
{
    std::cerr << owner->transform->getPosition() << std::endl;
    rendererPool->setPosition(id, owner->transform->getPosition());
}

const graphics::GraphicsId& GraphicsComponent::getGraphicsId()
{
    return id;
}
}