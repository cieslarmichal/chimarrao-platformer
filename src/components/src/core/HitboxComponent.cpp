#include "HitboxComponent.h"

#include "ComponentOwner.h"

namespace components::core
{

HitboxComponent::HitboxComponent(ComponentOwner* ownerInit, const utils::Vector2f& sizeInit,
                                 const utils::Vector2f& offsetInit)
    : Component(ownerInit), size{sizeInit}, offset{offsetInit}
{
}

void HitboxComponent::lateUpdate(utils::DeltaTime)
{
    if (not enabled)
    {
        return;
    }
    originPosition = owner->transform->getPosition() + offset;
}

bool HitboxComponent::intersects(const utils::Vector2f& position) const
{
    if (not enabled)
    {
        return false;
    }

    return position.x >= originPosition.x && position.x <= originPosition.x + size.x &&
           position.y >= originPosition.y && position.y <= originPosition.y + size.y;
}

}