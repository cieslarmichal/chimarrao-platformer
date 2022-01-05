#pragma once

#include <memory>

#include "DialogueTrack.h"

namespace game
{
class DialoguesReader
{
public:
    virtual ~DialoguesReader() = default;

    virtual DialogueTrack read(const std::string& dialogueTrackPath) const = 0;
};
}
