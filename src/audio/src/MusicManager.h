#pragma once

#include <string>

#include "MusicId.h"
#include "MusicPath.h"

namespace audio
{
class MusicManager
{
public:
    virtual ~MusicManager() = default;

    virtual MusicId acquire(const MusicPath&) = 0;
    virtual void release(const MusicId&) = 0;
    virtual void play(const MusicId&) = 0;
    virtual void pause(const MusicId&) = 0;
    virtual void stop(const MusicId&) = 0;
};
}