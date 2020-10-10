#pragma once

#include <memory>

#include "InputManager.h"
#include "InputObservationHandler.h"
#include "Window.h"

namespace input
{
class DefaultInputManager : public InputManager
{
public:
    explicit DefaultInputManager(std::unique_ptr<InputObservationHandler>, std::shared_ptr<window::Window>);

    void readInput() override;
    void registerObserver(InputObserver*) override;
    void removeObserver(InputObserver*) override;

private:
    void notifyObservers() override;

    std::unique_ptr<InputObservationHandler> observationHandler;
    std::shared_ptr<window::Window> window;
    InputStatus inputStatus;
};
}