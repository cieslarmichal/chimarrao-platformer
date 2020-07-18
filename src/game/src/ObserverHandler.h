#pragma once

#include "InputObserver.h"
#include "KeyboardStatus.h"

namespace game
{
class ObserverHandler
{
public:
    virtual ~ObserverHandler() = default;

    virtual void registerObserver(InputObserver*) = 0;
    virtual void removeObserver(InputObserver*) = 0;
    virtual void notifyObservers(const KeyboardStatus&) = 0;
};
}
