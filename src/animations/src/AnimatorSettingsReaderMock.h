#pragma once

#include "gmock/gmock.h"

#include "AnimatorSettingsReader.h"

namespace animations
{
class AnimatorSettingsReaderMock : public AnimatorSettingsReader
{
public:
    MOCK_METHOD(AnimatorsSettings, readAnimatorsSettings, (const utils::FilePath&), (const override));
};
}
