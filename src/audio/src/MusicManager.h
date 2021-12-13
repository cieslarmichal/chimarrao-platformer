#pragma once

#include "MusicId.h"
#include <string>

namespace audio
{
class MusicManager
{
public:
    virtual ~MusicManager() = default;

    virtual MusicId acquire(const std::string& musicPath) = 0;
    virtual void release(const MusicId&) = 0;
    virtual void play(const MusicId&) = 0;
    virtual void pause(const MusicId&) = 0;
    virtual void stop(const MusicId&) = 0;
};
}