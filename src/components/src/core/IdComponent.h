#pragma once

#include "Component.h"

namespace components::core
{
class IdComponent : public Component
{
public:
    IdComponent(ComponentOwner* owner);

    IdComponent(const IdComponent&) = delete;
    IdComponent& operator=(const IdComponent&) = delete;

    unsigned int getId() const;

private:
    static unsigned int count;
    unsigned int id;
};
}
