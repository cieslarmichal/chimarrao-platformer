#pragma once

#include "gmock/gmock.h"

#include "AnimatorSettingsReader.h"

namespace graphics::animation
{
class AnimatorSettingsReaderMock : public AnimatorSettingsReader
{
public:
    MOCK_METHOD(boost::optional<AnimatorsSettings>, readAnimatorsSettings, (const utils::FilePath&),
                (const override));
};
}