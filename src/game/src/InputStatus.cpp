#include "InputStatus.h"

namespace game
{

bool InputStatus::isKeyPressed(InputKey key) const
{
    return inputKeyMask.isBitSet(static_cast<unsigned>(key));
}

void InputStatus::setKeyPressed(InputKey key)
{
    inputKeyMask.setBit(static_cast<unsigned>(key));
}

utils::BitMask InputStatus::getMaskStatus() const
{
    return inputKeyMask;
}

void InputStatus::clearStatus()
{
    inputKeyMask.clear();
}

bool operator==(const InputStatus& lhs, const InputStatus& rhs)
{
    return lhs.getMaskStatus() == rhs.getMaskStatus();
}

}