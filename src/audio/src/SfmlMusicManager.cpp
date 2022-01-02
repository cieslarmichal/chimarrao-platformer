#include "SfmlMusicManager.h"

#include "MusicIdGenerator.h"
#include "exceptions/InvalidVolume.h"
#include "exceptions/MusicNotFound.h"

namespace audio
{
SfmlMusicManager::SfmlMusicManager(std::unique_ptr<MusicStorage> storage) : musicStorage{std::move(storage)}
{
}

MusicId SfmlMusicManager::acquire(const MusicPath& musicPath)
{
    const auto musicId = MusicIdGenerator::generateId();
    musicIdsToPaths[musicId] = musicPath;
    return musicId;
}

void SfmlMusicManager::release(const MusicId& musicId)
{
    throwIfMusicWithGivenIdNotFound(musicId);

    musicIdsToPaths.erase(musicId);
}

void SfmlMusicManager::play(const MusicId& musicId)
{
    throwIfMusicWithGivenIdNotFound(musicId);

    const auto musicPath = musicIdsToPaths.at(musicId);
    auto& music = musicStorage->getMusic(musicPath);

    music.setLoop(true);
    music.play();
}

void SfmlMusicManager::setVolume(const MusicId& musicId, int volumePercents)
{
    if (volumePercents < 0 or volumePercents > 100)
    {
        throw exceptions::InvalidVolume{"Volume should be set between 0 and 100 percents"};
    }

    throwIfMusicWithGivenIdNotFound(musicId);

    const auto musicPath = musicIdsToPaths.at(musicId);
    auto& music = musicStorage->getMusic(musicPath);

    music.setVolume(static_cast<float>(volumePercents));
}

void SfmlMusicManager::pause(const MusicId& musicId)
{
    throwIfMusicWithGivenIdNotFound(musicId);

    const auto musicPath = musicIdsToPaths.at(musicId);
    auto& music = musicStorage->getMusic(musicPath);

    music.pause();
}

void SfmlMusicManager::stop(const MusicId& musicId)
{
    throwIfMusicWithGivenIdNotFound(musicId);

    const auto musicPath = musicIdsToPaths.at(musicId);
    auto& music = musicStorage->getMusic(musicPath);

    music.stop();
}

void SfmlMusicManager::throwIfMusicWithGivenIdNotFound(const MusicId& musicId) const
{
    if (not musicIdsToPaths.contains(musicId))
    {
        throw exceptions::MusicNotFound{"music with given id not found"};
    }
}

}