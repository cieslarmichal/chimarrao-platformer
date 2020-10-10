#pragma once

#include "Vector.h"

namespace window
{
class WindowObserver
{
public:
    virtual ~WindowObserver() = default;

    virtual void handleWindowSizeChange(const utils::Vector2u& windowSize) = 0;
};
}