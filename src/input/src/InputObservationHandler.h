#pragma once

#include "InputObserver.h"
#include "InputStatus.h"

namespace input
{
class InputObservationHandler
{
public:
    virtual ~InputObservationHandler() = default;

    virtual void registerObserver(InputObserver*) = 0;
    virtual void removeObserver(InputObserver*) = 0;
    virtual void notifyObservers(const InputStatus&) = 0;
};
}
