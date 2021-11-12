#pragma once

#include "gmock/gmock.h"

#include "AttackStrategy.h"

namespace components::core
{
class AttackStrategyMock : public AttackStrategy
{
public:
    MOCK_METHOD(void, attack, (), (override));
};
}