#pragma once

#include "gmock/gmock.h"

#include "InputObservationHandler.h"

namespace game
{
class InputObservationHandlerMock : public InputObservationHandler
{
public:
    MOCK_METHOD1(registerObserver, void(InputObserver*));
    MOCK_METHOD1(removeObserver, void(InputObserver*));
    MOCK_METHOD1(notifyObservers, void(const InputStatus&));
};
}