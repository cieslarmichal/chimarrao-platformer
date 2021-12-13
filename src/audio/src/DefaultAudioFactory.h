#pragma once

#include "AudioFactory.h"

namespace audio
{
class DefaultAudioFactory : public AudioFactory
{
public:
    std::unique_ptr<MusicManager> createMusicManager() const override;
};
}
