#pragma once

#include "InputObserver.h"
#include "KeyboardStatus.h"

namespace game
{
class ObservableInput
{
public:
    virtual ~ObservableInput() = default;

    virtual void registerObserver(InputObserver*) = 0;
    virtual void removeObserver(InputObserver*) = 0;

protected:
    virtual void notifyObservers(const KeyboardStatus&) = 0;
};
}
