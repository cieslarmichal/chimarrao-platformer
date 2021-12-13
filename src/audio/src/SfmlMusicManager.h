#pragma once

#include "MusicManager.h"
#include "MusicStorage.h"
#include <memory>
#include <boost/functional/hash.hpp>

namespace audio
{
class SfmlMusicManager : public MusicManager
{
public:
    explicit SfmlMusicManager(std::unique_ptr<MusicStorage>);

    MusicId acquire(const MusicPath&) override;
    void release(const MusicId&) override;
    void play(const MusicId&) override;
    void pause(const MusicId&)  override;
    void stop(const MusicId&) override;

private:
    void throwIfMusicWithGivenIdNotFound(const MusicId&) const;

    std::unique_ptr<MusicStorage> musicStorage;
    std::unordered_map<MusicId, MusicPath, boost::hash<MusicId>> musicIdsToPaths;
};
}