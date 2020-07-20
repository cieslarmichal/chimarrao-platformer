#pragma once

#include <vector>

#include "InputObservationHandler.h"

namespace game
{
class DefaultInputObservationHandler : public InputObservationHandler
{
public:
    void registerObserver(InputObserver* observer) override;
    void removeObserver(InputObserver* observer) override;
    void notifyObservers(const InputStatus&) override;

private:
    std::vector<InputObserver*> observers;
};
}
