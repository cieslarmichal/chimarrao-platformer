#pragma once

#include "WindowObserver.h"

namespace gui
{
class ObservableWindow
{
public:
    virtual ~ObservableWindow() = default;

    virtual void registerObserver(WindowObserver*) = 0;
    virtual void removeObserver(WindowObserver*) = 0;

protected:
    virtual void notifyObservers() = 0;
};
}