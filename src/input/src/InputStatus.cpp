#include "InputStatus.h"

#include "StlOperators.h"

namespace input
{
bool InputStatus::isKeyPressed(InputKey key) const
{
    return pressedKeysMask.isBitSet(static_cast<unsigned>(key));
}

void InputStatus::setKeyPressed(InputKey key)
{
    pressedKeysMask.setBit(static_cast<unsigned>(key));
}

bool InputStatus::isKeyReleased(InputKey key) const
{
    return releasedKeysMask.isBitSet(static_cast<unsigned>(key));
}

void InputStatus::setKeyReleased(InputKey key)
{
    releasedKeysMask.setBit(static_cast<unsigned>(key));
}

void InputStatus::setMousePosition(const utils::Vector2f& position)
{
    mousePosition = position;
}

utils::Vector2f InputStatus::getMousePosition() const
{
    return mousePosition;
}

void InputStatus::clearStatus()
{
    pressedKeysMask.clear();
}

utils::BitMask InputStatus::getPressedKeysMask() const
{
    return pressedKeysMask;
}

utils::BitMask InputStatus::getReleasedKeysMask() const
{
    return pressedKeysMask;
}

bool operator==(const InputStatus& lhs, const InputStatus& rhs)
{
    return lhs.getPressedKeysMask() == rhs.getPressedKeysMask() &&
           lhs.getReleasedKeysMask() == rhs.getReleasedKeysMask() &&
           lhs.getMousePosition() == rhs.getMousePosition();
}

std::ostream& operator<<(std::ostream& os, const InputStatus& inputStatus)
{
    std::vector<InputKey> pressedKeys;
    std::vector<InputKey> releasedKeys;
    for (const auto& key : allKeys)
    {
        if (inputStatus.isKeyPressed(key))
        {
            pressedKeys.push_back(key);
        }

        if (inputStatus.isKeyReleased(key))
        {
            pressedKeys.push_back(key);
        }
    }

    os << "PressedKeys: " << pressedKeys << std::endl;
    os << "ReleasedKeys: " << releasedKeys << std::endl;
    os << "Mouse position: " << inputStatus.getMousePosition();
    return os;
}

}