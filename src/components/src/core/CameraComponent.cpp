#include "CameraComponent.h"

#include <utility>

#include "ComponentOwner.h"

components::core::CameraComponent::CameraComponent(components::core::ComponentOwner* owner,
                                                   std::shared_ptr<graphics::RendererPool> rendererPool,
                                                   utils::FloatRect mapRect, bool blockCameraOnRightSide)
    : Component(owner),
      rendererPool{std::move(rendererPool)},
      mapRect{mapRect},
      blockCameraOnRightSide{blockCameraOnRightSide}
{
}

void components::core::CameraComponent::lateUpdate(utils::DeltaTime, const input::Input&)
{
    auto cameraViewCenter = owner->transform->getPosition();
    auto cameraViewSize = rendererPool->getViewSize();

    if (cameraViewCenter.y < mapRect.top + cameraViewSize.y / 2)
    {
        cameraViewCenter.y = mapRect.top + cameraViewSize.y / 2;
    }
    else if (cameraViewCenter.y > mapRect.top + mapRect.height - cameraViewSize.y / 2)
    {
        cameraViewCenter.y = mapRect.top + mapRect.height - cameraViewSize.y / 2;
    }

    if (cameraViewCenter.x < mapRect.left + cameraViewSize.x / 2)
    {
        cameraViewCenter.x = mapRect.left + cameraViewSize.x / 2;
    }

    if (blockCameraOnRightSide and cameraViewCenter.x > mapRect.left + mapRect.width - cameraViewSize.x / 2)
    {
        cameraViewCenter.x = mapRect.left + mapRect.width - cameraViewSize.x / 2;
    }

    rendererPool->setCenter(cameraViewCenter);
}
