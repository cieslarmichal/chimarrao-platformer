#pragma once

#include "MusicManager.h"

namespace audio
{
class SfmlMusicManager : public MusicManager
{
public:
    MusicId acquire(const MusicPath&) override;
    void release(const MusicId&) override;
    void play(const MusicId&) override;
    void pause(const MusicId&)  override;
    void stop(const MusicId&) override;
};
}