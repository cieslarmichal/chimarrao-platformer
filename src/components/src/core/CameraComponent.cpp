#include "CameraComponent.h"

#include <utility>

#include "ComponentOwner.h"

components::core::CameraComponent::CameraComponent(components::core::ComponentOwner* owner,
                                                   std::shared_ptr<graphics::RendererPool> rendererPool,
                                                   utils::FloatRect mapRect)
    : Component(owner), rendererPool{std::move(rendererPool)}, mapRect{mapRect}
{
}

void components::core::CameraComponent::lateUpdate(utils::DeltaTime, const input::Input&)
{
    auto playerPosition = owner->transform->getPosition();
    if (playerPosition.y < mapRect.top + rendererPool->getViewSize().y / 2)
    {
        playerPosition.y = mapRect.top + rendererPool->getViewSize().y / 2;
    }
    else if (playerPosition.y > mapRect.top + mapRect.height - rendererPool->getViewSize().y / 2)
    {
        playerPosition.y = mapRect.top + mapRect.height - rendererPool->getViewSize().y / 2;
    }

    if (playerPosition.x < mapRect.left + rendererPool->getViewSize().x / 2)
    {
        playerPosition.x = mapRect.left + rendererPool->getViewSize().x / 2;
    }
    else if (playerPosition.x > mapRect.left + mapRect.width - rendererPool->getViewSize().x / 2)
    {
        playerPosition.x = mapRect.left + mapRect.width - rendererPool->getViewSize().x / 2;
    }

    rendererPool->setCenter(playerPosition);
}
