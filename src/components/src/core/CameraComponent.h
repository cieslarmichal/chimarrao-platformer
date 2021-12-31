#pragma once

#include "Component.h"
#include "Rect.h"
#include "RendererPool.h"

namespace components::core
{
class CameraComponent : public Component
{
public:
    CameraComponent(ComponentOwner* owner, std::shared_ptr<graphics::RendererPool> rendererPool,
                    utils::FloatRect mapRect, bool blockCameraOnRightSide = true);

    void lateUpdate(utils::DeltaTime time, const input::Input& input) override;

private:
    std::shared_ptr<graphics::RendererPool> rendererPool;
    utils::FloatRect mapRect;
    const bool blockCameraOnRightSide;
};
}