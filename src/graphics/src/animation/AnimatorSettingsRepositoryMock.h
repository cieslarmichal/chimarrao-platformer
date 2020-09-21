#pragma once

#include "gmock/gmock.h"

#include "AnimatorSettingsRepository.h"

namespace graphics::animation
{
class AnimatorSettingsRepositoryMock : public AnimatorSettingsRepository
{
public:
    MOCK_METHOD(boost::optional<AnimatorSettings>, getAnimatorSettings, (const std::string& animatorName), (const override));
};
}
