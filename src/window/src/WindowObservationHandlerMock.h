#pragma once

#include "gmock/gmock.h"

#include "WindowObservationHandler.h"

namespace window
{
class WindowObservationHandlerMock : public WindowObservationHandler
{
public:
    MOCK_METHOD1(registerObserver, void(WindowObserver*));
    MOCK_METHOD1(removeObserver, void(WindowObserver*));
    MOCK_METHOD1(notifyObservers, void(const utils::Vector2u& windowSize));
};
}