#pragma once

#include "gmock/gmock.h"

#include "AnimatorSettingsReader.h"

namespace graphics::animation
{
class AnimatorSettingsReaderMock : public AnimatorSettingsReader
{
public:
    MOCK_METHOD(AnimatorsSettings, readAnimatorsSettings, (const utils::FilePath&), (const override));
};
}
