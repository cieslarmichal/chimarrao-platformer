#include "SfmlMusicStorage.h"

#include <iostream>

#include "MusicLoader.h"
#include "exceptions/CannotAccessMusicFile.h"
#include "exceptions/MusicNotAvailable.h"

namespace audio
{

sf::Music& SfmlMusicStorage::getMusic(const MusicPath& path)
{
    if (not musicInStorage(path))
    {
        loadMusic(path);
    }
    return *musics.at(path);
}

void SfmlMusicStorage::loadMusic(const MusicPath& path)
{
    auto music = std::make_unique<sf::Music>();
    try
    {
        MusicLoader::load(*music, path);
    }
    catch (const exceptions::CannotAccessMusicFile& e)
    {
        std::cerr << e.what() << std::endl;
        throw exceptions::MusicNotAvailable{e.what()};
    }
    musics[path] = std::move(music);
}

bool SfmlMusicStorage::musicInStorage(const MusicPath& path)
{
    return musics.contains(path);
}

}