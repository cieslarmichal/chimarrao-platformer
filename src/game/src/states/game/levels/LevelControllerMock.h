#pragma once

#include "gmock/gmock.h"

#include "LevelController.h"

namespace game
{
class LevelControllerMock : public LevelController
{
public:
    MOCK_METHOD(SwitchToNextLevel, update, (const utils::DeltaTime& deltaTime, const input::Input& input), (override));
    MOCK_METHOD(void, activate, (), (override));
    MOCK_METHOD(void, deactivate, (), (override));
};
}