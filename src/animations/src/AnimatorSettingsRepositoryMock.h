#pragma once

#include "gmock/gmock.h"

#include "AnimatorSettingsRepository.h"

namespace animations
{
class AnimatorSettingsRepositoryMock : public AnimatorSettingsRepository
{
public:
    MOCK_METHOD(boost::optional<SingleFileAnimatorSettings>, getSingleFileAnimatorSettings,
                (const std::string& animatorName), (const override));
    MOCK_METHOD(boost::optional<MultipleFilesAnimatorSettings>, getMultipleFileAnimatorSettings,
                (const std::string& animatorName), (const override));
};
}
