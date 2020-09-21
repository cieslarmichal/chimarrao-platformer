#pragma once

#include <memory>

#include "InputManager.h"
#include "InputObservationHandler.h"

namespace input
{
class DefaultInputManager : public InputManager
{
public:
    explicit DefaultInputManager(std::unique_ptr<InputObservationHandler>);

    void readInput() override;

    void registerObserver(InputObserver*) override;
    void removeObserver(InputObserver*) override;

private:
    void notifyObservers() override;

    std::unique_ptr<InputObservationHandler> observerHandler;
    InputStatus inputStatus;
};
}