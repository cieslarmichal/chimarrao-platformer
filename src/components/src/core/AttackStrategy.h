#pragma once

namespace components::core
{
class AttackStrategy
{
public:
    virtual ~AttackStrategy() = default;

    virtual void attack() = 0;
};
}