#pragma once

#include <ostream>
#include <vector>

#include "Input.h"
#include "Vector.h"

namespace input
{
class InputSfml : public Input
{
public:
    InputSfml();

    bool isKeyPressed(InputKey) const override;
    void setKeyPressed(InputKey) override;
    bool isKeyReleased(InputKey) const override;
    void setReleasedKeys() override;
    void setMouseRelativePosition(const utils::Vector2f&) override;
    void setMouseAbsolutePosition(const utils::Vector2f&) override;
    utils::Vector2f getMouseRelativePosition() const override;
    utils::Vector2f getMouseAbsolutePosition() const override;
    void clearPressedKeys() override;

private:
    const unsigned long numberOfKeys;
    std::vector<bool> pressedKeys;
    std::vector<bool> pressedKeysHistory;
    std::vector<bool> releasedKeys;
    utils::Vector2f mouseRelativePosition{};
    utils::Vector2f mouseAbsolutePosition{};
};

bool operator==(const InputSfml&, const Input&);
std::ostream& operator<<(std::ostream& os, const InputSfml&);
}