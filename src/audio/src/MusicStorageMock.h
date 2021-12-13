#pragma once

#include "gmock/gmock.h"

#include "MusicStorage.h"

namespace audio
{
class MusicStorageMock : public MusicStorage
{
public:
    MOCK_METHOD(const sf::Music&, getMusic, (const MusicPath&));
};
}