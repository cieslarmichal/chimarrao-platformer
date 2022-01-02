#pragma once

#include "gmock/gmock.h"

#include "LevelController.h"

namespace game
{
class LevelControllerMock : public LevelController
{
public:
    MOCK_METHOD(SwitchToNextLevel, update, (), (override));
};
}