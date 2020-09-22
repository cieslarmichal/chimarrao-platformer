#include "DefaultInputObservationHandler.h"

#include <algorithm>

namespace input
{
void DefaultInputObservationHandler::registerObserver(InputObserver* observer)
{
    observers.push_back(observer);
}

void DefaultInputObservationHandler::removeObserver(InputObserver* observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void DefaultInputObservationHandler::notifyObservers(const InputStatus& inputStatus)
{
    for (const auto& observer : observers)
    {
        if (observer)
        {
            observer->handleInputStatus(inputStatus);
        }
    }
}
}