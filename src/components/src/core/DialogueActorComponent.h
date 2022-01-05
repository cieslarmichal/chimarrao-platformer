#pragma once

#include "Component.h"
#include "DialogueActor.h"

namespace components::core
{
class DialogueActorComponent : public Component
{
public:
    DialogueActorComponent(ComponentOwner*, DialogueActor);

    DialogueActor getActorType() const;

private:
    const DialogueActor actor;
};
}