#include "SfmlMusicManager.h"

namespace audio
{

MusicId SfmlMusicManager::acquire(const MusicPath& musicPath)
{
    return audio::MusicId();
}
void SfmlMusicManager::release(const MusicId&) {}
void SfmlMusicManager::play(const MusicId&) {}
void SfmlMusicManager::pause(const MusicId&) {}
void SfmlMusicManager::stop(const MusicId&) {}
}