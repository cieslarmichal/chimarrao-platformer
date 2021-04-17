#include "InputSfml.h"

#include <tuple>

#include "StlOperators.h"

namespace input
{

InputSfml::InputSfml()
    : numberOfKeys{static_cast<unsigned long>(allKeys.size())},
      pressedKeys(numberOfKeys, false),
      pressedKeysHistory(numberOfKeys, false),
      releasedKeys(numberOfKeys, false)
{
}

bool InputSfml::isKeyPressed(InputKey key) const
{
    return pressedKeys.at(static_cast<unsigned>(key));
}

void InputSfml::setKeyPressed(InputKey key)
{
    pressedKeys[static_cast<unsigned>(key)] = true;
}

bool InputSfml::isKeyReleased(InputKey key) const
{
    return releasedKeys.at(static_cast<unsigned>(key));
}

void InputSfml::setReleasedKeys()
{
    for (unsigned long i = 0; i < numberOfKeys; i++)
    {
        releasedKeys[i] = pressedKeysHistory[i] && not pressedKeys[i];
    }
    pressedKeysHistory = pressedKeys;
}

void InputSfml::setMousePosition(const utils::Vector2f& position)
{
    mousePosition = position;
}

utils::Vector2f InputSfml::getMousePosition() const
{
    return mousePosition;
}

void InputSfml::clearPressedKeys()
{
    for (unsigned long i = 0; i < numberOfKeys; i++)
    {
        pressedKeys[i] = false;
    }
}

namespace
{
std::tuple<std::vector<InputKey>, std::vector<InputKey>> getKeysInformation(const Input& inputStatus)
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

bool operator==(const Input& lhs, const Input& rhs)
{
    return getKeysInformation(lhs) == getKeysInformation(rhs) &&
           lhs.getMousePosition() == rhs.getMousePosition();
}

std::ostream& operator<<(std::ostream& os, const Input& inputStatus)
{
    const auto [pressedKeys, releasedKeys] = getKeysInformation(inputStatus);
    os << "PressedKeys: " << pressedKeys << std::endl;
    os << "ReleasedKeys: " << releasedKeys << std::endl;
    os << "Mouse position: " << inputStatus.getMousePosition();
    return os;
}

}