#pragma once

#include "Vector.h"

namespace gui
{
class WindowObserver
{
public:
    virtual ~WindowObserver() = default;

    virtual void windowSizeChanged(const utils::Vector2u& windowSize) = 0;
};
}