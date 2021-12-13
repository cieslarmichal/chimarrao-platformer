#pragma once

#include "SFML/Audio.hpp"

#include "MusicPath.h"

namespace audio
{
class MusicLoader
{
public:
    static void load(sf::Music&, const MusicPath&);
};
}