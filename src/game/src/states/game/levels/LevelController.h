#pragma once

#include "DeltaTime.h"
#include "Input.h"

namespace game
{
using SwitchToNextLevel = bool;

class LevelController
{
public:
    virtual ~LevelController() = default;

    virtual SwitchToNextLevel update(const utils::DeltaTime& deltaTime, const input::Input& input) = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
};
}