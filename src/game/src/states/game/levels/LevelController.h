#pragma once

namespace game
{
using SwitchToNextLevel = bool;

class LevelController
{
public:
    virtual ~LevelController() = default;

    virtual SwitchToNextLevel update() = 0;
};
}