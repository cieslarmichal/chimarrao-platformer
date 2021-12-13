#pragma once

#include "gmock/gmock.h"

#include "MusicManager.h"

namespace audio
{
class MusicManagerMock : public MusicManager
{
public:
    MOCK_METHOD(MusicId, acquire, (const MusicPath& musicPath), (override));
    MOCK_METHOD(void, release, (const MusicId&), (override));
    MOCK_METHOD(void, play, (const MusicId&), (override));
    MOCK_METHOD(void, pause, (const MusicId&), (override));
    MOCK_METHOD(void, stop, (const MusicId&), (override));
};
}
