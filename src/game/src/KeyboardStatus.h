#pragma once

#include "InputKey.h"
#include "KeyState.h"
#include <map>

namespace game
{
class KeyboardStatus
{
public:
    explicit KeyboardStatus();

    std::map<InputKey, KeyState> getKeyStates() const;
    bool isUpPressed() const;
    bool isDownPressed() const;
    bool isLeftPressed() const;
    bool isRightPressed() const;
    bool isSpacePressed() const;
    bool isShiftPressed() const;
    bool isEnterPressed() const;
    bool isEscapePressed() const;
    void setUpPressed();
    void setDownPressed();
    void setLeftPressed();
    void setRightPressed();
    void setSpacePressed();
    void setShiftPressed();
    void setEnterPressed();
    void setEscapePressed();


private:
    bool isPressed(InputKey) const;
    std::map<InputKey, KeyState> keyStates;
};

inline bool operator==(const KeyboardStatus& lhs, const KeyboardStatus& rhs)
{
    return lhs.getKeyStates() == rhs.getKeyStates();
}

}