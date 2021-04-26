#include "IdComponent.h"

namespace components::core
{
unsigned int IdComponent::count = 0;

IdComponent::IdComponent(ComponentOwner* owner) : Component{owner}, id{++count} {}

unsigned int IdComponent::getId() const
{
    return id;
}
}