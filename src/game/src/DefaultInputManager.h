#pragma once

#include <memory>

#include "InputManager.h"
#include "ObserverHandler.h"

namespace game
{
class DefaultInputManager : public InputManager
{
public:
    explicit DefaultInputManager(std::unique_ptr<ObserverHandler>);

    void readInput() override;

    void registerObserver(InputObserver*) override;
    void removeObserver(InputObserver*) override;

private:
    void notifyObservers(const KeyboardStatus&) override;

    std::unique_ptr<ObserverHandler> observerHandler;
};
}