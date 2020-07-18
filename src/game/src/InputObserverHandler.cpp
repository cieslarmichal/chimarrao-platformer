#include <algorithm>

#include "InputObserverHandler.h"

namespace game
{
void InputObserverHandler::registerObserver(InputObserver* observer)
{
    observers.push_back(observer);
}

void InputObserverHandler::removeObserver(InputObserver* observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void InputObserverHandler::notifyObservers(const KeyboardStatus& keyboardStatus)
{
    for (const auto& observer : observers)
    {
        if (observer)
        {
            observer->update(keyboardStatus);
        }
    }
}
}