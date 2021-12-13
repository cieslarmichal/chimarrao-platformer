#pragma once

#include "gmock/gmock.h"

#include "MusicStorage.h"

namespace audio
{
class MusicStorageMock : public MusicStorage
{
public:
    MOCK_METHOD(sf::Music&, getMusic, (const MusicPath&));
};
}