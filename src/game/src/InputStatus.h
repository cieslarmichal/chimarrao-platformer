#pragma once

#include "BitMask.h"
#include "InputKey.h"

namespace game
{
class InputStatus
{
public:
    bool isKeyPressed(InputKey) const;
    void setKeyPressed(InputKey);
    utils::BitMask getMaskStatus() const;
    void clearStatus();

private:
    utils::BitMask inputKeyMask;
};

bool operator==(const InputStatus&, const InputStatus&);
}