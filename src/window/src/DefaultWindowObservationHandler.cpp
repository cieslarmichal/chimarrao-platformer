#include "DefaultWindowObservationHandler.h"

#include <algorithm>

namespace window
{

void DefaultWindowObservationHandler::registerObserver(WindowObserver* observer)
{
    observers.push_back(observer);
}

void DefaultWindowObservationHandler::removeObserver(WindowObserver* observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void DefaultWindowObservationHandler::notifyObservers(const utils::Vector2u& windowSize)
{
    for (const auto& observer : observers)
    {
        if (observer)
        {
            observer->handleWindowSizeChange(windowSize);
        }
    }
}

}