#pragma once

#include <memory>

#include "InputManager.h"
#include "InputObservationHandler.h"

namespace game
{
class DefaultInputManager : public InputManager
{
public:
    explicit DefaultInputManager(std::unique_ptr<InputObservationHandler>);

    void readInput() override;

    void registerObserver(InputObserver*) override;
    void removeObserver(InputObserver*) override;

private:
    void notifyObservers(const InputStatus&) override;

    std::unique_ptr<InputObservationHandler> observerHandler;
    InputStatus inputKeysStatus;
};
}