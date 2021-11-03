#pragma once

#include "gmock/gmock.h"

#include "ItemEffect.h"

namespace components::core
{
class ItemEffectMock : public ItemEffect
{
public:
    MOCK_METHOD(void, affect, (ComponentOwner*), (const override));
};
}