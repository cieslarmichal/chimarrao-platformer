#pragma once

#include <SFML/Audio.hpp>

#include "MusicPath.h"

namespace audio
{
class MusicStorage
{
public:
    virtual ~MusicStorage() = default;

    virtual sf::Music& getMusic(const MusicPath&) = 0;
};
}