#pragma once

#include "WindowObserver.h"

namespace window
{
class WindowObservationHandler
{
public:
    virtual ~WindowObservationHandler() = default;

    virtual void registerObserver(WindowObserver*) = 0;
    virtual void removeObserver(WindowObserver*) = 0;
    virtual void notifyObservers(const utils::Vector2u& windowSize) = 0;
};
}
