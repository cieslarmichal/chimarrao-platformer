#pragma once

#include "DeltaTime.h"
#include "Input.h"
#include "StateType.h"
#include "StatesStatus.h"

namespace game
{
class States
{
public:
    virtual ~States() = default;

    virtual StatesStatus updateCurrentState(const utils::DeltaTime&, const input::Input&) = 0;
    virtual void deactivateCurrentState() = 0;
    virtual void addNextState(StateType) = 0;
    virtual void render() = 0;
};
}