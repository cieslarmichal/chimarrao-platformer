#pragma once

#include <boost/functional/hash.hpp>
#include <memory>

#include "MusicManager.h"
#include "MusicStorage.h"

namespace audio
{
class SfmlMusicManager : public MusicManager
{
public:
    explicit SfmlMusicManager(std::unique_ptr<MusicStorage>);

    MusicId acquire(const MusicPath&) override;
    void release(const MusicId&) override;
    void play(const MusicId&) override;
    void pause(const MusicId&) override;
    void stop(const MusicId&) override;

private:
    void throwIfMusicWithGivenIdNotFound(const MusicId&) const;

    std::unique_ptr<MusicStorage> musicStorage;
    std::unordered_map<MusicId, MusicPath, boost::hash<MusicId>> musicIdsToPaths;
};
}