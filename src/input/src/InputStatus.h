#pragma once

#include <ostream>
#include <vector>

#include "InputApi.h"
#include "InputKey.h"
#include "Vector.h"

namespace input
{
class InputStatus
{
public:
    INPUT_API InputStatus();

    INPUT_API bool isKeyPressed(InputKey) const;
    INPUT_API void setKeyPressed(InputKey);
    INPUT_API bool isKeyReleased(InputKey) const;
    INPUT_API void setReleasedKeys();
    INPUT_API void setMousePosition(const utils::Vector2f&);
    INPUT_API utils::Vector2f getMousePosition() const;
    INPUT_API void clearPressedKeys();

private:
    const unsigned long numberOfKeys;
    std::vector<bool> pressedKeys;
    std::vector<bool> pressedKeysHistory;
    std::vector<bool> releasedKeys;
    utils::Vector2f mousePosition{};
};

bool operator==(const InputStatus&, const InputStatus&);
std::ostream& operator<<(std::ostream& os, const InputStatus&);
}