#pragma once

#include "gmock/gmock.h"

#include "ObserverHandler.h"

namespace game
{
class ObserverHandlerMock : public ObserverHandler
{
public:
    MOCK_METHOD1(registerObserver, void(InputObserver*));
    MOCK_METHOD1(removeObserver, void(InputObserver*));
    MOCK_METHOD1(notifyObservers, void(const KeyboardStatus&));
};
}