#pragma once

#include <ostream>

#include "BitMask.h"
#include "InputKey.h"
#include "Vector.h"

namespace input
{
class InputStatus
{
public:
    bool isKeyPressed(InputKey) const;
    void setKeyPressed(InputKey);
    void setMousePosition(const utils::Vector2f&);
    utils::Vector2f getMousePosition() const;
    utils::BitMask getMaskStatus() const;
    void clearStatus();

private:
    utils::BitMask inputKeyMask{};
    utils::Vector2f mousePosition{};
};

bool operator==(const InputStatus&, const InputStatus&);
std::ostream& operator<<(std::ostream& os, const InputStatus&);
}