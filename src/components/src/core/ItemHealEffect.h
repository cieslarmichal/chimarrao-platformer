#pragma once

#include "ItemEffect.h"

namespace components::core
{
class ItemHealEffect : public ItemEffect
{
public:
    ItemHealEffect(unsigned healthPoints);

    void affect(ComponentOwner* target) const override;

private:
    const unsigned healthPoints;
};
}