#include "MusicLoader.h"

#include "exceptions/CannotAccessMusicFile.h"

namespace audio
{

void MusicLoader::load(sf::Music& music, const MusicPath& musicPath)
{
    if (not music.openFromFile(musicPath))
    {
        throw exceptions::CannotAccessMusicFile("Cannot load music file: " + musicPath);
    }
}
}