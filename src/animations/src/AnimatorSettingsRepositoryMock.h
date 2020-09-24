#pragma once

#include "gmock/gmock.h"

#include "AnimatorSettingsRepository.h"

namespace animations
{
class AnimatorSettingsRepositoryMock : public AnimatorSettingsRepository
{
public:
    MOCK_METHOD(boost::optional<AnimatorSettings>, getAnimatorSettings, (const std::string& animatorName),
                (const override));
};
}
