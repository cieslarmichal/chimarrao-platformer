#pragma once

#include <memory>

#include "MusicManager.h"

namespace audio
{
class AudioFactory
{
public:
    virtual ~AudioFactory() = default;

    virtual std::unique_ptr<MusicManager> createMusicManager() const = 0;

    static std::unique_ptr<AudioFactory> createAudioFactory();
};
}
