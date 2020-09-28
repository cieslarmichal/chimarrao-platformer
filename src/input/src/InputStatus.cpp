#include "InputStatus.h"

#include "StlOperators.h"

namespace input
{
bool InputStatus::isKeyPressed(InputKey key) const
{
    return inputKeyMask.isBitSet(static_cast<unsigned>(key));
}

void InputStatus::setKeyPressed(InputKey key)
{
    inputKeyMask.setBit(static_cast<unsigned>(key));
}

void InputStatus::setMousePosition(const utils::Vector2f& position)
{
    mousePosition = position;
}

utils::Vector2f InputStatus::getMousePosition() const
{
    return mousePosition;
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

std::ostream& operator<<(std::ostream& os, const InputStatus& inputStatus)
{
    std::vector<InputKey> pressedKeys;
    for (const auto& key : allKeys)
    {
        if (inputStatus.isKeyPressed(key))
        {
            pressedKeys.push_back(key);
        }
    }

    os << "PressedKeys: " << pressedKeys << std::endl;
    os << "Mouse position: " << inputStatus.getMousePosition();
    return os;
}

}