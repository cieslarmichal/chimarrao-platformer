#pragma once

#include <vector>

#include "WindowObservationHandler.h"

namespace window
{
class DefaultWindowObservationHandler : public WindowObservationHandler
{
public:
    void registerObserver(WindowObserver* observer) override;
    void removeObserver(WindowObserver* observer) override;
    void notifyObservers(const utils::Vector2u& windowSize) override;

private:
    std::vector<WindowObserver*> observers;
};
}
