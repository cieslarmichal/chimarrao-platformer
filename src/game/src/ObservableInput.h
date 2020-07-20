#pragma once

#include "InputObserver.h"
#include "InputStatus.h"

namespace game
{
class ObservableInput
{
public:
    virtual ~ObservableInput() = default;

    virtual void registerObserver(InputObserver*) = 0;
    virtual void removeObserver(InputObserver*) = 0;

protected:
    virtual void notifyObservers(const InputStatus&) = 0;
};
}
