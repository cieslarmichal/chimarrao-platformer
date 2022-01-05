#include "DialogueActorComponent.h"

namespace components::core
{

DialogueActorComponent::DialogueActorComponent(ComponentOwner* owner, DialogueActor dialogueActor)
    : Component(owner), actor{dialogueActor}
{
}

DialogueActor DialogueActorComponent::getActorType() const
{
    return actor;
}
}