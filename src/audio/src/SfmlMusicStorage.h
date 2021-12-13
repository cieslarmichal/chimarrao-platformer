#pragma once

#include <unordered_map>

#include "MusicStorage.h"

namespace audio
{
class SfmlMusicStorage : public MusicStorage
{
public:
    const sf::Music& getMusic(const MusicPath&) override;

private:
    void loadMusic(const MusicPath&);
    bool musicInStorage(const MusicPath&);

    std::unordered_map<MusicPath, std::unique_ptr<sf::Music>> musics;
};
}