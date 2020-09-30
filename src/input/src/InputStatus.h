#pragma once

#include <ostream>
#include <vector>

#include "InputKey.h"
#include "Vector.h"

namespace input
{
class InputStatus
{
public:
    InputStatus();

    bool isKeyPressed(InputKey) const;
    void setKeyPressed(InputKey);
    bool isKeyReleased(InputKey) const;
    void setReleasedKeys();
    void setMousePosition(const utils::Vector2f&);
    utils::Vector2f getMousePosition() const;
    void clearPressedKeys();

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