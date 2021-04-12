#include "DefaultInputManagerFactory.h"

#include "DefaultInputManager.h"
#include "DefaultInputObservationHandler.h"

namespace input
{

std::unique_ptr<InputManager>
DefaultInputManagerFactory::createInputManager(std::shared_ptr<window::Window> window) const
{
    return std::make_unique<DefaultInputManager>(std::make_unique<DefaultInputObservationHandler>(), window);
}
}