#include "InputStatus.h"

#include <tuple>

#include "InputKey.h"
#include "StlOperators.h"

namespace input
{

InputStatus::InputStatus()
    : numberOfKeys{allKeys.size()},
      pressedKeys(numberOfKeys, false),
      pressedKeysHistory(numberOfKeys, false),
      releasedKeys(numberOfKeys, false)
{
}

bool InputStatus::isKeyPressed(InputKey key) const
{
    return pressedKeys.at(static_cast<unsigned>(key));
}

void InputStatus::setKeyPressed(InputKey key)
{
    pressedKeys[static_cast<unsigned>(key)] = true;
}

bool InputStatus::isKeyReleased(InputKey key) const
{
    return releasedKeys.at(static_cast<unsigned>(key));
}

void InputStatus::setReleasedKeys()
{
    for (unsigned long i = 0; i < numberOfKeys; i++)
    {
        releasedKeys[i] = pressedKeysHistory[i] && not pressedKeys[i];
    }
    pressedKeysHistory = pressedKeys;
}

void InputStatus::setMousePosition(const utils::Vector2f& position)
{
    mousePosition = position;
}

utils::Vector2f InputStatus::getMousePosition() const
{
    return mousePosition;
}

void InputStatus::clearPressedKeys()
{
    for (unsigned long i = 0; i < numberOfKeys; i++)
    {
        pressedKeys[i] = false;
    }
}

namespace
{
std::tuple<std::vector<InputKey>, std::vector<InputKey>> getKeysInformation(const InputStatus& inputStatus)
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
    return {pressedKeys, releasedKeys};
}
}

bool operator==(const InputStatus& lhs, const InputStatus& rhs)
{
    return getKeysInformation(lhs) == getKeysInformation(rhs) &&
           lhs.getMousePosition() == rhs.getMousePosition();
}

std::ostream& operator<<(std::ostream& os, const InputStatus& inputStatus)
{
    const auto [pressedKeys, releasedKeys] = getKeysInformation(inputStatus);
    os << "PressedKeys: " << pressedKeys << std::endl;
    os << "ReleasedKeys: " << releasedKeys << std::endl;
    os << "Mouse position: " << inputStatus.getMousePosition();
    return os;
}

}