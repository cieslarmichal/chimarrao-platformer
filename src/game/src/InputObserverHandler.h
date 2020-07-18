#pragma once

#include "ObserverHandler.h"
#include <vector>

namespace game
{
class InputObserverHandler : public ObserverHandler
{
public:
    void registerObserver(InputObserver* observer) override;
    void removeObserver(InputObserver* observer) override;
    void notifyObservers(const KeyboardStatus&) override;

private:
    std::vector<InputObserver*> observers;
};
}
